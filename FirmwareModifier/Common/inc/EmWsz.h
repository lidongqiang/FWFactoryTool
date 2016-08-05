#ifndef _h_EmWsz
#define _h_EmWsz

#include "EmBase.h"

#include <wchar.h>

namespace em
{

class EmWsz  
{
public:
	EmWsz();
	virtual ~EmWsz();

	static wchar_t* Int2Str(wchar_t* szDest, int iNum);
	static wchar_t* Long2Str(wchar_t* szDest,INT64 iNum);
	static int Str2Int(const wchar_t* szSrc);
	static INT64 Str2Long(const wchar_t* szSrc);
	static wchar_t* Float2Str(wchar_t *szDest, double fNum);
	static int Char2Int(wchar_t chSrc);
	static wchar_t* Char2Str(wchar_t *szDest, wchar_t chSrc);
	static wchar_t Str2Char(const wchar_t* szSrc);
	
	static wchar_t* Lower(wchar_t* szSrc);
	static wchar_t* Upper(wchar_t* szSrc);
	static wchar_t* Reverse(wchar_t* szSrc);
	
	static unsigned int CountChar(const wchar_t* szSrc, wchar_t chFind);
	static unsigned int CountStr(const wchar_t* szSrc, const wchar_t* szFind);
	
	static wchar_t* Prefix(wchar_t* szSrc,const wchar_t* szPrefix);
	static wchar_t* PadLeft(wchar_t* szSrc, wchar_t chPad, int iTotalLen);
	static wchar_t* PadRight(wchar_t* szSrc, wchar_t chPad, int iTotalLen);
	static wchar_t* SubStr(wchar_t *szDest, const wchar_t *szSrc, unsigned int iStart, int iSubLen);
	
	static wchar_t* Repeat(wchar_t* szDest, const wchar_t* szSrc,int iCount);
	
	static wchar_t* AddSlashes(wchar_t* szSrc, wchar_t chFind);
	static wchar_t* StripSlashes(wchar_t* szSrc);
	
	static wchar_t* Dec2Bin(wchar_t* szSrc);
	static wchar_t* Bin2Dec(wchar_t* szSrc);
	static wchar_t* Hex2Dec(wchar_t* szSrc);
	static wchar_t* Dec2Hex(wchar_t* szSrc);
	static wchar_t* Bin2Hex(wchar_t* szSrc);
	static wchar_t* Hex2Bin(wchar_t* szSrc);
	
	static int FindChar(const wchar_t *szSrc, wchar_t chFind, unsigned int iFromPos);
	static int FindFrom(const wchar_t *szSrc, const wchar_t *szFind, unsigned int iFromPos);
	static int Find(const wchar_t *szSrc, const wchar_t *szFind);
	static int FindReverse(const wchar_t* szSrc, const wchar_t* szFind);

	static int CompareNoCase(const wchar_t* szLeft, const wchar_t* szRight );
	static int FindSafe(const wchar_t* szSrc, const wchar_t* szFind, unsigned int iLimit);
	static bool Equal(const wchar_t* szLeft, const wchar_t* szRight);
	static bool EqualNoCase(const wchar_t* szLeft, const wchar_t* szRight);
	
	static wchar_t* TrimLeft(wchar_t* szSrc);
	static wchar_t* TrimRight(wchar_t* szSrc);
	static wchar_t* Trim(wchar_t* szSrc);
	
	static wchar_t* FormatNum( wchar_t* szSrc, unsigned int nLenAfterPoint);
	
	static wchar_t* Replace( wchar_t* szSrc, const wchar_t* szFind, const wchar_t* szReplace);
	static wchar_t* Cut(wchar_t* szSrc, unsigned int nStart, unsigned int nLen);
	static wchar_t* Insert(wchar_t* szSrc, const wchar_t* szInsert, unsigned int nStart);
	static wchar_t* Splice(wchar_t* szSrc, const wchar_t* szInsert, unsigned int nStart, unsigned int nLen);
	
	static bool IsDigit(wchar_t chSrc);
	static bool IsLetter(wchar_t chSrc);
	static bool IsInt(const wchar_t* szSrc);
	static bool IsHexNumber(const wchar_t* szSrc);
	static bool IsNumber(const wchar_t* szSrc);
	static bool IsDomain(const wchar_t* szSrc);
	static bool IsEmail(const wchar_t* szSrc);
	static bool IsIp(const wchar_t* szSrc);
	static bool IsUrl(const wchar_t* szSrc);
	static bool IsAsc(const wchar_t* szSrc);
	/*yyyy-mm-dd hh:ii:ss*/
	static bool IsDateTime(const wchar_t* szSrc);
	static bool IsLimitedIn(const wchar_t* szSrc, const wchar_t* szLimited);
	
	static void AllocArray(wchar_t ***szArr, int iRow, int iCol);
	static void FreeArray(wchar_t ***szArr, int iRow);
	
	static __int64 IpTextToNum(const wchar_t* szIpText);
	static wchar_t* IpNumToText(wchar_t *szDest, __int64 iIpNum);
	
	static wchar_t* UrlGetProtocol(wchar_t *szDest, const wchar_t* szSrc);
	static wchar_t* UrlGetHost(wchar_t *szDest, const wchar_t* szSrc);
	static unsigned short UrlGetPort(const wchar_t* szSrc);
	static wchar_t* UrlGetPath(wchar_t *szDest, const wchar_t* szSrc);
	static wchar_t* UrlGetParam(wchar_t *szDest, const wchar_t* szSrc);
	static wchar_t* UrlGetPathWithParam(wchar_t *szDest, const wchar_t* szSrc);
	
	static wchar_t* FileDirFromPath(wchar_t* szDest, const wchar_t* szSrc);
	static wchar_t* FileNameFromPath(wchar_t* szDest, const wchar_t* szSrc);
	static wchar_t* FileTitleFromPath(wchar_t* szDest, const wchar_t* szSrc);
	static wchar_t* FileExtFromPath(wchar_t* szDest, const wchar_t* szSrc);
	static wchar_t* FileFixDir(wchar_t* szDest, const wchar_t* szSrc);
};//class EmWcs

}//namespace em

#endif // #ifndef _EM_WCS_H
