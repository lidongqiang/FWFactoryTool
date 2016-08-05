#include "stdafx.h"
#include "../inc/EmStlStr.h"
#include "../inc/EmSz.h"
#include "../inc/EmUtil.h"
#include "../inc/EmUrl.h"
#include "../inc/EmMd5.h"
#include "../inc/EmBase64.h"
#include "../inc/EmCharSet.h"
#include "../inc/EmStr.h"
#include "../inc/EmHandy.h"

using namespace std;
using namespace em;

std::string em::EmStlStr::Int2Str( int iNum )
{
	string strResult;
	char szDest[50] = {0};
	sprintf(szDest,"%d",iNum);
	strResult = szDest;
	return strResult;
}

std::string em::EmStlStr::Long2Str( __int64 iNum )
{
	string strResult;
	char szDest[50] = {0};
	_i64toa(iNum,szDest,10);
	strResult = szDest;
	return strResult;
}

int em::EmStlStr::Str2Int( const char* szSrc )
{
	return atoi(szSrc);
}

__int64 em::EmStlStr::Str2Long( const char* szSrc )
{
	if(!IsInt(szSrc)){
		return 0;
	}
	return _atoi64(szSrc);
}

double em::EmStlStr::Str2Float( const char* szSrc )
{
	if(!IsNumber(szSrc)){
		return 0;
	}
	return atof(szSrc);
}

std::string em::EmStlStr::Float2Str( double fNum )
{
	string strResult;
	char szDest[100] = {0};
	sprintf(szDest,"%f",fNum);
	strResult = szDest;
	return strResult;
}

int em::EmStlStr::Char2Int( char chSrc )
{
	return (int)chSrc - 0x30;
}

std::string em::EmStlStr::Char2Str( char chSrc )
{
	string strResult;
	char szDest[2] = {0};
	szDest[0] = chSrc;
	strResult = szDest;
	return strResult;
}

char em::EmStlStr::Str2Char( const char* szSrc )
{
	if(szSrc == NULL){
		return 0;
	}
	return szSrc[0];
}

std::string em::EmStlStr::Lower( const char* szSrc )
{
	if(szSrc == NULL || strlen(szSrc) == 0){
		return EM_EMPTY_SZ;
	}

	std::string strResult;
	char *szCopy = (char*)malloc(strlen(szSrc) + 1);
	strcpy(szCopy,szSrc);

	EmSz::Lower(szCopy);

	strResult = szCopy;
	free(szCopy);

	return strResult;
}

std::string em::EmStlStr::Upper( const char* szSrc )
{
	if(szSrc == NULL || strlen(szSrc) == 0){
		return EM_EMPTY_SZ;
	}

	std::string strSrc = szSrc;
	char *szCopy = (char*)malloc(strlen(szSrc) + 1);
	strcpy(szCopy,szSrc);
	
	EmSz::Upper(szCopy);
	
	strSrc = szCopy;
	free(szCopy);
	return strSrc;
}

std::string em::EmStlStr::Reverse( const char* szSrc )
{
	if(szSrc == NULL || strlen(szSrc) == 0){
		return EM_EMPTY_SZ;
	}

	std::string strSrc = szSrc;
	char *szCopy = (char*)malloc(strlen(szSrc) + 1);
	strcpy(szCopy,szSrc);

	EmSz::Reverse(szCopy);

	strSrc = szCopy;
	free(szCopy);
	return strSrc;
}

bool em::EmStlStr::Equal( const char* szLeft, const char* szRight )
{
	return EmSz::Equal(szLeft,szRight);
}

bool em::EmStlStr::EqualNoCase( const char* szLeft, const char* szRight)
{
	return EmSz::EqualNoCase(szLeft,szRight);
}

unsigned int em::EmStlStr::CountChar( const char* szSrc, char chFind )
{
	return EmSz::CountChar(szSrc,chFind);
}

unsigned int em::EmStlStr::CountStr( const char* szSrc, const char* szFind )
{
	return EmSz::CountStr(szSrc,szFind);
}

