#include "cryptseoupdater.h"
#include "cryptseoupdater_p.h"
#include "../updatecontroller_p.h"

#include <QtCore/QDebug>
#include <QtCore/QList>
#include <QtCore/QMap>

using namespace QtCryptseoUpdater;

const QStringList CryptseoUpdater::NormalUpdateArguments = {QStringLiteral("--updater")};
const QStringList CryptseoUpdater::PassiveUpdateArguments = {QStringLiteral("--updater"), QStringLiteral("skipPrompt=true")};
const QStringList CryptseoUpdater::HiddenUpdateArguments = {QStringLiteral("--silentUpdate")};

CryptseoUpdater::CryptseoUpdater(QObject *parent) :
	CryptseoUpdater("", parent)
{}

CryptseoUpdater::CryptseoUpdater(const QString &currentVersion, QObject *parent) :
	QObject(parent),
	d(new CryptseoUpdaterPrivate(this))
{
	d->currentVersion = currentVersion;
}

CryptseoUpdater::~CryptseoUpdater() {}

bool CryptseoUpdater::exitedNormally() const
{
	return d->normalExit;
}

int CryptseoUpdater::errorCode() const
{
	return d->lastErrorCode;
}

QByteArray CryptseoUpdater::errorLog() const
{
	return d->lastErrorLog;
}

bool CryptseoUpdater::willRunOnExit() const
{
	return d->runOnExit;
}

QString CryptseoUpdater::currentVersion() const
{
	return d->currentVersion;
}

bool CryptseoUpdater::isRunning() const
{
	return isUpdaterRunning;
}

QList<CryptseoUpdater::CryptseoUpdateInfo> CryptseoUpdater::updateInfo() const
{
	return d->updateInfos;
}

bool CryptseoUpdater::checkForUpdates()
{
	return d->startUpdateCheck();
}

void CryptseoUpdater::abortUpdateCheck(int maxDelay, bool async)
{
	d->stopUpdateCheck(maxDelay, async);
}

int CryptseoUpdater::scheduleUpdate(int delaySeconds, bool repeated)
{
	if((((qint64)delaySeconds) * 1000ll) > (qint64)INT_MAX) {
		qCWarning(logCryptseoUpdater) << "delaySeconds to big to be converted to msecs";
		return 0;
	}
	return d->scheduler->startSchedule(delaySeconds * 1000, repeated);
}

int CryptseoUpdater::scheduleUpdate(const QDateTime &when)
{
	return d->scheduler->startSchedule(when);
}

void CryptseoUpdater::cancelScheduledUpdate(int taskId)
{
	d->scheduler->cancelSchedule(taskId);
}

void CryptseoUpdater::runUpdaterOnExit(AdminAuthoriser *authoriser)
{
	runUpdaterOnExit(NormalUpdateArguments, authoriser);
}

void CryptseoUpdater::runUpdaterOnExit(const QStringList &arguments, AdminAuthoriser *authoriser)
{
	d->runOnExit = true;
	d->runArguments = arguments;
	d->adminAuth.reset(authoriser);
}

void CryptseoUpdater::cancelExitRun()
{
	d->runOnExit = false;
	d->adminAuth.reset();
}



CryptseoUpdater::CryptseoUpdateInfo::CryptseoUpdateInfo() :
	name(),
	version(),
	size(0ull)
{}

CryptseoUpdater::CryptseoUpdateInfo::CryptseoUpdateInfo(const CryptseoUpdater::CryptseoUpdateInfo &other) :
	name(other.name),
	version(other.version),
	size(other.size)
{}

CryptseoUpdater::CryptseoUpdateInfo::CryptseoUpdateInfo(QString name, QString version, quint64 size) :
	name(name),
	version(version),
	size(size)
{}

QDebug &operator<<(QDebug &debug, const CryptseoUpdater::CryptseoUpdateInfo &info)
{
	QDebugStateSaver state(debug);
	Q_UNUSED(state);

	debug.noquote() << QStringLiteral("{Name: \"%1\"; Version: %2; Size: %3}")
					   .arg(info.name)
					   .arg(info.version.toUtf8().constData())
					   .arg(info.size);
	return debug;
}
