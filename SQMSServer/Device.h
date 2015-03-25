#ifndef DEVICE_H
#define DEVICE_H

#include "DeviceStruct.h"
#include <QString>
#include <QStringList>

class IDevice{
public:
	virtual QString Read(DATACLASS dataClass) = 0;
	virtual int Write(DATACLASS dataClass, const QString& strParam) = 0;
	virtual int SetIp(QString strIp, int nPort = 10001) = 0;
	virtual DEVICETYPE IdType() = 0;
};

IDevice* CreateDevice(DEVICETYPE type);
void ReleaseDevice(IDevice *pDevice);

//--------------------------------------------------------------------------
//This function just use for parse atmos config file.
//Usage:
//       QStringList list = Dolby850ParseConfig(pDev->Read(CP850_GETCONFIG);
//Return:
//       All speaker channel name will be save in QStringList
//		 Please note that the position is very important.
//		 Such as list[0] = "L", if you want to make it output pinknoise, 
//		 you need to convert position to QString and pass it to IDevice
//		 pDev->Write(CP850_SETCHANNEL, QString::number(0));
QStringList Dolby850ParseConfig(const QString& config);
//--------------------------------------------------------------------------

#endif