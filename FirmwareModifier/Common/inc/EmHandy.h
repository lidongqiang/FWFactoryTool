#ifndef _h_EmHandy
#define _h_EmHandy

#include "EmBase.h"

#include <string>



namespace em
{

/*
some utility functions for some basic math operation  	
*/
class EmHandy
{
public:
	static __int64 GenerateLongKey(); 
	static std::string GenerateFileName(const char* szDir, const char *szExt);
	static void DebugTraceFile(const char *szFile, const char* szMsg);
	static void DebugAlert(const char* szMsg);

};//class EmHandy

}//namespace em

#endif //#ifndef _EM_HANDY_H