#ifndef _h_EmTstr
#define _h_EmTstr

#include "EmBase.h"
#include "EmStr.h"
#include <tchar.h>

#include <vector>
#include <wchar.h>

namespace em
{

class EmTstr
{

public:
	EmTstr();
	EmTstr(const TCHAR* szSrc);
	EmTstr(const EmTstr& xCopy);
	virtual ~EmTstr();

	int Length() const;
	int Capacity() const;
	const TCHAR* Sz() const;
	int Compare(const TCHAR* szSrc) const;
	bool Equals(const TCHAR* szSrc) const;
	TCHAR At(int iIndex) const;

	EmTstr& Resize(int iSize);
	EmTstr& Append(const TCHAR* szSrc);
	EmTstr& Append(const TCHAR* bufSrc, int iSize);
	EmTstr& Prepend(const TCHAR* szSrc);
	void Empty();
	EmTstr& Copy(const TCHAR* szSrc);

	TCHAR operator [] (int iIndex) const;
// 	operator const wchar_t* () const;
// 	operator const char* () const;
	EmTstr& operator = (const TCHAR* szSrc);
	EmTstr& operator = (const EmTstr& xOther);
	EmTstr& operator += (const TCHAR* szSrc); 
	
	friend bool operator == ( const EmTstr& xLeft, const EmTstr& xRight )
	{
		return xLeft.Equals(xRight.Sz());	
	}

	friend bool operator < (const EmTstr& xLeft, const EmTstr& xRight)
	{
		return _tcscmp(xLeft.Sz(), xRight.Sz()) < 0;
	}
	
	friend bool operator > (const EmTstr& xLeft, const EmTstr& xRight)
	{
		return _tcscmp(xLeft.Sz(), xRight.Sz()) > 0;
	}

	friend EmTstr operator + ( const TCHAR* szLeft, const EmTstr& xRight )
	{
		EmTstr xStr;
		xStr.Append(szLeft);
		xStr.Append(xRight.Sz());
		return xStr;
	}

	friend EmTstr operator + ( const EmTstr& xLeft, const TCHAR* szRight )
	{
		EmTstr xStr;
		xStr.Append(xLeft.Sz());
		xStr.Append(szRight);
		return xStr;
	}

	friend EmTstr operator + ( const EmTstr& xLeft, const EmTstr& xRight )
	{
		EmTstr xStr;
		xStr.Append(xLeft.Sz());
		xStr.Append(xRight.Sz());
		return xStr;
	}



public:
	static EmTstr Int2Str(int iNum);
	static EmTstr Long2Str(INT64 iNum);
	static int Str2Int(const TCHAR* szSrc);
	static INT64 Str2Long(const TCHAR* szSrc);
	static EmTstr Float2Str(double fNum);
	static int Char2Int(TCHAR chSrc);
	static EmTstr Char2Str(TCHAR chSrc);
	static TCHAR Str2Char(const TCHAR* szSrc);

	static EmTstr FromUtf8(const char* szSrc);
	static EmStr ToUtf8(const TCHAR* szSrc);
	static EmTstr FromAnsi(const char* szSrc);
	static EmStr ToAnsi(const TCHAR* szSrc);
	
	static EmTstr Lower(TCHAR* szSrc);
	static EmTstr Upper(TCHAR* szSrc);
	static EmTstr Reverse(TCHAR* szSrc);
	static bool Equal(const TCHAR* szLeft, const TCHAR* szRight);
	static bool EqualNoCase(const TCHAR* szLeft, const TCHAR* szRight);

	static unsigned int CountChar(const TCHAR* szSrc, TCHAR chFind);
	static unsigned int CountStr(const TCHAR* szSrc, const TCHAR* szFind);
	
	static EmTstr Prefix(const TCHAR* szSrc, const TCHAR* szPrefix);
	static EmTstr PadLeft(const TCHAR* szSrc, TCHAR chPad, int iTotalLen);
	static EmTstr PadRight(const TCHAR* szSrc, TCHAR chPad, int iTotalLen);
	static EmTstr SubStr(const TCHAR *szSrc, unsigned int iStart, int iSubLen);

	static EmTstr Repeat(const TCHAR* szSrc,int iCount);

