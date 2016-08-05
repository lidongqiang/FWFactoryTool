#ifndef _h_EmHttpBase
#define _h_EmHttpBase

#include "EmBase.h"

#include <string>
#include <map>



namespace em
{

class EmHttpBase
{

public:
	static int ExtractResponseHeadMap(const char* szHead, std::map<std::string,std::string> *pMapStr);
	static int ExtractResponseStatus(const char* szHead);
	static std::string MakeGetString(const char* szUrl, const std::map<std::string,std::string> *pMapStr = NULL);
	static std::string MakePostString(const char* szUrl, const char* szData);

};//class EmHttpBase

}//namespace em


#endif //#ifndef _EM_HTTP_BASE_H