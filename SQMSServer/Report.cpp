#include "Report.h"
#include "Log.h"
#include "Database.h"
#include "Sendmail.h"
#include "Config.h"
#include "Excel.h"
#include <QThread>
#include <xlsxdocument.h>

class SQMSReport : public IReport, public QThread
{
public:
	void SetSmtpSetting(SMTPSETTING ds);
	void Start();
	void Stop();
	void Exit();
	bool IsRun();
	bool IsRelease();

	SQMSReport();
	~SQMSReport();

protected:
	void run();
	bool isTime();
	void GenerateReport();
	void SendMail(const TQDevItemList& itemList);
	void doExcel(const TQDevItemList& itemList, int lang = 0);

private:
	SMTPSETTING mSS;
	ILog* pLog;
	IDatabase* pDb;
	IConfig* pCfg;
	bool isRun, isRelease;
	int nState;
	QString subject;
	QStringList failList, successList;
};

IReport* CreateReport()
{
	return new SQMSReport;
}

void ReleaseReport(IReport* pReport)
{
	if (pReport)
	{
		delete pReport;
	}
}

SQMSReport::SQMSReport():isRun(false), isRelease(false), nState(0)
{
	pLog = CreateLog();
	pDb = CreateDatabase();
	pCfg = CreateConfig();
	pLog->Write(LOG_REPORT, "Create SQMSReport.");
}

SQMSReport::~SQMSReport()
{
	Stop();
	pLog->Write(LOG_REPORT, "Release SQMSReport.");
	ReleaseConfig(pCfg);
	ReleaseDatabase(pDb);
	ReleaseLog(pLog);
}

void SQMSReport::Exit()
{
	isRun = false; 
	isRelease = true;
	exit();
}

bool SQMSReport::IsRun()
{
	if (!isTime() && nState == 3)
		nState = 1;
	if(!isRun && nState == 1)
		return false;
	else
		return true;
}

bool SQMSReport::IsRelease()
{
	return isRelease;
}

void SQMSReport::SetSmtpSetting(SMTPSETTING ds)
{
	mSS = ds;
}

void SQMSReport::Start()
{
	if(isRun == false)
	{
		isRun = true;
		nState = 1;
		start();
		pLog->Write(LOG_REPORT, "Start SQMSReport thread.");
	}
}

void SQMSReport::Stop()
{
	isRun = false;
	terminate();
	pLog->Write(LOG_REPORT, "Stop SQMSReport thread.");
}

void SQMSReport::run()
{
	while(isRun)
	{
		switch (nState)
		{
		case 1:
			if(isTime())
				nState = 2;
			//Only for test
			//GenerateReport();
			//-------------
			break;
		case 2:
			GenerateReport();
			nState = 3;
			break;
		case 3:
			if(!isTime())
				nState = 1;
			break;
		default:
			break;
		}
		sleep(1);
	}
}

bool SQMSReport::isTime()
{
	int tSend, t;
	QTime mTime = QTime::currentTime();
	tSend = mSS.mStartTime.hour()*3600 + mSS.mStartTime.minute()*60 + mSS.mStartTime.second();
	t = mTime.hour()*3600 + mTime.minute()*60 + mTime.second();
	if(t > (tSend - 600) && t < (tSend + 600))
		return true;
	else
		return false;
}

void SQMSReport::GenerateReport()
{
	TQDevItemList itemList;
	pDb->GetQDevList(itemList);
	if(itemList.size() > 0)
	{
		doExcel(itemList, 0);
		doExcel(itemList, 1);
		SendMail(itemList);
	}
	else
	{
		pLog->Write(LOG_REPORT, tr("GenerateReport: LLAS-100 not found"));
	}
}

