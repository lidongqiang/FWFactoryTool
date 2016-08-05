#include "stdafx.h"
#include "../inc/EmTstr.h"
#include "../inc/EmUtil.h"
#include "../inc/EmTsz.h"
#include "../inc/EmCharSet.h"
#include "../inc/EmUrl.h"
#include "../inc/EmMd5.h"
#include "../inc/EmBase64.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

using namespace em;

void em::EmTstr::Init()
{
	m_iLength = 0;
	m_iCapacity = 0;
	m_bufData = NULL;
}

em::EmTstr::EmTstr()
{
	Init();
}

em::EmTstr::EmTstr( const TCHAR* szSrc)
{
	Init();
	Copy(szSrc);
}

em::EmTstr::EmTstr( const EmTstr& xCopy)
{
	Init();
	Copy(xCopy.Sz());
}

em::EmTstr::~EmTstr()
{
	EM_FREE_NULL(m_bufData);
}

const TCHAR* em::EmTstr::Sz() const
{
	if(m_bufData == NULL){
		return EM_EMPTY_TSZ;
	}

	return m_bufData;
}

int em::EmTstr::Capacity() const
{
	return m_iCapacity;
}

TCHAR em::EmTstr::At( int iIndex ) const
{
	if(m_iLength <= iIndex){
		return 0;
	}else{
		return m_bufData[iIndex];
	}
}

EmTstr& em::EmTstr::Resize( int iSize )
{
	if(iSize == 0){
		Empty();
	}else{
		if(m_bufData == NULL){
			m_iCapacity = iSize;
			m_bufData = (TCHAR*)malloc((m_iCapacity + 1) * sizeof(TCHAR));
			m_bufData[0] = 0;
		}else{
			if(iSize > m_iCapacity){
				m_iCapacity = iSize;
				m_bufData = (TCHAR*)realloc(m_bufData, (m_iCapacity + 1) * sizeof(TCHAR)) ;
			}
		}
	}

	return *this;
}

int em::EmTstr::Length() const
{
	return m_iLength;
}

EmTstr& em::EmTstr::Append( const TCHAR* szSrc)
{
	if(szSrc == NULL || _tcslen(szSrc) == 0){
		return *this;
	}

	int iSrcLen = _tcslen(szSrc);
	if(m_bufData == NULL){
		m_iCapacity = (m_iLength + iSrcLen + 1) * 2;
		m_bufData = (TCHAR*)malloc((m_iCapacity + 1) * sizeof(TCHAR));
		m_bufData[0] = 0;
	}
	else if(m_iCapacity <= iSrcLen + m_iLength){
		m_iCapacity = (m_iLength + iSrcLen + 1) * 2;
		m_bufData = (TCHAR*)realloc(m_bufData,(m_iCapacity + 1) * sizeof(TCHAR));
	}
	_tcscat(m_bufData,szSrc);
	m_iLength += iSrcLen;

	return *this;
}

EmTstr& em::EmTstr::Append( const TCHAR* bufSrc, int iSize )
{
	if(bufSrc == NULL || iSize == 0){
		return *this;
	}
	
	int iSrcLen = iSize;
	if(m_bufData == NULL){
		m_iCapacity = (m_iLength + iSrcLen + 1) * 2;
		m_bufData = (TCHAR*)malloc((m_iCapacity + 1) * sizeof(TCHAR));
		m_bufData[0] = 0;
	}
	else if(m_iCapacity <= iSrcLen + m_iLength){
		m_iCapacity = (m_iLength + iSrcLen + 1) * 2;
		m_bufData = (TCHAR*)realloc(m_bufData,(m_iCapacity + 1) * sizeof(TCHAR));
	}
	memcpy(m_bufData + m_iLength, bufSrc, iSrcLen);
	m_iLength += iSrcLen;
	m_bufData[m_iLength] = 0;
	
	return *this;
}





void em::EmTstr::Empty()
{
	if(m_bufData == NULL){
		return;
	}
	EM_FREE_NULL(m_bufData);
	m_iLength = 0;
	m_iCapacity = 0;
}

bool em::EmTstr::Equals( const TCHAR* szSrc ) const
{
	if(szSrc == NULL){
		if(m_iLength == 0){
			return true;
		}else{
			return false;
		}
	}else{
		if(m_iLength != _tcslen(szSrc)){
			return false;
		}else{
			if(_tcscmp(m_bufData,szSrc) == 0){
				return true;
			}else{
				return false;
			}
		}
	}
}

int em::EmTstr::Compare( const TCHAR* szSrc ) const
{
	if(szSrc == NULL){
		if(m_iLength == 0){
			return 0;
		}else{
			return 1;
		}
	}else{
		return _tcscmp(m_bufData,szSrc);
	}
}

