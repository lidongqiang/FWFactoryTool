#include "stdafx.h"
#include "../inc/EmWsz.h"
#include "../inc/EmUtil.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

using namespace em;

wchar_t* em::EmWsz::Int2Str( wchar_t* szDest, int iNum )
{
	if(szDest == NULL){
		return NULL;
	}
	swprintf(szDest,L"%d",iNum);
	return szDest;
}

wchar_t* em::EmWsz::Long2Str( wchar_t* szDest,INT64 iNum )
{
	if(szDest == NULL){
		return NULL;
	}
	_i64tow(iNum,szDest,10);
	return szDest;
}

int em::EmWsz::Str2Int(const wchar_t* szSrc )
{
	if(szSrc == NULL || wcslen(szSrc) == 0){
		return 0;
	}

	return _wtoi(szSrc);
}

INT64 em::EmWsz::Str2Long( const wchar_t* szSrc )
{
	if(szSrc == NULL || wcslen(szSrc) == 0){
		return 0;
	}

	return _wtoi64(szSrc);
}

wchar_t* em::EmWsz::Float2Str( wchar_t *szDest, double fNum )
{
	if(szDest == NULL){
		return NULL;
	}
	swprintf(szDest,L"%f",fNum);
	return szDest;
}

int em::EmWsz::Char2Int( wchar_t chSrc )
{
	if(!IsDigit(chSrc)){
		return 0;
	}
	return (int)chSrc - 0x30;
}

wchar_t* em::EmWsz::Char2Str( wchar_t *szDest, wchar_t chSrc )
{
	if(szDest == NULL){
		return NULL;
	}
	szDest[0] = chSrc;
	szDest[1] = 0;
	return szDest;
}

wchar_t em::EmWsz::Str2Char( const wchar_t* szSrc )
{
	if(szSrc == NULL){
		return 0;
	}
	return szSrc[0];
}

wchar_t* em::EmWsz::Lower( wchar_t* szSrc )
{
	if(szSrc == NULL){
		return NULL;
	}
	wchar_t *pMove = szSrc;
	while(*pMove != 0)
	{
		*pMove = towlower(*pMove);
		pMove++;
	}
	return szSrc;
}

wchar_t* em::EmWsz::Upper( wchar_t* szSrc )
{
	if(szSrc == NULL){
		return NULL;
	}
	wchar_t *pMove = szSrc;
	while(*pMove != 0)
	{
		*pMove = towupper(*pMove);
		pMove++;
	}
	return szSrc;
}

wchar_t* em::EmWsz::Reverse( wchar_t* szSrc )
{
	if(szSrc == NULL){
		return NULL;
	}
	int iSrcLen = wcslen(szSrc);
	int i = 0;
	wchar_t chTemp = 0;
	if(iSrcLen <= 1)
	{
		return szSrc;
	}
	for(i = 0; i < iSrcLen / 2; i++)
	{
		chTemp = szSrc[i];
		szSrc[i] = szSrc[iSrcLen - i - 1];
		szSrc[iSrcLen - i - 1] = chTemp;
	}
	return szSrc;
}

unsigned int em::EmWsz::CountChar( const wchar_t* szSrc, wchar_t chFind )
{
	if(szSrc == NULL){
		return 0;
	}
	int iRetCount = 0;
	while(*szSrc != 0)
	{
		if(*szSrc == chFind)
		{
			iRetCount++;
		}
		szSrc++;
	}
	return iRetCount;
}

unsigned int em::EmWsz::CountStr( const wchar_t* szSrc, const wchar_t* szFind )
{
	if(szSrc == NULL || szFind == NULL){
		return 0;
	}
	unsigned int iRetCount = 0;
	bool bMatch = 0;
	int iFromPos = 0;
	unsigned int iSrcLen = wcslen(szSrc);
	unsigned int iFindLen = wcslen(szFind);
	unsigned int i = 0;
	unsigned int iMatchCharCount = 0;

	for(i = iFromPos; i < iSrcLen; i++)
	{
		if(!bMatch)
		{
			if(szSrc[i] == szFind[0])
			{
				bMatch = true;
				iMatchCharCount = 1;
			}
			else
			{
				continue;
			}
		}
		else
		{
			if(szSrc[i] == szFind[iMatchCharCount])
			{
				iMatchCharCount++;
			}
			else
			{
				bMatch = false;
				iMatchCharCount = 0;
			}
		}

		if(iMatchCharCount == iFindLen)
		{
			iRetCount++;
			iMatchCharCount = 0;
			bMatch = false;
		}
	}
	return iRetCount;
}

int em::EmWsz::FindFrom( const wchar_t *szSrc, const wchar_t *szFind, unsigned int iFromPos)
{
	if(szSrc == NULL || szFind == NULL){
		return -1;
	}
	int bMatch = 0;
	unsigned int iSrcLen = wcslen(szSrc);
	unsigned int iFindLen = wcslen(szFind);
	unsigned int i = 0;
	int iPos = -1;
	int iTempPos = 0;
	unsigned int iMatchCount = 0;
	if(iSrcLen - iFromPos < iFindLen
		|| iFromPos < 0
		|| iFromPos >= iSrcLen
		|| iFromPos < 0)
	{
		return -1;
	}
	for(i = iFromPos; i < iSrcLen; i++)
	{
		if(!bMatch)
		{
			if(szSrc[i] == szFind[0])
			{
				bMatch = 1;
				iMatchCount = 1;
				iTempPos = i;
			}
			else
			{
				continue;
			}
		}
		else
		{
			if(szSrc[i] == szFind[iMatchCount])
			{
				iMatchCount++;
			}
			else
			{
				bMatch = 0;
				iMatchCount = 0;
			}
		}
		if(iMatchCount == iFindLen)
		{
			return iTempPos;
		}
	}
	return -1;
}

