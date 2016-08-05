#ifndef _h_EmFileCache
#define _h_EmFileCache

#include "EmBase.h"

#include <map>
#include <string>



namespace em
{

typedef struct tagEmFileCacheEntry
{
	char szFileName[1024];
	char *bufContent;
	int iSize;
}EmFileCacheEntry;

class EmFileCache  
{
	
public:
	EmFileCacheEntry* GetFile(const char* szFileName);
	EmFileCacheEntry* LoadFileFromCache(const char* szFileName);
	int PackFileToCache(const char* szFileName);
	std::string MakeFileKey(const char* szFileName);
	void Clear();
	void RemoveFileInCache(const char* szFileName);
	bool ExistedFileInCache(const char* szFileName);
	static EmFileCache* GetInstance();
	static EmFileCacheEntry* SGetFile(const char* szFileName);
private:
	std::map<std::string, em::EmFileCacheEntry*>* m_pCacheTable;
	EmFileCache();;
	virtual ~EmFileCache();
	EmFileCache(EmFileCache& xCopy){};
	EmFileCache& operator=(EmFileCache& xCopy){};
	static EmFileCache* s_pInstance;
	
	
};//class EmFileCache

}//namespace em

#endif // #ifndef EM_FILE_CACHE_H
