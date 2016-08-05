#include "stdafx.h"
#include "../inc/EmCs.h"
#include "../inc/EmTsz.h"
#include "../inc/EmCharSet.h"
#include "../inc/EmUtil.h"
#include "../inc/EmMd5.h"
#include "../inc/EmUrl.h"
#include "../inc/EmBase64.h"
#include "../inc/EmTstr.h"

using namespace em;

CString em::EmCs::Int2Str(int num)
{
	CString cs;
	cs.Format(_T("%d"),num);
	return cs;
}


CString em::EmCs::Long2Str( INT64 num )
{
	CString cs;
	TCHAR szStr[20] = {0};
	_i64tot(num,szStr,10);
	cs = szStr;
	return cs;
}

int em::EmCs::Str2Int(LPCTSTR szSrc)
{
	int num = 0;
	num = _ttoi(szSrc);
	return num;
}

INT64 em::EmCs::Str2Long( LPCTSTR szSrc )
{
	INT64 num = 0;
	num = _ttoi64(szSrc);
	return num;
}

double em::EmCs::Str2Float(LPCTSTR szSrc)
{
	double num = 0;
	char mbsSrc[50];
	EmCharSet::Tcs2Mbs(mbsSrc,szSrc);
	num = atof(mbsSrc);
	return num;
}

CString em::EmCs::Float2Str(double fNum)
{
	CString csDst;
	csDst.Format(_T("%f"),fNum);
	return csDst;
}

int em::EmCs::Char2Int(TCHAR chSrc)
{
	int num = 0;
	char ch = EmCharSet::Tc2Ch(chSrc);
	num = (int)ch - 48;
	return num;

}

CString em::EmCs::Char2Str(TCHAR chSrc)
{
	CString csDest(chSrc,1);
	return csDest;
}

TCHAR em::EmCs::Str2Char(LPCTSTR szSrc)
{
	if(szSrc != NULL)
	{
		return *szSrc;
	}
	return 0;
}

CString em::EmCs::Lower(LPCTSTR szSrc)
{
	CString csDest = szSrc;
	csDest.MakeLower();
	return csDest;
}

CString em::EmCs::Upper(LPCTSTR szSrc)
{
	CString csDest = szSrc;
	csDest.MakeUpper();
	return csDest;
}

CString em::EmCs::Reverse(LPCTSTR szSrc)
{
	CString csDest = szSrc;
	csDest.MakeReverse();
	return csDest;
}

int em::EmCs::CountChar(LPCTSTR szSrc, TCHAR chFind)
{
	return EmTsz::CountChar(szSrc,chFind);
}

int em::EmCs::CountStr(LPCTSTR szSrc, LPCTSTR szFind)
{
	return EmTsz::CountStr(szSrc,szFind);
}

CString em::EmCs::Repeat(LPCTSTR szSrc,int iCount)
{
	if(szSrc == NULL)
	{
		return _T("");
	}

	CString csDest;
	int iDestLen = _tcslen(szSrc) * iCount;
	TCHAR* szDest = (TCHAR*)malloc(iDestLen * sizeof(TCHAR) + 2);
	memset(szDest,0,iDestLen * sizeof(TCHAR) + 2);
	
	EmTsz::Repeat(szDest,szSrc,iCount);

	csDest = szDest;
	EM_FREE_NULL(szDest);

	return csDest;
}

CString em::EmCs::Prefix(LPCTSTR szSrc, LPCTSTR szPrefix)
{
	if(szSrc == NULL || szPrefix == NULL)
	{
		return _T("");
	}
	
	CString csDest;
	int iDestLen = _tcslen(szSrc) * _tcslen(szPrefix);
	TCHAR* szDest = (TCHAR*)malloc(iDestLen * sizeof(TCHAR) + 2);
	memset(szDest,0,iDestLen * sizeof(TCHAR) + 2);
	_tcscpy(szDest,szSrc);

	EmTsz::Prefix(szDest,szPrefix);
	
	csDest = szDest;
	EM_FREE_NULL(szDest);
	
	return csDest;
}

CString em::EmCs::PadLeft(LPCTSTR szSrc, TCHAR chItem, int iTotalLen)
{
	if(szSrc == NULL)
	{
		return _T("");
	}
	
	CString csDest;
	int iDestLen = iTotalLen;
	TCHAR* szDest = (TCHAR*)malloc(iDestLen * sizeof(TCHAR) + 2);
	memset(szDest,0,iDestLen * sizeof(TCHAR) + 2);
	_tcscpy(szDest,szSrc);
	
	EmTsz::PadLeft(szDest,chItem,iTotalLen);
	
	csDest = szDest;
	EM_FREE_NULL(szDest);
	
	return csDest;
}

CString em::EmCs::PadRight(LPCTSTR szSrc, TCHAR chItem, int iTotalLen)
{
	if(szSrc == NULL)
	{
		return _T("");
	}
	
	CString csDest;
	int iDestLen = iTotalLen;
	TCHAR* szDest = (TCHAR*)malloc(iDestLen * sizeof(TCHAR) + 2);
	memset(szDest,0,iDestLen * sizeof(TCHAR) + 2);
	_tcscpy(szDest,szSrc);
	
	EmTsz::PadRight(szDest,chItem,iTotalLen);
	
	csDest = szDest;
	EM_FREE_NULL(szDest);
	
	return csDest;
}

