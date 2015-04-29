#include "Database.h"
#include "Log.h"

#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQueryModel>
#include <stdlib.h>
#include <QThread>

class SQMSDatabase : public IDatabase
{
public:
	SMTPSETTING GetSmtpSetting();
	void GetQDevList(TQDevItemList& itemList);
	void SaveCollectData(TValueItemList *pList, QMSDEVICE& mDev);
	QSqlQueryModel* ExecSql(QString sql);

	SQMSDatabase();
	~SQMSDatabase();

protected:
	void InitDB();
	void OpenDB();
	void CloseDB();

private:
	IConfig *pConfig;
	ILog *pLog;
	QSqlDatabase db;
	QSqlQueryModel *pModel;
	SMTPSETTING mSS;
	volatile char nFlag; 
};

SQMSDatabase* pSqmsDatabase = NULL;

IDatabase* CreateDatabase()
{
	if(pSqmsDatabase == NULL)
	{
		pSqmsDatabase = new SQMSDatabase;
	}
	return pSqmsDatabase;
}

void ReleaseDatabase(IDatabase *pDatabase)
{
}

SQMSDatabase::SQMSDatabase():nFlag(0)
{
	pConfig = CreateConfig();
	pLog = CreateLog();
	InitDB();
}

SQMSDatabase::~SQMSDatabase()
{
	db.close();
	ReleaseLog(pLog);
	ReleaseConfig(pConfig);
}

void SQMSDatabase::InitDB()
{
	DATABASESETTING mDS = pConfig->GetDatabaseSetting();

	db = QSqlDatabase::addDatabase("QMYSQL");
	QSqlError sqlerr = db.lastError();
	if (sqlerr.type() != QSqlError::NoError)
	{
		QString err;
		err.sprintf("DATABASE INIT ERROR: %s", sqlerr.text().toStdString().c_str());
		if(pLog)
			pLog->Write(LOG_DATABASE, err);
		return;
	}
	db.setHostName(mDS.strHost);
	db.setPort(mDS.nPort);
	db.setDatabaseName(mDS.strName);
	db.setUserName(mDS.strUsr);
	db.setPassword(mDS.strPwd);
}

void SQMSDatabase::OpenDB()
{
	if (!db.open())
	{
		QString err;
		err.sprintf("DATABASE CONNECTION ERROR: %s", db.lastError().text().toStdString().c_str());
		if (pLog)
			pLog->Write(LOG_DATABASE, err);
		return ;
	}
}

void SQMSDatabase::CloseDB()
{
	if (db.isOpen())
	{
		db.close();
	}
}

SMTPSETTING SQMSDatabase::GetSmtpSetting()
{
	while(nFlag)
		QThread::msleep(int((double)rand()/(double)UINT_MAX*50));
	nFlag = 1;
	OpenDB();
	if (db.isOpen())
	{
		QString sql = QObject::tr("select SenderId, SenderPwd, SmtpServer, ServerPort, RecvList, SendTime from smtpreportconfig");
		pModel = new QSqlQueryModel;
		pModel->setQuery(sql, db);
		if(pModel->lastError().isValid())
		{
			pLog->Write(LOG_DATABASE, QObject::tr("Execute GetSmtpSetting SQL failure."));
		}
		if(pModel->rowCount() == 0)
		{
			pLog->Write(LOG_DATABASE, QObject::tr("No SMTP Setting in database."));
		}
		else
		{
			mSS.strSender = pModel->data(pModel->index(0, 0)).toString();
			mSS.strPwd = pModel->data(pModel->index(0, 1)).toString();
			mSS.strServer = pModel->data(pModel->index(0, 2)).toString();
			mSS.nPort = pModel->data(pModel->index(0,3)).toInt();
			mSS.strRecvList = pModel->data(pModel->index(0,4)).toString();
			mSS.mStartTime = pModel->data(pModel->index(0, 5)).toTime();
		}
		delete pModel;
		pModel = NULL;
		CloseDB();
	}
	nFlag = 0;
	return mSS;
}

