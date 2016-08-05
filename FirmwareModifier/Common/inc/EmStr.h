#ifndef _h_EmStr
#define _h_EmStr

#include "EmBase.h"

#include <string.h>

#include <vector>
#include <string>

namespace em
{

/*
static functions for char* string
	*/
class EmStr
{

public:
	EmStr();
	EmStr(const char* szSrc);
	EmStr(const EmStr& xCopy);
	EmStr(const std::string& xString);
	virtual ~EmStr();

	int Length() const;
	int Capacity() const;
	const char* Sz() const;
	int Compare(const char* szSrc) const;
	bool Equals(const char* szSrc) const;
	char At(int iIndex) const;

	EmStr& Resize(int iSize);
	EmStr& Append(const char* szSrc);
	EmStr& Append(const char* bufSrc, int iSize);
	EmStr& Prepend(const char* szSrc);
	void Empty();
	EmStr& Copy(const char* szSrc);

	char operator [] (int iIndex) const;
	operator const char* () const;
	EmStr& operator = (const char* szSrc);
	EmStr& operator = (const EmStr& xOther);
	EmStr& operator = (const std::string& xString);
	EmStr& operator += (const char* szSrc); 
	
	friend bool operator == ( const EmStr& xLeft, const EmStr& xRight )
	{
		return xLeft.Equals(xRight.Sz());	
	}

	friend bool operator == ( const char* szLeft, const EmStr& xRight )
	{
		return xRight.Equals(szLeft);	
	}

	friend bool operator == ( const EmStr& xLeft, const char* szRight )
	{
		return xLeft.Equals(szRight);	
	}

	friend bool operator < (const EmStr& xLeft, const EmStr& xRight)
	{
		return strcmp(xLeft.Sz(), xRight.Sz()) < 0;
	}
	
	friend bool operator < (const char* szLeft, const EmStr& xRight)
	{
		return strcmp(szLeft, xRight.Sz()) < 0;
	}

	friend bool operator < (const EmStr& xLeft, const char* szRight)
	{
		return strcmp(xLeft.Sz(), szRight) < 0;
	}

	friend bool operator > (const EmStr& xLeft, const EmStr& xRight)
	{
		return strcmp(xLeft.Sz(), xRight.Sz()) > 0;
	}

	friend bool operator > (const char* szLeft, const EmStr& xRight)
	{
		return strcmp(szLeft, xRight.Sz()) > 0;
	}

	friend bool operator > (const EmStr& xLeft, const char* szRight)
	{
		return strcmp(xLeft.Sz(), szRight) > 0;
	}

	friend EmStr operator + ( const char* szLeft, const EmStr& xRight )
	{
		EmStr xStr;
		xStr.Append(szLeft);
		xStr.Append(xRight.Sz());
		return xStr;
	}

	friend EmStr operator + ( const EmStr& xLeft, const char* szRight )
	{
		EmStr xStr;
		xStr.Append(xLeft.Sz());
		xStr.Append(szRight);
		return xStr;
	}

	friend EmStr operator + ( const EmStr& xLeft, const EmStr& xRight )
	{
		EmStr xStr;
		xStr.Append(xLeft.Sz());
		xStr.Append(xRight.Sz());
		return xStr;
	}

public:
	static EmStr Int2Str(int iNum);
	static EmStr Long2Str(INT64 iNum);
	static int Str2Int(const char* szSrc);
	static INT64 Str2Long(const char* szSrc);
	static EmStr Float2Str(double fNum);
	static int Char2Int(char chSrc);
	static EmStr Char2Str(char chSrc);
	static char Str2Char(const char* szSrc);
	
	static EmStr Lower(const char* szSrc);
	static EmStr Upper(const char* szSrc);
	static EmStr Reverse(const char* szSrc);
	static bool Equal(const char* szLeft, const char* szRight);
	static bool EqualNoCase(const char* szLeft, const char* szRight);

	static unsigned int CountChar(const char* szSrc, char chFind);
	static unsigned int CountStr(const char* szSrc, const char* szFind);
	