	static EmTstr AddSlashes(const TCHAR* szSrc, TCHAR chFind);
	static EmTstr StripSlashes(const TCHAR* szSrc);
	
	static EmTstr Dec2Bin(const TCHAR* szSrc);
	static EmTstr Bin2Dec(const TCHAR* szSrc);
	static EmTstr Hex2Dec(const TCHAR* szSrc);
	static EmTstr Dec2Hex(const TCHAR* szSrc);
	static EmTstr Bin2Hex(const TCHAR* szSrc);
	static EmTstr Hex2Bin(const TCHAR* szSrc);

	static int FindChar(const TCHAR *szSrc, TCHAR chFind, unsigned int iFromPos);
	static int FindFrom(const TCHAR *szSrc, const TCHAR *szFind, unsigned int iFromPos);
	static int Find(const TCHAR *szSrc, const TCHAR *szFind);
	static int FindReverse(const TCHAR* szSrc, const TCHAR* szFind);
	static int FindSafe(const TCHAR* szSrc, const TCHAR* szFind, unsigned int iLimit);
	
	static EmTstr TrimLeft(const TCHAR* szSrc);
	static EmTstr TrimRight(const TCHAR* szSrc);
	static EmTstr Trim(const TCHAR* szSrc);

	static EmTstr FormatNum(const TCHAR* szSrc, unsigned int nLenAfterPoint);

	static EmTstr Replace(const TCHAR* szSrc, const TCHAR* szFind, const TCHAR* szReplace);
	static EmTstr Cut(const TCHAR* szSrc, unsigned int nStart, unsigned int nLen);
	static EmTstr Insert(const TCHAR* szSrc, const TCHAR* szInsert, unsigned int nStart);
	static EmTstr Splice(const TCHAR* szSrc, const TCHAR* szInsert, unsigned int nStart, unsigned int nLen);
	
	static bool IsDigit(TCHAR ch);
	static bool IsLetter(TCHAR ch);
	static bool IsInt(const TCHAR* szSrc);
	static bool IsNumber(const TCHAR* szSrc);
	static bool IsDomain(const TCHAR* szSrc);
	static bool IsEmail(const TCHAR* szSrc);
	static bool IsIp(const TCHAR* szSrc);
	static bool IsUrl(const TCHAR* szSrc);
	static bool IsAsc(const TCHAR* szSrc);
	static bool IsLimitedIn(const TCHAR* szSrc, const TCHAR* szLimited);
	/*yyyy-mm-dd hh:ii:ss*/
	static bool IsDateTime(const TCHAR* szSrc);

	static INT64 IpTextToNum(const TCHAR* szIpText);
	static EmTstr IpNumToText(INT64 iIpNum);

	static EmTstr UrlGetProtocol(const TCHAR* szSrc);
	static EmTstr UrlGetHost(const TCHAR* szSrc);
	static unsigned short UrlGetPort(const TCHAR* szSrc);
	static EmTstr UrlGetPath(const TCHAR* szSrc);
	static EmTstr UrlGetParam(const TCHAR* szSrc);
	static EmTstr UrlGetPathWithParam(const TCHAR* szSrc);

	static EmTstr FileDirFromPath(const TCHAR* szSrc);
	static EmTstr FileNameFromPath(const TCHAR* szSrc);
	static EmTstr FileTitleFromPath(const TCHAR* szSrc);
	static EmTstr FileExtFromPath(const TCHAR* szSrc);
	static EmTstr FileFixDir(const TCHAR* szSrc);

	static EmTstr Md5( const TCHAR* szSrc);
	static EmTstr UrlEncode(const TCHAR* szSrc, bool bEncodeAll);
	static EmTstr UrlDecode(const TCHAR* szSrc, bool bEncodeAll);
	static EmTstr Base64Encode( const TCHAR* szSrc);
	static EmTstr Base64Decode( const TCHAR* szSrc);

	static int ListInBuffer(std::vector<EmTstr>& rList, const TCHAR* szBuffer, int iBufSize);
protected:
	void Init();
	
protected:
	
	TCHAR* m_bufData;
	int m_iLength;
	int m_iCapacity;
};//class EmTstr  

}//namespace em

#endif // #ifndef _EM_STR_H
