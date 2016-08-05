#include "stdafx.h"
#include "../inc/EmMemory.h"

#include <afx.h>



void* em_alloc(void **ppAddr,int iCount, char* szFile, int iLine)
{
	char szLine[20] = {0};
	char szAllocPlace[500] = {0};
	char szLogDir[500] = {0};
	char szLogFile[20] = {0};

	bool bIsDir = false;
	DWORD dwAttrib = 0;
	char szLogFullPath[500] = {0};
	HANDLE hFile = NULL;
	DWORD dwWritten = 0;

	*ppAddr = malloc(iCount);
	
	_itoa(iLine,szLine,10);
	
	strcat(szAllocPlace,szLine);
	strcat(szAllocPlace,",");
	strcat(szAllocPlace,szFile);

	::GetCurrentDirectoryA(500,szLogDir);
	
	_itoa((int)*ppAddr,szLogFile,10);
	strcat(szLogFile,".alloc");
	strcat(szLogDir,"\\memory_trace\\");

	dwAttrib = ::GetFileAttributesA(szLogDir);
	bIsDir =(dwAttrib != -1 
		&& (dwAttrib & 0x10));
	if(!bIsDir)
	{
		::CreateDirectoryA(szLogDir,NULL);
	}
	
	strcat(szLogFullPath,szLogDir);
	strcat(szLogFullPath,szLogFile);

	hFile = ::CreateFileA(szLogFullPath
		, GENERIC_READ | GENERIC_WRITE
		, FILE_SHARE_READ | FILE_SHARE_WRITE
		,NULL
		,CREATE_ALWAYS
		,FILE_ATTRIBUTE_NORMAL
		,NULL);
	if((int)hFile == 0 || (int)hFile == -1)
	{
		return *ppAddr;
	}
	::WriteFile(hFile,szAllocPlace,strlen(szAllocPlace),&dwWritten,NULL);
	::CloseHandle(hFile);

	return *ppAddr;
}

void em_free( void **ppAddr )
{
	char szLine[20] = {0};
	char szLogDir[500] = {0};
	char szLogFile[20] = {0};
	bool bIsDir = false;
	DWORD dwAttrib = 0;
	char szLogFullPath[500] = {0};

	if(*ppAddr == NULL)
	{
		return;
	}

	::GetCurrentDirectoryA(500,szLogDir);
	
	_itoa((int)*ppAddr,szLogFile,10);
	strcat(szLogFile,".alloc");
	strcat(szLogDir,"\\memory_trace\\");
	
	dwAttrib = ::GetFileAttributesA(szLogDir);
	bIsDir =(dwAttrib != -1 
		&& (dwAttrib & 0x10));
	if(bIsDir)
	{
		strcat(szLogFullPath,szLogDir);
		strcat(szLogFullPath,szLogFile);
		::DeleteFileA(szLogFullPath);
	}
	
	free(*ppAddr);
	*ppAddr = NULL;

}

void em_new( void **ppAddr, char* szFile, int iLine )
{
	char szLine[20] = {0};
	char szAllocPlace[500] = {0};
	char szLogDir[500] = {0};
	char szLogFile[20] = {0};
	
	bool bIsDir = false;
	DWORD dwAttrib = 0;
	char szLogFullPath[500] = {0};
	HANDLE hFile = NULL;
	DWORD dwWritten = 0;
	
	_itoa(iLine,szLine,10);
	
	strcat(szAllocPlace,szLine);
	strcat(szAllocPlace,",");
	strcat(szAllocPlace,szFile);
	
	::GetCurrentDirectoryA(500,szLogDir);
	
	_itoa((int)*ppAddr,szLogFile,10);
	strcat(szLogFile,".new");
	strcat(szLogDir,"\\memory_trace\\");
	
	dwAttrib = ::GetFileAttributesA(szLogDir);
	bIsDir =(dwAttrib != -1 
		&& (dwAttrib & 0x10));
	if(!bIsDir)
	{
		::CreateDirectoryA(szLogDir,NULL);
	}
	
	strcat(szLogFullPath,szLogDir);
	strcat(szLogFullPath,szLogFile);
	
	hFile = ::CreateFileA(szLogFullPath
		, GENERIC_READ | GENERIC_WRITE
		, FILE_SHARE_READ | FILE_SHARE_WRITE
		,NULL
		,CREATE_ALWAYS
		,FILE_ATTRIBUTE_NORMAL
		,NULL);
	if((int)hFile == 0 || (int)hFile == -1)
	{
		return;
	}
	::WriteFile(hFile,szAllocPlace,strlen(szAllocPlace),&dwWritten,NULL);
	::CloseHandle(hFile);
}

void em_delete( void **ppAddr )
{
	char szLine[20] = {0};
	char szLogDir[500] = {0};
	char szLogFile[20] = {0};
	bool bIsDir = false;
	DWORD dwAttrib = 0;
	char szLogFullPath[500] = {0};
	
	if(*ppAddr == NULL)
	{
		return;
	}
	
	::GetCurrentDirectoryA(500,szLogDir);
	
	_itoa((int)*ppAddr,szLogFile,10);
	strcat(szLogFile,".new");
	strcat(szLogDir,"\\memory_trace\\");
	
	dwAttrib = ::GetFileAttributesA(szLogDir);
	bIsDir =(dwAttrib != -1 
		&& (dwAttrib & 0x10));
	if(bIsDir)
	{
		strcat(szLogFullPath,szLogDir);
		strcat(szLogFullPath,szLogFile);
		::DeleteFileA(szLogFullPath);
	}
	*ppAddr = NULL;
}
