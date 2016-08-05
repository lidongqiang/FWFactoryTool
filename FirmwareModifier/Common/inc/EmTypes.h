#ifndef _h_EmTypes
#define _h_EmTypes


namespace em
{
	enum EmEncode
	{
		EmEncodeNone = 0
		,EmEncodeAnsi = 1
		,EmEncodeUtf8 = 2
		,EmEncodeUnicode = 3	
	};
}

typedef __int64 INT64;

//#if VC_VERSION <= 6
//
//#ifndef ULONG_PTR
//#define ULONG_PTR unsigned long*
//#endif 
//
//
//#ifndef DWORD_PTR
//#define DWORD_PTR unsigned long*
//#endif 
//#endif


#endif //#ifndef _EM_TYPES_H