wchar_t* em::EmWsz::SubStr( wchar_t *szDest, const wchar_t *szSrc, unsigned int iStart, int iSubLen )
{
	if(szDest != NULL){
		szDest[0] = 0;
	}
	if(szSrc == NULL || szDest == NULL){
		return 0;
	}
	unsigned int iSrcLen = wcslen(szSrc);
	unsigned int i = 0;
	
	if(iSubLen == -1)
	{
		iSubLen = iSrcLen - iStart;
	}
	
	if(iStart + iSubLen > iSrcLen)
	{
		return NULL;
	}
	
	memcpy(szDest, szSrc + iStart, iSubLen * sizeof(wchar_t));
	szDest[iSubLen] = 0;
	return szDest;
}

int em::EmWsz::Find( const wchar_t *szSrc, const wchar_t *szFind )
{
	return FindFrom(szSrc,szFind,0);
}

wchar_t* em::EmWsz::Repeat(wchar_t *szDest, const wchar_t* szSrc, int iCount )
{
	if(szDest != NULL){
		szDest[0] = 0;
	}
	if(szSrc == NULL){
		return szDest;
	}
	size_t i = 0;
	int iLen = wcslen(szSrc);
	int iResultLen = iLen * iCount;
	for(i = 0; i < iCount; i++)
	{
		memcpy(szDest + (i * iLen),szSrc,iLen * sizeof(wchar_t));
	}
	szDest[iResultLen] = 0;
	return szDest;
}

wchar_t* em::EmWsz::Prefix(wchar_t* szSrc, const wchar_t* szPrefix )
{
	if(szSrc == NULL){
		return NULL;
	}
	if(szPrefix == NULL){
		return szSrc;
	}
	int iSrcLen = wcslen(szSrc);
	int iPreLen = wcslen(szPrefix);
	int iResultLen = iSrcLen + iPreLen;
	memmove(szSrc + iPreLen,szSrc,iSrcLen * sizeof(wchar_t));
	memcpy(szSrc,szPrefix,iPreLen * sizeof(wchar_t));
	szSrc[iResultLen] = 0;
	return szSrc;
}

wchar_t* em::EmWsz::PadLeft(wchar_t* szSrc, wchar_t chPad, int iTotalLen )
{
	if(szSrc == NULL){
		return NULL;
	}
	int iSrcLen = wcslen(szSrc);
	int iPadLen = iTotalLen - iSrcLen;
	int i = 0;
	if(iSrcLen >= iTotalLen)
	{
		return szSrc;
	}

	memmove(szSrc + iPadLen,szSrc,iSrcLen * sizeof(wchar_t));

	for(i = 0; i < iPadLen; i++)
	{
		szSrc[i] = chPad;
	}

	return szSrc;
}

wchar_t* em::EmWsz::AddSlashes( wchar_t* szSrc, wchar_t chFind )
{
	if(szSrc == NULL){
		return NULL;
	}
	int iSrcLen = wcslen(szSrc);
	wchar_t *szCopy = (wchar_t*)malloc((iSrcLen * 2 + 1) * sizeof(wchar_t));
	int i = 0;
	int iPos = 0;

	if(szCopy == NULL)
	{
		return NULL;
	}
	
	for(i = 0; i < iSrcLen; i++)
	{
		if(szSrc[i] == chFind
			|| szSrc[i] == L'\\')
		{
			szCopy[iPos++] = L'\\';
			szCopy[iPos++] = szSrc[i];
		}
		else
		{
			szCopy[iPos++] = szSrc[i];
		}
	}
	szCopy[iPos++] = 0;
	wcscpy(szSrc,szCopy);
	EM_FREE_NULL(szCopy);
	return szSrc;
}

wchar_t* em::EmWsz::StripSlashes( wchar_t* szSrc )
{
	if(szSrc == NULL){
		return NULL;
	}
	int iSrcLen = wcslen(szSrc);
	wchar_t *szCopy = (wchar_t*)malloc((iSrcLen + 1) * sizeof(wchar_t));
	int i = 0;
	int iPos = 0;
	
	if(szCopy == NULL)
	{
		return NULL;
	}
	
	bool bAddLast = true;
	for(i = 0; i < iSrcLen - 1; i++)
	{
		if(szSrc[i] == L'\\')
		{
			i++;
			szCopy[iPos++] = szSrc[i];
			if(i == iSrcLen - 1)
			{
				bAddLast = false;
			}
		}
		else
		{
			szCopy[iPos++] = szSrc[i];
		}
	}
	if(bAddLast)
	{
		szCopy[iPos++] = szSrc[iSrcLen - 1];
	}

	szCopy[iPos++] = 0;
	wcscpy(szSrc,szCopy);
	EM_FREE_NULL(szCopy);
	return szSrc;
}

wchar_t* em::EmWsz::Dec2Bin(wchar_t* szSrc )
{
	if(szSrc == NULL){
		return NULL;
	}
	INT64 num = _wtoi64(szSrc);
	wchar_t szTemp[255] = {0};
	INT64 iTempQuotient = num;
	INT64 iTempRemainder = 0;
	int iPos = 0;
	while(iTempQuotient / 2 != 0)
	{
		iTempRemainder = iTempQuotient % 2;
		iTempQuotient = iTempQuotient / 2;
		
		if(iTempRemainder == 0)
		{
			szTemp[iPos++] = L'0';
		}
		else
		{
			szTemp[iPos++] = L'1';
		}
	}
	if(iTempQuotient == 1)
	{
		szTemp[iPos++] = L'1';
	}
	wcscpy(szSrc,Reverse(szTemp));
	return szSrc;
}

