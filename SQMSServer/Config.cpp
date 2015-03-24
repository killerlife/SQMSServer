#include "Config.h"

#include <QSettings>

class SQMSConfig : public IConfig
{
public:
	DATABASESETTING GetDatabaseSetting();

	SQMSConfig();

private:
	DATABASESETTING mDS;
};

SQMSConfig sqmsConfig;

IConfig* CreateConfig()
{
	return &sqmsConfig;
}

void ReleaseConfig(IConfig *pConfig)
{
}

SQMSConfig::SQMSConfig()
{
	QSettings settings("config.ini", QSettings::IniFormat);
	mDS.strHost = settings.value("db_host", "localhost").toString();
	mDS.nPort = settings.value("db_port", "3306").toInt();
	mDS.strName = settings.value("db_name", "dev_monitor").toString();
	mDS.strUsr = settings.value("db_user", "root").toString();
	mDS.strPwd = settings.value("db_pass", "123qwe").toString();
	mDS.strRepPath = settings.value("rep_path", "d:/report").toString();
}

DATABASESETTING SQMSConfig::GetDatabaseSetting()
{
	return mDS;
}