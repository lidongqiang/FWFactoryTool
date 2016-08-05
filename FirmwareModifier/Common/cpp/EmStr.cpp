#include "stdafx.h"
#include "../inc/EmStr.h"
#include "../inc/EmUtil.h"
#include "../inc/EmSz.h"
#include "../inc/EmCharSet.h"
#include "../inc/EmMd5.h"
#include "../inc/EmBase64.h"
#include "../inc/EmUrl.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>


#include "../inc/EmHandy.h"

using namespace em;

void em::EmStr::Init()
{
	m_iLength = 0;
	m_iCapacity = 0;
	m_bufData = NULL;
}

em::EmStr::EmStr()
{
	Init();
}

em::EmStr::EmStr( const char* szSrc)
{
	Init();
	Copy(szSrc);
}

em::EmStr::EmStr( const EmStr& xCopy)
{
	Init();
	Copy(xCopy.Sz());
}

em::EmStr::EmStr(const std::string& xString){
	Init();
	Copy(xString.c_str());
}

em::EmStr::~EmStr()
{
	EM_FREE_NULL(m_bufData);
}

const char* em::EmStr::Sz() const
{
	if(m_bufData == NULL){
		return EM_EMPTY_SZ;
	}

	return m_bufData;
}

int em::EmStr::Capacity() const
{
	return m_iCapacity;
}

char em::EmStr::At( int iIndex ) const
{
	if(m_iLength <= iIndex){
		return 0;
	}else{
		return m_bufData[iIndex];
	}
}

EmStr& em::EmStr::Resize( int iSize )
{
	if(iSize == 0){
		Empty();
	}else{
		if(m_bufData == NULL){
			m_iCapacity = iSize;
			m_bufData = (char*)malloc((m_iCapacity + 1) * sizeof(char));
			m_bufData[0] = 0;
		}else{
			if(iSize > m_iCapacity){
				m_iCapacity = iSize;
				m_bufData = (char*)realloc(m_bufData, (m_iCapacity + 1) * sizeof(char)) ;
			}
		}
	}

	return *this;
}

int em::EmStr::Length() const
{
	return m_iLength;
}

EmStr& em::EmStr::Append( const char* szSrc)
{
	if(szSrc == NULL || strlen(szSrc) == 0){
		return *this;
	}

	int iSrcLen = strlen(szSrc);
	if(m_bufData == NULL){
		m_iCapacity = (m_iLength + iSrcLen + 1) * 2;
		m_bufData = (char*)malloc((m_iCapacity + 1) * sizeof(char));
		m_bufData[0] = 0;
	}
	else if(m_iCapacity <= iSrcLen + m_iLength){
		m_iCapacity = (m_iLength + iSrcLen + 1) * 2;
		m_bufData = (char*)realloc(m_bufData,(m_iCapacity + 1) * sizeof(char));
	}
	strcat(m_bufData,szSrc);
	m_iLength += iSrcLen;

	return *this;
}

EmStr& em::EmStr::Append( const char* bufSrc, int iSize )
{
	if(bufSrc == NULL || iSize == 0){
		return *this;
	}
	
	int iSrcLen = iSize;
	if(m_bufData == NULL){
		m_iCapacity = (m_iLength + iSrcLen + 1) * 2;
		m_bufData = (char*)malloc((m_iCapacity + 1) * sizeof(char));
		m_bufData[0] = 0;
	}
	else if(m_iCapacity <= iSrcLen + m_iLength){
		m_iCapacity = (m_iLength + iSrcLen + 1) * 2;
		m_bufData = (char*)realloc(m_bufData,(m_iCapacity + 1) * sizeof(char));
	}
	memcpy(m_bufData + m_iLength, bufSrc, iSrcLen);
	m_iLength += iSrcLen;
	m_bufData[m_iLength] = 0;
	
	return *this;
}