EmTstr& em::EmTstr::Prepend( const TCHAR* szSrc )
{
	if(szSrc == NULL || _tcslen(szSrc) == 0){
		return *this;
	}
	
	int iSrcLen = _tcslen(szSrc);
	if(m_iCapacity <= iSrcLen + m_iLength){
		m_iCapacity = (m_iLength + iSrcLen + 1) * 2;
		m_bufData = (TCHAR*)realloc(m_bufData,(m_iCapacity + 1) * sizeof(TCHAR));
	}

	memmove(m_bufData + iSrcLen, m_bufData,m_iLength + 1);
	memcpy(m_bufData,szSrc,iSrcLen);

	m_iLength += iSrcLen;
	return *this;
}

EmTstr& em::EmTstr::Copy( const TCHAR* szSrc )
{
	if(szSrc == NULL || _tcslen(szSrc) == 0){
		Empty();
	}else {
		int iSrcLen = _tcslen(szSrc);
		if(m_bufData == NULL){
			m_iCapacity = iSrcLen * 2;
			m_bufData = (TCHAR*)malloc((m_iCapacity + 1 ) * sizeof(TCHAR) );
		}else if(m_iCapacity <= iSrcLen){
			m_iCapacity = iSrcLen * 2;
			m_bufData = (TCHAR*)realloc(m_bufData, (m_iCapacity + 1) * sizeof(TCHAR));
		}
		
		_tcscpy(m_bufData,szSrc);
		m_iLength = iSrcLen;
		
	}
	return *this;
}


// em::EmTstr::operator const wchar_t* () const
// {
// 	return (const wchar_t*)m_bufData;
// }
// 
// em::EmTstr::operator const char* () const
// {
// 	return (const char*)m_bufData;
// }

TCHAR em::EmTstr::operator[]( int iIndex ) const
{
	return At(iIndex);
}

EmTstr& em::EmTstr::operator += ( const TCHAR* szSrc )
{
	return Append(szSrc);
}

EmTstr& em::EmTstr::operator = ( const TCHAR* szSrc )
{
	Copy(szSrc);
	return *this;
}

EmTstr& em::EmTstr::operator = ( const EmTstr& xOther )
{
	Copy(xOther.Sz());
	return *this;
}

em::EmTstr em::EmTstr::Int2Str( int iNum )
{
	TCHAR szCopy[50] = {0};
	EmTsz::Int2Str(szCopy,iNum);
	return szCopy;
}

em::EmTstr em::EmTstr::Long2Str( INT64 iNum )
{
	TCHAR szCopy[100] = {0};
	EmTsz::Long2Str(szCopy,iNum);
	return szCopy;
}

int em::EmTstr::Str2Int( const TCHAR* szSrc )
{
	return EmTsz::Str2Int(szSrc);
}

INT64 em::EmTstr::Str2Long( const TCHAR* szSrc )
{
	return EmTsz::Str2Long(szSrc);
}

em::EmTstr em::EmTstr::Float2Str( double fNum )
{
	TCHAR szCopy[100] = {0};
	EmTsz::Float2Str(szCopy,fNum);
	return szCopy;
}

int em::EmTstr::Char2Int( TCHAR chSrc )
{
	return EmTsz::Char2Int(chSrc);
}

em::EmTstr em::EmTstr::Char2Str( TCHAR chSrc )
{
	TCHAR szCopy[2] = {0};
	EmTsz::Char2Str(szCopy,chSrc);
	return szCopy;
}

TCHAR em::EmTstr::Str2Char( const TCHAR* szSrc )
{
	return EmTsz::Str2Char(szSrc);
}

em::EmTstr em::EmTstr::Lower( TCHAR* szSrc )
{
	if(szSrc == NULL || _tcslen(szSrc) == 0){
		return EM_EMPTY_TSZ;
	}
	EmTstr xStr;
	TCHAR *szCopy = (TCHAR*)malloc((_tcslen(szSrc) + 1) * sizeof(TCHAR));
	_tcscpy(szCopy,szSrc);
	EmTsz::Lower(szCopy);
	xStr = szCopy;
	free(szCopy);
	return xStr;
}

em::EmTstr em::EmTstr::Upper( TCHAR* szSrc )
{
	if(szSrc == NULL || _tcslen(szSrc) == 0){
		return EM_EMPTY_TSZ;
	}

	EmTstr xStr;
	TCHAR *szCopy = (TCHAR*)malloc((_tcslen(szSrc) + 1) * sizeof(TCHAR));
	_tcscpy(szCopy,szSrc);
	EmTsz::Upper(szCopy);
	xStr = szCopy;
	free(szCopy);
	return xStr;
}

em::EmTstr em::EmTstr::Reverse( TCHAR* szSrc )
{
	if(szSrc == NULL){
		return EM_EMPTY_TSZ;
	}
	if(_tcslen(szSrc) <= 1){
		return szSrc;
	}

	EmTstr xStr;
	TCHAR *szCopy = (TCHAR*)malloc((_tcslen(szSrc) + 1)* sizeof(TCHAR));
	_tcscpy(szCopy,szSrc);
	EmTsz::Reverse(szCopy);
	xStr = szCopy;
	free(szCopy);
	return xStr;
}