CString em::EmCs::SubStr(LPCTSTR szSrc, int iStart, int iSubLen /* = -1 */)
{
	
	if(szSrc == NULL || iSubLen == 0)
	{
		return _T("");
	}
	
	if(iSubLen == -1)
	{
		iSubLen = _tcslen(szSrc) - iStart;
	}	

	CString csResult;

	int iDestLen = iSubLen;
	TCHAR* szDest = (TCHAR*)malloc(iDestLen * sizeof(TCHAR) + 2);
	memset(szDest,0,iDestLen * sizeof(TCHAR) + 2);

	EmTsz::SubStr(szDest,szSrc,iStart,iSubLen);
	
	csResult = szDest;
	EM_FREE_NULL(szDest);
	
	return csResult;
}

int em::EmCs::Split(std::vector<CString> &rVec, LPCTSTR szSrc, LPCTSTR szDelimiter)
{
	CString csSrc = szSrc;
	int nStart = 0;
	int nCount = 0;
	int nPos = 0;
	int nDelimiterLen = _tcslen(szDelimiter);
	int nSrcLen = _tcslen(csSrc);
	rVec.clear();
	while(true)
	{
		nPos = csSrc.Find(szDelimiter,nStart);
		if(nPos == -1)
		{
			break;
		}
		nCount++;
		rVec.push_back(SubStr(csSrc,nStart,nPos - nStart));
		nStart = nPos + nDelimiterLen;
	}
	nCount++;
	rVec.push_back(SubStr(csSrc,nStart,-1));
	
	return nCount;
}

CString em::EmCs::Join(const std::vector<CString> &rVec, LPCTSTR szDelimiter)
{
	CString csDest;

	UINT nVecLen = rVec.size();
	UINT i = 0;
	for(i = 0; i < nVecLen; i++)
	{
		if(i > 0)
		{
			csDest += szDelimiter;
		}
		csDest += rVec[i];
	}

	return csDest;
}

int em::EmCs::XsvToMap(std::map<CString,CString> &rMap, LPCTSTR szSrc, LPCTSTR szSplit1, LPCTSTR szSplit2)
{
	int count = 0;
	std::vector<CString> vec1;
	std::vector<CString> vec2;
	int len1 = 0;
	int len2 = 0;
	int i = 0;

	CString csTemp1;
	CString csTemp2Left;
	CString csTemp2Right;

	rMap.clear();

	Split(vec1,szSrc,szSplit1);
	len1 = vec1.size();
	for(i = 0; i < len1; i++)
	{
		csTemp1 = vec1[i];
		
		vec2.clear();
		Split(vec2,csTemp1,szSplit2);
		len2 = vec2.size();
		if(len2 < 2)
		{
			continue;
		}
		csTemp2Left = vec2[0];
		csTemp2Right = vec2[1];
		rMap[csTemp2Left] = csTemp2Right;

		count++;
	}
	return count;
}

CString em::EmCs::MapToXsv(const std::map<CString,CString> &rMap, LPCTSTR szSplit1, LPCTSTR szSplit2)
{
	CString csDest;
	std::map<CString,CString>::const_iterator it;
	for(it = rMap.begin(); it != rMap.end(); it++ )
	{
		if(it != rMap.begin())
		{
			csDest += szSplit1;
		}

		csDest += it->first;
		csDest += szSplit2;
		csDest += it->second;
	}

	return csDest;
}

CString em::EmCs::AddSlashes(LPCTSTR szSrc, TCHAR chFind)
{
	if(szSrc == NULL)
	{
		return _T("");
	}
	
	CString csResult;
	int iDestLen = _tcslen(szSrc) * 2;
	TCHAR* szDest = (TCHAR*)malloc(iDestLen * sizeof(TCHAR) + 2);
	memset(szDest,0,iDestLen * sizeof(TCHAR) + 2);
	_tcscpy(szDest,szSrc);

	EmTsz::AddSlashes(szDest,chFind);
	
	csResult = szDest;
	EM_FREE_NULL(szDest);
	
	return csResult;
}

CString em::EmCs::StripSlashes(LPCTSTR szSrc)
{
	if(szSrc == NULL)
	{
		return _T("");
	}
	
	CString csResult;
	int iDestLen = _tcslen(szSrc);
	TCHAR* szDest = (TCHAR*)malloc(iDestLen * sizeof(TCHAR) + 2);
	memset(szDest,0,iDestLen * sizeof(TCHAR) + 2);
	_tcscpy(szDest,szSrc);

	EmTsz::StripSlashes(szDest);
	
	csResult = szDest;
	EM_FREE_NULL(szDest);
	
	return csResult;
}

