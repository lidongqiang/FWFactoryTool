#include "EmHttpDownload.h"
#include "EmHttpBase.h"
#include "EmNetBase.h"
#include "EmHttp.h"
#include "EmFile.h"
#include "EmSz.h"
#include "EmStlStr.h"
#include "EmHttpFileResponse.h"


#include "EmHandy.h"
#include "EmStr.h"
using namespace em;

em::EmHttpDownload::EmHttpDownload()
{
	m_pResponse = new EmHttpFileResponse();
	m_iMaxSize = 0;
	m_iTimeout = 3000;
	m_pReceiver = NULL;
	m_iStartPos = 0;
	m_iNeedSize = 0;
	m_iHeadLenth = 0;
	m_iTotalReceived = 0;
	m_iSrcFileSize = 0;
	m_bNeedStop = false;
	m_bStarted = false;
	m_bComplete = false;
	m_iRecvOnceSize = EM_NET_DEFAULT_RECV_ONCE_SIZE;

}

em::EmHttpDownload::~EmHttpDownload()
{
	m_pResponse->FreeMem();
	delete m_pResponse;
	m_pResponse = NULL;
}

void em::EmHttpDownload::SetRemoteUrl( const char* szRemoteUrl )
{
	m_strRemoteUrl = szRemoteUrl;
}

std::string em::EmHttpDownload::GetRemoteUrl()
{
	return m_strRemoteUrl;
}

void em::EmHttpDownload::SetLocalPath( const char* szLocalPath )
{
	m_strLocalPath = szLocalPath;
}

std::string em::EmHttpDownload::GetLocalPath()
{
	return m_strLocalPath;
}

void em::EmHttpDownload::SetMaxSize( int iMaxSize )
{
	m_iMaxSize = iMaxSize;
}

int em::EmHttpDownload::GetMaxSize()
{
	return m_iMaxSize;
}

int em::EmHttpDownload::Start()
{
	

	
	if(m_iMaxSize <= 0)
	{
		return EM_ERR_OBJ_NOT_READY;
	}
	if(!m_pResponse->HasAllocMem())
	{
		m_pResponse->AllocMem(m_iMaxSize);
	}
	m_bComplete = false;

	m_bStarted = true;
	m_bNeedStop = false;
	int iResult = 0;
	

	//iResult = EmHttp::GetFileByUrl(m_xResponse,m_strRemoteUrl,this,m_iMaxSize,m_iTimeout);
	iResult = EmHttp::GetFileRangeByUrl(*m_pResponse, m_strRemoteUrl.c_str()
		, m_iStartPos, m_iNeedSize, m_iRecvOnceSize, this, m_iTimeout);
	if(iResult <= 0)
	{
		return iResult;
	}
	m_bComplete = true;

	return m_pResponse->GetContentLen();
}

void em::EmHttpDownload::SetTimeout( int iTimeout )
{
	m_iTimeout = iTimeout;
}

int em::EmHttpDownload::GetTimeout()
{
	return m_iTimeout;
}

void em::EmHttpDownload::FreeMem()
{
	m_pResponse->FreeMem();
}

int em::EmHttpDownload::Save()
{
	if(m_strLocalPath.length() == 0)
	{
		return EM_ERR_OBJ_NOT_READY;
	}

	if(!m_bComplete)
	{
		return 0;
	}

	int iResult = 0;
	if(m_iStartPos == 0){
		iResult = EmFile::WriteBuffer(m_strLocalPath.c_str(),m_pResponse->GetContentBuffer(),m_pResponse->GetContentLen());
	}else{
		iResult = EmFile::AppendBuffer(m_strLocalPath.c_str(),m_pResponse->GetContentBuffer(),m_pResponse->GetContentLen());
	}

	return iResult;
}

int em::EmHttpDownload::OnReceiveOverflow( const char *bufRecv, int iRecvLimit, int iRecvAlready )
{
	if(m_pReceiver == NULL)
	{
		return 0;
	}

	return m_pReceiver->OnReceiveOverflow(bufRecv,iRecvLimit,iRecvAlready);
	
}

int em::EmHttpDownload::OnReceiveOnce( char *bufRecv, int iRecvLimit, int iRecvAlready, int iRecvLast )
{
	m_iTotalReceived = iRecvAlready;
	if(m_iTotalReceived > 100 && m_iHeadLenth == 0){
		int iPos = EmSz::FindSafe(bufRecv,"\r\n\r\n",500);
		if(iPos != -1){
			m_iHeadLenth = iPos + 4;
		}
	}
	int iResult = 0;

	if(m_pReceiver != NULL)
	{
		iResult = m_pReceiver->OnReceiveOnce(bufRecv,iRecvLimit,iRecvAlready,iRecvLast);
	}

	if(iResult == EM_NET_RECEIVE_STOP || m_bNeedStop){
		iResult = EM_NET_RECEIVE_STOP;
	}
	
	return iResult;
}

void em::EmHttpDownload::SetReceiver( IEmNetStreamReceiver* pReceiver )
{
	m_pReceiver = pReceiver;
}

IEmNetStreamReceiver* em::EmHttpDownload::GetReceiver()
{
	return m_pReceiver;
}

int em::EmHttpDownload::AllInOne( const char* szRemoteUrl, const char* szLocalPath, int iMaxSize
								 , IEmNetStreamReceiver *pReceiver/*= NULL*/, int iTimeout /*= 3000*/ )
{
	SetRemoteUrl(szRemoteUrl);
	SetLocalPath(szLocalPath);
	SetMaxSize(iMaxSize);
	SetTimeout(iTimeout);
	SetReceiver(pReceiver);
	int iResult = 0;
	iResult = Start();
	if(iResult < 0)
	{
		FreeMem();
		return iResult;
	}
	iResult = Save();
	if(iResult < 0)
	{
		FreeMem();
		return iResult;
	}

	FreeMem();
	return iResult;
}

void em::EmHttpDownload::SetStartPos( int iStartPos )
{
	m_iStartPos = iStartPos;
}

int em::EmHttpDownload::GetStartPos()
{
	return m_iStartPos;
}

void em::EmHttpDownload::SetNeedSize( int iNeedSize )
{
	m_iNeedSize = iNeedSize;
}

int em::EmHttpDownload::GetNeedSize()
{
	return m_iNeedSize;
}

void em::EmHttpDownload::SetRecvOnceSize( int iRecvOnceSize )
{
	m_iRecvOnceSize = iRecvOnceSize;
	m_pResponse->SetRecvOnceSize(m_iRecvOnceSize);
}

int em::EmHttpDownload::GetRecvOnceSize()
{
	return m_iRecvOnceSize;
}

int em::EmHttpDownload::GetContentLength()
{
	if(m_iHeadLenth == 0){
		return 0;
	}

	return m_iTotalReceived - m_iHeadLenth;
}

void em::EmHttpDownload::Stop()
{
	m_bNeedStop = true;
}

int em::EmHttpDownload::GetSrcFileSize()
{
	return m_iSrcFileSize;
}

void em::EmHttpDownload::SetSrcFileSize( int iSrcFileSize )
{
	m_iSrcFileSize = iSrcFileSize;
}
