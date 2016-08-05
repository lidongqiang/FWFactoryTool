#include "stdafx.h"
#include "../inc/EmFile.h"
#include "../inc/EmCharSet.h"
#include "../inc/EmUtil.h"
#include "../inc/EmTstr.h"
#include "../inc/EmStlStr.h"
#include <afx.h>

#ifndef INVALID_FILE_ATTRIBUTES
#define INVALID_FILE_ATTRIBUTES ((DWORD)-1)
#endif

using namespace em;

int EmFile::WriteBuffer(const char* szFileName, const char * bufWrite, int iBufSrcSize)
{
	EmFile::EnsureDir(szFileName);
	int iWrite = 0;
	if(iBufSrcSize == -1)
	{
		iBufSrcSize = strlen(bufWrite);
	}
	
	HANDLE hFile = ::CreateFile(EmTstr::FromUtf8(szFileName).Sz()
		, GENERIC_READ | GENERIC_WRITE
		, FILE_SHARE_READ | FILE_SHARE_WRITE
		,NULL
		,CREATE_ALWAYS
		,FILE_ATTRIBUTE_NORMAL
		,NULL);
	if(EmUtil::IsInvalidHandle(hFile))
	{
		return EM_ERR_FILE_OPEN_FILE;
	}
	OVERLAPPED overLap;
	memset(&overLap,0,sizeof(overLap));
	bool bLockOk = ::LockFileEx(hFile,LOCKFILE_EXCLUSIVE_LOCK,0,1,0,&overLap);
	if(!bLockOk)
	{
		return EM_ERR_FILE_LOCK;
	}
	DWORD dwWrite = 0;
	::WriteFile(hFile,bufWrite,iBufSrcSize,&dwWrite,NULL);
	iWrite = (int)dwWrite;
	::FlushFileBuffers(hFile);
	::UnlockFileEx(hFile,0,1,0,&overLap);
	::CloseHandle(hFile);
	return iWrite;
}

int EmFile::ReadBuffer(const char* szFileName, char * bufRead, int nReadLimit, bool bRemoveBom)
{
	int nRead = 0;
	if(!EmFile::IsFile(szFileName))
	{
		return EM_ERR_FILE_LOST_FILE;
	}
	
	HANDLE hFile = ::CreateFile(EmTstr::FromUtf8(szFileName).Sz()
		, GENERIC_READ
		, FILE_SHARE_READ | FILE_SHARE_WRITE
		,NULL
		,OPEN_EXISTING
		,0
		,NULL);
	if(EmUtil::IsInvalidHandle(hFile))
	{
		return EM_ERR_FILE_OPEN_FILE;
	}

	OVERLAPPED overLap;
	memset(&overLap,0,sizeof(overLap));
	bool bLockOk = ::LockFileEx(hFile,LOCKFILE_EXCLUSIVE_LOCK,0,1,0,&overLap);
	DWORD dwSize = ::GetFileSize(hFile,NULL);
	DWORD dwRead = 0;
	if(dwSize > 0)
	{
		::ReadFile(hFile,bufRead,dwSize,&dwRead,NULL);
		::FlushFileBuffers(hFile);
		nRead = dwRead;
	}
	::UnlockFileEx(hFile,0,1,0,&overLap);
	::CloseHandle(hFile);

	if(bRemoveBom)
	{
		char bufU8Bom[3] = {0};
		GetUtf8Bom(bufU8Bom);
		char bufU16Bom[2] = {0};
		GetUnicodeBom(bufU16Bom);
		char *bufNewPos = NULL;
		if(nRead >= 3 && (strncmp(bufU8Bom,bufRead,3) == 0))
		{
			bufNewPos = bufRead + 3;
			memmove(bufRead,bufNewPos,nRead - 3);
			memset(bufRead + nRead - 3,0,3);
			nRead -= 3;
		}
		else if(nRead >= 2 && (strncmp(bufU16Bom,bufRead,2) == 0))
		{
			bufNewPos = bufRead + 2;
			memmove(bufRead,bufNewPos,nRead - 2);
			memset(bufRead + nRead - 2,0,2);
			nRead -= 2;
		}

	}

	return nRead;
}

