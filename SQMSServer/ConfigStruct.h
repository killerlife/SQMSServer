#pragma once

#include <QString>
#include <QTime>
#include <QList>

typedef enum
{
	STATE_NONE = 0,
	STATE_RELOAD,
	STATE_MODSELECT,
	STATE_CREATETASK,
	STATE_MAINLOOP,
	STATE_END
} JOBSTATE;

typedef enum
{
	MODE_SIS = 0,
	MODE_POLLING,
	MODE_SCRIPT,
	MODE_INTERRUPT,
	MODE_END
} MODESYNC;

typedef enum
{
	DEV_LLAS100,
	DEV_USL100,
	DEV_END
} DEVICE_TYPE;

typedef struct  
{
	QString strHost;
	QString strName;
	QString strUsr;
	QString strPwd;
	int nPort;
	QString strRepPath;
} DATABASESETTING;

typedef struct
{
	QString strSender;
	QString strPwd;
	QString strServer;
	int nPort;
	QString strRecvList;
	QTime mStartTime;
} SMTPSETTING;

typedef struct
{
	QString strIp;
	QString strTheaterName;
	QString strTheaterNo;
	QString strScript;
	QTime tBefore;
	QTime tAfter;
	int nPort;
	MODESYNC mSyncMode;
	DEVICE_TYPE mDevType;
	double fix_ftl;
	double fix_lc;
	double fix_rc;
	double fix_cc;
	double fix_hb;
	double fix_la;
	double fix_ra;
}QMSDEVICE;

typedef QList<QMSDEVICE> TQDevItemList;