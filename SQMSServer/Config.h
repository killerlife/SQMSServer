#ifndef CONFIG_H
#define CONFIG_H

#include "ConfigStruct.h"

class IConfig
{
public:
	virtual DATABASESETTING GetDatabaseSetting() = 0;
	virtual DATABASESETTING GetRemoteDatabaseSetting()=0;
	virtual QString GetLastId()=0;
	virtual QString GetNational()=0;
	virtual QString GetCinema()=0;
};

IConfig* CreateConfig();
void ReleaseConfig(IConfig* pConfig);

#endif