#include "Excel.h"
#include <QDebug>
#define STD_VALUE_CHROMA 0.03
#define STD_VALUE_SPL 5
#define STD_VALUE_LUMINANCE 3

Excel::Excel(QObject *parent):mLang(0)
{

}

Excel::~Excel()
{

}

void Excel::setLang(int lang)
{
	mLang = lang;
}

void Excel::setHeader()
{
	format.setFontBold(false);
	format.setFontColor(QColor(255, 255, 255));
	format.setFontSize(24);
	format.setFontName("Book Antiqua");
	format.setPatternBackgroundColor(QColor("#3366FF"));

	switch (mLang)
	{
	case 1:
		xlsx.addSheet(QString::fromLocal8Bit("检测结果"));
		break;
	default:
		xlsx.addSheet("Test Result");
	}
	xlsx.setColumnWidth(2, 10); //Column B
	xlsx.setColumnWidth(8, 10); //Column H
	for(int i = 0; i<10; i++)
		xlsx.setColumnWidth(i+1, 12);
	xlsx.currentWorksheet()->setGridLinesVisible(true);
	format.setHorizontalAlignment(QXlsx::Format::AlignHCenter);
	xlsx.write(1, 1, title, format);
	xlsx.mergeCells("A1:J1", format);
	format.setFontSize(12);

	xlsx.write(2, 1, subTitle, format);
	xlsx.mergeCells("A2:J2", format);

	xlsx.mergeCells("A3:J3", format);
	format.setFontSize(12);
	format.setHorizontalAlignment(QXlsx::Format::AlignLeft);
	switch(mLang)
	{
	case 1:
		xlsx.write(4, 1, QString::fromLocal8Bit("影城名: ") + itemList.at(0).strTheaterName, format );
		xlsx.mergeCells("A4:J4", format);

		xlsx.write(5, 1, QString::fromLocal8Bit("日期: ") + QDate::currentDate().toString("M/d/yyyy"), format);
		break;
	default:
		xlsx.write(4, 1, "Theater: " + itemList.at(0).strTheaterName, format );
		xlsx.mergeCells("A4:J4", format);

		xlsx.write(5, 1, "Date: " + QDate::currentDate().toString("M/d/yyyy"), format);
	}
	xlsx.mergeCells("A5:J5", format);

	xlsx.write(6, 1, copy, format);
	xlsx.mergeCells("A6:J6", format);

	format.setFontName("Book Antiqua");
	format.setFontSize(16);
	format.setFontBold(true);
	format.setFontColor(QColor(0, 0, 0));
	format.setPatternBackgroundColor(QColor(255, 255, 255));
	format.setBorderStyle(Format::BorderThin);
	format.setBorderColor(QColor("#eee"));
// 	xlsx.write(9, 1, "The test results of today are below:", format);
// 	xlsx.mergeCells("A9:J9", format);
}

void Excel::setTheaterName(QString &theaterName)
{
	this->theaterName = theaterName; 
}

void Excel::setTQDevItemList(const TQDevItemList &itemList)
{
	this->itemList = itemList;
}

void Excel::setFileName(QString &fileName)
{
	this->fileName = fileName; 
}

void Excel::setBody(QString &copy)
{
	this->body = body;
}

void Excel::setCopy(QString &copy)
{
	this->copy = copy; 
}

void Excel::setCurrentDate(void)
{
	this->mDate = QDate::currentDate(); 

}

void Excel::setTitle(QString &title)
{
	this->title = title; 
}

void Excel::setSubTitle(QString &subTitle)
{
	this->subTitle = subTitle;
}

#include "QtXlsx/xlsxrichstring.h"

QStringList Excel::getFailList()
{
	return faileList;
}

QStringList Excel::getSuccessList()
{
	return successList;
}

void Excel::setTodaySPL()
{
	faileList.clear();
	successList.clear();
	nPosStart = 9;
	switch(mLang)
	{
	case 1:
		xlsx.write(nPosStart, 1, QString::fromLocal8Bit("当天检测结果如下:"), format);
		break;
	default:
		xlsx.write(nPosStart, 1, "The test results of today are below:", format);
	}

	xlsx.mergeCells("A9:J9", format);
	format.setPatternBackgroundColor(QColor("#4BACC6"));
	format.setFontBold(false);
	format.setFontName("Book Antiqua");
	format.setFontSize(12);
	format.setFontColor(QColor("#ffffff"));
	format.setHorizontalAlignment(QXlsx::Format::AlignHCenter);
	
	RichString rs;

	nPosStart = nPosStart + 2;
	switch(mLang)
	{
	case 1:
		rs.addFragment(QString::fromLocal8Bit("声压检测结果(dBC) - "), format);
		format.setFontColor(QColor("#ff0000"));
		rs.addFragment(QString::fromLocal8Bit("DCI要求标准值为85dBC"), format);

		xlsx.currentWorksheet()->writeString(nPosStart, 1, rs, format);
		//xlsx.write(11, 1, QString::fromLocal8Bit("声压检测结果(dBC) - DCI要求标准值为85dBC"), format);
		break;
	default:
		rs.addFragment(QString::fromLocal8Bit("Test result of SPL(dBC) - "), format);
		format.setFontColor(QColor("#ff0000"));
		rs.addFragment(QString::fromLocal8Bit("DCI requierment is 85dBC"), format);

		xlsx.currentWorksheet()->writeString(nPosStart, 1, rs, format);
		//xlsx.write(11, 1, "Test result of SPL(dBC) - DCI requierment is 85dBC", format);
	}
	//xlsx.mergeCells("A11:J11", format);
	xlsx.mergeCells(QXlsx::CellRange(nPosStart, 1, nPosStart, 10), format);
	//header
	format.setFontColor(QColor("#000000"));
	format.setFontBold(false);
	format.setPatternBackgroundColor(QColor("#B7DDE8"));

	format.setHorizontalAlignment(QXlsx::Format::AlignHCenter);
	format.setBorderStyle(Format::BorderThin);
	//format.setBorderColor(QColor("#B7DDE8"));
	format.setBorderColor(QColor("#9BBB59"));
	switch(mLang)
	{
	case 1:
		xlsx.write(12, 1, QString::fromLocal8Bit("影厅"), format);
		xlsx.write(12, 2, QString::fromLocal8Bit("左声道"), format);
		xlsx.write(12, 3, QString::fromLocal8Bit("右声道"), format);
		xlsx.write(12, 4, QString::fromLocal8Bit("中置"), format);
		xlsx.write(12, 5, QString::fromLocal8Bit("重低音"), format);
		xlsx.write(12, 6, QString::fromLocal8Bit("左环"), format);
		xlsx.write(12, 7, QString::fromLocal8Bit("右环"), format);
		xlsx.write(12, 8, QString::fromLocal8Bit("左后环"), format);
		xlsx.write(12, 9, QString::fromLocal8Bit("右后环"), format);
		break;
	default:
		xlsx.write(12, 1, "Hall name", format);
		xlsx.write(12, 2, "Left", format);
		xlsx.write(12, 3, "Right", format);
		xlsx.write(12, 4, "Center", format);
		xlsx.write(12, 5, "LFE", format);
		xlsx.write(12, 6, "LS", format);
		xlsx.write(12, 7, "RS", format);
		xlsx.write(12, 8, "LSS", format);
		xlsx.write(12, 9, "RSS", format);
	}
	switch(mLang)
	{
	case 1:
		xlsx.write(12, 10, QString::fromLocal8Bit("检测时间"), format);
		break;
	default:
		xlsx.write(12, 10, "Test time", format);
	}

	for(int i = 0; i<itemList.size(); i++ )
	{
		if(i%2 == 0)
		{
			format.setPatternBackgroundColor(QColor("#fffff"));
			format.setBorderStyle(QXlsx::Format::BorderThin);
			format.setBorderColor(QColor("#4BACC6"));
		}
		else
		{
			format.setPatternBackgroundColor(QColor("#B7DDE8"));
			format.setBorderStyle(QXlsx::Format::BorderThin);
			format.setBorderColor(QColor("#4BACC6"));
		}

		QString sql = QObject::tr("select cinema_hall, lc, rc, cc, hb, la, ra, stime from tb_dev_test_results where DATEDIFF(stime, CURRENT_DATE()) = 0 and cinema_hall = '%1' order by id desc limit 1;").arg(itemList.at(i).strTheaterNo);
		format.setFontBold(false);
		//qDebug()<<sql;
		QSqlQueryModel *model = pDb->ExecSql(sql);
		if (model == NULL)
		{
			pLog->Write(LOG_REPORT, QObject::tr("Query database error"));
			pLog->Write(LOG_DATABASE, sql);
			return;
		}

		sql = QObject::tr("SELECT\n"
			"	lc AS 'left',\n"
			"	rc AS 'right',\n"
			"	cc AS 'center',\n"
			"	cc AS 'lfe',\n"
			"	la AS 'ls',\n"
			"	ra AS 'rs',\n"
			"	lss,\n"
			"	rss, \n"
			"	s.cinema_hall\n"
			"   FROM\n"
			"	std_test s where cinema_hall = '%1';").arg(itemList.at(i).strTheaterNo); 

		QSqlQueryModel *stdModel = pDb->ExecSql(sql); 

		double std_left = stdModel->data(stdModel->index(0, 0)).toDouble(); 
		double std_right = stdModel->data(stdModel->index(0, 1)).toDouble(); 
		double std_center = stdModel->data(stdModel->index(0, 2)).toDouble(); 
		double std_lfe = stdModel->data(stdModel->index(0, 3)).toDouble(); 
		double std_ls = stdModel->data(stdModel->index(0, 4)).toDouble(); 
		double std_rs = stdModel->data(stdModel->index(0, 5)).toDouble(); 
		double std_lss = stdModel->data(stdModel->index(0, 6)).toDouble(); 
		double std_rss = stdModel->data(stdModel->index(0, 7)).toDouble(); 

		if(model->rowCount() > 0)
		{
			successList<<itemList.at(i).strTheaterNo;
			for(int j = 0; j<model->rowCount(); j++ )
			{
				QString cinema_hall = model->data(model->index(j, 0)).toString(); 
				double left =  model->data(model->index(j, 1)).toDouble(); 
				double right =  model->data(model->index(j, 2)).toDouble();
				double center =  model->data(model->index(j, 3)).toDouble();
				double lfe =  model->data(model->index(j, 4)).toDouble();
				double ls =  model->data(model->index(j,5)).toDouble();
				double rs =  model->data(model->index(j, 6)).toDouble();

				xlsx.write(i+13, 1, cinema_hall , format); //CINEMA HALL

				if(fabs(std_left - left) > STD_VALUE_SPL )
				{format.setFontColor(QColor("#ff0000")); }
				else
				{format.setFontColor(QColor("#000000")); }
				xlsx.write(i+13, 2, model->data(model->index(j, 1)).toString() , format); // Left
				if(fabs(std_right - right) > STD_VALUE_SPL )
				{format.setFontColor(QColor("#ff0000")); }
				else
				{format.setFontColor(QColor("#000000")); }
				xlsx.write(i+13, 3, model->data(model->index(j, 2)).toString(), format); // Right
				if(fabs(std_center - center) > STD_VALUE_SPL )
				{format.setFontColor(QColor("#ff0000")); }
				else
				{format.setFontColor(QColor("#000000")); }
				xlsx.write(i+13, 4, model->data(model->index(j, 3)).toString(), format); // Center
				if(fabs(std_lfe - lfe) > STD_VALUE_SPL )
				{format.setFontColor(QColor("#ff0000")); }
				else
				{format.setFontColor(QColor("#000000")); }
				xlsx.write(i+13, 5, model->data(model->index(j, 4)).toString(), format); // LFE
				if(fabs(std_ls - ls) > STD_VALUE_SPL )
				{format.setFontColor(QColor("#ff0000")); }
				else
				{format.setFontColor(QColor("#000000")); }
				xlsx.write(i+13, 6, model->data(model->index(j, 5)).toString(), format); // LS
				if(fabs(std_rs - rs) > STD_VALUE_SPL )
				{format.setFontColor(QColor("#ff0000")); }
				else
				{format.setFontColor(QColor("#000000")); }
				xlsx.write(i+13, 7, model->data(model->index(j, 6)).toString(), format); // RS
				format.setFontColor(QColor("#ff0000"));
				xlsx.write(i+13, 8, 0, format); //LSS
				xlsx.write(i+13, 9, 0, format); //RSS
				format.setFontColor(QColor("#000000"));
				xlsx.write(i+13, 10, model->data(model->index(j, 7)).toDateTime().toString("hh:mm:ss"), format);
			}
		}
		else
		{
			faileList<<itemList.at(i).strTheaterNo;
			format.setFontColor(QColor("#000"));
			xlsx.write(i+13, 1, itemList.at(i).strTheaterNo, format); //CINEMA HALL
			xlsx.write(i+13, 2, "N/A", format); // Left
			xlsx.write(i+13, 3, "N/A", format); // Right
			xlsx.write(i+13, 4, "N/A", format); // Center
			xlsx.write(i+13, 5, "N/A", format); // LFE
			xlsx.write(i+13, 6, "N/A", format); // LS
			xlsx.write(i+13, 7, "N/A", format); // RS
			xlsx.write(i+13, 8, "N/A", format); //LSS
			xlsx.write(i+13, 9, "N/A", format); //RSS
			xlsx.write(i+13, 10,"N/A", format);
		}
		delete model; 
		model = NULL;
		delete stdModel;
		stdModel = NULL;
	}
	pLog->Write(LOG_REPORT, "setTodaySPL OK.");
}

