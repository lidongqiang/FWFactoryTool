#ifndef _h_EmGuid
#define _h_EmGuid

#include "EmBase.h"

#include <string>

#include <afx.h>


#define EM_GUID_ZERO {0x00000000, 0x0000, 0x0000, {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}}
#define EM_GUID_USB  {0xa5dcbf10, 0x6530, 0x11d2, {0x90, 0x1f, 0x00, 0xc0, 0x4f, 0xb9, 0x51, 0xed}}
//#define EM_GUID_ADB  {0xf72fe0d4, 0xcbcb, 0x407d, {0x88, 0x14, 0x9e, 0xd6, 0x73, 0xd0, 0xdd, 0x6b}}
#define EM_GUID_ADB  {0x3F966BD9, 0xFA04, 0x4ec5, {0x99, 0x1C, 0xD3, 0x26, 0x97, 0x3b, 0x51, 0x28}}

namespace em
{
	
	class EmGuid
	{

	public:
		static GUID StrToObj(const char* szGuid);
		static std::string ObjToStr(const GUID& xGuid);
		static bool IsGuidStr(const char* szGuid);
		static bool Equal(const GUID& xGuid1, const GUID& xGuid2);
		
		static const GUID GuidZero;
		static const GUID GuidUsb;
		static const GUID GuidAdb;
		
	};//class EmGuid
	
	
	
	
	
	
}//namespace em

#endif //#ifndef EM_GUID_H