bool em::EmTstr::Equal( const TCHAR* szLeft, const TCHAR* szRight )
{
	return EmTsz::Equal(szLeft,szRight);
}

bool em::EmTstr::EqualNoCase( const TCHAR* szLeft, const TCHAR* szRight )
{
	return EmTsz::EqualNoCase(szLeft,szRight);
}

unsigned int em::EmTstr::CountChar( const TCHAR* szSrc, TCHAR chFind )
{
	return EmTsz::CountChar(szSrc,chFind);
}

unsigned int em::EmTstr::CountStr( const TCHAR* szSrc, const TCHAR* szFind )
{
	return EmTsz::EqualNoCase(szSrc,szFind);
}

em::EmTstr em::EmTstr::Prefix( const TCHAR* szSrc, const TCHAR* szPrefix )
{
	if(szSrc == NULL){
		szSrc = EM_EMPTY_TSZ;
	}
	if(szPrefix == NULL || _tcslen(szPrefix) == 0){
		return szSrc;
	}

	EmTstr xStr;
	TCHAR *szCopy = (TCHAR*)malloc((_tcslen(szSrc) + _tcslen(szPrefix) + 1) * sizeof(TCHAR));
	_tcscpy(szCopy,szSrc);
	EmTsz::Prefix(szCopy,szPrefix);
	xStr = szCopy;
	free(szCopy);
	return xStr;
}

em::EmTstr em::EmTstr::PadLeft( const TCHAR* szSrc, TCHAR chPad, int iTotalLen )
{
	if(szSrc == NULL){
		szSrc = EM_EMPTY_TSZ;
	}

	if(_tcslen(szSrc) >= iTotalLen){
		return szSrc;
	}

	EmTstr xStr;
	TCHAR *szCopy = (TCHAR*)malloc((_tcslen(szSrc) + iTotalLen + 1) * sizeof(TCHAR));
	_tcscpy(szCopy,szSrc);
	EmTsz::PadLeft(szCopy,chPad,iTotalLen);
	xStr = szCopy;
	free(szCopy);
	return xStr;
}

em::EmTstr em::EmTstr::PadRight( const TCHAR* szSrc, TCHAR chPad, int iTotalLen )
{
	if(szSrc == NULL){
		szSrc = EM_EMPTY_TSZ;
	}

	if(_tcslen(szSrc) >= iTotalLen){
		return szSrc;
	}
	
	EmTstr xStr;
	TCHAR *szCopy = (TCHAR*)malloc((_tcslen(szSrc) + iTotalLen + 1) * sizeof(TCHAR));
	_tcscpy(szCopy,szSrc);
	EmTsz::PadRight(szCopy,chPad,iTotalLen);
	xStr = szCopy;
	free(szCopy);
	return xStr;
}

em::EmTstr em::EmTstr::SubStr( const TCHAR *szSrc, unsigned int iStart, int iSubLen )
{
	if(szSrc == NULL){
		szSrc = EM_EMPTY_TSZ;
	}

	int iSrcLen = _tcslen(szSrc);
	if(iSrcLen == 0){
		return EM_EMPTY_TSZ;
	}

	if(iStart >= iSrcLen){
		return EM_EMPTY_TSZ;
	}

	if(iSubLen + iStart > iSrcLen ){
		return EM_EMPTY_TSZ;
	}

	EmTstr xStr;
	TCHAR *szCopy = (TCHAR*)malloc((_tcslen(szSrc) + 1) * sizeof(TCHAR));
	
	EmTsz::SubStr(szCopy,szSrc,iStart,iSubLen);
	xStr = szCopy;
	free(szCopy);
	return xStr;
}

em::EmTstr em::EmTstr::Repeat( const TCHAR* szSrc,int iCount )
{
	if(szSrc == NULL || _tcslen(szSrc) == 0 || iCount == 0){
		return EM_EMPTY_TSZ;
	}
	if(iCount == 1){
		return szSrc;
	}

	EmTstr xStr;
	TCHAR *szCopy = (TCHAR*)malloc((iCount * _tcslen(szSrc) + 1) * sizeof(TCHAR));
	EmTsz::Repeat(szCopy,szSrc,iCount);
	xStr = szCopy;
	free(szCopy);
	return xStr;
}

em::EmTstr em::EmTstr::AddSlashes( const TCHAR* szSrc, TCHAR chFind )
{
	if(szSrc == NULL || _tcslen(szSrc) == 0){
		return EM_EMPTY_TSZ;
	}

	EmTstr xStr;
	TCHAR *szCopy = (TCHAR*)malloc((2 * _tcslen(szSrc) + 1) * sizeof(TCHAR));
	EmTsz::AddSlashes(szCopy,chFind);
	xStr = szCopy;
	free(szCopy);
	return xStr;
}