void Excel::setTodayChroma()
{
	// Test result of Chroma
	// 色
	nPosStart = nPosStart + itemList.size() + 3;

	format.setPatternBackgroundColor("#4BACC6");
	format.setFontBold(false);
	format.setFontName("Book Antiqua");
	format.setFontSize(12);
	format.setFontColor(QColor("#ffffff"));
	format.setHorizontalAlignment(QXlsx::Format::AlignHCenter);
	format.setBorderColor(QColor("#4BACC6"));
	RichString rs;
	nPosStart = nPosStart + 1;
	switch(mLang)
	{
	case 1:
		rs.addFragment(QString::fromLocal8Bit("色度检测结果 - "), format);
		format.setFontColor(QColor("#ff0000"));
		rs.addFragment(QString::fromLocal8Bit("括号内为DCI要求标准值"), format);
	
		//xlsx.write(21, 1, QString::fromLocal8Bit("色度检测结果 - 括号内为DCI要求标准值"), format);
		xlsx.currentWorksheet()->writeString(nPosStart, 1, rs, format);
		break;
	default:
		rs.addFragment(QString::fromLocal8Bit("Test result of Chroma - "), format);
		format.setFontColor(QColor("#ff0000"));
		rs.addFragment(QString::fromLocal8Bit("The numbers in brackets are DCI requirement"), format);

		xlsx.currentWorksheet()->writeString(nPosStart, 1, rs, format);
// 		xlsx.write(21, 1, "Test result of Chroma", format);
	}
	xlsx.mergeCells(QXlsx::CellRange(nPosStart, 1, nPosStart,10), format);
//	xlsx.mergeCells("A21:J21", format);
	//header
	format.setFontColor(QColor("#000000"));
	format.setFontBold(false);
	format.setHorizontalAlignment(QXlsx::Format::AlignHCenter);
	format.setPatternBackgroundColor(QColor("#B7DDE8"));
	nPosStart++;  //nPosStart = 22;
	switch(mLang)
	{
	case 1:

		xlsx.write(nPosStart, 1, "", format);
		xlsx.write(nPosStart, 2, QString::fromLocal8Bit("白光(0.314, 0.351)"), format);
		xlsx.mergeCells(QXlsx::CellRange(nPosStart, 2, nPosStart, 3), format);
		/*xlsx.mergeCells("B22:C22", format);*/

		xlsx.write(nPosStart, 4, QString::fromLocal8Bit("红光(0.680, 0.320)"), format);
		xlsx.mergeCells(QXlsx::CellRange(nPosStart, 4, nPosStart, 5), format); 
		/*xlsx.mergeCells("D22:E22", format);*/


		xlsx.write(nPosStart, 6, QString::fromLocal8Bit("绿光(0.265, 0.690)"), format );
		xlsx.mergeCells(QXlsx::CellRange(nPosStart, 6, nPosStart, 7), format); 
		/*xlsx.mergeCells("F22:G22", format);*/

		xlsx.write(nPosStart, 8, QString::fromLocal8Bit("蓝光(0.150, 0.060)"), format);
		xlsx.mergeCells(QXlsx::CellRange(nPosStart, 8, nPosStart, 9), format); 
		/*xlsx.mergeCells("H22:I22", format);*/

		xlsx.write(nPosStart, 10, "", format);
		nPosStart++;  //nPosStart = 23; 
		xlsx.write(nPosStart, 1, QString::fromLocal8Bit("影厅"), format);
		xlsx.write(nPosStart, 2, "x", format);

		xlsx.write(nPosStart, 3, "y", format);
		xlsx.write(nPosStart, 4, "x", format);
		xlsx.write(nPosStart, 5, "y", format);
		xlsx.write(nPosStart, 6, "x", format);
		xlsx.write(nPosStart, 7, "y", format);
		xlsx.write(nPosStart, 8, "x", format);
		xlsx.write(nPosStart, 9, "y", format);
		xlsx.write(nPosStart, 10, QString::fromLocal8Bit("检测时间"), format);
		break;
	default:
		xlsx.write(nPosStart, 1, "", format);
		xlsx.write(nPosStart, 2, "WHITE(0.314, 0.351)", format);
		xlsx.mergeCells(QXlsx::CellRange(nPosStart, 2, nPosStart, 3), format);
		/*xlsx.mergeCells("B22:C22", format);*/

		xlsx.write(nPosStart, 4, "RED(0.680, 0.320)", format);
		xlsx.mergeCells(QXlsx::CellRange(nPosStart, 4, nPosStart, 5), format);
		/*xlsx.mergeCells("D22:E22", format);*/


		xlsx.write(nPosStart, 6, "GREEN(0.265, 0.690)", format );
		xlsx.mergeCells(QXlsx::CellRange(nPosStart, 6, nPosStart, 7), format);
		/*xlsx.mergeCells("F22:G22", format);*/

		xlsx.write(nPosStart, 8, "BLUE(0.150, 0.060)", format);
		xlsx.mergeCells(QXlsx::CellRange(nPosStart, 8, nPosStart, 9), format);
		/*xlsx.mergeCells("H22:I22", format);*/

		xlsx.write(nPosStart, 10, "", format);
		nPosStart++; // nPosStart = 23; 
		xlsx.write(nPosStart, 1, "Hall name", format);
		xlsx.write(nPosStart, 2, "x", format);

		xlsx.write(nPosStart, 3, "y", format);
		xlsx.write(nPosStart, 4, "x", format);
		xlsx.write(nPosStart, 5, "y", format);
		xlsx.write(nPosStart, 6, "x", format);
		xlsx.write(nPosStart, 7, "y", format);
		xlsx.write(nPosStart, 8, "x", format);
		xlsx.write(nPosStart, 9, "y", format);
		xlsx.write(nPosStart, 10, "Test time", format);	}

	format.setFontBold(false);
	int nTmp =  nPosStart;
	for(int i = 0; i<itemList.size(); i++ )
	{
		if(i%2 == 0)
		{
			format.setPatternBackgroundColor(QColor("#fffff"));
			format.setBorderStyle(QXlsx::Format::BorderThin);
			format.setBorderColor(QColor("#4BACC6"));
		}
		else
		{
			format.setPatternBackgroundColor(QColor("#B7DDE8"));
			format.setBorderStyle(QXlsx::Format::BorderThin);
			format.setBorderColor(QColor("#4BACC6"));

		}

		QString sql = QObject::tr(" SELECT"
			" t.cinema_hall,"
			" t.wx,"
			" t.wy,"
			" t.rx,"
			" t.ry,"
			" t.gx,"
			" t.gy,"
			" t.bluex,"
			" t.bluey,"
			" t.stime"
			" FROM"
			" tb_dev_test_results t"
			" WHERE"
			" DATEDIFF(stime, CURRENT_DATE()) = 0 and"
			" cinema_hall = '%1'"
			" ORDER BY"
			" id DESC"
			" LIMIT 1;").arg(itemList.at(i).strTheaterNo);
		QSqlQueryModel *model = pDb->ExecSql(sql);
		if (model == NULL)
		{
			pLog->Write(LOG_REPORT, QObject::tr("Query database error"));
			pLog->Write(LOG_DATABASE, sql);
			return;
		}


		sql = QObject::tr("SELECT\n"
			"	wx,\n"
			"	wy,\n"
			"	rx,\n"
			"	ry,\n"
			"	gx,\n"
			"	gy,\n"
			"	bluex,\n"
			"	bluey\n"
			"   FROM\n"
			"	std_test\n"
			"   WHERE\n"
			"	cinema_hall = '%1';").arg(itemList.at(i).strTheaterNo);
		QSqlQueryModel *stdModel = pDb->ExecSql(sql); 
		if(stdModel == NULL)
		{
			pLog->Write(LOG_REPORT, QObject::tr("Query database error"));
			pLog->Write(LOG_DATABASE, sql);
			return;
		}
		 

		double std_wx = stdModel->data(stdModel->index(0, 0)).toDouble(); 
		double std_wy = stdModel->data(stdModel->index(0, 1)).toDouble(); 
		double std_rx = stdModel->data(stdModel->index(0, 2)).toDouble(); 
		double std_ry = stdModel->data(stdModel->index(0, 3)).toDouble(); 
		double std_gx = stdModel->data(stdModel->index(0, 4)).toDouble(); 
		double std_gy = stdModel->data(stdModel->index(0, 5)).toDouble(); 
		double std_bx = stdModel->data(stdModel->index(0, 6)).toDouble(); 
		double std_by = stdModel->data(stdModel->index(0, 7)).toDouble(); 
		nPosStart = nTmp + i + 1; 
		if (model->rowCount()>0)
		{
			for(int j = 0; j<model->rowCount(); j++ )
			{
				double wx = model->data(model->index(j, 1)).toDouble(); 
				double wy = model->data(model->index(j, 2)).toDouble(); 
				double rx = model->data(model->index(j, 3)).toDouble(); 
				double ry = model->data(model->index(j, 4)).toDouble(); 
				double gx = model->data(model->index(j, 5)).toDouble(); 
				double gy = model->data(model->index(j, 6)).toDouble(); 
				double bx = model->data(model->index(j, 7)).toDouble(); 
				double by = model->data(model->index(j, 8)).toDouble(); 

				xlsx.write(nPosStart, 1, model->data(model->index(j, 0)).toString(), format); //CINEMA HALL

				if(fabs(std_wx - wx) > STD_VALUE_CHROMA )											// wx
				{format.setFontColor(QColor("#ff0000"));}
				else
				{format.setFontColor(QColor("#000000"));}
				xlsx.write(nPosStart, 2, model->data(model->index(j, 1)).toString(), format); 

				if(fabs(std_wy - wy) > STD_VALUE_CHROMA )											 // wy
				{format.setFontColor(QColor("#ff0000"));}
				else
				{format.setFontColor(QColor("#000000"));}			
				xlsx.write(nPosStart, 3, model->data(model->index(j, 2)).toString(), format); 

				if(fabs(std_rx - rx) > STD_VALUE_CHROMA )											// rx
				{format.setFontColor(QColor("#ff0000"));}
				else
				{format.setFontColor(QColor("#000000"));}
				xlsx.write(nPosStart, 4, model->data(model->index(j, 3)).toString(), format);  

				if(fabs(std_ry - ry) > STD_VALUE_CHROMA )										   // ry
				{format.setFontColor(QColor("#ff0000"));}
				else
				{format.setFontColor(QColor("#000000"));}
				xlsx.write(nPosStart, 5, model->data(model->index(j, 4)).toString(), format);  

				if(fabs(std_gx - gx) > STD_VALUE_CHROMA )											// gx
				{format.setFontColor(QColor("#ff0000"));}
				else
				{format.setFontColor(QColor("#000000"));}
				xlsx.write(nPosStart, 6, model->data(model->index(j, 5)).toString(), format); 

				if(fabs(std_gy - gy) > STD_VALUE_CHROMA )											// gy									
				{format.setFontColor(QColor("#ff0000"));}
				else
				{format.setFontColor(QColor("#000000"));}
				xlsx.write(nPosStart, 7, model->data(model->index(j, 6)).toString(), format); 

				if(fabs(std_bx - bx) > STD_VALUE_CHROMA )											//bx
				{format.setFontColor(QColor("#ff0000"));}
				else
				{format.setFontColor(QColor("#000000"));}
				xlsx.write(nPosStart, 8, model->data(model->index(j, 7)).toString(), format);  

				if(fabs(std_by - by) > STD_VALUE_CHROMA )											// by
				{format.setFontColor(QColor("#ff0000"));}
				else
				{format.setFontColor(QColor("#000000"));}
				xlsx.write(nPosStart, 9, model->data(model->index(j, 8)).toString(), format); 
				format.setFontColor(QColor("#000000"));
				xlsx.write(nPosStart, 10, model->data(model->index(j, 9)).toDateTime().toString("hh:mm:ss"), format); //stime
			}
		}
		else
		{
			format.setFontColor(QColor("#000000")); 
			xlsx.write(nPosStart, 1, itemList.at(i).strTheaterNo, format); //CINEMA HALL
			xlsx.write(nPosStart, 2, "N/A", format); // wx
			xlsx.write(nPosStart, 3, "N/A", format); // wy
			xlsx.write(nPosStart, 4, "N/A", format); // rx
			xlsx.write(nPosStart, 5, "N/A", format); // ry
			xlsx.write(nPosStart, 6, "N/A", format); // gx
			xlsx.write(nPosStart, 7, "N/A", format); // gy
			xlsx.write(nPosStart, 8, "N/A", format); //bx
			xlsx.write(nPosStart, 9, "N/A", format); //by
			xlsx.write(nPosStart, 10, "N/A", format); //stime
		}
		delete model;
		model = NULL;
		delete stdModel; 
		stdModel = NULL;
	}
	pLog->Write(LOG_REPORT, "setTodayChroma OK.");
}

