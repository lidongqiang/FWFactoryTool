#include "stdafx.h"
#include "../inc/EmWstr.h"
#include "../inc/EmUtil.h"
#include "../inc/EmWsz.h"

#if VC_VERSION <= 0X0006
#include <wchar.h>
#endif

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>


using namespace em;

void em::EmWstr::Init()
{
	m_iLength = 0;
	m_iCapacity = 0;
	m_bufData = NULL;
}

em::EmWstr::EmWstr()
{
	Init();
}

em::EmWstr::EmWstr( const wchar_t* szSrc)
{
	Init();
	Copy(szSrc);
}

em::EmWstr::EmWstr( const EmWstr& xCopy)
{
	Init();
	Copy(xCopy.Sz());
}

em::EmWstr::~EmWstr()
{
	EM_FREE_NULL(m_bufData);
}

const wchar_t* em::EmWstr::Sz() const
{
	if(m_bufData == NULL){
		return EM_EMPTY_WSZ;
	}

	return m_bufData;
}

int em::EmWstr::Capacity() const
{
	return m_iCapacity;
}

wchar_t em::EmWstr::At( int iIndex ) const
{
	if(m_iLength <= iIndex){
		return 0;
	}else{
		return m_bufData[iIndex];
	}
}

EmWstr& em::EmWstr::Resize( int iSize )
{
	if(iSize == 0){
		Empty();
	}else{
		if(m_bufData == NULL){
			m_iCapacity = iSize;
			m_bufData = (wchar_t*)malloc((m_iCapacity + 1) * sizeof(wchar_t));
			m_bufData[0] = 0;
		}else{
			if(iSize > m_iCapacity){
				m_iCapacity = iSize;
				m_bufData = (wchar_t*)realloc(m_bufData, (m_iCapacity + 1) * sizeof(wchar_t)) ;
			}
		}
	}

	return *this;
}

int em::EmWstr::Length() const
{
	return m_iLength;
}

EmWstr& em::EmWstr::Append( const wchar_t* szSrc)
{
	if(szSrc == NULL || wcslen(szSrc) == 0){
		return *this;
	}

	int iSrcLen = wcslen(szSrc);
	if(m_bufData == NULL){
		m_iCapacity = (m_iLength + iSrcLen + 1) * 2;
		m_bufData = (wchar_t*)malloc((m_iCapacity + 1) * sizeof(wchar_t));
		m_bufData[0] = 0;
	}
	else if(m_iCapacity <= iSrcLen + m_iLength){
		m_iCapacity = (m_iLength + iSrcLen + 1) * 2;
		m_bufData = (wchar_t*)realloc(m_bufData,(m_iCapacity + 1) * sizeof(wchar_t));
	}
	wcscat(m_bufData,szSrc);
	m_iLength += iSrcLen;

	return *this;
}

EmWstr& em::EmWstr::Append( const wchar_t* bufSrc, int iSize )
{
	if(bufSrc == NULL || iSize == 0){
		return *this;
	}
	
	int iSrcLen = iSize;
	if(m_bufData == NULL){
		m_iCapacity = (m_iLength + iSrcLen + 1) * 2;
		m_bufData = (wchar_t*)malloc((m_iCapacity + 1) * sizeof(wchar_t));
		m_bufData[0] = 0;
	}
	else if(m_iCapacity <= iSrcLen + m_iLength){
		m_iCapacity = (m_iLength + iSrcLen + 1) * 2;
		m_bufData = (wchar_t*)realloc(m_bufData,(m_iCapacity + 1) * sizeof(wchar_t));
	}
	memcpy(m_bufData + m_iLength, bufSrc, iSrcLen);
	m_iLength += iSrcLen;
	m_bufData[m_iLength] = 0;
	
	return *this;
}





void em::EmWstr::Empty()
{
	if(m_bufData == NULL){
		return;
	}
	EM_FREE_NULL(m_bufData);
	m_iLength = 0;
	m_iCapacity = 0;
}

