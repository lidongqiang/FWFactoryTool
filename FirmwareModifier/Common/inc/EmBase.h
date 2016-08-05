#ifndef _h_EmBase
#define _h_EmBase

#pragma warning (disable: 4018 4089 4221 4244 4503 4514 4541 4786 4099 4800)

#ifndef _CRT_NON_CONFORMING_SWPRINTFS
#define _CRT_NON_CONFORMING_SWPRINTFS
#endif 

#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif


#if(_WIN32_WINNT < 0x0501)
#undef _WIN32_WINNT
#define _WIN32_WINNT 0x0501
#endif

#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0501
#endif

#if(WINVER < 0x0501)
#undef WINVER
#define WINVER 0x0501
#endif

#ifndef WINVER
#define WINVER 0x0501
#endif

#include "EmTypes.h"
#include "EmConstants.h"
#include "EmErrorCode.h"
#include "EmMacros.h"

//#include <shlwapi.h>
//#pragma comment(lib,"shlwapi.lib")

//gdi+
//#include "GdiPlus.h"
//#pragma comment(lib,"GdiPlus.lib")

#endif //#ifndef _EM_BASE_H