void Excel::setTodayLuminance()
{
	// Test result of Luminance
	nPosStart = nPosStart + 3;  // nPosStart = 32
	format.setPatternBackgroundColor("#4BACC6");
	format.setFontBold(false);
	format.setFontName("Book Antiqua");
	format.setFontSize(12);
	format.setFontColor(QColor("#ffffff"));
	format.setHorizontalAlignment(QXlsx::Format::AlignHCenter);
	format.setBorderColor(QColor("#4BACC6"));
	RichString rs;
	switch(mLang)
	{
	case 1:
		rs.addFragment(QString::fromLocal8Bit("亮度检测结果(单位为ftL) - "), format);
		format.setFontColor(QColor("#ff0000"));
		rs.addFragment(QString::fromLocal8Bit("括号内为DCI要求标准值"), format);

		xlsx.currentWorksheet()->writeString(nPosStart, 1, rs, format);
		//xlsx.write(32, 1, QString::fromLocal8Bit("亮度检测结果 - 括号内为DCI要求标准值"), format);
		break;
	default:
		rs.addFragment(QString::fromLocal8Bit("Test result of Luminance(ftL) - "), format);
		format.setFontColor(QColor("#ff0000"));
		rs.addFragment(QString::fromLocal8Bit("The number in brackets is DCI requirement"), format);

		xlsx.currentWorksheet()->writeString(nPosStart, 1, rs, format);
// 		xlsx.write(32, 1, "Test result of Luminance(ftL)", format);
	}

	xlsx.mergeCells(QXlsx::CellRange(nPosStart, 1, nPosStart, 10), format);
	/*xlsx.mergeCells("A32:J32", format);*/
	//header
	format.setFontColor(QColor("#000000"));
	format.setFontBold(false);
	format.setHorizontalAlignment(QXlsx::Format::AlignHCenter);
	format.setPatternBackgroundColor(QColor("#B7DDE8"));
	nPosStart++; //nPosStart = 33;
	switch(mLang)
	{
	case 1:
		xlsx.write(nPosStart, 1, QString::fromLocal8Bit("影厅"), format);
		xlsx.write(nPosStart, 2, QString::fromLocal8Bit("白光(14ftL)"), format);
		xlsx.mergeCells(QXlsx::CellRange(nPosStart, 2, nPosStart, 3 ), format); 
		/*xlsx.mergeCells("B33:C33", format);*/

		xlsx.write(nPosStart, 4, QString::fromLocal8Bit("红光(2.95ftL)"), format);
		xlsx.mergeCells(QXlsx::CellRange(nPosStart, 4, nPosStart, 5 ), format); 
		/*xlsx.mergeCells("D33:E33", format);*/

		xlsx.write(nPosStart, 6, QString::fromLocal8Bit("绿光(10.11ftL)"), format);
		xlsx.mergeCells(QXlsx::CellRange(nPosStart, 6, nPosStart, 7 ), format); 
		/*xlsx.mergeCells("F33:G33", format);*/

		xlsx.write(nPosStart, 8, QString::fromLocal8Bit("蓝光(0.97ftL)"), format);
		xlsx.mergeCells(QXlsx::CellRange(nPosStart, 8, nPosStart, 9 ), format); 
		/*xlsx.mergeCells("H33:I33", format);*/

		xlsx.write(nPosStart, 10, QString::fromLocal8Bit("检测时间"), format);
		break;
	default:
		xlsx.write(nPosStart, 1, "Hall name", format);
		xlsx.write(nPosStart, 2, "WHITE(14ftL)", format);
		xlsx.mergeCells(QXlsx::CellRange(nPosStart, 2, nPosStart, 3 ), format); 
		/*xlsx.mergeCells("B33:C33", format);*/

		xlsx.write(nPosStart, 4, "RED(2.95ftL)", format);
		xlsx.mergeCells(QXlsx::CellRange(nPosStart, 4, nPosStart, 5 ), format); 
		/*xlsx.mergeCells("D33:E33", format);*/

		xlsx.write(nPosStart, 6, "GREEN(10.11ftL)", format);
		xlsx.mergeCells(QXlsx::CellRange(nPosStart, 6, nPosStart, 7 ), format); 
		/*xlsx.mergeCells("F33:G33", format);*/

		xlsx.write(nPosStart, 8, "BLUE(0.97ftL)", format);
		xlsx.mergeCells(QXlsx::CellRange(nPosStart, 8, nPosStart, 9 ), format); 
		/*xlsx.mergeCells("H33:I33", format);*/

		xlsx.write(nPosStart, 10, "Test time", format);
	}
	format.setFontBold(false);
	int nTmp =  nPosStart;
	for(int i = 0; i<itemList.size(); i++ )
	{

		if(i%2 == 0)
		{
			format.setPatternBackgroundColor(QColor("#fffff"));
			format.setBorderStyle(QXlsx::Format::BorderThin);
			format.setBorderColor(QColor("#4BACC6"));
		}
		else
		{
			format.setPatternBackgroundColor(QColor("#B7DDE8"));
			format.setBorderStyle(QXlsx::Format::BorderThin);
			format.setBorderColor(QColor("#4BACC6"));

		}
		QString sql = QObject::tr(" SELECT"
			" t.cinema_hall,"
			" t.white_ftl,"
			" t.red_ftl,"
			" t.green_ftl,"
			" t.blue_ftl,"
			" t.stime"
			" FROM"
			" tb_dev_test_results t"
			" WHERE"
			" DATEDIFF(stime, CURRENT_DATE()) = 0 and"
			" cinema_hall = '%1'"
			" ORDER BY"
			" id DESC"
			" LIMIT 1;").arg(itemList.at(i).strTheaterNo);

		QSqlQueryModel *model = pDb->ExecSql(sql);
		if (model == NULL)
		{
			pLog->Write(LOG_REPORT, QObject::tr("Query database error"));
			pLog->Write(LOG_DATABASE, sql);
			return;
		}

		sql = QObject::tr("SELECT\n"
			"	white_ftl,\n"
			"	red_ftl,\n"
			"	green_ftl,\n"
			"	blue_ftl\n"
			"   FROM\n"
			"	std_test\n"
			"   WHERE\n"
			"	cinema_hall = '%1';").arg(itemList.at(i).strTheaterNo); 

		QSqlQueryModel *stdModel = pDb->ExecSql(sql); 
		if (stdModel == NULL)
		{
			pLog->Write(LOG_REPORT, QObject::tr("Query database error"));
			pLog->Write(LOG_DATABASE, sql);
			return;
		}
		double std_white_ftl = stdModel->data(stdModel->index(0, 0)).toDouble(); 
		double std_red_ftl = stdModel->data(stdModel->index(0, 1)).toDouble(); 
		double std_green_ftl = stdModel->data(stdModel->index(0, 2)).toDouble(); 
		double std_blue_ftl = stdModel->data(stdModel->index(0, 3)).toDouble(); 
		
		nPosStart =  nTmp + i + 1; 
		if(model->rowCount() > 0)
		{
			for(int j = 0; j<model->rowCount(); j++ )
			{
				double white_ftl = model->data(model->index(j, 1)).toDouble(); 
				double red_ftl = model->data(model->index(j, 2)).toDouble(); 
				double green_ftl = model->data(model->index(j, 3)).toDouble(); 
				double blue_ftl = model->data(model->index(j, 4)).toDouble(); 

				xlsx.write(nPosStart, 1, model->data(model->index(j, 0)).toString(), format); //CINEMA HALL

				if(fabs(std_white_ftl - white_ftl)>3)
				{format.setFontColor(QColor("#ff0000"));}
				else 
				{format.setFontColor(QColor("#000000"));}
				xlsx.write(nPosStart, 2, model->data(model->index(j, 1)).toString(), format); // white ftl
				xlsx.mergeCells(QXlsx::CellRange(nPosStart, 2, nPosStart, 3), format);

				if(fabs(std_red_ftl - red_ftl)>3)
				{format.setFontColor(QColor("#ff0000"));}
				else 
				{format.setFontColor(QColor("#000000"));}
				xlsx.write(nPosStart, 4, model->data(model->index(j, 2)).toString(), format); // red ftl
				xlsx.mergeCells(QXlsx::CellRange(nPosStart, 4, nPosStart, 5), format);

				if(fabs(std_green_ftl - green_ftl)>3)
				{format.setFontColor(QColor("#ff0000"));}
				else 
				{format.setFontColor(QColor("#000000"));}
				xlsx.write(nPosStart, 6, model->data(model->index(j, 3)).toString(), format); // green ftl
				xlsx.mergeCells(QXlsx::CellRange(nPosStart, 6, nPosStart, 7), format);

				if(fabs(std_blue_ftl - blue_ftl)>3)
				{format.setFontColor(QColor("#ff0000"));}
				else 
				{format.setFontColor(QColor("#000000"));}
				xlsx.write(nPosStart, 8, model->data(model->index(j, 4)).toString(), format); // blue ftl
				xlsx.mergeCells(QXlsx::CellRange(nPosStart, 8, nPosStart, 9), format);
				format.setFontColor(QColor("#000000"));
				xlsx.write(nPosStart, 10, model->data(model->index(j, 5)).toDateTime().toString("hh:mm:ss"), format); // stime
			}
		}
		else
		{
			format.setFontColor(QColor("#000000"));
			xlsx.write(nPosStart, 1, itemList.at(i).strTheaterNo, format); //CINEMA HALL

			xlsx.write(nPosStart, 2, "N/A", format); // white ftl
			xlsx.mergeCells(QXlsx::CellRange(nPosStart, 2, nPosStart, 3), format);

			xlsx.write(nPosStart, 4, "N/A", format); // red ftl
			xlsx.mergeCells(QXlsx::CellRange(nPosStart, 4, nPosStart, 5), format);

			xlsx.write(nPosStart, 6, "N/A", format); // green ftl
			xlsx.mergeCells(QXlsx::CellRange(nPosStart, 6, nPosStart, 7), format);

			xlsx.write(nPosStart, 8, "N/A", format); // blue ftl
			xlsx.mergeCells(QXlsx::CellRange(nPosStart, 8, nPosStart, 9), format);
			//
			xlsx.write(nPosStart, 10,"N/A", format); // stime
		}
		delete model;
		model = NULL;
		delete stdModel;
		stdModel = NULL;
	}
	pLog->Write(LOG_REPORT, "setTodayLuminance OK.");
}

