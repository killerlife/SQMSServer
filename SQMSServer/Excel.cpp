#include "Excel.h"


Excel::Excel(QObject *parent)
{

}

Excel::~Excel()
{

}

void Excel::setHeader()
{
	format.setFontBold(false);
	format.setFontColor(QColor(255, 255, 255));
	format.setFontSize(24);
	format.setFontName("Book Antiqua");
	format.setPatternBackgroundColor(QColor("#3366FF"));

	xlsx.addSheet("Test Result");
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
	xlsx.write(4, 1, "Theater: " + itemList.at(0).strTheaterName, format );
	xlsx.mergeCells("A4:J4", format);

	xlsx.write(5, 1, "Date: " + QDate::currentDate().toString("M/d/yyyy"), format);
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


void Excel::setTodayChroma()
{
	// Test result of Chroma
	// 色域
	format.setPatternBackgroundColor("#4BACC6");
	format.setFontBold(false);
	format.setFontName("Book Antiqua");
	format.setFontSize(12);
	format.setFontColor(QColor("#ffffff"));
	format.setHorizontalAlignment(QXlsx::Format::AlignHCenter);
	format.setBorderColor(QColor("#4BACC6"));
	xlsx.write(21, 1, "Test result of Chroma", format);
	xlsx.mergeCells("A21:J21", format);
	//header
	format.setFontColor(QColor("#000000"));
	format.setFontBold(false);
	format.setHorizontalAlignment(QXlsx::Format::AlignHCenter);
	format.setPatternBackgroundColor(QColor("#B7DDE8"));
	xlsx.write(22, 1, "", format);
	xlsx.write(22, 2, "WHITE(0.314, 0.351)", format);
	xlsx.mergeCells("B22:C22", format);

	xlsx.write(22, 4, "RED(0.680, 0.320)", format);
	xlsx.mergeCells("D22:E22", format);


	xlsx.write(22, 6, "GREE(0.265, 0.690)", format );
	xlsx.mergeCells("F22:G22", format);

	xlsx.write(22, 8, "BLUE(0.150, 0.060)", format);
	xlsx.mergeCells("H22:I22", format);

	xlsx.write(22, 10, "", format);
	xlsx.write(23, 1, "Hall name", format);
	xlsx.write(23, 2, "x", format);
	xlsx.write(23, 3, "y", format);
	xlsx.write(23, 4, "x", format);
	xlsx.write(23, 5, "y", format);
	xlsx.write(23, 6, "x", format);
	xlsx.write(23, 7, "y", format);
	xlsx.write(23, 8, "x", format);
	xlsx.write(23, 9, "y", format);
	xlsx.write(23, 10, "Test time", format);
	format.setFontBold(false);
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

		if (model->rowCount()>0)
		{
			for(int j = 0; j<model->rowCount(); j++ )
			{
				double wx = model->data(model->index(j, 0)).toDouble(); 
				double wy = model->data(model->index(j, 1)).toDouble(); 
				double rx = model->data(model->index(j, 2)).toDouble(); 
				double ry = model->data(model->index(j, 3)).toDouble(); 
				double gx = model->data(model->index(j, 4)).toDouble(); 
				double gy = model->data(model->index(j, 5)).toDouble(); 
				double bx = model->data(model->index(j, 6)).toDouble(); 
				double by = model->data(model->index(j, 7)).toDouble(); 

				xlsx.write(i+24, 1, model->data(model->index(j, 0)).toString(), format); //CINEMA HALL

				if(fabs(std_wx - wx) > 0.03 )											// wx
				{format.setFontColor(QColor("#ff0000"));}
				else
				{format.setFontColor(QColor("#000000"));}
				xlsx.write(i+24, 2, model->data(model->index(j, 1)).toString(), format); 

				if(fabs(std_wy - wy) > 0.03 )											 // wy
				{format.setFontColor(QColor("#ff0000"));}
				else
				{format.setFontColor(QColor("#000000"));}			
				xlsx.write(i+24, 3, model->data(model->index(j, 2)).toString(), format); 

				if(fabs(std_rx - rx) > 0.03 )											// rx
				{format.setFontColor(QColor("#ff0000"));}
				else
				{format.setFontColor(QColor("#000000"));}
				xlsx.write(i+24, 4, model->data(model->index(j, 3)).toString(), format);  

				if(fabs(std_ry - ry) > 0.03 )										   // ry
				{format.setFontColor(QColor("#ff0000"));}
				else
				{format.setFontColor(QColor("#000000"));}
				xlsx.write(i+24, 5, model->data(model->index(j, 4)).toString(), format);  

				if(fabs(std_gx - gx) > 0.03 )											// gx
				{format.setFontColor(QColor("#ff0000"));}
				else
				{format.setFontColor(QColor("#000000"));}
				xlsx.write(i+24, 6, model->data(model->index(j, 5)).toString(), format); 

				if(fabs(std_gy - gy) > 0.03 )											// gy									
				{format.setFontColor(QColor("#ff0000"));}
				else
				{format.setFontColor(QColor("#000000"));}
				xlsx.write(i+24, 7, model->data(model->index(j, 6)).toString(), format); 

				if(fabs(std_bx - bx) > 0.03 )											//bx
				{format.setFontColor(QColor("#ff0000"));}
				else
				{format.setFontColor(QColor("#000000"));}
				xlsx.write(i+24, 8, model->data(model->index(j, 7)).toString(), format);  

				if(fabs(std_by - by) > 0.03 )											// by
				{format.setFontColor(QColor("#ff0000"));}
				else
				{format.setFontColor(QColor("#000000"));}
				xlsx.write(i+24, 9, model->data(model->index(j, 8)).toString(), format); 
				format.setFontColor(QColor("#000000"));
				xlsx.write(i+24, 10, model->data(model->index(j, 9)).toDateTime().toString("hh:mm:ss"), format); //stime
			}
		}
		else
		{
			format.setFontColor(QColor("#000000")); 
			xlsx.write(i+24, 1, itemList.at(i).strTheaterNo, format); //CINEMA HALL
			xlsx.write(i+24, 2, "N/A", format); // wx
			xlsx.write(i+24, 3, "N/A", format); // wy
			xlsx.write(i+24, 4, "N/A", format); // rx
			xlsx.write(i+24, 5, "N/A", format); // ry
			xlsx.write(i+24, 6, "N/A", format); // gx
			xlsx.write(i+24, 7, "N/A", format); // gy
			xlsx.write(i+24, 8, "N/A", format); //bx
			xlsx.write(i+24, 9, "N/A", format); //by
			xlsx.write(i+24, 10, "N/A", format); //stime
		}
		delete model;
		model = NULL;
		delete stdModel; 
		stdModel = NULL;
	}

}