void em::EmStr::Empty()
{
	if(m_bufData == NULL){
		return;
	}
	EM_FREE_NULL(m_bufData);
	m_iLength = 0;
	m_iCapacity = 0;
}

bool em::EmStr::Equals( const char* szSrc ) const
{
	if(szSrc == NULL){
		if(m_iLength == 0){
			return true;
		}else{
			return false;
		}
	}else{
		if(m_iLength != strlen(szSrc)){
			return false;
		}else{
			if(strcmp(m_bufData,szSrc) == 0){
				return true;
			}else{
				return false;
			}
		}
	}
}

int em::EmStr::Compare( const char* szSrc ) const
{
	if(szSrc == NULL){
		if(m_iLength == 0){
			return 0;
		}else{
			return 1;
		}
	}else{
		return strcmp(m_bufData,szSrc);
	}
}

EmStr& em::EmStr::Prepend( const char* szSrc )
{
	if(szSrc == NULL || strlen(szSrc) == 0){
		return *this;
	}
	
	int iSrcLen = strlen(szSrc);
	if(m_iCapacity <= iSrcLen + m_iLength){
		m_iCapacity = (m_iLength + iSrcLen + 1) * 2;
		m_bufData = (char*)realloc(m_bufData,(m_iCapacity + 1) * sizeof(char));
	}

	memmove(m_bufData + iSrcLen, m_bufData,m_iLength + 1);
	memcpy(m_bufData,szSrc,iSrcLen);

	m_iLength += iSrcLen;
	return *this;
}

EmStr& em::EmStr::Copy( const char* szSrc )
{
	if(szSrc == NULL || strlen(szSrc) == 0){
		Empty();
	}else {
		int iSrcLen = strlen(szSrc);
		if(m_bufData == NULL){
			m_iCapacity = iSrcLen * 2;
			m_bufData = (char*)malloc((m_iCapacity + 1 ) * sizeof(char) );
		}else if(m_iCapacity <= iSrcLen){
			m_iCapacity = iSrcLen * 2;
			m_bufData = (char*)realloc(m_bufData, (m_iCapacity + 1) * sizeof(char));
		}
		
		strcpy(m_bufData,szSrc);
		m_iLength = iSrcLen;
		
	}
	return *this;
}

em::EmStr::operator const char* () const
{
	return m_bufData;
}


char em::EmStr::operator[]( int iIndex ) const
{
	return At(iIndex);
}

EmStr& em::EmStr::operator += ( const char* szSrc )
{
	return Append(szSrc);
}

EmStr& em::EmStr::operator = ( const char* szSrc )
{
	Copy(szSrc);
	return *this;
}

EmStr& em::EmStr::operator = ( const EmStr& xOther )
{
	Copy(xOther.Sz());
	return *this;
}

EmStr& em::EmStr::operator = ( const std::string& xString )
{
	Copy(xString.c_str());
	return *this;
}

em::EmStr em::EmStr::Int2Str( int iNum )
{
	char szCopy[50] = {0};
	EmSz::Int2Str(szCopy,iNum);
	return szCopy;
}

em::EmStr em::EmStr::Long2Str( INT64 iNum )
{
	char szCopy[100] = {0};
	EmSz::Long2Str(szCopy,iNum);
	return szCopy;
}

int em::EmStr::Str2Int( const char* szSrc )
{
	return EmSz::Str2Int(szSrc);
}

INT64 em::EmStr::Str2Long( const char* szSrc )
{
	return EmSz::Str2Long(szSrc);
}

em::EmStr em::EmStr::Float2Str( double fNum )
{
	char szCopy[100] = {0};
	EmSz::Float2Str(szCopy,fNum);
	return szCopy;
}

int em::EmStr::Char2Int( char chSrc )
{
	return EmSz::Char2Int(chSrc);
}

em::EmStr em::EmStr::Char2Str( char chSrc )
{
	char szCopy[2] = {0};
	EmSz::Char2Str(szCopy,chSrc);
	return szCopy;
}

