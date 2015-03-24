#pragma once
#include "ConfigStruct.h"

#include <QThread>
#include <QTime>

typedef enum
{
	T_IN_RANGE = 0,
	T_AFTER_10MIN,
	T_BEFORE_10MIN,
	T_END
} T_KIND;

bool isTime(QTime tBefore, QTime tAfter, T_KIND nKind = T_IN_RANGE);

class ISISThread
{
public:
	virtual void Start() = 0;
	virtual void Stop() = 0;
	virtual void SetQDevice(QMSDEVICE qDev) = 0;
	virtual int GetRunState() = 0;
	virtual bool isTime(QTime mTime, T_KIND nKind = T_IN_RANGE) = 0;
};

ISISThread* CreateSISThread();
void ReleaseSISThread(ISISThread *pSIS);

