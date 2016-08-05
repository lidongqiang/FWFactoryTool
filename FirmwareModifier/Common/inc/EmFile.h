#ifndef _h_EmFile
#define _h_EmFile

#include "EmBase.h"

#include <vector>
#include <string>



namespace em
{

/*******

 EmFile is a class for file operations    
 only static methods
********/
class EmFile
{
	public:
	/*
	@bufWrite:buffer to write,
	@nBufSize:bytes to write; if -1, bufWrite must be zero-terminated,but be careful to unicode and utf8
	@return:buffer size written|error code
	*/
	static int WriteBuffer(const char* szFileName, const char * bufWrite, int nBufSize);

	/*
	@bufRead:buffer to place content
	@nReadLimit:max read limit
	@return:buffer size read|error code
	*/
	static int ReadBuffer(const char* szFileName, char * bufRead, int nReadLimit, bool bRemoveBom);

	/*
	read buffer by position
	@bufRead:buffer to place content
	@nReadLimit:max read limit
	@return:buffer size read|error code
	*/
	static int ReadBufferByPos(const char* szFileName, char * bufRead, unsigned int nPos, unsigned int nSize);

	/*
	@bufWrite:buffer to append
	@nBufSize:bytes to append; if -1, bufWrite must be zero-terminated,but be careful to unicode and utf8
	@return:buffer size read|error code
	*/
	static int AppendBuffer(const char* szFileName, const char * bufWrite, int nBufSize);

		/*
	@enc: encoding for file on the disk
	@return:bytes appended | error code
	*/
	static int AppendUtf8(const char* szFileName, const char* strContent, EmEncode eFileEncode);
	static int ReadUtf8(const char* szPath, char *bufUtf8, int nReadLimit, EmEncode eFileEncode);
	static int WriteUtf8(const char* szPath, const char *bufUtf8, EmEncode eFileEncode);
	static std::string ReadUtf8Str(const char* szPath, EmEncode enEncode);


	static int WriteBomUtf8(const char* szFileName);
	static int WriteBomUnicode(const char* szFileName);



	static bool IsExisted(const char* szPath);
	static int Size(const char* szPath);

	/*
	create a directory,not recursively
	@return: +:ok; 0:no action; -:error code
	*/
	static int MakeSingleDir(const char* szDirName);

	/*
	it will create hierarchy directory recursively
	@return: +:ok; 0:no action; -:error code
	*/
	static int MakeDir(const char* szDirName);

	static bool IsDir(const char* szPath);
	static bool IsFile(const char* szPath);

	/*
	@return: +:ok; 0:no action; -:error code
	*/
	static int DelFile(const char* szFileName);

	/*
	it will delete hierarchy directory recursively
	@return: +:ok; 0:no action; -:error code
	*/
	static int DelDir(const char* szDirName);

	/*
	delete all files and directories in the dir,
	if the directory not existed,make it
	@return: +:ok; 0:no action; -:error code
	*/
	static int EmptyDir(const char* szDirName);

	/*
	to make sure the directory existed, if not ,make it
	@return: +:ok; 0:no action; -:error code
	*/
	static int EnsureDir(const char* szPath);

	/*
	delete file or directory,no matter the szPath is a file or not
	@return: +:ok; 0:no action; -:error code
	*/
	static int DelPath(const char* szPath);



	/*
	create a single file,not recursively
	@return: +:ok; 0:no action; -:error code
	*/
	static int MakeSingleFile(const char* szPath);

	/*
	create a single file,it will create directories recursively
	@return: +:ok; 0:no action; -:error code
	*/
	static int MakeFile(const char* szPath, bool bOverwrite);

	/*
	move file savely, it will check source and dest dir
	@return: +:ok; 0:no action; -:error code
	*/
	static int MoveFileMy(const char* szSrcPath,const char* szDestPath,bool bOverwrite);

	/*
	copy file savely, it will check source and dest dir
	@return: +:ok; 0:no action; -:error code
	*/
	static int CopyFileMy(const char* szSrcPath,const char* szDestPath,bool bOverwrite);

	/*
	move dir savely, it will check source and dest dir
	@return: +:ok; 0:no action; -:error code
	*/
	static int MoveDir(const char* szSrcPath,const char* szDestPath,bool bOverwrite);

	/*
	copy dir savely, it will check source and dest dir
	@return: +:ok; 0:no action; -:error code
	*/
	static int CopyDir(const char* szSrcPath,const char* szDestPath,bool bOverwrite);

	/*
	rename file or directory
	can move file
	can not move dir
	@return: +:ok; 0:no action; -:error code
	*/
	static int Rename(const char* szSrcPath,const char* szDestPath, bool bOverrite);

	/*
	replace "\" with "/",and check whether it is end with "/" 
	*/
	
	/*
	@bSelfDelete: whether to delete the directory itself
	*/
	static void RecursiveDelete(const char* szPath, bool bSelfDelete);

	static void RecursiveMove(const char* szSrcPath,const char* szDestPath);

	static void RecursiveCopy(const char* szSrcPath,const char* szDestPath);

	/* 
	get file head bom for utf8
	*/
	static void GetUtf8Bom(char *bufBom);

	/*
	get file head bom for unicode 
	*/
	static void GetUnicodeBom(char *bufBom);


	/*
	list sub directories and files in the dir, NOT recursively
	@pVec:a vector to contain result
	@return: +:ok; 0:no action; -:error code
	*/
	static int ListAll(const char* szPath, std::vector<std::string> *pVec);

	/*
	list sub files in the dir, NOT recursively
	@pVec:a vector to contain result
	@return: +:ok; 0:no action; -:error code
	*/
	static int ListFile(const char* szPath, std::vector<std::string> *pVec);

	/*
	list sub directories in the dir, NOT recursively
	@pVec:a vector to contain result
	@return: +:ok; 0:no action; -:error code
	*/
	static int ListDir(const char* szPath, std::vector<std::string> *pVec);


	/*
	list sub files in the dir, recursively
	@pVec:a vector to contain result
	@return: +:ok; 0:no action; -:error code
	*/
	static int ListFileRecur(const char* szPath, std::vector<std::string> *pVec);


	

};//class EmFile

}//namespace em

#endif//#ifndef _EM_FILE_H