char em::EmStr::Str2Char( const char* szSrc )
{
	return EmSz::Str2Char(szSrc);
}

em::EmStr em::EmStr::Lower( const char* szSrc )
{
	if(szSrc == NULL || strlen(szSrc) == 0){
		return EM_EMPTY_SZ;
	}
	EmStr xStr;
	char *szCopy = (char*)malloc((strlen(szSrc) + 1) * sizeof(char));
	strcpy(szCopy,szSrc);
	EmSz::Lower(szCopy);
	xStr = szCopy;
	free(szCopy);
	return xStr;
}

em::EmStr em::EmStr::Upper( const char* szSrc )
{
	if(szSrc == NULL || strlen(szSrc) == 0){
		return EM_EMPTY_SZ;
	}

	EmStr xStr;
	char *szCopy = (char*)malloc((strlen(szSrc) + 1) * sizeof(char));
	strcpy(szCopy,szSrc);
	EmSz::Upper(szCopy);
	xStr = szCopy;
	free(szCopy);
	return xStr;
}

em::EmStr em::EmStr::Reverse( const char* szSrc )
{
	if(szSrc == NULL){
		return EM_EMPTY_SZ;
	}
	if(strlen(szSrc) <= 1){
		return szSrc;
	}

	EmStr xStr;
	char *szCopy = (char*)malloc((strlen(szSrc) + 1)* sizeof(char));
	strcpy(szCopy,szSrc);
	EmSz::Reverse(szCopy);
	xStr = szCopy;
	free(szCopy);
	return xStr;
}

bool em::EmStr::Equal( const char* szLeft, const char* szRight )
{
	return EmSz::Equal(szLeft,szRight);
}

bool em::EmStr::EqualNoCase( const char* szLeft, const char* szRight )
{
	return EmSz::EqualNoCase(szLeft,szRight);
}

unsigned int em::EmStr::CountChar( const char* szSrc, char chFind )
{
	return EmSz::CountChar(szSrc,chFind);
}

unsigned int em::EmStr::CountStr( const char* szSrc, const char* szFind )
{
	return EmSz::EqualNoCase(szSrc,szFind);
}

em::EmStr em::EmStr::Prefix( const char* szSrc, const char* szPrefix )
{
	if(szSrc == NULL){
		szSrc = EM_EMPTY_SZ;
	}
	if(szPrefix == NULL || strlen(szPrefix) == 0){
		return szSrc;
	}

	EmStr xStr;
	char *szCopy = (char*)malloc((strlen(szSrc) + strlen(szPrefix) + 1) * sizeof(char));
	strcpy(szCopy,szSrc);
	EmSz::Prefix(szCopy,szPrefix);
	xStr = szCopy;
	free(szCopy);
	return xStr;
}

em::EmStr em::EmStr::PadLeft( const char* szSrc, char chPad, int iTotalLen )
{
	if(szSrc == NULL){
		szSrc = EM_EMPTY_SZ;
	}

	if(strlen(szSrc) >= iTotalLen){
		return szSrc;
	}

	EmStr xStr;
	char *szCopy = (char*)malloc((strlen(szSrc) + iTotalLen + 1) * sizeof(char));
	strcpy(szCopy,szSrc);
	EmSz::PadLeft(szCopy,chPad,iTotalLen);
	xStr = szCopy;
	free(szCopy);
	return xStr;
}

em::EmStr em::EmStr::PadRight( const char* szSrc, char chPad, int iTotalLen )
{
	if(szSrc == NULL){
		szSrc = EM_EMPTY_SZ;
	}

	if(strlen(szSrc) >= iTotalLen){
		return szSrc;
	}
	
	EmStr xStr;
	char *szCopy = (char*)malloc((strlen(szSrc) + iTotalLen + 1) * sizeof(char));
	strcpy(szCopy,szSrc);
	EmSz::PadRight(szCopy,chPad,iTotalLen);
	xStr = szCopy;
	free(szCopy);
	return xStr;
}