std::string em::EmStlStr::Prefix( const char* szSrc,const char* szPrefix )
{
	if(szPrefix == NULL){
		szPrefix = EM_EMPTY_SZ;
	}

	if(szSrc == NULL){
		szSrc = EM_EMPTY_SZ;
	}

	std::string strSrc = szSrc;
	char *szCopy = (char*)malloc(strlen(szSrc) + 1);
	strcpy(szCopy,szSrc);

	EmSz::Prefix(szCopy,szPrefix);

	strSrc = szCopy;
	free(szCopy);
	return strSrc;
}

std::string em::EmStlStr::PadLeft( const char* szSrc, char chPad, int iTotalLen )
{
	if(szSrc == NULL){
		szSrc = EM_EMPTY_SZ;
	}

	std::string strSrc = szSrc;
	char *szCopy = (char*)malloc(strlen(szSrc) + 1);
	strcpy(szCopy,szSrc);

	EmSz::PadLeft(szCopy,chPad,iTotalLen);

	free(szCopy);
	return strSrc;
}

std::string em::EmStlStr::SubStr( const char* szSrc, unsigned int iStart, int iSubLen )
{
	if(szSrc == NULL || strlen(szSrc) == 0){
		return EM_EMPTY_SZ;
	}

	string strResult;
	char *szCopy = (char*)malloc(strlen(szSrc) + 1);

	EmSz::SubStr(szCopy,szSrc,iStart,iSubLen);
	
	strResult = szCopy;
	free(szCopy);
	return strResult;
}

std::string em::EmStlStr::Repeat( const char* szSrc,int iCount )
{
	if(szSrc == NULL || strlen(szSrc) == 0){
		return EM_EMPTY_SZ;
	}

	string strResult;

	char *szCopy = (char*)malloc(strlen(szSrc) + 1);

	EmSz::Repeat(szCopy,szSrc,iCount);
	
	strResult = szCopy;
	free(szCopy);
	return strResult;

}

std::string em::EmStlStr::AddSlashes( const char* szSrc, char chFind )
{
	if(szSrc == NULL || strlen(szSrc) == 0){
		return EM_EMPTY_SZ;
	}

	std::string strSrc = szSrc;
	char *szCopy = (char*)malloc(strlen(szSrc) * 2 + 1);
	strcpy(szCopy,szSrc);

	EmSz::AddSlashes(szCopy,chFind);
	
	strSrc = szCopy;
	free(szCopy);
	return strSrc;
}

std::string em::EmStlStr::StripSlashes( const char* szSrc )
{
	if(szSrc == NULL || strlen(szSrc) == 0){
		return EM_EMPTY_SZ;
	}

	std::string strSrc = szSrc;
	char *szCopy = (char*)malloc(strlen(szSrc) * 2 + 1);
	strcpy(szCopy,szSrc);

	EmSz::StripSlashes(szCopy);

	strSrc = szCopy;
	free(szCopy);
	return strSrc;
}

std::string em::EmStlStr::Dec2Bin( const char* szSrc )
{
	if(szSrc == NULL || strlen(szSrc) == 0){
		return EM_EMPTY_SZ;
	}

	std::string strSrc = szSrc;
	char szCopy[128] = {0};
	strcpy(szCopy,szSrc);

	EmSz::Dec2Bin(szCopy);

	strSrc = szCopy;
	return strSrc;
}

std::string em::EmStlStr::Bin2Dec( const char* szSrc )
{
	if(szSrc == NULL || strlen(szSrc) == 0){
		return EM_EMPTY_SZ;
	}

	std::string strSrc = szSrc;
	char szCopy[128] = {0};
	strcpy(szCopy,szSrc);

	EmSz::Bin2Dec(szCopy);

	strSrc = szCopy;
	return strSrc;
}

std::string em::EmStlStr::Hex2Dec( const char* szSrc )
{
	if(szSrc == NULL || strlen(szSrc) == 0){
		return EM_EMPTY_SZ;
	}

	std::string strSrc = szSrc;
	char szCopy[128] = {0};
	strcpy(szCopy,szSrc);

	EmSz::Hex2Dec(szCopy);

	strSrc = szCopy;
	return strSrc;
}