CString em::EmCs::Dec2Bin(LPCTSTR szSrc)
{
	if(szSrc == NULL)
	{
		return _T("");
	}
	
	CString csResult;
	TCHAR szDest[255] = {0};
	_tcscpy(szDest,szSrc);

	EmTsz::Dec2Bin(szDest);
	
	csResult = szDest;
	return csResult;
}

CString em::EmCs::Bin2Dec(LPCTSTR szSrc)
{
	if(szSrc == NULL)
	{
		return _T("");
	}
	
	CString csResult;
	TCHAR szDest[255] = {0};
	_tcscpy(szDest,szSrc);

	EmTsz::Bin2Dec(szDest);
	
	csResult = szDest;
	return csResult;
}

CString em::EmCs::Hex2Dec(LPCTSTR szSrc)
{
	if(szSrc == NULL)
	{
		return _T("");
	}
	
	CString csResult;
	TCHAR szDest[255] = {0};
	_tcscpy(szDest,szSrc);

	EmTsz::Hex2Dec(szDest);
	
	csResult = szDest;
	return csResult;
}

CString em::EmCs::Dec2Hex(LPCTSTR szSrc)
{
	if(szSrc == NULL)
	{
		return _T("");
	}
	
	CString csResult;
	TCHAR szDest[255] = {0};
	_tcscpy(szDest,szSrc);
	
	EmTsz::Dec2Hex(szDest);
	
	csResult = szDest;
	return csResult;
}

CString em::EmCs::Bin2Hex(LPCTSTR szSrc)
{
	if(szSrc == NULL)
	{
		return _T("");
	}
	
	CString csResult;
	TCHAR szDest[255] = {0};
	_tcscpy(szDest,szSrc);
	
	EmTsz::Bin2Hex(szDest);
	
	csResult = szDest;
	return csResult;
}

CString em::EmCs::Hex2Bin(LPCTSTR szSrc)
{
	if(szSrc == NULL)
	{
		return _T("");
	}
	
	CString csResult;
	TCHAR szDest[255] = {0};
	_tcscpy(szDest,szSrc);
	
	EmTsz::Hex2Bin(szDest);
	
	csResult = szDest;
	return csResult;
}

int em::EmCs::Find(LPCTSTR szSrc, LPCTSTR szFind)
{
	return EmTsz::Find(szSrc,szFind);
}

int em::EmCs::FindFrom(LPCTSTR szSrc, LPCTSTR szFind, int nStart)
{
	return EmTsz::FindFrom(szSrc,szFind,nStart);
}

int em::EmCs::FindReverse(LPCTSTR szSrc, LPCTSTR szFind)
{
	return EmTsz::FindReverse(szSrc,szFind);
}


int em::EmCs::FindChar(LPCTSTR szSrc, TCHAR chFind)
{
	return EmTsz::FindChar(szSrc,chFind,0);
}

CString em::EmCs::TrimLeft(LPCTSTR szSrc)
{
	CString csSrc = szSrc;
	csSrc.TrimLeft();
	return csSrc;
}

CString em::EmCs::TrimRight(LPCTSTR szSrc)
{
	CString csSrc = szSrc;
	csSrc.TrimRight();
	return csSrc;
}

CString em::EmCs::Trim(LPCTSTR szSrc)
{
	CString csSrc = szSrc;
	csSrc.TrimLeft();
	csSrc.TrimRight();
	return csSrc;
}

CString em::EmCs::Cut(LPCTSTR szSrc, UINT nStart, UINT nLen)
{
	if(szSrc == NULL)
	{
		return _T("");
	}
	
	CString csResult;
	int iDestLen = _tcslen(szSrc);
	TCHAR* szDest = (TCHAR*)malloc(iDestLen * sizeof(TCHAR) + 2);
	memset(szDest,0,iDestLen * sizeof(TCHAR) + 2);
	_tcscpy(szDest,szSrc);

	EmTsz::Cut(szDest,nStart,nLen);
	
	csResult = szDest;
	EM_FREE_NULL(szDest);
	
	return csResult;
}

CString em::EmCs::Insert(LPCTSTR szSrc, LPCTSTR szInsert, UINT nStart)
{
	if(szSrc == NULL || szInsert == NULL)
	{
		return _T("");
	}
	
	CString csResult;
	int iDestLen = _tcslen(szSrc) + _tcslen(szInsert);
	TCHAR* szDest = (TCHAR*)malloc(iDestLen * sizeof(TCHAR) + 2);
	memset(szDest,0,iDestLen * sizeof(TCHAR) + 2);
	_tcscpy(szDest,szSrc);

	EmTsz::Insert(szDest,szInsert,nStart);
	
	csResult = szDest;
	EM_FREE_NULL(szDest);
	
	return csResult;
}

