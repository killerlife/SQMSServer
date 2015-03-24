#ifndef ILOG_H
#define ILOG_H

#include "LogStruct.h"
#include <QString>
#include <QDateTime>

class ILog
{
public:
	virtual int Write(LOGTYPE type, QString strLog) = 0;
	virtual int Delete(QDateTime timeAfter, QDateTime timeBefore) = 0;
};

ILog* CreateLog();
void ReleaseLog(ILog* pLog);

#endif