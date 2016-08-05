#include "stdafx.h"
#include "../inc/EmCharSet.h"

#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <string.h>

using namespace em;
bool em::EmCharSet::s_bSetLocal = false;

char em::EmCharSet::Tc2ChW(TCHAR chTcs)
{
	return (char)chTcs;
}

TCHAR em::EmCharSet::Ch2TcW(char chMbs)
{
	return (wchar_t)chMbs;
}

char em::EmCharSet::Tc2ChA(TCHAR chTcs)
{
	return (char)chTcs;
}

TCHAR em::EmCharSet::Ch2TcA(char chMbs)
{
	return (char)chMbs;
}

int em::EmCharSet::Tcs2MbsW(char*  szMbs, const TCHAR*  szTcs)
{
	CheckSetLocale();
	
	if(szMbs != NULL){
		szMbs[0] = 0;
	}

	if(szMbs == NULL || szTcs == NULL){
		return 0;
	}

	int iSrcLen = wcslen((wchar_t*)szTcs);
	if(iSrcLen == 0){
		return 0;
	}

	int iResult = wcstombs(szMbs,(wchar_t *)szTcs,iSrcLen * 2 + 2);
	return iResult;
}

int em::EmCharSet::Tcs2MbsA(char*  szMbs, const TCHAR*  szTcs)
{
	if(szMbs != NULL){
		szMbs[0] = 0;
	}
	
	if(szMbs == NULL || szTcs == NULL){
		return 0;
	}
	
	int iSrcLen = strlen((const char*)szTcs);
	if(iSrcLen == 0){
		return 0;
	}

	strcpy(szMbs,(const char *)szTcs);
	return iSrcLen;
}

int em::EmCharSet::Mbs2TcsW(TCHAR*  szTcs, const char*  szMbs)
{
	CheckSetLocale();

	if(szTcs != NULL){
		szTcs[0] = 0;
	}

	if(szTcs == NULL || szMbs == NULL){
		return 0;
	}

	int iSrcLen = strlen(szMbs);
	if(iSrcLen == 0){
		return 0;
	}

	int iResult = mbstowcs((wchar_t *)szTcs,szMbs,iSrcLen * 2 + 2);
	return iResult;
}

int em::EmCharSet::Mbs2TcsA(TCHAR*  szTcs, const char*  szMbs)
{
	if(szTcs != NULL){
		szTcs[0] = 0;
	}
	
	if(szTcs == NULL || szMbs == NULL){
		return 0;
	}
	
	int iSrcLen = strlen((const char*)szMbs);
	if(iSrcLen == 0){
		return 0;
	}
	strcpy((char *)szTcs,szMbs);
	return iSrcLen;
}