wchar_t* em::EmWsz::Bin2Dec( wchar_t* szSrc )
{
	if(szSrc == NULL){
		return NULL;
	}
	int iSrcLen = wcslen(szSrc);
	int i = 0;
	INT64 iSum = 0;
	int iPow = 0;
	if(iSrcLen <= 0)
	{
		return szSrc;
	}
	for(i = iSrcLen - 1; i >= 0; i--)
	{
		if(szSrc[i] != L'0')
		{
			iSum += EmUtil::PowPlus(2,iPow);
		}
		iPow++;
	}
	swprintf(szSrc,L"%d",iSum);
	return szSrc;
}

wchar_t* em::EmWsz::Hex2Dec( wchar_t* szSrc )
{
	if(szSrc == NULL){
		return NULL;
	}
	int iSrcLen = wcslen(szSrc);
	int i = 0;
	INT64 iSum = 0;
	int iPow = 0;
	int iDigit = 0;
	for(i = iSrcLen - 1; i >= 0; i--)
	{
		iDigit = em::EmUtil::HexChar2Int(szSrc[i]);
		if(szSrc[i] != L'0')
		{
			iSum += ( iDigit * EmUtil::PowPlus(16,iPow) );
		}
		iPow++;
	}
	swprintf(szSrc,L"%d",iSum);
	return szSrc;
}

wchar_t* em::EmWsz::Dec2Hex( wchar_t* szSrc )
{
	if(szSrc == NULL){
		return NULL;
	}
	INT64 iDec = _wtoi64(szSrc);
	swprintf(szSrc,L"%x",iDec);
	return szSrc;
}

wchar_t* em::EmWsz::Bin2Hex( wchar_t* szSrc )
{
	if(szSrc == NULL){
		return NULL;
	}
	Bin2Dec(szSrc);
	Dec2Hex(szSrc);
	return szSrc;
}

wchar_t* em::EmWsz::Hex2Bin( wchar_t* szSrc )
{
	if(szSrc == NULL){
		return NULL;
	}
	Hex2Dec(szSrc);
	Dec2Bin(szSrc);
	return szSrc;
}

int em::EmWsz::FindReverse( const wchar_t* szSrc, const wchar_t* szFind )
{
	if(szSrc == NULL || szFind == NULL){
		return 0;
	}
	int iFromPos = 0;
	int bMatch = 0;
	int iSrcLen = wcslen(szSrc);
	int iFindLen = wcslen(szFind);
	int i = 0;
	int iPos = -1;
	int iTempPos = 0;
	int iMatchCount = 0;
	iFromPos = iSrcLen - 1;
	if(iSrcLen < iFindLen)
	{
		return -1;
	}
	for(i = iFromPos; i >= 0; i--)
	{
		if(!bMatch)
		{
			if(szSrc[i] == szFind[iFindLen - 1])
			{
				bMatch = 1;
				iMatchCount = 1;
				iTempPos = i;
			}
			else
			{
				continue;
			}
		}
		else
		{
			if(szSrc[i] == szFind[iFindLen - iMatchCount - 1])
			{
				iMatchCount++;
			}
			else
			{
				bMatch = 0;
				iMatchCount = 0;
			}
		}
		if(iMatchCount == iFindLen)
		{
			return i;
		}
	}
	return -1;
}

wchar_t* em::EmWsz::TrimLeft( wchar_t* szSrc )
{
	if(szSrc == NULL){
		return NULL;
	}
	wchar_t *szBegin = szSrc;
	int iSrcLen = wcslen(szSrc);
	int iBeginSpaces = 0;
	int iDestLen = 0;
	int i = 0;
	
	if(iSrcLen == 0)
	{
		return szSrc;
	}
	
	while(iswspace(*szBegin))
	{
		szBegin++;
		iBeginSpaces++;
	}
	iDestLen = iSrcLen - iBeginSpaces;
	
	for(i = 0; i < iDestLen; i++)
	{
		szSrc[i] = szSrc[i + iBeginSpaces];
	}
	szSrc[iDestLen] = 0;
	
	return szSrc;
}

wchar_t* em::EmWsz::TrimRight( wchar_t* szSrc )
{
	if(szSrc == NULL){
		return NULL;
	}
	int iSrcLen = wcslen(szSrc);
	wchar_t *szEnd = szSrc + iSrcLen - 1;
	
	if(iSrcLen == 0)
	{
		return szSrc;
	}
	
	while(iswspace(*szEnd))
	{
		szEnd--;
	}
	szEnd++;
	*szEnd = 0;
	
	return szSrc;
}

wchar_t* em::EmWsz::Trim( wchar_t* szSrc )
{
	if(szSrc == NULL){
		return NULL;
	}
	TrimLeft(szSrc);
	TrimRight(szSrc);
	return szSrc;
}