em::EmTstr em::EmTstr::StripSlashes( const TCHAR* szSrc )
{
	if(szSrc == NULL || _tcslen(szSrc) == 0){
		return EM_EMPTY_TSZ;
	}

	EmTstr xStr;
	TCHAR *szCopy = (TCHAR*)malloc((2 * _tcslen(szSrc) + 1) * sizeof(TCHAR));
	EmTsz::StripSlashes(szCopy);
	xStr = szCopy;
	free(szCopy);
	return xStr;
}

em::EmTstr em::EmTstr::Dec2Bin( const TCHAR* szSrc )
{
	if(szSrc == NULL || _tcslen(szSrc) == 0){
		return EM_EMPTY_TSZ;
	}
	TCHAR szCopy[200] = {0};
	_tcscpy(szCopy,szSrc);
	EmTsz::Dec2Bin(szCopy);
	return szCopy;
}

em::EmTstr em::EmTstr::Bin2Dec( const TCHAR* szSrc )
{
	if(szSrc == NULL || _tcslen(szSrc) == 0){
		return EM_EMPTY_TSZ;
	}
	TCHAR szCopy[200] = {0};
	_tcscpy(szCopy,szSrc);
	EmTsz::Bin2Dec(szCopy);
	return szCopy;
}

em::EmTstr em::EmTstr::Hex2Dec( const TCHAR* szSrc )
{
	if(szSrc == NULL || _tcslen(szSrc) == 0){
		return EM_EMPTY_TSZ;
	}
	TCHAR szCopy[200] = {0};
	_tcscpy(szCopy,szSrc);
	EmTsz::Hex2Dec(szCopy);
	return szCopy;
}

em::EmTstr em::EmTstr::Dec2Hex( const TCHAR* szSrc )
{
	if(szSrc == NULL || _tcslen(szSrc) == 0){
		return EM_EMPTY_TSZ;
	}
	TCHAR szCopy[200] = {0};
	_tcscpy(szCopy,szSrc);
	EmTsz::Dec2Hex(szCopy);
	return szCopy;
}

em::EmTstr em::EmTstr::Bin2Hex( const TCHAR* szSrc )
{
	if(szSrc == NULL || _tcslen(szSrc) == 0){
		return EM_EMPTY_TSZ;
	}
	TCHAR szCopy[200] = {0};
	_tcscpy(szCopy,szSrc);
	EmTsz::Bin2Hex(szCopy);
	return szCopy;
}

em::EmTstr em::EmTstr::Hex2Bin( const TCHAR* szSrc )
{
	if(szSrc == NULL || _tcslen(szSrc) == 0){
		return EM_EMPTY_TSZ;
	}
	TCHAR szCopy[200] = {0};
	_tcscpy(szCopy,szSrc);
	EmTsz::Hex2Bin(szCopy);
	return szCopy;
}

int em::EmTstr::FindChar( const TCHAR *szSrc, TCHAR chFind, unsigned int iFromPos )
{
	return EmTsz::FindChar(szSrc,chFind,iFromPos);
}

int em::EmTstr::FindFrom( const TCHAR *szSrc, const TCHAR *szFind, unsigned int iFromPos )
{
	return EmTsz::FindFrom(szSrc,szFind,iFromPos);
}

int em::EmTstr::Find( const TCHAR *szSrc, const TCHAR *szFind )
{
	return EmTsz::Find(szSrc,szFind);
}

int em::EmTstr::FindReverse( const TCHAR* szSrc, const TCHAR* szFind )
{
	return EmTsz::FindReverse(szSrc,szFind);
}

int em::EmTstr::FindSafe( const TCHAR* szSrc, const TCHAR* szFind, unsigned int iLimit )
{
	return EmTsz::FindSafe(szSrc,szFind,iLimit);
}

em::EmTstr em::EmTstr::TrimLeft( const TCHAR* szSrc )
{
	if(szSrc == NULL || _tcslen(szSrc) == 0){
		return EM_EMPTY_TSZ;
	}
	EmTstr xStr;
	TCHAR *szCopy = (TCHAR*)malloc((_tcslen(szSrc) + 1) * sizeof(TCHAR));
	_tcscpy(szCopy,szSrc);
	EmTsz::TrimLeft(szCopy);
	xStr = szCopy;
	free(szCopy);
	return xStr;
}

em::EmTstr em::EmTstr::TrimRight( const TCHAR* szSrc )
{
	if(szSrc == NULL || _tcslen(szSrc) == 0){
		return EM_EMPTY_TSZ;
	}
	EmTstr xStr;
	TCHAR *szCopy = (TCHAR*)malloc((_tcslen(szSrc) + 1) * sizeof(TCHAR));
	_tcscpy(szCopy,szSrc);
	EmTsz::TrimRight(szCopy);
	xStr = szCopy;
	free(szCopy);
	return xStr;
}

