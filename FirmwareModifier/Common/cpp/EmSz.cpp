#include "stdafx.h"
#include "../inc/EmSz.h"
#include "../inc/EmUtil.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

using namespace em;

char* em::EmSz::Int2Str( char* szDest, int iNum )
{
	if(szDest == NULL){
		return NULL;
	}
	sprintf(szDest,"%d",iNum);
	return szDest;
}

char* em::EmSz::Long2Str( char* szDest,INT64 iNum )
{
	if(szDest == NULL){
		return NULL;
	}
	_i64toa(iNum,szDest,10);
	return szDest;
}

int em::EmSz::Str2Int(const char* szSrc )
{
	if(szSrc == NULL || strlen(szSrc) == 0){
		return 0;
	}

	return atoi(szSrc);
}

INT64 em::EmSz::Str2Long( const char* szSrc )
{
	if(szSrc == NULL || strlen(szSrc) == 0){
		return 0;
	}

	return _atoi64(szSrc);
}

char* em::EmSz::Float2Str( char *szDest, double fNum )
{
	if(szDest == NULL){
		return NULL;
	}
	sprintf(szDest,"%f",fNum);
	return szDest;
}

int em::EmSz::Char2Int( char chSrc )
{
	if(!IsDigit(chSrc)){
		return 0;
	}
	return (int)chSrc - 0x30;
}

char* em::EmSz::Char2Str( char *szDest, char chSrc )
{
	if(szDest == NULL){
		return NULL;
	}
	szDest[0] = chSrc;
	szDest[1] = 0;
	return szDest;
}

char em::EmSz::Str2Char( const char* szSrc )
{
	if(szSrc == NULL){
		return 0;
	}
	return szSrc[0];
}

char* em::EmSz::Lower( char* szSrc )
{
	if(szSrc == NULL){
		return NULL;
	}
	char *pMove = szSrc;
	while(*pMove != 0)
	{
		*pMove = tolower(*pMove);
		pMove++;
	}
	return szSrc;
}

char* em::EmSz::Upper( char* szSrc )
{
	if(szSrc == NULL){
		return NULL;
	}
	char *pMove = szSrc;
	while(*pMove != 0)
	{
		*pMove = toupper(*pMove);
		pMove++;
	}
	return szSrc;
}

