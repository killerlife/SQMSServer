#include "SISThread.h"
#include "Device.h"
#include "Log.h"
#include "Script.h"
#include "Database.h"

#include <QQueue>

#define SIS_DEBUG 1

class SISThread : public ISISThread, public QThread
{
public:
	void Start();
	void Stop();
	void SetQDevice(QMSDEVICE qDev);
	int GetRunState(){return nRunState;};
	bool isTime(QTime mTime, T_KIND nKind = T_IN_RANGE);

	SISThread();
	~SISThread();

protected:
	void run();
	bool SISCheck();

private:
	int nRunState;
	bool isRun;
	QMSDEVICE mQDev;
	IDevice* pDev;
	ILog * pLog;
	IScript* pScript;
	IDatabase* pDB;
	double white_ftl;
	QQueue<float> queue;
};

ISISThread* CreateSISThread()
{
	return new SISThread;
}

void ReleaseSISThread(ISISThread *pSIS)
{
	if (pSIS)
	{
		delete pSIS;
	}
}

SISThread::SISThread():isRun(false), nRunState(0), pDev(NULL), pDB(NULL), white_ftl(0)
{
	pLog = CreateLog();
	pDB = CreateDatabase();
}

SISThread::~SISThread()
{
	Stop();
	ReleaseDevice(pDev);
	ReleaseDatabase(pDB);
	ReleaseLog(pLog);
	ReleaseScript(pScript);
}

void SISThread::Start()
{
	isRun = true;
	if(nRunState == 0)
	{
		start();
		nRunState = 1;
	}
}

void SISThread::Stop()
{
	isRun = false;
	terminate();
}

void SISThread::SetQDevice(QMSDEVICE qDev)
{
	mQDev = qDev;
// 	switch(mQDev.mDevType)
// 	{
// 	case DEV_LLAS100:
// 		pDev = CreateDevice(DEVICE_LLAS100);
// 		pDev->SetIp(mQDev.strIp);
// 		break;
// 	}
}

void SISThread::run()
{
	switch(mQDev.mDevType)
	{
	case DEV_LLAS100:
		pDev = CreateDevice(DEVICE_LLAS100);
		pDev->SetIp(mQDev.strIp, mQDev.nPort);
		pScript = CreateScript();
		pScript->AddDevice(DEVICE_LLAS100, pDev);
		pScript->SetFixValue(mQDev);
		pScript->LoadScript(mQDev.strScript);
		//for test
// 		pScript->ExecScript();
// 		if(pDB)
// 			pDB->SaveCollectData(pScript->GetTestValueList(), mQDev);
		break;
	}

	while (isRun)
	{
		if(SISCheck() && nRunState == 1)
		{
			white_ftl = queue.at(0);
			for(int i = 0; i<queue.size(); i++)
			{
				if(white_ftl < queue.at(i))
				{
					white_ftl = queue.at(i); 
				}
			}
			pLog = CreateLog();
			pLog->Write(LOG_SISTHREAD, mQDev.strIp + ":" + "Get it!!");
			pScript->ExecScript();
			if (pDB)
			{
				LISTITEM item;
				item.name = "white_ftl";
				item.value = QString::number(white_ftl, 'g', 6);
				pScript->GetTestValueList()->push_back(item);
				pDB->SaveCollectData(pScript->GetTestValueList(), mQDev);
			}
			nRunState = 2;
// 			printf("%s: Got it!!\n", mQDev.strIp.toStdString().c_str());
// 			while(isRun)
// 			{
// 				pLog->Write(LOG_SISTHREAD, mQDev.strIp + ":" + "Get it!!");
// 				printf("%s \n", pDev->Read(DATA_FTL).toStdString().c_str());
// 				msleep(10);
// 			}
		}
		sleep(1);
	}
}

bool SISThread::isTime(QTime mTime, T_KIND nKind)
{
	int tBefore, tAfter, t;
	tBefore = mQDev.tBefore.hour()*3600 + mQDev.tBefore.minute()*60 + mQDev.tBefore.second();
	tAfter = mQDev.tAfter.hour()*3600 + mQDev.tAfter.minute()*60 + mQDev.tAfter.second();
	t = mTime.hour()*3600 + mTime.minute()*60 + mTime.second();
	
	if(mQDev.tBefore > mQDev.tAfter)
	{
		tAfter += 24*60*60;

		if(mTime < mQDev.tBefore)
		{
			t += 24*60*60;
		}
	}

	switch(nKind)
	{
	case T_IN_RANGE:
		if(t > tBefore && t < tAfter)
			return true;
		else
			return false;
	case T_AFTER_10MIN:
		if(t > (tAfter + 600))
			return true;
		else
			return false;
	case T_BEFORE_10MIN:
		if(t < tBefore && t > (tBefore - 600))
			return true;
		else
			return false;
	default:
		return false;
	}
}

bool isTime(QTime tBefore, QTime tAfter, T_KIND nKind)
{
	int nBefore, nAfter, t;
	QTime mTime = QTime::currentTime();
	nBefore = tBefore.hour()*3600 + tBefore.minute()*60 + tBefore.second();
	nAfter = tAfter.hour()*3600 + tAfter.minute()*60 + tAfter.second();
	t = mTime.hour()*3600 + mTime.minute()*60 + mTime.second();

	if(tBefore > tAfter)
	{
		nAfter += 24*60*60;

		if(mTime < tBefore)
		{
			t += 24*60*60;
		}
	}

	switch(nKind)
	{
	case T_IN_RANGE:
		if(t > nBefore && t < nAfter)
			return true;
		else
			return false;
	case T_AFTER_10MIN:
		if(t > nAfter && t < (nAfter + 600))
			return true;
		else
			return false;
	case T_BEFORE_10MIN:
		if(t > (nBefore - 600))
			return true;
		else
			return false;
	default:
		return false;
	}
}

