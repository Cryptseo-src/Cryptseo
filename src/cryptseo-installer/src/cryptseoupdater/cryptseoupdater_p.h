#ifndef CSEOUPDATER_CSEOUPDATER_P_H
#define CSEOUPDATER_CSEOUPDATER_P_H

#include "cryptseoupdater.h"
#include "simplescheduler_p.h"
#include "atomfeeder.h"
#include "downloadmanager.h"

#include <QtCore/QProcess>
#include <QtCore/QLoggingCategory>
#include <QtCore/QException>

#include <exception>

namespace QtCryptseoUpdater
{

class CryptseoUpdaterPrivate : public QObject
{
public:
	CryptseoUpdater *q;

	QString currentVersion;
	QList<CryptseoUpdater::CryptseoUpdateInfo> updateInfos;
	bool normalExit;
	int lastErrorCode;
	QByteArray lastErrorLog;

	AtomFeeder *atomFeeder;
	int currentVersionPos;
	QString newVersion;
	DownloadManager *downloadManager;

	SimpleScheduler *scheduler;

	bool runOnExit;
	QStringList runArguments;
	QScopedPointer<AdminAuthoriser> adminAuth;

	CryptseoUpdaterPrivate(CryptseoUpdater *q_ptr);
	~CryptseoUpdaterPrivate();

	bool startUpdateCheck();
	void stopUpdateCheck(int delay, bool async);

	void updaterError();

public Q_SLOTS:
	void onAppAboutToExit();
	void onUpdaterReady();
	void onDownloadProgress(DownloadManager::DownloadProgress progress);
	void onDownloadFinished(DownloadManager::DownloadProgress progress);
	void onDownloadCheckSize(DownloadManager::DownloadProgress progress);

private:
	QString getDownloadUrl(QString version);
};

}

Q_DECLARE_LOGGING_CATEGORY(logCryptseoUpdater)

#endif // CSEOUPDATER_CSEOUPDATER_P_H