int EmFile::ReadBufferByPos( const char* szFileName, char * bufRead, unsigned int nPos, unsigned int nSize )
{
	int nRead = 0;
	if(!EmFile::IsFile(szFileName))
	{
		return EM_ERR_FILE_LOST_FILE;
	}
	

	HANDLE hFile = ::CreateFile(EmTstr::FromUtf8(szFileName).Sz()
		, GENERIC_READ
		, FILE_SHARE_READ | FILE_SHARE_WRITE
		,NULL
		,OPEN_EXISTING
		,0
		,NULL);
	if(EmUtil::IsInvalidHandle(hFile))
	{
		return EM_ERR_FILE_OPEN_FILE;
	}
	
	long iMoved = 0;

	OVERLAPPED overLap;
	memset(&overLap,0,sizeof(overLap));
	bool bLockOk = ::LockFileEx(hFile,LOCKFILE_EXCLUSIVE_LOCK,0,1,0,&overLap);
	DWORD dwSize = ::GetFileSize(hFile,NULL);
	if(dwSize <= nPos)
	{
		return EM_ERR_FILE_POS;
	}
	DWORD dwRead = 0;
	if(dwSize > 0)
	{
		::SetFilePointer(hFile,nPos,&iMoved,FILE_BEGIN);
		::ReadFile(hFile,bufRead,nSize,&dwRead,NULL);
		::FlushFileBuffers(hFile);
		nRead = dwRead;
	}
	::UnlockFileEx(hFile,0,1,0,&overLap);
	::CloseHandle(hFile);

	return nRead;
}


int EmFile::AppendBuffer(const char* szFileName, const char * bufWrite, int iBufSrcSize)
{
	EmFile::EnsureDir(szFileName);
	int iWrite = 0;
	if(iBufSrcSize == -1)
	{
		iBufSrcSize = strlen(bufWrite);
	}
	
	HANDLE hFile = ::CreateFile(EmTstr::FromUtf8(szFileName).Sz()
		, GENERIC_READ | GENERIC_WRITE | GENERIC_EXECUTE 
		, FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE
		,NULL
		,OPEN_ALWAYS
		,FILE_ATTRIBUTE_NORMAL
		,NULL);
	if(EmUtil::IsInvalidHandle(hFile))
	{
		return EM_ERR_FILE_OPEN_FILE;
	}
	OVERLAPPED overLap;
	memset(&overLap,0,sizeof(overLap));
	bool bLockOk = ::LockFileEx(hFile,LOCKFILE_EXCLUSIVE_LOCK,0,1,0,&overLap);
	DWORD dwWrite = 0;
	::SetFilePointer(hFile,0,NULL,FILE_END);
	::WriteFile(hFile,bufWrite,iBufSrcSize,&dwWrite,NULL);
	iWrite = (int)dwWrite;
	::FlushFileBuffers(hFile);
	::UnlockFileEx(hFile,0,1,0,&overLap);
	::CloseHandle(hFile);

	return iWrite;
}

int EmFile::AppendUtf8( const char* szFileName, const char* szContent, EmEncode eFileEncode )
{
	int iCountOfChar = strlen(szContent);
	int iAllocSize = 0;
	int iWriteBytes = 0;

	if(!EmFile::IsExisted(szFileName))
	{
		return WriteUtf8(szFileName,szContent,eFileEncode);
	}
	
	if(eFileEncode == EmEncodeAnsi)
	{
		iAllocSize = iCountOfChar * 3 + 2;
		char *bufAnsi = (char*)malloc(iAllocSize);
		int iConvertSize = EmCharSet::Utf8ToAnsi(bufAnsi,szContent);
		iWriteBytes = EmFile::AppendBuffer(szFileName,bufAnsi,iConvertSize);
		EM_FREE_NULL( bufAnsi);
	}
	else if(eFileEncode == EmEncodeUnicode)
	{
		iAllocSize = iCountOfChar * 2 + 2;
		char *bufUnicode = (char*)malloc(iAllocSize);
		int iConvertSize = EmCharSet::Utf8ToUnicode((wchar_t*)bufUnicode,szContent);
		iWriteBytes = EmFile::AppendBuffer(szFileName,bufUnicode,iConvertSize * 2);
		EM_FREE_NULL( bufUnicode);
	}
	else if(eFileEncode == EmEncodeUtf8)
	{
		iWriteBytes = EmFile::AppendBuffer(szFileName,szContent,iCountOfChar);
	}
	return iWriteBytes;
}


bool EmFile::IsExisted(const char* szPath)
{
	return (::GetFileAttributes(EmTstr::FromUtf8(szPath).Sz()) != INVALID_FILE_ATTRIBUTES);
}