bool em::EmWstr::Equals( const wchar_t* szSrc ) const
{
	if(szSrc == NULL){
		if(m_iLength == 0){
			return true;
		}else{
			return false;
		}
	}else{
		if(m_iLength != wcslen(szSrc)){
			return false;
		}else{
			if(wcscmp(m_bufData,szSrc) == 0){
				return true;
			}else{
				return false;
			}
		}
	}
}

int em::EmWstr::Compare( const wchar_t* szSrc ) const
{
	if(szSrc == NULL){
		if(m_iLength == 0){
			return 0;
		}else{
			return 1;
		}
	}else{
		return wcscmp(m_bufData,szSrc);
	}
}

EmWstr& em::EmWstr::Prepend( const wchar_t* szSrc )
{
	if(szSrc == NULL || wcslen(szSrc) == 0){
		return *this;
	}
	
	int iSrcLen = wcslen(szSrc);
	if(m_iCapacity <= iSrcLen + m_iLength){
		m_iCapacity = (m_iLength + iSrcLen + 1) * 2;
		m_bufData = (wchar_t*)realloc(m_bufData,(m_iCapacity + 1) * sizeof(wchar_t));
	}

	memmove(m_bufData + iSrcLen, m_bufData,m_iLength + 1);
	memcpy(m_bufData,szSrc,iSrcLen);

	m_iLength += iSrcLen;
	return *this;
}

EmWstr& em::EmWstr::Copy( const wchar_t* szSrc )
{
	if(szSrc == NULL || wcslen(szSrc) == 0){
		Empty();
	}else {
		int iSrcLen = wcslen(szSrc);
		if(m_bufData == NULL){
			m_iCapacity = iSrcLen * 2;
			m_bufData = (wchar_t*)malloc((m_iCapacity + 1 ) * sizeof(wchar_t) );
		}else if(m_iCapacity <= iSrcLen){
			m_iCapacity = iSrcLen * 2;
			m_bufData = (wchar_t*)realloc(m_bufData, (m_iCapacity + 1) * sizeof(wchar_t));
		}
		
		wcscpy(m_bufData,szSrc);
		m_iLength = iSrcLen;
		
	}
	return *this;
}

em::EmWstr::operator const wchar_t* () const
{
	return m_bufData;
}


char em::EmWstr::operator[]( int iIndex ) const
{
	return At(iIndex);
}

EmWstr& em::EmWstr::operator += ( const wchar_t* szSrc )
{
	return Append(szSrc);
}

EmWstr& em::EmWstr::operator = ( const wchar_t* szSrc )
{
	Copy(szSrc);
	return *this;
}

em::EmWstr em::EmWstr::Int2Str( int iNum )
{
	wchar_t szCopy[50] = {0};
	EmWsz::Int2Str(szCopy,iNum);
	return szCopy;
}

em::EmWstr em::EmWstr::Long2Str( INT64 iNum )
{
	wchar_t szCopy[100] = {0};
	EmWsz::Long2Str(szCopy,iNum);
	return szCopy;
}

int em::EmWstr::Str2Int( const wchar_t* szSrc )
{
	return EmWsz::Str2Int(szSrc);
}

INT64 em::EmWstr::Str2Long( const wchar_t* szSrc )
{
	return EmWsz::Str2Long(szSrc);
}

em::EmWstr em::EmWstr::Float2Str( double fNum )
{
	wchar_t szCopy[100] = {0};
	EmWsz::Float2Str(szCopy,fNum);
	return szCopy;
}

int em::EmWstr::Char2Int( wchar_t chSrc )
{
	return EmWsz::Char2Int(chSrc);
}

em::EmWstr em::EmWstr::Char2Str( wchar_t chSrc )
{
	wchar_t szCopy[2] = {0};
	EmWsz::Char2Str(szCopy,chSrc);
	return szCopy;
}

wchar_t em::EmWstr::Str2Char( const wchar_t* szSrc )
{
	return EmWsz::Str2Char(szSrc);
}