void SQMSDatabase::GetQDevList(TQDevItemList& itemList)
{
	while(nFlag)
		QThread::msleep(int((double)rand()/(double)UINT_MAX*50));
	nFlag = 1;
	OpenDB();
	if (db.isOpen())
	{
		QString sql = QObject::tr("select cinema_hall, dev_ip, fix_ftl, fix_lc, fix_rc, fix_cc, fix_hb, fix_la, fix_ra, start_datetime, end_datetime, cinema_name, script, sync_mode, port from tb_dev_corrected_value_conf ");
		pModel = new QSqlQueryModel;
		pModel->setQuery(sql, db);
		if (pModel->lastError().isValid())
		{
			pLog->Write(LOG_DATABASE, QObject::tr("Execute GetQDevList SQL failure."));
			delete pModel;
			pModel = NULL;
			CloseDB();
			nFlag = 0;
			return;
		}
		itemList.clear();
		QMSDEVICE item;
		for(int i = 0; i < pModel->rowCount(); i++)
		{
			item.strTheaterNo = pModel->data(pModel->index(i, 0)).toString();
			item.strIp = pModel->data(pModel->index(i, 1)).toString();
			if (pModel->data(pModel->index(i, 14)).toInt() == 0)
				item.nPort = 10001;
			else
				item.nPort = pModel->data(pModel->index(i, 14)).toInt();

			item.mDevType = DEV_LLAS100;
			item.fix_ftl = pModel->data(pModel->index(i, 2)).toDouble();
			item.fix_lc = pModel->data(pModel->index(i, 3)).toDouble();
			item.fix_rc = pModel->data(pModel->index(i, 4)).toDouble();
			item.fix_cc = pModel->data(pModel->index(i, 5)).toDouble();
			item.fix_hb = pModel->data(pModel->index(i, 6)).toDouble();
			item.fix_la = pModel->data(pModel->index(i, 7)).toDouble();
			item.fix_ra = pModel->data(pModel->index(i, 8)).toDouble();
			item.tBefore = pModel->data(pModel->index(i, 9)).toTime();
			item.tAfter = pModel->data(pModel->index(i, 10)).toTime();
			item.strTheaterName = pModel->data(pModel->index(i, 11)).toString();
			//item.mSyncMode = MODE_SIS;
			item.mSyncMode = (MODESYNC)pModel->data(pModel->index(i, 13)).toInt();
			item.strScript = pModel->data(pModel->index(i, 12)).toString();
			if (item.strScript.isEmpty() || item.strScript == "")
			{
				item.strScript = "sleep=12000;\
						sensor.xy.white=0;\
						sleep=5000;\
						sensor.ftl.red=0;sensor.xy.red=0;\
						sleep=5000;\
						sensor.ftl.green=0;sensor.xy.green=0;\
						sleep=5000;\
						sensor.ftl.blue=0;sensor.xy.blue=0;\
						sleep=5000;\
						sensor.spl.left=0;\
						sleep=5000;\
						sensor.spl.right=0;\
						sleep=5000;\
						sensor.spl.center=0;\
						sleep=5000;\
						sensor.spl.lfe=0;\
						sleep=5000;\
						sensor.spl.ls=0;\
						sleep=5000;\
						sensor.spl.rs=0";
			}
			itemList.push_back(item);
		}
		delete pModel;
		pModel = NULL;
		CloseDB();
	}
	nFlag = 0;
}

void SQMSDatabase::SaveCollectData(TValueItemList *pList, QMSDEVICE& mDev)
{
	while(nFlag)
		QThread::msleep(int((double)rand()/(double)UINT_MAX*50));
	nFlag = 1;
	OpenDB();
	if (db.isOpen())
	{
		QString s1 = QString("insert into tb_dev_test_results(");
		QString s2 = QString(")values(");
		for(int i = 0; i < pList->size(); i++)
		{
			LISTITEM item = pList->at(i);
			if(!item.name.contains("unsupport"))
			{
				s1 = s1 + item.name + ", ";
				s2 = s2 + "'" + item.value + "', ";
			}
		}
		s1 += QString("cinema_name, cinema_hall, dev_ip, stime, etime");
		s2 = s2 + "'" + mDev.strTheaterName + "', '" + mDev.strTheaterNo + "', '"\
			+ mDev.strIp + "', '" + QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss") \
			+ "', '" + QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
		QString sql = s1 + s2 + "');";
		sql.remove("\r");
		sql.remove("\n");
		pModel = new QSqlQueryModel;
		pModel->setQuery(sql, db);
		if (pModel->lastError().isValid())
		{
			pLog->Write(LOG_DATABASE, QObject::tr("Execute SaveCollectData SQL failure."));
			pLog->Write(LOG_DATABASE, sql);
			pLog->Write(LOG_DATABASE, pModel->lastError().text());
			delete pModel;
			pModel = NULL;
			CloseDB();
			nFlag = 0;
			return;
		}
		pLog->Write(LOG_DATABASE, sql);
		delete pModel;
		pModel = NULL;
		CloseDB();
	}
	nFlag = 0;
}

QSqlQueryModel* SQMSDatabase::ExecSql(QString sql)
{
	while(nFlag)
		QThread::msleep(int((double)rand()/(double)UINT_MAX*50));
	nFlag = 1;
	OpenDB();
	if (db.isOpen())
	{
		pModel = new QSqlQueryModel;
		pModel->setQuery(sql, db);
		if (pModel->lastError().isValid())
		{
			pLog->Write(LOG_DATABASE, QObject::tr("Execute ExecSql SQL failure."));
			pLog->Write(LOG_DATABASE, sql);
			pLog->Write(LOG_DATABASE, pModel->lastError().text());
			delete pModel;
			pModel = NULL;
			CloseDB();
			nFlag = 0;
			return NULL;
		}
		CloseDB();
		nFlag = 0;
		return pModel;
	}
	nFlag = 0;
	return NULL;
}