int EmFile::MakeDir(const char* szDirName)
{
	int errCode = 0;

	if(EmFile::IsDir(szDirName))
	{
		return 0;
	}

	if(IsFile(szDirName))
	{
		errCode = DelFile(szDirName);
		if(errCode < 0)
		{
			return errCode;
		}
	}


	std::string strPath = EmStlStr::FileFixDir(szDirName);

	std::vector<std::string> xPathList;
	EmStlStr::Split(xPathList,strPath.c_str(),"/");
	int len = xPathList.size();
	std::string strCumulativePath;
	std::string strTempPath;
	
	for(int i = 0; i < len ; i++)
	{
		strTempPath = xPathList[i];
		if(strTempPath.length() == 0)
		{
			continue;
		}
	
		if(i != 0)
		{
			strCumulativePath.append("/");
		}

		strCumulativePath.append(strTempPath);

		if(!EmFile::IsDir(strCumulativePath.c_str()))
		{
			if(EmFile::IsFile(strCumulativePath.c_str()))
			{
				errCode = EmFile::DelFile(strCumulativePath.c_str());
				if(errCode < 0)
				{
					return errCode;
				}
			}
			bool bCreated = ::CreateDirectory(EmTstr::FromUtf8(strCumulativePath.c_str()).Sz(),NULL);
			if(!bCreated)
			{
				return EM_ERR_FILE_CREATE_DIR;
			}
		}
	}


	return 1;
}

bool EmFile::IsDir(const char* szPath)
{
	bool bIsDir = false;
	DWORD dwAttrib = ::GetFileAttributes(EmTstr::FromUtf8(szPath).Sz());

	bIsDir =(dwAttrib != INVALID_FILE_ATTRIBUTES 
		&& (dwAttrib & FILE_ATTRIBUTE_DIRECTORY));
	return bIsDir;
}

bool EmFile::IsFile(const char* szPath)
{
	bool bIsFile = false;
	DWORD dwAttrib = ::GetFileAttributes(EmTstr::FromUtf8(szPath).Sz());

	return (dwAttrib != INVALID_FILE_ATTRIBUTES
	         && !(dwAttrib & FILE_ATTRIBUTE_DIRECTORY));
	return bIsFile;
}

int EmFile::DelFile(const char* szFileName)
{
	bool bDeleted = ::DeleteFile(EmTstr::FromUtf8(szFileName).Sz());

	if(!bDeleted)
	{
		return EM_ERR_FILE_DELETE_FILE;
	}

	return 1;
}

int EmFile::DelDir(const char* szDirName)
{
	if(!IsDir(szDirName))
	{
		return 0;
	}
	EmFile::RecursiveDelete(szDirName,true);
	if(IsDir(szDirName))
	{
		return EM_ERR_FILE_DELETE_DIR;
	}
	return 1;
}

int EmFile::EmptyDir(const char* szDirName)
{
	int iErr = 0;
	if(!IsDir(szDirName))
	{
		iErr = EmFile::MakeDir(szDirName);
		if(iErr < 0)
		{
			return iErr;
		}
		else
		{
			return 1;
		}
	}

	EmFile::RecursiveDelete(szDirName,false);

	return 1;
}

int EmFile::DelPath(const char* szPath)
{
	if(EmFile::IsFile(szPath))
	{
		return EmFile::DelFile(szPath);
	}
	else if(EmFile::IsDir(szPath))
	{
		return EmFile::DelDir(szPath);
	}
	else
	{
		return 0;
	}
}

int EmFile::MakeFile(const char* szPath, bool bOverwrite)
{
	int iErr = 0;

	if(!bOverwrite && IsFile(szPath))
	{
		return 0;
	}

	std::string strDir = EmStlStr::FileDirFromPath(szPath);
	if(EmStlStr::Find(strDir.c_str(),"/") != -1)
	{
		iErr = MakeDir(strDir.c_str());
		if(iErr < 0)
		{
			return iErr;
		}
	}

	if(IsFile(szPath))
	{
		iErr = DelFile(szPath);
		if(iErr < 0)
		{
			return iErr;
		}
	}
	else if(IsDir(szPath))
	{
		iErr = DelDir(szPath);
		if(iErr < 0)
		{
			return iErr;
		}
	}

	MakeSingleFile(szPath);


	return 1;
}

int EmFile::EnsureDir(const char* szPath)
{
	int iErr = 0;
	std::string strDir = EmStlStr::FileDirFromPath(szPath);
	if(strDir.length() == 0)
	{
		return 0;
	}

	if(IsDir(strDir.c_str()))
	{
		return 0;
	}

	iErr = MakeDir(strDir.c_str());
	if(iErr < 0)
	{
		return iErr;
	}

	return 1;
}

int EmFile::MoveFileMy(const char* szSrcPath,const char* szDestPath,bool bOverwrite)
{
	int iErr = 0;
	if(!EmFile::IsFile(szSrcPath))
	{
		return EM_ERR_FILE_LOST_FILE;
	}

	if(EmFile::IsExisted(szDestPath))
	{
		if(bOverwrite)
		{
			iErr = EmFile::DelPath(szDestPath);
			if(iErr < 0)
			{
				return iErr;
			}
		}
		else
		{
			return 0;
		}
	}

	iErr = EnsureDir(szDestPath);
	if(iErr < 0)
	{
		return iErr;
	}

	bool bMoved = ::MoveFile(EmTstr::FromUtf8(szSrcPath).Sz(),EmTstr::FromUtf8(szDestPath).Sz());
	if(!bMoved)
	{
		return EM_ERR_FILE_MOVE;
	}

	return 1;
}

