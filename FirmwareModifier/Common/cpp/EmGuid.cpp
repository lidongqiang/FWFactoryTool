#include "stdafx.h"
#include "../inc/EmGuid.h"
#include "../inc/EmStlStr.h"


using namespace em;


const GUID em::EmGuid::GuidZero = EM_GUID_ZERO;
const GUID em::EmGuid::GuidUsb = EM_GUID_USB;
const GUID em::EmGuid::GuidAdb = EM_GUID_ADB;
/*
string format: 12341234-1234-1234-1234-123412341234
object format: { 0x12341234, 0x1234, 0x1234, { 0x12, 0x34, 0x12, 0x34, 0x12, 0x34, 0x12, 0x34 } }
*/
GUID em::EmGuid::StrToObj( const char* szGuid )
{
	GUID xGuid;
	memset(&xGuid,0,sizeof(xGuid));

	if(!IsGuidStr(szGuid))
	{
		return xGuid;
	}

	std::string csGuid = EmStlStr::Lower(szGuid);

	std::string strHex1;
	std::string strHex2;
	std::string strHex3;
	std::string strHex4;
	std::string strHex4_0;
	std::string strHex4_1;
	std::string strHex4_2;
	std::string strHex4_3;
	std::string strHex4_4;
	std::string strHex4_5;
	std::string strHex4_6;
	std::string strHex4_7;


	strHex1 += EmStlStr::SubStr(szGuid,0,8);

	strHex2 += EmStlStr::SubStr(szGuid,9,4);

	strHex3 += EmStlStr::SubStr(szGuid,14,4);

	strHex4 += EmStlStr::SubStr(szGuid,19,4);
	strHex4 += EmStlStr::SubStr(szGuid,24,4);
	strHex4 += EmStlStr::SubStr(szGuid,28,8);

	strHex4_0 = EmStlStr::SubStr(strHex4.c_str(),0,2);
	strHex4_1 = EmStlStr::SubStr(strHex4.c_str(),2,2);
	strHex4_2 = EmStlStr::SubStr(strHex4.c_str(),4,2);
	strHex4_3 = EmStlStr::SubStr(strHex4.c_str(),6,2);
	strHex4_4 = EmStlStr::SubStr(strHex4.c_str(),8,2);
	strHex4_5 = EmStlStr::SubStr(strHex4.c_str(),10,2);
	strHex4_6 = EmStlStr::SubStr(strHex4.c_str(),12,2);
	strHex4_7 = EmStlStr::SubStr(strHex4.c_str(),14,2);

	std::string strDec1 = EmStlStr::Hex2Dec(strHex1.c_str());
	std::string strDec2 = EmStlStr::Hex2Dec(strHex2.c_str());
	std::string strDec3 = EmStlStr::Hex2Dec(strHex3.c_str());
	std::string strDec4_0 = EmStlStr::Hex2Dec(strHex4_0.c_str());
	std::string strDec4_1 = EmStlStr::Hex2Dec(strHex4_1.c_str());
	std::string strDec4_2 = EmStlStr::Hex2Dec(strHex4_2.c_str());
	std::string strDec4_3 = EmStlStr::Hex2Dec(strHex4_3.c_str());
	std::string strDec4_4 = EmStlStr::Hex2Dec(strHex4_4.c_str());
	std::string strDec4_5 = EmStlStr::Hex2Dec(strHex4_5.c_str());
	std::string strDec4_6 = EmStlStr::Hex2Dec(strHex4_6.c_str());
	std::string strDec4_7 = EmStlStr::Hex2Dec(strHex4_7.c_str());

	xGuid.Data1 = EmStlStr::Str2Long(strDec1.c_str());
	xGuid.Data2 = EmStlStr::Str2Int(strDec2.c_str());
	xGuid.Data3 = EmStlStr::Str2Int(strDec3.c_str());
	xGuid.Data4[0] = EmStlStr::Str2Int(strDec4_0.c_str());
	xGuid.Data4[1] = EmStlStr::Str2Int(strDec4_1.c_str());
	xGuid.Data4[2] = EmStlStr::Str2Int(strDec4_2.c_str());
	xGuid.Data4[3] = EmStlStr::Str2Int(strDec4_3.c_str());
	xGuid.Data4[4] = EmStlStr::Str2Int(strDec4_4.c_str());
	xGuid.Data4[5] = EmStlStr::Str2Int(strDec4_5.c_str());
	xGuid.Data4[6] = EmStlStr::Str2Int(strDec4_6.c_str());
	xGuid.Data4[7] = EmStlStr::Str2Int(strDec4_7.c_str());

	
	return xGuid;
}