char* em::EmSz::Reverse( char* szSrc )
{
	if(szSrc == NULL){
		return NULL;
	}
	int iSrcLen = strlen(szSrc);
	int i = 0;
	char chTemp = 0;
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

unsigned int em::EmSz::CountChar( const char* szSrc, char chFind )
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

unsigned int em::EmSz::CountStr( const char* szSrc, const char* szFind )
{
	if(szSrc == NULL || szFind == NULL){
		return 0;
	}
	unsigned int iRetCount = 0;
	bool bMatch = 0;
	int iFromPos = 0;
	unsigned int iSrcLen = strlen(szSrc);
	unsigned int iFindLen = strlen(szFind);
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

int em::EmSz::FindFrom( const char *szSrc, const char *szFind, unsigned int iFromPos)
{
	if(szSrc == NULL || szFind == NULL){
		return -1;
	}
	int bMatch = 0;
	unsigned int iSrcLen = strlen(szSrc);
	unsigned int iFindLen = strlen(szFind);
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

char* em::EmSz::SubStr( char *szDest, const char *szSrc, unsigned int iStart, int iSubLen )
{
	if(szDest != NULL){
		szDest[0] = 0;
	}
	if(szSrc == NULL || szDest == NULL){
		return 0;
	}
	unsigned int iSrcLen = strlen(szSrc);
	unsigned int i = 0;
	
	if(iSubLen == -1)
	{
		iSubLen = iSrcLen - iStart;
	}
	
	if(iStart + iSubLen > iSrcLen)
	{
		return NULL;
	}
	
	memcpy(szDest, szSrc + iStart, iSubLen * sizeof(char));
	szDest[iSubLen] = 0;
	return szDest;
}

int em::EmSz::Find( const char *szSrc, const char *szFind )
{
	return FindFrom(szSrc,szFind,0);
}

char* em::EmSz::Repeat(char *szDest, const char* szSrc, int iCount )
{
	if(szDest != NULL){
		szDest[0] = 0;
	}
	if(szSrc == NULL){
		return szDest;
	}
	size_t i = 0;
	int iLen = strlen(szSrc);
	int iResultLen = iLen * iCount;
	for(i = 0; i < iCount; i++)
	{
		memcpy(szDest + (i * iLen),szSrc,iLen * sizeof(char));
	}
	szDest[iResultLen] = 0;
	return szDest;
}

char* em::EmSz::Prefix(char* szSrc, const char* szPrefix )
{
	if(szSrc == NULL){
		return NULL;
	}
	if(szPrefix == NULL){
		return szSrc;
	}
	int iSrcLen = strlen(szSrc);
	int iPreLen = strlen(szPrefix);
	int iResultLen = iSrcLen + iPreLen;
	memmove(szSrc + iPreLen,szSrc,iSrcLen * sizeof(char));
	memcpy(szSrc,szPrefix,iPreLen * sizeof(char));
	szSrc[iResultLen] = 0;
	return szSrc;
}

char* em::EmSz::PadLeft(char* szSrc, char chPad, int iTotalLen )
{
	if(szSrc == NULL){
		return NULL;
	}
	int iSrcLen = strlen(szSrc);
	int iPadLen = iTotalLen - iSrcLen;
	int i = 0;
	if(iSrcLen >= iTotalLen)
	{
		return szSrc;
	}

	memmove(szSrc + iPadLen,szSrc,iSrcLen * sizeof(char));

	for(i = 0; i < iPadLen; i++)
	{
		szSrc[i] = chPad;
	}

	return szSrc;
}

char* em::EmSz::AddSlashes( char* szSrc, char chFind )
{
	if(szSrc == NULL){
		return NULL;
	}
	int iSrcLen = strlen(szSrc);
	char *szCopy = (char*)malloc((iSrcLen * 2 + 1) * sizeof(char));
	int i = 0;
	int iPos = 0;

	if(szCopy == NULL)
	{
		return NULL;
	}
	
	for(i = 0; i < iSrcLen; i++)
	{
		if(szSrc[i] == chFind
			|| szSrc[i] == '\\')
		{
			szCopy[iPos++] = '\\';
			szCopy[iPos++] = szSrc[i];
		}
		else
		{
			szCopy[iPos++] = szSrc[i];
		}
	}
	szCopy[iPos++] = 0;
	strcpy(szSrc,szCopy);
	EM_FREE_NULL(szCopy);
	return szSrc;
}

char* em::EmSz::StripSlashes( char* szSrc )
{
	if(szSrc == NULL){
		return NULL;
	}
	int iSrcLen = strlen(szSrc);
	char *szCopy = (char*)malloc((iSrcLen + 1) * sizeof(char));
	int i = 0;
	int iPos = 0;
	
	if(szCopy == NULL)
	{
		return NULL;
	}
	
	bool bAddLast = true;
	for(i = 0; i < iSrcLen - 1; i++)
	{
		if(szSrc[i] == '\\')
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
	strcpy(szSrc,szCopy);
	EM_FREE_NULL(szCopy);
	return szSrc;
}

char* em::EmSz::Dec2Bin(char* szSrc )
{
	if(szSrc == NULL){
		return NULL;
	}
	INT64 num = _atoi64(szSrc);
	char szTemp[255] = {0};
	INT64 iTempQuotient = num;
	INT64 iTempRemainder = 0;
	int iPos = 0;
	while(iTempQuotient / 2 != 0)
	{
		iTempRemainder = iTempQuotient % 2;
		iTempQuotient = iTempQuotient / 2;
		
		if(iTempRemainder == 0)
		{
			szTemp[iPos++] = '0';
		}
		else
		{
			szTemp[iPos++] = '1';
		}
	}
	if(iTempQuotient == 1)
	{
		szTemp[iPos++] = '1';
	}
	strcpy(szSrc,Reverse(szTemp));
	return szSrc;
}

char* em::EmSz::Bin2Dec( char* szSrc )
{
	if(szSrc == NULL){
		return NULL;
	}
	int iSrcLen = strlen(szSrc);
	int i = 0;
	INT64 iSum = 0;
	int iPow = 0;
	if(iSrcLen <= 0)
	{
		return szSrc;
	}
	for(i = iSrcLen - 1; i >= 0; i--)
	{
		if(szSrc[i] != '0')
		{
			iSum += EmUtil::PowPlus(2,iPow);
		}
		iPow++;
	}
	sprintf(szSrc,"%d",iSum);
	return szSrc;
}

char* em::EmSz::Hex2Dec( char* szSrc )
{
	if(szSrc == NULL){
		return NULL;
	}
	int iSrcLen = strlen(szSrc);
	int i = 0;
	INT64 iSum = 0;
	int iPow = 0;
	int iDigit = 0;
	for(i = iSrcLen - 1; i >= 0; i--)
	{
		iDigit = em::EmUtil::HexChar2Int(szSrc[i]);
		if(szSrc[i] != '0')
		{
			iSum += ( iDigit * EmUtil::PowPlus(16,iPow) );
		}
		iPow++;
	}
	sprintf(szSrc,"%d",iSum);
	return szSrc;
}

char* em::EmSz::Dec2Hex( char* szSrc )
{
	if(szSrc == NULL){
		return NULL;
	}
	INT64 iDec = _atoi64(szSrc);
	sprintf(szSrc,"%x",iDec);
	return szSrc;
}

char* em::EmSz::Bin2Hex( char* szSrc )
{
	if(szSrc == NULL){
		return NULL;
	}
	Bin2Dec(szSrc);
	Dec2Hex(szSrc);
	return szSrc;
}

char* em::EmSz::Hex2Bin( char* szSrc )
{
	if(szSrc == NULL){
		return NULL;
	}
	Hex2Dec(szSrc);
	Dec2Bin(szSrc);
	return szSrc;
}

int em::EmSz::FindReverse( const char* szSrc, const char* szFind )
{
	if(szSrc == NULL || szFind == NULL){
		return 0;
	}
	int iFromPos = 0;
	int bMatch = 0;
	int iSrcLen = strlen(szSrc);
	int iFindLen = strlen(szFind);
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

char* em::EmSz::TrimLeft( char* szSrc )
{
	if(szSrc == NULL){
		return NULL;
	}

	int iSrcLen = strlen(szSrc);
	int iDestLen = 0;
	int iSpaceLen = 0;
	int i = 0;
	
	if(iSrcLen == 0)
	{
		return szSrc;
	}
	
	for(i = 0; i < iSrcLen; i++){
		if(EmUtil::IsSpaceChar(szSrc[i])){
			iSpaceLen++;
			continue;
		}else{
			break;
		}
	}

	if(iSpaceLen == 0){
		return szSrc;
	}

	
	iDestLen = iSrcLen - iSpaceLen;

	memmove(szSrc,szSrc + iSpaceLen,iSrcLen - iSpaceLen + 1);
	szSrc[iDestLen] = 0;
	
	return szSrc;
}

char* em::EmSz::TrimRight( char* szSrc )
{
	if(szSrc == NULL){
		return NULL;
	}
	int iSrcLen = strlen(szSrc);
	
	if(iSrcLen == 0)
	{
		return szSrc;
	}

	int i = 0;
	for(i = iSrcLen - 1; i >= 0; i--){
		char chTemp = szSrc[i];
		if( EmUtil::IsSpaceChar(chTemp) ){
			szSrc[i] = 0;
		}else{
			break;
		}
	}
	
	return szSrc;
}

char* em::EmSz::Trim( char* szSrc )
{
	if(szSrc == NULL){
		return NULL;
	}
	TrimLeft(szSrc);
	TrimRight(szSrc);
	return szSrc;
}

char* em::EmSz::FormatNum(char* szSrc, unsigned int nLenAfterPoint )
{
	if(szSrc == NULL){
		return NULL;
	}
	int iSrcLen = strlen(szSrc);
	char szDest[255] = {0};
	int i = 0;
	int iDotPos = 0;
	char szPlusZeros[255] = {0};
	char szIntPart[255] = {0};
	char szDecPart[255] = {0};
	size_t iSrcDecLen = 0;
	char szNewInt[255] = {0};
	int iNewInt = 0;
	char chNext = '0';
	int iNextNum = 0;
	char szNewDec[255] = {0};
	
	if(iSrcLen == 0)
	{
		return szSrc;
	}
	
	iDotPos = Find(szSrc,".");
	
	if(iDotPos == -1)
	{
		Repeat(szPlusZeros,"0",nLenAfterPoint);
		strcpy(szDest,szSrc);
		strcat(szDest,".");
		strcat(szDest,szPlusZeros);
	}
	else
	{
		SubStr(szIntPart,szSrc,0,iDotPos);
		SubStr(szDecPart,szSrc,iDotPos + 1,iSrcLen - 1 - iDotPos);
		iSrcDecLen = strlen(szDecPart);
		if(iSrcDecLen == nLenAfterPoint)
		{
			strcpy(szDest,szSrc);
		}
		else if(iSrcDecLen < nLenAfterPoint)
		{
			Repeat(szPlusZeros,"0",nLenAfterPoint - iSrcDecLen);
			strcpy(szDest,szSrc);
			strcat(szDest,szPlusZeros);
		}
		else
		{
			strcpy(szNewInt,szIntPart);
			strncat(szNewInt,szDecPart,nLenAfterPoint);
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
				strcpy(szDest,szNewInt);
			}
			else
			{
				if(strlen(szNewInt) <= nLenAfterPoint)
				{
					Repeat(szPlusZeros,"0",nLenAfterPoint - strlen(szNewInt) + 1);
					Prefix(szPlusZeros,szNewInt);
				}
				
				SubStr(szNewDec,szNewInt,strlen(szNewInt) - nLenAfterPoint,nLenAfterPoint);
				strncpy(szDest,szNewInt,strlen(szNewInt) - nLenAfterPoint);
				strcat(szDest,".");
				
				strcat(szDest,szNewDec);
			}
		}
	}
	
	strcpy(szSrc,szDest);
	return szSrc;
}

char* em::EmSz::Cut(char* szSrc, unsigned int nStart, unsigned int nLen )
{
	if(szSrc == NULL){
		return NULL;
	}
	size_t iSrcLen = strlen(szSrc);
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

char* em::EmSz::Insert( char* szSrc, const char* szInsert, unsigned int nStart )
{
	if(szSrc == NULL){
		return NULL;
	}
	if(szInsert == NULL){
		szInsert = EM_EMPTY_SZ;
	}
	size_t iSrcLen = strlen(szSrc);
	size_t iAppendLen = strlen(szInsert);
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

char* em::EmSz::Replace( char* szSrc, const char* szFind, const char* szReplace )
{
	if(szSrc == NULL){
		return NULL;
	}
	if(szFind == NULL){
		return szSrc;
	}
	if(szReplace == NULL){
		szReplace = EM_EMPTY_SZ;
	}
	int iSrcLen = strlen(szSrc);
	int iFindLen = strlen(szFind);
	int iReplaceLen = strlen(szReplace);
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

char* em::EmSz::Splice( char* szSrc, const char* szInsert, unsigned int nStart, unsigned int nLen )
{
	if(szSrc == NULL){
		return NULL;
	}
	if(szInsert == NULL){
		szInsert = EM_EMPTY_SZ;
	}
	size_t iSrcLen = strlen(szSrc);
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

bool em::EmSz::IsDigit( char chSrc )
{
	return isdigit(chSrc);
}

bool em::EmSz::IsLetter( char chSrc )
{
	if((chSrc >= 'a' && chSrc <= 'z' )
		||(chSrc >= 'A' && chSrc <= 'Z' )
	)
	{
		return true;
	}
	return false;
}

bool em::EmSz::IsInt( const char* szSrc )
{
	if(szSrc == NULL){
		return false;
	}
	int iSrcLen = strlen(szSrc);
	int i = 0;
	if(iSrcLen == 0)
	{
		return false;
	}
	
	if(szSrc[0] != '-' && szSrc[0] != '+' && !isdigit(szSrc[0]))
	{
		return false;
	}

	for(i = 1; i < iSrcLen; i++)
	{
		if(!isdigit(szSrc[i]))
		{
			return false;
		}
	}
	return true;
}

bool em::EmSz::IsHexNumber( const char* szSrc )
{
	if(szSrc == NULL || strlen(szSrc) == 0){
		return false;
	}
	char *szValid = "abcdefABCDEF0123456789";
	int iSrcLen = strlen(szSrc);
	int i = 0;
	if(iSrcLen == 0)
	{
		return false;
	}
	
	if( strcmp(szSrc,"0x")==0
		|| strcmp(szSrc,"0X") == 0)
	{
		return false;
	}
	
	for(i = 0; i < iSrcLen; i++)
	{
		if(strchr(szValid,szSrc[i]) == NULL)
		{
			if(i == 1 && (szSrc[i] == 'x' || szSrc[i] == 'X'))
			{
				continue;
			}
			return false;
		}
	}
	
	return true;
}

bool em::EmSz::IsNumber( const char* szSrc )
{
	if(szSrc == NULL){
		return false;
	}
	unsigned int iSrcLen = strlen(szSrc);
	unsigned int i = 0;
	char chTemp = 0;
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
			if(chTemp == '.')
			{
				iDotCount++;
				if(iDotCount > 1)
				{
					return false;
				}
			}
			if( !isdigit(chTemp) && chTemp != '.')
			{
				return false;
			}
		}
		
		if(chTemp == '.')
		{
			return false;
		}
		
		chTemp = *szSrc;
		if( !isdigit(chTemp) )
		{
			if(chTemp != '-' && chTemp != '+')
			{
				return false;
			}
		}
	}
	else if(iSrcLen == 1)
	{
		chTemp = *szSrc;
		if( !isdigit(chTemp) )
		{
			return false;
		}
	}
	return true;
}

bool em::EmSz::IsDomain( const char* szSrc )
{
	if(szSrc == NULL){
		return false;
	}
	int iSrcLen = strlen(szSrc);
	int i = 0;
	char chTemp = 0;
	int iDotPos = 0;
	
	if(iSrcLen < 4
		|| iSrcLen > 50)
	{
		return false;
	}
	
	for(i = 0; i < iSrcLen; i++)
	{
		chTemp = szSrc[i];
		if(isdigit(chTemp)
			|| isalpha(chTemp)
			|| chTemp == '-'
			|| chTemp == '.'
			)
		{
			continue;
		}
		else
		{
			return false;
		}
	}
	
	
	if(CountStr(szSrc,".") == 0)
	{
		return false;
	}
	
	iDotPos = Find(szSrc,".");
	
	if(iDotPos == -1)
	{
		return false;
	}
	
	if(szSrc[iSrcLen - 1] == '.'
		|| szSrc[0] == '.')
	{
		return false;
	}
	
	return true;
}

bool em::EmSz::IsEmail( const char* szSrc )
{
	if(szSrc == NULL){
		return false;
	}
	int iSrcLen = strlen(szSrc);
	int i = 0;
	char chTemp = 0;
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
		if(isdigit(chTemp)
			|| isalpha(chTemp)
			|| chTemp == '-'
			|| chTemp == '_'
			|| chTemp == '@'
			|| chTemp == '.'
			)
		{
			continue;
		}
		else
		{
			return false;
		}
	}
	
	if(CountStr(szSrc,"@") != 1)
	{
		return false;
	}
	
	iAtPos = Find(szSrc,"@");
	
	if(iAtPos == 0
		|| iAtPos > iSrcLen - 5)
	{
		return false;
	}
	
	if(CountStr(szSrc,".") == 0)
	{
		return false;
	}
	
	iDotPos = Find(szSrc,".");
	
	if(iDotPos == -1)
	{
		return false;
	}
	
	if(szSrc[iSrcLen - 1] == '.'
		|| szSrc[0] == '.')
	{
		return false;
	}
	
	return true;
}

bool em::EmSz::IsIp( const char* szSrc )
{
	if(szSrc == NULL){
		return false;
	}
	int iSrcLen = strlen(szSrc);
	int i = 0;
	char chTemp = 0;
	
	if(iSrcLen < 7
		|| iSrcLen > 15)
	{
		return false;
	}
	
	for(i = 0; i < iSrcLen; i++)
	{
		chTemp = szSrc[i];
		if(isdigit(chTemp)
			|| chTemp == '.'
			)
		{
			continue;
		}
		else
		{
			return false;
		}
	}
	
	
	if(CountStr(szSrc,".") != 3)
	{
		return false;
	}
	
	if(szSrc[iSrcLen - 1] == '.'
		|| szSrc[0] == '.')
	{
		return false;
	}
	
	return true;
}

bool em::EmSz::IsUrl( const char* szSrc )
{
	if(szSrc == NULL){
		return false;
	}
	int iSrcLen = strlen(szSrc);
	int i = 0;
	char chTemp = 0;
	int iDotPos = 0;
	int iProtocolPos = 0;
	char szValid[100] = "~!#$%^&()_-+={[}]|:;,./\"'";
	
	if(iSrcLen < 10
		|| iSrcLen > 255)
	{
		return false;
	}
	
	for(i = 0; i < iSrcLen; i++)
	{
		chTemp = szSrc[i];
		if(isdigit(chTemp)
			|| isalpha(chTemp)
			|| strchr(szValid,chTemp) != NULL
			)
		{
			continue;
		}
		else
		{
			return false;
		}
	}
	
	if(CountStr(szSrc,".") == 0)
	{
		return false;
	}
	
	iDotPos = Find(szSrc,".");
	
	if(iDotPos == -1)
	{
		return false;
	}
	
	if(szSrc[iSrcLen - 1] == '.'
		|| szSrc[0] == '.')
	{
		return false;
	}
	
	iProtocolPos = Find(szSrc,"://");
	
	if(iProtocolPos == -1
		|| iProtocolPos < 3
		|| iProtocolPos > 10)
	{
		return false;
	}
	
	return true;
}

bool em::EmSz::IsAsc( const char* szSrc )
{
	if(szSrc == NULL){
		return false;
	}
	unsigned int iSrcLen = strlen(szSrc);
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

bool em::EmSz::IsLimitedIn( const char* szSrc, const char* szLimited )
{
	if(szSrc == NULL){
		return false;
	}
	if(szLimited == NULL){
		return false;
	}
	int iSrcLen = strlen(szSrc);
	for(int i = 0; i < iSrcLen; i++)
	{
		char ch = szSrc[i];
		if(strchr(szLimited,ch) == NULL)
		{
			return false;
		}
	}
	return true;
}

bool em::EmSz::IsDateTime( const char* szSrc )
{
	if(szSrc == NULL){
		return false;
	}
	int iSrcLen = strlen(szSrc);

	if(iSrcLen != 19)
	{
		return false;
	}


	if(szSrc[4] != '-')
	{
		return false;
	}
	if(szSrc[7] != '-')
	{
		return false;
	}
	if(szSrc[10] != ' ')
	{
		return false;
	}
	if(szSrc[13] != ':')
	{
		return false;
	}
	if(szSrc[16] != ':')
	{
		return false;
	}

	if( !isdigit(szSrc[0])
		|| !isdigit(szSrc[18]) )
	{
		return false;
	}

	return true;
}

INT64 em::EmSz::IpTextToNum( const char* szIpText )
{
	if(szIpText == NULL){
		return 0;
	}
	if(!IsIp(szIpText))
	{
		return 0;
	}
	
	INT64 iTotal = 0;
	
	char szPart0[5] = {0};
	char szPart1[5] = {0};
	char szPart2[5] = {0};
	char szPart3[5] = {0};

	int iPointPos0 = -1;

	int iPointPos1 = FindFrom(szIpText,".",0);
	if(iPointPos1 == -1)
	{
		return 0;
	}

	int iPointPos2 = FindFrom(szIpText,".",iPointPos1 + 1);
	if(iPointPos2 == -1)
	{
		return 0;
	}

	int iPointPos3 = FindFrom(szIpText,".",iPointPos2 + 1);
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

char* em::EmSz::IpNumToText( char *szDest, INT64 iIpNum )
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

	char szPart0[5] = {0};
	char szPart1[5] = {0};
	char szPart2[5] = {0};
	char szPart3[5] = {0};

	szDest[0] = 0;

	Long2Str(szPart0,iPart0);
	Long2Str(szPart1,iPart1);
	Long2Str(szPart2,iPart2);
	Long2Str(szPart3,iPart3);

	strcat(szDest,szPart0);
	strcat(szDest,".");
	strcat(szDest,szPart1);
	strcat(szDest,".");
	strcat(szDest,szPart2);
	strcat(szDest,".");
	strcat(szDest,szPart3);

	return szDest;
}

void em::EmSz::AllocArray( char ***szArr, int iRow, int iCol )
{
	int i = 0;
	*szArr = (char**)malloc(sizeof(char*) * iRow);
	for(i = 0; i < iRow; i++)
	{
		(*szArr)[i] = (char*)malloc(sizeof(char*) * iCol);
		memset((*szArr)[i] ,0,iCol);
	}
}

void em::EmSz::FreeArray( char ***szArr, int iRow )
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

char* em::EmSz::UrlGetProtocol( char *szDest, const char* szSrc )
{
	if(szDest != NULL){
		szDest[0] = 0;
	}

	if(szSrc == NULL){
		return NULL;
	}

	int iColonPos = Find(szSrc,":/");
	int iSrcLen = strlen(szSrc);

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

char* em::EmSz::UrlGetHost( char *szDest, const char* szSrc )
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
	int iSrcLen = strlen(szSrc);
	int iPortColonPos = 0;

	if(iSrcLen < 3)
	{
		szDest[0] = 0;
		return szDest;
	}

	iFindPos = Find(szSrc,":/");
	if(iFindPos == -1)
	{
		iBegin = 0;
	}else
	{
		iBegin = iFindPos + 2;
		if(szSrc[iBegin] == '/')
		{
			iBegin++;
		}
	}

	iFindPos = FindFrom(szSrc,"/",iBegin + 1);
	if(iFindPos == -1)
	{
		iEnd = iSrcLen - 1;
	}
	else
	{
		iEnd = iFindPos - 1;
	}

	SubStr(szDest,szSrc,iBegin,iEnd - iBegin + 1);
	iPortColonPos = Find(szDest,":");
	if(iPortColonPos != -1)
	{
		szDest[iPortColonPos] = 0;
	}
	return szDest;


}

unsigned short em::EmSz::UrlGetPort( const char* szSrc )
{
	if(szSrc == NULL){
		return 0;
	}
	unsigned short iRetPort = 0;
	int iBegin = 0;
	int iEnd = 0;
	int iFindPos = 0;
	int iSrcLen = strlen(szSrc);
	int iPortColonPos = 0;
	
	char szTemp[20] = {0};

	if(iSrcLen < 3)
	{
		return 0;
	}
	
	iFindPos = Find(szSrc,":/");
	if(iFindPos == -1)
	{
		iFindPos = Find(szSrc,":");
	}
	else
	{
		iFindPos = FindFrom(szSrc,":",iFindPos + 1);
	}

	if(iFindPos == -1)
	{
		return 80;
	}else
	{
		iBegin = iFindPos + 1;
	}

	iFindPos = FindFrom(szSrc,"/",iBegin + 1);
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
		iRetPort = atoi(szTemp);
	}
	else
	{
		iRetPort = 80;
	}
	
	return iRetPort;
}

char* em::EmSz::UrlGetPath( char *szDest, const char* szSrc )
{
	if(szDest != NULL){
		szDest[0] = 0;
	}

	if(szSrc == NULL || szDest == NULL){
		return NULL;
	}


	int iSrcLen = strlen(szSrc);
	int iFindPos = 0;

	iFindPos = FindFrom(szSrc,":/",0);
	if(iFindPos != -1)
	{
		iFindPos += 3;
		if(szSrc[iFindPos] == '/')
		{
			iFindPos++;
		}
	}
	iFindPos = FindFrom(szSrc,"/",iFindPos);
	if(iFindPos == -1)
	{
		strcpy(szDest,"/");
	}
	else
	{
		SubStr(szDest,szSrc,iFindPos,-1);
	}

	iFindPos = Find(szDest,"?");
	if(iFindPos != -1)
	{
		szDest[iFindPos] = 0;
	}

	return szDest;
}

char* em::EmSz::UrlGetParam( char *szDest, const char* szSrc )
{
	if(szDest != NULL){
		szDest[0] = 0;
	}

	if(szSrc == NULL || szDest == NULL){
		return NULL;
	}
	
	int iSrcLen = strlen(szSrc);
	int iFindPos = 0;

	iFindPos = FindFrom(szSrc,"?",0);
	if(iFindPos == -1)
	{
		strcpy(szDest,"");
	}
	else
	{
		SubStr(szDest,szSrc,iFindPos + 1,-1);
	}
	
	return szDest;
}

char* em::EmSz::UrlGetPathWithParam( char *szDest, const char* szSrc )
{
	if(szDest != NULL){
		szDest[0] = 0;
	}

	if(szSrc == NULL || szDest == NULL){
		return NULL;
	}
	
	int iSrcLen = strlen(szSrc);
	int iFindPos = 0;
	
	iFindPos = FindFrom(szSrc,":/",0);
	if(iFindPos != -1)
	{
		iFindPos += 3;
		if(szSrc[iFindPos] == '/')
		{
			iFindPos++;
		}
	}
	else
	{
		iFindPos = 0;
	}

	iFindPos = FindFrom(szSrc,"/",iFindPos);
	if(iFindPos == -1)
	{
		strcpy(szDest,"/");
	}
	else
	{
		SubStr(szDest,szSrc,iFindPos,-1);
	}
	
	return szDest;
}



int em::EmSz::FindChar( const char *szSrc, char chFind, unsigned int iFromPos )
{
	if(szSrc == NULL){
		return -1;
	}
	int iRetPos = -1;
	int iSrcLen = strlen(szSrc);
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

bool em::EmSz::Equal( const char* szLeft, const char* szRight )
{
	if(szLeft == NULL && szRight == NULL)
	{
		return true;
	}
	else if( ! ( (szLeft != NULL) && (szRight != NULL) ) )
	{
		return false;
	}

	if(strcmp(szLeft,szRight) != 0)
	{
		return false;
	}
	return true;
}

bool em::EmSz::EqualNoCase( const char* szLeft, const char* szRight )
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

	iLeftLen = strlen(szLeft);
	iRightLen = strlen(szRight);
	if(iLeftLen != iRightLen)
	{
		return false;
	}

	for(i = 0; i < iLeftLen; i++)
	{
		if( toupper( szLeft[i] ) != toupper(szRight[i]))
		{
			return false;
		}
	}

	return true;
}

char* em::EmSz::PadRight( char* szSrc, char chPad, int iTotalLen )
{
	if(szSrc == NULL){
		return NULL;
	}
	int iSrcLen = strlen(szSrc);
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

int em::EmSz::FindSafe( const char* szSrc, const char* szFind, unsigned int iLimit )
{
	if(szSrc == NULL || szFind == NULL){
		return -1;
	}

	int i = 0;
	bool bCheck = false;
	int iCheckPos = 0;
	char chTemp = 0;
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

		if(bCheck && iCheckPos + 1 == strlen(szFind)){
			return i - iCheckPos;
		}
	}
	return -1;
}

char* em::EmSz::FileDirFromPath( char* szDest, const char* szSrc )
{
	if(szDest != NULL){
		szDest[0] = 0;
	}

	if(szSrc == NULL || szDest == NULL){
		return NULL;
	}

	strcpy(szDest,szSrc);
	Replace(szDest,"\\","/");
	int nSlashPos = FindReverse(szDest,"/");
	if(nSlashPos == -1)
	{
		szDest[0] = 0;
		return szDest;
	}
	Cut(szDest,nSlashPos + 1,strlen(szDest) - nSlashPos - 1);
	return szDest;
}

char* em::EmSz::FileNameFromPath( char* szDest, const char* szSrc )
{
	if(szDest != NULL){
		szDest[0] = 0;
	}

	if(szSrc == NULL || szDest == NULL){
		return NULL;
	}
	strcpy(szDest,szSrc);
	Replace(szDest,"\\","/");
	int nSlashPos = FindReverse(szDest,"/");
	if(nSlashPos == -1)
	{
		szDest[0] = 0;
		return szDest;
	}
	Cut(szDest,0,nSlashPos + 1);
	return szDest;
}

char* em::EmSz::FileTitleFromPath( char* szDest, const char* szSrc )
{
	if(szDest != NULL){
		szDest[0] = 0;
	}
	
	if(szSrc == NULL || szDest == NULL){
		return NULL;
	}
	
	FileNameFromPath(szDest,szSrc);
	int nSlashPos = FindReverse(szDest,".");
	if(nSlashPos == -1)
	{
		szDest[0] = 0;
		return szDest;
	}
	Cut(szDest,nSlashPos,strlen(szDest) - nSlashPos);
	return szDest;
}

char* em::EmSz::FileExtFromPath( char* szDest, const char* szSrc )
{
	if(szDest != NULL){
		szDest[0] = 0;
	}

	if(szSrc == NULL || szDest == NULL){
		return NULL;
	}
	strcpy(szDest,szSrc);
	int nSlashPos = FindReverse(szDest,".");
	if(nSlashPos == -1)
	{
		szDest[0] = 0;
		return szDest;
	}
	Cut(szDest,0,nSlashPos);
	return szDest;
}

char* em::EmSz::FileFixDir( char* szDest, const char* szSrc )
{
	if(szDest != NULL){
		szDest[0] = 0;
	}
	if(szSrc == NULL || szDest == NULL){
		return NULL;
	}

	strcpy(szDest,szSrc);

	Replace(szDest,"\\","/");
	if(szDest[strlen(szDest) - 1] != '/'){
		strcat(szDest,"/");
	}

	return szDest;
}

char* em::EmSz::HexIncrease( char *szSrc )
{
	if(!IsHexNumber(szSrc))
	{
		return NULL;
	}

	Upper(szSrc);
	int iSrcLen = strlen(szSrc);
	bool bAddPrefix = false;
	char* szSrcSave = szSrc;
	if(Find(szSrc,"0X") == 0)
	{
		szSrc = szSrc + 2;
		iSrcLen -= 2;
		bAddPrefix = true;
	}
	
	char *SZ_HEX_CHARS = "0123456789ABCDEF";
	int i = 0;
	int iIndex = 0;
	bool bOverflow = false;
	for(i = iSrcLen - 1; i >= 0; i--)
	{
		iIndex = FindChar(SZ_HEX_CHARS,szSrc[i],0);
		if(iIndex == 15)
		{
			szSrc[i] = '0';
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
		memmove(szSrc + 1,szSrc,(iSrcLen + 1) * sizeof(char));
		szSrc[0] = '1';
	}

	if(bAddPrefix)
	{
		szSrc = szSrcSave;
	}

	return szSrc;
}

int em::EmSz::HexStr2IntBytes( unsigned char* pByte, const char* szStr )
{
	if(szStr == NULL || pByte == NULL){
		return 0;
	}

	int iStrLen = strlen(szStr);
	char* szCurrent = NULL;
	char szTemp[3] = {0};
	char szDec[50] = {0};
	int iPos = 0;
	int iTempDec = 0;
	int iByteCount = 0;
	for(int i = 0; i < iStrLen; i++)
	{
		if(i == iStrLen - 1)
		{
			break;
		}
		memset(szTemp,0,3 * sizeof(char));
		memset(szDec,0,50 * sizeof(char));
		strncpy(szTemp,szStr + i,2);
		strcpy(szDec,szTemp);
		Hex2Dec(szDec);
		iTempDec = Str2Int(szDec);
		pByte[iByteCount++] = (unsigned char)iTempDec;
		i++;
	}
	return iByteCount;
}

char* em::EmSz::HexStrFromIntBytes( char* szHex, const unsigned char* pBytes, int iByteCount )
{
	if(szHex == NULL || pBytes == NULL){
		return NULL;
	}

	char szTemp[10] = {0};
	int iTempInt = 0;
	szHex[0] = 0;
	for(int i = 0; i < iByteCount; i++)
	{
		iTempInt = pBytes[i];
		memset(szTemp,0,10 * sizeof(char));
		Int2Str(szTemp,iTempInt);
		Dec2Hex(szTemp);
		PadLeft(szTemp,'0',2);
		strcat(szHex,szTemp);
	}
	return szHex;
}

char* em::EmSz::Delimit( char* szSrc, int iUnitLen, char chDeimiter )
{
	if(szSrc == NULL || strlen(szSrc) == 0)
	{
		return NULL;
	}
	int iSrcLen = strlen(szSrc);

	if(iSrcLen == 0)
	{
		return szSrc;
	}

	char *szCopy = (char*)malloc( (iSrcLen * 2 + 1) * sizeof(char) );
	memset(szCopy,0,(iSrcLen * 2 + 1) * sizeof(char));
	strcpy(szCopy,szSrc);
	
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
	strcpy(szSrc,szCopy);
	free(szCopy);
	return szSrc;
}

char* em::EmSz::StrToHex( char* szDest, const char* szSrc )
{
	if(szDest == NULL){
		return NULL;
	}

	szDest[0] = 0;

	if(szSrc == NULL){
		return szDest;
	}

	int iSrcLen = strlen(szSrc);
	if(iSrcLen == 0){
		return szDest;
	}

	int i = 0;
	const unsigned char* szUnsigned = (const unsigned char*)szSrc; 
	char szTemp[5] = {0};
	for(i = 0; i < iSrcLen; i++){
		unsigned char chUnsignedTemp = szUnsigned[i];
		Int2Str(szTemp,chUnsignedTemp);
		Dec2Hex(szTemp);
		strcat(szDest,szTemp);
	}

	return szDest;
}
