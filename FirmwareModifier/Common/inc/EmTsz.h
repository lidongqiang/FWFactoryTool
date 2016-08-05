#ifndef _h_EmTsz
#define _h_EmTsz

#include "EmBase.h"

#include <tchar.h>

namespace em
{

/*static functions for TCHAR* string */
class EmTsz  
{
public:
	EmTsz();
	virtual ~EmTsz();

	static TCHAR* Int2Str(TCHAR* szDest, int iNum);
	static TCHAR* Long2Str(TCHAR* szDest,__int64 iNum);
	static int Str2Int(const TCHAR* szSrc);
	static __int64 Str2Long(const TCHAR* szSrc);
	static TCHAR* Float2Str(TCHAR *szDest, double fNum);
	static int Char2Int(TCHAR chSrc);
	static TCHAR* Char2Str(TCHAR *szDest, TCHAR chSrc);
	static TCHAR Str2Char(const TCHAR* szSrc);
	
	static TCHAR* Lower(TCHAR* szSrc);
	static TCHAR* Upper(TCHAR* szSrc);
	static TCHAR* Reverse(TCHAR* szSrc);
	
	static unsigned int CountChar(const TCHAR* szSrc, TCHAR chFind);
	static unsigned int CountStr(const TCHAR* szSrc, const TCHAR* szFind);
	
	static TCHAR* Prefix(TCHAR* szSrc,const TCHAR* szPrefix);
	static TCHAR* PadLeft(TCHAR* szSrc, TCHAR chPad, int iTotalLen);
	static TCHAR* PadRight(TCHAR* szSrc, TCHAR chPad, int iTotalLen);
	static TCHAR* SubStr(TCHAR *szDest, const TCHAR *szSrc, unsigned int iStart, int iSubLen);
	
	static TCHAR* Repeat(TCHAR* szDest, const TCHAR* szSrc,int iCount);
	
	static TCHAR* AddSlashes(TCHAR* szSrc, TCHAR chFind);
	static TCHAR* StripSlashes(TCHAR* szSrc);
	
	static TCHAR* Dec2Bin(TCHAR* szSrc);
	static TCHAR* Bin2Dec(TCHAR* szSrc);
	static TCHAR* Hex2Dec(TCHAR* szSrc);
	static TCHAR* Dec2Hex(TCHAR* szSrc);
	static TCHAR* Bin2Hex(TCHAR* szSrc);
	static TCHAR* Hex2Bin(TCHAR* szSrc);
	
	static int FindChar(const TCHAR *szSrc, TCHAR chFind, unsigned int iFromPos);
	static int FindFrom(const TCHAR *szSrc, const TCHAR *szFind, unsigned int iFromPos);
	static int Find(const TCHAR *szSrc, const TCHAR *szFind);
	static int FindReverse(const TCHAR* szSrc, const TCHAR* szFind);
	static int CompareNoCase(const TCHAR* szLeft, const TCHAR* szRight );
	static int FindSafe(const TCHAR* szSrc, const TCHAR* szFind, unsigned int iLimit);
	static bool Equal(const TCHAR* szLeft, const TCHAR* szRight);
	static bool EqualNoCase(const TCHAR* szLeft, const TCHAR* szRight);
	
	static TCHAR* TrimLeft(TCHAR* szSrc);
	static TCHAR* TrimRight(TCHAR* szSrc);
	static TCHAR* Trim(TCHAR* szSrc);
	
	static TCHAR* FormatNum( TCHAR* szSrc, unsigned int nLenAfterPoint);
	
	static TCHAR* Replace( TCHAR* szSrc, const TCHAR* szFind, const TCHAR* szReplace);
	static TCHAR* Cut(TCHAR* szSrc, unsigned int nStart, unsigned int nLen);
	static TCHAR* Insert(TCHAR* szSrc, const TCHAR* szInsert, unsigned int nStart);
	static TCHAR* Splice(TCHAR* szSrc, const TCHAR* szInsert, unsigned int nStart, unsigned int nLen);
	
	static bool IsDigit(TCHAR chSrc);
	static bool IsLetter(TCHAR chSrc);
	static bool IsInt(const TCHAR* szSrc);
	static bool IsHexNumber(const TCHAR* szSrc);
	static bool IsNumber(const TCHAR* szSrc);
	static bool IsDomain(const TCHAR* szSrc);
	static bool IsEmail(const TCHAR* szSrc);
	static bool IsIp(const TCHAR* szSrc);
	static bool IsUrl(const TCHAR* szSrc);
	static bool IsAsc(const TCHAR* szSrc);
	/*yyyy-mm-dd hh:ii:ss*/
	static bool IsDateTime(const TCHAR* szSrc);
	static bool IsLimitedIn(const TCHAR* szSrc, const TCHAR* szLimited);
	
	static __int64 IpTextToNum(const TCHAR* szIpText);
	static TCHAR* IpNumToText(TCHAR *szDest, __int64 iIpNum);
	
	static void AllocArray(TCHAR ***szArr, int iRow, int iCol);
	static void FreeArray(TCHAR ***szArr, int iRow);
	
	static TCHAR* UrlGetProtocol(TCHAR *szDest, const TCHAR* szSrc);
	static TCHAR* UrlGetHost(TCHAR *szDest, const TCHAR* szSrc);
	static unsigned short UrlGetPort(const TCHAR* szSrc);
	static TCHAR* UrlGetPath(TCHAR *szDest, const TCHAR* szSrc);
	static TCHAR* UrlGetParam(TCHAR *szDest, const TCHAR* szSrc);
	static TCHAR* UrlGetPathWithParam(TCHAR *szDest, const TCHAR* szSrc);
	
	static TCHAR* FileDirFromPath(TCHAR* szDest, const TCHAR* szSrc);
	static TCHAR* FileNameFromPath(TCHAR* szDest, const TCHAR* szSrc);
	static TCHAR* FileTitleFromPath(TCHAR* szDest, const TCHAR* szSrc);
	static TCHAR* FileExtFromPath(TCHAR* szDest, const TCHAR* szSrc);
	static TCHAR* FileFixDir(TCHAR* szDest, const TCHAR* szSrc);

	static TCHAR* HexIncrease(TCHAR *szSrc);
	static int HexStr2IntBytes(unsigned char* pByte, const TCHAR* szStr);
	static TCHAR* HexStrFromIntBytes(TCHAR* szHex, const unsigned char* pBytes, int iByteCount);
	static TCHAR* Delimit(TCHAR* szSrc, int iUnitLen, TCHAR chDeimiter);



};//class EmTcs

}//namespace em

#endif //#ifndef _EM_TCS_H
