#pragma once

class IJobDistribute
{
public:
	virtual void Start() = 0;
	virtual void Stop() = 0;
};

IJobDistribute* CreateJobDistribute();
void ReleaseJobDistribute(IJobDistribute* pJob);