int em::EmCharSet::UnicodeToUtf8(char* szUtf8, const wchar_t* szUnicode)
{
	int iLen = 0;
	wchar_t chWcsTemp;
	int i = 0;
	
	if(szUtf8 != NULL){
		szUtf8[0] = 0;
	}

	if(szUtf8 == NULL || szUnicode == NULL)
	{
		return 0;
	}

	int iUnicodeSize = wcslen(szUnicode);
	if(iUnicodeSize == 0){
		return 0;
	}

	for(i = 0; i < iUnicodeSize; i++)
	{
		chWcsTemp = szUnicode[i];
		if(chWcsTemp == 0)
		{
			break;
		}
		else if(chWcsTemp < 0x0000007F)
		{
			szUtf8[iLen++] = (char)chWcsTemp;
		}
		else if(chWcsTemp < 0x0000007FF)
		{
			szUtf8[iLen++] = 0xC0 | (chWcsTemp >> 6);
			szUtf8[iLen++] = 0x80 | (chWcsTemp & 0x3F);
		}
		else if(chWcsTemp < 0x00000FFFF)
		{
			szUtf8[iLen++] = 0xE0 | (chWcsTemp >> 12);
			szUtf8[iLen++] = 0x80 | ((chWcsTemp >> 6) & 0x3F);
			szUtf8[iLen++] = 0x80 | (chWcsTemp & 0x3F);
		}
		else if(chWcsTemp < 0x0001FFFFF)
		{
			szUtf8[iLen++] = 0xF0 | (((int)chWcsTemp) >> 18);
			szUtf8[iLen++] = 0x80 | ((chWcsTemp >> 12) & 0x3F);
			szUtf8[iLen++] = 0x80 | ((chWcsTemp >> 6) & 0x3F);
			szUtf8[iLen++] = 0x80 | (chWcsTemp & 0x3F);
		}
		else if(chWcsTemp < 0x03FFFFFFF)
		{
			szUtf8[iLen++] = 0xF8 | (((int)chWcsTemp) >> 24);
			szUtf8[iLen++] = 0x80 | ((((int)chWcsTemp) >> 18) & 0x3F);
			szUtf8[iLen++] = 0x80 | ((chWcsTemp >> 12) & 0x3F);
			szUtf8[iLen++] = 0x80 | ((chWcsTemp >> 6) & 0x3F);
			szUtf8[iLen++] = 0x80 | (chWcsTemp & 0x3F);
		}
		else if(chWcsTemp < 0x7FFFFFFFF)
		{
			szUtf8[iLen++] = 0xFC | (((int)chWcsTemp) >> 30);
			szUtf8[iLen++] = 0x80 | ((((int)chWcsTemp) >> 24) & 0x3F);
			szUtf8[iLen++] = 0x80 | ((((int)chWcsTemp) >> 18) & 0x3F);
			szUtf8[iLen++] = 0x80 | ((chWcsTemp >> 12) & 0x3F);
			szUtf8[iLen++] = 0x80 | ((chWcsTemp >> 6) & 0x3F);
			szUtf8[iLen++] = 0x80 | (chWcsTemp & 0x3F);
		}
	}

	((unsigned char*)szUtf8)[iLen] = 0;

	return iLen;
}