void Excel::setTodayLuminanceCDM()
{
	// Test result of Luminance
	nPosStart = nPosStart + 3;
	format.setPatternBackgroundColor("#4BACC6");
	format.setFontBold(false);
	format.setFontName("Book Antiqua");
	format.setFontSize(12);
	format.setFontColor(QColor("#ffffff"));
	format.setHorizontalAlignment(QXlsx::Format::AlignHCenter);
	format.setBorderColor(QColor("#4BACC6"));
	RichString rs;
	switch(mLang)
	{
	case 1:
		rs.addFragment(QString::fromLocal8Bit("亮度检测结果(单位为cd/m2) - "), format);
		format.setFontColor(QColor("#ff0000"));
		rs.addFragment(QString::fromLocal8Bit("括号内为DCI要求标准值"), format);

		xlsx.currentWorksheet()->writeString(nPosStart, 1, rs, format);
		break;
	default:
		rs.addFragment(QString::fromLocal8Bit("Test result of Luminance(cd/m2) - "), format);
		format.setFontColor(QColor("#ff0000"));
		rs.addFragment(QString::fromLocal8Bit("The number in brackets is DCI requirement"), format);
		xlsx.currentWorksheet()->writeString(nPosStart, 1, rs, format);
// 		xlsx.write(42, 1, "Test result of Luminance(cd/m2)", format);
	}

	xlsx.mergeCells(QXlsx::CellRange(nPosStart, 1, nPosStart, 10), format); 
	/*xlsx.mergeCells("A42:J42", format);*/
	//header
	format.setFontColor(QColor("#000000"));
	format.setFontBold(false);
	format.setHorizontalAlignment(QXlsx::Format::AlignHCenter);
	format.setPatternBackgroundColor(QColor("#B7DDE8"));
	nPosStart++; 
	switch(mLang)
	{
	case 1:
		xlsx.write(nPosStart, 1, QString::fromLocal8Bit("影厅"), format);
		xlsx.write(nPosStart, 2, QString::fromLocal8Bit("白光(48cd/m2)"), format);
		xlsx.mergeCells(QXlsx::CellRange(nPosStart, 2, nPosStart, 3), format); 

		xlsx.write(nPosStart, 4, QString::fromLocal8Bit("红光(10.06cd/m2)"), format);
		xlsx.mergeCells(QXlsx::CellRange(nPosStart, 4, nPosStart, 5), format); 

		xlsx.write(nPosStart, 6, QString::fromLocal8Bit("绿光(34.64cd/m2)"), format);
		xlsx.mergeCells(QXlsx::CellRange(nPosStart, 6, nPosStart, 7), format); 

		xlsx.write(nPosStart, 8, QString::fromLocal8Bit("蓝光(3.31cd/m2)"), format);
		xlsx.mergeCells(QXlsx::CellRange(nPosStart, 8, nPosStart, 9), format); 

		xlsx.write(nPosStart, 10, QString::fromLocal8Bit("检测时间"), format);
		break;
	default:
		xlsx.write(nPosStart, 1, "Hall name", format);
		xlsx.write(nPosStart, 2, "WHITE(48cd/m2)", format);
		xlsx.mergeCells(QXlsx::CellRange(nPosStart, 2, nPosStart, 3), format); 
		/*xlsx.mergeCells("B43:C43", format);*/

		xlsx.write(nPosStart, 4, "RED(10.06cd/m2)", format);
		xlsx.mergeCells(QXlsx::CellRange(nPosStart, 4, nPosStart, 5), format); 
		/*xlsx.mergeCells("D43:E43", format);*/

		xlsx.write(nPosStart, 6, "GREEN(34.64cd/m2)", format);
		xlsx.mergeCells(QXlsx::CellRange(nPosStart, 6, nPosStart, 7), format); 
		/*xlsx.mergeCells("F43:G43", format);*/

		xlsx.write(nPosStart, 8, "BLUE(3.31cd/m2)", format);
		xlsx.mergeCells(QXlsx::CellRange(nPosStart, 8, nPosStart, 9), format); 
// 		xlsx.mergeCells("H43:I43", format);

		xlsx.write(nPosStart, 10, "Test time", format);
	}
	format.setFontBold(false);
	int nTmp = nPosStart; 
	for(int i = 0; i<itemList.size(); i++ )
	{

		if(i%2 == 0)
		{
			format.setPatternBackgroundColor(QColor("#fffff"));
			format.setBorderStyle(QXlsx::Format::BorderThin);
			format.setBorderColor(QColor("#4BACC6"));
		}
		else
		{
			format.setPatternBackgroundColor(QColor("#B7DDE8"));
			format.setBorderStyle(QXlsx::Format::BorderThin);
			format.setBorderColor(QColor("#4BACC6"));

		}
		QString sql = QObject::tr(" SELECT"
			" t.cinema_hall,"
			" t.white_ftl,"
			" t.red_ftl,"
			" t.green_ftl,"
			" t.blue_ftl,"
			" t.stime"
			" FROM"
			" tb_dev_test_results t"
			" WHERE"
			" DATEDIFF(stime, CURRENT_DATE()) = 0 and"
			" cinema_hall = '%1'"
			" ORDER BY"
			" id DESC"
			" LIMIT 1;").arg(itemList.at(i).strTheaterNo);

		QSqlQueryModel *model = pDb->ExecSql(sql);
		if (model == NULL)
		{
			pLog->Write(LOG_REPORT, QObject::tr("Query database error"));
			pLog->Write(LOG_DATABASE, sql);
			return;
		}

		sql = QObject::tr("SELECT\n"
			"	white_ftl,\n"
			"	red_ftl,\n"
			"	green_ftl,\n"
			"	blue_ftl\n"
			"   FROM\n"
			"	std_test\n"
			"   WHERE\n"
			"	cinema_hall = '%1';").arg(itemList.at(i).strTheaterNo); 

		QSqlQueryModel *stdModel = pDb->ExecSql(sql); 
		if (stdModel == NULL)
		{
			pLog->Write(LOG_REPORT, QObject::tr("Query database error"));
			pLog->Write(LOG_DATABASE, sql);
			return;
		}
		double std_white_ftl = stdModel->data(stdModel->index(0, 0)).toDouble(); 
		double std_red_ftl = stdModel->data(stdModel->index(0, 1)).toDouble(); 
		double std_green_ftl = stdModel->data(stdModel->index(0, 2)).toDouble(); 
		double std_blue_ftl = stdModel->data(stdModel->index(0, 3)).toDouble(); 

		nPosStart = nTmp + i + 1; 
		if(model->rowCount() > 0)
		{
			for(int j = 0; j<model->rowCount(); j++ )
			{
				double white_ftl = model->data(model->index(j, 1)).toDouble(); 
				double red_ftl = model->data(model->index(j, 2)).toDouble(); 
				double green_ftl = model->data(model->index(j, 3)).toDouble(); 
				double blue_ftl = model->data(model->index(j, 4)).toDouble(); 

				xlsx.write(nPosStart, 1, model->data(model->index(j, 0)).toString(), format); //CINEMA HALL

				if(fabs(std_white_ftl - white_ftl)>3)
				{format.setFontColor(QColor("#ff0000"));}
				else 
				{format.setFontColor(QColor("#000000"));}
				xlsx.write(nPosStart, 2, QString::number(model->data(model->index(j, 1)).toFloat() * 3.183/0.929), format); // white ftl
				xlsx.mergeCells(QXlsx::CellRange(nPosStart, 2, nPosStart, 3), format);

				if(fabs(std_red_ftl - red_ftl)>3)
				{format.setFontColor(QColor("#ff0000"));}
				else 
				{format.setFontColor(QColor("#000000"));}
				xlsx.write(nPosStart, 4, QString::number(model->data(model->index(j, 2)).toFloat() * 3.183/0.929), format); // red ftl
				xlsx.mergeCells(QXlsx::CellRange(nPosStart, 4, nPosStart, 5), format);

				if(fabs(std_green_ftl - green_ftl)>3)
				{format.setFontColor(QColor("#ff0000"));}
				else 
				{format.setFontColor(QColor("#000000"));}
				xlsx.write(nPosStart, 6, QString::number(model->data(model->index(j, 3)).toFloat() * 3.183/0.929), format); // green ftl
				xlsx.mergeCells(QXlsx::CellRange(nPosStart, 6, nPosStart, 7), format);

				if(fabs(std_blue_ftl - blue_ftl)>3)
				{format.setFontColor(QColor("#ff0000"));}
				else 
				{format.setFontColor(QColor("#000000"));}
				xlsx.write(nPosStart, 8, QString::number(model->data(model->index(j, 4)).toFloat() * 3.183/0.929), format); // blue ftl
				xlsx.mergeCells(QXlsx::CellRange(nPosStart, 8, nPosStart, 9), format);
				format.setFontColor(QColor("#000000"));
				xlsx.write(nPosStart, 10, model->data(model->index(j, 5)).toDateTime().toString("hh:mm:ss"), format); // stime
			}
		}
		else
		{
			format.setFontColor(QColor("#000000"));
			xlsx.write(nPosStart, 1, itemList.at(i).strTheaterNo, format); //CINEMA HALL

			xlsx.write(nPosStart, 2, "N/A", format); // white ftl
			xlsx.mergeCells(QXlsx::CellRange(nPosStart, 2, nPosStart, 3), format);

			xlsx.write(nPosStart, 4, "N/A", format); // red ftl
			xlsx.mergeCells(QXlsx::CellRange(nPosStart, 4, nPosStart, 5), format);

			xlsx.write(nPosStart, 6, "N/A", format); // green ftl
			xlsx.mergeCells(QXlsx::CellRange(nPosStart, 6, nPosStart, 7), format);

			xlsx.write(nPosStart, 8, "N/A", format); // blue ftl
			xlsx.mergeCells(QXlsx::CellRange(nPosStart, 8, nPosStart, 9), format);
			//
			xlsx.write(nPosStart, 10,"N/A", format); // stime
		}
		delete model;
		model = NULL;
		delete stdModel;
		stdModel = NULL;
	}
	pLog->Write(LOG_REPORT, "setTodayLuminance OK.");
}