em::EmTstr em::EmTstr::Trim( const TCHAR* szSrc )
{
	if(szSrc == NULL || _tcslen(szSrc) == 0){
		return EM_EMPTY_TSZ;
	}
	EmTstr xStr;
	TCHAR *szCopy = (TCHAR*)malloc((_tcslen(szSrc) + 1) * sizeof(TCHAR));
	_tcscpy(szCopy,szSrc);
	EmTsz::Trim(szCopy);
	xStr = szCopy;
	free(szCopy);
	return xStr;
}

em::EmTstr em::EmTstr::FormatNum( const TCHAR* szSrc, unsigned int nLenAfterPoint )
{
	EmTstr xStr;
	TCHAR szCopy[500] = {0};
	_tcscpy(szCopy,szSrc);
	EmTsz::FormatNum(szCopy,nLenAfterPoint);
	xStr = szCopy;
	return xStr;
}

em::EmTstr em::EmTstr::Replace( const TCHAR* szSrc, const TCHAR* szFind, const TCHAR* szReplace )
{
	if(szSrc == NULL || _tcslen(szSrc) == 0){
		return EM_EMPTY_TSZ;
	}

	if(szFind == NULL || _tcslen(szFind) == 0){
		return szSrc;
	}

	if(szReplace == NULL){
		szReplace = EM_EMPTY_TSZ;
	}

	EmTstr xStr;
	TCHAR *szCopy = (TCHAR*)malloc( 255 + ( _tcslen(szSrc) * EmUtil::Abs(_tcslen(szReplace) - _tcslen(szFind) + 1) + 1) * sizeof(TCHAR));
	_tcscpy(szCopy,szSrc);
	EmTsz::Replace(szCopy,szFind,szReplace);
	xStr = szCopy;
	free(szCopy);
	return xStr;
}

em::EmTstr em::EmTstr::Cut( const TCHAR* szSrc, unsigned int nStart, unsigned int nLen )
{
	if(szSrc == NULL || _tcslen(szSrc) == 0){
		return EM_EMPTY_TSZ;
	}

	int iSrcLen = _tcslen(szSrc);
	if(nStart >= iSrcLen){
		return EM_EMPTY_TSZ;
	}
	if(nStart + nLen > iSrcLen){
		return EM_EMPTY_TSZ;
	}


	EmTstr xStr;
	TCHAR *szCopy = (TCHAR*)malloc(( _tcslen(szSrc) + 1) * sizeof(TCHAR) );
	_tcscpy(szCopy,szSrc);
	EmTsz::Cut(szCopy,nStart,nLen);
	xStr = szCopy;
	free(szCopy);
	return xStr;
}

em::EmTstr em::EmTstr::Insert( const TCHAR* szSrc, const TCHAR* szInsert, unsigned int nStart )
{
	if(szSrc == NULL){
		szSrc = EM_EMPTY_TSZ;
	}
	
	if(nStart >= _tcslen(szSrc)){
		return EM_EMPTY_TSZ;
	}

	if(szInsert == NULL || _tcslen(szInsert) == 0){
		return szSrc;
	}

	EmTstr xStr;
	TCHAR *szCopy = (TCHAR*)malloc(( _tcslen(szSrc) + _tcslen(szInsert) + 1) * sizeof(TCHAR));
	_tcscpy(szCopy,szSrc);
	EmTsz::Insert(szCopy,szInsert,nStart);
	xStr = szCopy;
	free(szCopy);
	return xStr;
}

em::EmTstr em::EmTstr::Splice( const TCHAR* szSrc, const TCHAR* szInsert, unsigned int nStart, unsigned int nLen )
{
	if(szSrc == NULL){
		szSrc = EM_EMPTY_TSZ;
	}
	
	if(nStart >= _tcslen(szSrc)){
		return EM_EMPTY_TSZ;
	}
	
	if(szInsert == NULL || _tcslen(szInsert) == 0){
		szInsert = EM_EMPTY_TSZ;
	}

	if(nStart + nLen > _tcslen(szSrc)){
		return EM_EMPTY_TSZ;
	}

	EmTstr xStr;
	TCHAR *szCopy = (TCHAR*)malloc(( _tcslen(szSrc) + _tcslen(szInsert) + 1) * sizeof(TCHAR));
	_tcscpy(szCopy,szSrc);
	EmTsz::Splice(szCopy,szInsert,nStart,nLen);
	xStr = szCopy;
	free(szCopy);
	return xStr;
}

bool em::EmTstr::IsDigit( TCHAR ch )
{
	return EmTsz::IsDigit(ch);
}

bool em::EmTstr::IsLetter( TCHAR ch )
{
	return EmTsz::IsLetter(ch);
}

bool em::EmTstr::IsInt( const TCHAR* szSrc )
{
	return EmTsz::IsInt(szSrc);
}

