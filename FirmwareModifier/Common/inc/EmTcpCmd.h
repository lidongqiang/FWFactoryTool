#ifndef _h_EmTcpCmd
#define _h_EmTcpCmd

#include "EmBase.h"
#include "EmTcpBase.h"

#include <string>

namespace em
{
	class EmTcpCmd
	{
	public:

		int m_iType;
		int m_iContentSize;
		char* m_bufContent;
		std::string* m_pstrLocalFileName;
		std::string* m_pstrRemoteFileName;

	public:

		EmTcpCmd();
		virtual ~EmTcpCmd();

		int MakeTextCmd(const char* szText);
		int MakeFileCmd(const char* szLocalFileName, const char* szRemoteFileName);
		int MakeRawCmd(const char* bufContent, int iContentSize);
		int MakeBeatCmd();
		int MakeCloseCmd();
	};
}
#endif