std::string em::EmStlStr::Dec2Hex( const char* szSrc )
{
	if(szSrc == NULL || strlen(szSrc) == 0){
		return EM_EMPTY_SZ;
	}

	std::string strSrc = szSrc;
	char szCopy[128] = {0};
	strcpy(szCopy,szSrc);

	EmSz::Dec2Hex(szCopy);

	strSrc = szCopy;
	return strSrc;
}

std::string em::EmStlStr::Bin2Hex( const char* szSrc )
{
	if(szSrc == NULL || strlen(szSrc) == 0){
		return EM_EMPTY_SZ;
	}
	
	std::string strSrc = szSrc;
	strSrc = Bin2Dec(strSrc.c_str());
	strSrc = Dec2Hex(strSrc.c_str());
	return strSrc;
}

std::string em::EmStlStr::Hex2Bin( const char* szSrc )
{
	if(szSrc == NULL || strlen(szSrc) == 0){
		return EM_EMPTY_SZ;
	}

	std::string strSrc = szSrc;
	strSrc = Hex2Dec(strSrc.c_str());
	strSrc = Dec2Bin(strSrc.c_str());
	return strSrc;
}

int em::EmStlStr::FindChar(const char* szSrc, char chFind, unsigned int iFromPos )
{
	return EmSz::FindChar(szSrc,chFind,iFromPos);
}

int em::EmStlStr::FindFrom( const char* szSrc, const char* szFind, unsigned int iFromPos )
{
	return EmSz::FindFrom(szSrc,szFind,iFromPos);
}

int em::EmStlStr::Find( const char* szSrc, const char* szFind )
{
	return EmSz::Find(szSrc,szFind);
}

int em::EmStlStr::FindReverse( const char* szSrc, const char* szFind )
{
	return EmSz::FindReverse(szSrc,szFind);
}

string em::EmStlStr::Replace(const char* szSrc, const char* szFind, const char* szReplace) 
{
	if(szSrc == NULL || strlen(szSrc) == 0){
		return EM_EMPTY_SZ;
	}

	if(szFind == NULL || strlen(szFind) == 0){
		return NULL;
	}

	if(szReplace == NULL){
		szReplace = EM_EMPTY_SZ;
	}

	string strSrc;
	int iSrcLen = strlen(szSrc);
	int iFindLen = strlen(szFind);
	int iReplaceLen = strlen(szReplace);
	int iDiffLen = EmUtil::Abs(iFindLen - iReplaceLen) + 1;

	int iAllocSize = iSrcLen * iDiffLen + 1;
	char *szCopy = (char*)malloc(iAllocSize);
	strcpy(szCopy,szSrc);

	EmSz::Replace(szCopy,szFind,szReplace);

	strSrc = szCopy;
	free(szCopy);

	return strSrc;
}

std::string em::EmStlStr::TrimLeft( const char* szSrc )
{
	if(szSrc == NULL || strlen(szSrc) == 0){
		return EM_EMPTY_SZ;
	}

	std::string strSrc = szSrc;
	char *szCopy = (char*)malloc(strlen(szSrc) + 1);
	strcpy(szCopy,szSrc);

	EmSz::TrimLeft(szCopy);

	strSrc = szCopy;
	free(szCopy);
	return strSrc;
}

std::string em::EmStlStr::TrimRight( const char* szSrc )
{
	if(szSrc == NULL || strlen(szSrc) == 0){
		return EM_EMPTY_SZ;
	}

	std::string strSrc;
	char *szCopy = (char*)malloc(strlen(szSrc) + 1);
	strcpy(szCopy,szSrc);


	EmSz::TrimRight(szCopy);
	
	strSrc = szCopy;
	free(szCopy);

	return strSrc;
}

std::string em::EmStlStr::Trim( const char* szSrc )
{
	if(szSrc == NULL || strlen(szSrc) == 0){
		return EM_EMPTY_SZ;
	}

	std::string strSrc = szSrc;


	strSrc = TrimLeft(strSrc.c_str());
	strSrc = TrimRight(strSrc.c_str());

	return strSrc;
}

std::string em::EmStlStr::FormatNum( const char* szSrc, unsigned int nLenAfterPoint )
{
	if(szSrc == NULL){
		szSrc = EM_EMPTY_SZ;
	}
	
	std::string strSrc;
	char szCopy[255] = {0};
	strcpy(szCopy,szSrc);

	EmSz::FormatNum(szCopy,nLenAfterPoint);

	strSrc = szCopy;
	return strSrc;
}