em::EmStr em::EmStr::SubStr( const char *szSrc, unsigned int iStart, int iSubLen )
{
	if(szSrc == NULL){
		szSrc = EM_EMPTY_SZ;
	}

	int iSrcLen = strlen(szSrc);
	if(iSrcLen == 0){
		return EM_EMPTY_SZ;
	}

	if(iStart >= iSrcLen){
		return EM_EMPTY_SZ;
	}

	if(iSubLen + iStart > iSrcLen ){
		return EM_EMPTY_SZ;
	}

	EmStr xStr;
	char *szCopy = (char*)malloc((strlen(szSrc) + 1) * sizeof(char));
	
	EmSz::SubStr(szCopy,szSrc,iStart,iSubLen);
	xStr = szCopy;
	free(szCopy);
	return xStr;
}

em::EmStr em::EmStr::Repeat( const char* szSrc,int iCount )
{
	if(szSrc == NULL || strlen(szSrc) == 0 || iCount == 0){
		return EM_EMPTY_SZ;
	}
	if(iCount == 1){
		return szSrc;
	}

	EmStr xStr;
	char *szCopy = (char*)malloc((iCount * strlen(szSrc) + 1) * sizeof(char));
	EmSz::Repeat(szCopy,szSrc,iCount);
	xStr = szCopy;
	free(szCopy);
	return xStr;
}

em::EmStr em::EmStr::AddSlashes( const char* szSrc, char chFind )
{
	if(szSrc == NULL || strlen(szSrc) == 0){
		return EM_EMPTY_SZ;
	}

	EmStr xStr;
	char *szCopy = (char*)malloc((2 * strlen(szSrc) + 1) * sizeof(char));
	EmSz::AddSlashes(szCopy,chFind);
	xStr = szCopy;
	free(szCopy);
	return xStr;
}

em::EmStr em::EmStr::StripSlashes( const char* szSrc )
{
	if(szSrc == NULL || strlen(szSrc) == 0){
		return EM_EMPTY_SZ;
	}

	EmStr xStr;
	char *szCopy = (char*)malloc((2 * strlen(szSrc) + 1) * sizeof(char));
	EmSz::StripSlashes(szCopy);
	xStr = szCopy;
	free(szCopy);
	return xStr;
}

em::EmStr em::EmStr::Dec2Bin( const char* szSrc )
{
	if(szSrc == NULL || strlen(szSrc) == 0){
		return EM_EMPTY_SZ;
	}
	char szCopy[200] = {0};
	strcpy(szCopy,szSrc);
	EmSz::Dec2Bin(szCopy);
	return szCopy;
}

em::EmStr em::EmStr::Bin2Dec( const char* szSrc )
{
	if(szSrc == NULL || strlen(szSrc) == 0){
		return EM_EMPTY_SZ;
	}
	char szCopy[200] = {0};
	strcpy(szCopy,szSrc);
	EmSz::Bin2Dec(szCopy);
	return szCopy;
}

em::EmStr em::EmStr::Hex2Dec( const char* szSrc )
{
	if(szSrc == NULL || strlen(szSrc) == 0){
		return EM_EMPTY_SZ;
	}
	char szCopy[200] = {0};
	strcpy(szCopy,szSrc);
	EmSz::Hex2Dec(szCopy);
	return szCopy;
}

em::EmStr em::EmStr::Dec2Hex( const char* szSrc )
{
	if(szSrc == NULL || strlen(szSrc) == 0){
		return EM_EMPTY_SZ;
	}
	char szCopy[200] = {0};
	strcpy(szCopy,szSrc);
	EmSz::Dec2Hex(szCopy);
	return szCopy;
}

em::EmStr em::EmStr::Bin2Hex( const char* szSrc )
{
	if(szSrc == NULL || strlen(szSrc) == 0){
		return EM_EMPTY_SZ;
	}
	char szCopy[200] = {0};
	strcpy(szCopy,szSrc);
	EmSz::Bin2Hex(szCopy);
	return szCopy;
}