CString em::EmCs::Splice(LPCTSTR szSrc, LPCTSTR szInsert, UINT nStart, UINT nLen)
{
	if(szSrc == NULL || szInsert == NULL)
	{
		return _T("");
	}
	
	CString csResult;
	int iDestLen = _tcslen(szSrc) + _tcslen(szInsert);
	TCHAR* szDest = (TCHAR*)malloc(iDestLen * sizeof(TCHAR) + 2);
	memset(szDest,0,iDestLen * sizeof(TCHAR) + 2);
	_tcscpy(szDest,szSrc);

	EmTsz::Splice(szDest,szInsert,nStart,nLen);
	
	csResult = szDest;
	EM_FREE_NULL(szDest);
	
	return csResult;
}

CString em::EmCs::Replace(LPCTSTR szSrc, LPCTSTR szFind, LPCTSTR szReplace)
{
	if(szSrc == NULL || szFind == NULL || szReplace == NULL)
	{
		return _T("");
	}
	
	CString csResult;
	int iDestLen = 0;
	int iCountStr = EmTsz::CountStr(szSrc,szFind);
	int iPlusLen = _tcslen(szReplace) - _tcslen(szFind) ;
	iPlusLen = iPlusLen > 0 ? iPlusLen : -iPlusLen;
	iPlusLen *= iCountStr;
	iDestLen = _tcslen(szSrc) + iPlusLen;
	TCHAR* szDest = (TCHAR*)malloc(iDestLen * sizeof(TCHAR) + 2);
	memset(szDest,0,iDestLen * sizeof(TCHAR) + 2);
	_tcscpy(szDest,szSrc);
	
	EmTsz::Replace(szDest,szFind,szReplace);
	
	csResult = szDest;
	EM_FREE_NULL(szDest);
	
	return csResult;
}

CString em::EmCs::FormatNum(LPCTSTR szSrc, UINT nLenAfterPoint)
{
	if(szSrc == NULL)
	{
		return _T("");
	}
	
	CString csResult;
	TCHAR szDest[255] = {0};
	_tcscpy(szDest,szSrc);
	
	EmTsz::FormatNum(szDest,nLenAfterPoint);
	
	csResult = szDest;
	return csResult;
}

CString em::EmCs::Md5(LPCTSTR szSrc)
{
	return EmTstr::Md5(szSrc).Sz();
}

CString em::EmCs::UrlEncodeUtf8(LPCTSTR szSrc,bool bEncodeAll)
{
	CString csDest;
	
	UINT srcLen = _tcslen(szSrc);
	
	int iAllocUtf8Src = srcLen * 9 + 2;
	char *bufUtf8Src = (char*)malloc(iAllocUtf8Src);
	memset(bufUtf8Src,0,iAllocUtf8Src);
	

	int iAllocUtf8Dest = srcLen * 9 + 2;
	char *bufUtf8Dest = (char*)malloc(iAllocUtf8Dest);;
	memset(bufUtf8Dest,0,iAllocUtf8Dest);
	

	int iAllocTcsDest = iAllocUtf8Dest + 2;
	TCHAR *bufTcsDest = (TCHAR*)malloc(iAllocTcsDest);;
	memset(bufTcsDest,0,iAllocTcsDest);
	

	EmCharSet::TcsToUtf8(bufUtf8Src,szSrc);
	
	EmUrl::Encode(bufUtf8Dest,bufUtf8Src,bEncodeAll);
	
	EmCharSet::Utf8ToTcs(bufTcsDest,bufUtf8Dest);
	

	csDest = bufTcsDest;
	
	EM_FREE_NULL( bufUtf8Src);
	EM_FREE_NULL( bufUtf8Dest);
	EM_FREE_NULL( bufTcsDest);
	
	return csDest;
}

CString em::EmCs::UrlDecodeUtf8(LPCTSTR szSrc,bool bEncodeAll)
{
	CString csDest;
	UINT srcLen = _tcslen(szSrc);

	int iAllocUtf8Src = srcLen * 9 + 2;
	char *bufUtf8Src = (char*)malloc(iAllocUtf8Src);;
	memset(bufUtf8Src,0,iAllocUtf8Src);

	int iAllocUtf8Dest = srcLen * 9 + 2;
	char *bufUtf8Dest = (char*)malloc(iAllocUtf8Dest);;
	memset(bufUtf8Dest,0,iAllocUtf8Dest);
	
	int iAllocTcsDest = iAllocUtf8Dest + 2;
	TCHAR *bufTcsDest = (TCHAR*)malloc(iAllocTcsDest);;
	memset(bufTcsDest,0,iAllocTcsDest);
	
	EmCharSet::TcsToUtf8(bufUtf8Src,szSrc);
	
	EmUrl::Decode(bufUtf8Dest,bufUtf8Src,bEncodeAll);
	
	EmCharSet::Utf8ToTcs(bufTcsDest,bufUtf8Dest);
	
	csDest = bufTcsDest;
	
	EM_FREE_NULL(  bufUtf8Src);
	EM_FREE_NULL(  bufUtf8Dest);
	EM_FREE_NULL(  bufTcsDest);
	
	return csDest;
}

