#ifndef _h_EmDllRegister
#define _h_EmDllRegister

#include "EmBase.h"

namespace em
{

class EmDllRegister  
{
public:
		/*
		return values:
		0:register successfully
		-1:could not load dll
		-2:register failed
		-3:no entry point
		*/
		static int Register(const char* szPath);
		
		/*
		return values:
		0:unregister ok
		-1:could not find path
		-2:could not unregister
		-3:could not find entry point
		*/
		static int Unregister(const char* szPath);
		
		static bool HasRegister(const char* szName);

};//class EmDllRegister 

}//namespace em;

#endif // #ifndef _EM_DLL_REGISTER_H