void Excel::setTodaySPL()
{
	
	xlsx.write(9, 1, "The test results of today are below:", format);
	
	xlsx.mergeCells("A9:J9", format);
	format.setPatternBackgroundColor(QColor("#4BACC6"));
	format.setFontBold(false);
	format.setFontName("Book Antiqua");
	format.setFontSize(12);
	format.setFontColor(QColor("#ffffff"));
	format.setHorizontalAlignment(QXlsx::Format::AlignHCenter);

	xlsx.write(11, 1, "Test result of SPL(dBC) - DCI requierment is 85dBC", format);
	xlsx.mergeCells("A11:J11", format);
	//header
	format.setFontColor(QColor("#000000"));
	format.setFontBold(false);
	format.setPatternBackgroundColor(QColor("#B7DDE8"));

	format.setHorizontalAlignment(QXlsx::Format::AlignHCenter);
	format.setBorderStyle(Format::BorderThin);
	format.setBorderColor(QColor("#B7DDE8"));
	xlsx.write(12, 1, "Hall name", format);
	xlsx.write(12, 2, "Left", format);
	xlsx.write(12, 3, "Right", format);
	xlsx.write(12, 4, "Center", format);
	xlsx.write(12, 5, "LFE", format);
	xlsx.write(12, 6, "LS", format);
	xlsx.write(12, 7, "RS", format);
	xlsx.write(12, 8, "LSS", format);
	xlsx.write(12, 9, "RSS", format);
	xlsx.write(12, 10, "Test time", format);

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

				if(fabs(std_left - left) > 5 )
				{format.setFontColor(QColor("#ff0000")); }
				else
				{format.setFontColor(QColor("#000000")); }
				xlsx.write(i+13, 2, model->data(model->index(j, 1)).toString() , format); // Left
				if(fabs(std_right - right) > 5 )
				{format.setFontColor(QColor("#ff0000")); }
				else
				{format.setFontColor(QColor("#000000")); }
				xlsx.write(i+13, 3, model->data(model->index(j, 2)).toString(), format); // Right
				if(fabs(std_center - center) > 5 )
				{format.setFontColor(QColor("#ff0000")); }
				else
				{format.setFontColor(QColor("#000000")); }
				xlsx.write(i+13, 4, model->data(model->index(j, 3)).toString(), format); // Center
				if(fabs(std_lfe - lfe) > 5 )
				{format.setFontColor(QColor("#ff0000")); }
				else
				{format.setFontColor(QColor("#000000")); }
				xlsx.write(i+13, 5, model->data(model->index(j, 4)).toString(), format); // LFE
				if(fabs(std_ls - ls) > 5 )
				{format.setFontColor(QColor("#ff0000")); }
				else
				{format.setFontColor(QColor("#000000")); }
				xlsx.write(i+13, 6, model->data(model->index(j, 5)).toString(), format); // LS
				if(fabs(std_ls - ls) > 5 )
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

}