void Excel::setLastChroma()
{
	//  最近七天色域
	nPosStart = nPosStart + 2; 
	int nTmp = nPosStart; // nPosStart = 54
	for(int i = 0; i<itemList.size(); i++ )
	{
		//header
		format.setPatternBackgroundColor("#8064A2");
		format.setFontBold(false);
		format.setFontName("Book Antiqua");
		format.setFontSize(12);
		format.setFontColor(QColor("#ffffff"));
		format.setHorizontalAlignment(QXlsx::Format::AlignHCenter);
		format.setBorderColor(QColor("#8064A2"));
		RichString rs;
		nPosStart = nTmp + (7+4)*i + 1 ;  
		switch(mLang)
		{
		case 1:
			rs.addFragment(QString::fromLocal8Bit("色度检测结果 - "), format);
			format.setFontColor(QColor("#ff0000"));
			rs.addFragment(QString::fromLocal8Bit("括号内为DCI要求标准值"), format);

			xlsx.currentWorksheet()->writeString(nPosStart, 1, rs, format);
// 			xlsx.write(118+i*11, 1, QString::fromLocal8Bit("色度检测结果 - 括号内为DCI要求标准值"), format);
			break;
		default:
			rs.addFragment(QString::fromLocal8Bit("Test result of Chroma - "), format);
			format.setFontColor(QColor("#ff0000"));
			rs.addFragment(QString::fromLocal8Bit("The numbers in brackets are DCI requirement"), format);
			xlsx.currentWorksheet()->writeString(nPosStart, 1, rs, format);
// 			xlsx.write(125+i*11, 1, "Test result of Chroma", format);
		}
		xlsx.mergeCells(QXlsx::CellRange(/*125+i*11*/nPosStart, 1, /*125+i*11*/nPosStart, 10), format);
		format.setFontColor(QColor("#000000"));
		format.setFontBold(false);
		format.setHorizontalAlignment(QXlsx::Format::AlignHCenter);
		format.setPatternBackgroundColor(QColor("#CCC1D9"));
		nPosStart++;
		switch(mLang)
		{
		case 1:
			xlsx.write(nPosStart, 1, "", format);
			xlsx.write(nPosStart, 2, QString::fromLocal8Bit("白光(0.314, 0.351)"), format);
			xlsx.mergeCells(QXlsx::CellRange(nPosStart, 2, nPosStart, 3), format);
			xlsx.write(nPosStart, 4, QString::fromLocal8Bit("红光(0.680, 0.320)"), format);
			xlsx.mergeCells(QXlsx::CellRange(nPosStart, 4, nPosStart, 5), format);

			xlsx.write(nPosStart, 6, QString::fromLocal8Bit("绿光(0.265, 0.690)"), format );
			xlsx.mergeCells(QXlsx::CellRange(nPosStart, 6, nPosStart, 7), format);

			xlsx.write(nPosStart, 8, QString::fromLocal8Bit("蓝光(0.150, 0.060)"), format);
			xlsx.mergeCells(QXlsx::CellRange(nPosStart, 8, nPosStart, 9), format);

			xlsx.write(nPosStart, 10, "", format);
			nPosStart++;
			xlsx.write(nPosStart, 1, QString::fromLocal8Bit("影厅"), format);
			xlsx.write(nPosStart, 2, "x", format);
			xlsx.write(nPosStart, 3, "y", format);
			xlsx.write(nPosStart, 4, "x", format);
			xlsx.write(nPosStart, 5, "y", format);
			xlsx.write(nPosStart, 6, "x", format);
			xlsx.write(nPosStart, 7, "y", format);
			xlsx.write(nPosStart, 8, "x", format);
			xlsx.write(nPosStart, 9, "y", format);
			xlsx.write(nPosStart, 10, QString::fromLocal8Bit("检测日期"), format);
			break;
		default:
		xlsx.write(nPosStart, 1, "", format);
		xlsx.write(nPosStart, 2, "WHITE(0.314, 0.351)", format);
		xlsx.mergeCells(QXlsx::CellRange(nPosStart, 2, nPosStart, 3), format);
		xlsx.write(nPosStart, 4, "RED(0.680, 0.320)", format);
		xlsx.mergeCells(QXlsx::CellRange(nPosStart, 4, nPosStart, 5), format);

		xlsx.write(nPosStart, 6, "GREEN(0.265, 0.690)", format );
		xlsx.mergeCells(QXlsx::CellRange(nPosStart, 6, nPosStart, 7), format);

		xlsx.write(nPosStart, 8, "BLUE(0.150, 0.060)", format);
		xlsx.mergeCells(QXlsx::CellRange(nPosStart, 8, nPosStart, 9), format);

		xlsx.write(nPosStart, 10, "", format);
		nPosStart++;
		xlsx.write(nPosStart, 1, "Hall name", format);
		xlsx.write(nPosStart, 2, "x", format);
		xlsx.write(nPosStart, 3, "y", format);
		xlsx.write(nPosStart, 4, "x", format);
		xlsx.write(nPosStart, 5, "y", format);
		xlsx.write(nPosStart, 6, "x", format);
		xlsx.write(nPosStart, 7, "y", format);
		xlsx.write(nPosStart, 8, "x", format);
		xlsx.write(nPosStart, 9, "y", format);
		xlsx.write(nPosStart, 10, "Test date", format);		}

		format.setFontBold(false);
		for(int j = 0; j<7; j++ )
		{
			QString sql = QObject::tr(" SELECT"
				" t.cinema_hall,"
				" t.wx,"
				" t.wy,"
				" t.rx,"
				" t.ry,"
				" t.gx,"
				" t.gy,"
				" t.bluex,"
				" t.bluey,"
				" t.stime"
				" FROM"
				" tb_dev_test_results t"
				" where cinema_hall = '%1'"
				" and date(stime) = date('%2')"
				" ORDER BY stime DESC limit 1;").arg(itemList.at(i).strTheaterNo).arg(QDate::currentDate().addDays(-j).toString("yyyy-M-d"));
			QSqlQueryModel *model = pDb->ExecSql(sql);
			if (model == NULL)
			{
				pLog->Write(LOG_REPORT, QObject::tr("Query database error"));
				pLog->Write(LOG_DATABASE, sql);
				return;
			}

			sql = QObject::tr("SELECT\n"
				"	wx,\n"
				"	wy,\n"
				"	rx,\n"
				"	ry,\n"
				"	gx,\n"
				"	gy,\n"
				"	bluex,\n"
				"	bluey\n"
				"   FROM\n"
				"	std_test\n"
				"   WHERE\n"
				"	cinema_hall = '%1';").arg(itemList.at(i).strTheaterNo);
			QSqlQueryModel *stdModel = pDb->ExecSql(sql); 
			if(stdModel == NULL)
			{
				pLog->Write(LOG_REPORT, QObject::tr("Query database error"));
				pLog->Write(LOG_DATABASE, sql);
				return;
			}


			double std_wx = stdModel->data(stdModel->index(0, 0)).toDouble(); 
			double std_wy = stdModel->data(stdModel->index(0, 1)).toDouble(); 
			double std_rx = stdModel->data(stdModel->index(0, 2)).toDouble(); 
			double std_ry = stdModel->data(stdModel->index(0, 3)).toDouble(); 
			double std_gx = stdModel->data(stdModel->index(0, 4)).toDouble(); 
			double std_gy = stdModel->data(stdModel->index(0, 5)).toDouble(); 
			double std_bx = stdModel->data(stdModel->index(0, 6)).toDouble(); 
			double std_by = stdModel->data(stdModel->index(0, 7)).toDouble(); 

			if(j%2==0)
			{
				format.setPatternBackgroundColor(QColor("#fffff"));
				format.setBorderStyle(QXlsx::Format::BorderThin);
				format.setBorderColor(QColor("##CCC1D9"));
			}
			else
			{
				format.setPatternBackgroundColor(QColor("#CCC1D9"));
				format.setBorderStyle(QXlsx::Format::BorderThin);
				format.setBorderColor(QColor("##CCC1D9"));

			}
			nPosStart++;
			if(model->rowCount() > 0 )
			{
				xlsx.write(nPosStart, 1, model->data(model->index(0, 0)).toString(), format); //CINEMA HALL
				double wx = model->data(model->index(0, 1)).toDouble(); 
				double wy = model->data(model->index(0, 2)).toDouble(); 
				double rx = model->data(model->index(0, 3)).toDouble(); 
				double ry = model->data(model->index(0, 4)).toDouble(); 
				double gx = model->data(model->index(0, 5)).toDouble(); 
				double gy = model->data(model->index(0, 6)).toDouble(); 
				double bx = model->data(model->index(0, 7)).toDouble(); 
				double by = model->data(model->index(0, 8)).toDouble(); 
				if(fabs(std_wx - wx) > STD_VALUE_CHROMA )											// wx
				{format.setFontColor(QColor("#ff0000"));}
				else
				{format.setFontColor(QColor("#000000"));}
				xlsx.write(nPosStart, 2, model->data(model->index(0, 1)).toString(), format);  

				if(fabs(std_wy - wy) > STD_VALUE_CHROMA )											 // wy
				{format.setFontColor(QColor("#ff0000"));}
				else
				{format.setFontColor(QColor("#000000"));}	
				xlsx.write(nPosStart, 3, model->data(model->index(0, 2)).toString(), format); 

				if(fabs(std_rx - rx) > STD_VALUE_CHROMA )											// rx
				{format.setFontColor(QColor("#ff0000"));}
				else
				{format.setFontColor(QColor("#000000"));}
				xlsx.write(nPosStart, 4, model->data(model->index(0, 3)).toString(), format);  

				if(fabs(std_ry - ry) > STD_VALUE_CHROMA )										   // ry
				{format.setFontColor(QColor("#ff0000"));}
				else
				{format.setFontColor(QColor("#000000"));}
				xlsx.write(nPosStart, 5, model->data(model->index(0, 4)).toString(), format);  

				if(fabs(std_gx - gx) > STD_VALUE_CHROMA )											// gx
				{format.setFontColor(QColor("#ff0000"));}
				else
				{format.setFontColor(QColor("#000000"));}
				xlsx.write(nPosStart, 6, model->data(model->index(0, 5)).toString(), format);  

				if(fabs(std_gy - gy) > STD_VALUE_CHROMA )											// gy									
				{format.setFontColor(QColor("#ff0000"));}
				else
				{format.setFontColor(QColor("#000000"));}
				xlsx.write(nPosStart, 7, model->data(model->index(0, 6)).toString(), format);  

				if(fabs(std_bx - bx) > STD_VALUE_CHROMA )											//bx
				{format.setFontColor(QColor("#ff0000"));}
				else
				{format.setFontColor(QColor("#000000"));}
				xlsx.write(nPosStart, 8, model->data(model->index(0, 7)).toString(), format); 

				if(fabs(std_by - by) > STD_VALUE_CHROMA )											// by
				{format.setFontColor(QColor("#ff0000"));}
				else
				{format.setFontColor(QColor("#000000"));}
				xlsx.write(nPosStart, 9, model->data(model->index(0, 8)).toString(), format); 
				
				format.setFontColor(QColor("#000000"));
				xlsx.write(nPosStart, 10, model->data(model->index(0, 9)).toDateTime().toString("M/d"), format); //stime
			}
			else
			{
				format.setFontColor(QColor("#000000"));
				xlsx.write(nPosStart, 1, itemList.at(i).strTheaterNo, format); //CINEMA HALL
				xlsx.write(nPosStart, 2, "N/A", format); // wx
				xlsx.write(nPosStart, 3, "N/A", format); // wy
				xlsx.write(nPosStart, 4, "N/A", format); // rx
				xlsx.write(nPosStart, 5, "N/A", format); // ry
				xlsx.write(nPosStart, 6, "N/A", format); // gx
				xlsx.write(nPosStart, 7, "N/A", format); // gy
				xlsx.write(nPosStart, 8, "N/A", format); //bx
				xlsx.write(nPosStart, 9, "N/A", format); //by
				xlsx.write(nPosStart, 10, QDate::currentDate().addDays(-j).toString("M/d"), format); //stime
			}
			delete model;
			model = NULL;
			delete stdModel;
			stdModel = NULL;
		}
		nTmp++;
	}
	pLog->Write(LOG_REPORT, "setLast7DayChroma OK");
}