CString em::EmCs::UrlEncodeAnsi(LPCTSTR szSrc,bool bEncodeAll)
{
	CString csDest;
	
	UINT srcLen = _tcslen(szSrc);
	
	int iAllocAnsiSrc = srcLen * 9 + 2;
	char *bufAnsiSrc = (char*)malloc(iAllocAnsiSrc);;
	memset(bufAnsiSrc,0,iAllocAnsiSrc);
	
	int iAllocAnsiDest = srcLen * 9 + 2;
	char *bufAnsiDest = (char*)malloc(iAllocAnsiDest);;
	memset(bufAnsiDest,0,iAllocAnsiDest);
	
	int iAllocTcsDest = iAllocAnsiDest + 1;
	TCHAR *bufTcsDest = (TCHAR*)malloc(iAllocTcsDest);;
	memset(bufTcsDest,0,iAllocTcsDest);
	
	EmCharSet::Tcs2Mbs(bufAnsiSrc,szSrc);
	
	EmUrl::Encode(bufAnsiDest,bufAnsiSrc,bEncodeAll);
	
	
	EmCharSet::Mbs2Tcs(bufTcsDest,bufAnsiDest);
	
	
	csDest = bufTcsDest;
	
	EM_FREE_NULL(  bufAnsiSrc);
	EM_FREE_NULL(  bufAnsiDest);
	EM_FREE_NULL(  bufTcsDest);
	
	return csDest;
}

CString em::EmCs::UrlDecodeAnsi(LPCTSTR szSrc,bool bEncodeAll)
{
	CString csDest;
	UINT srcLen = _tcslen(szSrc);
	
	int iAllocAnsiSrc = srcLen * 9 + 2;
	char *bufAnsiSrc = (char*)malloc(iAllocAnsiSrc);
	memset(bufAnsiSrc,0,iAllocAnsiSrc);
	
	int iAllocAnsiDest = srcLen * 9 + 2;
	char *bufAnsiDest = (char*)malloc(iAllocAnsiDest);
	memset(bufAnsiDest,0,iAllocAnsiDest);
	
	int iAllocTcsDest = iAllocAnsiDest + 1;
	TCHAR *bufTcsDest = (TCHAR*)malloc(iAllocTcsDest);
	memset(bufTcsDest,0,iAllocTcsDest);
	
	EmCharSet::Tcs2Mbs(bufAnsiSrc,szSrc);
	
	EmUrl::Decode(bufAnsiDest,bufAnsiSrc,bEncodeAll);
	
	EmCharSet::Mbs2Tcs(bufTcsDest,bufAnsiDest);
	
	csDest = bufTcsDest;
	
	EM_FREE_NULL(  bufAnsiSrc);
	EM_FREE_NULL(  bufAnsiDest);
	EM_FREE_NULL(  bufTcsDest);
	
	return csDest;
}

CString em::EmCs::Base64Encode(LPCTSTR szSrc)
{
	return EmTstr::Base64Encode(szSrc).Sz();
}

CString em::EmCs::Base64Decode(LPCTSTR szSrc)
{
	return EmTstr::Base64Decode(szSrc).Sz();
}

CString em::EmCs::EscapeEncode(LPCTSTR szSrc)
{
	CString csDest = szSrc;
	
	csDest.Replace(_T("="), _T("_ESCEQUAL"));
	csDest.Replace(_T("#"), _T("_ESCPOUND"));
	csDest.Replace(_T("|"), _T("_ESCBAR"));
	csDest.Replace(_T("&"), _T("_ESCAMP"));
	csDest.Replace(_T(":"), _T("_ESCCOLON"));
	csDest.Replace(_T(","), _T("_ESCCOMMA"));
	csDest.Replace(_T("<"), _T("_ESCLT"));
	csDest.Replace(_T(">"), _T("_ESCGT"));
	csDest.Replace(_T("\r"), _T("_ESCRTN"));
	csDest.Replace(_T("\n"), _T("_ESCLN"));
	csDest.Replace(_T("/"), _T("_ESCSLASH"));
	csDest.Replace(_T("\\"), _T("_ESCBSLASH"));
	csDest.Replace(_T("'"), _T("_ESCQUOTE"));
	csDest.Replace(_T("\""), _T("_ESCDQUOTE"));
	csDest.Replace(_T("%"), _T("_ESCPCT"));

	return csDest;
}


CString em::EmCs::EscapeDecode(LPCTSTR szSrc)
{
	CString csDest = szSrc;
	
	csDest.Replace(_T("_ESCEQUAL"), _T("="));
	csDest.Replace(_T("_ESCPOUND"), _T("#"));
	csDest.Replace(_T("_ESCBAR"), _T("|"));
	csDest.Replace(_T("_ESCAMP"), _T("&"));
	csDest.Replace(_T("_ESCCOLON"), _T(":"));
	csDest.Replace(_T("_ESCCOMMA"), _T(","));
	csDest.Replace(_T("_ESCLT"), _T("<"));
	csDest.Replace(_T("_ESCGT"), _T(">"));
	csDest.Replace(_T("_ESCRTN"), _T("\n"));
	csDest.Replace(_T("_ESCLN"), _T("\n"));
	csDest.Replace(_T("_ESCSLASH"), _T("/"));
	csDest.Replace(_T("_ESCBSLASH"), _T("\\"));
	csDest.Replace(_T("_ESCQUOTE"), _T("'"));
	csDest.Replace(_T("_ESCDQUOTE"), _T("\""));
	csDest.Replace(_T("_ESCPCT"), _T("%"));
	
	return csDest;
}