wchar_t* em::EmWsz::FormatNum(wchar_t* szSrc, unsigned int nLenAfterPoint )
{
	if(szSrc == NULL){
		return NULL;
	}
	int iSrcLen = wcslen(szSrc);
	wchar_t szDest[255] = {0};
	int i = 0;
	int iDotPos = 0;
	wchar_t szPlusZeros[255] = {0};
	wchar_t szIntPart[255] = {0};
	wchar_t szDecPart[255] = {0};
	size_t iSrcDecLen = 0;
	wchar_t szNewInt[255] = {0};
	int iNewInt = 0;
	wchar_t chNext = L'0';
	int iNextNum = 0;
	wchar_t szNewDec[255] = {0};
	
	if(iSrcLen == 0)
	{
		return szSrc;
	}
	
	iDotPos = Find(szSrc,L".");
	
	if(iDotPos == -1)
	{
		Repeat(szPlusZeros,L"0",nLenAfterPoint);
		wcscpy(szDest,szSrc);
		wcscat(szDest,L".");
		wcscat(szDest,szPlusZeros);
	}
	else
	{
		SubStr(szIntPart,szSrc,0,iDotPos);
		SubStr(szDecPart,szSrc,iDotPos + 1,iSrcLen - 1 - iDotPos);
		iSrcDecLen = wcslen(szDecPart);
		if(iSrcDecLen == nLenAfterPoint)
		{
			wcscpy(szDest,szSrc);
		}
		else if(iSrcDecLen < nLenAfterPoint)
		{
			Repeat(szPlusZeros,L"0",nLenAfterPoint - iSrcDecLen);
			wcscpy(szDest,szSrc);
			wcscat(szDest,szPlusZeros);
		}
		else
		{
			wcscpy(szNewInt,szIntPart);
			wcsncat(szNewInt,szDecPart,nLenAfterPoint);
			iNewInt = Str2Int(szNewInt);
			chNext = szDecPart[nLenAfterPoint];
			iNextNum = Char2Int(chNext);
			
			if(iNextNum >= 5)
			{
				iNewInt++;
			}
			
			Int2Str(szNewInt,iNewInt);
			
			if(nLenAfterPoint == 0)
			{
				wcscpy(szDest,szNewInt);
			}
			else
			{
				if(wcslen(szNewInt) <= nLenAfterPoint)
				{
					Repeat(szPlusZeros,L"0",nLenAfterPoint - wcslen(szNewInt) + 1);
					Prefix(szPlusZeros,szNewInt);
				}
				
				SubStr(szNewDec,szNewInt,wcslen(szNewInt) - nLenAfterPoint,nLenAfterPoint);
				wcsncpy(szDest,szNewInt,wcslen(szNewInt) - nLenAfterPoint);
				wcscat(szDest,L".");
				
				wcscat(szDest,szNewDec);
			}
		}
	}
	
	wcscpy(szSrc,szDest);
	return szSrc;
}

wchar_t* em::EmWsz::Cut(wchar_t* szSrc, unsigned int nStart, unsigned int nLen )
{
	if(szSrc == NULL){
		return NULL;
	}
	size_t iSrcLen = wcslen(szSrc);
	size_t i = 0;
	size_t iDestLen = iSrcLen - nLen;
	
	if(nStart + nLen > iSrcLen)
	{
		return szSrc;
	}
	
	for(i = nStart; i < iDestLen; i++)
	{
		szSrc[i] = szSrc[i + nLen];
	}
	szSrc[iDestLen] = 0;
	return szSrc;
}

wchar_t* em::EmWsz::Insert( wchar_t* szSrc, const wchar_t* szInsert, unsigned int nStart )
{
	if(szSrc == NULL){
		return NULL;
	}
	if(szInsert == NULL){
		szInsert = EM_EMPTY_WSZ;
	}
	size_t iSrcLen = wcslen(szSrc);
	size_t iAppendLen = wcslen(szInsert);
	size_t iDestLen = iSrcLen + iAppendLen;
	size_t iRange = iSrcLen - nStart;
	size_t i = 0;
	if(nStart > iSrcLen)
	{
		return szSrc;
	}
	
	for(i = 1; i <= iRange; i++)
	{
		szSrc[iDestLen - i] = szSrc[iDestLen - i - iAppendLen];
	}
	
	for(i = 0; i < iAppendLen; i++)
	{
		szSrc[nStart + i] = szInsert[i];
	}
	
	szSrc[iDestLen] = 0;
	return szSrc;
}

wchar_t* em::EmWsz::Replace( wchar_t* szSrc, const wchar_t* szFind, const wchar_t* szReplace )
{
	if(szSrc == NULL){
		return NULL;
	}
	if(szFind == NULL){
		return szSrc;
	}
	if(szReplace == NULL){
		szReplace = EM_EMPTY_WSZ;
	}
	int iSrcLen = wcslen(szSrc);
	int iFindLen = wcslen(szFind);
	int iReplaceLen = wcslen(szReplace);
	int iTempLen = 0;
	int iTotalLen = iSrcLen;
	int iFindPos = 0;
	int iDiffLen = iReplaceLen - iFindLen;
	while(1)
	{
		iFindPos = FindFrom(szSrc,szFind,iFindPos);
		if(iFindPos == -1)
		{
			break;
		}
		else
		{
			Splice(szSrc,szReplace,iFindPos,iFindLen);
			iFindPos += iReplaceLen;
			iTotalLen += iDiffLen;
		}
	}
	szSrc[iTotalLen] = 0;
	return szSrc;
}

wchar_t* em::EmWsz::Splice( wchar_t* szSrc, const wchar_t* szInsert, unsigned int nStart, unsigned int nLen )
{
	if(szSrc == NULL){
		return NULL;
	}
	if(szInsert == NULL){
		szInsert = EM_EMPTY_WSZ;
	}
	size_t iSrcLen = wcslen(szSrc);
	if(nStart < 0
		|| nLen < 0
		|| nStart + nLen > iSrcLen)
	{
		return szSrc;
	}
	Cut(szSrc,nStart,nLen);
	Insert(szSrc,szInsert,nStart);
	return szSrc;
}

bool em::EmWsz::IsDigit( wchar_t chSrc )
{
	return iswdigit(chSrc);
}

bool em::EmWsz::IsLetter( wchar_t chSrc )
{
	if((chSrc >= L'a' && chSrc <= L'z' )
		||(chSrc >= L'A' && chSrc <= L'Z' )
	)
	{
		return true;
	}
	return false;
}

bool em::EmWsz::IsInt( const wchar_t* szSrc )
{
	if(szSrc == NULL){
		return false;
	}
	int iSrcLen = wcslen(szSrc);
	int i = 0;
	if(iSrcLen == 0)
	{
		return false;
	}
	
	if(szSrc[0] != L'-' && szSrc[0] != L'+' && !iswdigit(szSrc[0]))
	{
		return false;
	}

	for(i = 1; i < iSrcLen; i++)
	{
		if(!iswdigit(szSrc[i]))
		{
			return false;
		}
	}
	return true;
}