void Excel::setLastLuminance()
{
	nPosStart = nPosStart + 2;
	int nTmp = nPosStart;
	for(int i = 0; i<itemList.size(); i++ )
	{
		format.setPatternBackgroundColor("#F79646");
		format.setFontBold(false);
		format.setFontName("Book Antiqua");
		format.setFontSize(12);
		format.setFontColor(QColor("#ffffff"));
		format.setHorizontalAlignment(QXlsx::Format::AlignHCenter);
		format.setBorderColor(QColor("#F79646"));
		RichString rs;
		nPosStart = nTmp + (7+ 3)*i + 1;
		switch(mLang)
		{
		case 1:
			rs.addFragment(QString::fromLocal8Bit("亮度检测结果(单位为ftL) - "), format);
			format.setFontColor(QColor("#ff0000"));
			rs.addFragment(QString::fromLocal8Bit("括号内为DCI要求标准值"), format);

			xlsx.currentWorksheet()->writeString(nPosStart, 1, rs, format);
// 			xlsx.write(202+i*10, 1, QString::fromLocal8Bit("亮度检测结果 - 括号内为DCI要求标准值"), format);
			break;
		default:
			rs.addFragment(QString::fromLocal8Bit("Test result of Luminance(ftL) - "), format);
			format.setFontColor(QColor("#ff0000"));
			rs.addFragment(QString::fromLocal8Bit("The numbers in brackets is DCI requirement"), format);

			xlsx.currentWorksheet()->writeString(nPosStart, 1, rs, format);
// 			xlsx.write(202+i*10, 1, "Test result of Luminance(ftL)", format);
		}
		xlsx.mergeCells(QXlsx::CellRange(nPosStart, 1, nPosStart, 10), format);
		//header
		format.setFontColor(QColor("#000000"));
		format.setFontBold(false);
		format.setHorizontalAlignment(QXlsx::Format::AlignHCenter);
		format.setPatternBackgroundColor(QColor("#FBD5B5"));
		nPosStart++;
		switch(mLang)
		{
		case 1:
			xlsx.write(nPosStart, 1, QString::fromLocal8Bit("影厅"), format);
			xlsx.write(nPosStart, 2, QString::fromLocal8Bit("白光(14ftL)"), format);
			xlsx.mergeCells(QXlsx::CellRange(nPosStart, 2, nPosStart, 3), format);

			xlsx.write(nPosStart, 4, QString::fromLocal8Bit("红光(2.95ftL)"), format);
			xlsx.mergeCells(QXlsx::CellRange(nPosStart, 4, nPosStart, 5), format);

			xlsx.write(nPosStart, 6, QString::fromLocal8Bit("绿光(10.11ftL)"), format);
			xlsx.mergeCells(QXlsx::CellRange(nPosStart, 6, nPosStart, 7), format);

			xlsx.write(nPosStart, 8, QString::fromLocal8Bit("蓝光(0.97ftL)"), format);
			xlsx.mergeCells(QXlsx::CellRange(nPosStart, 8, nPosStart, 9), format);

			xlsx.write(nPosStart, 10, QString::fromLocal8Bit("检测日期"), format);
			break;
		default:
		xlsx.write(nPosStart, 1, "Hall name", format);
		xlsx.write(nPosStart, 2, "WHITE(14ftL)", format);
		xlsx.mergeCells(QXlsx::CellRange(nPosStart, 2, nPosStart, 3), format);

		xlsx.write(nPosStart, 4, "RED(2.95ftL)", format);
		xlsx.mergeCells(QXlsx::CellRange(nPosStart, 4, nPosStart, 5), format);

		xlsx.write(nPosStart, 6, "GREEN(10.11ftL)", format);
		xlsx.mergeCells(QXlsx::CellRange(nPosStart, 6, nPosStart, 7), format);

		xlsx.write(nPosStart, 8, "BLUE(0.97ftL)", format);
		xlsx.mergeCells(QXlsx::CellRange(nPosStart, 8, nPosStart, 9), format);

		xlsx.write(nPosStart, 10, "Test date", format);		}

		format.setFontBold(false);


		for(int j = 0; j<7; j++ )
		{
			QString sql = QObject::tr(" SELECT"
				" t.cinema_hall,"
				" t.white_ftl,"
				" t.red_ftl,"
				" t.green_ftl,"
				" t.blue_ftl,"
				" t.stime"
				" FROM"
				" tb_dev_test_results t"
				" where cinema_hall = '%1'"
				" and date(stime) = date('%2')"
				" GROUP BY date(stime) ORDER BY stime DESC;").arg(itemList.at(i).strTheaterNo).arg(QDate::currentDate().addDays(-j).toString("yyyy-M-d"));
			QSqlQueryModel *model = pDb->ExecSql(sql);
			if (model == NULL)
			{
				pLog->Write(LOG_REPORT, QObject::tr("Query database error"));
				pLog->Write(LOG_DATABASE, sql);
				return;
			}

			sql = QObject::tr("SELECT\n"
				"	white_ftl,\n"
				"	red_ftl,\n"
				"	green_ftl,\n"
				"	blue_ftl\n"
				"   FROM\n"
				"	std_test\n"
				"   WHERE\n"
				"	cinema_hall = '%1';").arg(itemList.at(i).strTheaterNo); 

			QSqlQueryModel *stdModel = pDb->ExecSql(sql); 
			if (stdModel == NULL)
			{
				pLog->Write(LOG_REPORT, QObject::tr("Query database error"));
				pLog->Write(LOG_DATABASE, sql);
				return;
			}
			double std_white_ftl = stdModel->data(stdModel->index(0, 0)).toDouble(); 
			double std_red_ftl = stdModel->data(stdModel->index(0, 1)).toDouble(); 
			double std_green_ftl = stdModel->data(stdModel->index(0, 2)).toDouble(); 
			double std_blue_ftl = stdModel->data(stdModel->index(0, 3)).toDouble(); 

			if(j%2==0)
			{
				format.setPatternBackgroundColor(QColor("#fffff"));
				format.setBorderStyle(QXlsx::Format::BorderThin);
				format.setBorderColor(QColor("#F79646"));
			}
			else
			{
				format.setPatternBackgroundColor(QColor("#FBD5B5"));
				format.setBorderStyle(QXlsx::Format::BorderThin);
				format.setBorderColor(QColor("#F79646"));
			}
			nPosStart++;
			if(model->rowCount() > 0)
			{
				double white_ftl = model->data(model->index(0, 1)).toDouble(); 
				double red_ftl = model->data(model->index(0, 2)).toDouble(); 
				double green_ftl = model->data(model->index(0, 3)).toDouble(); 
				double blue_ftl = model->data(model->index(0, 4)).toDouble(); 

				xlsx.write(nPosStart, 1, model->data(model->index(0, 0)).toString(), format); //CINEMA HALL
				if(fabs(std_white_ftl - white_ftl)>3)
				{format.setFontColor(QColor("#ff0000"));}
				else 
				{format.setFontColor(QColor("#000000"));}
				xlsx.write(nPosStart, 2, model->data(model->index(0, 1)).toString(), format); // white ftl
				xlsx.mergeCells(QXlsx::CellRange(nPosStart, 2, nPosStart, 3), format);

				if(fabs(std_red_ftl - red_ftl)>3)
				{format.setFontColor(QColor("#ff0000"));}
				else 
				{format.setFontColor(QColor("#000000"));}
				xlsx.write(nPosStart, 4, model->data(model->index(0, 2)).toString(), format); // red ftl
				xlsx.mergeCells(QXlsx::CellRange(nPosStart, 4, nPosStart, 5), format);

				if(fabs(std_green_ftl - green_ftl)>3)
				{format.setFontColor(QColor("#ff0000"));}
				else 
				{format.setFontColor(QColor("#000000"));}
				xlsx.write(nPosStart, 6, model->data(model->index(0, 3)).toString(), format); // green ftl
				xlsx.mergeCells(QXlsx::CellRange(nPosStart, 6, nPosStart, 7), format);

				if(fabs(std_blue_ftl - blue_ftl)>3)
				{format.setFontColor(QColor("#ff0000"));}
				else 
				{format.setFontColor(QColor("#000000"));}
				xlsx.write(nPosStart, 8, model->data(model->index(0, 4)).toString(), format); // blue ftl
				xlsx.mergeCells(QXlsx::CellRange(nPosStart, 8, nPosStart, 9), format);

				format.setFontColor(QColor("#000000"));
				xlsx.write(nPosStart, 10, model->data(model->index(0, 5)).toDateTime().toString("M/d"), format); // stime
			}
			else
			{
				format.setFontColor(QColor("#000000"));
				xlsx.write(nPosStart, 1, itemList.at(i).strTheaterNo, format); //CINEMA HALL

				xlsx.write(nPosStart, 2, "N/A", format); // white ftl
				xlsx.mergeCells(QXlsx::CellRange(nPosStart, 2, nPosStart, 3), format);

				xlsx.write(nPosStart, 4, "N/A", format); // red ftl
				xlsx.mergeCells(QXlsx::CellRange(nPosStart, 4, nPosStart, 5), format);

				xlsx.write(nPosStart, 6, "N/A", format); // green ftl
				xlsx.mergeCells(QXlsx::CellRange(nPosStart, 6, nPosStart, 7), format);

				xlsx.write(nPosStart, 8, "N/A", format); // blue ftl
				xlsx.mergeCells(QXlsx::CellRange(nPosStart, 8, nPosStart, 9), format);
				xlsx.write(nPosStart, 10, QDate::currentDate().addDays(-j).toString("M/d"), format); // stime
			}
			delete model;
			model = NULL;
			delete stdModel;
			stdModel = NULL;
		}
		nTmp++;
	}
	pLog->Write(LOG_REPORT, "setLast7DayLuminance OK.");
}