	static EmStr Prefix(const char* szSrc, const char* szPrefix);
	static EmStr PadLeft(const char* szSrc, char chPad, int iTotalLen);
	static EmStr PadRight(const char* szSrc, char chPad, int iTotalLen);
	static EmStr SubStr(const char *szSrc, unsigned int iStart, int iSubLen);

	static EmStr Repeat(const char* szSrc,int iCount);

	static EmStr AddSlashes(const char* szSrc, char chFind);
	static EmStr StripSlashes(const char* szSrc);
	
	static EmStr Dec2Bin(const char* szSrc);
	static EmStr Bin2Dec(const char* szSrc);
	static EmStr Hex2Dec(const char* szSrc);
	static EmStr Dec2Hex(const char* szSrc);
	static EmStr Bin2Hex(const char* szSrc);
	static EmStr Hex2Bin(const char* szSrc);
	static EmStr Str2Hex(const char* szSrc);

	static int FindChar(const char *szSrc, char chFind, unsigned int iFromPos);
	static int FindFrom(const char *szSrc, const char *szFind, unsigned int iFromPos);
	static int Find(const char *szSrc, const char *szFind);
	static int FindReverse(const char* szSrc, const char* szFind);
	static int FindSafe(const char* szSrc, const char* szFind, unsigned int iLimit);
	
	static EmStr TrimLeft(const char* szSrc);
	static EmStr TrimRight(const char* szSrc);
	static EmStr Trim(const char* szSrc);

	static EmStr FormatNum(const char* szSrc, unsigned int nLenAfterPoint);

	static EmStr Replace(const char* szSrc, const char* szFind, const char* szReplace);
	static EmStr Cut(const char* szSrc, unsigned int nStart, unsigned int nLen);
	static EmStr Insert(const char* szSrc, const char* szInsert, unsigned int nStart);
	static EmStr Splice(const char* szSrc, const char* szInsert, unsigned int nStart, unsigned int nLen);
	
	static bool IsDigit(char ch);
	static bool IsLetter(char ch);
	static bool IsInt(const char* szSrc);
	static bool IsNumber(const char* szSrc);
	static bool IsDomain(const char* szSrc);
	static bool IsEmail(const char* szSrc);
	static bool IsIp(const char* szSrc);
	static bool IsUrl(const char* szSrc);
	static bool IsAsc(const char* szSrc);
	static bool IsLimitedIn(const char* szSrc, const char* szLimited);
	/*yyyy-mm-dd hh:ii:ss*/
	static bool IsDateTime(const char* szSrc);

	static INT64 IpTextToNum(const char* szIpText);
	static EmStr IpNumToText(INT64 iIpNum);

	static EmStr UrlGetProtocol(const char* szSrc);
	static EmStr UrlGetHost(const char* szSrc);
	static unsigned short UrlGetPort(const char* szSrc);
	static EmStr UrlGetPath(const char* szSrc);
	static EmStr UrlGetParam(const char* szSrc);
	static EmStr UrlGetPathWithParam(const char* szSrc);

	static EmStr FileDirFromPath(const char* szSrc);
	static EmStr FileNameFromPath(const char* szSrc);
	static EmStr FileTitleFromPath(const char* szSrc);
	static EmStr FileExtFromPath(const char* szSrc);
	static EmStr FileFixDir(const char* szSrc);

	static EmStr AnsiToUtf8(const char* szSrc);
	static EmStr Utf8ToAnsi(const char* szSrc);

	static EmStr Md5( const char* szSrc);
	static EmStr UrlEncode(const char* szSrc, bool bEncodeAll);
	static EmStr UrlDecode(const char* szSrc, bool bEncodeAll);
	static EmStr Base64Encode( const char* szSrc);
	static EmStr Base64Decode( const char* szSrc);

	static int ListInBuffer(std::vector<EmStr>& rList, const char* szBuffer, int iBufSize);
	static EmStr Delimit(const char* szSrc, int iUnitLen, char chDelimiter);

protected:
	void Init();
	
protected:
	
	char* m_bufData;
	int m_iLength;
	int m_iCapacity;
};//class EmStr  

}//namespace em

#endif // #ifndef _EM_STR_H