bool SISThread::SISCheck()
{
	double ftl = 0.0;
	int count_white = 0;
	int count_black = 0;
	int cnt = 0;

	
	QQueue<int> q1, q2, q3, q4;
	int oneflag = 0;
	if (!pDev)
	{
		pLog->Write(LOG_SISTHREAD, tr("Device pointer is NULL"));
		return false;
	}
	queue.clear(); 
	while(isRun)
	{
		ftl = pDev->Read(DATA_FTL).toDouble();
		msleep(20);
		switch(oneflag)
		{
		case 0:
			if(q1.size()>90)
			{
				//q1.dequeue();
				q1.clear();
				queue.clear();
#ifdef SIS_DEBUG
   				pLog->Write(LOG_SISTHREAD, mQDev.strIp + ": reset 1");
#endif
			}
			if(ftl > 1)
			{
				q1.enqueue(1);
				queue.enqueue(ftl);
				
			}
			else if(ftl < 0.4 && q1.size() > 8)
			{
#ifdef SIS_DEBUG
  				pLog->Write(LOG_SISTHREAD, mQDev.strIp + ": goto 2");
#endif // SIS_DEBUG
				q2.clear();
				q2.enqueue(0);
				oneflag = 1;
			}
			else {
#ifdef SIS_DEBUG
  				pLog->Write(LOG_SISTHREAD, mQDev.strIp + ": unknow clear 1");
#endif // SIS_DEBUG
				q1.clear();
				queue.clear();
			}
			break;
		case 1:
			if(q2.size()>90)
			{
#ifdef SIS_DEBUG
 				pLog->Write(LOG_SISTHREAD, mQDev.strIp + ": reset 2");
#endif // SIS_DEBUG
				//q2.dequeue();
				q2.clear();
				q1.clear();
				queue.clear();
#ifdef SIS_DEBUG
  				pLog->Write(LOG_SISTHREAD, mQDev.strIp + ": goto 1");
#endif // SIS_DEBUG
				oneflag = 0;
				break;
			}
			if(ftl < 0.4)
				q2.enqueue(0);
			else if(ftl > 1 && q2.size() > 12)
			{
#ifdef SIS_DEBUG
 				pLog->Write(LOG_SISTHREAD, mQDev.strIp + ": goto 3");
#endif // SIS_DEBUG
				q3.clear();
				q3.enqueue(1);
				oneflag = 2;
			}
			else {
				q1.clear();
				queue.clear();
				oneflag = 0;
#ifdef SIS_DEBUG
 				pLog->Write(LOG_SISTHREAD, mQDev.strIp + ": unknow clear 2");
#endif // SIS_DEBUG
			}
			break;
		case 2:
			if(q3.size()>90)
			{
#ifdef SIS_DEBUG
 				pLog->Write(LOG_SISTHREAD, mQDev.strIp + ": reset 3");
#endif // SIS_DEBUG
				//q3.dequeue();
				q3.clear();
				q1.clear();
				q2.clear();
#ifdef SIS_DEBUG
 				pLog->Write(LOG_SISTHREAD, mQDev.strIp + ": goto 1");
#endif // SIS_DEBUG
				oneflag = 0;
			}
			if(ftl > 1)
				q3.enqueue(1);
			else if(ftl < 0.4 && q3.size() > 8)
			{
#ifdef SIS_DEBUG
 				pLog->Write(LOG_SISTHREAD, mQDev.strIp + ": goto 4");
#endif // SIS_DEBUG
				q4.clear();
				q4.enqueue(0);
#if 0
				oneflag = 3;
#else
				return true;
#endif
			}
			else
			{
#ifdef SIS_DEBUG
 				pLog->Write(LOG_SISTHREAD, mQDev.strIp + ": unknow clear 3");
#endif // SIS_DEBUG
				q1.clear();
				queue.clear();
				oneflag = 0;
			}
			break;
#if 0
		case 3:
			if(q4.size()>90)
			{
#ifdef SIS_DEBUG
 				pLog->Write(LOG_SISTHREAD, mQDev.strIp + ": reset 3");
#endif // SIS_DEBUG
				q4.clear();
				q1.clear();
				q2.clear();
				q3.clear();
				queue.clear();
				//q4.dequeue();
#ifdef SIS_DEBUG
 				pLog->Write(LOG_SISTHREAD, mQDev.strIp + ": goto 1");
#endif // SIS_DEBUG
				oneflag = 0;
			}
			if(ftl < 0.4)
			{
				q4.enqueue(0);
// 				if(q1.size() > 8 && q2.size() > 12 && q3.size() > 8 && q4.size() > 12)
// 				{
// 					while(isRun)
// 					{
// 						ftl = pDev->Read(DATA_FTL).toDouble();
// 						if(ftl > 0.4)
// 							return true;
// 						msleep(10);
// 					}
// 				}
			}
			else if(ftl > 0.4 && q4.size() > 12 && q4.size() < 90)
			{
				return true;
// 				if(q1.size() > 8 && q2.size() > 12 && q3.size() > 8 && q4.size() > 12)
// 				{
// 					while(isRun)
// 					{
// 						ftl = pDev->Read(DATA_FTL).toDouble();
// 						if(ftl > 0.4)
// 							return true;
// 						msleep(10);
// 					}
// 				}
			}
			else
			{
#ifdef SIS_DEBUG
 				pLog->Write(LOG_SISTHREAD, mQDev.strIp + ": unknow clear 4");
#endif // SIS_DEBUG
				q1.clear();
				queue.clear();
				q1.enqueue(1);
				oneflag = 0;
			}
			break;
#endif
		}
	}
	return false;
}
