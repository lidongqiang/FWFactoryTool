#include "EmTcpClient.h"
#include "EmNet.h"
#include "EmTcpConnectWorker.h"
#include "EmTcpClientMonitorThread.h"
#include "EmSz.h"
#include "EmFile.h"
#include "EmStlStr.h"
#include "EmHandy.h"

using namespace em;

em::EmTcpClient::EmTcpClient()
{
	m_iRecvBufSize = EM_TCP_DEFAULT_RECV_BUF_SIZE;
	m_iRecvTimeout = EM_TCP_DEFAULT_RECV_TIMEOUT;
	m_iRecvMaxIdle = EM_TCP_DEFAULT_RECV_MAX_IDLE;
	m_iSendTimeout = EM_TCP_DEFAULT_SEND_TIMEOUT;
	m_iSendBufSize = EM_TCP_DEFAULT_SEND_BUF_SIZE;
	m_iSendMaxIdle = EM_TCP_DEFAULT_SEND_MAX_IDLE;
	m_iSendFileSleep = EM_TCP_DEFAULT_SEND_FILE_SLEEP;
	m_iServerPort = 0;
	m_pstrServerName = new std::string();
	m_pConnectWorker = NULL;
	m_iRunningState = 1;
	m_bNeedMonitoring = false;
	m_pMonitorThread = new EmTcpClientMonitorThread();
	m_pMonitorThread->SetWorker(this);
	
}

em::EmTcpClient::~EmTcpClient()
{
	m_bNeedMonitoring = false;
	while(!m_pMonitorThread->IsSafeEnd()){
		::Sleep(10);
	}
	if(m_pConnectWorker != NULL){
		m_pConnectWorker->Stop();
		EM_DELETE_NULL(m_pConnectWorker);
	}
	EM_DELETE_NULL(m_pstrServerName);
	EM_DELETE_NULL(m_pMonitorThread);
}


std::string em::EmTcpClient::GetServerName()
{
	return *m_pstrServerName;
}

int em::EmTcpClient::GetServerPort()
{
	return m_iServerPort;
}

EmTcpConnectWorker* em::EmTcpClient::GetConnectWorker()
{
	return m_pConnectWorker;
}

int em::EmTcpClient::SendCmdFile( const char* szLocalFileName, const char* szRemoteFileName )
{
	if(m_pConnectWorker == NULL){
		return 0;
	}
	return m_pConnectWorker->SendCmdFile(szLocalFileName,szRemoteFileName);
}

int em::EmTcpClient::SendCmdRaw( const char* bufSend, int iSize )
{
	if(m_pConnectWorker == NULL){
		return 0;
	}
	return m_pConnectWorker->SendCmdRaw(bufSend,iSize);
}

int em::EmTcpClient::SendCmdText( const char* szText)
{
	if(m_pConnectWorker == NULL){
		return 0;
	}
	return m_pConnectWorker->SendCmdText(szText);
}

int em::EmTcpClient::Connect( const char* szServerName, int iServerPort , IEmTcpConnectCallback* pConnectCallback )
{
	m_iRunningState = 1;
	int iResult = 0;
	try{
		if(m_pConnectWorker != NULL){
			m_pConnectWorker->Stop();
			EM_DELETE_NULL(m_pConnectWorker);
		}

		*m_pstrServerName = szServerName;
		m_iServerPort = iServerPort;

		m_xSock = EmNet::SocketCreateTcp();
		EmNet::SocketSetTimeoutRecv(m_xSock,m_iRecvTimeout);
		EmNet::SocketSetTimeoutSend(m_xSock,m_iSendTimeout);
		EmNetServer xServer;
		memset(&xServer,0,sizeof(xServer));
		strcpy(xServer.szHostName,m_pstrServerName->c_str());
		xServer.nPort = iServerPort;

		iResult = EmNet::TcpConnect(m_xSock,&xServer,3);
		if(iResult < 0){
			return iResult;
		}

		INT64 iKey = EmHandy::GenerateLongKey();
		m_pConnectWorker = new EmTcpConnectWorker(m_xSock,iKey);
		m_pConnectWorker->SetConnectCallback(pConnectCallback);
		m_pConnectWorker->SetRecvBufSize(m_iRecvBufSize);
		m_pConnectWorker->SetRecvMaxIdle(m_iRecvMaxIdle);
		m_pConnectWorker->SetRecvTimeout(m_iRecvTimeout);
		m_pConnectWorker->SetSendTimeout(m_iSendTimeout);
		m_pConnectWorker->SetSendFileSleep(m_iSendFileSleep);
		m_pConnectWorker->SetSendBufSize(m_iSendBufSize);
		m_pConnectWorker->SetSendMaxIdle(m_iSendMaxIdle);
		m_pConnectWorker->SetWorkSide(EM_TCP_SIDE_CLIENT);
		m_pConnectWorker->SetRunnningStateNoticer(this);
		m_pConnectWorker->InitVariables();
		
		m_pConnectWorker->Start();
		m_pMonitorThread->Start();

	}catch(...){
		return EM_ERR_NET_CONNECT;
	}

	return iResult;
}

void em::EmTcpClient::Close()
{
	m_iRunningState = 0;
	m_bNeedMonitoring = false;
	
	//EmHandy::DebugTraceFile("c:/tcp.txt","EmTcpClient::Close 1");

	while(!m_pMonitorThread->IsSafeEnd()){
		//EmHandy::DebugTraceFile("c:/tcp.txt","EmTcpClient::Close 11");
		::Sleep(10);
	}

	//EmHandy::DebugTraceFile("c:/tcp.txt","EmTcpClient::Close 2");
	
	if(m_pConnectWorker != NULL){
		m_pConnectWorker->Stop();
	}

	//EmHandy::DebugTraceFile("c:/tcp.txt","EmTcpClient::Close 3");
}

void em::EmTcpClient::SetRecvTimeout( int iTimeout )
{
	m_iRecvTimeout = iTimeout;
}

void em::EmTcpClient::SetRecvBufSize( int iSize )
{
	m_iRecvBufSize = iSize;
}

void em::EmTcpClient::SetRecvMaxIdle( int iMaxIdle )
{
	m_iRecvMaxIdle = iMaxIdle;
}

void em::EmTcpClient::SetSendFileSleep( int iSendFileSleep )
{
	m_iSendFileSleep = iSendFileSleep;
}

void em::EmTcpClient::SetSendTimeout( int iTimeout )
{
	m_iSendTimeout = iTimeout;
}

void em::EmTcpClient::SetSendBufSize( int iSize )
{
	m_iSendBufSize = iSize;
}

void em::EmTcpClient::SetSendMaxIdle( int iMaxIdle )
{
	m_iSendMaxIdle = iMaxIdle;
}

bool em::EmTcpClient::IsConnected()
{
	if(m_pConnectWorker == NULL){
		return false;
	}
	return m_pConnectWorker->IsWorking();
}

int em::EmTcpClient::GetClientPort()
{
	return EmNet::SocketPeerPort(m_xSock);
}

int em::EmTcpClient::GetState()
{
	return m_iRunningState;
}

void em::EmTcpClient::SetState( int iState )
{
	m_iRunningState = iState;
}

void em::EmTcpClient::ProcMonitor()
{
	while(true){
		if(!m_bNeedMonitoring){
			break;
		}
		::Sleep(100);
		//EmHandy::DebugTraceFile("c:/tcp.txt","client monitor");
		if(m_pConnectWorker->IsHanging()){
			m_pConnectWorker->Stop();
		}
	}
}