em::EmWstr em::EmWstr::Lower( wchar_t* szSrc )
{
	if(szSrc == NULL || wcslen(szSrc) == 0){
		return EM_EMPTY_WSZ;
	}
	EmWstr xStr;
	wchar_t *szCopy = (wchar_t*)malloc((wcslen(szSrc) + 1) * sizeof(wchar_t));
	wcscpy(szCopy,szSrc);
	EmWsz::Lower(szCopy);
	xStr = szCopy;
	free(szCopy);
	return xStr;
}

em::EmWstr em::EmWstr::Upper( wchar_t* szSrc )
{
	if(szSrc == NULL || wcslen(szSrc) == 0){
		return EM_EMPTY_WSZ;
	}

	EmWstr xStr;
	wchar_t *szCopy = (wchar_t*)malloc((wcslen(szSrc) + 1) * sizeof(wchar_t));
	wcscpy(szCopy,szSrc);
	EmWsz::Upper(szCopy);
	xStr = szCopy;
	free(szCopy);
	return xStr;
}

em::EmWstr em::EmWstr::Reverse( wchar_t* szSrc )
{
	if(szSrc == NULL){
		return EM_EMPTY_WSZ;
	}
	if(wcslen(szSrc) <= 1){
		return szSrc;
	}

	EmWstr xStr;
	wchar_t *szCopy = (wchar_t*)malloc((wcslen(szSrc) + 1)* sizeof(wchar_t));
	wcscpy(szCopy,szSrc);
	EmWsz::Reverse(szCopy);
	xStr = szCopy;
	free(szCopy);
	return xStr;
}

bool em::EmWstr::Equal( const wchar_t* szLeft, const wchar_t* szRight )
{
	return EmWsz::Equal(szLeft,szRight);
}

bool em::EmWstr::EqualNoCase( const wchar_t* szLeft, const wchar_t* szRight )
{
	return EmWsz::EqualNoCase(szLeft,szRight);
}

unsigned int em::EmWstr::CountChar( const wchar_t* szSrc, wchar_t chFind )
{
	return EmWsz::CountChar(szSrc,chFind);
}

unsigned int em::EmWstr::CountStr( const wchar_t* szSrc, const wchar_t* szFind )
{
	return EmWsz::EqualNoCase(szSrc,szFind);
}

em::EmWstr em::EmWstr::Prefix( const wchar_t* szSrc, const wchar_t* szPrefix )
{
	if(szSrc == NULL){
		szSrc = EM_EMPTY_WSZ;
	}
	if(szPrefix == NULL || wcslen(szPrefix) == 0){
		return szSrc;
	}

	EmWstr xStr;
	wchar_t *szCopy = (wchar_t*)malloc((wcslen(szSrc) + wcslen(szPrefix) + 1) * sizeof(wchar_t));
	wcscpy(szCopy,szSrc);
	EmWsz::Prefix(szCopy,szPrefix);
	xStr = szCopy;
	free(szCopy);
	return xStr;
}

em::EmWstr em::EmWstr::PadLeft( const wchar_t* szSrc, wchar_t chPad, int iTotalLen )
{
	if(szSrc == NULL){
		szSrc = EM_EMPTY_WSZ;
	}

	if(wcslen(szSrc) >= iTotalLen){
		return szSrc;
	}

	EmWstr xStr;
	wchar_t *szCopy = (wchar_t*)malloc((wcslen(szSrc) + iTotalLen + 1) * sizeof(wchar_t));
	wcscpy(szCopy,szSrc);
	EmWsz::PadLeft(szCopy,chPad,iTotalLen);
	xStr = szCopy;
	free(szCopy);
	return xStr;
}

em::EmWstr em::EmWstr::PadRight( const wchar_t* szSrc, wchar_t chPad, int iTotalLen )
{
	if(szSrc == NULL){
		szSrc = EM_EMPTY_WSZ;
	}

	if(wcslen(szSrc) >= iTotalLen){
		return szSrc;
	}
	
	EmWstr xStr;
	wchar_t *szCopy = (wchar_t*)malloc((wcslen(szSrc) + iTotalLen + 1) * sizeof(wchar_t));
	wcscpy(szCopy,szSrc);
	EmWsz::PadRight(szCopy,chPad,iTotalLen);
	xStr = szCopy;
	free(szCopy);
	return xStr;
}

