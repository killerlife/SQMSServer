#ifndef SCRIPT_H
#define SCRIPT_H
#include "ScriptStruct.h"
#include "DeviceStruct.h"
#include "Device.h"
#include "ConfigStruct.h"

class IScript{
public:
	virtual int LoadScript(char* fn) = 0; //This function load script from file, fn is file name.
	virtual int LoadScript(QString script) = 0; //This function load script from QString, script can be store in database or another buffer.
	virtual int ExecScript() = 0; //This function execute script.
	virtual int AddDevice(DEVICETYPE type, IDevice *pDev, int pos = 0) = 0; //This function add device into script class, the device can be sensor/audio processor/projector.
	virtual TValueItemList* GetTestValueList() = 0; //This function return the test results. It's must be call after ExecScript().
	virtual int SetFixValue(const QMSDEVICE& mQdev) = 0; //This function set fix value of hall to make the collection data correct.
};

IScript* CreateScript();
void ReleaseScript(IScript* pScript);

#endif