bool em::EmWsz::IsNumber( const wchar_t* szSrc )
{
	if(szSrc == NULL){
		return false;
	}
	unsigned int iSrcLen = wcslen(szSrc);
	unsigned int i = 0;
	wchar_t chTemp = 0;
	int iDotCount = 0;
	if(iSrcLen == 0)
	{
		return false;
	}
	else if(iSrcLen > 1)
	{
		for(i = 1; i < iSrcLen; i++)
		{
			chTemp = *(szSrc + i); 
			if(chTemp == L'.')
			{
				iDotCount++;
				if(iDotCount > 1)
				{
					return false;
				}
			}
			if( !iswdigit(chTemp) && chTemp != L'.')
			{
				return false;
			}
		}
		
		if(chTemp == L'.')
		{
			return false;
		}
		
		chTemp = *szSrc;
		if( !iswdigit(chTemp) )
		{
			if(chTemp != L'-' && chTemp != L'+')
			{
				return false;
			}
		}
	}
	else if(iSrcLen == 1)
	{
		chTemp = *szSrc;
		if( !iswdigit(chTemp) )
		{
			return false;
		}
	}
	return true;
}

bool em::EmWsz::IsDomain( const wchar_t* szSrc )
{
	if(szSrc == NULL){
		return false;
	}
	int iSrcLen = wcslen(szSrc);
	int i = 0;
	wchar_t chTemp = 0;
	int iDotPos = 0;
	
	if(iSrcLen < 4
		|| iSrcLen > 50)
	{
		return false;
	}
	
	for(i = 0; i < iSrcLen; i++)
	{
		chTemp = szSrc[i];
		if(iswdigit(chTemp)
			|| iswalpha(chTemp)
			|| chTemp == L'-'
			|| chTemp == L'.'
			)
		{
			continue;
		}
		else
		{
			return false;
		}
	}
	
	
	if(CountStr(szSrc,L".") == 0)
	{
		return false;
	}
	
	iDotPos = Find(szSrc,L".");
	
	if(iDotPos == -1)
	{
		return false;
	}
	
	if(szSrc[iSrcLen - 1] == L'.'
		|| szSrc[0] == L'.')
	{
		return false;
	}
	
	return true;
}

bool em::EmWsz::IsEmail( const wchar_t* szSrc )
{
	if(szSrc == NULL){
		return false;
	}
	int iSrcLen = wcslen(szSrc);
	int i = 0;
	wchar_t chTemp = 0;
	int iAtPos = 0;
	int iDotPos = 0;
	
	if(iSrcLen < 6
		|| iSrcLen > 50)
	{
		return false;
	}
	
	for(i = 0; i < iSrcLen; i++)
	{
		chTemp = szSrc[i];
		if(iswdigit(chTemp)
			|| iswalpha(chTemp)
			|| chTemp == L'-'
			|| chTemp == L'_'
			|| chTemp == L'@'
			|| chTemp == L'.'
			)
		{
			continue;
		}
		else
		{
			return false;
		}
	}
	
	if(CountStr(szSrc,L"@") != 1)
	{
		return false;
	}
	
	iAtPos = Find(szSrc,L"@");
	
	if(iAtPos == 0
		|| iAtPos > iSrcLen - 5)
	{
		return false;
	}
	
	if(CountStr(szSrc,L".") == 0)
	{
		return false;
	}
	
	iDotPos = Find(szSrc,L".");
	
	if(iDotPos == -1)
	{
		return false;
	}
	
	if(szSrc[iSrcLen - 1] == L'.'
		|| szSrc[0] == L'.')
	{
		return false;
	}
	
	return true;
}

bool em::EmWsz::IsIp( const wchar_t* szSrc )
{
	if(szSrc == NULL){
		return false;
	}
	int iSrcLen = wcslen(szSrc);
	int i = 0;
	wchar_t chTemp = 0;
	
	if(iSrcLen < 7
		|| iSrcLen > 15)
	{
		return false;
	}
	
	for(i = 0; i < iSrcLen; i++)
	{
		chTemp = szSrc[i];
		if(iswdigit(chTemp)
			|| chTemp == L'.'
			)
		{
			continue;
		}
		else
		{
			return false;
		}
	}
	
	
	if(CountStr(szSrc,L".") != 3)
	{
		return false;
	}
	
	if(szSrc[iSrcLen - 1] == L'.'
		|| szSrc[0] == L'.')
	{
		return false;
	}
	
	return true;
}

bool em::EmWsz::IsUrl( const wchar_t* szSrc )
{
	if(szSrc == NULL){
		return false;
	}
	int iSrcLen = wcslen(szSrc);
	int i = 0;
	wchar_t chTemp = 0;
	int iDotPos = 0;
	int iProtocolPos = 0;
	wchar_t szValid[100] = L"~!#$%^&()_-+={[}]|:;,./\"'";
	
	if(iSrcLen < 10
		|| iSrcLen > 255)
	{
		return false;
	}
	
	for(i = 0; i < iSrcLen; i++)
	{
		chTemp = szSrc[i];
		if(iswdigit(chTemp)
			|| iswalpha(chTemp)
			|| wcschr(szValid,chTemp) != NULL
			)
		{
			continue;
		}
		else
		{
			return false;
		}
	}
	
	if(CountStr(szSrc,L".") == 0)
	{
		return false;
	}
	
	iDotPos = Find(szSrc,L".");
	
	if(iDotPos == -1)
	{
		return false;
	}
	
	if(szSrc[iSrcLen - 1] == L'.'
		|| szSrc[0] == L'.')
	{
		return false;
	}
	
	iProtocolPos = Find(szSrc,L"://");
	
	if(iProtocolPos == -1
		|| iProtocolPos < 3
		|| iProtocolPos > 10)
	{
		return false;
	}
	
	return true;
}