bool em::EmGuid::IsGuidStr( const char* szGuid )
{
	
	if(szGuid == NULL)
	{
		return false;
	}
	if(strlen(szGuid) != 36)
	{
		return false;
	}
	if(EmStlStr::CountChar(szGuid,'-') != 4)
	{
		return false;
	}

	return true;
}

bool em::EmGuid::Equal( const GUID& xGuid1, const GUID& xGuid2 )
{
	if(xGuid1.Data1 != xGuid2.Data1)
	{
		return false;
	}

	if(xGuid1.Data2 != xGuid2.Data2)
	{
		return false;
	}

	if(xGuid1.Data3 != xGuid2.Data3)
	{
		return false;
	}

	for(int i = 0; i < 8; i++)
	{
		if(xGuid1.Data4[i] != xGuid2.Data4[i])
		{
			return false;
		}
	}

	return true;
}

std::string em::EmGuid::ObjToStr( const GUID& xGuid )
{
	std::string strResult;
	std::string strPart1;
	std::string strPart2;
	std::string strPart3;
	std::string strPart4;
	std::string strPart5;

	std::string strData4_0;
	std::string strData4_1;
	std::string strData4_2;
	std::string strData4_3;
	std::string strData4_4;
	std::string strData4_5;
	std::string strData4_6;
	std::string strData4_7;

	strPart1 =  EmStlStr::Dec2Hex( EmStlStr::Long2Str(xGuid.Data1).c_str() );
	strPart2 =  EmStlStr::Dec2Hex( EmStlStr::Int2Str(xGuid.Data2).c_str() );
	strPart3 =  EmStlStr::Dec2Hex( EmStlStr::Int2Str(xGuid.Data3).c_str() );
	strData4_0 =  EmStlStr::Dec2Hex( EmStlStr::Int2Str(xGuid.Data4[0]).c_str() );
	strData4_1 =  EmStlStr::Dec2Hex( EmStlStr::Int2Str(xGuid.Data4[1]).c_str() );
	strData4_2 =  EmStlStr::Dec2Hex( EmStlStr::Int2Str(xGuid.Data4[2]).c_str() );
	strData4_3 =  EmStlStr::Dec2Hex( EmStlStr::Int2Str(xGuid.Data4[3]).c_str() );
	strData4_4 =  EmStlStr::Dec2Hex( EmStlStr::Int2Str(xGuid.Data4[4]).c_str() );
	strData4_5 =  EmStlStr::Dec2Hex( EmStlStr::Int2Str(xGuid.Data4[5]).c_str() );
	strData4_6 =  EmStlStr::Dec2Hex( EmStlStr::Int2Str(xGuid.Data4[6]).c_str() );
	strData4_7 =  EmStlStr::Dec2Hex( EmStlStr::Int2Str(xGuid.Data4[7]).c_str() );
	strPart4 += strData4_0;
	strPart4 += strData4_1;

	strPart5 += strData4_2;
	strPart5 += strData4_3;
	strPart5 += strData4_4;
	strPart5 += strData4_5;
	strPart5 += strData4_6;
	strPart5 += strData4_7;

	strResult += strPart1;
	strResult += "-";
	strResult += strPart2;
	strResult += "-";
	strResult += strPart3;
	strResult += "-";
	strResult += strPart4;
	strResult += "-";
	strResult += strPart5;

	return strResult;
}
