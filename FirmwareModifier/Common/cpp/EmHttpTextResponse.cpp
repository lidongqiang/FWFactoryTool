#include "EmHttpTextResponse.h"

em::EmHttpTextResponse::EmHttpTextResponse()
{
	m_iStatusCode = 0;
}

em::EmHttpTextResponse::~EmHttpTextResponse()
{
	
}

int em::EmHttpTextResponse::GetStatusCode()
{
	return m_iStatusCode;
}

void em::EmHttpTextResponse::SetStatusCode( int iStatusCode )
{
	m_iStatusCode = iStatusCode;
}

std::string em::EmHttpTextResponse::GetHead()
{
	return m_strHead;
}

void em::EmHttpTextResponse::SetHead( const char* szHead )
{
	m_strHead = szHead;
}

std::string em::EmHttpTextResponse::GetContent()
{
	return m_strContent;
}

void em::EmHttpTextResponse::SetContent( const char* szContent )
{
	m_strContent = szContent;
}
