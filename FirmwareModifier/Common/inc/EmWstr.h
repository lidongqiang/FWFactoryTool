#ifndef _h_EmWstr
#define _h_EmWstr

#include "EmBase.h"

#include <wchar.h>


namespace em
{

/*
static functions for char* string
	*/
class EmWstr
{

public:
	EmWstr();
	EmWstr(const wchar_t* szSrc);
	EmWstr(const EmWstr& xCopy);
	virtual ~EmWstr();

	int Length() const;
	int Capacity() const;
	const wchar_t* Sz() const;
	int Compare(const wchar_t* szSrc) const;
	bool Equals(const wchar_t* szSrc) const;
	wchar_t At(int iIndex) const;

	EmWstr& Resize(int iSize);
	EmWstr& Append(const wchar_t* szSrc);
	EmWstr& Append(const wchar_t* bufSrc, int iSize);
	EmWstr& Prepend(const wchar_t* szSrc);
	void Empty();
	EmWstr& Copy(const wchar_t* szSrc);

	char operator [] (int iIndex) const;
	operator const wchar_t* () const;
	EmWstr& operator = (const wchar_t* szSrc);
	EmWstr& operator += (const wchar_t* szSrc); 
	
	friend bool operator == ( const EmWstr& xLeft, const EmWstr& xRight )
	{
		return xLeft.Equals(xRight.Sz());	
	}

	friend bool operator == ( const wchar_t* szLeft, const EmWstr& xRight )
	{
		return xRight.Equals(szLeft);	
	}

	friend bool operator == ( const EmWstr& xLeft, const wchar_t* szRight )
	{
		return xLeft.Equals(szRight);	
	}

	friend bool operator < (const EmWstr& xLeft, const EmWstr& xRight)
	{
		return wcscmp(xLeft.Sz(), xRight.Sz()) < 0;
	}

	friend bool operator < (const wchar_t* szLeft, const EmWstr& xRight)
	{
		return wcscmp(szLeft, xRight.Sz()) < 0;
	}

	friend bool operator < (const EmWstr& xLeft, const wchar_t* szRight)
	{
		return wcscmp(xLeft.Sz(), szRight) < 0;
	}
	
	friend bool operator > (const EmWstr& xLeft, const EmWstr& xRight)
	{
		return wcscmp(xLeft.Sz(), xRight.Sz()) > 0;
	}

	friend bool operator > (const wchar_t* szLeft, const EmWstr& xRight)
	{
		return wcscmp(szLeft, xRight.Sz()) > 0;
	}

	friend bool operator > (const EmWstr& xLeft, const wchar_t* szRight)
	{
		return wcscmp(xLeft.Sz(), szRight) > 0;
	}

	friend EmWstr operator + ( const wchar_t* szLeft, const EmWstr& xRight )
	{
		EmWstr xStr;
		xStr.Append(szLeft);
		xStr.Append(xRight.Sz());
		return xStr;
	}

	friend EmWstr operator + ( const EmWstr& xLeft, const wchar_t* szRight )
	{
		EmWstr xStr;
		xStr.Append(xLeft.Sz());
		xStr.Append(szRight);
		return xStr;
	}

	friend EmWstr operator + ( const EmWstr& xLeft, const EmWstr& xRight )
	{
		EmWstr xStr;
		xStr.Append(xLeft.Sz());
		xStr.Append(xRight.Sz());
		return xStr;
	}

public:
	static EmWstr Int2Str(int iNum);
	static EmWstr Long2Str(INT64 iNum);
	static int Str2Int(const wchar_t* szSrc);
	static INT64 Str2Long(const wchar_t* szSrc);
	static EmWstr Float2Str(double fNum);
	static int Char2Int(wchar_t chSrc);
	static EmWstr Char2Str(wchar_t chSrc);
	static wchar_t Str2Char(const wchar_t* szSrc);
	
	static EmWstr Lower(wchar_t* szSrc);
	static EmWstr Upper(wchar_t* szSrc);
	static EmWstr Reverse(wchar_t* szSrc);
	static bool Equal(const wchar_t* szLeft, const wchar_t* szRight);
	static bool EqualNoCase(const wchar_t* szLeft, const wchar_t* szRight);

	static unsigned int CountChar(const wchar_t* szSrc, wchar_t chFind);
	static unsigned int CountStr(const wchar_t* szSrc, const wchar_t* szFind);
	
