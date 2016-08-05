#include "stdafx.h"
#include "../inc/EmOsVersion.h"
#include <afx.h>


std::string em::EmOsVersion::GetVersionText()
{
	std::string strOsVersionText;
	OSVERSIONINFO xOsVersion;
	memset(&xOsVersion,0,sizeof(OSVERSIONINFO));
	xOsVersion.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
	::GetVersionEx(&xOsVersion);
	
	DWORD dwMajor = xOsVersion.dwMajorVersion;
	DWORD dwMinor = xOsVersion.dwMinorVersion;
	
	if(dwMajor >= 6 && dwMinor > 1)
	{
		strOsVersionText = "win7+";
	}
	else if(dwMajor == 6 && dwMinor == 1)
	{
		strOsVersionText = "win7";
	}
	else if(dwMajor == 6 && dwMinor == 0)
	{
		strOsVersionText = "vista";
	}
	else if(dwMajor == 5 && dwMinor == 2)
	{
		strOsVersionText = "w2k3";
	}
	else if(dwMajor == 5 && dwMinor == 1)
	{
		strOsVersionText = "wxp";
	}
	else if(dwMajor == 5 && dwMinor == 0)
	{
		strOsVersionText = "w2k";
	}
	else if(dwMajor == 4 && dwMinor == 0)
	{
		strOsVersionText = "wnt4";
	}
	else if(dwMajor == 3 && dwMinor == 0)
	{
		strOsVersionText = "wnt35";
	}
	//DOS based list
	else if(dwMajor == 4 && dwMinor == 90)
	{
		strOsVersionText = "wme";
	}
	else if(dwMajor == 4 && dwMinor == 10)
	{
		strOsVersionText = "w98";
	}
	else if(dwMajor == 4 && dwMinor == 0)
	{
		strOsVersionText = "w95";
	}

	return strOsVersionText;
}