std::string em::EmStlStr::Cut( const char* szSrc, unsigned int nStart, unsigned int nLen )
{
	if(szSrc == NULL || strlen(szSrc) == 0){
		return EM_EMPTY_SZ;
	}

	std::string strSrc;
	char *szCopy = (char*)malloc(strlen(szSrc) + 1);
	strcpy(szCopy,szSrc);

	EmSz::Cut(szCopy,nStart,nLen);

	strSrc = szCopy;
	free(szCopy);
	return strSrc;
}

std::string em::EmStlStr::Insert( const char* szSrc, const char* szInsert, unsigned int nStart )
{
	if(szSrc == NULL){
		szSrc = EM_EMPTY_SZ;
	}

	if(szInsert == NULL){
		szInsert = EM_EMPTY_SZ;
	}

	std::string strSrc;
	char *szCopy = (char*)malloc(strlen(szSrc) + 1);
	strcpy(szCopy,szSrc);

	EmSz::Insert(szCopy,szInsert,nStart);

	strSrc = szCopy;
	free(szCopy);
	return strSrc;
}

std::string em::EmStlStr::Splice( const char* szSrc, const char* szInsert, unsigned int nStart, unsigned int nLen )
{
	if(szSrc == NULL){
		szSrc = EM_EMPTY_SZ;
	}
	
	if(szInsert == NULL){
		szInsert = EM_EMPTY_SZ;
	}

	std::string strSrc;
	char *szCopy = (char*)malloc(strlen(szSrc) + 1);
	strcpy(szCopy,szSrc);

	EmSz::Splice(szCopy,szInsert,nStart,nLen);

	strSrc = szCopy;
	free(szCopy);
	return strSrc;
}

std::string em::EmStlStr::UrlEncode( const char* szSrc, bool bEncodeAll )
{
	if(szSrc == NULL || strlen(szSrc) == 0){
		return "";
	}

	string strResult;
	char *szCopy = (char*)malloc(strlen(szSrc) * 6 + 6);
	szCopy[0] = 0;

	EmUrl::Encode(szCopy,szSrc,bEncodeAll);

	strResult = szCopy;
	free(szCopy);
	return strResult;

}

std::string em::EmStlStr::UrlDecode( const char* szSrc, bool bEncodeAll )
{
	if(szSrc == NULL || strlen(szSrc) == 0){
		return "";
	}

	string strResult;
	char *szCopy = (char*)malloc(strlen(szSrc) * 6 + 6);
	szCopy[0] = 0;
	
	EmUrl::Decode(szCopy,szSrc,bEncodeAll);
	
	strResult = szCopy;
	free(szCopy);
	return strResult;
}

std::string em::EmStlStr::Md5( const char* szSrc )
{
	if(szSrc == NULL){
		szSrc = EM_EMPTY_SZ;
	}

	string strResult;
	char szCopy[33] = {0};
	
	EmMd5::Encode(szCopy,szSrc);
	
	strResult = szCopy;
	return strResult;
}

std::string em::EmStlStr::Base64Encode( const char* szSrc )
{
	if(szSrc == NULL){
		szSrc = EM_EMPTY_SZ;
	}

	string strResult;
	char *szCopy = (char*)malloc(strlen(szSrc) * 4 + 4);
	szCopy[0] = 0;
	
	EmBase64::Encode(szCopy,szSrc);
	
	strResult = szCopy;
	free(szCopy);
	return strResult;
}

std::string em::EmStlStr::Base64Decode( const char* szSrc )
{
	if(szSrc == NULL){
		szSrc = EM_EMPTY_SZ;
	}

	string strResult;
	char *szCopy = (char*)malloc(strlen(szSrc) * 4 + 4);
	szCopy[0] = 0;
	
	EmBase64::Decode(szCopy,szSrc);
	
	strResult = szCopy;
	free(szCopy);
	return strResult;
}

