#include <QtCore/QCoreApplication>

#include "Config.h"
#include "Database.h"
#include "JobDistribute.h"
#include "Log.h"

//#define WIN_SERVICE 1

#ifdef WIN_SERVICE
#include <windows.h>
#include <stdio.h>
#include <wchar.h>
#include <stdarg.h>
#include <varargs.h>

void Init();
BOOL IsInstalled();
BOOL Install();
BOOL Uninstall();
void LogEvent(LPCTSTR pszFormat, ...);
void WINAPI ServiceMain();
void WINAPI ServiceStrl(DWORD dwOpcode);

QString szServiceName = "SQMSServer Service";
BOOL bInstall;
SERVICE_STATUS_HANDLE hServiceStatus;
SERVICE_STATUS status;
DWORD dwThreadID;

#endif
//#include "QFile"

// extern QStringList Dolby850ParseConfig(const QString& config);
// QString s;
#ifndef WIN_SERVICE
#include <windows.h>
#include <QAbstractNativeEventFilter>
class SQMSEventFilter: public QAbstractNativeEventFilter
{
public:
	virtual bool nativeEventFilter(const QByteArray &eventType, void *message, long *) Q_DECL_OVERRIDE
	{
		if (eventType == "windows_generic_MSG")
		{
			if (((MSG*)message)->message == WM_CLOSE)
			{
				QCoreApplication::exit();
				return true;
			}
		}
		return false;
	}
};

int main(int argc, char *argv[])
{
	QCoreApplication a(argc, argv);

	SQMSEventFilter eventFilter;
	a.installNativeEventFilter(&eventFilter);

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
#endif

#ifdef WIN_SERVICE

#include <QAbstractNativeEventFilter>
//We need to rewrite QCoreApplication to process WM_CLOSE to stop service
class SQMSEventFilter: public QAbstractNativeEventFilter
{
public:
	virtual bool nativeEventFilter(const QByteArray &eventType, void *message, long *) Q_DECL_OVERRIDE
	{
		if (eventType == "windows_generic_MSG")
		{
			if (((MSG*)message)->message == WM_CLOSE)
			{
				QCoreApplication::exit();
				return true;
			}
		}
		return false;
	}
};

int main(int argc, char *argv[])
{
	//SetCurrentDirectory(QString("D:/work/sqmsserver/Release").toStdWString().c_str());
	Init();

	dwThreadID = ::GetCurrentThreadId();

	SERVICE_TABLE_ENTRY st[] =
	{
		{ (LPWSTR)szServiceName.toStdWString().c_str(), (LPSERVICE_MAIN_FUNCTION)ServiceMain },
		{ NULL, NULL }
	};

	if (argc > 1)
	{
		if (stricmp(argv[1], "/install") == 0)
		{
			Install();
		}
		else if (stricmp(argv[1], "/uninstall") == 0)
		{
			Uninstall();
		}	
		else
		{
			if (!::StartServiceCtrlDispatcher(st))
			{
				LogEvent((LPWSTR)QString("Register Service Main Function Error!").toStdWString().c_str());
			}
		}
	}
	else
	{
		if (!::StartServiceCtrlDispatcher(st))
		{
			LogEvent((LPWSTR)QString("Register Service Main Function Error!").toStdWString().c_str());
		}
	}

	return 0;
}

void Init()
{
	hServiceStatus = NULL;
	status.dwServiceType = SERVICE_WIN32_OWN_PROCESS;
	status.dwCurrentState = SERVICE_STOPPED;
	status.dwControlsAccepted = SERVICE_ACCEPT_STOP;
	status.dwWin32ExitCode = 0;
	status.dwServiceSpecificExitCode = 0;
	status.dwCheckPoint = 0;
	status.dwWaitHint = 0;
}

void WINAPI ServiceMain()
{
	// Register the control request handler
	status.dwCurrentState = SERVICE_START_PENDING;
	status.dwControlsAccepted = SERVICE_ACCEPT_STOP;

	//×¢
	hServiceStatus = RegisterServiceCtrlHandler((LPWSTR)szServiceName.toStdWString().c_str(), ServiceStrl);
	if (hServiceStatus == NULL)
	{
		LogEvent(QString("Handler not installed").toStdWString().c_str());
		return;
	}
	SetServiceStatus(hServiceStatus, &status);

	status.dwWin32ExitCode = S_OK;
	status.dwCheckPoint = 0;
	status.dwWaitHint = 0;
	status.dwCurrentState = SERVICE_RUNNING;
	SetServiceStatus(hServiceStatus, &status);

	int count = 1;

	WCHAR dir[200];
	GetModuleFileName(NULL, dir, 200);
	QString path = QString::fromWCharArray(dir);
	path.remove("SQMSServer.exe");
	SetCurrentDirectory((LPCWSTR)path.toStdWString().c_str());
	CreateLog()->Write(LOG_CONFIG, path);
	QCoreApplication a(count, NULL);
	SQMSEventFilter eventFilter;
	a.installNativeEventFilter(&eventFilter);
	a.addLibraryPath(path);

	IJobDistribute* pJob = CreateJobDistribute();
	pJob->Start();
	a.exec();


	status.dwCurrentState = SERVICE_STOPPED;
	SetServiceStatus(hServiceStatus, &status);
	LogEvent(QString("Service stopped").toStdWString().c_str());
}

BOOL IsInstalled()
{
	BOOL bResult = FALSE;

	//íª¾íº·Æ¹
	SC_HANDLE hSCM = ::OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);

	if (hSCM != NULL)
	{
		//íª¾íº·
		SC_HANDLE hService = ::OpenService(hSCM, (LPWSTR)szServiceName.toStdWString().c_str(), SERVICE_QUERY_CONFIG);
		if (hService != NULL)
		{
			bResult = TRUE;
			::CloseServiceHandle(hService);
		}
		::CloseServiceHandle(hSCM);
	}
	return bResult;
}

