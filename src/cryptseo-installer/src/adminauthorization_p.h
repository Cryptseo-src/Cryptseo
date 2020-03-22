#ifndef QTCSEOUPDATER_ADMINAUTHORIZATION_P_H
#define QTCSEOUPDATER_ADMINAUTHORIZATION_P_H

#include "cryptseoupdater/adminauthoriser.h"

namespace QtCryptseoUpdater
{

class AdminAuthorization : public AdminAuthoriser
{
public:
	bool hasAdminRights() override;
	bool executeAsAdmin(const QString &program, const QStringList &arguments) override;
};

}

#endif // QTCSEOUPDATER_ADMINAUTHORIZATION_P_H