em::EmStr em::EmStr::Hex2Bin( const char* szSrc )
{
	if(szSrc == NULL || strlen(szSrc) == 0){
		return EM_EMPTY_SZ;
	}
	char szCopy[200] = {0};
	strcpy(szCopy,szSrc);
	EmSz::Hex2Bin(szCopy);
	return szCopy;
}

int em::EmStr::FindChar( const char *szSrc, char chFind, unsigned int iFromPos )
{
	return EmSz::FindChar(szSrc,chFind,iFromPos);
}

int em::EmStr::FindFrom( const char *szSrc, const char *szFind, unsigned int iFromPos )
{
	return EmSz::FindFrom(szSrc,szFind,iFromPos);
}

int em::EmStr::Find( const char *szSrc, const char *szFind )
{
	return EmSz::Find(szSrc,szFind);
}

int em::EmStr::FindReverse( const char* szSrc, const char* szFind )
{
	return EmSz::FindReverse(szSrc,szFind);
}

int em::EmStr::FindSafe( const char* szSrc, const char* szFind, unsigned int iLimit )
{
	return EmSz::FindSafe(szSrc,szFind,iLimit);
}

em::EmStr em::EmStr::TrimLeft( const char* szSrc )
{
	if(szSrc == NULL || strlen(szSrc) == 0){
		return EM_EMPTY_SZ;
	}
	EmStr xStr;
	char *szCopy = (char*)malloc((strlen(szSrc) + 1) * sizeof(char));
	strcpy(szCopy,szSrc);
	EmSz::TrimLeft(szCopy);
	xStr = szCopy;
	free(szCopy);
	return xStr;
}

em::EmStr em::EmStr::TrimRight( const char* szSrc )
{
	if(szSrc == NULL || strlen(szSrc) == 0){
		return EM_EMPTY_SZ;
	}
	EmStr xStr;
	char *szCopy = (char*)malloc((strlen(szSrc) + 1) * sizeof(char));
	strcpy(szCopy,szSrc);
	EmSz::TrimRight(szCopy);
	xStr = szCopy;
	free(szCopy);
	return xStr;
}

em::EmStr em::EmStr::Trim( const char* szSrc )
{
	if(szSrc == NULL || strlen(szSrc) == 0){
		return EM_EMPTY_SZ;
	}
	EmStr xStr;
	char *szCopy = (char*)malloc((strlen(szSrc) + 1) * sizeof(char));
	strcpy(szCopy,szSrc);
	EmSz::Trim(szCopy);
	xStr = szCopy;
	free(szCopy);
	return xStr;
}

em::EmStr em::EmStr::FormatNum( const char* szSrc, unsigned int nLenAfterPoint )
{
	EmStr xStr;
	char szCopy[500] = {0};
	strcpy(szCopy,szSrc);
	EmSz::FormatNum(szCopy,nLenAfterPoint);
	xStr = szCopy;
	return xStr;
}

em::EmStr em::EmStr::Replace( const char* szSrc, const char* szFind, const char* szReplace )
{
	if(szSrc == NULL || strlen(szSrc) == 0){
		return EM_EMPTY_SZ;
	}

	if(szFind == NULL || strlen(szFind) == 0){
		return szSrc;
	}

	if(szReplace == NULL){
		szReplace = EM_EMPTY_SZ;
	}

	EmStr xStr;
	int iSrcLen = strlen(szSrc);
	int iFindLen = strlen(szFind);
	int iReplaceLen = strlen(szReplace);
	int iDiffLen = EmUtil::Abs(iFindLen - iReplaceLen) + iSrcLen + 1;

	char *szCopy = (char*)malloc(iSrcLen * iDiffLen + 1);
	strcpy(szCopy,szSrc);

	EmSz::Replace(szCopy,szFind,szReplace);


	xStr = szCopy;
	free(szCopy);
	return xStr;
}