std::string em::EmCs::Mfc2Stl( LPCTSTR szSrc )
{
	std::string strDest;
	if(_tcslen(szSrc) == 0)
	{
		return strDest;
	}

	int nAlloc = _tcslen(szSrc) * 2 + 4;
	char * pMbs = (char*)malloc(nAlloc);
	memset(pMbs,0,nAlloc);
	EmCharSet::Tcs2Mbs(pMbs,szSrc);
	strDest = pMbs;

	EM_FREE_NULL(  pMbs);

	return strDest;
}

CString em::EmCs::Stl2Mfc( std::string strSrc )
{
	CString csDest;
	if(strSrc.size() == 0)
	{
		return csDest;
	}

	int nAlloc = strSrc.size() + 4;
	const char *pMbs = strSrc.c_str();
	TCHAR *pTcs = (TCHAR*)malloc(nAlloc);;
	memset(pTcs,0,nAlloc);
	EmCharSet::Mbs2Tcs(pTcs,pMbs);
	csDest = pTcs;

	return csDest;
}

CString em::EmCs::FileDirFromPath( LPCTSTR szSrc )
{
	CString csDest = szSrc;
	csDest.Replace(_T("\\"),_T("/"));
	
	int nSlashPos = FindReverse(csDest,_T("/"));
	
	if(nSlashPos == -1)
	{
		csDest.Empty();
		return csDest;
	}
	csDest = SubStr(csDest,0,nSlashPos + 1);
	return csDest;
}

CString em::EmCs::FileNameFromPath( LPCTSTR szSrc )
{
	CString csDest = szSrc;
	csDest.Replace(_T("\\"),_T("/"));
	int nSlashPos = FindReverse(csDest,_T("/"));
	if(nSlashPos == -1)
	{
		return csDest;
	}
	csDest = SubStr(csDest,nSlashPos + 1,-1);
	return csDest;
}

CString em::EmCs::FileTitleFromPath( LPCTSTR szSrc )
{
	CString csDest = FileNameFromPath(szSrc);
	int nSlashPos = FindReverse(csDest,_T("."));
	if(nSlashPos == -1)
	{
		return csDest;
	}
	csDest = SubStr(csDest,0,nSlashPos);
	return csDest;
}

CString em::EmCs::FileExtFromPath( LPCTSTR szSrc )
{
	CString csDest = szSrc;
	int nSlashPos = FindReverse(csDest,_T("."));
	if(nSlashPos == -1)
	{
		csDest.Empty();
		return csDest;
	}
	csDest = SubStr(csDest,nSlashPos + 1,-1);
	return csDest;
}

CString EmCs::FileFixDir(LPCTSTR szDir)
{
	CString csResult = EmCs::Replace(szDir,_T("\\"),_T("/"));
	if(csResult.Right(0) != _T("/")){
		csResult += _T("/");
	}
	return csResult;
}

bool em::EmCs::IsLimitedIn( LPCTSTR szSrc, LPCTSTR szLimited )
{
	return EmTsz::IsLimitedIn(szSrc,szLimited);
}

INT64 em::EmCs::IpTextToNum( LPCTSTR szIpText )
{
	if(!EmTsz::IsIp(szIpText))
	{
		return 0;
	}

	INT64 iTotal = 0;
	std::vector<CString> vecParts;
	Split(vecParts,szIpText,_T("."));

	if(vecParts.size() < 4)
	{
		return 0;
	}
	
	INT64 iPart0 = Str2Long(vecParts[0]);
	INT64 iPart1 = Str2Long(vecParts[1]);
	INT64 iPart2 = Str2Long(vecParts[2]);
	INT64 iPart3 = Str2Long(vecParts[3]);
	
	iTotal += (iPart0 << 24);
	iTotal += (iPart1 << 16);
	iTotal += (iPart2 << 8);
	iTotal += iPart3;
	
	return iTotal;
}

CString em::EmCs::IpNumToText( INT64 iIpNum )
{
	INT64 iTotal = iIpNum;
	
	INT64 iPart0 = (iTotal >> 24);
	INT64 iPart1 = (iTotal >> 16) & 0x00FF;
	INT64 iPart2 = (iTotal >>  8) & 0x0000FF;
	INT64 iPart3 = (iTotal >>  0) & 0x000000FF;
	
	CString csPart0 = Long2Str(iPart0);
	CString csPart1 = Long2Str(iPart1);
	CString csPart2 = Long2Str(iPart2);
	CString csPart3 = Long2Str(iPart3);
	
	CString csTotal = _T("");
	csTotal += csPart0 + _T(".");
	csTotal += csPart1 + _T(".");
	csTotal += csPart2 + _T(".");
	csTotal += csPart3;

	return csTotal;
}

