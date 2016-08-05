#ifndef _h_EmBase64
#define _h_EmBase64

#include "../inc/EmBase.h"


namespace em
{
	class EmBase64
	{
	public:
		static char* Encode(char *szDest, const char *szSrc);   
		static char* Decode(char *szDest, const char *szSrc);  
	};
}


#endif