int em::EmCharSet::Utf8ToUnicode(wchar_t* szUnicode, const char* szUtf8)
{
	int iLen = 0;
	int iLoop = 0;
	unsigned char chTemp;
	wchar_t chWcsTemp;
	int iSizeTemp = 0;

	if(szUnicode != NULL){
		szUnicode[0] = 0;
	}

	if(szUnicode == NULL || szUnicode == NULL){
		return 0;
	}
	
	int iUtf8Size = strlen(szUtf8);
	if(iUtf8Size == 0){
		return 0;
	}

	unsigned int iMax = 1024*1024*1024;
	unsigned int iCount = 0;

	while(1)
	{
		if(iLoop > iUtf8Size)
		{
			break;
		}
		if(iCount++ > iMax){
			break;
		}
		chWcsTemp = 0;
		chTemp = szUtf8[iLoop];

		if(chTemp == 0)
		{
			break;
		}

		if((chTemp & 0x80) == 0)
		{
			iSizeTemp = 1;
			chWcsTemp = chTemp;
		}
		else if((chTemp & 0xE0) == 0xC0)
		{
			iSizeTemp = 2;
		
			chWcsTemp = (szUtf8[iLoop] & 0x3F);
			chWcsTemp = chWcsTemp << 6;
	
			chWcsTemp = chWcsTemp | (szUtf8[iLoop+1] & 0x3F);

		}
		else if((chTemp & 0xF0) == 0xE0)
		{
			iSizeTemp = 3;

			chWcsTemp = (szUtf8[iLoop] & 0x0F);
			chWcsTemp = chWcsTemp << 6;
	
			chWcsTemp = chWcsTemp | (szUtf8[iLoop+1] & 0x3F);
			chWcsTemp = chWcsTemp << 6;
			
			chWcsTemp = chWcsTemp | (szUtf8[iLoop+2] & 0x3F);

		}
		else if((chTemp & 0xF8) == 0xF0)
		{
			iSizeTemp = 4;

			chWcsTemp = (szUtf8[iLoop] & 0x0F);
			chWcsTemp = chWcsTemp << 6;
	
			chWcsTemp = chWcsTemp | (szUtf8[iLoop+1] & 0x3F);
			chWcsTemp = chWcsTemp << 6;

			chWcsTemp = chWcsTemp | (szUtf8[iLoop+2] & 0x3F);
			chWcsTemp = chWcsTemp << 6;
			
			chWcsTemp = chWcsTemp | (szUtf8[iLoop+3] & 0x3F);
		}
		else if((chTemp & 0xFC) == 0xF8)
		{
			iSizeTemp = 5;

			chWcsTemp = (szUtf8[iLoop] & 0x07);
			chWcsTemp = chWcsTemp << 6;
	
			chWcsTemp = chWcsTemp | (szUtf8[iLoop+1] & 0x3F);
			chWcsTemp = chWcsTemp << 6;

			chWcsTemp = chWcsTemp | (szUtf8[iLoop+2] & 0x3F);
			chWcsTemp = chWcsTemp << 6;

			chWcsTemp = chWcsTemp | (szUtf8[iLoop+3] & 0x3F);
			chWcsTemp = chWcsTemp << 6;
			
			chWcsTemp = chWcsTemp | (szUtf8[iLoop+4] & 0x3F);
		}
		else if((chTemp & 0xFE) == 0xFC)
		{
			iSizeTemp = 6;

			chWcsTemp = (szUtf8[iLoop] & 0x03);
			chWcsTemp = chWcsTemp << 6;
	
			chWcsTemp = chWcsTemp | (szUtf8[iLoop+1] & 0x3F);
			chWcsTemp = chWcsTemp << 6;

			chWcsTemp = chWcsTemp | (szUtf8[iLoop+2] & 0x3F);
			chWcsTemp = chWcsTemp << 6;

			chWcsTemp = chWcsTemp | (szUtf8[iLoop+3] & 0x3F);
			chWcsTemp = chWcsTemp << 6;
			
			chWcsTemp = chWcsTemp | (szUtf8[iLoop+4] & 0x3F);
			chWcsTemp = chWcsTemp << 6;

			chWcsTemp = chWcsTemp | (szUtf8[iLoop+5] & 0x3F);
		}
		iLoop += iSizeTemp;
		szUnicode[iLen++] = chWcsTemp;
	}
	szUnicode[iLen] = 0;
	return iLen;
}

int em::EmCharSet::Tcs2WcsW( wchar_t* szWcs, const TCHAR*  szTcs)
{
	if(szWcs != NULL){
		szWcs[0] = 0;
	}

	if(szWcs == NULL || szTcs == NULL){
		return 0;
	}

	int iSrcLen = wcslen((const wchar_t*)szTcs);
	if(iSrcLen == 0){
		return 0;
	}

	wcscpy(szWcs,(wchar_t*)szTcs);
	return iSrcLen;
}

int em::EmCharSet::Tcs2WcsA( wchar_t* szWcs, const TCHAR*  szTcs)
{
	CheckSetLocale();

	if(szWcs != NULL){
		szWcs[0] = 0;
	}
	
	if(szWcs == NULL || szTcs == NULL){
		return 0;
	}
	
	int iSrcLen = strlen((const char*)szTcs);
	if(iSrcLen == 0){
		return 0;
	}

	szWcs[0] = 0;
	int iResult = mbstowcs(szWcs,(const char*)szTcs,iSrcLen * 2 + 2);
	return iResult;
}

