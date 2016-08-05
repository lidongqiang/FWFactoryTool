#include "stdafx.h"
#include "../inc/EmTsz.h"
#include "../inc/EmUtil.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

using namespace em;

TCHAR* em::EmTsz::Int2Str( TCHAR* szDest, int iNum )
{
	if(szDest == NULL){
		return NULL;
	}
	_stprintf(szDest,_T("%d"),iNum);
	return szDest;
}

TCHAR* em::EmTsz::Long2Str( TCHAR* szDest,INT64 iNum )
{
	if(szDest == NULL){
		return NULL;
	}
	_i64tot(iNum,szDest,10);
	return szDest;
}

int em::EmTsz::Str2Int(const TCHAR* szSrc )
{
	if(szSrc == NULL || _tcslen(szSrc) == 0){
		return 0;
	}

	return _ttoi(szSrc);
}

INT64 em::EmTsz::Str2Long( const TCHAR* szSrc )
{
	if(szSrc == NULL || _tcslen(szSrc) == 0){
		return 0;
	}

	return _ttoi64(szSrc);
}

TCHAR* em::EmTsz::Float2Str( TCHAR *szDest, double fNum )
{
	if(szDest == NULL){
		return NULL;
	}
	_stprintf(szDest,_T("%f"),fNum);
	return szDest;
}

int em::EmTsz::Char2Int( TCHAR chSrc )
{
	if(!IsDigit(chSrc)){
		return 0;
	}
	return (int)chSrc - 0x30;
}

TCHAR* em::EmTsz::Char2Str( TCHAR *szDest, TCHAR chSrc )
{
	if(szDest == NULL){
		return NULL;
	}
	szDest[0] = chSrc;
	szDest[1] = 0;
	return szDest;
}

TCHAR em::EmTsz::Str2Char( const TCHAR* szSrc )
{
	if(szSrc == NULL){
		return 0;
	}
	return szSrc[0];
}

TCHAR* em::EmTsz::Lower( TCHAR* szSrc )
{
	if(szSrc == NULL){
		return NULL;
	}
	TCHAR *pMove = szSrc;
	while(*pMove != 0)
	{
		*pMove = _totlower(*pMove);
		pMove++;
	}
	return szSrc;
}

TCHAR* em::EmTsz::Upper( TCHAR* szSrc )
{
	if(szSrc == NULL){
		return NULL;
	}
	TCHAR *pMove = szSrc;
	while(*pMove != 0)
	{
		*pMove = _totupper(*pMove);
		pMove++;
	}
	return szSrc;
}

