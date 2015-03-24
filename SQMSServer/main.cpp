
#include <QtCore/QCoreApplication>

#include "Config.h"
#include "Database.h"
#include "JobDistribute.h"
#include "Log.h"

int main(int argc, char *argv[])
{
	QCoreApplication a(argc, argv);

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