int em::EmCharSet::Wcs2TcsW( TCHAR*  szTcs, const wchar_t* szWcs)
{
	if(szTcs != NULL){
		szTcs[0] = 0;
	}
	
	if(szTcs == NULL || szWcs == NULL){
		return 0;
	}
	
	int iSrcLen = wcslen(szWcs);
	if(iSrcLen == 0){
		return 0;
	}

	wcscpy((wchar_t*)szTcs,szWcs);
	return iSrcLen;
}

int em::EmCharSet::Wcs2TcsA( TCHAR* szTcs, const wchar_t* szWcs)
{
	CheckSetLocale();

	if(szTcs != NULL){
		szTcs[0] = 0;
	}
	
	if(szTcs == NULL || szWcs == NULL){
		return 0;
	}
	
	int iSrcLen = wcslen(szWcs);
	if(iSrcLen == 0){
		return 0;
	}

	int iResult = wcstombs((char*)szTcs,szWcs,iSrcLen * 2 + 2);
	return iResult;
}

int em::EmCharSet::TcsToUtf8( char* szUtf8, const TCHAR* szTcs )
{
	int iResult = 0;
	int iCountOfChar = 0;
	wchar_t *szWcs = NULL;

	if(szUtf8 != NULL){
		szUtf8[0] = 0;
	}

	if(szUtf8 == NULL || szTcs == NULL)
	{
		return 0;
	}

	iCountOfChar = _tcslen(szTcs);
	if(iCountOfChar == 0)
	{
		return 0;
	}

	if(sizeof(TCHAR) == sizeof(wchar_t)){
		iResult = UnicodeToUtf8(szUtf8,(wchar_t*)szTcs);
	}else{
		szWcs = (wchar_t*)malloc(iCountOfChar * 3 + 2);
		szWcs[0] = 0;
		iResult = Tcs2Wcs(szWcs,szTcs);
		if(iResult > 0){
			iResult = UnicodeToUtf8(szUtf8,(wchar_t*)szTcs);
		}
		EM_FREE_NULL(szWcs);
	}

	return iResult;
}

int em::EmCharSet::Utf8ToTcs( TCHAR* szTcs, const char* szUtf8 )
{
	int iResult = 0;
	int iCountOfChar = 0;
	wchar_t *szWcs = NULL;

	if(szTcs != NULL){
		szTcs[0] = 0;
	}
	
	if(szUtf8 == NULL || szTcs == NULL)
	{
		return 0;
	}
	
	iCountOfChar = strlen(szUtf8);
	if(iCountOfChar == 0)
	{
		return 0;
	}
	
	if(sizeof(wchar_t) == sizeof(TCHAR)){
		iResult = Utf8ToUnicode((wchar_t*)szTcs,szUtf8);
	}else{
		szWcs = (wchar_t*)malloc(iCountOfChar * 3 + 2);
		szWcs[0] = 0;
		iResult = Utf8ToUnicode(szWcs,szUtf8);
		if(iResult <= 0){
			return 0;
		}
		iResult = Wcs2Tcs(szTcs,szWcs);
	}

	return iResult;
}

int em::EmCharSet::TcsToOther( char *szOther, const TCHAR* szTcs, EmEncode eEncoding)
{
	int iDestLength = 0;
	wchar_t* szWcs = NULL;

	if(szOther != NULL){
		szOther[0] = 0;
	}

	if(szOther == NULL || szTcs == NULL){
		return 0;
	}

	if(_tcslen(szTcs) == 0){
		return 0;
	}
	
	if(eEncoding == EmEncodeAnsi)
	{
		iDestLength = Tcs2Mbs(szOther,szTcs);
	}
	else if(eEncoding == EmEncodeUnicode)
	{
		iDestLength = Tcs2Wcs((wchar_t*)szOther,szTcs);
	}
	else if(eEncoding == EmEncodeUtf8)
	{
		iDestLength = TcsToUtf8(szOther,szTcs);
	}
	return iDestLength;
}

