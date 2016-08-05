#include "stdafx.h"
#include "../inc/EmHandy.h"
#include "../inc/EmUtil.h"
#include "../inc/EmTime.h"
#include "../inc/EmSz.h"
#include "../inc/EmFile.h"
#include <afx.h>

using namespace em;
using namespace std;

__int64 em::EmHandy::GenerateLongKey()
{
	__int64 iResult = 0;

	EmTime xTime;
	xTime.LoadCurrentTime();
	iResult = xTime.CurrentStamp();
	iResult *= 1000;
	iResult += xTime.CurrentMillisec();
	iResult *= 10000;
	iResult += EmUtil::Rand(1000,9999);

	return iResult;
}

std::string em::EmHandy::GenerateFileName( const char* szDir, const char *szExt )
{
	string strResult;
	if(szDir == NULL || szExt == NULL || strlen(szDir) > 400){
		return strResult;
	}

	char szTempFileName[500] = {0};
	strcpy(szTempFileName,szDir);
	EmSz::Replace(szTempFileName,"\\","/");
	int iTempLen = strlen(szTempFileName);
	if(szTempFileName[iTempLen - 1] != '/'){
		szTempFileName[iTempLen] = '/';
	}

	char szLongKey[50] = {0};
	INT64 iLongKey = EmHandy::GenerateLongKey();
	EmSz::Long2Str(szLongKey,iLongKey);
	strcat(szTempFileName,szLongKey);
	strcat(szTempFileName,szExt);
	strResult = szTempFileName;
	return strResult;
}

void em::EmHandy::DebugTraceFile(const char *szFile, const char* szMsg )
{
	std::string strTemp;
	strTemp.append("\r\n");
	strTemp.append(szMsg);
	EmFile::AppendUtf8(szFile,strTemp.c_str(),EmEncodeUtf8);
}

void em::EmHandy::DebugAlert( const char* szMsg )
{
	::MessageBoxA(NULL,szMsg,"et debug",0);
}