em::EmStr em::EmStr::Cut( const char* szSrc, unsigned int nStart, unsigned int nLen )
{
	if(szSrc == NULL || strlen(szSrc) == 0){
		return EM_EMPTY_SZ;
	}

	int iSrcLen = strlen(szSrc);
	if(nStart >= iSrcLen){
		return EM_EMPTY_SZ;
	}
	if(nStart + nLen > iSrcLen){
		return EM_EMPTY_SZ;
	}


	EmStr xStr;
	char *szCopy = (char*)malloc(( strlen(szSrc) + 1) * sizeof(char) );
	strcpy(szCopy,szSrc);
	EmSz::Cut(szCopy,nStart,nLen);
	xStr = szCopy;
	free(szCopy);
	return xStr;
}

em::EmStr em::EmStr::Insert( const char* szSrc, const char* szInsert, unsigned int nStart )
{
	if(szSrc == NULL){
		szSrc = EM_EMPTY_SZ;
	}
	
	if(nStart >= strlen(szSrc)){
		return EM_EMPTY_SZ;
	}

	if(szInsert == NULL || strlen(szInsert) == 0){
		return szSrc;
	}

	EmStr xStr;
	char *szCopy = (char*)malloc(( strlen(szSrc) + strlen(szInsert) + 1) * sizeof(char));
	strcpy(szCopy,szSrc);
	EmSz::Insert(szCopy,szInsert,nStart);
	xStr = szCopy;
	free(szCopy);
	return xStr;
}

em::EmStr em::EmStr::Splice( const char* szSrc, const char* szInsert, unsigned int nStart, unsigned int nLen )
{
	if(szSrc == NULL){
		szSrc = EM_EMPTY_SZ;
	}
	
	if(nStart >= strlen(szSrc)){
		return EM_EMPTY_SZ;
	}
	
	if(szInsert == NULL || strlen(szInsert) == 0){
		szInsert = EM_EMPTY_SZ;
	}

	if(nStart + nLen > strlen(szSrc)){
		return EM_EMPTY_SZ;
	}

	EmStr xStr;
	char *szCopy = (char*)malloc(( strlen(szSrc) + strlen(szInsert) + 1) * sizeof(char));
	strcpy(szCopy,szSrc);
	EmSz::Splice(szCopy,szInsert,nStart,nLen);
	xStr = szCopy;
	free(szCopy);
	return xStr;
}

bool em::EmStr::IsDigit( char ch )
{
	return EmSz::IsDigit(ch);
}

bool em::EmStr::IsLetter( char ch )
{
	return EmSz::IsLetter(ch);
}

bool em::EmStr::IsInt( const char* szSrc )
{
	return EmSz::IsInt(szSrc);
}

bool em::EmStr::IsNumber( const char* szSrc )
{
	return EmSz::IsNumber(szSrc);
}

bool em::EmStr::IsDomain( const char* szSrc )
{
	return EmSz::IsDomain(szSrc);
}

bool em::EmStr::IsEmail( const char* szSrc )
{
	return EmSz::IsEmail(szSrc);
}

bool em::EmStr::IsIp( const char* szSrc )
{
	return EmSz::IsIp(szSrc);
}

bool em::EmStr::IsUrl( const char* szSrc )
{
	return EmSz::IsUrl(szSrc);
}

bool em::EmStr::IsAsc( const char* szSrc )
{
	return EmSz::IsAsc(szSrc);
}

bool em::EmStr::IsLimitedIn( const char* szSrc, const char* szLimited )
{
	return EmSz::IsLimitedIn(szSrc,szLimited);
}

bool em::EmStr::IsDateTime( const char* szSrc )
{
	return EmSz::IsDateTime(szSrc);
}

INT64 em::EmStr::IpTextToNum( const char* szIpText )
{
	return EmSz::IpTextToNum(szIpText);
}