int em::EmCharSet::OtherToTcs( TCHAR *szTcs, const char *szOther, EmEncode eEncoding)
{
	if(szTcs != NULL){
		szTcs[0] = 0;
	}
	
	if(szOther == NULL || szTcs == NULL){
		return 0;
	}
	
	if(strlen(szOther) == 0){
		return 0;
	}

	int iDestLength = 0;
	if(eEncoding == EmEncodeAnsi)
	{
		iDestLength = Mbs2Tcs(szTcs,szOther);
	}else if(eEncoding == EmEncodeUnicode)
	{
		iDestLength = Wcs2TcsW(szTcs,(wchar_t*)szOther);
	}else if(eEncoding == EmEncodeUtf8)
	{
		iDestLength = Utf8ToTcs(szTcs,szOther);
	}
	return iDestLength;
}

char em::EmCharSet::Tc2Ch( TCHAR tc )
{
	if(sizeof(TCHAR) == 1)
	{
		return Tc2ChA(tc);
	}
	else
	{
		return Tc2ChW(tc);
	}
}

TCHAR em::EmCharSet::Ch2Tc( char ch )
{
	if(sizeof(TCHAR) == 1)
	{
		return Ch2TcA(ch);
	}
	else
	{
		return Ch2TcW(ch);
	}
}

int em::EmCharSet::Tcs2Mbs( char*  szMbs, const TCHAR*  szTcs)
{
	if(sizeof(TCHAR) == 1)
	{
		return Tcs2MbsA(szMbs, szTcs);
	}
	else
	{
		return Tcs2MbsW(szMbs, szTcs);
	}
}

int em::EmCharSet::Mbs2Tcs( TCHAR*  szTcs, const char*  szMbs)
{
	if(sizeof(TCHAR) == 1)
	{
		return Mbs2TcsA(szTcs, szMbs);
	}
	else
	{
		return Mbs2TcsW(szTcs, szMbs);
	}
}

int em::EmCharSet::Tcs2Wcs( wchar_t* szWcs, const TCHAR*  szTcs )
{
	if(sizeof(TCHAR) == 1)
	{
		return Tcs2WcsA(szWcs, szTcs);
	}
	else
	{
		return Tcs2WcsW(szWcs, szTcs);
	}
}

int em::EmCharSet::Wcs2Tcs( TCHAR*  szTcs, const wchar_t* szWcs )
{
	if(sizeof(TCHAR) == 1)
	{
		return Wcs2TcsA(szTcs, szWcs);
	}
	else
	{
		return Wcs2TcsW(szTcs, szWcs);
	}
}

int em::EmCharSet::Mbs2Wcs( wchar_t* szWcs, const char* szMbs )
{
	CheckSetLocale();

	if(szWcs != NULL)
	{
		szWcs[0] = 0;
	}

	if(szWcs == NULL || szMbs == NULL){
		return 0;
	}

	int iSrcLen = strlen(szMbs);
	if(iSrcLen == 0){
		return 0;
	}

	return	mbstowcs(szWcs,szMbs,strlen(szMbs) * 2 + 2);
}

int em::EmCharSet::Wcs2Mbs( char* szMbs, const wchar_t* szWcs )
{
	CheckSetLocale();

	if(szMbs != NULL)
	{
		szMbs[0] = 0;
	}
	
	if(szWcs == NULL || szMbs == NULL){
		return 0;
	}
	
	int iSrcLen = wcslen(szWcs);
	if(iSrcLen == 0){
		return 0;
	}

	int iResult = wcstombs(szMbs,szWcs,wcslen(szWcs) * 2 + 2);

	return iResult;
}