int EmFile::CopyFileMy(const char* szSrcPath,const char* szDestPath,bool bOverwrite)
{
	int iErr = 0;
	if(!EmFile::IsFile(szSrcPath))
	{
		return EM_ERR_FILE_LOST_FILE;
	}

	if(EmFile::IsExisted(szDestPath))
	{
		if(bOverwrite)
		{
			iErr = EmFile::DelPath(szDestPath);
			if(iErr < 0)
			{
				return iErr;
			}
		}
		else
		{
			return 0;
		}
	}

	iErr = EnsureDir(szDestPath);
	if(iErr < 0)
	{
		return iErr;
	}

	bool bCopy = ::CopyFile(EmTstr::FromUtf8(szSrcPath).Sz(),EmTstr::FromUtf8(szDestPath).Sz(),true);
	if(!bCopy)
	{
		return EM_ERR_FILE_COPY;
	}
	
	return 1;
}

int EmFile::MoveDir(const char* szSrcPath,const char* szDestPath,bool bOverwrite)
{
	int iErr = 0;
	if(!EmFile::IsDir(szSrcPath))
	{
		return EM_ERR_FILE_LOST_DIR;
	}

	if(EmFile::IsExisted(szDestPath))
	{
		if(!bOverwrite)
		{
			return 0;
		}
		else
		{
			iErr = EmFile::DelPath(szDestPath);
			if(iErr < 0)
			{
				return iErr;
			}
		}
	}

	iErr = EmFile::MakeDir(szDestPath);
	if(iErr < 0)
	{
		return iErr;
	}

	EmFile::RecursiveMove(szSrcPath,szDestPath);

	iErr = EmFile::DelDir(szSrcPath);
	if(iErr < 0)
	{
		return iErr;
	}

	return 1;
}

int EmFile::CopyDir(const char* szSrcPath,const char* szDestPath,bool bOverwrite)
{
	int iErr = 0;
	if(!EmFile::IsDir(szSrcPath))
	{
		return EM_ERR_FILE_LOST_DIR;
	}

	if(EmFile::IsExisted(szDestPath))
	{
		if(!bOverwrite)
		{
			return 0;
		}
		else
		{
			iErr = EmFile::DelPath(szDestPath);
			if(iErr < 0)
			{
				return iErr;
			}
		}
	}
	
	iErr = EmFile::MakeDir(szDestPath);
	if(iErr < 0)
	{
		return iErr;
	}
	
	EmFile::RecursiveCopy(szSrcPath,szDestPath);
	
	return 1;
}



void EmFile::RecursiveDelete(const char* szPath, bool bSelfDelete)
{
	std::string strCurrPath = EmStlStr::FileFixDir(szPath);
	
	if(EmFile::IsFile(szPath))
	{
		::DeleteFile(EmTstr::FromUtf8(szPath).Sz());
	}
	else if(EmFile::IsDir(szPath))
	{	
		std::string strSubPath = strCurrPath.append(".");
		
		WIN32_FIND_DATA findData;
		memset(&findData,0,sizeof(findData));
		HANDLE hFind = ::FindFirstFile(EmTstr::FromUtf8(strSubPath.c_str()).Sz(),&findData);
		std::string strTempShort;
		std::string strTempFull;
		
		while(true)
		{
			if(EmUtil::IsInvalidHandle(hFind))
			{
				break;
			}
			strTempShort = EmTstr::ToUtf8(findData.cFileName);
			strTempFull = strCurrPath + strTempShort;
			
			if(strTempShort.compare(".") == 0
				|| strTempShort.compare("..") == 0)
			{
				//no action
			}
			else
			{
				EmFile::RecursiveDelete(strTempFull.c_str(),true);
			}
			bool bFound = ::FindNextFile(hFind,&findData);
			if(!bFound)
			{
				break;
			}
		}
		
		::FindClose(hFind);
		
		if(bSelfDelete)
		{
			::RemoveDirectory(EmTstr::FromUtf8(strCurrPath.c_str()).Sz());
		}
	}
}