void Excel::setTodayLuminance()
{
	// Test result of Luminance
	// 亮度
	format.setPatternBackgroundColor("#4BACC6");
	format.setFontBold(false);
	format.setFontName("Book Antiqua");
	format.setFontSize(12);
	format.setFontColor(QColor("#ffffff"));
	format.setHorizontalAlignment(QXlsx::Format::AlignHCenter);
	format.setBorderColor(QColor("#4BACC6"));
	xlsx.write(32, 1, "Test result of Luminance", format);
	xlsx.mergeCells("A32:J32", format);
	//header
	format.setFontColor(QColor("#000000"));
	format.setFontBold(false);
	format.setHorizontalAlignment(QXlsx::Format::AlignHCenter);
	format.setPatternBackgroundColor(QColor("#B7DDE8"));
	xlsx.write(33, 1, "Hall name", format);
	xlsx.write(33, 2, "WHITE(14ftL)", format);
	xlsx.mergeCells("B33:C33", format);

	xlsx.write(33, 4, "RED(2.95ftL)", format);
	xlsx.mergeCells("D33:E33", format);

	xlsx.write(33, 6, "GREEN(10.11ftL)", format);
	xlsx.mergeCells("F33:G33", format);

	xlsx.write(33, 8, "BLUE(0.97ftL)", format);
	xlsx.mergeCells("H33:I33", format);

	xlsx.write(33, 10, "Test time", format);
	format.setFontBold(false);
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
		

		if(model->rowCount() > 0)
		{
			for(int j = 0; j<model->rowCount(); j++ )
			{
				double white_ftl = model->data(model->index(j, 1)).toDouble(); 
				double red_ftl = model->data(model->index(j, 2)).toDouble(); 
				double green_ftl = model->data(model->index(j, 3)).toDouble(); 
				double blue_ftl = model->data(model->index(j, 4)).toDouble(); 

				xlsx.write(i+34, 1, model->data(model->index(j, 0)).toString(), format); //CINEMA HALL

				if(fabs(std_white_ftl - white_ftl)>3)
				{format.setFontColor(QColor("#ff0000"));}
				else 
				{format.setFontColor(QColor("#000000"));}
				xlsx.write(i+34, 2, model->data(model->index(j, 1)).toString(), format); // white ftl
				xlsx.mergeCells(QXlsx::CellRange(i+34, 2, i+34, 3), format);

				if(fabs(std_red_ftl - red_ftl)>3)
				{format.setFontColor(QColor("#ff0000"));}
				else 
				{format.setFontColor(QColor("#000000"));}
				xlsx.write(i+34, 4, model->data(model->index(j, 2)).toString(), format); // red ftl
				xlsx.mergeCells(QXlsx::CellRange(i+34, 4, i+34, 5), format);

				if(fabs(std_green_ftl - green_ftl)>3)
				{format.setFontColor(QColor("#ff0000"));}
				else 
				{format.setFontColor(QColor("#000000"));}
				xlsx.write(i+34, 6, model->data(model->index(j, 3)).toString(), format); // green ftl
				xlsx.mergeCells(QXlsx::CellRange(i+34, 6, i+34, 7), format);

				if(fabs(std_blue_ftl - blue_ftl)>3)
				{format.setFontColor(QColor("#ff0000"));}
				else 
				{format.setFontColor(QColor("#000000"));}
				xlsx.write(i+34, 8, model->data(model->index(j, 4)).toString(), format); // blue ftl
				xlsx.mergeCells(QXlsx::CellRange(i+34, 8, i+34, 9), format);
				format.setFontColor(QColor("#000000"));
				xlsx.write(i+34, 10, model->data(model->index(j, 5)).toDateTime().toString("hh:mm:ss"), format); // stime
			}
		}
		else
		{
			format.setFontColor(QColor("#000000"));
			xlsx.write(i+34, 1, itemList.at(i).strTheaterNo, format); //CINEMA HALL

			xlsx.write(i+34, 2, "N/A", format); // white ftl
			xlsx.mergeCells(QXlsx::CellRange(i+34, 2, i+34, 3), format);

			xlsx.write(i+34, 4, "N/A", format); // red ftl
			xlsx.mergeCells(QXlsx::CellRange(i+34, 4, i+34, 5), format);

			xlsx.write(i+34, 6, "N/A", format); // green ftl
			xlsx.mergeCells(QXlsx::CellRange(i+34, 6, i+34, 7), format);

			xlsx.write(i+34, 8, "N/A", format); // blue ftl
			xlsx.mergeCells(QXlsx::CellRange(i+34, 8, i+34, 9), format);
			//
			xlsx.write(i+34, 10,"N/A", format); // stime
		}
		delete model;
		model = NULL;
		delete stdModel;
		stdModel = NULL;
	}

}

