#include "RemoteAccessThread.h"
#include <QString>
#include <QDebug>
#include <QModelIndex>
#include <qlogging.h>
#include <QSettings>
RemoteAccessThread::RemoteAccessThread(QObject *parent)
	: QThread(parent), model(NULL), m_remoteModel(NULL)
{
	pLog = CreateLog();
	pDb = CreateDatabase();
	mLAST_ID = getLastId();
	pLog->Write(LOG_DEBUG, mLAST_ID);
	mNATIONAL = getNational();
	mCINEMA = getCinema();
	maxId = getLocalMaxId();
	startId = mLAST_ID;
	endId = maxId;
}
RemoteAccessThread::~RemoteAccessThread()
{
	if(pCfg)delete pCfg; 
	if(pLog)delete pLog;
}

QString RemoteAccessThread::getLocalMaxId()
{
	QString sql = "select id from tb_dev_test_results order by id desc limit 1;"; 
	QSqlQueryModel *model = pDb->ExecSql(sql); 
	pLog->Write(LOG_DEBUG, sql);
	if(!model) return 0; 
	; 
	QString MaxId = model->data(model->index(0,0)).toString();
	if(MaxId > 0 && !model )
	{
			delete model;
			model = NULL;
	}
	this->maxId = MaxId; 
	return MaxId;
}

// 生成远程数据库需要执行的SQL.
// startId : 本地数据库记录起始ID. 
// endId : 本地数据库结束ID . 

QString RemoteAccessThread:: makeSQLs(QString startId, QString endId)
{
	QString tmp, sql, sqls; 
	sql = tmp.sprintf("SELECT\n"
		"	t.national,\n"
		"   t.cinema_line as 'cinema',\n"
		"	t.cinema_name as 't_name',\n"
		"	t.cinema_hall as 't_num',\n"
		"	t.lc as 'S_l',\n"
		"	t.rc as 'S_r',\n"
		"	t.cc as 'S_c',\n"
		"	t.hb as 'S_lfe',\n"
		"	t.la as 'S_ls',\n"
		"	t.ra as 'S_rs',\n"
		"   t.left_after as 'S_lss',\n"
		"   t.right_after as 'S_rss',\n"
		"	t.wx as 'Cx_w',\n"
		"	t.wy as 'Cy_w',\n"
		"	t.rx as 'Cx_r',\n"
		"	t.ry as 'Cy_r',\n"
		"	t.gx as 'Cx_g',\n"
		"	t.gy as 'Cy_g',\n"
		"	t.bluex as 'Cx_b',\n"
		"	t.bluey as 'Cy_b',\n"
		"	t.white_ftl as 'F_w',\n"
		"	t.red_ftl as 'F_r',\n"
		"	t.green_ftl as 'F_g',\n"
		"	t.blue_ftl as 'F_b',\n"
		"	t.temper_c as 'T_c',\n"
		"	t.stime,\n"
		"	t.etime\n"
		"	FROM\n"
		"	tb_dev_test_results t  where id BETWEEN %s and %s;", startId.toStdString().c_str(), endId.toStdString().c_str());
		pLog->Write(LOG_DEBUG, sql);
		model = NULL;
	   model = pDb->ExecSql(sql); 
	   if(!model)return "error";
	   if(model->rowCount() < 0 ) return "";
	 
	   int rowCount = model->rowCount(); 
	   int colCount = model->columnCount(); 
	   sql = "";
	   for(int i = 0; i < rowCount; i++ )
	   {
		   tmp = "INSERT INTO `TestResults` (\n"
			   "	`National`,\n"
			   "	`Cinema`,\n"
			   "	`T_name`,\n"
			   "	`T_num`,\n"
			   "	`S_l`,\n"
			   "	`S_r`,\n"
			   "	`S_c`,\n"
			   "	`S_lfe`,\n"
			   "	`S_ls`,\n"
			   "	`S_rs`,\n"
			   "	`S_lss`,\n"
			   "	`S_rss`,\n"
			   "	`F_w`,\n"
			   "	`F_r`,\n"
			   "	`F_g`,\n"
			   "	`F_b`,\n"
			   "	`Cx_w`,\n"
			   "	`Cy_w`,\n"
			   "	`Cx_r`,\n"
			   "	`Cy_r`,\n"
			   "	`Cx_g`,\n"
			   "	`Cy_g`,\n"
			   "	`Cx_b`,\n"
			   "	`Cy_b`,\n"
			   "	`T_c`,\n"
			   "	`stime`,\n"
			   "	`etime`\n"
			   ")\n"
			   "VALUES\n"
			   "	("; 

		   for(int j = 0; j< colCount-1; j++)
		   {
				tmp += "'" + model->data(model->index(i, j)).toString() + "',";
		   }
			tmp += "'" + model->data(model->index(i, colCount-1)).toString() + "'";
		   tmp += "); "; 
		   sql += tmp;
	   }
	   if(model)
	   {
		   delete model;
		   model = NULL;
	   }
	   return sql;
}

QString RemoteAccessThread::getLastId()
{

	QSettings settings("config.ini", QSettings::IniFormat); 
	QString m = 0; 
	m = settings.value("LAST_ID", "0").toString();
	return m;
}

QString RemoteAccessThread::getCinema()
{
	QSettings settings("config.ini", QSettings::IniFormat); 
	QString m = settings.value("CINEMA", "JinYi").toString();
	return m;
}

QString RemoteAccessThread::getNational()
{
	QSettings settings("config.ini", QSettings::IniFormat); 
	QString m = settings.value("NATIONAL", "China").toString();
	return m;
}

void RemoteAccessThread::run()
{
 	pLog->Write(LOG_INFO, "It is running!");
	QString sqls = makeSQLs(startId, endId);
	
	m_remoteModel = pDb->ExecRemoteSql(sqls);
	pLog->Write(LOG_DEBUG, sqls);
	if(m_remoteModel)
	{
		delete m_remoteModel; 
		m_remoteModel = NULL;
		setLastId(QString::number(endId.toLongLong()+1));
	}
	
	


}

void RemoteAccessThread::setLastId(QString LastId)
{
	QSettings settings("config.ini", QSettings::IniFormat); 
	settings.setValue("LAST_ID", LastId); 
}
void RemoteAccessThread::setDB(IDatabase *pDb)
{
	this->pDb = pDb; 
}

void RemoteAccessThread::setLog(ILog *pLog)
{
	this->pLog = pLog;
}

void RemoteAccessThread::setConfig(IConfig *pCfg)
{
	this->pCfg = pCfg; 
}