void EmFile::RecursiveMove(const char* szSrcPath,const char* szDestPath)
{

	std::string strSrcPath = EmStlStr::FileFixDir(szSrcPath);
	std::string strDestPath = EmStlStr::FileFixDir(szDestPath);
	std::string strNewSrcPath = strSrcPath;
	std::string strNewDestPath = strDestPath;
	
	if(!EmFile::IsDir(strDestPath.c_str()))
	{
		::CreateDirectory(EmTstr::FromUtf8(strDestPath.c_str()).Sz(),NULL);
	}

	std::string strFindPath = strSrcPath + "*.*";
	
	WIN32_FIND_DATA findData;
	memset(&findData,0,sizeof(findData));
	HANDLE hFind = ::FindFirstFile(EmTstr::FromUtf8(strFindPath.c_str()).Sz(),&findData);
	std::string strTempShort;
	std::string strTempFull;

	while(true)
	{
		if(EmUtil::IsInvalidHandle(hFind))
		{
			break;
		}
		strTempShort = EmTstr::ToUtf8(findData.cFileName);
		strTempFull = strSrcPath + strTempShort;

		if(strTempShort.compare(".") == 0
			|| strTempShort.compare("..") == 0)
		{
			//no action
		}
		else if(IsDir(strTempFull.c_str()))
		{
			strNewSrcPath = strSrcPath;
			strNewSrcPath.append(strTempShort);
			strNewSrcPath.append("/");
			strNewDestPath = strDestPath;
			strNewDestPath.append(strTempShort);
			strNewDestPath.append("/");
			RecursiveMove(strNewSrcPath.c_str(),strNewDestPath.c_str());
		}
		else
		{
			strNewSrcPath = strSrcPath;
			strNewSrcPath.append(strTempShort);
			strNewDestPath = strDestPath;
			strNewDestPath.append(strTempShort);
			if(IsFile(strNewDestPath.c_str()))
			{
				::DeleteFile(EmTstr::FromUtf8(strNewDestPath.c_str()).Sz());
			}
			::MoveFile(EmTstr::FromUtf8(strNewSrcPath.c_str()).Sz(),EmTstr::FromUtf8(strNewDestPath.c_str()).Sz());
		}

		bool bFound = ::FindNextFile(hFind,&findData);
		if(!bFound)
		{
			break;
		}
	}
	::FindClose(hFind);
	::RemoveDirectory(EmTstr::FromUtf8(strSrcPath.c_str()).Sz());

}

void EmFile::RecursiveCopy(const char* szSrcPath,const char* szDestPath)
{

	std::string strSrcPath = EmStlStr::FileFixDir(szSrcPath);
	std::string strDestPath = EmStlStr::FileFixDir(szDestPath);
	std::string strNewSrcPath = strSrcPath;
	std::string strNewDestPath = strDestPath;
	
	if(!EmFile::IsDir(strDestPath.c_str()))
	{
		::CreateDirectory(EmTstr::FromUtf8(strDestPath.c_str()).Sz(),NULL);
	}

	std::string strFindPath = strSrcPath + "*.*";
	
	WIN32_FIND_DATA findData;
	memset(&findData,0,sizeof(findData));
	HANDLE hFind = ::FindFirstFile(EmTstr::FromUtf8(strFindPath.c_str()).Sz(),&findData);
	std::string strTempShort;
	std::string strTempFull;

	while(true)
	{
		if(EmUtil::IsInvalidHandle(hFind))
		{
			break;
		}
		strTempShort = EmTstr::ToUtf8(findData.cFileName);
		strTempFull = strSrcPath + strTempShort;

		if(strTempShort.compare(".") == 0
			|| strTempShort.compare("..") == 0)
		{
			//no action
		}
		else if(IsDir(strTempFull.c_str()))
		{
			strNewSrcPath = strSrcPath;
			strNewSrcPath.append(strTempShort);
			strNewSrcPath.append("/");
			strNewDestPath = strDestPath;
			strNewDestPath.append(strTempShort);
			strNewDestPath.append("/");
			RecursiveCopy(strNewSrcPath.c_str(),strNewDestPath.c_str());
		}
		else
		{
			strNewSrcPath = strSrcPath;
			strNewSrcPath.append(strTempShort);
			strNewDestPath = strDestPath;
			strNewDestPath.append(strTempShort);
			::CopyFile(EmTstr::FromUtf8(strNewSrcPath.c_str()).Sz(), EmTstr::FromUtf8(strNewDestPath.c_str()).Sz(),false);
		}

		bool bFound = ::FindNextFile(hFind,&findData);
		if(!bFound)
		{
			break;
		}
	}
	::FindClose(hFind);

}

void EmFile::GetUtf8Bom(char *bufBom)
{
	if(bufBom == NULL)
	{
		return;
	}
	bufBom[0] = (char)0xEF;
	bufBom[1] = (char)0xBB;
	bufBom[2] = (char)0xBF;
}

