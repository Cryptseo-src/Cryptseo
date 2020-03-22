#include "cryptseoupdater.h"
#include "cryptseoupdater_p.h"

#include <QtCore/QCoreApplication>
#include <QtCore/QFileInfo>
#include <QtCore/QDir>
#include <QtCore/QXmlStreamReader>
#include <QtCore/QTimer>

#define REPOSITORY_URL "https://github.com/Cryptseo/Cryptseo/releases.atom"

std::atomic<bool> isUpdaterRunning(false);

using namespace QtCryptseoUpdater;

Q_LOGGING_CATEGORY(logCryptseoUpdater, "CryptseoUpdater")

CryptseoUpdaterPrivate::CryptseoUpdaterPrivate(CryptseoUpdater *q_ptr) :
	QObject(nullptr),
	q(q_ptr),
	currentVersion(),
	updateInfos(),
	normalExit(true),
	lastErrorCode(0),
	lastErrorLog(),
	atomFeeder(nullptr),
	currentVersionPos(-1),
	downloadManager(nullptr),
	scheduler(new SimpleScheduler(this)),
	runOnExit(false),
	runArguments(),
	adminAuth(nullptr)
{
	isUpdaterRunning = false;
	connect(qApp, &QCoreApplication::aboutToQuit,
			this, &CryptseoUpdaterPrivate::onAppAboutToExit,
			Qt::DirectConnection);
	connect(scheduler, &SimpleScheduler::scheduleTriggered,
			this, &CryptseoUpdaterPrivate::startUpdateCheck);
}

CryptseoUpdaterPrivate::~CryptseoUpdaterPrivate()
{
	if (runOnExit)
		qCWarning(logCryptseoUpdater) << "CryptseoUpdater destroyed with run on exit active before the application quit";

	if (atomFeeder) {
		delete atomFeeder;
		atomFeeder = nullptr;
	}

	if (downloadManager) {
		delete downloadManager;
		downloadManager = nullptr;
	}
}

bool CryptseoUpdaterPrivate::startUpdateCheck()
{
	if (isUpdaterRunning) {
		return false;
	}

	updateInfos.clear();
	normalExit = true;
	lastErrorCode = 0;
	lastErrorLog.clear();
	currentVersionPos = -1;

	atomFeeder = new AtomFeeder(REPOSITORY_URL);

	connect(atomFeeder, &AtomFeeder::getVersionListDone,
			this, &CryptseoUpdaterPrivate::onUpdaterReady);

	atomFeeder->start();

	isUpdaterRunning = true;

	emit q->updateInfoChanged(updateInfos);
	emit q->runningChanged(true);
	return true;
}

void CryptseoUpdaterPrivate::stopUpdateCheck(int delay, bool async)
{
	if (atomFeeder) {
		if (delay > 0) {
			atomFeeder->stop();
			if (async){
				QTimer::singleShot(delay, this, [this](){
					stopUpdateCheck(0, false);
				});
			} else {
				atomFeeder->stop();
			}
		} else {
			atomFeeder->stop();
		}
	}
}

QString CryptseoUpdaterPrivate::getDownloadUrl(QString version)
{
#if defined(Q_OS_WIN32)
	//QString fileName = "cryptseo-qt-win32.zip";
	QString fileName = "cryptseo-qt-win.zip";
#elif defined(Q_OS_WIN)
	//QString fileName = "cryptseo-qt-win64.zip";
	QString fileName = "cryptseo-qt-win.zip";
#elif defined(Q_OS_OSX)
	QString fileName = "cryptseo-qt-mac.dmg";
#else
	QString fileName = 1 ? "cryptseo-qt-linux-16.zip" : "cryptseo-qt-linux-18.zip";
#endif
	return "https://github.com/Cryptseo/Cryptseo/releases/download/" + version + "/" + fileName;
}

void CryptseoUpdaterPrivate::onDownloadProgress(DownloadManager::DownloadProgress progress)
{

}

void CryptseoUpdaterPrivate::onDownloadFinished(DownloadManager::DownloadProgress progress)
{

}

void CryptseoUpdaterPrivate::onDownloadCheckSize(DownloadManager::DownloadProgress progress)
{
	if (progress.totalSize > 0) {
		CryptseoUpdater::CryptseoUpdateInfo updateInfo("Cryptseo wallet", newVersion, progress.totalSize);
		updateInfos.append(updateInfo);

		if (downloadManager) {
			downloadManager->deleteLater();
			downloadManager = nullptr;
		}
		if (atomFeeder) {
			atomFeeder->deleteLater();
			atomFeeder = nullptr;
		}
		emit q->runningChanged(false);
		if (!updateInfos.isEmpty())
			emit q->updateInfoChanged(updateInfos);
		emit q->checkUpdatesDone(!updateInfos.isEmpty(), false);

		isUpdaterRunning = false;
	} else {
		onUpdaterReady();
	}
}

void CryptseoUpdaterPrivate::onUpdaterReady()
{
	if (atomFeeder) {
		normalExit = true;
		lastErrorCode = 0;
		lastErrorLog = "";
		currentVersionPos++;
		if (currentVersionPos < atomFeeder->getVersionListCount()) {
			QString version = atomFeeder->getVersion(currentVersionPos);
			//if (version != currentVersion)
			{
				if (downloadManager == nullptr) {
					downloadManager = new DownloadManager(this);
					connect(downloadManager, &DownloadManager::downloadFinished,
							this, &CryptseoUpdaterPrivate::onDownloadCheckSize);
				}
				newVersion = version;
				downloadManager->append(getDownloadUrl(version), true);
				return;
			}
		}
		if (downloadManager) {
			downloadManager->deleteLater();
			downloadManager = nullptr;
		}
		atomFeeder->deleteLater();
		atomFeeder = nullptr;
		emit q->runningChanged(false);
		if (!updateInfos.isEmpty())
			emit q->updateInfoChanged(updateInfos);
		emit q->checkUpdatesDone(!updateInfos.isEmpty(), false);

		isUpdaterRunning = false;
	}
}

void CryptseoUpdaterPrivate::updaterError()
{
	if (atomFeeder) {
		normalExit = false;
		lastErrorCode = -1;
		lastErrorLog = "Error!";
		atomFeeder->deleteLater();
		atomFeeder = nullptr;

		isUpdaterRunning = false;
		emit q->runningChanged(false);
		emit q->checkUpdatesDone(false, true);
	}
}

void CryptseoUpdaterPrivate::onAppAboutToExit()
{
	if (runOnExit) {
		QFileInfo appInfo(QCoreApplication::applicationFilePath());
		auto ok = false;
		if (adminAuth && !adminAuth->hasAdminRights())
			ok = adminAuth->executeAsAdmin(appInfo.absoluteFilePath(), runArguments);
		else {
			ok = QProcess::startDetached(appInfo.absoluteFilePath(),
										 runArguments,
										 appInfo.absolutePath());
		}

		if (!ok) {
			qCWarning(logCryptseoUpdater) << "Unable to start" << appInfo.absoluteFilePath()
										<< "with arguments" << runArguments
										<< "as" << (adminAuth ? "admin/root" : "current user");
		}

		runOnExit = false;//prevent warning
	}
}