bool em::EmTstr::IsNumber( const TCHAR* szSrc )
{
	return EmTsz::IsNumber(szSrc);
}

bool em::EmTstr::IsDomain( const TCHAR* szSrc )
{
	return EmTsz::IsDomain(szSrc);
}

bool em::EmTstr::IsEmail( const TCHAR* szSrc )
{
	return EmTsz::IsEmail(szSrc);
}

bool em::EmTstr::IsIp( const TCHAR* szSrc )
{
	return EmTsz::IsIp(szSrc);
}

bool em::EmTstr::IsUrl( const TCHAR* szSrc )
{
	return EmTsz::IsUrl(szSrc);
}

bool em::EmTstr::IsAsc( const TCHAR* szSrc )
{
	return EmTsz::IsAsc(szSrc);
}

bool em::EmTstr::IsLimitedIn( const TCHAR* szSrc, const TCHAR* szLimited )
{
	return EmTsz::IsLimitedIn(szSrc,szLimited);
}

bool em::EmTstr::IsDateTime( const TCHAR* szSrc )
{
	return EmTsz::IsDateTime(szSrc);
}

INT64 em::EmTstr::IpTextToNum( const TCHAR* szIpText )
{
	return EmTsz::IpTextToNum(szIpText);
}

em::EmTstr em::EmTstr::IpNumToText( INT64 iIpNum )
{
	TCHAR szCopy[100] = {0};
	EmTsz::IpNumToText(szCopy,iIpNum);
	return szCopy;
}

em::EmTstr em::EmTstr::UrlGetProtocol( const TCHAR* szSrc )
{
	TCHAR szCopy[1024] = {0};
	EmTsz::UrlGetProtocol(szCopy,szSrc);
	return szCopy;
}

em::EmTstr em::EmTstr::UrlGetHost( const TCHAR* szSrc )
{
	TCHAR szCopy[1024] = {0};
	EmTsz::UrlGetHost(szCopy,szSrc);
	return szCopy;
}

unsigned short em::EmTstr::UrlGetPort( const TCHAR* szSrc )
{
	return EmTsz::UrlGetPort(szSrc);
}

em::EmTstr em::EmTstr::UrlGetPath( const TCHAR* szSrc )
{
	TCHAR szCopy[1024] = {0};
	EmTsz::UrlGetPath(szCopy,szSrc);
	return szCopy;
}

em::EmTstr em::EmTstr::UrlGetParam( const TCHAR* szSrc )
{
	TCHAR szCopy[1024] = {0};
	EmTsz::UrlGetParam(szCopy,szSrc);
	return szCopy;
}

em::EmTstr em::EmTstr::UrlGetPathWithParam( const TCHAR* szSrc )
{
	TCHAR szCopy[1024] = {0};
	EmTsz::UrlGetPathWithParam(szCopy,szSrc);
	return szCopy;
}

em::EmTstr em::EmTstr::FileDirFromPath( const TCHAR* szSrc )
{
	TCHAR szCopy[1024] = {0};
	EmTsz::FileDirFromPath(szCopy,szSrc);
	return szCopy;
}

em::EmTstr em::EmTstr::FileNameFromPath( const TCHAR* szSrc )
{
	TCHAR szCopy[1024] = {0};
	EmTsz::FileNameFromPath(szCopy,szSrc);
	return szCopy;
}

em::EmTstr em::EmTstr::FileTitleFromPath( const TCHAR* szSrc )
{
	TCHAR szCopy[1024] = {0};
	EmTsz::FileTitleFromPath(szCopy,szSrc);
	return szCopy;
}

em::EmTstr em::EmTstr::FileExtFromPath( const TCHAR* szSrc )
{
	TCHAR szCopy[1024] = {0};
	EmTsz::FileExtFromPath(szCopy,szSrc);
	return szCopy;
}

em::EmTstr em::EmTstr::FileFixDir( const TCHAR* szSrc )
{
	TCHAR szCopy[1024] = {0};
	EmTsz::FileFixDir(szCopy,szSrc);
	return szCopy;
}

em::EmTstr em::EmTstr::FromUtf8( const char* szSrc )
{
	TCHAR *szDest = NULL;
	int iSrcLen = 0;
	EmTstr strResult;
	
	if(szSrc == NULL)
	{
		return EM_EMPTY_TSZ;
	}
	
	iSrcLen = strlen(szSrc);
	if(iSrcLen == 0)
	{
		return EM_EMPTY_TSZ;
	}
	
	szDest = (TCHAR *)malloc((iSrcLen + 1) * sizeof(TCHAR));
	szDest[0] = 0;
	EmCharSet::Utf8ToTcs(szDest,szSrc);
	strResult = szDest;
	EM_FREE_NULL(szDest);
	return strResult;
}

