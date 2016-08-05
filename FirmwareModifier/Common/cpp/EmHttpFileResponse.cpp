#include "EmHttpFileResponse.h"
#include "EmSz.h"
#include "EmNetBase.h"
#include "EmHttpBase.h"
#include "EmStlStr.h"


using namespace em;

em::EmHttpFileResponse::EmHttpFileResponse()
{
	m_iContentLen = 0;
	m_iHeadLen = 0;
	m_iLimit = 0;
	m_iTotalLen = 0;
	m_bAlloc = false;
	m_iRecvOnceSize = EM_NET_DEFAULT_RECV_ONCE_SIZE;
	Clear();
}

em::EmHttpFileResponse::~EmHttpFileResponse()
{
	Clear();
}

char * em::EmHttpFileResponse::AllocMem( int iLimit )
{
	m_iLimit = iLimit;
	Clear();
	m_bufTotal = (char*)malloc(iLimit + 4096 + m_iRecvOnceSize);
	m_bAlloc = true;
	return m_bufTotal;
}

int em::EmHttpFileResponse::FreeMem()
{
	Clear();
	int iResult = 0;
	return iResult;
}

std::string em::EmHttpFileResponse::GetHeadText()
{
	if(m_iHeadLen == 0)
	{
		return "";
	}

	std::string strResult;
	strResult.append(m_bufTotal,m_iHeadLen);
	return strResult;
}

char * em::EmHttpFileResponse::GetContentBuffer()
{
	if(m_iContentLen == 0)
	{
		return NULL;
	}
	return m_bufContent;
}

char * em::EmHttpFileResponse::GetTotalBuffer()
{
	return m_bufTotal;
}

int em::EmHttpFileResponse::GetStatusCode()
{
	if(m_iTotalLen == 0)
	{
		return EM_ERR_OBJ_NOT_READY;
	}

	int iResult = EmHttpBase::ExtractResponseStatus(GetHeadText().c_str());

	return iResult;
}

int em::EmHttpFileResponse::Compute()
{
	if(m_iTotalLen == 0)
	{
		return EM_ERR_OBJ_NOT_READY;
	}

	m_bufHead = m_bufTotal;

	int iBrPos = 0;
	iBrPos = EmSz::Find(m_bufTotal,"\r\n\r\n");
	if(iBrPos == -1)
	{
		iBrPos = EmSz::Find(m_bufTotal,"\n\n");
	}
	
	if(iBrPos == -1)
	{
		return EM_ERR_STR_FORMAT;
	}

	m_iHeadLen = iBrPos;

	if(EmSz::Find(m_bufTotal,"\r\n\r\n"))
	{
		m_bufContent = m_bufTotal + iBrPos + 4;
		m_iContentLen = m_iTotalLen - m_iHeadLen - 4;
	}
	else
	{
		m_bufContent = m_bufTotal + iBrPos + 2;
		m_iContentLen = m_iTotalLen - m_iHeadLen - 2;
	}

	int iResult = 0;
	return iResult;
}

int em::EmHttpFileResponse::GetContentLen()
{
	return m_iContentLen;
}

void em::EmHttpFileResponse::Clear()
{
	if(m_bAlloc && m_bufTotal != NULL)
	{
		free( m_bufTotal);
		m_bufTotal = NULL;
	}

	m_bAlloc = false;
	m_iLimit = 0;
	m_bufTotal = NULL;
	m_bufHead = NULL;
	m_bufContent = NULL;
	m_iContentLen = 0;
	m_iHeadLen = 0;
	m_iTotalLen = 0;
}

void em::EmHttpFileResponse::SetTotalLen( int iLen )
{
	m_iTotalLen = iLen;
}

int em::EmHttpFileResponse::GetTotalLen()
{
	return m_iTotalLen;
}

int em::EmHttpFileResponse::GetHeadLen()
{
	return m_iHeadLen;
}

bool em::EmHttpFileResponse::HasAllocMem()
{
	return m_bAlloc;
}

void em::EmHttpFileResponse::SetRecvOnceSize( int iRecvOnceSize )
{
	m_iRecvOnceSize = iRecvOnceSize;
}