void EmFile::GetUnicodeBom(char *bufBom)
{
	if(bufBom == NULL)
	{
		return;
	}
	bufBom[0] = (char)0xFF;
	bufBom[1] = (char)0xFE;
}

int EmFile::Size( const char* szPath )
{
	int iFileSize = 0;
	WIN32_FIND_DATA xFindData;
	HANDLE hFind = NULL;
	DWORD fileSize = 0;
	hFind = FindFirstFile(EmTstr::FromUtf8(szPath).Sz() ,&xFindData);
	if(hFind != INVALID_HANDLE_VALUE)
		iFileSize = xFindData.nFileSizeLow;
	::FindClose(hFind);

	return iFileSize;
// 	if(!EmFile::IsFile(szPath))
// 	{
// 		return 0;
// 	}
// 	HANDLE hFile = ::CreateFile(EmTstr::FromUtf8(szPath).Sz()
// 		, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
// 	if(hFile == INVALID_HANDLE_VALUE){
// 		//::MessageBoxA(NULL,EmStlStr::Int2Str(::GetLastError()).c_str(),NULL,0);
// 		return 0;
// 	}
// 	iFileSize = ::GetFileSize(hFile,NULL);
// 	::CloseHandle(hFile);
// 	if(iFileSize < 0){
// 		iFileSize = 0;
// 	}
//	return iFileSize;
}

int EmFile::MakeSingleFile( const char* szPath )
{
	return WriteBuffer(szPath,"",0);
	//HANDLE hFile = ::CreateFile(EmTstr::FromUtf8(szPath).Sz()
	//	, GENERIC_READ | GENERIC_WRITE | GENERIC_EXECUTE 
	//	, FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE
	//	, NULL, CREATE_ALWAYS
	//	, FILE_ATTRIBUTE_NORMAL
	//	, NULL);

	//if(!EmUtil::IsInvalidHandle(hFile))
	//{
	//	return EM_ERR_FILE_CREATE_FILE;
	//}
	//OVERLAPPED overLap;
	//memset(&overLap,0,sizeof(overLap));
	//bool bLockOk = ::LockFileEx(hFile,LOCKFILE_EXCLUSIVE_LOCK,0,1,0,&overLap);
	//::UnlockFileEx(hFile,0,1,0,&overLap);
	//::CloseHandle(hFile);
	//return 1;
}

int EmFile::MakeSingleDir( const char* szDirName )
{
	if(IsDir(szDirName))
	{
		return 0;
	}

	if(IsFile(szDirName))
	{
		return EM_ERR_FILE_EXISTED_FILE;
	}

	bool bCreated = ::CreateDirectory(EmTstr::FromUtf8(szDirName).Sz(),NULL);
	if(!bCreated)
	{
		return EM_ERR_FILE_CREATE_DIR;
	}

	return 1;
}

int EmFile::Rename( const char* szSrcPath, const char* szDestPath, bool bOverrite)
{
	int iErr = 0;
	if(!bOverrite && IsExisted(szDestPath))
	{
		return 0;
	}

	if(bOverrite && IsExisted(szDestPath))
	{
		iErr = DelPath(szDestPath);
		if(iErr < 0)
		{
			return iErr;
		}
	}

	int iResult = _trename(EmTstr::FromUtf8(szSrcPath).Sz(), EmTstr::FromUtf8(szDestPath).Sz());
	if(iResult != 0)
	{
		return EM_ERR_FILE_RENAME;
	}

	return 1;
}

int EmFile::ListAll( const char* szPath, std::vector<std::string> *pVec )
{
	if(pVec == NULL)
	{
		return EM_ERR_MEM_NULL;
	}

	if(!IsDir(szPath))
	{
		return EM_ERR_FILE_LOST_DIR;
	}

	std::string strSrcPath = EmStlStr::FileFixDir(szPath);
	std::string strFindPath = strSrcPath + "*.*";
	
	WIN32_FIND_DATA findData;
	memset(&findData,0,sizeof(findData));
	HANDLE hFind = ::FindFirstFile(EmTstr::FromUtf8(strFindPath.c_str()).Sz(),&findData);
	std::string strTempShort;
	std::string strTempFull;
	while(true)
	{
		if(EmUtil::IsInvalidHandle(hFind))
		{
			break;
		}
		strTempShort = EmTstr::ToUtf8(findData.cFileName);
		strTempFull = strSrcPath + strTempShort;
		
		if(strTempShort.compare(".") == 0
			|| strTempShort.compare("..") == 0)
		{
			//no action
		}
		else if(IsDir(strTempFull.c_str()))
		{
			pVec->push_back(strTempFull);
		}
		else if(IsFile(strTempFull.c_str()))
		{
			pVec->push_back(strTempFull);
		}
		
		bool bFound = ::FindNextFile(hFind,&findData);
		if(!bFound)
		{
			break;
		}
	}
	::FindClose(hFind);

	return 1;
}

