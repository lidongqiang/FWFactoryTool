#ifndef _h_EmCharSet
#define _h_EmCharSet


#include "EmBase.h"

#include <tchar.h>
#include <wchar.h>




namespace em
{

	class EmCharSet
	{
	private:

		static char Tc2ChW(TCHAR chTcs);
		static char Tc2ChA(TCHAR chTcs);

		static TCHAR Ch2TcW(char chMbs);
		static TCHAR Ch2TcA(char chMbs);
		
		static int Tcs2MbsW(char* szMbs, const TCHAR*  szTcs);
		static int Tcs2MbsA(char* szMbs, const TCHAR*  szTcs);
		
		static int Mbs2TcsW(TCHAR* szTcs, const char*  szMbs);
		static int Mbs2TcsA(TCHAR* szTcs, const char*  szMbs);
		
		static int Tcs2WcsW(wchar_t* szWcs, const TCHAR*  szTcs);
		static int Tcs2WcsA(wchar_t* szWcs, const TCHAR*  szTcs);
		
		static int Wcs2TcsW(TCHAR*  szTcs, const wchar_t* szWcs);
		static int Wcs2TcsA(TCHAR*  szTcs, const wchar_t* szWcs);
		
	public:
		static char Tc2Ch(TCHAR chTcs);
		static TCHAR Ch2Tc(char chMbs);
		static int Tcs2Mbs(char* szMbs, const TCHAR*  szTcs);
		static int Mbs2Tcs(TCHAR*  szTcs, const char*  szMbs);
		static int Tcs2Wcs(wchar_t* szWcs, const TCHAR*  szTcs);
		static int Wcs2Tcs(TCHAR*  szTcs, const wchar_t* wcs);
		static int Mbs2Wcs(wchar_t* szWcs, const char* szMbs);
		static int Wcs2Mbs(char* mbs, const wchar_t* wcs);

		static int UnicodeToUtf8(char* szUtf8, const wchar_t* szUnicode);
		static int UnicodeToUtf8Size(const wchar_t* szUnicode);
		static int Utf8ToUnicode(wchar_t* szUnicode, const char* szUtf8);
		static int Utf8ToUnicodeSize(const char* szUtf8);
		static int TcsToUtf8(char* szUtf8, const TCHAR* szTcs);
		static int Utf8ToTcs(TCHAR* szTcs, const char* szUtf8);
		static int AnsiToUtf8(char *szUtf8, const char *szAnsi);
		static int Utf8ToAnsi(char *szAnsi, const char *szUtf8);
		
		static int TcsToOther( char *szOther, const TCHAR* szTcs, EmEncode eEncoding);
		static int OtherToTcs( TCHAR *szTcs, const char *szOther, EmEncode eEncoding);

		static void CheckSetLocale();
	private:
		static bool s_bSetLocal;


	};//EmCharSet

}//namespace em


#endif//#ifndef _EM_CHAR_SET_H