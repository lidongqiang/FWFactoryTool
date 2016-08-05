#ifndef _h_EmCs
#define _h_EmCs

#include "EmBase.h"

#include <afx.h>

#include <vector>
#include <map>
#include <string>



namespace em
{
	class EmCs
	{
	public:
		static CString Int2Str(int num);
		static CString Long2Str(INT64 num);
		static int Str2Int(LPCTSTR szSrc);
		static INT64 Str2Long(LPCTSTR szSrc);
		static double Str2Float(LPCTSTR szSrc);
		static CString Float2Str(double fNum);
		static int Char2Int(TCHAR chSrc);
		static CString Char2Str(TCHAR chSrc);
		static TCHAR Str2Char(LPCTSTR szSrc);
		static std::string Mfc2Stl(LPCTSTR szSrc);
		static CString Stl2Mfc(std::string strSrc);
		static CString FromUtf8(const char* szUtf8);
		static std::string ToUtf8(LPCTSTR szSrc);
		static CString FromAnsi(const char* szAnsi);
		static std::string ToAnsi(LPCTSTR szSrc);

		static CString Lower(LPCTSTR szSrc);
		static CString Upper(LPCTSTR szSrc);
		static CString Reverse(LPCTSTR szSrc);

		static int CountChar(LPCTSTR szSrc, TCHAR chFind);
		static int CountStr(LPCTSTR szSrc, LPCTSTR szFind);
		
		static CString Repeat(LPCTSTR szSrc,int iCount);
		static CString Prefix(LPCTSTR szSrc,LPCTSTR szPrefix);
		static CString PadLeft(LPCTSTR szSrc, TCHAR chItem, int iTotalLen);
		static CString PadRight(LPCTSTR szSrc, TCHAR chItem, int iTotalLen);

		static CString SubStr(LPCTSTR szSrc, int iStart, int iSubLen = -1);

		static int Split(std::vector<CString> &rVec, LPCTSTR szSrc, LPCTSTR szDelimiter);
		static CString Join(const std::vector<CString> &rVec, LPCTSTR szDelimiter);
		static int XsvToMap(std::map<CString,CString> &rMap, LPCTSTR szSrc, LPCTSTR szSplit1, LPCTSTR szSplit2);
		static CString MapToXsv(const std::map<CString,CString> &rMap, LPCTSTR szSplit1, LPCTSTR szSplit2);
	
		static CString AddSlashes(LPCTSTR szSrc, TCHAR chFind);
		static CString StripSlashes(LPCTSTR szSrc);

		static CString Dec2Bin(LPCTSTR szSrc);
		static CString Bin2Dec(LPCTSTR szSrc);
		static CString Hex2Dec(LPCTSTR szSrc);
		static CString Dec2Hex(LPCTSTR szSrc);
		static CString Bin2Hex(LPCTSTR szSrc);
		static CString Hex2Bin(LPCTSTR szSrc);

		static int Find(LPCTSTR szSrc, LPCTSTR szFind);
		static int FindFrom(LPCTSTR szSrc, LPCTSTR szFind, int nStart);
		static int FindReverse(LPCTSTR szSrc, LPCTSTR szFind);
		static int FindChar(LPCTSTR szSrc, TCHAR chFind);
		static bool IsLimitedIn(LPCTSTR szSrc, LPCTSTR szLimited);

		static CString TrimLeft(LPCTSTR szSrc);
		static CString TrimRight(LPCTSTR szSrc);
		static CString Trim(LPCTSTR szSrc);

		static CString Cut(LPCTSTR szSrc, UINT nStart, UINT nLen);
		static CString Insert(LPCTSTR szSrc, LPCTSTR szInsert, UINT nStart);
		static CString Splice(LPCTSTR szSrc, LPCTSTR szInsert, UINT nStart, UINT nLen);
		static CString Replace(LPCTSTR szSrc, LPCTSTR szFind, LPCTSTR szReplace);

		static CString FormatNum(LPCTSTR szSrc, UINT nLenAfterPoint);

		static CString UrlEncodeAnsi(LPCTSTR szSrc,bool bEncodeAll);
		static CString UrlDecodeAnsi(LPCTSTR szSrc,bool bEncodeAll);

		static CString UrlEncodeUtf8(LPCTSTR szSrc,bool bEncodeAll);
		static CString UrlDecodeUtf8(LPCTSTR szSrc,bool bEncodeAll);

		static CString Md5(LPCTSTR szSrc);
		static CString Base64Encode(LPCTSTR szSrc);
		static CString Base64Decode(LPCTSTR szSrc);

		static CString EscapeEncode(LPCTSTR szSrc);
		static CString EscapeDecode(LPCTSTR szSrc);

		static CString FileDirFromPath(LPCTSTR szSrc);
		static CString FileNameFromPath(LPCTSTR szSrc);
		static CString FileTitleFromPath(LPCTSTR szSrc);
		static CString FileExtFromPath(LPCTSTR szSrc);
		static CString FileFixDir(LPCTSTR szDir);

		static INT64 IpTextToNum(LPCTSTR szIpText);
		static CString IpNumToText(INT64 iIpNum);
		
		static CString UrlGetProtocol(LPCTSTR szSrc);
		static CString UrlGetHost(LPCTSTR szSrc);
		static USHORT UrlGetPort(LPCTSTR szSrc);
		static CString UrlGetPath(LPCTSTR szSrc);
		static CString UrlGetParam(LPCTSTR szSrc);
		static CString UrlGetPathWithParam(LPCTSTR szSrc);

		static CString HexIncrease(LPCTSTR szSrc);
		static CString Delimit(LPCTSTR szSrc, int iUnitLen, TCHAR chDelimiter);

		static int ListInBuffer(std::vector<CString>& rList, LPCTSTR szBuffer, int iBufSize);
		
	};//class EmCs
	
}//namespace em

#endif //#ifndef _EM_CS_H