int EmFile::ListFile( const char* szPath, std::vector<std::string> *pVec )
{
	if(pVec == NULL)
	{
		return EM_ERR_MEM_NULL;
	}

	if(!IsDir(szPath))
	{
		return EM_ERR_FILE_LOST_DIR;
	}

	std::string strSrcPath = EmStlStr::FileFixDir(szPath);
	std::string strFindPath = strSrcPath + "*.*";
	
	WIN32_FIND_DATA findData;
	memset(&findData,0,sizeof(findData));
	HANDLE hFind = ::FindFirstFile(EmTstr::FromUtf8(strFindPath.c_str()).Sz(),&findData);
	std::string strTempShort;
	std::string strTempFull;
	while(true)
	{
		if(EmUtil::IsInvalidHandle(hFind))
		{
			break;
		}
		strTempShort = EmTstr::ToUtf8(findData.cFileName);
		strTempFull = strSrcPath + strTempShort;
		
		if(strTempShort.compare(".") == 0
			|| strTempShort.compare("..") == 0)
		{
			//no action
		}
		else if(IsDir(strTempFull.c_str()))
		{
			//no action
		}
		else if(IsFile(strTempFull.c_str()))
		{
			pVec->push_back(strTempFull);
		}
		
		bool bFound = ::FindNextFile(hFind,&findData);
		if(!bFound)
		{
			break;
		}
	}
	::FindClose(hFind);

	return 1;
}

int EmFile::ListDir( const char* szPath, std::vector<std::string> *pVec )
{
	if(pVec == NULL)
	{
		return EM_ERR_MEM_NULL;
	}

	if(!IsDir(szPath))
	{
		return EM_ERR_FILE_LOST_DIR;
	}

	std::string strSrcPath = EmStlStr::FileFixDir(szPath);
	std::string strFindPath = strSrcPath + "*.*";
	
	WIN32_FIND_DATA findData;
	memset(&findData,0,sizeof(findData));
	HANDLE hFind = ::FindFirstFile(EmTstr::FromUtf8(strFindPath.c_str()).Sz(),&findData);
	std::string strTempShort;
	std::string strTempFull;
	while(true)
	{
		if(EmUtil::IsInvalidHandle(hFind))
		{
			break;
		}
		strTempShort = EmTstr::ToUtf8(findData.cFileName);
		strTempFull = strSrcPath + strTempShort;
		
		if(strTempShort.compare(".") == 0
			|| strTempShort.compare("..") == 0)
		{
			//no action
		}
		else if(IsDir(strTempFull.c_str()))
		{
			pVec->push_back(strTempFull);
		}
		else if(IsFile(strTempFull.c_str()))
		{
			//no action
		}
		
		bool bFound = ::FindNextFile(hFind,&findData);
		if(!bFound)
		{
			break;
		}
	}
	::FindClose(hFind);

	return 1;
}

int EmFile::ListFileRecur( const char* szPath, std::vector<std::string> *pVec )
{
	if(pVec == NULL)
	{
		return EM_ERR_MEM_NULL;
	}

	if(!IsDir(szPath))
	{
		return EM_ERR_FILE_LOST_DIR;
	}

	std::string strSrcPath = EmStlStr::FileFixDir(szPath);
	std::string strFindPath = strSrcPath + "*.*";
	
	WIN32_FIND_DATA findData;
	memset(&findData,0,sizeof(findData));
	HANDLE hFind = ::FindFirstFile(EmTstr::FromUtf8(strFindPath.c_str()).Sz(),&findData);
	std::string strTempShort;
	std::string strTempFull;
	while(true)
	{
		if(EmUtil::IsInvalidHandle(hFind))
		{
			break;
		}
		strTempShort = EmTstr::ToUtf8(findData.cFileName);
		strTempFull = strSrcPath + strTempShort;
		
		if(strTempShort.compare(".") == 0
			|| strTempShort.compare("..") == 0)
		{
			//no action
		}
		else if(IsDir(strTempFull.c_str()))
		{
			std::string strNewDir = strSrcPath + strTempShort;
			ListFileRecur(strNewDir.c_str(),pVec);
		}
		else if(IsFile(strTempFull.c_str()))
		{
			pVec->push_back(strTempFull);
		}
		
		bool bFound = ::FindNextFile(hFind,&findData);
		if(!bFound)
		{
			break;
		}
	}
	::FindClose(hFind);

	return 1;
}