CString em::EmCs::UrlGetProtocol( LPCTSTR szSrc )
{
	CString csResult;
	CString csSrc = szSrc;
	if(csSrc.GetLength() == 0)
	{
		return _T("");
	}

	int iColonSlashPos = FindFrom(szSrc,_T(":/"),0);
	if(iColonSlashPos == -1)
	{
		csResult = _T("");
	}
	else if(iColonSlashPos > 0)
	{
		csResult = SubStr(szSrc,0,iColonSlashPos);
	}

	return csResult;
}

CString em::EmCs::UrlGetHost( LPCTSTR szSrc )
{
	CString csResult;
	CString csSrc = szSrc;
	int iLen = csSrc.GetLength();
	if(iLen == 0)
	{
		return csResult;
	}

	int iSlashPos1 = FindFrom(szSrc,_T("//"),0);
	int iSlashPos1Offset = 0;
	if(iSlashPos1 == -1)
	{
		iSlashPos1Offset = 0;
	} 
	else
	{
		if( csSrc.GetAt(iSlashPos1 + 2) == _T('/'))
		{
			iSlashPos1Offset = iSlashPos1 + 3;
		}else
		{
			iSlashPos1Offset = iSlashPos1 + 2;
		}
	}
	
	if(iSlashPos1Offset == iLen -1)
	{
		return csResult;
	}
	
	int iColonPos = FindFrom(szSrc,_T(":"),iSlashPos1Offset);
	if(iColonPos != -1)
	{
		csResult = SubStr(szSrc,iSlashPos1Offset,iColonPos - iSlashPos1Offset);
		return csResult;
	}

	int iSlashPos2 = FindFrom(szSrc,_T("/"),iSlashPos1Offset);
	if(iSlashPos2 == iSlashPos1Offset)
	{
		return csResult;
	}
	
	if(iSlashPos2 == -1)
	{
		csResult = SubStr(szSrc,iSlashPos1Offset, - 1);
	}else
	{
		csResult = SubStr(szSrc,iSlashPos1Offset,iSlashPos2 - iSlashPos1Offset);
	}
	
	return csResult;
}

USHORT em::EmCs::UrlGetPort( LPCTSTR szSrc )
{
	USHORT nResult;
	CString csSrc = szSrc;
	if(csSrc.GetLength() == 0)
	{
		return 0;
	}

	int iColonCount = CountStr(szSrc,_T(":"));
	if(iColonCount == 0)
	{
		return 80;
	}
	else
	{
		int iColonSlashPos = FindFrom(szSrc,_T("://"),0);
		int iColonSlashOffset = 0;
		if(iColonSlashPos == -1)
		{
			iColonSlashOffset = 0;
		}else
		{
			iColonSlashOffset = iColonSlashPos + 3;
		}

		int iColonSecondPos = FindFrom(szSrc,_T(":"),iColonSlashOffset);
		if(iColonSecondPos == -1)
		{
			return 80;
		}

		int iSlashSecondPos = FindFrom(szSrc,_T("/"),iColonSlashOffset);
		CString csPortStr;
		if(iSlashSecondPos == -1)
		{
			csPortStr = SubStr(szSrc,iColonSecondPos + 1,-1);
			if(EmTsz::IsNumber(csPortStr))
			{
				return Str2Int(csPortStr);
			}else
			{
				return 0;
			}
		}
		else if(iSlashSecondPos < iColonSecondPos)
		{
			return 80;
		}else
		{
			csPortStr = SubStr(szSrc,iColonSecondPos + 1, iSlashSecondPos - iColonSecondPos -1);
			if(EmTsz::IsNumber(csPortStr))
			{
				return Str2Int(csPortStr);
			}else
			{
				return 0;
			}
		}
	}
	return nResult;
}

CString em::EmCs::UrlGetPath( LPCTSTR szSrc )
{
	CString csResult;
	CString csSrc = szSrc;
	if(csSrc.GetLength() == 0)
	{
		return _T("");
	}
	int iColonSlashPos = FindFrom(szSrc,_T("://"),0);
	int iNextStartPos = 0;
	if(iColonSlashPos != -1)
	{
		iNextStartPos = iColonSlashPos + 3;
	}
	
	if(csSrc.GetAt(iNextStartPos) == _T('/'))
	{
		iNextStartPos += 1;
	}
	
	int iSecondSlashPos = FindFrom(szSrc,_T("/"),iNextStartPos);
	if(iSecondSlashPos == -1)
	{
		csResult = _T("/");
	}else
	{
		
		csResult = SubStr(szSrc,iSecondSlashPos, -1);
	}
	int nQuestionMarkPos = FindFrom(csResult,_T("?"),0);
	if(nQuestionMarkPos > 0)
	{
		csResult = SubStr(csResult,0,nQuestionMarkPos);
	}
	return csResult;
}

