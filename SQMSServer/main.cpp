
#include <QtCore/QCoreApplication>

#include "Config.h"
#include "Database.h"
#include "JobDistribute.h"
#include "Log.h"

//#include "QFile"

// extern QStringList Dolby850ParseConfig(const QString& config);
// QString s;

int main(int argc, char *argv[])
{
	QCoreApplication a(argc, argv);
// 	QFile file("r:\\atmos_config_jingyi.xml");
// 
// 	file.open(QFile::ReadOnly|QFile::Text);
// 	s = file.readAll();
// 	QStringList list = Dolby850ParseConfig(s);

// 	IConfig* pConfig = CreateConfig();
// 	DATABASESETTING mDS = pConfig->GetDatabaseSetting();
// 	ReleaseConfig(pConfig);

// 	ILog* pLog = CreateLog();
// 	pLog->Write(LOG_CONFIG, "test");
// 	ReleaseLog(pLog);

// 	IDatabase* pDatabase = CreateDatabase();
// 	SMTPSETTING ss = pDatabase->GetSmtpSetting();
// 	ReleaseDatabase(pDatabase);
	IJobDistribute* pJob = CreateJobDistribute();
	pJob->Start();
	//while(1);
	return a.exec();
}
