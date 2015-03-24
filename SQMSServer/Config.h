#ifndef CONFIG_H
#define CONFIG_H

#include "ConfigStruct.h"

class IConfig
{
public:
	virtual DATABASESETTING GetDatabaseSetting() = 0;
};

IConfig* CreateConfig();
void ReleaseConfig(IConfig* pConfig);

#endif