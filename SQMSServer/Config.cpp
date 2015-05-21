#include "Config.h"

#include <QSettings>

class SQMSConfig : public IConfig
{
public:
	DATABASESETTING GetDatabaseSetting();
	DATABASESETTING GetRemoteDatabaseSetting();
	QString GetLastId() ;
	QString GetNational() ;
	QString GetCinema();
	SQMSConfig();

private:
	DATABASESETTING mDS;
	DATABASESETTING mRDS;
	QString mLAST_ID;
	QString mNATIONAL;
	QString mCinema;
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

	mRDS.strHost = settings.value("rdb_host", "bdm113183169.my3w.com").toString(); 
	mRDS.nPort = settings.value("rdb_port", "3306").toInt(); 
	mRDS.strName = settings.value("rdb_name", "bdm113183169_db").toString(); 
	mRDS.strUsr = settings.value("rdb_user", "bdm113183169").toString(); 
	mRDS.strPwd = settings.value("rdb_pass", "leonis62670467").toString();
 
	mLAST_ID = settings.value("LAST_ID", "0").toString();
	mNATIONAL = settings.value("NATIONAL", "China").toString();
	mCinema = settings.value("CINEMA", "JinYi").toString();
#if 0
	settings.setValue("db_host", "localhost");
	settings.setValue("db_port", "3306"); 
	settings.setValue("db_name", "dev_monitor"); 
	settings.setValue("db_user", "root"); 
	settings.setValue("db_pass", "123qwe"); 
	settings.setValue("rep_path", "d:/report"); 

	settings.setValue("rdb_host", "bdm113183169.my3w.com");
	settings.setValue("rdb_port", "3306"); 
	settings.setValue("rdb_name", "bdm113183169_db"); 
	settings.setValue("rdb_user", "bdm113183169"); 
	settings.setValue("rdb_pass", "leonis62670467"); 

	settings.setValue("LAST_ID", "0"); 
	settings.setValue("NATIONAL", "China"); 
	settings.setValue("CINEMA", "JinYi");
	#endif

}

DATABASESETTING SQMSConfig::GetDatabaseSetting()
{
	return mDS;
}

DATABASESETTING SQMSConfig::GetRemoteDatabaseSetting()
{
	return mRDS;
}

QString SQMSConfig::GetLastId() 
{
	return mLAST_ID;
}

QString SQMSConfig::GetNational() 
{
	return mNATIONAL;
}

QString SQMSConfig::GetCinema()
{
	return mCinema;
}