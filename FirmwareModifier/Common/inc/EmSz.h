#ifndef _h_EmSz
#define _h_EmSz

#include "EmBase.h"

namespace em
{

/*
static functions for char* string
	*/
class EmSz  
{
public:
	static char* Int2Str(char* szDest, int iNum);
	static char* Long2Str(char* szDest, INT64 iNum);
	static int Str2Int(const char* szSrc);
	static INT64 Str2Long(const char* szSrc);
	static char* Float2Str(char *szDest, double fNum);

	/* 
	only for digit 0-9
	usage: 
	int a = EmStr::Char2Int('5')
	ASSERT(a==5);
	*/
	static int Char2Int(char chSrc);
	static char* Char2Str(char *szDest, char chSrc);
	static char Str2Char(const char* szSrc);
	
	static char* Lower(char* szSrc);
	static char* Upper(char* szSrc);
	static char* Reverse(char* szSrc);
	static bool Equal(const char* szLeft, const char* szRight);
	static bool EqualNoCase(const char* szLeft, const char* szRight);

	static unsigned int CountChar(const char* szSrc, char chFind);
	static unsigned int CountStr(const char* szSrc, const char* szFind);
	
	static char* Prefix(char* szSrc,const char* szPrefix);
	static char* PadLeft(char* szSrc, char chPad, int iTotalLen);
	static char* PadRight(char* szSrc, char chPad, int iTotalLen);
	static char* SubStr(char *szDest, const char *szSrc, unsigned int iStart, int iSubLen);

	static char* Repeat(char* szDest, const char* szSrc,int iCount);

	static char* AddSlashes(char* szSrc, char chFind);
	static char* StripSlashes(char* szSrc);
	
	static char* Dec2Bin(char* szSrc);
	static char* Bin2Dec(char* szSrc);
	static char* Hex2Dec(char* szSrc);
	static char* Dec2Hex(char* szSrc);
	static char* Bin2Hex(char* szSrc);
	static char* Hex2Bin(char* szSrc);
	static char* StrToHex(char* szDest, const char* szSrc);

	static int FindChar(const char *szSrc, char chFind, unsigned int iFromPos);
	static int FindFrom(const char *szSrc, const char *szFind, unsigned int iFromPos);
	static int Find(const char *szSrc, const char *szFind);
	static int FindReverse(const char* szSrc, const char* szFind);
	static int FindSafe(const char* szSrc, const char* szFind, unsigned int iLimit);
	
	static char* TrimLeft(char* szSrc);
	static char* TrimRight(char* szSrc);
	static char* Trim(char* szSrc);

	static char* FormatNum( char* szSrc, unsigned int nLenAfterPoint);

	static char* Replace( char* szSrc, const char* szFind, const char* szReplace);
	static char* Cut(char* szSrc, unsigned int nStart, unsigned int nLen);
	static char* Insert(char* szSrc, const char* szInsert, unsigned int nStart);
	static char* Splice(char* szSrc, const char* szInsert, unsigned int nStart, unsigned int nLen);
	
	static bool IsDigit(char chSrc);
	static bool IsLetter(char chSrc);
	static bool IsInt(const char* szSrc);
	static bool IsHexNumber(const char* szSrc);
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
	static char* IpNumToText(char *szDest, INT64 iIpNum);

	static void AllocArray(char ***szArr, int iRow, int iCol);
	static void FreeArray(char ***szArr, int iRow);

	static char* UrlGetProtocol(char *szDest, const char* szSrc);
	static char* UrlGetHost(char *szDest, const char* szSrc);
	static unsigned short UrlGetPort(const char* szSrc);
	static char* UrlGetPath(char *szDest, const char* szSrc);
	static char* UrlGetParam(char *szDest, const char* szSrc);
	static char* UrlGetPathWithParam(char *szDest, const char* szSrc);

	static char* FileDirFromPath(char* szDest, const char* szSrc);
	static char* FileNameFromPath(char* szDest, const char* szSrc);
	static char* FileTitleFromPath(char* szDest, const char* szSrc);
	static char* FileExtFromPath(char* szDest, const char* szSrc);
	static char* FileFixDir(char* szDest, const char* szSrc);

	static char* HexIncrease(char *szSrc);
	static int HexStr2IntBytes(unsigned char* pByte, const char* szStr);
	static char* HexStrFromIntBytes(char* szHex, const unsigned char* pBytes, int iByteCount);
	static char* Delimit(char* szSrc, int iUnitLen, char chDeimiter);


};//class EmStr  

}//namespace em

#endif // #ifndef _EM_STR_H