CString em::EmCs::UrlGetParam( LPCTSTR szSrc )
{
	CString csResult;
	CString csSrc = szSrc;
	if(csSrc.GetLength() == 0)
	{
		return _T("");
	}

	int nQuestionMarkPos = Find(szSrc,_T("?"));
	if(nQuestionMarkPos == -1)
	{
		return _T("");
	}
	else
	{
		if(nQuestionMarkPos == csSrc.GetLength() - 1)
		{
			return _T("");
		}
		csResult = SubStr(szSrc,nQuestionMarkPos + 1, -1);
	}
	return csResult;
}

CString em::EmCs::UrlGetPathWithParam( LPCTSTR szSrc )
{
	CString csResult;
	CString csSrc = szSrc;
	if(csSrc.GetLength() == 0)
	{
		return _T("");
	}
	int iColonSlashPos = FindFrom(szSrc,_T("://"),0);
	int iNextStartPos = 0;
	
	if(iColonSlashPos != -1)
	{
		iNextStartPos = iColonSlashPos + 3;
	}

	if(csSrc.GetAt(iNextStartPos) == _T('/'))
	{
		iNextStartPos += 1;
	}

	int iSecondSlashPos = FindFrom(szSrc,_T("/"),iNextStartPos);
	
	if(iSecondSlashPos == -1)
	{
		csResult = _T("/");
	}else
	{
		csResult = SubStr(szSrc,iSecondSlashPos, -1);
	}
	return csResult;
}


CString em::EmCs::FromUtf8( const char* szUtf8 )
{
	TCHAR *tcs = NULL;
	int iSrcLen = 0;
	CString csResult;

	if(szUtf8 == NULL)
	{
		return _T("");
	}

	iSrcLen = strlen(szUtf8);
	if(iSrcLen == 0)
	{
		return _T("");
	}

	tcs = (TCHAR *)malloc((iSrcLen + 1) * sizeof(TCHAR));
	memset(tcs,0,(iSrcLen + 1) * sizeof(TCHAR));
	EmCharSet::Utf8ToTcs(tcs,szUtf8);
	csResult = tcs;
	EM_FREE_NULL(tcs);
	return csResult;

}

std::string em::EmCs::ToUtf8( LPCTSTR szSrc )
{
	std::string strResult;
	char* mbs = NULL;
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

	mbs = (char*)malloc(iSrcLen * 3 + 1);
	memset(mbs,0,iSrcLen * 3 + 1);
	EmCharSet::TcsToUtf8(mbs,szSrc);
	strResult = mbs;
	EM_FREE_NULL(mbs);
	return strResult;
}

CString em::EmCs::HexIncrease( LPCTSTR szSrc )
{
	CString csResult;

	if(_tcslen(szSrc) > 255)
	{
		return csResult;
	}

	TCHAR szSrcWritable[255] = {0};
	_tcscpy(szSrcWritable,szSrc);
	EmTsz::HexIncrease(szSrcWritable);
	csResult = szSrcWritable;
	return csResult;
}

CString em::EmCs::Delimit( LPCTSTR szSrc, int iUnitLen, TCHAR chDelimiter )
{
	if(szSrc == NULL || _tcslen(szSrc) == 0)
	{
		return _T("");
	}

	TCHAR *szCopy = (TCHAR*)malloc((_tcslen(szSrc) * 2 + 1) * sizeof(TCHAR) );
	memset(szCopy,0,(_tcslen(szSrc) * 2 + 1) * sizeof(TCHAR));
	_tcscpy(szCopy,szSrc);
	EmTsz::Delimit(szCopy,iUnitLen,chDelimiter);
	CString csResult = szCopy;
	EM_FREE_NULL(szCopy);
	return csResult;
}

CString em::EmCs::FromAnsi( const char* szAnsi )
{
	CString csResult;

	if(szAnsi != NULL
		&& strlen(szAnsi) > 0)
	{
		TCHAR *tcs = (TCHAR*)malloc(strlen(szAnsi) * 2 + 2);
		memset(tcs,0,strlen(szAnsi) * 2 + 2);

		EmCharSet::Mbs2Tcs(tcs,szAnsi);

		csResult = tcs;

		EM_FREE_NULL(tcs);
	}

	return csResult;
}

std::string em::EmCs::ToAnsi( LPCTSTR szSrc )
{
	std::string strResult;
	
	if(szSrc != NULL
		&& _tcslen(szSrc) > 0)
	{
		char *mbs = (char*)malloc(_tcslen(szSrc) * 2 + 2);
		memset(mbs,0,_tcslen(szSrc) * 2 + 2);
		
		EmCharSet::Tcs2Mbs(mbs,szSrc);
		
		strResult = mbs;
		
		EM_FREE_NULL(mbs);
	}
	
	return strResult;
}

int em::EmCs::ListInBuffer( std::vector<CString>& rList, LPCTSTR szBuffer, int iBufSize )
{
	rList.clear();
	if(iBufSize == 0){
		return 0;
	}
	int i = 0;
	int iTempSize = 0;
	TCHAR *szTemp = (TCHAR*)malloc((iBufSize + 1) * sizeof(TCHAR));
	memset(szTemp,0,(iBufSize + 1) * sizeof(TCHAR));
	
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