__int64 em::EmStlStr::IpTextToNum( const char* szIpText )
{
	if(szIpText == NULL || strlen(szIpText) == 0){
		return 0;
	}

	return EmSz::IpTextToNum(szIpText);
}

std::string em::EmStlStr::IpNumToText( __int64 iIpNum )
{
	string strResult;
	char szCopy[20] = {0};
	
	EmSz::IpNumToText(szCopy, iIpNum);
	
	strResult = szCopy;
	return strResult;
}

unsigned short em::EmStlStr::UrlGetPort( const char* szSrc )
{
	return EmSz::UrlGetPort(szSrc);
}

std::string em::EmStlStr::UrlGetProtocol( const char* szSrc )
{
	if(szSrc == NULL || strlen(szSrc) == 0){
		return EM_EMPTY_SZ;
	}

	string strResult;
	char szCopy[20] = {0};
	
	EmSz::UrlGetProtocol(szCopy, szSrc);
	
	strResult = szCopy;
	return strResult;
}

std::string em::EmStlStr::UrlGetHost( const char* szSrc )
{
	if(szSrc == NULL || strlen(szSrc) == 0){
		return EM_EMPTY_SZ;
	}

	string strResult;
	char szCopy[4096] = {0};
	
	EmSz::UrlGetHost(szCopy, szSrc);
	
	strResult = szCopy;
	return strResult;
}

std::string em::EmStlStr::UrlGetPath( const char* szSrc )
{
	if(szSrc == NULL || strlen(szSrc) == 0){
		return EM_EMPTY_SZ;
	}

	string strResult;
	char szCopy[4096] = {0};
	
	EmSz::UrlGetPath(szCopy, szSrc);
	
	strResult = szCopy;
	return strResult;
}

std::string em::EmStlStr::UrlGetParam( const char* szSrc )
{
	if(szSrc == NULL || strlen(szSrc) == 0){
		return EM_EMPTY_SZ;
	}

	string strResult;
	char szCopy[4096] = {0};
	
	EmSz::UrlGetParam(szCopy, szSrc);
	
	strResult = szCopy;
	return strResult;
}

std::string em::EmStlStr::UrlGetPathWithParam( const char* szSrc )
{
	if(szSrc == NULL || strlen(szSrc) == 0){
		return EM_EMPTY_SZ;
	}
	
	string strResult;
	char szCopy[4096] = {0};
	
	EmSz::UrlGetPathWithParam(szCopy, szSrc);
	
	strResult = szCopy;
	return strResult;
}

int em::EmStlStr::Split( std::vector<std::string> &rVec, const char* szSrc, const char* szDelimiter )
{
	rVec.clear();
	if(szSrc == NULL 
		|| szDelimiter == NULL 
		|| strlen(szSrc) == 0
		|| strlen(szDelimiter) == 0
	){
		rVec.push_back(EM_EMPTY_SZ);
		return 1;
	}
	int nStart = 0;
	int nCount = 0;
	int nPos = 0;
	int nDelimiterLen = strlen(szDelimiter);
	int nSrcLen = strlen(szSrc);
	char szTemp[40960] = {0};
	
	rVec.clear();

	while(true)
	{
		nPos = em::EmSz::FindFrom(szSrc,szDelimiter,nStart);
		if(nPos == -1)
		{
			break;
		}
		nCount++;
		em::EmSz::SubStr(szTemp,szSrc,nStart,nPos - nStart);
		rVec.push_back(szTemp);
		nStart = nPos + nDelimiterLen;
	}
	nCount++;
	em::EmSz::SubStr(szTemp,szSrc,nStart,-1);
	rVec.push_back(szTemp);
	
	return nCount;
}

std::string em::EmStlStr::Join(const std::vector<std::string> &rVec, const char* szDelimiter )
{
	if(szDelimiter == NULL){
		szDelimiter = EM_EMPTY_SZ;
	}
	string strResult;
	char szCopy[40960] = {0};
	unsigned int nVecLen = rVec.size();
	unsigned int i = 0;
	std::string strTemp;
	szCopy[0] = 0;
	for(i = 0; i < nVecLen; i++)
	{
		if(i > 0)
		{
			strcat(szCopy,szDelimiter);
		}
		strTemp = rVec[i];
		strcat(szCopy,strTemp.c_str());
	}
	strResult = szCopy;
	return strResult;
}

