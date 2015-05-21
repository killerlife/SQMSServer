#ifndef REMOTEACCESSTHREAD_H
#define REMOTEACCESSTHREAD_H
#include "Database.h"
#include <QThread>
#include "Config.h"
#include "Log.h"
#include <QString>

typedef struct _TestResultsItem
{
	QString national; 
	QString cinema; 
	QString t_name; 
	QString T_num; 
	QString S_l; 
	QString S_r; 
	QString S_c; 
	QString S_lfe; 
	QString S_ls; 
	QString S_rs; 
	QString S_lss;
	QString S_rss;
	QString F_w; 
	QString F_r; 
	QString F_g; 
	QString F_b; 
	QString Cx_w; 
	QString Cy_w; 
	QString Cx_r;
	QString Cy_r; 
	QString Cx_g;
	QString Cy_g; 
	QString Cx_b;
	QString Cy_b; 
	QString T_c; 
	QString stime;
	QString etime;
}RemoteResultItem;



class RemoteAccessThread : public QThread
{
	Q_OBJECT

public:
	RemoteAccessThread(QObject *parent);
	~RemoteAccessThread();
	void setDB(IDatabase *pDb); 
	void setLog(ILog *pLog);
	void setConfig(IConfig *pCfg);
	void setLastId(QString LastId);
	
protected:
	void run(); 
	 QString getLocalMaxId();
	 QString getLastId() ; 
	QString getNational() ;
	QString getCinema();
	QString makeSQLs(QString startId, QString endId);

private:
	IDatabase *pDb; 
	IConfig *pCfg; 
	ILog *pLog;
	QString mLAST_ID;
	QString mNATIONAL;
	QString mCINEMA;
	QSqlQueryModel *model;
	QSqlQueryModel *m_remoteModel;
	 QList<RemoteResultItem> remote_result_list; 
	  QString  maxId;
	  QString startId;
	  QString  endId;
	
};

#endif // REMOTEACCESSTHREAD_H