void Excel::setLastChroma()
{
	// 最近七天的 色域
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
		xlsx.write(118+i*11, 1, "Test result of Chroma", format);
		xlsx.mergeCells(QXlsx::CellRange(118+i*11, 1, 118+i*11, 10), format);
		format.setFontColor(QColor("#000000"));
		format.setFontBold(false);
		format.setHorizontalAlignment(QXlsx::Format::AlignHCenter);
		format.setPatternBackgroundColor(QColor("#CCC1D9"));
		xlsx.write(119+i*11, 1, "", format);
		xlsx.write(119+i*11, 2, "WHITE(0.314, 0.351)", format);
		xlsx.mergeCells(QXlsx::CellRange(119+i*11, 2, 119+i*11, 3), format);
		xlsx.write(119+i*11, 4, "RED(0.680, 0.320)", format);
		xlsx.mergeCells(QXlsx::CellRange(119+i*11, 4, 119+i*11, 5), format);

		xlsx.write(119+i*11, 6, "GREE(0.265, 0.690)", format );
		xlsx.mergeCells(QXlsx::CellRange(119+i*11, 6, 119+i*11, 7), format);

		xlsx.write(119+i*11, 8, "BLUE(0.150, 0.060)", format);
		xlsx.mergeCells(QXlsx::CellRange(119+i*11, 8, 119+i*11, 9), format);

		xlsx.write(119+i*11, 10, "", format);
		xlsx.write(120+i*11, 1, "Hall name", format);
		xlsx.write(120+i*11, 2, "x", format);
		xlsx.write(120+i*11, 3, "y", format);
		xlsx.write(120+i*11, 4, "x", format);
		xlsx.write(120+i*11, 5, "y", format);
		xlsx.write(120+i*11, 6, "x", format);
		xlsx.write(120+i*11, 7, "y", format);
		xlsx.write(120+i*11, 8, "x", format);
		xlsx.write(120+i*11, 9, "y", format);
		xlsx.write(120+i*11, 10, "Test date", format);
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
				" GROUP BY date(stime) ORDER BY stime DESC;").arg(itemList.at(i).strTheaterNo).arg(QDate::currentDate().addDays(-j).toString("yyyy-M-d"));
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
			if(model->rowCount() > 0 )
			{
				xlsx.write(j+121+i*11, 1, model->data(model->index(0, 0)).toString(), format); //CINEMA HALL
				double wx = model->data(model->index(0, 0)).toDouble(); 
				double wy = model->data(model->index(0, 1)).toDouble(); 
				double rx = model->data(model->index(0, 2)).toDouble(); 
				double ry = model->data(model->index(0, 3)).toDouble(); 
				double gx = model->data(model->index(0, 4)).toDouble(); 
				double gy = model->data(model->index(0, 5)).toDouble(); 
				double bx = model->data(model->index(0, 6)).toDouble(); 
				double by = model->data(model->index(0, 7)).toDouble(); 
				if(fabs(std_wx - wx) > 0.03 )											// wx
				{format.setFontColor(QColor("#ff0000"));}
				else
				{format.setFontColor(QColor("#000000"));}
				xlsx.write(j+121+i*11, 2, model->data(model->index(0, 1)).toString(), format);  

				if(fabs(std_wy - wy) > 0.03 )											 // wy
				{format.setFontColor(QColor("#ff0000"));}
				else
				{format.setFontColor(QColor("#000000"));}	
				xlsx.write(j+121+i*11, 3, model->data(model->index(0, 2)).toString(), format); 

				if(fabs(std_rx - rx) > 0.03 )											// rx
				{format.setFontColor(QColor("#ff0000"));}
				else
				{format.setFontColor(QColor("#000000"));}
				xlsx.write(j+121+i*11, 4, model->data(model->index(0, 3)).toString(), format);  

				if(fabs(std_ry - ry) > 0.03 )										   // ry
				{format.setFontColor(QColor("#ff0000"));}
				else
				{format.setFontColor(QColor("#000000"));}
				xlsx.write(j+121+i*11, 5, model->data(model->index(0, 4)).toString(), format);  

				if(fabs(std_gx - gx) > 0.03 )											// gx
				{format.setFontColor(QColor("#ff0000"));}
				else
				{format.setFontColor(QColor("#000000"));}
				xlsx.write(j+121+i*11, 6, model->data(model->index(0, 5)).toString(), format);  

				if(fabs(std_gy - gy) > 0.03 )											// gy									
				{format.setFontColor(QColor("#ff0000"));}
				else
				{format.setFontColor(QColor("#000000"));}
				xlsx.write(j+121+i*11, 7, model->data(model->index(0, 6)).toString(), format);  

				if(fabs(std_bx - bx) > 0.03 )											//bx
				{format.setFontColor(QColor("#ff0000"));}
				else
				{format.setFontColor(QColor("#000000"));}
				xlsx.write(j+121+i*11, 8, model->data(model->index(0, 7)).toString(), format); 

				if(fabs(std_by - by) > 0.03 )											// by
				{format.setFontColor(QColor("#ff0000"));}
				else
				{format.setFontColor(QColor("#000000"));}
				xlsx.write(j+121+i*11, 9, model->data(model->index(0, 8)).toString(), format); 
				
				format.setFontColor(QColor("#000000"));
				xlsx.write(j+121+i*11, 10, model->data(model->index(0, 9)).toDateTime().toString("M/d"), format); //stime
			}
			else
			{
				format.setFontColor(QColor("#000000"));
				xlsx.write(j+121+i*11, 1, itemList.at(i).strTheaterNo, format); //CINEMA HALL
				xlsx.write(j+121+i*11, 2, "N/A", format); // wx
				xlsx.write(j+121+i*11, 3, "N/A", format); // wy
				xlsx.write(j+121+i*11, 4, "N/A", format); // rx
				xlsx.write(j+121+i*11, 5, "N/A", format); // ry
				xlsx.write(j+121+i*11, 6, "N/A", format); // gx
				xlsx.write(j+121+i*11, 7, "N/A", format); // gy
				xlsx.write(j+121+i*11, 8, "N/A", format); //bx
				xlsx.write(j+121+i*11, 9, "N/A", format); //by
				xlsx.write(j+121+i*11, 10, QDate::currentDate().addDays(-j).toString("M/d"), format); //stime
			}
			delete model;
			model = NULL;
			delete stdModel;
			stdModel = NULL;
		}
	}

}