int em::EmStlStr::XsvToMap( std::map<std::string,std::string> &rMap, const char* szSrc, const char* szSplit1, const char* szSplit2 )
{
	rMap.clear();
	if(szSrc == NULL
		|| szSplit1 == NULL 
		|| szSplit2 == NULL
		|| strlen(szSrc) == 0
		|| strlen(szSplit1) == 0
		|| strlen(szSplit2) == 0
	){
		return 0;
	}

	int count = 0;
	std::vector<std::string> vec1;
	std::vector<std::string> vec2;
	int len1 = 0;
	int len2 = 0;
	int i = 0;
	//int j = 0;
	std::string strTemp1;
	std::string strTemp2Left;
	std::string strTemp2Right;
	
	rMap.clear();
	
	Split(vec1,szSrc,szSplit1);
	len1 = vec1.size();
	for(i = 0; i < len1; i++)
	{
		strTemp1 = vec1[i];
		
		vec2.clear();
		Split(vec2,strTemp1.c_str(),szSplit2);
		len2 = vec2.size();
		if(len2 < 2)
		{
			continue;
		}
		strTemp2Left = vec2[0];
		strTemp2Right = vec2[1];
		rMap[strTemp2Left] = strTemp2Right;
		
		count++;
	}
	return count;
}

std::string em::EmStlStr::MapToXsv( const std::map<std::string,std::string> &rMap, const char* szSplit1, const char* szSplit2 )
{
	if(szSplit1 == NULL){
		szSplit1 = EM_EMPTY_SZ;
	}

	if(szSplit2 == NULL){
		szSplit2 = EM_EMPTY_SZ;
	}

	string strResult;
	char szDest[40960] = {0};

	std::map<std::string,std::string>::const_iterator it;

	szDest[0] = 0;
	for(it = rMap.begin(); it != rMap.end(); it++ )
	{
		if(it != rMap.begin())
		{
			strcat(szDest,szSplit1);
		}
		
		strcat(szDest,it->first.c_str());
		strcat(szDest,szSplit2);
		strcat(szDest,it->second.c_str());
	}
	
	strResult = szDest;
	return strResult;
}

int em::EmStlStr::ListInBuffer( std::vector<std::string>& rList, const char* szBuffer, int iBufSize )
{
	rList.clear();
	if(iBufSize == 0){
		return 0;
	}
	int i = 0;
	int iTempSize = 0;
	char szTemp[40960] = {0};

	
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
	
	return rList.size();
}

std::string em::EmStlStr::FileDirFromPath( const char* szSrc )
{
	if(szSrc == NULL || strlen(szSrc) == 0){
		return EM_EMPTY_SZ;
	}

	char szDest[1024] = {0};
	EmSz::FileDirFromPath(szDest,szSrc);
	std::string strDest = szDest;
	return strDest;
}

std::string em::EmStlStr::FileNameFromPath( const char* szSrc )
{
	if(szSrc == NULL || strlen(szSrc) == 0){
		return EM_EMPTY_SZ;
	}

	char szDest[1024] = {0};
	EmSz::FileNameFromPath(szDest,szSrc);
	std::string strDest = szDest;
	return strDest;
}

std::string em::EmStlStr::FileTitleFromPath( const char* szSrc )
{
	if(szSrc == NULL || strlen(szSrc) == 0){
		return EM_EMPTY_SZ;
	}

	char szDest[1024] = {0};
	EmSz::FileTitleFromPath(szDest,szSrc);
	std::string strDest = szDest;
	return strDest;
}

std::string em::EmStlStr::FileExtFromPath( const char* szSrc )
{
	if(szSrc == NULL || strlen(szSrc) == 0){
		return EM_EMPTY_SZ;
	}

	char szDest[1024] = {0};
	EmSz::FileExtFromPath(szDest,szSrc);
	std::string strDest = szDest;
	return strDest;
}

std::string em::EmStlStr::FileFixDir(const char* szSrc)
{
	if(szSrc == NULL || strlen(szSrc) == 0){
		return EM_EMPTY_SZ;
	}

	char szDest[1024] = {0};
	EmSz::FileFixDir(szDest,szSrc);
	std::string strDest = szDest;
	return strDest;
}

