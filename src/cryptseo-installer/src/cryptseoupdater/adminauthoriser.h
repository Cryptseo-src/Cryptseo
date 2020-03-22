#ifndef CSEOUPDATER_ADMINAUTHORISER_H
#define CSEOUPDATER_ADMINAUTHORISER_H

#include <QtCore/qstring.h>
#include <QtCore/qstringlist.h>

namespace QtCryptseoUpdater
{

//! An interface to run programs with elevated rights
class AdminAuthoriser
{
public:
	virtual inline ~AdminAuthoriser() {}
	//! Tests whether this program already has elevated rights or not
	virtual bool hasAdminRights() = 0;
	//! Runs a program with the given arguments with elevated rights
	virtual bool executeAsAdmin(const QString &program, const QStringList &arguments) = 0;
};

}

#endif // CSEOUPDATER_ADMINAUTHORISER_H