em::EmStr em::EmTstr::ToUtf8( const TCHAR* szSrc )
{
	EmStr strResult;
	char* szDest = NULL;
	int iSrcLen = 0;
	
	if(szSrc == NULL)
	{
		return "";
	}
	
	iSrcLen = _tcslen(szSrc);

	
	if(iSrcLen == 0)
	{
		return "";
	}

	szDest = (char*)malloc(iSrcLen * 6 + 1);
	memset(szDest,0,iSrcLen * 6 + 1);
	EmCharSet::TcsToUtf8(szDest,szSrc);
	strResult = szDest;
	EM_FREE_NULL(szDest);
	return strResult;
}

em::EmTstr em::EmTstr::FromAnsi( const char* szSrc )
{
	EmTstr strResult;
	if(szSrc == NULL){
		return strResult;
	}
	int iSrcLen = strlen(szSrc);

	if(iSrcLen > 0)
	{
		TCHAR *szDest = (TCHAR*)malloc(iSrcLen* 2 + 2);
		memset(szDest,0,iSrcLen * 2 + 2);

		EmCharSet::Mbs2Tcs(szDest,szSrc);

		strResult = szDest;
		EM_FREE_NULL(szDest);
	}

	return strResult;
}

em::EmStr em::EmTstr::ToAnsi( const TCHAR* szSrc )
{
	EmStr strResult;
	if(szSrc == NULL){
		return strResult;
	}
	int iSrcLen = _tcslen(szSrc);

	if(iSrcLen > 0)
	{
		char *szDest = (char*)malloc(iSrcLen * 2 + 2);
		memset(szDest,0,iSrcLen * 2 + 2);
		
		EmCharSet::Tcs2Mbs(szDest,szSrc);
		
		strResult = szDest;
		
		EM_FREE_NULL(szDest);
	}
	
	return strResult;
}

