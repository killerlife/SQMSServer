#ifndef EXCEL_H
#define EXCEL_H
#include <Qtxlsx\xlsxdocument.h>
#include <Qtxlsx\xlsxformat.h>
#include <QObject>

#include <QSqlQueryModel>
#include "Database.h"
#include "Config.h"
#include "Log.h"
#include "LogStruct.h"


 
using namespace QXlsx;


class Excel
{
 
public:
	Excel(QObject *parent = 0);
	~Excel();
	void setLang(int lang);
	void setHeader();
	void setTitle(QString &title); 
	void setSubTitle(QString &subTitle); 
	void setFileName(QString &fileName); 
	void setBody(QString &body);
	void setCopy(QString &copy);
	void setTheaterName(QString &theaterName); 
	void setTQDevItemList(const TQDevItemList &itemList);
	void setCurrentDate(void); 
	void setTodaySPL();
	void setTodayChroma();
	void setTodayLuminance();
	void setTodayLuminanceCDM();
	void setLastSPL();
	void setLastChroma();
	void setLastLuminance();
	void setLastLuminanceCDM();
	void setErrorFormat(Format &format); 
	void setWarningFormat(Format &format); 
	void setSuccessFormat(Format &format); 
	void setDefaultFormat(Format &format);
	void setThemeBlue(Format &format);   // ɫɫ
	void setThemeOrange(Format &format); // ɫ
	void setThemePurple(Format &format); // ɫ
	void setDb(IDatabase* pDb);
	void setModel(QSqlQueryModel *model); 
	void setLog(ILog* pLog);
	void SaveAs(QString &fileName);
	QStringList getFailList();
	QStringList getSuccessList();
private:
	QString fileName;
	QString title;
	QString subTitle;
	QString theaterName;
	QString body;
	QString copy; 
	QStringList faileList, successList;
	QDate mDate;
	Format format;
	Format warning_format;
	IDatabase* pDb;
	/*IDatabase* pDb2;*/
	QSqlQueryModel *model;
	ILog* pLog;
	TQDevItemList itemList;
	Document xlsx;
	int mLang;	
	int nPosStart;
};

#endif // EXCEL_H
