#include ".\logfileoperation.h"

#include <QDateTime>
#include <QDir>

CLogFileOperation::CLogFileOperation(void)
{
}

CLogFileOperation::~CLogFileOperation(void)
{
}

bool CLogFileOperation::IsDirectory(const char* path)
{
	QDir dir(path);

	if(dir.exists()) 
		return true;
	else
		return false;
}

bool CLogFileOperation::CreateDirectory(const char* path)
{
	QDir dir;
	if(dir.mkpath(path))
		return true;
	else
		return false;
}

bool CLogFileOperation::DeleteDirectory(const char* path)
{
	QDir dir;
	return dir.rmdir(path);
}

bool CLogFileOperation::OpenFile(const char* file)
{
	m_pfile = fopen(file,"r");
	if (m_pfile == NULL)
		return false;
	return true;
}

bool CLogFileOperation::CloseFile()
{
	if (m_pfile != NULL){
		fclose(m_pfile);
	}
	return true;
}

bool CLogFileOperation::Write(const char* file,const int type, const char* context)
{
	FILE* pf = fopen(file,"a+");
	if (NULL == pf){
		return false;
	}

	QString sType;
	switch (type)
	{
	case LOG_JOBDISTRIBUTE:
		sType = "JobDistribute";
		break;
	case LOG_MODESELECT:
		sType = "ModeSelect";
		break;
	case LOG_DATABASE:
		sType = "Database";
		break;
	case LOG_CONFIG:
		sType = "Config";
		break;
	case LOG_POLLINGTHREAD:
		sType = "PollingThread";
		break;
	case LOG_SISTHREAD:
		sType = "SISThread";
		break;
	case LOG_SCRIPTTHREAD:
		sType = "ScriptThread";
		break;
	case LOG_INTERRUPTTHREAD:
		sType = "InterruptThread";
		break;
	case LOG_REPORT:
		sType = "Report";
		break;
	case LOG_INTERRUPTCLIENTTHREAD:
		sType = "InterruptClientThread";
		break;
	case LOG_SCRIPT:
		sType = "Script";
		break;
	case LOG_DEVICE:
		sType = "Device";
		break;
	case LOG_SENDMAIL:
		sType = "SendMail";
		break;
	case LOG_INFO:
		sType = "INFO";
		break;
	case LOG_DEBUG:
		sType = "DEBUG";
		break;
	default:
		sType = "Unknow";
		break;
	}
	fprintf(pf,"[%s]\t<%s> %s\n",
		sType.toStdString().c_str(),
		QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss").toStdString().c_str(),
		context);


	if (NULL != pf){
		fclose(pf);
	}
	return true;
}