TCHAR* em::EmTsz::Reverse( TCHAR* szSrc )
{
	if(szSrc == NULL){
		return NULL;
	}
	int iSrcLen = _tcslen(szSrc);
	int i = 0;
	TCHAR chTemp = 0;
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

unsigned int em::EmTsz::CountChar( const TCHAR* szSrc, TCHAR chFind )
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

unsigned int em::EmTsz::CountStr( const TCHAR* szSrc, const TCHAR* szFind )
{
	if(szSrc == NULL || szFind == NULL){
		return 0;
	}
	unsigned int iRetCount = 0;
	bool bMatch = 0;
	int iFromPos = 0;
	unsigned int iSrcLen = _tcslen(szSrc);
	unsigned int iFindLen = _tcslen(szFind);
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

int em::EmTsz::FindFrom( const TCHAR *szSrc, const TCHAR *szFind, unsigned int iFromPos)
{
	if(szSrc == NULL || szFind == NULL){
		return -1;
	}
	int bMatch = 0;
	unsigned int iSrcLen = _tcslen(szSrc);
	unsigned int iFindLen = _tcslen(szFind);
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

TCHAR* em::EmTsz::SubStr( TCHAR *szDest, const TCHAR *szSrc, unsigned int iStart, int iSubLen )
{
	if(szDest != NULL){
		szDest[0] = 0;
	}
	if(szSrc == NULL || szDest == NULL){
		return 0;
	}
	unsigned int iSrcLen = _tcslen(szSrc);
	unsigned int i = 0;
	
	if(iSubLen == -1)
	{
		iSubLen = iSrcLen - iStart;
	}
	
	if(iStart + iSubLen > iSrcLen)
	{
		return NULL;
	}
	
	memcpy(szDest, szSrc + iStart, iSubLen * sizeof(TCHAR));
	szDest[iSubLen] = 0;
	return szDest;
}

int em::EmTsz::Find( const TCHAR *szSrc, const TCHAR *szFind )
{
	return FindFrom(szSrc,szFind,0);
}

TCHAR* em::EmTsz::Repeat(TCHAR *szDest, const TCHAR* szSrc, int iCount )
{
	if(szDest != NULL){
		szDest[0] = 0;
	}
	if(szSrc == NULL){
		return szDest;
	}
	size_t i = 0;
	int iLen = _tcslen(szSrc);
	int iResultLen = iLen * iCount;
	for(i = 0; i < iCount; i++)
	{
		memcpy(szDest + (i * iLen),szSrc,iLen * sizeof(TCHAR));
	}
	szDest[iResultLen] = 0;
	return szDest;
}

TCHAR* em::EmTsz::Prefix(TCHAR* szSrc, const TCHAR* szPrefix )
{
	if(szSrc == NULL){
		return NULL;
	}
	if(szPrefix == NULL){
		return szSrc;
	}
	int iSrcLen = _tcslen(szSrc);
	int iPreLen = _tcslen(szPrefix);
	int iResultLen = iSrcLen + iPreLen;
	memmove(szSrc + iPreLen,szSrc,iSrcLen * sizeof(TCHAR));
	memcpy(szSrc,szPrefix,iPreLen * sizeof(TCHAR));
	szSrc[iResultLen] = 0;
	return szSrc;
}

TCHAR* em::EmTsz::PadLeft(TCHAR* szSrc, TCHAR chPad, int iTotalLen )
{
	if(szSrc == NULL){
		return NULL;
	}
	int iSrcLen = _tcslen(szSrc);
	int iPadLen = iTotalLen - iSrcLen;
	int i = 0;
	if(iSrcLen >= iTotalLen)
	{
		return szSrc;
	}

	memmove(szSrc + iPadLen,szSrc,iSrcLen * sizeof(TCHAR));

	for(i = 0; i < iPadLen; i++)
	{
		szSrc[i] = chPad;
	}

	return szSrc;
}

TCHAR* em::EmTsz::AddSlashes( TCHAR* szSrc, TCHAR chFind )
{
	if(szSrc == NULL){
		return NULL;
	}
	int iSrcLen = _tcslen(szSrc);
	TCHAR *szCopy = (TCHAR*)malloc((iSrcLen * 2 + 1) * sizeof(TCHAR));
	int i = 0;
	int iPos = 0;

	if(szCopy == NULL)
	{
		return NULL;
	}
	
	for(i = 0; i < iSrcLen; i++)
	{
		if(szSrc[i] == chFind
			|| szSrc[i] == _T('\\'))
		{
			szCopy[iPos++] = _T('\\');
			szCopy[iPos++] = szSrc[i];
		}
		else
		{
			szCopy[iPos++] = szSrc[i];
		}
	}
	szCopy[iPos++] = 0;
	_tcscpy(szSrc,szCopy);
	EM_FREE_NULL(szCopy);
	return szSrc;
}

TCHAR* em::EmTsz::StripSlashes( TCHAR* szSrc )
{
	if(szSrc == NULL){
		return NULL;
	}
	int iSrcLen = _tcslen(szSrc);
	TCHAR *szCopy = (TCHAR*)malloc((iSrcLen + 1) * sizeof(TCHAR));
	int i = 0;
	int iPos = 0;
	
	if(szCopy == NULL)
	{
		return NULL;
	}
	
	bool bAddLast = true;
	for(i = 0; i < iSrcLen - 1; i++)
	{
		if(szSrc[i] == _T('\\'))
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
	_tcscpy(szSrc,szCopy);
	EM_FREE_NULL(szCopy);
	return szSrc;
}

TCHAR* em::EmTsz::Dec2Bin(TCHAR* szSrc )
{
	if(szSrc == NULL){
		return NULL;
	}
	INT64 num = _ttoi64(szSrc);
	TCHAR szTemp[255] = {0};
	INT64 iTempQuotient = num;
	INT64 iTempRemainder = 0;
	int iPos = 0;
	while(iTempQuotient / 2 != 0)
	{
		iTempRemainder = iTempQuotient % 2;
		iTempQuotient = iTempQuotient / 2;
		
		if(iTempRemainder == 0)
		{
			szTemp[iPos++] = _T('0');
		}
		else
		{
			szTemp[iPos++] = _T('1');
		}
	}
	if(iTempQuotient == 1)
	{
		szTemp[iPos++] = _T('1');
	}
	_tcscpy(szSrc,Reverse(szTemp));
	return szSrc;
}

TCHAR* em::EmTsz::Bin2Dec( TCHAR* szSrc )
{
	if(szSrc == NULL){
		return NULL;
	}
	int iSrcLen = _tcslen(szSrc);
	int i = 0;
	INT64 iSum = 0;
	int iPow = 0;
	if(iSrcLen <= 0)
	{
		return szSrc;
	}
	for(i = iSrcLen - 1; i >= 0; i--)
	{
		if(szSrc[i] != _T('0'))
		{
			iSum += EmUtil::PowPlus(2,iPow);
		}
		iPow++;
	}
	_stprintf(szSrc,_T("%d"),iSum);
	return szSrc;
}

TCHAR* em::EmTsz::Hex2Dec( TCHAR* szSrc )
{
	if(szSrc == NULL){
		return NULL;
	}
	int iSrcLen = _tcslen(szSrc);
	int i = 0;
	INT64 iSum = 0;
	int iPow = 0;
	int iDigit = 0;
	for(i = iSrcLen - 1; i >= 0; i--)
	{
		iDigit = em::EmUtil::HexChar2Int(szSrc[i]);
		if(szSrc[i] != _T('0'))
		{
			iSum += ( iDigit * EmUtil::PowPlus(16,iPow) );
		}
		iPow++;
	}
	_stprintf(szSrc,_T("%d"),iSum);
	return szSrc;
}

TCHAR* em::EmTsz::Dec2Hex( TCHAR* szSrc )
{
	if(szSrc == NULL){
		return NULL;
	}
	INT64 iDec = _ttoi64(szSrc);
	_stprintf(szSrc,_T("%x"),iDec);
	return szSrc;
}

TCHAR* em::EmTsz::Bin2Hex( TCHAR* szSrc )
{
	if(szSrc == NULL){
		return NULL;
	}
	Bin2Dec(szSrc);
	Dec2Hex(szSrc);
	return szSrc;
}

TCHAR* em::EmTsz::Hex2Bin( TCHAR* szSrc )
{
	if(szSrc == NULL){
		return NULL;
	}
	Hex2Dec(szSrc);
	Dec2Bin(szSrc);
	return szSrc;
}

int em::EmTsz::FindReverse( const TCHAR* szSrc, const TCHAR* szFind )
{
	if(szSrc == NULL || szFind == NULL){
		return 0;
	}
	int iFromPos = 0;
	int bMatch = 0;
	int iSrcLen = _tcslen(szSrc);
	int iFindLen = _tcslen(szFind);
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

TCHAR* em::EmTsz::TrimLeft( TCHAR* szSrc )
{
	if(szSrc == NULL){
		return NULL;
	}
	TCHAR *szBegin = szSrc;
	int iSrcLen = _tcslen(szSrc);
	int iBeginSpaces = 0;
	int iDestLen = 0;
	int i = 0;
	
	if(iSrcLen == 0)
	{
		return szSrc;
	}
	
	while(_istspace(*szBegin))
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

TCHAR* em::EmTsz::TrimRight( TCHAR* szSrc )
{
	if(szSrc == NULL){
		return NULL;
	}
	int iSrcLen = _tcslen(szSrc);
	TCHAR *szEnd = szSrc + iSrcLen - 1;
	
	if(iSrcLen == 0)
	{
		return szSrc;
	}
	
	while(_istspace(*szEnd))
	{
		szEnd--;
	}
	szEnd++;
	*szEnd = 0;
	
	return szSrc;
}

TCHAR* em::EmTsz::Trim( TCHAR* szSrc )
{
	if(szSrc == NULL){
		return NULL;
	}
	TrimLeft(szSrc);
	TrimRight(szSrc);
	return szSrc;
}

TCHAR* em::EmTsz::FormatNum(TCHAR* szSrc, unsigned int nLenAfterPoint )
{
	if(szSrc == NULL){
		return NULL;
	}
	int iSrcLen = _tcslen(szSrc);
	TCHAR szDest[255] = {0};
	int i = 0;
	int iDotPos = 0;
	TCHAR szPlusZeros[255] = {0};
	TCHAR szIntPart[255] = {0};
	TCHAR szDecPart[255] = {0};
	size_t iSrcDecLen = 0;
	TCHAR szNewInt[255] = {0};
	int iNewInt = 0;
	TCHAR chNext = _T('0');
	int iNextNum = 0;
	TCHAR szNewDec[255] = {0};
	
	if(iSrcLen == 0)
	{
		return szSrc;
	}
	
	iDotPos = Find(szSrc,_T("."));
	
	if(iDotPos == -1)
	{
		Repeat(szPlusZeros,_T("0"),nLenAfterPoint);
		_tcscpy(szDest,szSrc);
		_tcscat(szDest,_T("."));
		_tcscat(szDest,szPlusZeros);
	}
	else
	{
		SubStr(szIntPart,szSrc,0,iDotPos);
		SubStr(szDecPart,szSrc,iDotPos + 1,iSrcLen - 1 - iDotPos);
		iSrcDecLen = _tcslen(szDecPart);
		if(iSrcDecLen == nLenAfterPoint)
		{
			_tcscpy(szDest,szSrc);
		}
		else if(iSrcDecLen < nLenAfterPoint)
		{
			Repeat(szPlusZeros,_T("0"),nLenAfterPoint - iSrcDecLen);
			_tcscpy(szDest,szSrc);
			_tcscat(szDest,szPlusZeros);
		}
		else
		{
			_tcscpy(szNewInt,szIntPart);
			_tcsncat(szNewInt,szDecPart,nLenAfterPoint);
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
				_tcscpy(szDest,szNewInt);
			}
			else
			{
				if(_tcslen(szNewInt) <= nLenAfterPoint)
				{
					Repeat(szPlusZeros,_T("0"),nLenAfterPoint - _tcslen(szNewInt) + 1);
					Prefix(szPlusZeros,szNewInt);
				}
				
				SubStr(szNewDec,szNewInt,_tcslen(szNewInt) - nLenAfterPoint,nLenAfterPoint);
				_tcsncpy(szDest,szNewInt,_tcslen(szNewInt) - nLenAfterPoint);
				_tcscat(szDest,_T("."));
				
				_tcscat(szDest,szNewDec);
			}
		}
	}
	
	_tcscpy(szSrc,szDest);
	return szSrc;
}

TCHAR* em::EmTsz::Cut(TCHAR* szSrc, unsigned int nStart, unsigned int nLen )
{
	if(szSrc == NULL){
		return NULL;
	}
	size_t iSrcLen = _tcslen(szSrc);
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

TCHAR* em::EmTsz::Insert( TCHAR* szSrc, const TCHAR* szInsert, unsigned int nStart )
{
	if(szSrc == NULL){
		return NULL;
	}
	if(szInsert == NULL){
		szInsert = EM_EMPTY_TSZ;
	}
	size_t iSrcLen = _tcslen(szSrc);
	size_t iAppendLen = _tcslen(szInsert);
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

TCHAR* em::EmTsz::Replace( TCHAR* szSrc, const TCHAR* szFind, const TCHAR* szReplace )
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
	int iSrcLen = _tcslen(szSrc);
	int iFindLen = _tcslen(szFind);
	int iReplaceLen = _tcslen(szReplace);
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

TCHAR* em::EmTsz::Splice( TCHAR* szSrc, const TCHAR* szInsert, unsigned int nStart, unsigned int nLen )
{
	if(szSrc == NULL){
		return NULL;
	}
	if(szInsert == NULL){
		szInsert = EM_EMPTY_WSZ;
	}
	size_t iSrcLen = _tcslen(szSrc);
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

bool em::EmTsz::IsDigit( TCHAR chSrc )
{
	return _istdigit(chSrc);
}

bool em::EmTsz::IsLetter( TCHAR chSrc )
{
	if((chSrc >= _T('a') && chSrc <= _T('z') )
		||(chSrc >= _T('A') && chSrc <= _T('Z') )
	)
	{
		return true;
	}
	return false;
}

bool em::EmTsz::IsInt( const TCHAR* szSrc )
{
	if(szSrc == NULL){
		return false;
	}
	int iSrcLen = _tcslen(szSrc);
	int i = 0;
	if(iSrcLen == 0)
	{
		return false;
	}
	
	if(szSrc[0] != _T('-') && szSrc[0] != _T('+') && !_istdigit(szSrc[0]))
	{
		return false;
	}

	for(i = 1; i < iSrcLen; i++)
	{
		if(!_istdigit(szSrc[i]))
		{
			return false;
		}
	}
	return true;
}

bool em::EmTsz::IsHexNumber( const TCHAR* szSrc )
{
	if(szSrc == NULL || _tcslen(szSrc) == 0){
		return false;
	}

	TCHAR *szValid = _T("abcdefABCDEF0123456789");
	int iSrcLen = _tcslen(szSrc);
	int i = 0;
	if(iSrcLen == 0)
	{
		return false;
	}
	
	if( _tcscmp(szSrc,_T("0x"))==0
		|| _tcscmp(szSrc,_T("0X")) == 0)
	{
		return false;
	}
	
	for(i = 0; i < iSrcLen; i++)
	{
		if(_tcschr(szValid,szSrc[i]) == NULL)
		{
			if(i == 1 && (szSrc[i] == _T('x') || szSrc[i] == _T('X')))
			{
				continue;
			}
			return false;
		}
	}
	
	return true;
}

bool em::EmTsz::IsNumber( const TCHAR* szSrc )
{
	if(szSrc == NULL){
		return false;
	}
	unsigned int iSrcLen = _tcslen(szSrc);
	unsigned int i = 0;
	TCHAR chTemp = 0;
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
			if(chTemp == _T('.'))
			{
				iDotCount++;
				if(iDotCount > 1)
				{
					return false;
				}
			}
			if( !_istdigit(chTemp) && chTemp != _T('.'))
			{
				return false;
			}
		}
		
		if(chTemp == _T('.'))
		{
			return false;
		}
		
		chTemp = *szSrc;
		if( !_istdigit(chTemp) )
		{
			if(chTemp != _T('-') && chTemp != _T('+'))
			{
				return false;
			}
		}
	}
	else if(iSrcLen == 1)
	{
		chTemp = *szSrc;
		if( !_istdigit(chTemp) )
		{
			return false;
		}
	}
	return true;
}

bool em::EmTsz::IsDomain( const TCHAR* szSrc )
{
	if(szSrc == NULL){
		return false;
	}
	int iSrcLen = _tcslen(szSrc);
	int i = 0;
	TCHAR chTemp = 0;
	int iDotPos = 0;
	
	if(iSrcLen < 4
		|| iSrcLen > 50)
	{
		return false;
	}
	
	for(i = 0; i < iSrcLen; i++)
	{
		chTemp = szSrc[i];
		if(_istdigit(chTemp)
			|| _istalpha(chTemp)
			|| chTemp == _T('-')
			|| chTemp == _T('.')
			)
		{
			continue;
		}
		else
		{
			return false;
		}
	}
	
	
	if(CountStr(szSrc,_T(".")) == 0)
	{
		return false;
	}
	
	iDotPos = Find(szSrc,_T("."));
	
	if(iDotPos == -1)
	{
		return false;
	}
	
	if(szSrc[iSrcLen - 1] == _T('.')
		|| szSrc[0] == _T('.'))
	{
		return false;
	}
	
	return true;
}

bool em::EmTsz::IsEmail( const TCHAR* szSrc )
{
	if(szSrc == NULL){
		return false;
	}
	int iSrcLen = _tcslen(szSrc);
	int i = 0;
	TCHAR chTemp = 0;
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
		if(_istdigit(chTemp)
			|| _istalpha(chTemp)
			|| chTemp == _T('-')
			|| chTemp == _T('_')
			|| chTemp == _T('@')
			|| chTemp == _T('.')
			)
		{
			continue;
		}
		else
		{
			return false;
		}
	}
	
	if(CountStr(szSrc,_T("@")) != 1)
	{
		return false;
	}
	
	iAtPos = Find(szSrc,_T("@"));
	
	if(iAtPos == 0
		|| iAtPos > iSrcLen - 5)
	{
		return false;
	}
	
	if(CountStr(szSrc,_T(".")) == 0)
	{
		return false;
	}
	
	iDotPos = Find(szSrc,_T("."));
	
	if(iDotPos == -1)
	{
		return false;
	}
	
	if(szSrc[iSrcLen - 1] == _T('.')
		|| szSrc[0] == _T('.'))
	{
		return false;
	}
	
	return true;
}

bool em::EmTsz::IsIp( const TCHAR* szSrc )
{
	if(szSrc == NULL){
		return false;
	}
	int iSrcLen = _tcslen(szSrc);
	int i = 0;
	TCHAR chTemp = 0;
	
	if(iSrcLen < 7
		|| iSrcLen > 15)
	{
		return false;
	}
	
	for(i = 0; i < iSrcLen; i++)
	{
		chTemp = szSrc[i];
		if(_istdigit(chTemp)
			|| chTemp == _T('.')
			)
		{
			continue;
		}
		else
		{
			return false;
		}
	}
	
	
	if(CountStr(szSrc,_T(".")) != 3)
	{
		return false;
	}
	
	if(szSrc[iSrcLen - 1] == _T('.')
		|| szSrc[0] == _T('.'))
	{
		return false;
	}
	
	return true;
}

bool em::EmTsz::IsUrl( const TCHAR* szSrc )
{
	if(szSrc == NULL){
		return false;
	}
	int iSrcLen = _tcslen(szSrc);
	int i = 0;
	TCHAR chTemp = 0;
	int iDotPos = 0;
	int iProtocolPos = 0;
	TCHAR szValid[100] = _T("~!#$%^&()_-+={[}]|:;,./\"'");
	
	if(iSrcLen < 10
		|| iSrcLen > 255)
	{
		return false;
	}
	
	for(i = 0; i < iSrcLen; i++)
	{
		chTemp = szSrc[i];
		if(_istdigit(chTemp)
			|| _istalpha(chTemp)
			|| _tcschr(szValid,chTemp) != NULL
			)
		{
			continue;
		}
		else
		{
			return false;
		}
	}
	
	if(CountStr(szSrc,_T(".")) == 0)
	{
		return false;
	}
	
	iDotPos = Find(szSrc,_T("."));
	
	if(iDotPos == -1)
	{
		return false;
	}
	
	if(szSrc[iSrcLen - 1] == _T('.')
		|| szSrc[0] == _T('.'))
	{
		return false;
	}
	
	iProtocolPos = Find(szSrc,_T("://"));
	
	if(iProtocolPos == -1
		|| iProtocolPos < 3
		|| iProtocolPos > 10)
	{
		return false;
	}
	
	return true;
}

bool em::EmTsz::IsAsc( const TCHAR* szSrc )
{
	if(szSrc == NULL){
		return false;
	}
	unsigned int iSrcLen = _tcslen(szSrc);
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

bool em::EmTsz::IsLimitedIn( const TCHAR* szSrc, const TCHAR* szLimited )
{
	if(szSrc == NULL){
		return false;
	}
	if(szLimited == NULL){
		return false;
	}
	int iSrcLen = _tcslen(szSrc);
	for(int i = 0; i < iSrcLen; i++)
	{
		TCHAR ch = szSrc[i];
		if(_tcschr(szLimited,ch) == NULL)
		{
			return false;
		}
	}
	return true;
}

bool em::EmTsz::IsDateTime( const TCHAR* szSrc )
{
	if(szSrc == NULL){
		return false;
	}
	int iSrcLen = _tcslen(szSrc);

	if(iSrcLen != 19)
	{
		return false;
	}


	if(szSrc[4] != _T('-'))
	{
		return false;
	}
	if(szSrc[7] != _T('-'))
	{
		return false;
	}
	if(szSrc[10] != _T(' '))
	{
		return false;
	}
	if(szSrc[13] != _T(':'))
	{
		return false;
	}
	if(szSrc[16] != _T(':'))
	{
		return false;
	}

	if( !_istdigit(szSrc[0])
		|| !_istdigit(szSrc[18]) )
	{
		return false;
	}

	return true;
}

INT64 em::EmTsz::IpTextToNum( const TCHAR* szIpText )
{
	if(szIpText == NULL){
		return 0;
	}
	if(!IsIp(szIpText))
	{
		return 0;
	}
	
	INT64 iTotal = 0;
	
	TCHAR szPart0[5] = {0};
	TCHAR szPart1[5] = {0};
	TCHAR szPart2[5] = {0};
	TCHAR szPart3[5] = {0};

	int iPointPos0 = -1;

	int iPointPos1 = FindFrom(szIpText,_T("."),0);
	if(iPointPos1 == -1)
	{
		return 0;
	}

	int iPointPos2 = FindFrom(szIpText,_T("."),iPointPos1 + 1);
	if(iPointPos2 == -1)
	{
		return 0;
	}

	int iPointPos3 = FindFrom(szIpText,_T("."),iPointPos2 + 1);
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

TCHAR* em::EmTsz::IpNumToText( TCHAR *szDest, INT64 iIpNum )
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

	TCHAR szPart0[5] = {0};
	TCHAR szPart1[5] = {0};
	TCHAR szPart2[5] = {0};
	TCHAR szPart3[5] = {0};

	szDest[0] = 0;

	Long2Str(szPart0,iPart0);
	Long2Str(szPart1,iPart1);
	Long2Str(szPart2,iPart2);
	Long2Str(szPart3,iPart3);

	_tcscat(szDest,szPart0);
	_tcscat(szDest,_T("."));
	_tcscat(szDest,szPart1);
	_tcscat(szDest,_T("."));
	_tcscat(szDest,szPart2);
	_tcscat(szDest,_T("."));
	_tcscat(szDest,szPart3);

	return szDest;
}

void em::EmTsz::AllocArray( TCHAR ***szArr, int iRow, int iCol )
{
	int i = 0;
	*szArr = (TCHAR**)malloc(sizeof(TCHAR*) * iRow);
	for(i = 0; i < iRow; i++)
	{
		(*szArr)[i] = (TCHAR*)malloc(sizeof(TCHAR*) * iCol);
		memset((*szArr)[i] ,0,iCol);
	}
}

void em::EmTsz::FreeArray( TCHAR ***szArr, int iRow )
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

TCHAR* em::EmTsz::UrlGetProtocol( TCHAR *szDest, const TCHAR* szSrc )
{
	if(szDest != NULL){
		szDest[0] = 0;
	}

	if(szSrc == NULL){
		return NULL;
	}

	int iColonPos = Find(szSrc,_T(":/"));
	int iSrcLen = _tcslen(szSrc);

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

TCHAR* em::EmTsz::UrlGetHost( TCHAR *szDest, const TCHAR* szSrc )
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
	int iSrcLen = _tcslen(szSrc);
	int iPortColonPos = 0;

	if(iSrcLen < 3)
	{
		szDest[0] = 0;
		return szDest;
	}

	iFindPos = Find(szSrc,_T(":/"));
	if(iFindPos == -1)
	{
		iBegin = 0;
	}else
	{
		iBegin = iFindPos + 2;
		if(szSrc[iBegin] == _T('/'))
		{
			iBegin++;
		}
	}

	iFindPos = FindFrom(szSrc,_T("/"),iBegin + 1);
	if(iFindPos == -1)
	{
		iEnd = iSrcLen - 1;
	}
	else
	{
		iEnd = iFindPos - 1;
	}

	SubStr(szDest,szSrc,iBegin,iEnd - iBegin + 1);
	iPortColonPos = Find(szDest,_T(":"));
	if(iPortColonPos != -1)
	{
		szDest[iPortColonPos] = 0;
	}
	return szDest;


}

unsigned short em::EmTsz::UrlGetPort( const TCHAR* szSrc )
{
	if(szSrc == NULL){
		return 0;
	}
	unsigned short iRetPort = 0;
	int iBegin = 0;
	int iEnd = 0;
	int iFindPos = 0;
	int iSrcLen = _tcslen(szSrc);
	int iPortColonPos = 0;
	
	TCHAR szTemp[20] = {0};

	if(iSrcLen < 3)
	{
		return 0;
	}
	
	iFindPos = Find(szSrc,_T(":/"));
	if(iFindPos == -1)
	{
		iFindPos = Find(szSrc,_T(":"));
	}
	else
	{
		iFindPos = FindFrom(szSrc,_T(":"),iFindPos + 1);
	}

	if(iFindPos == -1)
	{
		return 80;
	}else
	{
		iBegin = iFindPos + 1;
	}

	iFindPos = FindFrom(szSrc,_T("/"),iBegin + 1);
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
		iRetPort = _ttoi(szTemp);
	}
	else
	{
		iRetPort = 80;
	}
	
	return iRetPort;
}

TCHAR* em::EmTsz::UrlGetPath( TCHAR *szDest, const TCHAR* szSrc )
{
	if(szDest != NULL){
		szDest[0] = 0;
	}

	if(szSrc == NULL || szDest == NULL){
		return NULL;
	}


	int iSrcLen = _tcslen(szSrc);
	int iFindPos = 0;

	iFindPos = FindFrom(szSrc,_T(":/"),0);
	if(iFindPos != -1)
	{
		iFindPos += 3;
		if(szSrc[iFindPos] == _T('/'))
		{
			iFindPos++;
		}
	}
	iFindPos = FindFrom(szSrc,_T("/"),iFindPos);
	if(iFindPos == -1)
	{
		_tcscpy(szDest,_T("/"));
	}
	else
	{
		SubStr(szDest,szSrc,iFindPos,-1);
	}

	iFindPos = Find(szDest,_T("?"));
	if(iFindPos != -1)
	{
		szDest[iFindPos] = 0;
	}

	return szDest;
}

TCHAR* em::EmTsz::UrlGetParam( TCHAR *szDest, const TCHAR* szSrc )
{
	if(szDest != NULL){
		szDest[0] = 0;
	}

	if(szSrc == NULL || szDest == NULL){
		return NULL;
	}
	
	int iSrcLen = _tcslen(szSrc);
	int iFindPos = 0;

	iFindPos = FindFrom(szSrc,_T("?"),0);
	if(iFindPos == -1)
	{
		_tcscpy(szDest,EM_EMPTY_WSZ);
	}
	else
	{
		SubStr(szDest,szSrc,iFindPos + 1,-1);
	}
	
	return szDest;
}

TCHAR* em::EmTsz::UrlGetPathWithParam( TCHAR *szDest, const TCHAR* szSrc )
{
	if(szDest != NULL){
		szDest[0] = 0;
	}

	if(szSrc == NULL || szDest == NULL){
		return NULL;
	}
	
	int iSrcLen = _tcslen(szSrc);
	int iFindPos = 0;
	
	iFindPos = FindFrom(szSrc,_T(":/"),0);
	if(iFindPos != -1)
	{
		iFindPos += 3;
		if(szSrc[iFindPos] == _T('/'))
		{
			iFindPos++;
		}
	}
	else
	{
		iFindPos = 0;
	}

	iFindPos = FindFrom(szSrc,_T("/"),iFindPos);
	if(iFindPos == -1)
	{
		_tcscpy(szDest,_T("/"));
	}
	else
	{
		SubStr(szDest,szSrc,iFindPos,-1);
	}
	
	return szDest;
}



int em::EmTsz::FindChar( const TCHAR *szSrc, TCHAR chFind, unsigned int iFromPos )
{
	if(szSrc == NULL){
		return -1;
	}
	int iRetPos = -1;
	int iSrcLen = _tcslen(szSrc);
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

bool em::EmTsz::Equal( const TCHAR* szLeft, const TCHAR* szRight )
{
	if(szLeft == NULL && szRight == NULL)
	{
		return true;
	}
	else if( ! ( (szLeft != NULL) && (szRight != NULL) ) )
	{
		return false;
	}

	if(_tcscmp(szLeft,szRight) != 0)
	{
		return false;
	}
	return true;
}

bool em::EmTsz::EqualNoCase( const TCHAR* szLeft, const TCHAR* szRight )
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

	iLeftLen = _tcslen(szLeft);
	iRightLen = _tcslen(szRight);
	if(iLeftLen != iRightLen)
	{
		return false;
	}

	for(i = 0; i < iLeftLen; i++)
	{
		if( _totupper( szLeft[i] ) != _totupper(szRight[i]))
		{
			return false;
		}
	}

	return true;
}

TCHAR* em::EmTsz::PadRight( TCHAR* szSrc, TCHAR chPad, int iTotalLen )
{
	if(szSrc == NULL){
		return NULL;
	}
	int iSrcLen = _tcslen(szSrc);
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

int em::EmTsz::FindSafe( const TCHAR* szSrc, const TCHAR* szFind, unsigned int iLimit )
{
	if(szSrc == NULL || szFind == NULL){
		return -1;
	}

	int i = 0;
	bool bCheck = false;
	int iCheckPos = 0;
	TCHAR chTemp = 0;
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

		if(bCheck && iCheckPos + 1 == _tcslen(szFind)){
			return i - iCheckPos;
		}
	}
	return -1;
}

TCHAR* em::EmTsz::FileDirFromPath( TCHAR* szDest, const TCHAR* szSrc )
{
	if(szDest != NULL){
		szDest[0] = 0;
	}

	if(szSrc == NULL || szDest == NULL){
		return NULL;
	}

	_tcscpy(szDest,szSrc);
	Replace(szDest,_T("\\"),_T("/"));
	int nSlashPos = FindReverse(szDest,_T("/"));
	if(nSlashPos == -1)
	{
		szDest[0] = 0;
		return szDest;
	}
	Cut(szDest,nSlashPos + 1,_tcslen(szDest) - nSlashPos - 1);
	return szDest;
}

TCHAR* em::EmTsz::FileNameFromPath( TCHAR* szDest, const TCHAR* szSrc )
{
	if(szDest != NULL){
		szDest[0] = 0;
	}

	if(szSrc == NULL || szDest == NULL){
		return NULL;
	}
	_tcscpy(szDest,szSrc);
	Replace(szDest,_T("\\"),_T("/"));
	int nSlashPos = FindReverse(szDest,_T("/"));
	if(nSlashPos == -1)
	{
		szDest[0] = 0;
		return szDest;
	}
	Cut(szDest,0,nSlashPos + 1);
	return szDest;
}

TCHAR* em::EmTsz::FileTitleFromPath( TCHAR* szDest, const TCHAR* szSrc )
{
	if(szDest != NULL){
		szDest[0] = 0;
	}
	
	if(szSrc == NULL || szDest == NULL){
		return NULL;
	}
	
	FileNameFromPath(szDest,szSrc);
	int nSlashPos = FindReverse(szDest,_T("."));
	if(nSlashPos == -1)
	{
		szDest[0] = 0;
		return szDest;
	}
	Cut(szDest,nSlashPos,_tcslen(szDest) - nSlashPos);
	return szDest;
}

TCHAR* em::EmTsz::FileExtFromPath( TCHAR* szDest, const TCHAR* szSrc )
{
	if(szDest != NULL){
		szDest[0] = 0;
	}

	if(szSrc == NULL || szDest == NULL){
		return NULL;
	}
	_tcscpy(szDest,szSrc);
	int nSlashPos = FindReverse(szDest,_T("."));
	if(nSlashPos == -1)
	{
		szDest[0] = 0;
		return szDest;
	}
	Cut(szDest,0,nSlashPos);
	return szDest;
}

TCHAR* em::EmTsz::FileFixDir( TCHAR* szDest, const TCHAR* szSrc )
{
	if(szDest != NULL){
		szDest[0] = 0;
	}
	if(szSrc == NULL || szDest == NULL){
		return NULL;
	}

	_tcscpy(szDest,szSrc);

	Replace(szDest,_T("\\"),_T("/"));
	if(szDest[_tcslen(szDest) - 1] != _T('/')){
		_tcscat(szDest,_T("/"));
	}

	return szDest;
}

TCHAR* em::EmTsz::HexIncrease( TCHAR *szSrc )
{
	if(!IsHexNumber(szSrc))
	{
		return NULL;
	}

	Upper(szSrc);
	int iSrcLen = _tcslen(szSrc);
	bool bAddPrefix = false;
	TCHAR* szSrcSave = szSrc;
	if(Find(szSrc,_T("0X")) == 0)
	{
		szSrc = szSrc + 2;
		iSrcLen -= 2;
		bAddPrefix = true;
	}
	
	TCHAR *SZ_HEX_CHARS = _T("0123456789ABCDEF");
	int i = 0;
	int iIndex = 0;
	bool bOverflow = false;
	for(i = iSrcLen - 1; i >= 0; i--)
	{
		iIndex = FindChar(SZ_HEX_CHARS,szSrc[i],0);
		if(iIndex == 15)
		{
			szSrc[i] = _T('0');
		}
		else
		{
			szSrc[i] = SZ_HEX_CHARS[iIndex + 1];
			break;
		}
		if(i == 0)
		{
			bOverflow = true;
		}
	}
	
	if(bOverflow)
	{
		memmove(szSrc + 1,szSrc,(iSrcLen + 1) * sizeof(TCHAR));
		szSrc[0] = _T('1');
	}

	if(bAddPrefix)
	{
		szSrc = szSrcSave;
	}

	return szSrc;
}

int em::EmTsz::HexStr2IntBytes( unsigned char* pByte, const TCHAR* szStr )
{
	if(szStr == NULL || pByte == NULL){
		return 0;
	}

	int iStrLen = _tcslen(szStr);
	TCHAR* szCurrent = NULL;
	TCHAR szTemp[3] = {0};
	TCHAR szDec[50] = {0};
	int iPos = 0;
	int iTempDec = 0;
	int iByteCount = 0;
	for(int i = 0; i < iStrLen; i++)
	{
		if(i == iStrLen - 1)
		{
			break;
		}
		memset(szTemp,0,3 * sizeof(TCHAR));
		memset(szDec,0,50 * sizeof(TCHAR));
		_tcsncpy(szTemp,szStr + i,2);
		_tcscpy(szDec,szTemp);
		Hex2Dec(szDec);
		iTempDec = Str2Int(szDec);
		pByte[iByteCount++] = (unsigned char)iTempDec;
		i++;
	}
	return iByteCount;
}

TCHAR* em::EmTsz::HexStrFromIntBytes( TCHAR* szHex, const unsigned char* pBytes, int iByteCount )
{
	if(szHex == NULL || pBytes == NULL){
		return NULL;
	}

	TCHAR szTemp[10] = {0};
	int iTempInt = 0;
	szHex[0] = 0;
	for(int i = 0; i < iByteCount; i++)
	{
		iTempInt = pBytes[i];
		memset(szTemp,0,10 * sizeof(TCHAR));
		Int2Str(szTemp,iTempInt);
		Dec2Hex(szTemp);
		PadLeft(szTemp,_T('0'),2);
		_tcscat(szHex,szTemp);
	}
	return szHex;
}

TCHAR* em::EmTsz::Delimit( TCHAR* szSrc, int iUnitLen, TCHAR chDeimiter )
{
	if(szSrc == NULL || _tcslen(szSrc) == 0)
	{
		return NULL;
	}
	int iSrcLen = _tcslen(szSrc);

	if(iSrcLen == 0)
	{
		return szSrc;
	}

	TCHAR *szCopy = (TCHAR*)malloc( (iSrcLen * 2 + 1) * sizeof(TCHAR) );
	memset(szCopy,0,(iSrcLen * 2 + 1) * sizeof(TCHAR));
	_tcscpy(szCopy,szSrc);
	
	int iPos = 0;
	for(int i = 0; i < iSrcLen / iUnitLen; i++)
	{
		if(i != 0)
		{
			szCopy[iPos++] = chDeimiter;
		}
		szCopy[iPos++] = szSrc[i*2];
		szCopy[iPos++] = szSrc[i*2+1];
	}
	_tcscpy(szSrc,szCopy);
	free(szCopy);
	return szSrc;
}