em::EmWstr em::EmWstr::SubStr( const wchar_t *szSrc, unsigned int iStart, int iSubLen )
{
	if(szSrc == NULL){
		szSrc = EM_EMPTY_WSZ;
	}

	int iSrcLen = wcslen(szSrc);
	if(iSrcLen == 0){
		return EM_EMPTY_WSZ;
	}

	if(iStart >= iSrcLen){
		return EM_EMPTY_WSZ;
	}

	if(iSubLen + iStart > iSrcLen ){
		return EM_EMPTY_WSZ;
	}

	EmWstr xStr;
	wchar_t *szCopy = (wchar_t*)malloc((wcslen(szSrc) + 1) * sizeof(wchar_t));
	
	EmWsz::SubStr(szCopy,szSrc,iStart,iSubLen);
	xStr = szCopy;
	free(szCopy);
	return xStr;
}

em::EmWstr em::EmWstr::Repeat( const wchar_t* szSrc,int iCount )
{
	if(szSrc == NULL || wcslen(szSrc) == 0 || iCount == 0){
		return EM_EMPTY_WSZ;
	}
	if(iCount == 1){
		return szSrc;
	}

	EmWstr xStr;
	wchar_t *szCopy = (wchar_t*)malloc((iCount * wcslen(szSrc) + 1) * sizeof(wchar_t));
	EmWsz::Repeat(szCopy,szSrc,iCount);
	xStr = szCopy;
	free(szCopy);
	return xStr;
}

em::EmWstr em::EmWstr::AddSlashes( const wchar_t* szSrc, wchar_t chFind )
{
	if(szSrc == NULL || wcslen(szSrc) == 0){
		return EM_EMPTY_WSZ;
	}

	EmWstr xStr;
	wchar_t *szCopy = (wchar_t*)malloc((2 * wcslen(szSrc) + 1) * sizeof(wchar_t));
	EmWsz::AddSlashes(szCopy,chFind);
	xStr = szCopy;
	free(szCopy);
	return xStr;
}

em::EmWstr em::EmWstr::StripSlashes( const wchar_t* szSrc )
{
	if(szSrc == NULL || wcslen(szSrc) == 0){
		return EM_EMPTY_WSZ;
	}

	EmWstr xStr;
	wchar_t *szCopy = (wchar_t*)malloc((2 * wcslen(szSrc) + 1) * sizeof(wchar_t));
	EmWsz::StripSlashes(szCopy);
	xStr = szCopy;
	free(szCopy);
	return xStr;
}

em::EmWstr em::EmWstr::Dec2Bin( const wchar_t* szSrc )
{
	if(szSrc == NULL || wcslen(szSrc) == 0){
		return EM_EMPTY_WSZ;
	}
	wchar_t szCopy[200] = {0};
	wcscpy(szCopy,szSrc);
	EmWsz::Dec2Bin(szCopy);
	return szCopy;
}

em::EmWstr em::EmWstr::Bin2Dec( const wchar_t* szSrc )
{
	if(szSrc == NULL || wcslen(szSrc) == 0){
		return EM_EMPTY_WSZ;
	}
	wchar_t szCopy[200] = {0};
	wcscpy(szCopy,szSrc);
	EmWsz::Bin2Dec(szCopy);
	return szCopy;
}

em::EmWstr em::EmWstr::Hex2Dec( const wchar_t* szSrc )
{
	if(szSrc == NULL || wcslen(szSrc) == 0){
		return EM_EMPTY_WSZ;
	}
	wchar_t szCopy[200] = {0};
	wcscpy(szCopy,szSrc);
	EmWsz::Hex2Dec(szCopy);
	return szCopy;
}

