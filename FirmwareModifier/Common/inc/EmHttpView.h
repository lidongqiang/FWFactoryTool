#ifndef _h_EmHttpView
#define _h_EmHttpView

#include "EmBase.h"

#include <string>


namespace em
{

class EmHttpView
{
public:
	EmHttpView();
	virtual ~EmHttpView();

	static int Post(std::string &strResult, const char* szUrl, const char* szData
		, EmEncode eEncode = EmEncodeUtf8, int iMaxResult = 5000, int iTimeout=3000);

	static int Get(std::string &strResult, const char* szUrl
		, EmEncode eEncode = EmEncodeUtf8, int iMaxResult = 5000, int iTimeout=3000);


};//class EmHttpView 

}//namespace em

#endif //#ifndef _EM_HTTP_VIEW_H