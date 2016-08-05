#ifndef _h_EmHttpTextResponse
#define _h_EmHttpTextResponse

#include "EmBase.h"

#include <string>




namespace em
{

class EmHttpTextResponse
{

public:
	EmHttpTextResponse();
	virtual ~EmHttpTextResponse();

	int GetStatusCode();
	void SetStatusCode(int iStatusCode);
	
	std::string GetHead();
	void SetHead(const char* szHead);

	std::string GetContent();
	void SetContent(const char* szContent);

private:
	int m_iStatusCode;
	std::string m_strHead;
	std::string m_strContent;

};//class EmHttpTextResponse

} //namespace em;
#endif //#ifndef _EM_HTTP_TEXT_RESPONSE_H