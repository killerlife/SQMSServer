#pragma once

#include "Config.h"
#include "ScriptStruct.h"
#include <QSqlQueryModel>

class IDatabase
{
public:
	virtual SMTPSETTING GetSmtpSetting() = 0;
	virtual void GetQDevList(TQDevItemList& itemList) = 0;
	virtual void SaveCollectData(TValueItemList* pList, QMSDEVICE& mDev) = 0;
	virtual QSqlQueryModel* ExecSql(QString sql) = 0;
	virtual QSqlQueryModel* ExecRemoteSql(QString sql) = 0;
};

IDatabase* CreateDatabase();

void ReleaseDatabase(IDatabase *pDatabase);