void SQMSReport::SendMail(const TQDevItemList& itemList)
{
	QString file = pCfg->GetDatabaseSetting().strRepPath + "/" + "LEONIS-S-QMS-ExaminationReport-"+ itemList.at(0).strTheaterName +"-" + QDate::currentDate().toString("yyyy-MM-dd") + ".xlsx";
	QStringList files;
	files.append(file);
	file = pCfg->GetDatabaseSetting().strRepPath + "/" + QString::fromLocal8Bit("LEONIS数字电影放映质量管理系统测试报告-")+ itemList.at(0).strTheaterName +QString::fromLocal8Bit("店-") + QDate::currentDate().toString("yyyy-MM-dd") + ".xlsx";
	files.append(file);
	QString s = "LEONIS-S-QMS-ExaminationReport-" + itemList.at(0).strTheaterName + "-" + QDateTime::currentDateTime().toString("yyyy-MM-dd");
	//QString body = QString::fromLocal8Bit("<p>这封邮件由LEONIS S-QMS100自动发送，请勿回复。<br>This email was sent by LEONIS S-QMS100 automatically, do not reply please.</p>"
	//	"<p>关于测试数据的说明:<br>1. 亮度测试目前是以2D放映为标准测试，如果检测期间放映机通道设置不正确或3D系统的偏振片没有从镜头前移开，会对检测结果造成影响，使检测结果偏低。<br>"
	//	"2. 根据DCI的要求，放映机安装时需要校准色彩空间，保证所有影厅放映机输出的色彩空间一致，如果色彩空间设置正确，所有影厅检测到的色域坐标就是DCI所要求的标准值。呈现的结果就是所有影厅的色域坐标一致。</p>");
	QString body;
//	if (failList.size()>0)
	{
		body = QString::fromLocal8Bit("<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0 Transitional//EN\">"
			"<HTML><HEAD><META http-equiv=Content-Type content=\"text/html; charset=gb2312\">"
			"<META content=\"MSHTML 6.00.2900.6550\" name=GENERATOR><STYLE>"
			"BLOCKQUOTE { MARGIN-TOP: 0px; MARGIN-BOTTOM: 0px; MARGIN-LEFT: 2em	}"
			"OL { MARGIN-TOP: 0px; MARGIN-BOTTOM: 0px }"
			"UL { MARGIN-TOP: 0px; MARGIN-BOTTOM: 0px }"
			"DIV.FoxDiv20150326131445984163 { COLOR: #000000 }"
			"P { MARGIN-TOP: 0px; MARGIN-BOTTOM: 0px }"
			"DIV.FoxDiv20150327091325890006 { FONT-SIZE: 10.5pt; MARGIN: 10px; COLOR: #000080; LINE-HEIGHT: 1.5; FONT-FAMILY: 宋体 }"
			"BODY { FONT-SIZE: 10.5pt; COLOR: #000080; LINE-HEIGHT: 1.5; FONT-FAMILY: 宋体 }"
			"</STYLE></HEAD><BODY style=\"MARGIN: 10px\"><DIV class=FoxDiv20150327091325890006>"
			"<DIV>这封邮件由LEONIS S-QMS100自动发送，请勿回复。"
			"<DIV>This email was sent by LEONIS S-QMS100 automatically, do not reply please.</DIV>"
			"<DIV>&nbsp;</DIV>"
			"<DIV>&nbsp;</DIV>"
			"<DIV>截止到");
		body += QDateTime::currentDateTime().toString("yyyy") + QString::fromLocal8Bit("年") + 
			QDateTime::currentDateTime().toString("MM") + QString::fromLocal8Bit("月") + 
			QDateTime::currentDateTime().toString("dd") + QString::fromLocal8Bit("号") +
			QDateTime::currentDateTime().toString("HH:mm:ss") + QString::fromLocal8Bit("(设置的邮件发送时间)：</DIV><DIV>&nbsp;</DIV>");
		if(successList.size()>0)
		{
			body += QString::fromLocal8Bit("<DIV>采集到检测数据的影厅序号为：");
			int i;
			for(i = 0; i<successList.size()-1; i++)
			{
				body = body + successList.at(i) + QString::fromLocal8Bit("，");
			}
			body = body + successList.at(i) + QString::fromLocal8Bit("。</DIV>");
		}
		if (failList.size()>0)
		{
			body += QString::fromLocal8Bit("<DIV>未采集到检测数据的影厅序号为：<font color=\"#ff0000\">");
			int i;
			for (i = 0; i <failList.size() - 1; i++)
			{
				body = body + failList.at(i) + QString::fromLocal8Bit("，");
			}
			body = body + failList.at(i) + QString::fromLocal8Bit("。</font></DIV>");
			body += QString::fromLocal8Bit("<DIV>&nbsp;</DIV>");
			body += QString::fromLocal8Bit("<DIV style=\"COLOR: #ff0000\">"
				"建议检查未采集到检测数据的影厅是否按照操作要求正确放映检测片，同时检查以上各厅安装的LLAS-100检测仪电源是否工作正常、网络连接是否正常。");
		}

		body += QString::fromLocal8Bit("</DIV><DIV>&nbsp;</DIV><DIV>&nbsp;</DIV>"
			"<DIV>关于测试数据的说明: <DIV>&nbsp;</DIV>"
			"<DIV>1. 亮度测试目前是以2D放映为标准测试，如果检测期间放映机通道设置不正确或3D系统的偏振片没有从镜头前移开，会对检测结果造成影响，使检测结果偏低。</DIV>"
			"<DIV style=\"COLOR: #ff0000\">建议测试时确保通道正确且3D设备移开。</DIV>"
			"<DIV>&nbsp;</DIV>"
			"<DIV>2. 根据DCI的要求，放映机安装时需要校准色彩空间，保证所有影厅放映机输出的色彩空间一致，如果色彩空间设置正确，所有影厅检测到的色域坐标就是DCI所要求的标准值。</DIV>"
			"<DIV style=\"COLOR: #ff0000\">呈现的结果就是所有影厅的色域坐标一致。</DIV></DIV>"
			"<DIV>&nbsp;</DIV>"
			"</DIV></BODY></HTML>");
	}
#if 0
	else
	{
		body = QString::fromLocal8Bit("<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0 Transitional//EN\">"
		"<HTML><HEAD><META http-equiv=Content-Type content=\"text/html; charset=gb2312\">"
		"<META content=\"MSHTML 6.00.2900.6550\" name=GENERATOR><STYLE>"
		"BLOCKQUOTE { MARGIN-TOP: 0px; MARGIN-BOTTOM: 0px; MARGIN-LEFT: 2em	}"
		"OL { MARGIN-TOP: 0px; MARGIN-BOTTOM: 0px }"
		"UL { MARGIN-TOP: 0px; MARGIN-BOTTOM: 0px }"
		"DIV.FoxDiv20150326131445984163 { COLOR: #000000 }"
		"P { MARGIN-TOP: 0px; MARGIN-BOTTOM: 0px }"
		"DIV.FoxDiv20150327091325890006 { FONT-SIZE: 10.5pt; MARGIN: 10px; COLOR: #000080; LINE-HEIGHT: 1.5; FONT-FAMILY: 宋体 }"
		"BODY { FONT-SIZE: 10.5pt; COLOR: #000080; LINE-HEIGHT: 1.5; FONT-FAMILY: 宋体 }"
		"</STYLE></HEAD><BODY style=\"MARGIN: 10px\"><DIV class=FoxDiv20150327091325890006>"
		"<DIV>这封邮件由LEONIS S-QMS100自动发送，请勿回复。"
		"<DIV>This email was sent by LEONIS S-QMS100 automatically, do not reply please.</DIV>"
		"<DIV>&nbsp;</DIV>"
		"<DIV>&nbsp;</DIV>"
		"<DIV>关于测试数据的说明: <DIV>&nbsp;</DIV>"
		"<DIV>1. 亮度测试目前是以2D放映为标准测试，如果检测期间放映机通道设置不正确或3D系统的偏振片没有从镜头前移开，会对检测结果造成影响，使检测结果偏低。</DIV>"
		"<DIV style=\"COLOR: #ff0000\">建议测试时确保通道正确且3D设备移开。</DIV>"
		"<DIV>&nbsp;</DIV>"
		"<DIV>2. 根据DCI的要求，放映机安装时需要校准色彩空间，保证所有影厅放映机输出的色彩空间一致，如果色彩空间设置正确，所有影厅检测到的色域坐标就是DCI所要求的标准值。</DIV>"
		"<DIV style=\"COLOR: #ff0000\">呈现的结果就是所有影厅的色域坐标一致。</DIV></DIV>"
		"<DIV>&nbsp;</DIV>"
		"</DIV></BODY></HTML>");
	}
#endif
	ISendmail* pMail = NULL;

	pMail = CreateSendmail();
	pMail->SetSmtpSetting(pDb->GetSmtpSetting(), (IReport*)this);
	pMail->Send(s, body, files);
	exec();
	ReleaseSendmail(pMail);
}