void Excel::setLastLuminanceCDM()
{
	//  
	nPosStart = nPosStart + 2; 
	int nTmp = nPosStart;
	for(int i = 0; i<itemList.size(); i++ )
	{
		format.setPatternBackgroundColor("#4BACC6");
		format.setFontBold(false);
		format.setFontName("Book Antiqua");
		format.setFontSize(12);
		format.setFontColor(QColor("#ffffff"));
		format.setHorizontalAlignment(QXlsx::Format::AlignHCenter);
		format.setBorderColor(QColor("#6BDDB1"));
		RichString rs;
		nPosStart = nTmp + (7+3)*i + 1;
		switch(mLang)
		{
		case 1:
			rs.addFragment(QString::fromLocal8Bit("亮度检测结果(单位为cd/m2) - "), format);
			format.setFontColor(QColor("#ff0000"));
			rs.addFragment(QString::fromLocal8Bit("括号内为DCI要求标准值"), format);

			xlsx.currentWorksheet()->writeString(nPosStart, 1, rs, format);
			// 			xlsx.write(272+i*10, 1, QString::fromLocal8Bit("亮度检测结果 - 括号内为DCI要求标准值"), format);
			break;
		default:
			rs.addFragment(QString::fromLocal8Bit("Test result of Luminance(ftL) - "), format);
			format.setFontColor(QColor("#ff0000"));
			rs.addFragment(QString::fromLocal8Bit("The number in brackets is DCI requirement"), format);
			xlsx.currentWorksheet()->writeString(nPosStart, 1, rs, format);
// 			xlsx.write(272+i*10, 1, "Test result of Luminance(cd/m2)", format);
		}
		xlsx.mergeCells(QXlsx::CellRange(nPosStart, 1, nPosStart, 10), format);
		//header
		format.setFontColor(QColor("#000000"));
		format.setFontBold(false);
		format.setHorizontalAlignment(QXlsx::Format::AlignHCenter);
		format.setPatternBackgroundColor(QColor("#B7DDE8"));
		nPosStart++;
		switch(mLang)
		{
		case 1:
			xlsx.write(nPosStart, 1, QString::fromLocal8Bit("影厅"), format);
			xlsx.write(nPosStart, 2, QString::fromLocal8Bit("白光(48cd/m2)"), format);
			xlsx.mergeCells(QXlsx::CellRange(nPosStart, 2, nPosStart, 3), format);

			xlsx.write(nPosStart, 4, QString::fromLocal8Bit("红光(10.06cd/m2)"), format);
			xlsx.mergeCells(QXlsx::CellRange(nPosStart, 4, nPosStart, 5), format);

			xlsx.write(nPosStart, 6, QString::fromLocal8Bit("绿光(34.64cd/m2)"), format);
			xlsx.mergeCells(QXlsx::CellRange(nPosStart, 6, nPosStart, 7), format);

			xlsx.write(nPosStart, 8, QString::fromLocal8Bit("蓝光(3.31cd/m2)"), format);
			xlsx.mergeCells(QXlsx::CellRange(nPosStart, 8, nPosStart, 9), format);

			xlsx.write(nPosStart, 10, QString::fromLocal8Bit("检测日期"), format);
			break;
		default:
			xlsx.write(nPosStart, 1, "Hall name", format);
			xlsx.write(nPosStart, 2, "WHITE(48cd/m2)", format);
			xlsx.mergeCells(QXlsx::CellRange(nPosStart, 2, nPosStart, 3), format);

			xlsx.write(nPosStart, 4, "RED(10.06cd/m2)", format);
			xlsx.mergeCells(QXlsx::CellRange(nPosStart, 4, nPosStart, 5), format);

			xlsx.write(nPosStart, 6, "GREEN(34.64cd/m2)", format);
			xlsx.mergeCells(QXlsx::CellRange(nPosStart, 6, nPosStart, 7), format);

			xlsx.write(nPosStart, 8, "BLUE(3.31cd/m2)", format);
			xlsx.mergeCells(QXlsx::CellRange(nPosStart, 8, nPosStart, 9), format);

			xlsx.write(nPosStart, 10, "Test date", format);
		}

		format.setFontBold(false);


		for(int j = 0; j<7; j++ )
		{
			QString sql = QObject::tr(" SELECT"
				" t.cinema_hall,"
				" t.white_ftl,"
				" t.red_ftl,"
				" t.green_ftl,"
				" t.blue_ftl,"
				" t.stime"
				" FROM"
				" tb_dev_test_results t"
				" where cinema_hall = '%1'"
				" and date(stime) = date('%2')"
				" GROUP BY date(stime) ORDER BY stime DESC;").arg(itemList.at(i).strTheaterNo).arg(QDate::currentDate().addDays(-j).toString("yyyy-M-d"));
			QSqlQueryModel *model = pDb->ExecSql(sql);
			if (model == NULL)
			{
				pLog->Write(LOG_REPORT, QObject::tr("Query database error"));
				pLog->Write(LOG_DATABASE, sql);
				return;
			}

			sql = QObject::tr("SELECT\n"
				"	white_ftl,\n"
				"	red_ftl,\n"
				"	green_ftl,\n"
				"	blue_ftl\n"
				"   FROM\n"
				"	std_test\n"
				"   WHERE\n"
				"	cinema_hall = '%1';").arg(itemList.at(i).strTheaterNo); 

			QSqlQueryModel *stdModel = pDb->ExecSql(sql); 
			if (stdModel == NULL)
			{
				pLog->Write(LOG_REPORT, QObject::tr("Query database error"));
				pLog->Write(LOG_DATABASE, sql);
				return;
			}
			double std_white_ftl = stdModel->data(stdModel->index(0, 0)).toDouble(); 
			double std_red_ftl = stdModel->data(stdModel->index(0, 1)).toDouble(); 
			double std_green_ftl = stdModel->data(stdModel->index(0, 2)).toDouble(); 
			double std_blue_ftl = stdModel->data(stdModel->index(0, 3)).toDouble(); 

			if(j%2==0)
			{
				format.setPatternBackgroundColor(QColor("#fffff"));
				format.setBorderStyle(QXlsx::Format::BorderThin);
				format.setBorderColor(QColor("#6BDDB1"));
			}
			else
			{
				format.setPatternBackgroundColor(QColor("#B7DDE8"));
				format.setBorderStyle(QXlsx::Format::BorderThin);
				format.setBorderColor(QColor("#6BDDB1"));
			}
			nPosStart++;
			if(model->rowCount() > 0)
			{
				double white_ftl = model->data(model->index(0, 1)).toDouble(); 
				double red_ftl = model->data(model->index(0, 2)).toDouble(); 
				double green_ftl = model->data(model->index(0, 3)).toDouble(); 
				double blue_ftl = model->data(model->index(0, 4)).toDouble(); 

				xlsx.write(nPosStart, 1, model->data(model->index(0, 0)).toString(), format); //CINEMA HALL
				if(fabs(std_white_ftl - white_ftl)>3)
				{format.setFontColor(QColor("#ff0000"));}
				else 
				{format.setFontColor(QColor("#000000"));}
				xlsx.write(nPosStart, 2, QString::number(model->data(model->index(0, 1)).toFloat() * 3.183/0.929), format); // white ftl
				xlsx.mergeCells(QXlsx::CellRange(nPosStart, 2, nPosStart, 3), format);

				if(fabs(std_red_ftl - red_ftl)>3)
				{format.setFontColor(QColor("#ff0000"));}
				else 
				{format.setFontColor(QColor("#000000"));}
				xlsx.write(nPosStart, 4, QString::number(model->data(model->index(0, 2)).toFloat()*3.183/0.929), format); // red ftl
				xlsx.mergeCells(QXlsx::CellRange(nPosStart, 4, nPosStart, 5), format);

				if(fabs(std_green_ftl - green_ftl)>3)
				{format.setFontColor(QColor("#ff0000"));}
				else 
				{format.setFontColor(QColor("#000000"));}
				xlsx.write(nPosStart, 6, QString::number(model->data(model->index(0, 3)).toFloat()*3.183/0.929), format); // green ftl
				xlsx.mergeCells(QXlsx::CellRange(nPosStart, 6, nPosStart, 7), format);

				if(fabs(std_blue_ftl - blue_ftl)>3)
				{format.setFontColor(QColor("#ff0000"));}
				else 
				{format.setFontColor(QColor("#000000"));}
				xlsx.write(nPosStart, 8, QString::number(model->data(model->index(0, 4)).toFloat()*3.183/0.929), format); // blue ftl
				xlsx.mergeCells(QXlsx::CellRange(nPosStart, 8, nPosStart, 9), format);

				format.setFontColor(QColor("#000000"));
				xlsx.write(nPosStart, 10, model->data(model->index(0, 5)).toDateTime().toString("M/d"), format); // stime
			}
			else
			{
				format.setFontColor(QColor("#000000"));
				xlsx.write(nPosStart, 1, itemList.at(i).strTheaterNo, format); //CINEMA HALL

				xlsx.write(nPosStart, 2, "N/A", format); // white ftl
				xlsx.mergeCells(QXlsx::CellRange(nPosStart, 2, nPosStart, 3), format);

				xlsx.write(nPosStart, 4, "N/A", format); // red ftl
				xlsx.mergeCells(QXlsx::CellRange(nPosStart, 4, nPosStart, 5), format);

				xlsx.write(nPosStart, 6, "N/A", format); // green ftl
				xlsx.mergeCells(QXlsx::CellRange(nPosStart, 6, nPosStart, 7), format);

				xlsx.write(nPosStart, 8, "N/A", format); // blue ftl
				xlsx.mergeCells(QXlsx::CellRange(nPosStart, 8, nPosStart, 9), format);
				xlsx.write(nPosStart, 10, QDate::currentDate().addDays(-j).toString("M/d"), format); // stime
			}
			delete model;
			model = NULL;
			delete stdModel;
			stdModel = NULL;
		}
		nTmp++; 
	}
	pLog->Write(LOG_REPORT, "setLast7DayLuminance OK.");
}