int em::EmCharSet::UnicodeToUtf8Size( const wchar_t* szUnicode )
{
	int iLen = 0;
	wchar_t chWcsTemp;
	int iUnicodeSize = wcslen(szUnicode);
	int i = 0;
	
	if(szUnicode == NULL)
	{
		return 0;
	}

	if(iUnicodeSize == 0){
		return 0;
	}

	for(i = 0; i < iUnicodeSize; i++)
	{
		chWcsTemp = szUnicode[i];
		if(chWcsTemp == 0)
		{
			break;
		}
		else if(chWcsTemp < 0x0000007F)
		{
			iLen += 1;
		}
		else if(chWcsTemp < 0x0000007FF)
		{
			iLen += 2;
		}
		else if(chWcsTemp < 0x00000FFFF)
		{
			iLen += 3;
		}
		else if(chWcsTemp < 0x0001FFFFF)
		{
			iLen += 4;
		}
		else if(chWcsTemp < 0x03FFFFFFF)
		{
			iLen += 5;
		}
		else if(chWcsTemp < 0x7FFFFFFFF)
		{
			iLen += 6;
		}
	}
	
	return iLen;
}

int em::EmCharSet::Utf8ToUnicodeSize( const char* szUtf8 )
{
	int iLen = 0;
	int iLoop = 0;
	int iUtf8Size = strlen(szUtf8);
	unsigned char chTemp;
	wchar_t chWcsTemp;
	int iSizeTemp = 0;

	if(szUtf8 == NULL)
	{
		return 0;
	}

	if(iUtf8Size == 0){
		return 0;
	}

	while(1)
	{
		if(iLoop >= iUtf8Size)
		{
			break;
		}
		chWcsTemp = 0;
		chTemp = szUtf8[iLoop];

		if(chTemp == 0)
		{
			break;
		}

		if((chTemp & 0x80) == 0)
		{
			iSizeTemp = 1;
		}
		else if((chTemp & 0xE0) == 0xC0)
		{
			iSizeTemp = 2;
		}
		else if((chTemp & 0xF0) == 0xE0)
		{
			iSizeTemp = 3;
		}
		else if((chTemp & 0xF8) == 0xF0)
		{
			iSizeTemp = 4;
		}
		else if((chTemp & 0xFC) == 0xF8)
		{
			iSizeTemp = 5;
		}
		else if((chTemp & 0xFE) == 0xFC)
		{
			iSizeTemp = 6;
		}
		iLoop += iSizeTemp;
		iLen++;
	}
	return iLen;
}

int em::EmCharSet::AnsiToUtf8( char *szUtf8, const char *szAnsi )
{
	if(szUtf8 != NULL){
		szUtf8[0] = 0;
	}

	if(szUtf8 == NULL || szAnsi == NULL){
		return 0;
	}
	int iSrcLen = strlen(szAnsi);
	if(iSrcLen == 0){
		return 0;
	}

	int iResult = 0;
	
	char *szWcs = (char*)malloc(iSrcLen * 3 + 2);
	szUtf8[0] = 0;
	iResult = Mbs2Wcs((wchar_t*)szWcs,szAnsi);
	if(iResult > 0){
		iResult = UnicodeToUtf8(szUtf8,(wchar_t*)szWcs);
	}
	EM_FREE_NULL(szWcs);
	return iResult;
}

int em::EmCharSet::Utf8ToAnsi( char *szAnsi, const char *szUtf8 )
{
	if(szAnsi != NULL){
		szAnsi[0] = 0;
	}
	
	if(szUtf8 == NULL || szAnsi == NULL){
		return 0;
	}
	
	int iSrcLen = strlen(szUtf8);
	if(iSrcLen == 0){
		return 0;
	}

	int iResult = 0;
	char *szWcs = (char*)malloc((iSrcLen + 1) * 6);
	szAnsi[0] = 0;
	iResult = Utf8ToUnicode((wchar_t*)szWcs,szUtf8);
	if(iResult > 0){
		iResult = Wcs2Mbs(szAnsi,(wchar_t*)szWcs);
	}
	
	EM_FREE_NULL(szWcs);
	return iResult;
}

void em::EmCharSet::CheckSetLocale()
{
	if(!s_bSetLocal)
	{
		setlocale(LC_ALL,"chs");
	}
}
