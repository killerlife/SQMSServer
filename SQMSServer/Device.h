#ifndef DEVICE_H
#define DEVICE_H

#include "DeviceStruct.h"
#include <QString>

class IDevice{
public:
	virtual QString Read(DATACLASS dataClass) = 0;
	virtual int Write(DATACLASS dataClass, const QString& strParam) = 0;
	virtual int SetIp(QString strIp, int nPort = 10001) = 0;
};

IDevice* CreateDevice(DEVICETYPE type);
void ReleaseDevice(IDevice *pDevice);

#endif