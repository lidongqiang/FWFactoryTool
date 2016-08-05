#ifndef _h_EmStlStr
#define _h_EmStlStr

#include "EmBase.h"

#include <tchar.h>

#include <string>
#include <map>
#include <vector>






namespace em
{

class EmStlStr  
{
public:
	EmStlStr();
	virtual ~EmStlStr();

	static std::string Int2Str(int iNum);
	static std::string Long2Str(__int64 iNum);
	static int Str2Int(const char* szSrc);
	static __int64 Str2Long(const char* szSrc);
	static double Str2Float(const char* szSrc);
	static std::string Float2Str(double fNum);
	static int Char2Int(char chSrc);
	static std::string Char2Str(char chSrc);
	static char Str2Char(const char* szSrc);
	static std::string Utf8ToAnsi(const char* szSrc);
	static std::string AnsiToUtf8(const char* szSrc);
	static std::string UnicodeToUtf8(const wchar_t* szSrc);
	static std::string TcsToUtf8(const TCHAR* szSrc);
	
	static std::string Lower(const char* szSrc);
	static std::string Upper(const char* szSrc);
	static std::string Reverse(const char* szSrc);
	static bool Equal(const char* szLeft, const char* szRight);
	static bool EqualNoCase(const char* szLeft, const char* szRight);

	static unsigned int  CountChar(const char* szSrc, char chFind);
	static unsigned int  CountStr(const char* szSrc, const char* szFind);
	
	static std::string Prefix(const char* szSrc,const char* szPrefix);
	static std::string PadLeft(const char* szSrc, char chPad, int iTotalLen);
	static std::string SubStr(const char* szSrc, unsigned int  iStart, int iSubLen);

	static std::string Repeat(const char* szSrc,int iCount);

	static std::string AddSlashes(const char* szSrc, char chFind);
	static std::string StripSlashes(const char* szSrc);
	
	static std::string Dec2Bin(const char* szSrc);
	static std::string Bin2Dec(const char* szSrc);
	static std::string Hex2Dec(const char* szrSrc);
	static std::string Dec2Hex(const char* szSrc);
	static std::string Bin2Hex(const char* szSrc);
	static std::string Hex2Bin(const char* szSrc);
	static std::string Str2Hex(const char* szSrc);

	static int FindChar(const char* szSrc, char chFind, unsigned int  iFromPos);
	static int FindFrom(const char* szSrc, const char* szFind, unsigned int  iFromPos);
	static int Find(const char* szSrc, const char* szFind);
	static int FindReverse(const char* szSrc, const char* szFind);
	
	static std::string TrimLeft(const char* szSrc);
	static std::string TrimRight(const char* szSrc);
	static std::string Trim(const char* szSrc);

	static std::string FormatNum( const char* szSrc, unsigned int  nLenAfterPoint);

	static std::string Replace(const char* szSrc, const char* szFind, const char* szReplace);
	static std::string Cut(const char* szSrc, unsigned int  nStart, unsigned int  nLen);
	static std::string Insert(const char* szSrc, const char* szInsert, unsigned int  nStart);
	static std::string Splice(const char* szSrc, const char* szInsert, unsigned int  nStart, unsigned int  nLen);
	
	static std::string UrlEncode(const char* szSrc, bool bEncodeAll);
	static std::string UrlDecode(const char* szSrc, bool bEncodeAll);
	
	static std::string Md5( const char* szSrc);
	static std::string Base64Encode( const char* szSrc);
	static std::string Base64Decode( const char* szSrc);
	
	static __int64 IpTextToNum(const char* szIpText);
	static std::string IpNumToText( __int64 iIpNum);

	static bool IsDigit(char chSrc);
	static bool IsLetter(char chSrc);
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

	static unsigned short  UrlGetPort(const char* szSrc);
	static std::string UrlGetProtocol(const char* szSrc);
	static std::string UrlGetHost(const char* szSrc);
	static std::string UrlGetPath(const char* szSrc);
	static std::string UrlGetParam(const char* szSrc);
	static std::string UrlGetPathWithParam(const char* szSrc);

	static std::string  FileDirFromPath(const char* szSrc);
	static std::string  FileNameFromPath(const char* szSrc);
	static std::string  FileTitleFromPath(const char* szSrc);
	static std::string  FileExtFromPath(const char* szSrc);
	static std::string  FileFixDir(const char* szSrc);
	
	static int Split(std::vector<std::string> &rVec, const char* szSrc, const char* szDelimiter);
	static std::string Join(const std::vector<std::string> &rVec, const char* szDelimiter);
	static int XsvToMap(std::map<std::string,std::string> &rMap, const char* szSrc, const char* szSplit1, const char* szSplit2);
	static std::string MapToXsv(const std::map<std::string,std::string> &rMap, const char* szSplit1, const char* szSplit2);

	static int ListInBuffer(std::vector<std::string>& rList, const char* szBuffer, int iBufSize);
	
};//class EmStlStr

}//namespace em

#endif // #ifndef _EM_STL_STR_H