em::EmStr em::EmStr::IpNumToText( INT64 iIpNum )
{
	char szCopy[100] = {0};
	EmSz::IpNumToText(szCopy,iIpNum);
	return szCopy;
}

em::EmStr em::EmStr::UrlGetProtocol( const char* szSrc )
{
	char szCopy[1024] = {0};
	EmSz::UrlGetProtocol(szCopy,szSrc);
	return szCopy;
}

em::EmStr em::EmStr::UrlGetHost( const char* szSrc )
{
	char szCopy[1024] = {0};
	EmSz::UrlGetHost(szCopy,szSrc);
	return szCopy;
}

unsigned short em::EmStr::UrlGetPort( const char* szSrc )
{
	return EmSz::UrlGetPort(szSrc);
}

em::EmStr em::EmStr::UrlGetPath( const char* szSrc )
{
	char szCopy[1024] = {0};
	EmSz::UrlGetPath(szCopy,szSrc);
	return szCopy;
}

em::EmStr em::EmStr::UrlGetParam( const char* szSrc )
{
	char szCopy[1024] = {0};
	EmSz::UrlGetParam(szCopy,szSrc);
	return szCopy;
}

em::EmStr em::EmStr::UrlGetPathWithParam( const char* szSrc )
{
	char szCopy[1024] = {0};
	EmSz::UrlGetPathWithParam(szCopy,szSrc);
	return szCopy;
}

em::EmStr em::EmStr::FileDirFromPath( const char* szSrc )
{
	char szCopy[1024] = {0};
	EmSz::FileDirFromPath(szCopy,szSrc);
	return szCopy;
}

em::EmStr em::EmStr::FileNameFromPath( const char* szSrc )
{
	char szCopy[1024] = {0};
	EmSz::FileNameFromPath(szCopy,szSrc);
	return szCopy;
}

em::EmStr em::EmStr::FileTitleFromPath( const char* szSrc )
{
	char szCopy[1024] = {0};
	EmSz::FileTitleFromPath(szCopy,szSrc);
	return szCopy;
}

em::EmStr em::EmStr::FileExtFromPath( const char* szSrc )
{
	char szCopy[1024] = {0};
	EmSz::FileExtFromPath(szCopy,szSrc);
	return szCopy;
}

em::EmStr em::EmStr::FileFixDir( const char* szSrc )
{
	char szCopy[1024] = {0};
	EmSz::FileFixDir(szCopy,szSrc);
	return szCopy;
}

em::EmStr em::EmStr::AnsiToUtf8( const char* szSrc )
{
	if(szSrc == NULL){
		return EM_EMPTY_SZ;
	}
	int iSrcLen = strlen(szSrc);
	if(iSrcLen == 0){
		return EM_EMPTY_SZ;
	}

	EmStr strResult;
	char* szResult = (char*)malloc(iSrcLen * 3 + 1);
	szResult[0] = 0;
	EmCharSet::AnsiToUtf8(szResult,szSrc);
	strResult = szResult;
	free(szResult);
	return strResult;

}

em::EmStr em::EmStr::Utf8ToAnsi( const char* szSrc )
{
	if(szSrc == NULL){
		return EM_EMPTY_SZ;
	}
	int iSrcLen = strlen(szSrc);
	if(iSrcLen == 0){
		return EM_EMPTY_SZ;
	}

	EmStr strResult;
	char* szResult = (char*)malloc(iSrcLen * 2 + 1);
	szResult[0] = 0;
	EmCharSet::Utf8ToAnsi(szResult,szSrc);
	strResult = szResult;
	free(szResult);
	return strResult;
}

EmStr em::EmStr::Md5( const char* szSrc )
{
	if(szSrc == NULL){
		szSrc = EM_EMPTY_SZ;
	}
	
	EmStr strResult;
	char szCopy[33] = {0};
	
	EmMd5::Encode(szCopy,szSrc);
	
	strResult = szCopy;
	return strResult;
}