em::EmWstr em::EmWstr::Dec2Hex( const wchar_t* szSrc )
{
	if(szSrc == NULL || wcslen(szSrc) == 0){
		return EM_EMPTY_WSZ;
	}
	wchar_t szCopy[200] = {0};
	wcscpy(szCopy,szSrc);
	EmWsz::Dec2Hex(szCopy);
	return szCopy;
}

em::EmWstr em::EmWstr::Bin2Hex( const wchar_t* szSrc )
{
	if(szSrc == NULL || wcslen(szSrc) == 0){
		return EM_EMPTY_WSZ;
	}
	wchar_t szCopy[200] = {0};
	wcscpy(szCopy,szSrc);
	EmWsz::Bin2Hex(szCopy);
	return szCopy;
}

em::EmWstr em::EmWstr::Hex2Bin( const wchar_t* szSrc )
{
	if(szSrc == NULL || wcslen(szSrc) == 0){
		return EM_EMPTY_WSZ;
	}
	wchar_t szCopy[200] = {0};
	wcscpy(szCopy,szSrc);
	EmWsz::Hex2Bin(szCopy);
	return szCopy;
}

int em::EmWstr::FindChar( const wchar_t *szSrc, wchar_t chFind, unsigned int iFromPos )
{
	return EmWsz::FindChar(szSrc,chFind,iFromPos);
}

int em::EmWstr::FindFrom( const wchar_t *szSrc, const wchar_t *szFind, unsigned int iFromPos )
{
	return EmWsz::FindFrom(szSrc,szFind,iFromPos);
}

int em::EmWstr::Find( const wchar_t *szSrc, const wchar_t *szFind )
{
	return EmWsz::Find(szSrc,szFind);
}

int em::EmWstr::FindReverse( const wchar_t* szSrc, const wchar_t* szFind )
{
	return EmWsz::FindReverse(szSrc,szFind);
}

int em::EmWstr::FindSafe( const wchar_t* szSrc, const wchar_t* szFind, unsigned int iLimit )
{
	return EmWsz::FindSafe(szSrc,szFind,iLimit);
}

em::EmWstr em::EmWstr::TrimLeft( const wchar_t* szSrc )
{
	if(szSrc == NULL || wcslen(szSrc) == 0){
		return EM_EMPTY_WSZ;
	}
	EmWstr xStr;
	wchar_t *szCopy = (wchar_t*)malloc((wcslen(szSrc) + 1) * sizeof(wchar_t));
	wcscpy(szCopy,szSrc);
	EmWsz::TrimLeft(szCopy);
	xStr = szCopy;
	free(szCopy);
	return xStr;
}

em::EmWstr em::EmWstr::TrimRight( const wchar_t* szSrc )
{
	if(szSrc == NULL || wcslen(szSrc) == 0){
		return EM_EMPTY_WSZ;
	}
	EmWstr xStr;
	wchar_t *szCopy = (wchar_t*)malloc((wcslen(szSrc) + 1) * sizeof(wchar_t));
	wcscpy(szCopy,szSrc);
	EmWsz::TrimRight(szCopy);
	xStr = szCopy;
	free(szCopy);
	return xStr;
}

em::EmWstr em::EmWstr::Trim( const wchar_t* szSrc )
{
	if(szSrc == NULL || wcslen(szSrc) == 0){
		return EM_EMPTY_WSZ;
	}
	EmWstr xStr;
	wchar_t *szCopy = (wchar_t*)malloc((wcslen(szSrc) + 1) * sizeof(wchar_t));
	wcscpy(szCopy,szSrc);
	EmWsz::Trim(szCopy);
	xStr = szCopy;
	free(szCopy);
	return xStr;
}

em::EmWstr em::EmWstr::FormatNum( const wchar_t* szSrc, unsigned int nLenAfterPoint )
{
	EmWstr xStr;
	wchar_t szCopy[500] = {0};
	wcscpy(szCopy,szSrc);
	EmWsz::FormatNum(szCopy,nLenAfterPoint);
	xStr = szCopy;
	return xStr;
}

