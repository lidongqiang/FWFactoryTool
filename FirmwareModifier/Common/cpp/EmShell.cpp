#include "stdafx.h"
#include "../inc/EmShell.h"
#include "../inc/EmTstr.h"
#include <SHLOBJ.H>

int em::EmShell::CreateLnk( const char* szSrcFile, const char* szLnkPath, const char* szDesc )
{
	std::string strFileName = szSrcFile;
	std::string   strShortCutLocation = szLnkPath;
	IShellLink*   pLink;
	IPersistFile*   pPersistFile;
	if(SUCCEEDED(CoInitialize(NULL)))
	{
		if(!SUCCEEDED(CoCreateInstance(CLSID_ShellLink,NULL,CLSCTX_INPROC_SERVER,IID_IShellLink,(void**)&pLink)))
		{
			pLink-> SetPath( EmTstr::FromUtf8( strFileName.c_str() ).Sz());
			pLink-> SetDescription(EmTstr::FromUtf8( szDesc ).Sz());
			pLink-> SetShowCmd(SW_SHOW);
			if(SUCCEEDED(pLink-> QueryInterface(IID_IPersistFile,
				(void   **)&pPersistFile)))
			{
				
				pPersistFile->Save(EmTstr::FromUtf8(strShortCutLocation.c_str()).Sz(),   true);
				pPersistFile->Release();
			}
			pLink-> Release();
		}else
		{
			CoUninitialize();
			return EM_ERR_COM_CREATE;
		}
		
		CoUninitialize();
	 }
	else
	{
		return EM_ERR_COM_INIT;
	}
	return 0;
}