bool em::EmWsz::IsAsc( const wchar_t* szSrc )
{
	if(szSrc == NULL){
		return false;
	}
	unsigned int iSrcLen = wcslen(szSrc);
	int i = 0;
	for(i = 0; i < iSrcLen; i++)
	{
		if( ! isascii( *(szSrc + i) ))
		{
			return false;
		}
	}
	return true;
}

bool em::EmWsz::IsLimitedIn( const wchar_t* szSrc, const wchar_t* szLimited )
{
	if(szSrc == NULL){
		return false;
	}
	if(szLimited == NULL){
		return false;
	}
	int iSrcLen = wcslen(szSrc);
	for(int i = 0; i < iSrcLen; i++)
	{
		wchar_t ch = szSrc[i];
		if(wcschr(szLimited,ch) == NULL)
		{
			return false;
		}
	}
	return true;
}

bool em::EmWsz::IsDateTime( const wchar_t* szSrc )
{
	if(szSrc == NULL){
		return false;
	}
	int iSrcLen = wcslen(szSrc);

	if(iSrcLen != 19)
	{
		return false;
	}


	if(szSrc[4] != L'-')
	{
		return false;
	}
	if(szSrc[7] != L'-')
	{
		return false;
	}
	if(szSrc[10] != L' ')
	{
		return false;
	}
	if(szSrc[13] != L':')
	{
		return false;
	}
	if(szSrc[16] != L':')
	{
		return false;
	}

	if( !iswdigit(szSrc[0])
		|| !iswdigit(szSrc[18]) )
	{
		return false;
	}

	return true;
}

INT64 em::EmWsz::IpTextToNum( const wchar_t* szIpText )
{
	if(szIpText == NULL){
		return 0;
	}
	if(!IsIp(szIpText))
	{
		return 0;
	}
	
	INT64 iTotal = 0;
	
	wchar_t szPart0[5] = {0};
	wchar_t szPart1[5] = {0};
	wchar_t szPart2[5] = {0};
	wchar_t szPart3[5] = {0};

	int iPointPos0 = -1;

	int iPointPos1 = FindFrom(szIpText,L".",0);
	if(iPointPos1 == -1)
	{
		return 0;
	}

	int iPointPos2 = FindFrom(szIpText,L".",iPointPos1 + 1);
	if(iPointPos2 == -1)
	{
		return 0;
	}

	int iPointPos3 = FindFrom(szIpText,L".",iPointPos2 + 1);
	if(iPointPos3 == -1)
	{
		return 0;
	}

	SubStr(szPart0, szIpText,iPointPos0 + 1,iPointPos1 - iPointPos0 - 1);
	SubStr(szPart1, szIpText,iPointPos1 + 1,iPointPos2 - iPointPos1 - 1);
	SubStr(szPart2, szIpText,iPointPos2 + 1,iPointPos3 - iPointPos2 - 1);
	SubStr(szPart3, szIpText,iPointPos3 + 1, -1);

	INT64 iPart0 = Str2Long(szPart0);
	INT64 iPart1 = Str2Long(szPart1);
	INT64 iPart2 = Str2Long(szPart2);
	INT64 iPart3 = Str2Long(szPart3);
	
	iTotal += (iPart0 << 24);
	iTotal += (iPart1 << 16);
	iTotal += (iPart2 << 8);
	iTotal += iPart3;
	
	return iTotal;
}

wchar_t* em::EmWsz::IpNumToText( wchar_t *szDest, INT64 iIpNum )
{
	if(szDest != NULL){
		szDest[0] = 0;
	}

	if(szDest == NULL){
		return NULL;
	}

	INT64 iTotal = iIpNum;
	
	INT64 iPart0 = (iTotal >> 24);
	INT64 iPart1 = (iTotal >> 16) & 0x00FF;
	INT64 iPart2 = (iTotal >>  8) & 0x0000FF;
	INT64 iPart3 = (iTotal >>  0) & 0x000000FF;

	wchar_t szPart0[5] = {0};
	wchar_t szPart1[5] = {0};
	wchar_t szPart2[5] = {0};
	wchar_t szPart3[5] = {0};

	szDest[0] = 0;

	Long2Str(szPart0,iPart0);
	Long2Str(szPart1,iPart1);
	Long2Str(szPart2,iPart2);
	Long2Str(szPart3,iPart3);

	wcscat(szDest,szPart0);
	wcscat(szDest,L".");
	wcscat(szDest,szPart1);
	wcscat(szDest,L".");
	wcscat(szDest,szPart2);
	wcscat(szDest,L".");
	wcscat(szDest,szPart3);

	return szDest;
}

void em::EmWsz::AllocArray( wchar_t ***szArr, int iRow, int iCol )
{
	int i = 0;
	*szArr = (wchar_t**)malloc(sizeof(wchar_t*) * iRow);
	for(i = 0; i < iRow; i++)
	{
		(*szArr)[i] = (wchar_t*)malloc(sizeof(wchar_t*) * iCol);
		memset((*szArr)[i] ,0,iCol);
	}
}

void em::EmWsz::FreeArray( wchar_t ***szArr, int iRow )
{
	int i = 0;
	if(*szArr == NULL)
	{
		return;
	}
	for(i = 0; i < iRow; i++)
	{
		if((*szArr)[i] == NULL)
		{
			continue;
		}
		EM_FREE_NULL((*szArr)[i] );
	}
	EM_FREE_NULL(*szArr);
}