em::EmWstr em::EmWstr::Replace( const wchar_t* szSrc, const wchar_t* szFind, const wchar_t* szReplace )
{
	if(szSrc == NULL || wcslen(szSrc) == 0){
		return EM_EMPTY_WSZ;
	}

	if(szFind == NULL || wcslen(szFind) == 0){
		return szSrc;
	}

	if(szReplace == NULL){
		szReplace = EM_EMPTY_WSZ;
	}

	EmWstr xStr;
	wchar_t *szCopy = (wchar_t*)malloc( 255 + ( wcslen(szSrc) * EmUtil::Abs(wcslen(szReplace) - wcslen(szFind)) + 1) * sizeof(wchar_t));
	wcscpy(szCopy,szSrc);
	EmWsz::Replace(szCopy,szFind,szReplace);
	xStr = szCopy;
	free(szCopy);
	return xStr;
}

em::EmWstr em::EmWstr::Cut( const wchar_t* szSrc, unsigned int nStart, unsigned int nLen )
{
	if(szSrc == NULL || wcslen(szSrc) == 0){
		return EM_EMPTY_WSZ;
	}

	int iSrcLen = wcslen(szSrc);
	if(nStart >= iSrcLen){
		return EM_EMPTY_WSZ;
	}
	if(nStart + nLen > iSrcLen){
		return EM_EMPTY_WSZ;
	}


	EmWstr xStr;
	wchar_t *szCopy = (wchar_t*)malloc(( wcslen(szSrc) + 1) * sizeof(wchar_t) );
	wcscpy(szCopy,szSrc);
	EmWsz::Cut(szCopy,nStart,nLen);
	xStr = szCopy;
	free(szCopy);
	return xStr;
}

em::EmWstr em::EmWstr::Insert( const wchar_t* szSrc, const wchar_t* szInsert, unsigned int nStart )
{
	if(szSrc == NULL){
		szSrc = EM_EMPTY_WSZ;
	}
	
	if(nStart >= wcslen(szSrc)){
		return EM_EMPTY_WSZ;
	}

	if(szInsert == NULL || wcslen(szInsert) == 0){
		return szSrc;
	}

	EmWstr xStr;
	wchar_t *szCopy = (wchar_t*)malloc(( wcslen(szSrc) + wcslen(szInsert) + 1) * sizeof(wchar_t));
	wcscpy(szCopy,szSrc);
	EmWsz::Insert(szCopy,szInsert,nStart);
	xStr = szCopy;
	free(szCopy);
	return xStr;
}

em::EmWstr em::EmWstr::Splice( const wchar_t* szSrc, const wchar_t* szInsert, unsigned int nStart, unsigned int nLen )
{
	if(szSrc == NULL){
		szSrc = EM_EMPTY_WSZ;
	}
	
	if(nStart >= wcslen(szSrc)){
		return EM_EMPTY_WSZ;
	}
	
	if(szInsert == NULL || wcslen(szInsert) == 0){
		szInsert = EM_EMPTY_WSZ;
	}

	if(nStart + nLen > wcslen(szSrc)){
		return EM_EMPTY_WSZ;
	}

	EmWstr xStr;
	wchar_t *szCopy = (wchar_t*)malloc(( wcslen(szSrc) + wcslen(szInsert) + 1) * sizeof(wchar_t));
	wcscpy(szCopy,szSrc);
	EmWsz::Splice(szCopy,szInsert,nStart,nLen);
	xStr = szCopy;
	free(szCopy);
	return xStr;
}

bool em::EmWstr::IsDigit( wchar_t ch )
{
	return EmWsz::IsDigit(ch);
}

bool em::EmWstr::IsLetter( wchar_t ch )
{
	return EmWsz::IsLetter(ch);
}

bool em::EmWstr::IsInt( const wchar_t* szSrc )
{
	return EmWsz::IsInt(szSrc);
}