void Excel::setLastLuminance()
{
	// 最近七天的 亮度


	for(int i = 0; i<itemList.size(); i++ )
	{
		format.setPatternBackgroundColor("#F79646");
		format.setFontBold(false);
		format.setFontName("Book Antiqua");
		format.setFontSize(12);
		format.setFontColor(QColor("#ffffff"));
		format.setHorizontalAlignment(QXlsx::Format::AlignHCenter);
		format.setBorderColor(QColor("#F79646"));
		xlsx.write(195+i*10, 1, "Test result of Luminance", format);
		xlsx.mergeCells(QXlsx::CellRange(195+i*10, 1, 195+i*10, 10), format);
		//header
		format.setFontColor(QColor("#000000"));
		format.setFontBold(false);
		format.setHorizontalAlignment(QXlsx::Format::AlignHCenter);
		format.setPatternBackgroundColor(QColor("#FBD5B5"));
		xlsx.write(196+i*10, 1, "Hall name", format);
		xlsx.write(196+i*10, 2, "WHITE(14ftL)", format);
		xlsx.mergeCells(QXlsx::CellRange(196+i*10, 2, 196+i*10, 3), format);

		xlsx.write(196+i*10, 4, "RED(2.95ftL)", format);
		xlsx.mergeCells(QXlsx::CellRange(196+i*10, 4, 196+i*10, 5), format);

		xlsx.write(196+i*10, 6, "GREEN(10.11ftL)", format);
		xlsx.mergeCells(QXlsx::CellRange(196+i*10, 6, 196+i*10, 7), format);

		xlsx.write(196+i*10, 8, "BLUE(0.97ftL)", format);
		xlsx.mergeCells(QXlsx::CellRange(196+i*10, 8, 196+i*10, 9), format);

		xlsx.write(196+i*10, 10, "Test date", format);
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
			if(model->rowCount() > 0)
			{
				double white_ftl = model->data(model->index(0, 1)).toDouble(); 
				double red_ftl = model->data(model->index(0, 2)).toDouble(); 
				double green_ftl = model->data(model->index(0, 3)).toDouble(); 
				double blue_ftl = model->data(model->index(0, 4)).toDouble(); 

				xlsx.write(j+197+i*10, 1, model->data(model->index(0, 0)).toString(), format); //CINEMA HALL
				if(fabs(std_white_ftl - white_ftl)>3)
				{format.setFontColor(QColor("#ff0000"));}
				else 
				{format.setFontColor(QColor("#000000"));}
				xlsx.write(j+197+i*10, 2, model->data(model->index(0, 1)).toString(), format); // white ftl
				xlsx.mergeCells(QXlsx::CellRange(j+197+i*10, 2, j+197+i*10, 3), format);

				if(fabs(std_red_ftl - red_ftl)>3)
				{format.setFontColor(QColor("#ff0000"));}
				else 
				{format.setFontColor(QColor("#000000"));}
				xlsx.write(j+197+i*10, 4, model->data(model->index(0, 2)).toString(), format); // red ftl
				xlsx.mergeCells(QXlsx::CellRange(j+197+i*10, 4, j+197+i*10, 5), format);

				if(fabs(std_green_ftl - green_ftl)>3)
				{format.setFontColor(QColor("#ff0000"));}
				else 
				{format.setFontColor(QColor("#000000"));}
				xlsx.write(j+197+i*10, 6, model->data(model->index(0, 3)).toString(), format); // green ftl
				xlsx.mergeCells(QXlsx::CellRange(j+197+i*10, 6, j+197+i*10, 7), format);

				if(fabs(std_blue_ftl - blue_ftl)>3)
				{format.setFontColor(QColor("#ff0000"));}
				else 
				{format.setFontColor(QColor("#000000"));}
				xlsx.write(j+197+i*10, 8, model->data(model->index(0, 4)).toString(), format); // blue ftl
				xlsx.mergeCells(QXlsx::CellRange(j+197+i*10, 8, j+197+i*10, 9), format);

				format.setFontColor(QColor("#000000"));
				xlsx.write(j+197+i*10, 10, model->data(model->index(0, 5)).toDateTime().toString("M/d"), format); // stime
			}
			else
			{
				format.setFontColor(QColor("#000000"));
				xlsx.write(j+197+i*10, 1, itemList.at(i).strTheaterNo, format); //CINEMA HALL

				xlsx.write(j+197+i*10, 2, "N/A", format); // white ftl
				xlsx.mergeCells(QXlsx::CellRange(j+197+i*10, 2, j+197+i*10, 3), format);

				xlsx.write(j+197+i*10, 4, "N/A", format); // red ftl
				xlsx.mergeCells(QXlsx::CellRange(j+197+i*10, 4, j+197+i*10, 5), format);

				xlsx.write(j+197+i*10, 6, "N/A", format); // green ftl
				xlsx.mergeCells(QXlsx::CellRange(j+197+i*10, 6, j+197+i*10, 7), format);

				xlsx.write(j+197+i*10, 8, "N/A", format); // blue ftl
				xlsx.mergeCells(QXlsx::CellRange(j+197+i*10, 8, j+197+i*10, 9), format);
				xlsx.write(j+197+i*10, 10, QDate::currentDate().addDays(-j).toString("M/d"), format); // stime
			}
			delete model;
			model = NULL;
			delete stdModel;
			stdModel = NULL;
		}
	}

}