wchar_t* em::EmWsz::UrlGetProtocol( wchar_t *szDest, const wchar_t* szSrc )
{
	if(szDest != NULL){
		szDest[0] = 0;
	}

	if(szSrc == NULL){
		return NULL;
	}

	int iColonPos = Find(szSrc,L":/");
	int iSrcLen = wcslen(szSrc);

	if(iSrcLen < 3)
	{
		szDest[0] = 0;
		return szDest;
	}

	if(iColonPos == -1)
	{
		szDest[0] = 0;
	}
	else
	{
		SubStr(szDest,szSrc,0,iColonPos);
	}
	Lower(szDest);
	return szDest;
}

wchar_t* em::EmWsz::UrlGetHost( wchar_t *szDest, const wchar_t* szSrc )
{
	if(szDest != NULL){
		szDest[0] = 0;
	}

	if(szSrc == NULL){
		return NULL;
	}

	int iBegin = 0;
	int iEnd = 0;
	int iFindPos = 0;
	int iSrcLen = wcslen(szSrc);
	int iPortColonPos = 0;

	if(iSrcLen < 3)
	{
		szDest[0] = 0;
		return szDest;
	}

	iFindPos = Find(szSrc,L":/");
	if(iFindPos == -1)
	{
		iBegin = 0;
	}else
	{
		iBegin = iFindPos + 2;
		if(szSrc[iBegin] == L'/')
		{
			iBegin++;
		}
	}

	iFindPos = FindFrom(szSrc,L"/",iBegin + 1);
	if(iFindPos == -1)
	{
		iEnd = iSrcLen - 1;
	}
	else
	{
		iEnd = iFindPos - 1;
	}

	SubStr(szDest,szSrc,iBegin,iEnd - iBegin + 1);
	iPortColonPos = Find(szDest,L":");
	if(iPortColonPos != -1)
	{
		szDest[iPortColonPos] = 0;
	}
	return szDest;


}

unsigned short em::EmWsz::UrlGetPort( const wchar_t* szSrc )
{
	if(szSrc == NULL){
		return 0;
	}
	unsigned short iRetPort = 0;
	int iBegin = 0;
	int iEnd = 0;
	int iFindPos = 0;
	int iSrcLen = wcslen(szSrc);
	int iPortColonPos = 0;
	
	wchar_t szTemp[20] = {0};

	if(iSrcLen < 3)
	{
		return 0;
	}
	
	iFindPos = Find(szSrc,L":/");
	if(iFindPos == -1)
	{
		iFindPos = Find(szSrc,L":");
	}
	else
	{
		iFindPos = FindFrom(szSrc,L":",iFindPos + 1);
	}

	if(iFindPos == -1)
	{
		return 80;
	}else
	{
		iBegin = iFindPos + 1;
	}

	iFindPos = FindFrom(szSrc,L"/",iBegin + 1);
	if(iFindPos == -1)
	{
		iEnd = iSrcLen - 1;
	}else
	{
		iEnd = iFindPos - 1;
	}
	
	if(iEnd > iBegin)
	{
		SubStr(szTemp,szSrc,iBegin,iEnd - iBegin + 1);
		iRetPort = _wtoi(szTemp);
	}
	else
	{
		iRetPort = 80;
	}
	
	return iRetPort;
}

wchar_t* em::EmWsz::UrlGetPath( wchar_t *szDest, const wchar_t* szSrc )
{
	if(szDest != NULL){
		szDest[0] = 0;
	}

	if(szSrc == NULL || szDest == NULL){
		return NULL;
	}


	int iSrcLen = wcslen(szSrc);
	int iFindPos = 0;

	iFindPos = FindFrom(szSrc,L":/",0);
	if(iFindPos != -1)
	{
		iFindPos += 3;
		if(szSrc[iFindPos] == L'/')
		{
			iFindPos++;
		}
	}
	iFindPos = FindFrom(szSrc,L"/",iFindPos);
	if(iFindPos == -1)
	{
		wcscpy(szDest,L"/");
	}
	else
	{
		SubStr(szDest,szSrc,iFindPos,-1);
	}

	iFindPos = Find(szDest,L"?");
	if(iFindPos != -1)
	{
		szDest[iFindPos] = 0;
	}

	return szDest;
}

wchar_t* em::EmWsz::UrlGetParam( wchar_t *szDest, const wchar_t* szSrc )
{
	if(szDest != NULL){
		szDest[0] = 0;
	}

	if(szSrc == NULL || szDest == NULL){
		return NULL;
	}
	
	int iSrcLen = wcslen(szSrc);
	int iFindPos = 0;

	iFindPos = FindFrom(szSrc,L"?",0);
	if(iFindPos == -1)
	{
		wcscpy(szDest,EM_EMPTY_WSZ);
	}
	else
	{
		SubStr(szDest,szSrc,iFindPos + 1,-1);
	}
	
	return szDest;
}

wchar_t* em::EmWsz::UrlGetPathWithParam( wchar_t *szDest, const wchar_t* szSrc )
{
	if(szDest != NULL){
		szDest[0] = 0;
	}

	if(szSrc == NULL || szDest == NULL){
		return NULL;
	}
	
	int iSrcLen = wcslen(szSrc);
	int iFindPos = 0;
	
	iFindPos = FindFrom(szSrc,L":/",0);
	if(iFindPos != -1)
	{
		iFindPos += 3;
		if(szSrc[iFindPos] == L'/')
		{
			iFindPos++;
		}
	}
	else
	{
		iFindPos = 0;
	}

	iFindPos = FindFrom(szSrc,L"/",iFindPos);
	if(iFindPos == -1)
	{
		wcscpy(szDest,L"/");
	}
	else
	{
		SubStr(szDest,szSrc,iFindPos,-1);
	}
	
	return szDest;
}