BOOL Install()
{
	if (IsInstalled())
		return TRUE;

	//íª¾íº·Æ¹
	SC_HANDLE hSCM = ::OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
	if (hSCM == NULL)
	{
		MessageBox(NULL, QString("Couldn't open service manager").toStdWString().c_str(), (LPWSTR)szServiceName.toStdWString().c_str(), MB_OK);
		return FALSE;
	}

	// Get the executable file path
	TCHAR szFilePath[MAX_PATH];
	::GetModuleFileName(NULL, szFilePath, MAX_PATH);

	//
	SC_HANDLE hService = ::CreateService(
		hSCM, (LPWSTR)szServiceName.toStdWString().c_str(), (LPWSTR)szServiceName.toStdWString().c_str(),
		SERVICE_ALL_ACCESS, SERVICE_WIN32_OWN_PROCESS,
		SERVICE_AUTO_START, SERVICE_ERROR_NORMAL,
		szFilePath, NULL, NULL, QString("MySQL").toStdWString().c_str(), NULL, NULL);

	if (hService == NULL)
	{
		::CloseServiceHandle(hSCM);
		MessageBox(NULL, QString("Couldn't create service").toStdWString().c_str(), (LPWSTR)szServiceName.toStdWString().c_str(), MB_OK);
		return FALSE;
	}

	::CloseServiceHandle(hService);
	::CloseServiceHandle(hSCM);
	return TRUE;
}

BOOL Uninstall()
{
	if (!IsInstalled())
		return TRUE;

	SC_HANDLE hSCM = ::OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);

	if (hSCM == NULL)
	{
		MessageBox(NULL, QString("Couldn't open service manager").toStdWString().c_str(), (LPWSTR)szServiceName.toStdWString().c_str(), MB_OK);
		return FALSE;
	}

	SC_HANDLE hService = ::OpenService(hSCM, (LPWSTR)szServiceName.toStdWString().c_str(), SERVICE_STOP | DELETE);

	if (hService == NULL)
	{
		::CloseServiceHandle(hSCM);
		MessageBox(NULL, QString("Couldn't open service").toStdWString().c_str(), (LPWSTR)szServiceName.toStdWString().c_str(), MB_OK);
		return FALSE;
	}
	SERVICE_STATUS status;
	::ControlService(hService, SERVICE_CONTROL_STOP, &status);

	//É¾
	BOOL bDelete = ::DeleteService(hService);
	::CloseServiceHandle(hService);
	::CloseServiceHandle(hSCM);

	if (bDelete)
		return TRUE;

	LogEvent(QString("Service could not be deleted").toStdWString().c_str());
	return FALSE;
}

void LogEvent(LPCTSTR pFormat, ...)
{
	TCHAR    chMsg[256];
	HANDLE  hEventSource;
	LPTSTR  lpszStrings[1];
	va_list pArg;

	va_start(pArg, pFormat);
	vswprintf(chMsg, pFormat, pArg);
	va_end(pArg);

	lpszStrings[0] = chMsg;

	hEventSource = RegisterEventSource(NULL, (LPWSTR)szServiceName.toStdWString().c_str());
	if (hEventSource != NULL)
	{
		ReportEvent(hEventSource, EVENTLOG_INFORMATION_TYPE, 0, 0, NULL, 1, 0, (LPCTSTR*) &lpszStrings[0], NULL);
		DeregisterEventSource(hEventSource);
	}
}

void WINAPI ServiceStrl(DWORD dwOpcode)
{
	switch (dwOpcode)
	{
	case SERVICE_CONTROL_STOP:
		status.dwCurrentState = SERVICE_STOP_PENDING;
		SetServiceStatus(hServiceStatus, &status);

		//Killerlife
		//Can't Send/Recv message under the QT framework.
		//I was do my best. So directly call exit to stop service.
		//--------------------------------------------------------
		//PostThreadMessage(dwThreadID, WM_CLOSE, 0, 0);
		QCoreApplication::exit();
		//--------------------------------------------------------
		break;
	case SERVICE_CONTROL_PAUSE:
		break;
	case SERVICE_CONTROL_CONTINUE:
		break;
	case SERVICE_CONTROL_INTERROGATE:
		break;
	case SERVICE_CONTROL_SHUTDOWN:
		break;
	default:
		LogEvent(QString("Bad service request").toStdWString().c_str());
	}
}
#endif