void Excel::setLastSPL()
{
	// The test results of last 7 days are below:
	nPosStart = nPosStart + 3;   // nPosStart = 52
	format.setPatternBackgroundColor(QColor("#fff"));
	format.setHorizontalAlignment(QXlsx::Format::AlignLeft);
	format.setFontBold(true);
	format.setFontSize(16);
	format.setFontColor(QColor("#000"));
	format.setBorderColor(QColor("#ffffff"));
	switch (mLang)
	{
	case 1:
		xlsx.write(nPosStart, 1, QString::fromLocal8Bit("最近7天检测结果如下:"), format);
		break;
	default:
	xlsx.write(nPosStart, 1, "The test results of last 7 days are below:", format);
	}

	xlsx.mergeCells(QXlsx::CellRange(nPosStart, 1, nPosStart, 10), format); 
	/*xlsx.mergeCells("A53:J53", format);*/
	//压
	nPosStart = nPosStart + 2; 
	int nTmp = nPosStart; // nPosStart = 54
	for(int i = 0; i<itemList.size(); i++)
	{
		//header
		format.setPatternBackgroundColor(QColor("#9BBB59"));
		format.setFontColor(QColor("#fff"));
		format.setFontSize(12);
		format.setFontBold(false);
		format.setHorizontalAlignment(QXlsx::Format::AlignHCenter);
		RichString rs;
		nPosStart = nTmp + (7+3)*i + 1 ;  
		 
		 
		switch(mLang)
		{
		case 1:
			rs.addFragment(QString::fromLocal8Bit("声压检测结果(dBC) - "), format);
			format.setFontColor(QColor("#ff0000"));
			rs.addFragment(QString::fromLocal8Bit("DCI要求标准值为85dBC"), format);

			xlsx.currentWorksheet()->writeString(nPosStart, 1, rs, format);
			//xlsx.write(47+i*10, 1, QString::fromLocal8Bit("声压检测结果(dBC) - DCI要求标准值为85dBC"), format);
			break;
		default:
			rs.addFragment(QString::fromLocal8Bit("Test result of SPL(dBC) - "), format);
			format.setFontColor(QColor("#ff0000"));
			rs.addFragment(QString::fromLocal8Bit("DCI requierment is 85dBC"), format);

			xlsx.currentWorksheet()->writeString(nPosStart, 1, rs, format);
// 			xlsx.write(55+i*10, 1, "Test result of SPL (dBC) - DCI requierment is 85dBC", format);
		}
		xlsx.mergeCells(QXlsx::CellRange(nPosStart, 1, nPosStart, 10), format);
		format.setFontColor(QColor("#000"));
		format.setPatternBackgroundColor(QColor("#D7E3BC"));
		format.setBorderColor(QColor("#9BBB59"));
		nPosStart++;
		switch (mLang)
		{
		case 1:
			xlsx.write(nPosStart, 1, QString::fromLocal8Bit("影厅"), format);
			xlsx.write(nPosStart, 2, QString::fromLocal8Bit("左声道"), format);
			xlsx.write(nPosStart, 3, QString::fromLocal8Bit("右声道"), format);
			xlsx.write(nPosStart, 4, QString::fromLocal8Bit("中置"), format);
			xlsx.write(nPosStart, 5, QString::fromLocal8Bit("重低音"), format);
			xlsx.write(nPosStart, 6, QString::fromLocal8Bit("左环"), format);
			xlsx.write(nPosStart, 7, QString::fromLocal8Bit("右环"), format);
			xlsx.write(nPosStart, 8, QString::fromLocal8Bit("左后环"), format);
			xlsx.write(nPosStart, 9, QString::fromLocal8Bit("右后环"), format);
			xlsx.write(nPosStart, 10, QString::fromLocal8Bit("检测日期"), format);
			break;
		default:
			xlsx.write(nPosStart, 1, "Hall name", format);
			xlsx.write(nPosStart, 2, "Left", format);
			xlsx.write(nPosStart, 3, "Right", format);
			xlsx.write(nPosStart, 4, "Center", format);
			xlsx.write(nPosStart, 5, "LFE", format);
			xlsx.write(nPosStart, 6, "LS", format);
			xlsx.write(nPosStart, 7, "RS", format);
			xlsx.write(nPosStart, 8, "LSS", format);
			xlsx.write(nPosStart, 9, "RSS", format);
			xlsx.write(nPosStart, 10, "Test date", format);
		}

		format.setFontSize(12);
		format.setPatternBackgroundColor(QColor("#fff"));
		format.setFontBold(false);
		int nPosStart2 = nPosStart;
		for(int j = 0; j<7; j++)
		{
			if(j%2 == 0)
			{
				format.setPatternBackgroundColor(QColor("#fffff"));
				format.setBorderStyle(QXlsx::Format::BorderThin);
				format.setBorderColor(QColor("#9BBB59")); //9BBB59
			}
			else
			{
				format.setPatternBackgroundColor(QColor("#D7E3BC"));
				format.setBorderStyle(QXlsx::Format::BorderThin);
				format.setBorderColor(QColor("#9BBB59"));
			}
			QString sql = QObject::tr("SELECT"
				" t.cinema_hall,"
				" t.lc,"
				" t.rc,"
				" t.cc,"
				" t.hb,"
				" t.la,"
				" t.ra,"
				" t.left_after,"
				" t.right_after,"
				" date(t.stime)"
				" FROM"
				" tb_dev_test_results t"
				" where cinema_hall = '%1'"
				" and date(stime) = date('%2')"
				" GROUP BY date(stime) ORDER BY stime DESC;").arg(itemList.at(i).strTheaterNo).arg(QDate::currentDate().addDays(-j).toString("yyyy-M-d"));

			QSqlQueryModel *model = pDb->ExecSql(sql);
			if (model == NULL)
			{
				pLog->Write(LOG_REPORT, QObject::tr("Query database error"));
				pLog->Write(LOG_DATABASE, sql);
				return;
			}
			sql = QObject::tr("SELECT\n"
				"	lc AS 'left',\n"
				"	rc AS 'right',\n"
				"	cc AS 'center',\n"
				"	cc AS 'lfe',\n"
				"	la AS 'ls',\n"
				"	ra AS 'rs',\n"
				"	lss,\n"
				"	rss, \n"
				"	s.cinema_hall\n"
				"   FROM\n"
				"	std_test s where cinema_hall = '%1';").arg(itemList.at(i).strTheaterNo); 

			QSqlQueryModel *stdModel = pDb->ExecSql(sql); 

			double std_left = stdModel->data(stdModel->index(0, 0)).toDouble(); 
			double std_right = stdModel->data(stdModel->index(0, 1)).toDouble(); 
			double std_center = stdModel->data(stdModel->index(0, 2)).toDouble(); 
			double std_lfe = stdModel->data(stdModel->index(0, 3)).toDouble(); 
			double std_ls = stdModel->data(stdModel->index(0, 4)).toDouble(); 
			double std_rs = stdModel->data(stdModel->index(0, 5)).toDouble(); 
			double std_lss = stdModel->data(stdModel->index(0, 6)).toDouble(); 
			double std_rss = stdModel->data(stdModel->index(0, 7)).toDouble(); 
			nPosStart++; // nPosStart = 57
			if(model->rowCount() > 0)
			{
				xlsx.write(nPosStart, 1, model->data(model->index(0, 0)).toString(), format );
				double left =  model->data(model->index(0, 1)).toDouble(); 
				double right =  model->data(model->index(0, 2)).toDouble();
				double center =  model->data(model->index(0, 3)).toDouble();
				double lfe =  model->data(model->index(0, 4)).toDouble();
				double ls =  model->data(model->index(0,5)).toDouble();
				double rs =  model->data(model->index(0, 6)).toDouble();
				double lss = 0; 
				double rss = 0;
				if(fabs(std_left - left ) > 5)
				{
					format.setFontColor(QColor("#ff0000")); 
				}
				else
				{
					format.setFontColor(QColor("#000000")); 
				}
				xlsx.write(/*j+57+i*10*/nPosStart, 2, model->data(model->index(0, 1)).toString(), format );

				if(fabs(std_right - right ) > 5)
				{
					format.setFontColor(QColor("#ff0000")); 
				}
				else
				{
					format.setFontColor(QColor("#000000")); 
				}
				xlsx.write(nPosStart, 3, model->data(model->index(0, 2)).toString(), format );

				if(fabs(std_center - center ) > 5)
				{
					format.setFontColor(QColor("#ff0000")); 
				}
				else
				{
					format.setFontColor(QColor("#000000")); 
				}
				xlsx.write(nPosStart, 4, model->data(model->index(0, 3)).toString(), format );

				if(fabs(std_lfe - lfe ) > 5)
				{
					format.setFontColor(QColor("#ff0000")); 
				}
				else
				{
					format.setFontColor(QColor("#000000")); 
				}
				xlsx.write(nPosStart, 5, model->data(model->index(0, 4)).toString(), format );

				if(fabs(std_ls - ls ) > 5)
				{
					format.setFontColor(QColor("#ff0000")); 
				}
				else
				{
					format.setFontColor(QColor("#000000")); 
				}
				xlsx.write(nPosStart, 6, model->data(model->index(0, 5)).toString(), format );

				if(fabs(std_rs - rs ) > 5)
				{
					format.setFontColor(QColor("#ff0000")); 
				}
				else
				{
					format.setFontColor(QColor("#000000")); 
				}
				xlsx.write(nPosStart, 7, model->data(model->index(0, 6)).toString(), format );

				if(fabs(std_left - left ) > 5)
				{
					format.setFontColor(QColor("#ff0000")); 
				}
				else
				{
					format.setFontColor(QColor("#000000")); 
				}

				if(fabs(std_lss - lss ) > 5)
				{
					format.setFontColor(QColor("#ff0000")); 
				}
				else
				{
					format.setFontColor(QColor("#000000")); 
				}
				xlsx.write(nPosStart, 8, model->data(model->index(0, 7)).toString(), format );

				if(fabs(std_rss - rss ) > 5)
				{
					format.setFontColor(QColor("#ff0000")); 
				}
				else
				{
					format.setFontColor(QColor("#000000")); 
				}
				xlsx.write(nPosStart, 9, model->data(model->index(0, 8)).toString(), format );

				format.setFontColor(QColor("#000000"));
				xlsx.write(nPosStart, 10, model->data(model->index(0, 9)).toDate().toString("M/d"), format );
			}
			else
			{
				format.setFontColor(QColor("#000000"));
				xlsx.write(nPosStart, 1, itemList.at(i).strTheaterNo, format );
				xlsx.write(nPosStart, 2, "N/A", format );
				xlsx.write(nPosStart, 3, "N/A", format );
				xlsx.write(nPosStart, 4, "N/A", format );
				xlsx.write(nPosStart, 5, "N/A", format );
				xlsx.write(nPosStart, 6, "N/A", format );
				xlsx.write(nPosStart, 7, "N/A", format );
				xlsx.write(nPosStart, 8, "N/A", format );
				xlsx.write(nPosStart, 9, "N/A", format );
				xlsx.write(nPosStart, 10, QDate::currentDate().addDays(-j).toString("M/d"), format );
			}
			delete model;
			model = NULL;
			delete stdModel;
			stdModel = NULL;
		}
		
		nTmp++;
	}

	pLog->Write(LOG_REPORT, "setLast7DaySPL OK.");
}

void Excel::SaveAs(QString &fileName)
{
	xlsx.saveAs(fileName);
	pLog->Write(LOG_REPORT, "Save " + fileName + " OK.");
}

void Excel::setDefaultFormat(Format &format)
{

}

void Excel::setWarningFormat(Format &format)
{

}

void Excel::setErrorFormat(Format &format)
{

}

void Excel::setSuccessFormat(Format &format)
{

}

void Excel::setThemeBlue(Format &format)
{

}

void Excel::setDb(IDatabase* pDb)
{
	this->pDb = pDb;
}
void Excel::setModel(QSqlQueryModel *model)
{
	this->model = model;
}

void Excel::setLog(ILog* pLog)
{
	this->pLog = pLog;
}