bool em::EmWstr::IsNumber( const wchar_t* szSrc )
{
	return EmWsz::IsNumber(szSrc);
}

bool em::EmWstr::IsDomain( const wchar_t* szSrc )
{
	return EmWsz::IsDomain(szSrc);
}

bool em::EmWstr::IsEmail( const wchar_t* szSrc )
{
	return EmWsz::IsEmail(szSrc);
}

bool em::EmWstr::IsIp( const wchar_t* szSrc )
{
	return EmWsz::IsIp(szSrc);
}

bool em::EmWstr::IsUrl( const wchar_t* szSrc )
{
	return EmWsz::IsUrl(szSrc);
}

bool em::EmWstr::IsAsc( const wchar_t* szSrc )
{
	return EmWsz::IsAsc(szSrc);
}

bool em::EmWstr::IsLimitedIn( const wchar_t* szSrc, const wchar_t* szLimited )
{
	return EmWsz::IsLimitedIn(szSrc,szLimited);
}

bool em::EmWstr::IsDateTime( const wchar_t* szSrc )
{
	return EmWsz::IsDateTime(szSrc);
}

INT64 em::EmWstr::IpTextToNum( const wchar_t* szIpText )
{
	return EmWsz::IpTextToNum(szIpText);
}

em::EmWstr em::EmWstr::IpNumToText( INT64 iIpNum )
{
	wchar_t szCopy[100] = {0};
	EmWsz::IpNumToText(szCopy,iIpNum);
	return szCopy;
}

em::EmWstr em::EmWstr::UrlGetProtocol( const wchar_t* szSrc )
{
	wchar_t szCopy[1024] = {0};
	EmWsz::UrlGetProtocol(szCopy,szSrc);
	return szCopy;
}

em::EmWstr em::EmWstr::UrlGetHost( const wchar_t* szSrc )
{
	wchar_t szCopy[1024] = {0};
	EmWsz::UrlGetHost(szCopy,szSrc);
	return szCopy;
}

unsigned short em::EmWstr::UrlGetPort( const wchar_t* szSrc )
{
	return EmWsz::UrlGetPort(szSrc);
}

em::EmWstr em::EmWstr::UrlGetPath( const wchar_t* szSrc )
{
	wchar_t szCopy[1024] = {0};
	EmWsz::UrlGetPath(szCopy,szSrc);
	return szCopy;
}

em::EmWstr em::EmWstr::UrlGetParam( const wchar_t* szSrc )
{
	wchar_t szCopy[1024] = {0};
	EmWsz::UrlGetParam(szCopy,szSrc);
	return szCopy;
}

em::EmWstr em::EmWstr::UrlGetPathWithParam( const wchar_t* szSrc )
{
	wchar_t szCopy[1024] = {0};
	EmWsz::UrlGetPathWithParam(szCopy,szSrc);
	return szCopy;
}

em::EmWstr em::EmWstr::FileDirFromPath( const wchar_t* szSrc )
{
	wchar_t szCopy[1024] = {0};
	EmWsz::FileDirFromPath(szCopy,szSrc);
	return szCopy;
}

em::EmWstr em::EmWstr::FileNameFromPath( const wchar_t* szSrc )
{
	wchar_t szCopy[1024] = {0};
	EmWsz::FileNameFromPath(szCopy,szSrc);
	return szCopy;
}

em::EmWstr em::EmWstr::FileTitleFromPath( const wchar_t* szSrc )
{
	wchar_t szCopy[1024] = {0};
	EmWsz::FileTitleFromPath(szCopy,szSrc);
	return szCopy;
}

em::EmWstr em::EmWstr::FileExtFromPath( const wchar_t* szSrc )
{
	wchar_t szCopy[1024] = {0};
	EmWsz::FileExtFromPath(szCopy,szSrc);
	return szCopy;
}

em::EmWstr em::EmWstr::FileFixDir( const wchar_t* szSrc )
{
	wchar_t szCopy[1024] = {0};
	EmWsz::FileFixDir(szCopy,szSrc);
	return szCopy;
}










