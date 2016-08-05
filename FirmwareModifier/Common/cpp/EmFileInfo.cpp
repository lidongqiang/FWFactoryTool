#include "stdafx.h"
#include "../inc/EmFileInfo.h"
#include "../inc/EmStlStr.h"
#include "../inc/EmTstr.h"
#include "../inc/EmFile.h"
#include <afx.h>

using namespace em;
using namespace std;


em::EmTime em::EmFileInfo::GetAccessTime( const char* szFileName )
{
	EmTime xTime;
	WIN32_FIND_DATA xFindData;
	memset(&xFindData,0,sizeof(xFindData));
	HANDLE hFild = ::FindFirstFile(EmTstr::FromUtf8(szFileName).Sz(),&xFindData);
	::FindClose(hFild);
	if(hFild <= 0){
		return xTime;
	}

	EmTime::FileTimeToObject(&xTime,&xFindData.ftLastAccessTime);
	return xTime;
}

em::EmTime em::EmFileInfo::GetModifyTime( const char* szFileName )
{
	EmTime xTime;
	WIN32_FIND_DATA xFindData;
	memset(&xFindData,0,sizeof(xFindData));
	HANDLE hFild = ::FindFirstFile(EmTstr::FromUtf8(szFileName).Sz(),&xFindData);
	::FindClose(hFild);
	if(hFild <= 0){
		return xTime;
	}
	
	EmTime::FileTimeToObject(&xTime,&xFindData.ftLastWriteTime);
	return xTime;
}

em::EmTime em::EmFileInfo::GetCreateTime( const char* szFileName )
{
	EmTime xTime;
	WIN32_FIND_DATA xFindData;
	memset(&xFindData,0,sizeof(xFindData));
	HANDLE hFild = ::FindFirstFile(EmTstr::FromUtf8(szFileName).Sz(),&xFindData);
	::FindClose(hFild);
	if(hFild <= 0){
		return xTime;
	}
	
	EmTime::FileTimeToObject(&xTime,&xFindData.ftCreationTime);
	return xTime;
}

int em::EmFileInfo::UpdateAccessTime( const char* szFileName, EmTime& xTime )
{
	if(!EmFile::IsExisted(szFileName)){
		return EM_ERR_FILE_LOST;
	}

	EmTime xEtAccessTime = xTime;
	EmTime xEtModifyTime = GetModifyTime(szFileName);
	EmTime xEtCreateTime = GetCreateTime(szFileName);

	HANDLE hFile= ::CreateFile(EmTstr::FromUtf8(szFileName).Sz()
		,GENERIC_WRITE,0,NULL,OPEN_EXISTING,FILE_FLAG_BACKUP_SEMANTICS,NULL);

	if(hFile <= 0){
		return EM_ERR_FILE_OPEN;
	}

	FILETIME xFtAccessTime;
	FILETIME xFtModifyTime;
	FILETIME xFtCreateTime;

	EmTime::ObjectToFileTime(&xFtAccessTime,&xEtAccessTime);
	EmTime::ObjectToFileTime(&xFtModifyTime,&xEtModifyTime);
	EmTime::ObjectToFileTime(&xFtCreateTime,&xEtCreateTime);

	::SetFileTime(hFile,&xFtCreateTime,&xFtAccessTime,&xFtModifyTime);

	return 0;
}

int em::EmFileInfo::UpdateModifyTime( const char* szFileName, EmTime& xTime )
{
	if(!EmFile::IsExisted(szFileName)){
		return EM_ERR_FILE_LOST;
	}
	
	EmTime xEtAccessTime = GetAccessTime(szFileName);
	EmTime xEtModifyTime = xTime;
	EmTime xEtCreateTime = GetCreateTime(szFileName);
	
	HANDLE hFile= ::CreateFile(EmTstr::FromUtf8(szFileName).Sz()
		,GENERIC_WRITE,0,NULL,OPEN_EXISTING,FILE_FLAG_BACKUP_SEMANTICS,NULL);
	
	if(hFile <= 0){
		return EM_ERR_FILE_OPEN;
	}
	
	FILETIME xFtAccessTime;
	FILETIME xFtModifyTime;
	FILETIME xFtCreateTime;
	
	EmTime::ObjectToFileTime(&xFtAccessTime,&xEtAccessTime);
	EmTime::ObjectToFileTime(&xFtModifyTime,&xEtModifyTime);
	EmTime::ObjectToFileTime(&xFtCreateTime,&xEtCreateTime);
	
	::SetFileTime(hFile,&xFtCreateTime,&xFtAccessTime,&xFtModifyTime);
	
	return 0;
}

int em::EmFileInfo::UpdateCreateTime( const char* szFileName, EmTime& xTime )
{
	if(!EmFile::IsExisted(szFileName)){
		return EM_ERR_FILE_LOST;
	}
	
	EmTime xEtAccessTime = GetAccessTime(szFileName);
	EmTime xEtModifyTime = GetModifyTime(szFileName);
	EmTime xEtCreateTime = xTime;
	
	HANDLE hFile= ::CreateFile(EmTstr::FromUtf8(szFileName).Sz()
		,GENERIC_WRITE,0,NULL,OPEN_EXISTING,FILE_FLAG_BACKUP_SEMANTICS,NULL);
	
	if(hFile <= 0){
		return EM_ERR_FILE_OPEN;
	}
	
	FILETIME xFtAccessTime;
	FILETIME xFtModifyTime;
	FILETIME xFtCreateTime;
	
	EmTime::ObjectToFileTime(&xFtAccessTime,&xEtAccessTime);
	EmTime::ObjectToFileTime(&xFtModifyTime,&xEtModifyTime);
	EmTime::ObjectToFileTime(&xFtCreateTime,&xEtCreateTime);
	
	::SetFileTime(hFile,&xFtCreateTime,&xFtAccessTime,&xFtModifyTime);
	
	return 0;
}
