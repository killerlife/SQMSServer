#include "Log.h"
#include "LogFileOperation.h"

#include <QThread>
#include <QString>

class SQMSLog : public ILog
{
public:
	int Write(LOGTYPE type, QString strLog);
	int Delete(QDateTime timeAfter, QDateTime timeBefore);

	SQMSLog();

private:
	CLogFileOperation mLogFileOp;
	volatile char nFlag;
};

int log_ref = 0;

//SQMSLog *sqmsLog;
SQMSLog sqmsLog;

ILog* CreateLog()
{
#if 0
	return new SQMSLog;
#else
	return &sqmsLog;
#endif
}

void ReleaseLog(ILog* pLog)
{
#if 0
	if (pLog)
	{
		delete pLog;
	}
#else
	return;
#endif
}

SQMSLog::SQMSLog():nFlag(0)
{

}

int SQMSLog::Delete(QDateTime timeAfter, QDateTime timeBefore)
{
	return 0;
}

int SQMSLog::Write(LOGTYPE type, QString strLog)
{
	QString file = "./log";//SQMSServer-" + QDate::currentDate().toString("yyyy-MM-dd") + ".log";
	while(nFlag)
		QThread::msleep(int((double)rand()/(double)UINT_MAX*50));
	nFlag = 1;

	if(mLogFileOp.IsDirectory(file.toStdString().c_str()) == false)
		if (mLogFileOp.CreateDirectory(file.toStdString().c_str()) == false)
			return 0;
	
	file = file + "/SQMSServer-" + QDate::currentDate().toString("yyyy-MM-dd") + ".log";

	int result = mLogFileOp.Write(file.toStdString().c_str(), type, strLog.toStdString().c_str());
	nFlag = 0;
	return result;
}