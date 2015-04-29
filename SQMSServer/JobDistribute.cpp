#include "JobDistribute.h"
#include "Database.h"
#include "Log.h"
#include "ConfigStruct.h"
#include "ModSelect.h"
#include "SISThread.h"
#include "Report.h"

#include <QThread>

static JOBSTATE jobState = STATE_NONE;
typedef QList<ISISThread *> LISTSIS;

class SQMSJobDistribute : public IJobDistribute, public QThread
{
public:
	void Start();
	void Stop();

	SQMSJobDistribute();
	~SQMSJobDistribute();

protected:
	void run();

	void ReloadConfig();
	void CreateTask();
	void Mainloop();

private:
	IDatabase* pDb;
	ILog* pLog;
	IModSelect* pMod;
	bool isRun;
	SMTPSETTING mSS;
	TQDevItemList mQDevList;
	LISTSIS mSISList;
	bool taskCreate;
	QList<IReport*> mRepList;
//	IReport * pRep;
};

IJobDistribute* CreateJobDistribute()
{
	return new SQMSJobDistribute;
}

void ReleaseJobDistribute(IJobDistribute* pJob)
{
	if (pJob)
	{
		delete pJob;
	}
}

SQMSJobDistribute::SQMSJobDistribute() : isRun(false), taskCreate(false)
{
	pLog = CreateLog();
	pDb = CreateDatabase();
// 	pMod = CreateModSelect();
	//pMod->Start();
}

SQMSJobDistribute::~SQMSJobDistribute()
{
	for (int i = 0; i < mRepList.size(); i++)
	{
		ReleaseReport(mRepList.at(i));
	}
	mRepList.clear();
	ReleaseDatabase(pDb);
	ReleaseLog(pLog);
	ReleaseModSelect(pMod);
}

void SQMSJobDistribute::Start()
{
	isRun = true;
	jobState = STATE_RELOAD;
	start();
}

void SQMSJobDistribute::Stop()
{
	isRun = false;
	terminate();
// 	pMod->Stop();
}

void SQMSJobDistribute::run()
{
	while(isRun)
	{
		switch(jobState)
		{
		case STATE_NONE:
			break;
		case STATE_RELOAD:
			ReloadConfig();
			jobState = STATE_CREATETASK;
			break;
		case STATE_CREATETASK:
			CreateTask();
			break;
		case STATE_MAINLOOP:
			Mainloop();
			break;
		default:
			break;
		}
		sleep(1);
	}
}

void SQMSJobDistribute::ReloadConfig()
{
	mSS = pDb->GetSmtpSetting();
	pDb->GetQDevList(mQDevList);
}

void SQMSJobDistribute::CreateTask()
{
	for (int i = 0; i < mRepList.size(); i++)
		{
		IReport* pRep = (IReport*)mRepList.front();
		mRepList.pop_front();
		if (!pRep->IsRun())
			ReleaseReport(pRep);
		else
			mRepList.push_back(pRep);
	}
	
	if(taskCreate == false)
	{
		for (int i = 0; i < mQDevList.size(); i++)
		{
			if(isTime(mQDevList.at(i).tBefore, mQDevList.at(i).tAfter, T_BEFORE_10MIN))
			{
				for (int j = 0; j < mQDevList.size(); j++)
				{
					if(mQDevList.at(j).mSyncMode == 0)
					{
						ISISThread* pItem = CreateSISThread();
						pItem->SetQDevice(mQDevList.at(j));
						mSISList.push_back(pItem);
					}
				}
				taskCreate = true;
				jobState = STATE_MAINLOOP;
				return;
			}
			return;
		}
	}
}

void SQMSJobDistribute::Mainloop()
{
	QTime currTime = QTime::currentTime();
	LISTSIS::iterator itor;

#if 0
	if (!pRep->IsRun())
	{
		//ReleaseReport(pRep);
		pRep = CreateReport();
		pRep->SetSmtpSetting(mSS);
		pRep->Start();
	}
#endif // 0

	for(int i = 0; i <mSISList.size(); i++)
	{
		if(mSISList.at(i)->isTime(currTime))
			mSISList.at(i)->Start();
		else
		{
			mSISList.at(i)->Stop();
		}
	}
	for (int i = 0; i < mSISList.size(); i++)
	{
		if (mSISList.at(i)->isTime(currTime, T_AFTER_10MIN))
		{
			itor = mSISList.begin() + i;
			ReleaseSISThread(mSISList.at(i));
			mSISList.erase(itor);
			break;
		}
	}
	if (mSISList.size() == 0)
	{
		IReport* pRep = CreateReport();
		pRep->SetSmtpSetting(mSS);
		pRep->Start();
		mRepList.push_back(pRep);
		taskCreate = false;
		jobState = STATE_CREATETASK;
	}
}