int EmFile::WriteBomUtf8( const char* szFileName )
{
	char bom[3] = {0};
	GetUtf8Bom(bom);
	return WriteBuffer(szFileName,bom,3);
}

int EmFile::WriteBomUnicode( const char* szFileName )
{
	char bom[2] = {0};
	GetUnicodeBom(bom);
	return WriteBuffer(szFileName,bom,2);
}

int EmFile::ReadUtf8(const char* szPath, char *bufUtf8, int nReadLimit, EmEncode eFileEncode)
{
	int iResult = 0;
	bufUtf8[0] = 0;

	if(!IsFile(szPath))
	{
		return EM_ERR_FILE_LOST_FILE;
	}
	
	int iFileSize = Size(szPath);
	if(iFileSize <= 0)
	{
		return 0;
	}

	if(eFileEncode == EmEncodeUtf8)
	{
		iResult = ReadBuffer(szPath,bufUtf8,nReadLimit,true);
		if(iResult >= 0){
			if(iResult >= 3){
				char bufBomUtf8[4] = {0};
				GetUtf8Bom(bufBomUtf8);
				if(strncmp(bufBomUtf8,bufUtf8,3) == 0){
					memmove(bufUtf8,bufUtf8 + 3,iResult - 3);
					iResult -= 3;
				}
			}
			bufUtf8[iResult] = 0;
		}
	}
	else
	{
		char *bufSrc = (char*)malloc(iFileSize + 2);
		bufSrc[0] = 0;
		iResult = ReadBuffer(szPath,bufSrc,iFileSize,true);

		if(iResult >= 0 && eFileEncode == EmEncodeUnicode)
		{
			bufSrc[iResult] = 0;
			bufSrc[iResult + 1] = 0;

			if(iResult >= 3){
				char bufBomUnicode[3] = {0};
				GetUnicodeBom(bufBomUnicode);
				if(strncmp(bufBomUnicode,bufSrc,2) == 0){
					memmove(bufSrc,bufSrc + 3,iResult - 2);
					iResult -= 2;
				}
			}
			bufSrc[iResult] = 0;
			bufSrc[iResult + 1] = 0;
			iResult = EmCharSet::UnicodeToUtf8(bufUtf8,(const wchar_t*)bufSrc);
		}
		else if(iResult >= 0 && eFileEncode == EmEncodeAnsi)
		{
			bufSrc[iResult] = 0;
			iResult = EmCharSet::AnsiToUtf8(bufUtf8,(const char*)bufSrc);
		}

		EM_FREE_NULL(bufSrc);
	}

	return iResult;
}

int EmFile::WriteUtf8( const char* szPath, const char *szUtf8, EmEncode eFileEncode )
{
	int iResult = 0;
	int iSrcLen = strlen(szUtf8);
	
	int iDestLen = 0;
	char bufBom[3] = {0};

	if(eFileEncode == EmEncodeAnsi)
	{
		char *bufDest = NULL;
		bufDest = (char*)malloc(iSrcLen * 1 + 1);
		iDestLen = EmCharSet::Utf8ToAnsi(bufDest,szUtf8);
		iResult = WriteBuffer(szPath,bufDest,iDestLen);
		EM_FREE_NULL(bufDest);
	}
	else if(eFileEncode == EmEncodeUnicode)
	{
		char *bufDest = NULL;
		iResult = WriteBomUnicode(szPath);
		if(iResult < 0){
			return iResult;
		}

		bufDest = (char*)malloc((iSrcLen + 2) * 2);
		iDestLen = EmCharSet::Utf8ToUnicode((wchar_t*)(bufDest),szUtf8);
		if(iDestLen > 0){
			iResult = AppendBuffer(szPath, bufDest,iDestLen * 2);
		}
		EM_FREE_NULL(bufDest);
	}
	else if(eFileEncode == EmEncodeUtf8)
	{
		char *bufDest = NULL;
		iResult = WriteBomUtf8(szPath);
		if(iResult < 0){
			return iResult;
		}
		iResult = AppendBuffer(szPath,szUtf8,iSrcLen);
	}
	
	return iResult;
}

std::string EmFile::ReadUtf8Str( const char* szPath, EmEncode eFileEncode )
{
	std::string strResult;
	int iResult = 0;

	if(!IsFile(szPath))
	{
		return "";
	}
	
	int iFileSize = Size(szPath);
	if(iFileSize <= 0)
	{
		return "";
	}

	char* szUtf8 = (char*)malloc(iFileSize * 3 + 6);

	iResult = ReadUtf8(szPath,szUtf8,iFileSize,eFileEncode);
	strResult = szUtf8;

	EM_FREE_NULL(szUtf8);
	return strResult;
}




