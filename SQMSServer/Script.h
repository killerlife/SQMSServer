#ifndef SCRIPT_H
#define SCRIPT_H
#include "ScriptStruct.h"
#include "DeviceStruct.h"
#include "Device.h"
#include "ConfigStruct.h"

class IScript{
public:
	virtual int LoadScript(char* fn) = 0;
	virtual int LoadScript(QString script) = 0;
	virtual int ExecScript() = 0;
	virtual int AddDevice(DEVICETYPE type, IDevice *pDev, int pos = 0) = 0;
	virtual TValueItemList* GetTestValueList() = 0;
	virtual int SetFixValue(const QMSDEVICE& mQdev) = 0;
};

IScript* CreateScript();
void ReleaseScript(IScript* pScript);

#endif