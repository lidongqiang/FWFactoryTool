#include "EmTcpCmd.h"

using namespace em;
using namespace std;

em::EmTcpCmd::EmTcpCmd()
{
	m_pstrLocalFileName = new string();
	m_pstrRemoteFileName = new string();
	m_bufContent = NULL;
	m_iType = 0;
	m_iContentSize = 0;
}

em::EmTcpCmd::~EmTcpCmd()
{
	EM_DELETE_NULL(m_pstrLocalFileName);
	EM_DELETE_NULL(m_pstrRemoteFileName);
	EM_FREE_NULL(m_bufContent);
}

int em::EmTcpCmd::MakeTextCmd( const char* szContent)
{
	try{
		m_iType = EM_TCP_CMD_TEXT;
		m_iContentSize = strlen(szContent);
		EM_FREE_NULL(m_bufContent);
		m_bufContent = (char*)malloc(m_iContentSize + EM_TCP_PACK_MIN_SIZE);
		if(m_iContentSize > 0){
			memcpy(m_bufContent + EM_TCP_PACK_HEAD_SIZE,szContent,m_iContentSize);
		}
	}catch(...){
		return EM_ERR_UNKNOWN;
	}

	return 0;
}

int em::EmTcpCmd::MakeFileCmd( const char* szLocalFileName,const char* szRemoteFileName )
{
	try{
		m_iType = EM_TCP_CMD_FILE;
		*m_pstrLocalFileName = szLocalFileName;
		*m_pstrRemoteFileName = szRemoteFileName;
	}catch(...){
		return EM_ERR_UNKNOWN;
	}

	return 0;
}

int em::EmTcpCmd::MakeRawCmd( const char* bufContent, int iContentSize )
{
	try{
		m_iType = EM_TCP_CMD_RAW;
		m_iContentSize = iContentSize;
		EM_FREE_NULL(m_bufContent);
		m_bufContent = (char*)malloc(iContentSize + EM_TCP_PACK_MIN_SIZE);
		memcpy(m_bufContent + EM_TCP_PACK_HEAD_SIZE,bufContent,iContentSize);
	}catch(...){
		return EM_ERR_UNKNOWN;
	}

	return 0;
}

int em::EmTcpCmd::MakeBeatCmd()
{
	m_iType = EM_TCP_CMD_BEAT;
	return 0;
}

int em::EmTcpCmd::MakeCloseCmd()
{
	m_iType = EM_TCP_CMD_CLOSE;
	return 0;
}
