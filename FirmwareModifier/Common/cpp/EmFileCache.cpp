#include "stdafx.h"
#include "../inc/EmFileCache.h"

#include "../inc/EmFile.h"



using namespace em;
using namespace std;

EmFileCache* EmFileCache::s_pInstance = NULL;

EmFileCacheEntry* em::EmFileCache::GetFile( const char* szFileName )
{
	EmFileCacheEntry* pCacheEntry = NULL;
	
	if(!ExistedFileInCache(szFileName))
	{
		PackFileToCache(szFileName);
	}
	pCacheEntry = LoadFileFromCache(szFileName);

	return pCacheEntry;
}

EmFileCacheEntry* em::EmFileCache::LoadFileFromCache( const char* szFileName )
{
	std::map<std::string,em::EmFileCacheEntry*>::iterator itItem;
	std::string strKey = MakeFileKey(szFileName);
	itItem = m_pCacheTable->find(strKey);
	if(itItem == m_pCacheTable->end()){
		return NULL;
	}
	return itItem->second;
}

int em::EmFileCache::PackFileToCache( const char* szFileName )
{
	if(!EmFile::IsFile(szFileName))
	{
		return EM_ERR_FILE_LOST;
	}

	int iFileSize = EmFile::Size(szFileName);
	char* pFileNew =  (char*)malloc(iFileSize + 1);
	memset(pFileNew+iFileSize,0,1);
	EmFile::ReadBuffer(szFileName,pFileNew,iFileSize,false);
	EmFileCacheEntry *pCacheEntry = new EmFileCacheEntry();
	strcpy(pCacheEntry->szFileName,szFileName);
	pCacheEntry->iSize = iFileSize;
	pCacheEntry->bufContent = pFileNew;

	std::string strKey = MakeFileKey(szFileName);
	(*m_pCacheTable)[strKey] = pCacheEntry;
	
	return 0;
}

bool em::EmFileCache::ExistedFileInCache( const char* szFileName )
{
	if(m_pCacheTable->find(szFileName) != m_pCacheTable->end()){
		return true;
	}
	return false;
}

std::string em::EmFileCache::MakeFileKey( const char* szFileName )
{
	std::string strResult;
	strResult += szFileName;
	return strResult;
}

void em::EmFileCache::RemoveFileInCache( const char* szFileName )
{
	std::string strKey = MakeFileKey(szFileName);
	EmFileCacheEntry *pEntry = (*m_pCacheTable)[strKey.c_str()];
	if(pEntry == NULL)
	{
		return;
	}
	EM_FREE_NULL( pEntry->bufContent);
	EM_DELETE_NULL( pEntry);
	m_pCacheTable->erase(strKey.c_str());
	return ;
}

EmFileCache* em::EmFileCache::GetInstance()
{
	if(s_pInstance == NULL)
	{
		s_pInstance = new EmFileCache();
	}

	return s_pInstance;
}

EmFileCacheEntry* em::EmFileCache::SGetFile( const char* szFileName )
{
	EmFileCache *pInstance = GetInstance();
	return pInstance->GetFile(szFileName);
}

void em::EmFileCache::Clear()
{
	std::map<std::string,em::EmFileCacheEntry*>::iterator iterMap;
	while(true)
	{
		iterMap = m_pCacheTable->begin();
		if(iterMap == m_pCacheTable->end())
		{
			break;
		}
		RemoveFileInCache(iterMap->second->szFileName);
	}
}

em::EmFileCache::EmFileCache()
{
	m_pCacheTable = new std::map<std::string,em::EmFileCacheEntry*>();
}

em::EmFileCache::~EmFileCache()
{
	delete m_pCacheTable;
	m_pCacheTable = NULL;
}