std::string em::EmStlStr::Utf8ToAnsi( const char* szSrc )
{
	if(szSrc == NULL || strlen(szSrc) == 0){
		return "";
	}
	std::string strResult;
	int iSrcLen = strlen(szSrc);
	char* ansiResult = (char*)malloc(iSrcLen * 1 + 1);
	memset(ansiResult,0,iSrcLen * 1 + 1);

	EmCharSet::Utf8ToAnsi(ansiResult,szSrc);
	strResult = ansiResult;
	free(ansiResult);
	return strResult;
}

std::string em::EmStlStr::AnsiToUtf8( const char* szSrc )
{
	if(szSrc == NULL || strlen(szSrc) == 0){
		return "";
	}
	std::string strResult;
	int iSrcLen = strlen(szSrc);
	char* utf8Result = (char*)malloc(iSrcLen * 3 + 1);
	memset(utf8Result,0,iSrcLen * 3 + 1);

	EmCharSet::AnsiToUtf8(utf8Result,szSrc);
	strResult = utf8Result;
	free(utf8Result);
	return strResult;
}

std::string em::EmStlStr::UnicodeToUtf8( const wchar_t* szSrc )
{
	if(szSrc == NULL || wcslen(szSrc) == 0){
		return "";
	}
	std::string strResult;
	int iSrcLen = wcslen(szSrc);
	char* utf8Result = (char*)malloc(iSrcLen * 3 + 1);
	memset(utf8Result,0,iSrcLen * 3 + 1);

	EmCharSet::UnicodeToUtf8(utf8Result,szSrc);
	strResult = utf8Result;
	free(utf8Result);
	return strResult;
}

std::string em::EmStlStr::TcsToUtf8( const TCHAR* szSrc )
{
	if(szSrc == NULL || _tcslen(szSrc) == 0){
		return "";
	}
	std::string strResult;
	int iSrcLen = _tcslen(szSrc);
	char* utf8Result = (char*)malloc(iSrcLen * 3 + 1);
	memset(utf8Result,0,iSrcLen * 3 + 1);

	EmCharSet::TcsToUtf8(utf8Result,szSrc);
	strResult = utf8Result;
	free(utf8Result);
	return strResult;
}

bool em::EmStlStr::IsDigit( char chSrc )
{
	return EmSz::IsDigit(chSrc);
}

bool em::EmStlStr::IsLetter( char chSrc )
{
	return EmSz::IsLetter(chSrc);
}

bool em::EmStlStr::IsInt( const char* szSrc )
{
	return EmSz::IsInt(szSrc);
}

bool em::EmStlStr::IsNumber( const char* szSrc )
{
	return EmSz::IsNumber(szSrc);
}

bool em::EmStlStr::IsDomain( const char* szSrc )
{
	return EmSz::IsDomain(szSrc);
}

bool em::EmStlStr::IsEmail( const char* szSrc )
{
	return EmSz::IsEmail(szSrc);
}

bool em::EmStlStr::IsIp( const char* szSrc )
{
	return EmSz::IsIp(szSrc);
}

bool em::EmStlStr::IsUrl( const char* szSrc )
{
	return EmSz::IsUrl(szSrc);
}

bool em::EmStlStr::IsAsc( const char* szSrc )
{
	return EmSz::IsAsc(szSrc);
}

bool em::EmStlStr::IsLimitedIn( const char* szSrc, const char* szLimited )
{
	return EmSz::IsLimitedIn(szSrc,szLimited);
}

bool em::EmStlStr::IsDateTime( const char* szSrc )
{
	return EmSz::IsDateTime(szSrc);
}

std::string em::EmStlStr::Str2Hex( const char* szSrc )
{
	if(szSrc == NULL || strlen(szSrc) == 0){
		return EM_EMPTY_SZ;
	}

	std::string strResult;
	char *szCopy = (char*)malloc(strlen(szSrc) * 2 + 1);
	szCopy[0] = 0;

	EmSz::StrToHex(szCopy,szSrc);

	strResult = szCopy;
	free(szCopy);
	return strResult;
}