EmStr em::EmStr::UrlEncode( const char* szSrc, bool bEncodeAll )
{
	if(szSrc == NULL || strlen(szSrc) == 0){
		return EM_EMPTY_SZ;
	}
	
	EmStr strResult;
	char *szCopy = (char*)malloc(strlen(szSrc) * 6 + 6);
	szCopy[0] = 0;
	
	EmUrl::Encode(szCopy,szSrc,bEncodeAll);
	
	strResult = szCopy;
	free(szCopy);
	return strResult;
	
}

EmStr em::EmStr::UrlDecode( const char* szSrc, bool bEncodeAll )
{
	if(szSrc == NULL || strlen(szSrc) == 0){
		return EM_EMPTY_SZ;
	}
	
	EmStr strResult;
	char *szCopy = (char*)malloc(strlen(szSrc) * 6 + 6);
	szCopy[0] = 0;
	
	EmUrl::Decode(szCopy,szSrc,bEncodeAll);
	
	strResult = szCopy;
	free(szCopy);
	return strResult;
}

EmStr em::EmStr::Base64Encode( const char* szSrc )
{
	if(szSrc == NULL){
		szSrc = EM_EMPTY_SZ;
	}
	
	EmStr strResult;
	char *szCopy = (char*)malloc(strlen(szSrc) * 4 + 4);
	szCopy[0] = 0;
	
	EmBase64::Encode(szCopy,szSrc);
	
	strResult = szCopy;
	free(szCopy);
	return strResult;
}

EmStr em::EmStr::Base64Decode( const char* szSrc )
{
	if(szSrc == NULL){
		szSrc = EM_EMPTY_SZ;
	}
	
	EmStr strResult;
	char *szCopy = (char*)malloc(strlen(szSrc) * 4 + 4);
	szCopy[0] = 0;
	
	EmBase64::Decode(szCopy,szSrc);
	
	strResult = szCopy;
	free(szCopy);
	return strResult;
}

int em::EmStr::ListInBuffer( std::vector<EmStr>& rList, const char* szBuffer, int iBufSize )
{
	rList.clear();
	if(iBufSize == 0){
		return 0;
	}
	int i = 0;
	int iTempSize = 0;
	char *szTemp = (char*)malloc((iBufSize + 1) * sizeof(char));
	szTemp[0] = 0;
	
	for(i = 0; i < iBufSize; i++){
		if(szBuffer[i] == 0){
			szTemp[iTempSize++] = 0;
			rList.push_back(szTemp);
			iTempSize = 0;
		}else{
			szTemp[iTempSize++] = szBuffer[i];
		}
	}
	
	if(iTempSize > 0){
		rList.push_back(szTemp);
	}
	
	free(szTemp);
	
	return rList.size();

}

em::EmStr em::EmStr::Str2Hex( const char* szSrc )
{
	if(szSrc == NULL || strlen(szSrc) == 0){
		return EM_EMPTY_SZ;
	}

	EmStr strResult;
	char *szCopy = (char*)malloc(strlen(szSrc) * 2 + 1);
	szCopy[0] = 0;

	EmSz::StrToHex(szCopy,szSrc);

	strResult = szCopy;
	free(szCopy);
	return strResult;
}

em::EmStr em::EmStr::Delimit( const char* szSrc, int iUnitLen, char chDelimiter )
{
	if(szSrc == NULL)
	{
		return EM_EMPTY_SZ;
	}

	int iSrcLen = strlen(szSrc);

	if(iSrcLen == 0){
		return EM_EMPTY_SZ;
	}

	char *szCopy = (char*)malloc((iSrcLen * 2 + 1) * sizeof(char) );
	memset(szCopy,0,(iSrcLen * 2 + 1) * sizeof(char));
	strcpy(szCopy,szSrc);
	EmSz::Delimit(szCopy,iUnitLen,chDelimiter);
	EmStr strResult = szCopy;
	EM_FREE_NULL(szCopy);
	return strResult;
}