int em::EmWsz::FindChar( const wchar_t *szSrc, wchar_t chFind, unsigned int iFromPos )
{
	if(szSrc == NULL){
		return -1;
	}
	int iRetPos = -1;
	int iSrcLen = wcslen(szSrc);
	int i = 0;
	if(iFromPos >= iSrcLen)
	{
		return -1;
	}

	for(i = iFromPos; i < iSrcLen; i++)
	{
		if(szSrc[i] == chFind)
		{
			return i;
		}
	}

	return iRetPos;
}

bool em::EmWsz::Equal( const wchar_t* szLeft, const wchar_t* szRight )
{
	if(szLeft == NULL && szRight == NULL)
	{
		return true;
	}
	else if( ! ( (szLeft != NULL) && (szRight != NULL) ) )
	{
		return false;
	}

	if(wcscmp(szLeft,szRight) != 0)
	{
		return false;
	}
	return true;
}

bool em::EmWsz::EqualNoCase( const wchar_t* szLeft, const wchar_t* szRight )
{
	
	int iLeftLen = 0;
	int iRightLen = 0;
	int i = 0;
	
	if(szLeft == NULL && szRight == NULL)
	{
		return true;
	}
	else if( ! ( (szLeft != NULL) && (szRight != NULL) ) )
	{
		return false;
	}

	iLeftLen = wcslen(szLeft);
	iRightLen = wcslen(szRight);
	if(iLeftLen != iRightLen)
	{
		return false;
	}

	for(i = 0; i < iLeftLen; i++)
	{
		if( towupper( szLeft[i] ) != towupper(szRight[i]))
		{
			return false;
		}
	}

	return true;
}

wchar_t* em::EmWsz::PadRight( wchar_t* szSrc, wchar_t chPad, int iTotalLen )
{
	if(szSrc == NULL){
		return NULL;
	}
	int iSrcLen = wcslen(szSrc);
	int iPadLen = iTotalLen - iSrcLen;
	int i = 0;
	if(iSrcLen >= iTotalLen)
	{
		return szSrc;
	}
	
	for(i = 0; i < iPadLen; i++)
	{
		szSrc[i + iSrcLen] = chPad;
	}
	
	return szSrc;
}

int em::EmWsz::FindSafe( const wchar_t* szSrc, const wchar_t* szFind, unsigned int iLimit )
{
	if(szSrc == NULL || szFind == NULL){
		return -1;
	}

	int i = 0;
	bool bCheck = false;
	int iCheckPos = 0;
	wchar_t chTemp = 0;
	if(szSrc == NULL || szFind == NULL){
		return -1;
	}
	for(i = 0; i < iLimit; i++){
		chTemp = szSrc[i];
		if(bCheck){
			if(chTemp == szFind[iCheckPos + 1]){
				iCheckPos++;
			}else{
				bCheck = false;
				iCheckPos = 0;
			}
		}else{
			if(chTemp == szFind[0]){
				bCheck = true;
				iCheckPos = 0;
			}
		}

		if(bCheck && iCheckPos + 1 == wcslen(szFind)){
			return i - iCheckPos;
		}
	}
	return -1;
}

wchar_t* em::EmWsz::FileDirFromPath( wchar_t* szDest, const wchar_t* szSrc )
{
	if(szDest != NULL){
		szDest[0] = 0;
	}

	if(szSrc == NULL || szDest == NULL){
		return NULL;
	}

	wcscpy(szDest,szSrc);
	Replace(szDest,L"\\",L"/");
	int nSlashPos = FindReverse(szDest,L"/");
	if(nSlashPos == -1)
	{
		szDest[0] = 0;
		return szDest;
	}
	Cut(szDest,nSlashPos + 1,wcslen(szDest) - nSlashPos - 1);
	return szDest;
}

wchar_t* em::EmWsz::FileNameFromPath( wchar_t* szDest, const wchar_t* szSrc )
{
	if(szDest != NULL){
		szDest[0] = 0;
	}

	if(szSrc == NULL || szDest == NULL){
		return NULL;
	}
	wcscpy(szDest,szSrc);
	Replace(szDest,L"\\",L"/");
	int nSlashPos = FindReverse(szDest,L"/");
	if(nSlashPos == -1)
	{
		szDest[0] = 0;
		return szDest;
	}
	Cut(szDest,0,nSlashPos + 1);
	return szDest;
}

wchar_t* em::EmWsz::FileTitleFromPath( wchar_t* szDest, const wchar_t* szSrc )
{
	if(szDest != NULL){
		szDest[0] = 0;
	}
	
	if(szSrc == NULL || szDest == NULL){
		return NULL;
	}
	
	FileNameFromPath(szDest,szSrc);
	int nSlashPos = FindReverse(szDest,L".");
	if(nSlashPos == -1)
	{
		szDest[0] = 0;
		return szDest;
	}
	Cut(szDest,nSlashPos,wcslen(szDest) - nSlashPos);
	return szDest;
}

wchar_t* em::EmWsz::FileExtFromPath( wchar_t* szDest, const wchar_t* szSrc )
{
	if(szDest != NULL){
		szDest[0] = 0;
	}

	if(szSrc == NULL || szDest == NULL){
		return NULL;
	}
	wcscpy(szDest,szSrc);
	int nSlashPos = FindReverse(szDest,L".");
	if(nSlashPos == -1)
	{
		szDest[0] = 0;
		return szDest;
	}
	Cut(szDest,0,nSlashPos);
	return szDest;
}

wchar_t* em::EmWsz::FileFixDir( wchar_t* szDest, const wchar_t* szSrc )
{
	if(szDest != NULL){
		szDest[0] = 0;
	}
	if(szSrc == NULL || szDest == NULL){
		return NULL;
	}

	wcscpy(szDest,szSrc);

	Replace(szDest,L"\\",L"/");
	if(szDest[wcslen(szDest) - 1] != L'/'){
		wcscat(szDest,L"/");
	}

	return szDest;
}