int em::EmTstr::ListInBuffer( std::vector<EmTstr>& rList, const TCHAR* szBuffer, int iBufSize )
{
	rList.clear();
	if(iBufSize == 0){
		return 0;
	}
	int i = 0;
	int iTempSize = 0;
	TCHAR *szTemp = (TCHAR*)malloc((iBufSize + 1) * sizeof(TCHAR));
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

EmTstr em::EmTstr::Md5( const TCHAR* szSrc )
{
	if(szSrc == NULL){
		szSrc = EM_EMPTY_TSZ;
	}
	
	EmTstr strResult;
	int iSrcLen = _tcslen(szSrc);

	if(sizeof(TCHAR) == sizeof(char)){
		char *szAnsiResult = (char*)malloc(iSrcLen * 6 + 6);
		szAnsiResult[0] = 0;
		EmMd5::Encode(szAnsiResult,(const char*)szSrc);
		strResult = (TCHAR*)szAnsiResult;
		free(szAnsiResult);
	}else{
		char *szAnsiResult = (char*)malloc(iSrcLen * 6 + 6);
		szAnsiResult[0] = 0;
		wchar_t *szUnicodeResult = (wchar_t*)malloc(iSrcLen * 6 + 6);
		szUnicodeResult[0] = 0;
		char *szAnsiSrc = (char*)malloc(iSrcLen * 6 + 6);
		szAnsiSrc[0] = 0;
		
		EmCharSet::Tcs2Mbs(szAnsiSrc,szSrc);		
		EmMd5::Encode(szAnsiResult,szAnsiSrc);
		EmCharSet::Mbs2Tcs(szUnicodeResult,szAnsiResult);
		
		strResult = (const TCHAR*)szUnicodeResult;
		free(szAnsiSrc);
		free(szAnsiResult);
		free(szUnicodeResult);
	}
	
	return strResult;
}

EmTstr em::EmTstr::UrlEncode( const TCHAR* szSrc, bool bEncodeAll )
{
	if(szSrc == NULL || _tcslen(szSrc) == 0){
		return EM_EMPTY_TSZ;
	}
	
	EmTstr strResult;
	int iSrcLen = _tcslen(szSrc);

	if(sizeof(TCHAR) == sizeof(char)){
		char *szAnsiResult = (char*)malloc(iSrcLen * 6 + 6);
		szAnsiResult[0] = 0;
		EmUrl::Encode(szAnsiResult,(const char*)szSrc,bEncodeAll);
		strResult = (TCHAR*)szAnsiResult;
		free(szAnsiResult);
	}else{
		char *szAnsiResult = (char*)malloc(iSrcLen * 6 + 6);
		szAnsiResult[0] = 0;
		wchar_t *szUnicodeResult = (wchar_t*)malloc(iSrcLen * 6 + 6);
		szUnicodeResult[0] = 0;
		char *szAnsiSrc = (char*)malloc(iSrcLen * 6 + 6);
		szAnsiSrc[0] = 0;

		EmCharSet::Tcs2Mbs(szAnsiSrc,szSrc);		
		EmUrl::Encode(szAnsiResult,szAnsiSrc,bEncodeAll);
		EmCharSet::Mbs2Tcs(szUnicodeResult,szAnsiResult);

		strResult = (const TCHAR*)szUnicodeResult;
		free(szAnsiSrc);
		free(szAnsiResult);
		free(szUnicodeResult);
	}
	
	return strResult;
}

EmTstr em::EmTstr::UrlDecode( const TCHAR* szSrc, bool bEncodeAll )
{
	if(szSrc == NULL || _tcslen(szSrc) == 0){
		return EM_EMPTY_TSZ;
	}
	
	EmTstr strResult;
	int iSrcLen = _tcslen(szSrc);

	if(sizeof(TCHAR) == sizeof(char)){
		char *szAnsiResult = (char*)malloc(iSrcLen * 6 + 6);
		szAnsiResult[0] = 0;
		EmUrl::Encode(szAnsiResult,(const char*)szSrc,bEncodeAll);
		strResult = (TCHAR*)szAnsiResult;
		free(szAnsiResult);
	}else{
		char *szAnsiResult = (char*)malloc(iSrcLen * 6 + 6);
		szAnsiResult[0] = 0;
		wchar_t *szUnicodeResult = (wchar_t*)malloc(iSrcLen * 6 + 6);
		szUnicodeResult[0] = 0;
		char *szAnsiSrc = (char*)malloc(iSrcLen * 6 + 6);
		szAnsiSrc[0] = 0;
		
		EmCharSet::Tcs2Mbs(szAnsiSrc,szSrc);		
		EmUrl::Decode(szAnsiResult,szAnsiSrc,bEncodeAll);
		EmCharSet::Mbs2Tcs(szUnicodeResult,szAnsiResult);
		
		strResult = (const TCHAR*)szUnicodeResult;
		free(szAnsiSrc);
		free(szAnsiResult);
		free(szUnicodeResult);
	}
	
	return strResult;
}

EmTstr em::EmTstr::Base64Encode( const TCHAR* szSrc )
{
	if(szSrc == NULL || _tcslen(szSrc) == 0){
		return EM_EMPTY_TSZ;
	}
	
	EmTstr strResult;
	int iSrcLen = _tcslen(szSrc);

	if(sizeof(TCHAR) == sizeof(char)){
		char *szAnsiResult = (char*)malloc(iSrcLen * 6 + 6);
		szAnsiResult[0] = 0;
		EmBase64::Encode(szAnsiResult,(const char*)szSrc);
		strResult = (TCHAR*)szAnsiResult;
		free(szAnsiResult);
	}else{
		char *szAnsiResult = (char*)malloc(iSrcLen * 6 + 6);
		szAnsiResult[0] = 0;
		wchar_t *szUnicodeResult = (wchar_t*)malloc(iSrcLen * 6 + 6);
		szUnicodeResult[0] = 0;
		char *szAnsiSrc = (char*)malloc(iSrcLen * 6 + 6);
		szAnsiSrc[0] = 0;
		
		EmCharSet::Tcs2Mbs(szAnsiSrc,szSrc);		
		EmBase64::Encode(szAnsiResult,szAnsiSrc);
		EmCharSet::Mbs2Tcs(szUnicodeResult,szAnsiResult);
		
		strResult = (const TCHAR*)szUnicodeResult;
		free(szAnsiSrc);
		free(szAnsiResult);
		free(szUnicodeResult);
	}
	
	return strResult;
}

EmTstr em::EmTstr::Base64Decode( const TCHAR* szSrc )
{
	if(szSrc == NULL || _tcslen(szSrc) == 0){
		return EM_EMPTY_TSZ;
	}
	
	EmTstr strResult;
	int iSrcLen = _tcslen(szSrc);

	if(sizeof(TCHAR) == sizeof(char)){
		char *szAnsiResult = (char*)malloc(iSrcLen * 6 + 6);
		szAnsiResult[0] = 0;
		EmBase64::Decode(szAnsiResult,(const char*)szSrc);
		strResult = (TCHAR*)szAnsiResult;
		free(szAnsiResult);
	}else{
		char *szAnsiResult = (char*)malloc(iSrcLen * 6 + 6);
		szAnsiResult[0] = 0;
		wchar_t *szUnicodeResult = (wchar_t*)malloc(iSrcLen * 6 + 6);
		szUnicodeResult[0] = 0;
		char *szAnsiSrc = (char*)malloc(iSrcLen * 6 + 6);
		szAnsiSrc[0] = 0;
		
		EmCharSet::Tcs2Mbs(szAnsiSrc,szSrc);		
		EmBase64::Decode(szAnsiResult,szAnsiSrc);
		EmCharSet::Mbs2Tcs(szUnicodeResult,szAnsiResult);
		
		strResult = (const TCHAR*)szUnicodeResult;
		free(szAnsiSrc);
		free(szAnsiResult);
		free(szUnicodeResult);
	}

	return strResult;
}






