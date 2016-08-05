#include "EmTcpServer.h"
#include "EmTcpConnectWorker.h"
#include "EmTcpServerAcceptWorker.h"
#include "EmTcpServerMonitorThread.h"
#include "EmHandy.h"

using namespace em;

em::EmTcpServer::EmTcpServer()
{
	m_iSendFileSleep = EM_TCP_DEFAULT_SEND_FILE_SLEEP;
	m_iSendTimeout = EM_TCP_DEFAULT_SEND_TIMEOUT;
	m_iSendBufSize = EM_TCP_DEFAULT_SEND_BUF_SIZE;
	m_iSendMaxIdle = EM_TCP_DEFAULT_SEND_MAX_IDLE;
	m_iRecvTimeout = EM_TCP_DEFAULT_RECV_TIMEOUT;
	m_iRecvBufSize = EM_TCP_DEFAULT_RECV_BUF_SIZE;
	m_iRecvMaxIdle = EM_TCP_DEFAULT_RECV_MAX_IDLE;

	m_iAcceptTimeout = EM_TCP_DEFAULT_ACCEPT_TIMEOUT;
	m_iAcceptMaxIdle = EM_TCP_DEFAULT_ACCEPT_MAX_IDLE;

	m_pAcceptWorker = new EmTcpServerAcceptWorker();
	m_iRunningState = 1;
	m_bNeedMonitoring = false;
	m_pMonitorThread = new EmTcpServerMonitorThread();
	m_pMonitorThread->SetWorker(this);
	
}

em::EmTcpServer::~EmTcpServer()
{
	m_bNeedMonitoring = false;
	while(!m_pMonitorThread->IsSafeEnd()){
		::Sleep(10);
	}
	Stop();
	EM_DELETE_NULL(m_pAcceptWorker);
	EM_DELETE_NULL(m_pMonitorThread);
}

void em::EmTcpServer::SetRecvTimeout( int iRecvTimeout )
{
	m_iRecvTimeout = iRecvTimeout;
}

void em::EmTcpServer::SetRecvBufSize( int iRecvBufSize )
{
	m_iRecvBufSize = iRecvBufSize;
}

void em::EmTcpServer::SetRecvMaxIdle( int iRecvMaxIdle )
{
	m_iRecvMaxIdle = iRecvMaxIdle;
}

void em::EmTcpServer::SetSendTimeout( int iSendTimeout )
{
	m_iSendTimeout = iSendTimeout;
}

void em::EmTcpServer::SetSendMaxIdle( int iSendMaxIdle )
{
	m_iSendMaxIdle = iSendMaxIdle;
}

void em::EmTcpServer::SetSendBufSize( int iSendBufSize )
{
	m_iSendBufSize = iSendBufSize;
}

void em::EmTcpServer::SetSendFileSleep( int iSendFileSleep )
{
	m_iSendFileSleep = iSendFileSleep;
}

void em::EmTcpServer::SetAcceptMaxIdle( int iAcceptMaxIdle )
{
	m_iAcceptMaxIdle = iAcceptMaxIdle;
}

void em::EmTcpServer::SetAcceptTimeout( int iAcceptTimeout )
{
	m_iAcceptTimeout = iAcceptTimeout;
}

int em::EmTcpServer::SendCmdClose( INT64 iKey )
{
	return m_pAcceptWorker->SendCmdClose(iKey);
}

int em::EmTcpServer::SendCmdText( const char* szSend, INT64 iKey )
{
	return m_pAcceptWorker->SendCmdText(szSend,iKey);
}

int em::EmTcpServer::SendCmdFile( const char* szLocalFileName, const char* szRemoteFileName, INT64 iKey )
{
	return m_pAcceptWorker->SendCmdFile(szLocalFileName,szRemoteFileName,iKey);
}

int em::EmTcpServer::SendCmdRaw( const char* bufRaw,int iSize, INT64 iKey )
{
	return m_pAcceptWorker->SendCmdRaw(bufRaw,iSize,iKey);
}

int em::EmTcpServer::Start( const char* szHost, int iPort , IEmTcpServerAcceptCallback* pAcceptCallback )
{
	m_iRunningState = 1;
	int iResult = 0;
	try{
		if(m_pAcceptWorker->HasBind()){
			m_pAcceptWorker->Stop();
			EM_DELETE_NULL(m_pAcceptWorker);
			m_pAcceptWorker = new EmTcpServerAcceptWorker();
		}
		

		m_pAcceptWorker->SetAcceptCallback(pAcceptCallback);
		m_pAcceptWorker->SetAcceptMaxIdle(m_iAcceptMaxIdle);
		m_pAcceptWorker->SetAcceptTimeout(m_iAcceptTimeout);

		m_pAcceptWorker->SetRecvBufSize(m_iRecvBufSize);
		m_pAcceptWorker->SetRecvMaxIdle(m_iRecvMaxIdle);
		m_pAcceptWorker->SetRecvTimeout(m_iRecvTimeout);
		
		m_pAcceptWorker->SetSendBufSize(m_iSendBufSize);
		m_pAcceptWorker->SetSendMaxIdle(m_iSendMaxIdle);
		m_pAcceptWorker->SetSendTimeout(m_iSendTimeout);

		m_pAcceptWorker->SetRunnningStateNoticer(this);
		
		iResult = m_pAcceptWorker->Bind(szHost,iPort);
		if(iResult < 0){
			return iResult;
		}
		
		
		m_pAcceptWorker->Start();
		m_pMonitorThread->Start();
		/**/
		

	}catch(...){
		return EM_ERR_NET;
	}
	return iResult;
}

void em::EmTcpServer::Stop()
{
	m_iRunningState = 0;
	m_bNeedMonitoring = false;
	while(!m_pMonitorThread->IsSafeEnd()){
		::Sleep(10);
	}
	m_pAcceptWorker->Stop();
}

int em::EmTcpServer::GetState()
{
	return m_iRunningState;
}

void em::EmTcpServer::SetState( int iState )
{
	m_iRunningState = iState;
}

void em::EmTcpServer::ProcMonitor()
{
	while(true){
		if(!m_bNeedMonitoring){
			break;
		}
		::Sleep(100);
		//EmHandy::DebugTraceFile("c:/tcp.txt","server monitor");
		if(m_pAcceptWorker->IsHanging()){
			m_pAcceptWorker->Stop();
		}
		
	}
	
}