void SQMSReport::doExcel(const TQDevItemList& itemList, int lang)
{
	QString title;
	QString subTitle;
	QString copy;
	QString fileName;
	QDate mDate = QDate::currentDate();
	switch (lang)
	{
	case 1:
		title = QString::fromLocal8Bit("数字电影放映质量管理系统(QMS)检测报告");
		subTitle = QString::fromLocal8Bit("本检测报告由LEONIS SQMS-100自动发送");
		copy = QString::fromUtf8("Copyright \xc2\xa9 LEONIS CINEMA, all rights reserved.");
		fileName = pCfg->GetDatabaseSetting().strRepPath + "/" + QString::fromLocal8Bit("LEONIS数字电影放映质量管理系统测试报告-")+ itemList.at(0).strTheaterName +QString::fromLocal8Bit("店-") + QDate::currentDate().toString("yyyy-MM-dd") + ".xlsx";
		break;
	default:
		title = "Examination report of Digital Cinema QMS";
		subTitle = "This report was generated by SQMS-100 automatically";
		copy = QString::fromUtf8("Copyright \xc2\xa9 LEONIS CINEMA, all rights reserved.");
		fileName = pCfg->GetDatabaseSetting().strRepPath + "/" + "LEONIS-S-QMS-ExaminationReport-"+ itemList.at(0).strTheaterName +"-" + QDate::currentDate().toString("yyyy-MM-dd") + ".xlsx";
	}

	Excel excel; 
	excel.setLang(lang);
	excel.setTitle(title); 
	excel.setSubTitle(subTitle); 
	excel.setCopy(copy); 
	excel.setFileName(fileName); 
	excel.setCurrentDate();
	excel.setLog(pLog);
	excel.setDb(pDb);
	excel.setTQDevItemList(itemList);
	excel.setHeader();
	excel.setTodaySPL(); 
	excel.setTodayChroma();
	excel.setTodayLuminance();
	excel.setTodayLuminanceCDM();
	excel.setLastSPL(); 
	excel.setLastChroma(); 
	excel.setLastLuminance();
	excel.setLastLuminanceCDM();
	excel.SaveAs(fileName);
	failList = excel.getFailList();
	successList = excel.getSuccessList();
}