	static EmWstr Prefix(const wchar_t* szSrc, const wchar_t* szPrefix);
	static EmWstr PadLeft(const wchar_t* szSrc, wchar_t chPad, int iTotalLen);
	static EmWstr PadRight(const wchar_t* szSrc, wchar_t chPad, int iTotalLen);
	static EmWstr SubStr(const wchar_t *szSrc, unsigned int iStart, int iSubLen);

	static EmWstr Repeat(const wchar_t* szSrc,int iCount);

	static EmWstr AddSlashes(const wchar_t* szSrc, wchar_t chFind);
	static EmWstr StripSlashes(const wchar_t* szSrc);
	
	static EmWstr Dec2Bin(const wchar_t* szSrc);
	static EmWstr Bin2Dec(const wchar_t* szSrc);
	static EmWstr Hex2Dec(const wchar_t* szSrc);
	static EmWstr Dec2Hex(const wchar_t* szSrc);
	static EmWstr Bin2Hex(const wchar_t* szSrc);
	static EmWstr Hex2Bin(const wchar_t* szSrc);

	static int FindChar(const wchar_t *szSrc, wchar_t chFind, unsigned int iFromPos);
	static int FindFrom(const wchar_t *szSrc, const wchar_t *szFind, unsigned int iFromPos);
	static int Find(const wchar_t *szSrc, const wchar_t *szFind);
	static int FindReverse(const wchar_t* szSrc, const wchar_t* szFind);
	static int FindSafe(const wchar_t* szSrc, const wchar_t* szFind, unsigned int iLimit);
	
	static EmWstr TrimLeft(const wchar_t* szSrc);
	static EmWstr TrimRight(const wchar_t* szSrc);
	static EmWstr Trim(const wchar_t* szSrc);

	static EmWstr FormatNum(const wchar_t* szSrc, unsigned int nLenAfterPoint);

	static EmWstr Replace(const wchar_t* szSrc, const wchar_t* szFind, const wchar_t* szReplace);
	static EmWstr Cut(const wchar_t* szSrc, unsigned int nStart, unsigned int nLen);
	static EmWstr Insert(const wchar_t* szSrc, const wchar_t* szInsert, unsigned int nStart);
	static EmWstr Splice(const wchar_t* szSrc, const wchar_t* szInsert, unsigned int nStart, unsigned int nLen);
	
	static bool IsDigit(wchar_t ch);
	static bool IsLetter(wchar_t ch);
	static bool IsInt(const wchar_t* szSrc);
	static bool IsNumber(const wchar_t* szSrc);
	static bool IsDomain(const wchar_t* szSrc);
	static bool IsEmail(const wchar_t* szSrc);
	static bool IsIp(const wchar_t* szSrc);
	static bool IsUrl(const wchar_t* szSrc);
	static bool IsAsc(const wchar_t* szSrc);
	static bool IsLimitedIn(const wchar_t* szSrc, const wchar_t* szLimited);
	/*yyyy-mm-dd hh:ii:ss*/
	static bool IsDateTime(const wchar_t* szSrc);

	static INT64 IpTextToNum(const wchar_t* szIpText);
	static EmWstr IpNumToText(INT64 iIpNum);

	static EmWstr UrlGetProtocol(const wchar_t* szSrc);
	static EmWstr UrlGetHost(const wchar_t* szSrc);
	static unsigned short UrlGetPort(const wchar_t* szSrc);
	static EmWstr UrlGetPath(const wchar_t* szSrc);
	static EmWstr UrlGetParam(const wchar_t* szSrc);
	static EmWstr UrlGetPathWithParam(const wchar_t* szSrc);

	static EmWstr FileDirFromPath(const wchar_t* szSrc);
	static EmWstr FileNameFromPath(const wchar_t* szSrc);
	static EmWstr FileTitleFromPath(const wchar_t* szSrc);
	static EmWstr FileExtFromPath(const wchar_t* szSrc);
	static EmWstr FileFixDir(const wchar_t* szSrc);

protected:
	void Init();
	
protected:
	
	wchar_t* m_bufData;
	int m_iLength;
	int m_iCapacity;
};//class EmWstr  

}//namespace em

#endif // #ifndef _EM_STR_H