void Excel::setLastSPL()
{
	// The test results of last 7 days are below:
	format.setPatternBackgroundColor(QColor("#fff"));
	format.setHorizontalAlignment(QXlsx::Format::AlignLeft);
	format.setFontBold(true);
	format.setFontSize(16);
	format.setFontColor(QColor("#000"));
	format.setBorderColor(QColor("#ffffff"));
	xlsx.write(45, 1, "The test results of last 7 days are below:", format);
	xlsx.mergeCells("A45:J45", format);
	//最近七天声压
	for(int i = 0; i<itemList.size(); i++)
	{
		//header
		format.setPatternBackgroundColor(QColor("#9BBB59"));
		format.setFontColor(QColor("#fff"));
		format.setFontSize(12);
		format.setFontBold(false);
		format.setHorizontalAlignment(QXlsx::Format::AlignHCenter);
		xlsx.write(47+i*10, 1, "Test result of SPL (dBC) - DCI requierment is 85dBC", format);
		xlsx.mergeCells(QXlsx::CellRange(47+i*10, 1, 47+i*10, 10), format);
		format.setFontColor(QColor("#000"));
		format.setPatternBackgroundColor(QColor("#D7E3BC"));
		xlsx.write(48+i*10, 1, "Hall name", format);
		xlsx.write(48+i*10, 2, "Left", format);
		xlsx.write(48+i*10, 3, "Right", format);
		xlsx.write(48+i*10, 4, "Center", format);
		xlsx.write(48+i*10, 5, "LFE", format);
		xlsx.write(48+i*10, 6, "LS", format);
		xlsx.write(48+i*10, 7, "RS", format);
		xlsx.write(48+i*10, 8, "LSS", format);
		xlsx.write(48+i*10, 9, "RSS", format);
		xlsx.write(48+i*10, 10, "Test date", format);

		format.setFontSize(12);
		format.setPatternBackgroundColor(QColor("#fff"));
		format.setFontBold(false);
		for(int j = 0; j<7; j++)
		{
			if(j%2 == 0)
			{
				format.setPatternBackgroundColor(QColor("#fffff"));
				format.setBorderStyle(QXlsx::Format::BorderThin);
				format.setBorderColor(QColor("#D7E3BC"));
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

			if(model->rowCount() > 0)
			{
				xlsx.write(j+49+i*10, 1, model->data(model->index(0, 0)).toString(), format );
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
				xlsx.write(j+49+i*10, 2, model->data(model->index(0, 1)).toString(), format );

				if(fabs(std_right - right ) > 5)
				{
					format.setFontColor(QColor("#ff0000")); 
				}
				else
				{
					format.setFontColor(QColor("#000000")); 
				}
				xlsx.write(j+49+i*10, 3, model->data(model->index(0, 2)).toString(), format );

				if(fabs(std_center - center ) > 5)
				{
					format.setFontColor(QColor("#ff0000")); 
				}
				else
				{
					format.setFontColor(QColor("#000000")); 
				}
				xlsx.write(j+49+i*10, 4, model->data(model->index(0, 3)).toString(), format );

				if(fabs(std_lfe - lfe ) > 5)
				{
					format.setFontColor(QColor("#ff0000")); 
				}
				else
				{
					format.setFontColor(QColor("#000000")); 
				}
				xlsx.write(j+49+i*10, 5, model->data(model->index(0, 4)).toString(), format );

				if(fabs(std_ls - ls ) > 5)
				{
					format.setFontColor(QColor("#ff0000")); 
				}
				else
				{
					format.setFontColor(QColor("#000000")); 
				}
				xlsx.write(j+49+i*10, 6, model->data(model->index(0, 5)).toString(), format );

				if(fabs(std_rs - rs ) > 5)
				{
					format.setFontColor(QColor("#ff0000")); 
				}
				else
				{
					format.setFontColor(QColor("#000000")); 
				}
				xlsx.write(j+49+i*10, 7, model->data(model->index(0, 6)).toString(), format );

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
				xlsx.write(j+49+i*10, 8, model->data(model->index(0, 7)).toString(), format );

				if(fabs(std_rss - rss ) > 5)
				{
					format.setFontColor(QColor("#ff0000")); 
				}
				else
				{
					format.setFontColor(QColor("#000000")); 
				}
				xlsx.write(j+49+i*10, 9, model->data(model->index(0, 8)).toString(), format );

				format.setFontColor(QColor("#000000"));
				xlsx.write(j+49+i*10, 10, model->data(model->index(0, 9)).toDate().toString("M/d"), format );
			}
			else
			{
				format.setFontColor(QColor("#000000"));
				xlsx.write(j+49+i*10, 1, itemList.at(i).strTheaterNo, format );
				xlsx.write(j+49+i*10, 2, "N/A", format );
				xlsx.write(j+49+i*10, 3, "N/A", format );
				xlsx.write(j+49+i*10, 4, "N/A", format );
				xlsx.write(j+49+i*10, 5, "N/A", format );
				xlsx.write(j+49+i*10, 6, "N/A", format );
				xlsx.write(j+49+i*10, 7, "N/A", format );
				xlsx.write(j+49+i*10, 8, "N/A", format );
				xlsx.write(j+49+i*10, 9, "N/A", format );
				xlsx.write(j+49+i*10, 10, QDate::currentDate().addDays(-j).toString("M/d"), format );
			}
			delete model;
			model = NULL;
			delete stdModel;
			stdModel = NULL;
		}
	}

}



void Excel::SaveAs(QString &fileName)
{
	xlsx.saveAs(fileName);
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