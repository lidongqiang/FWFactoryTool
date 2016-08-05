#ifndef _h_EmShell
#define _h_EmShell

#include "EmBase.h"

namespace em
{

class EmShell  
{
public:
	static int CreateLnk(const char* szSrcFile, const char* szLnkPath, const char* szDesc);

};//class EmShell

}//namespace em;

#endif //#ifndef _EM_SHELL_H
