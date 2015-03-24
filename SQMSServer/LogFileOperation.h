#pragma once
#include "LogStruct.h"
#include <stdio.h>
#include <stdlib.h>

class CLogFileOperation 
{
public:
	CLogFileOperation(void);
	~CLogFileOperation(void);

public:
	bool IsDirectory(const char* path);
	bool CreateDirectory(const char* path);
	bool DeleteDirectory(const char* path);

	bool OpenFile(const char* file);
	bool CloseFile();

	bool Write(const char* file,const int type, const char* context);

private:
	FILE* m_pfile;

};
