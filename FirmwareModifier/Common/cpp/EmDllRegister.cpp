#include "stdafx.h"
#include "../inc/EmDllRegister.h"
#include "../inc/EmTstr.h"

#include <afx.h>
/*
return values:
0:register successfully
-1:could not load dll
-2:register failed
-3:no entry point
*/
using namespace em;

int EmDllRegister::Register(const char* szPath)
{
	int nResult = 0;
	HINSTANCE hLib = ::LoadLibrary(EmTstr::FromUtf8(szPath).Sz());
    if (hLib == NULL)
    {
		nResult = -1;
		return nResult;
    }
	
    FARPROC lpDllEntryPoint;
    lpDllEntryPoint = ::GetProcAddress(hLib,"DllRegisterServer");
	
    if(lpDllEntryPoint != NULL)
    {
        if(FAILED((*lpDllEntryPoint)()))
        {
            nResult = -2;
        }
		else
		{
			nResult = 0;
		}
    }
    else
	{
        nResult = -3;
	}
	::FreeLibrary(hLib);
	return nResult;
}

/*
return values:
0:unregister ok
-1:could not find path
-2:could not unregister
-3:could not find entry point
*/
int EmDllRegister::Unregister(const char* szPath)
{
	int nResult = 0;
	HINSTANCE hLib = ::LoadLibrary(EmTstr::FromUtf8(szPath).Sz());
    if (hLib==NULL)
    {
        nResult = -1;
    }
	
    FARPROC lpDllEntryPoint;
    lpDllEntryPoint = ::GetProcAddress(hLib,"DllUnregisterServer");
	
    if(lpDllEntryPoint!=NULL)
    {
        if(FAILED((*lpDllEntryPoint)()))
        {
            nResult = -2;
        }
		else
		{
			nResult = 0;
		}
    }
    else
	{
        return -3;
	}
	::FreeLibrary(hLib);
	return nResult;
}

bool EmDllRegister::HasRegister(const char* szName)
{
	bool bResult = false;
    HKEY hKey;        
	if(::RegOpenKeyEx(HKEY_CLASSES_ROOT,EmTstr::FromUtf8(szName).Sz(),0,KEY_READ,&hKey) != ERROR_SUCCESS)
	{
        bResult = false;
	}
    else
	{
        bResult = true;
	}
	return bResult;	
}