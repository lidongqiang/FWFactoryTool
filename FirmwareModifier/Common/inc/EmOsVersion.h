#ifndef _h_EmOsVersion
#define _h_EmOsVersion

#include "EmBase.h"

#include <string>



#define EM_OS_VER_WIN7 "win7"
#define EM_OS_VER_VISTA "vista"
#define EM_OS_VER_W2K3 "w2k3"
#define EM_OS_VER_WXP "wxp"
#define EM_OS_VER_W2K "w2k"
#define EM_OS_VER_WNT4 "wnt4"
#define EM_OS_VER_WNT35 "wnt35"
#define EM_OS_VER_WME "wme"
#define EM_OS_VER_W98 "w98"
#define EM_OS_VER_W95 "w95"

namespace em
{
	
	class EmOsVersion
	{
	public:
		
		static std::string GetVersionText();
		
		
	};//class EmOsVersion 
	
}//namespace em

#endif //#ifndef _EM_OS_VERSION_H