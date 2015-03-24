#pragma once

#include "ConfigStruct.h"
#include "Log.h"
#include "Database.h"

#include <Qthread>

class IReport
{
public:
	virtual void SetSmtpSetting(SMTPSETTING ds) = 0;
	virtual void Start() = 0;
	virtual void Stop() = 0;
	virtual void Exit() = 0;
	virtual bool IsRun() = 0;
};

IReport * CreateReport();
void ReleaseReport(IReport* pReport);
