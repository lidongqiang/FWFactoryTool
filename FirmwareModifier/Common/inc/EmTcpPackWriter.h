#ifndef _h_EmTcpPackWriter
#define _h_EmTcpPackWriter

#include "EmBase.h"
#include "EmTcpBase.h"

#include <string>
#include <vector>
#include <map>



namespace em
{
	class EmTcpPackWriter
	{
	public:
		static int StaticInit();
		static int StaticExit();
		static const char* GetBeatBytes();
		static const char* GetCloseBytes();
		static int MakeTextBytes(char* bufHolder, int iContentSize);
		static int MakeRawBytes(char* bufHolder, int iContentSize);
		static int MakeFileBytes(char* bufHolder,int iBufSize
			, const char* szLocalFileName, const char* szRemoteFileName, int iFileSize, int iFileOffset, int iPartSize);

	private:
		static bool s_bStaticInited;
		static char* s_bufBeatCache;
		static char* s_bufCloseCache;

	};
}
#endif