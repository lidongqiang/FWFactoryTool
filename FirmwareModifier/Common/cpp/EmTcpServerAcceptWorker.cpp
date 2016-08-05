#include "EmTcpServerAcceptWorker.h"
#include "EmTcpServerAcceptThread.h"
#include "EmTcpServerCleanThread.h"
#include "EmTcpConnectWorker.h"
#include "IEmTcpServerAcceptCallback.h"
#include "EmCs.h"
#include "EmNet.h"
#include "EmTime.h"
#include "EmHandy.h"
#include "EmFile.h"
#include <WinSock2.h>

#ifndef INVALID_SOCKET
#define INVALID_SOCKET (SOCKET)(~0)
#endif

using namespace em;
using namespace std;

em::EmTcpServerAcceptWorker::EmTcpServerAcceptWorker( )
{
	m_xSockServer = INVALID_SOCKET;
	m_bNeedWorking = true;
	m_bBind = false;
	m_bIsAccepting = false;
	m_bIsCleaning = false;
	
	m_bStop = false;
	m_bStart = false;

	m_iSendFileSleep = EM_TCP_DEFAULT_SEND_FILE_SLEEP;
	m_iSendTimeout = EM_TCP_DEFAULT_SEND_TIMEOUT;
	m_iSendBufSize = EM_TCP_DEFAULT_SEND_BUF_SIZE;
	m_iSendMaxIdle = EM_TCP_DEFAULT_SEND_MAX_IDLE;
	m_iRecvTimeout = EM_TCP_DEFAULT_RECV_TIMEOUT;
	m_iRecvBufSize = EM_TCP_DEFAULT_RECV_BUF_SIZE;
	m_iRecvMaxIdle = EM_TCP_DEFAULT_RECV_MAX_IDLE;

	m_iAcceptTimeout = EM_TCP_DEFAULT_ACCEPT_TIMEOUT;
	m_iAcceptMaxIdle = EM_TCP_DEFAULT_ACCEPT_MAX_IDLE;

	m_pAcceptCallback = NULL;
	try{
		m_pConnectTable = new std::map<INT64,EmTcpConnectWorker*>();
		m_pAcceptTh = new EmTcpServerAcceptThread();
		m_pAcceptTh->SetWorker(this);
		m_pCleanTh = new EmTcpServerCleanThread();
		m_pCleanTh->SetWorker(this);
	}catch(...){
	}
}

em::EmTcpServerAcceptWorker::~EmTcpServerAcceptWorker()
{
	Stop();
	EM_DELETE_NULL(m_pAcceptTh);
	EM_DELETE_NULL(m_pCleanTh);
	EM_DELETE_NULL(m_pConnectTable);
}

bool em::EmTcpServerAcceptWorker::IsWorking()
{
	if(m_bIsAccepting && m_bIsCleaning){
		return true;
	}
	return false;
}

int em::EmTcpServerAcceptWorker::Start()
{
	if(!m_bBind){
		return 0;
	}

	if(m_bStart){
		return 0;
	}
	m_bNeedWorking = true;
	m_bStart = true;
	m_bStop = false;
	
	m_pAcceptTh->Start();
	m_pCleanTh->Start();
	
	return 1;
}

void em::EmTcpServerAcceptWorker::Stop()
{
	//EmHandy::DebugTraceFile("c:/tcp.txt","EmTcpServerAcceptWorker::Stop 0");
	if(m_bStop){
		return;
	}

	//EmHandy::DebugTraceFile("c:/tcp.txt","EmTcpServerAcceptWorker::Stop 2");

	m_bStop = true;
	m_bStart = false;
	

	string strTemp;
	


	//EmNet::SocketShutdown(m_xSockServer);
	EmNet::SocketClose(&m_xSockServer);
	m_bBind = false;

	//EmHandy::DebugTraceFile("c:/tcp.txt","EmTcpServerAcceptWorker::Stop 3");
	int iResult = 0;
	//m_pRunningStateNoticer->SetState(0);
	
	m_bNeedWorking = false;

	while(true){
		if(!m_bIsAccepting){
			break;
		}else{
			::Sleep(10);
		}		
	}

	//EmHandy::DebugTraceFile("c:/tcp.txt","EmTcpServerAcceptWorker::Stop 4");

	while(true){
		if(!m_bIsCleaning){
			break;
		}else{
			//EmHandy::DebugTraceFile("c:/tcp.txt","EmTcpServerAcceptWorker::Stop 44");
			::Sleep(10);
		}
	}

	//EmHandy::DebugTraceFile("c:/tcp.txt","EmTcpServerAcceptWorker::Stop 5");
	while(true){
		if(m_pAcceptTh->IsSafeEnd()){
			break;
		}else{
			::Sleep(10);
		}
	}

	//EmHandy::DebugTraceFile("c:/tcp.txt","EmTcpServerAcceptWorker::Stop 6");

	while(true){
		if(m_pCleanTh->IsSafeEnd()){
			break;
		}else{
			::Sleep(10);
		}
	}

	//EmHandy::DebugTraceFile("c:/tcp.txt","EmTcpServerAcceptWorker::Stop 7");

	std::map<INT64,EmTcpConnectWorker*>::iterator itWorker;
	for(itWorker = m_pConnectTable->begin(); itWorker != m_pConnectTable->end(); itWorker++){
		INT64 iKeyTemp = itWorker->first;
		EmTcpConnectWorker* pWorker = itWorker->second;
		if(pWorker != NULL){
			pWorker->Stop();
		}
	}

	//EmHandy::DebugTraceFile("c:/tcp.txt","EmTcpServerAcceptWorker::Stop 8");
	while(true){
		if(m_pConnectTable->size() == 0){
			break;
		}
		itWorker = m_pConnectTable->begin();
		EmTcpConnectWorker* pWorker = itWorker->second;
		if(!pWorker->HasExit()){
			::Sleep(10);	
		}else if(pWorker != NULL){
			delete pWorker;
			m_pConnectTable->erase(itWorker);
		}
	}
	
	//EmHandy::DebugTraceFile("c:/tcp.txt","EmTcpServerAcceptWorker::Stop 9");
	if(m_pAcceptCallback != NULL){
		try{
			iResult = m_pAcceptCallback->OnAcceptClose();
		}catch(...){}
	}
	//EmHandy::DebugTraceFile("c:/tcp.txt","EmTcpServerAcceptWorker::Stop 10");

}

int em::EmTcpServerAcceptWorker::Bind(const char* szHostName, int iHostPort)
{
	if(m_bBind){
		return 0;
	}
	m_bBind = true;

	m_strHostName = szHostName;
	m_iHostPort = iHostPort;
	int iResult = 0;

	m_xSockServer = EmNet::SocketCreateTcp();
	EmNet::SocketSetTimeoutRecv(m_xSockServer,m_iRecvTimeout);
	EmNet::SocketSetTimeoutSend(m_xSockServer,m_iSendTimeout);
	if(m_xSockServer == INVALID_SOCKET){
		EmNet::SocketClose(&m_xSockServer);
		m_bBind = false;
		return EM_ERR_NET;
	}
	EmNetServer xServer;
	memset(&xServer,0,sizeof(xServer));
	
	xServer.nPort = m_iHostPort;
	if(m_strHostName.size() == 0){
		xServer.bAny = true;
	}else{
		xServer.bAny = false;
		strcpy(xServer.szHostName,m_strHostName.c_str());
	}
	iResult = EmNet::SocketBind(m_xSockServer,&xServer);
	if(iResult < 0){
		EmNet::SocketClose(&m_xSockServer);
		m_bBind = false;
		return iResult;
	}
	iResult = EmNet::TcpListen(m_xSockServer);
	if(iResult < 0){
		EmNet::SocketClose(&m_xSockServer);
		m_bBind = false;
		return iResult;
	}
	
	return 0;
}

void em::EmTcpServerAcceptWorker::ProcAccept()
{
	string strTemp;
	int iResult = 0;
	
	INT64 iTimeStamp = EmTime::CurrentStamp13();
	m_iAcceptLastTime = iTimeStamp;
	INT64 iKeyCurrent = 0;
	INT64 iKeyPrevious = 0;
	EmNetClient xClient;

	m_bIsAccepting = true;
	while(true){

		if(m_pRunningStateNoticer->GetState() == 0){
			break;
		}

		if(!m_bNeedWorking
		){
			break;
		}

		iTimeStamp = EmTime::CurrentStamp13();
		memset(&xClient,0,sizeof(xClient));
	
		if(iTimeStamp - m_iAcceptLastTime >= m_iAcceptMaxIdle){
			if(m_pAcceptCallback != NULL){
				try{
					iResult = m_pAcceptCallback->OnAcceptMaxIdle();
				}catch(...){}
			}
			break;
		}
		
		try
		{
			SOCKET xSockClientNew = EmNet::TcpAccept(m_xSockServer,&xClient);
			EmNet::SocketSetTimeoutRecv(xSockClientNew,m_iRecvTimeout);
			EmNet::SocketSetTimeoutSend(xSockClientNew,m_iSendTimeout);
			if(xSockClientNew == INVALID_SOCKET){
				break;
			}
			m_iAcceptLastTime = iTimeStamp;
			
 			iKeyPrevious = iKeyCurrent;
 			iKeyCurrent = EmHandy::GenerateLongKey();
 			EmTcpConnectWorker* pConnectWorker = new EmTcpConnectWorker(xSockClientNew,iKeyCurrent);
 			pConnectWorker->SetSendFileSleep(m_iSendFileSleep);
			pConnectWorker->SetSendTimeout(m_iSendTimeout);
			pConnectWorker->SetSendBufSize(m_iSendBufSize);
			pConnectWorker->SetSendMaxIdle(m_iSendMaxIdle);
			pConnectWorker->SetRecvBufSize(m_iRecvBufSize);
			pConnectWorker->SetRecvMaxIdle(m_iRecvMaxIdle);
			pConnectWorker->SetRecvTimeout(m_iRecvTimeout);
 			pConnectWorker->SetWorkSide(EM_TCP_SIDE_SERVER);
			pConnectWorker->SetRunnningStateNoticer(m_pRunningStateNoticer);
 			(*m_pConnectTable)[iKeyCurrent] = pConnectWorker;

			if(m_pAcceptCallback != NULL){
				try{
					iResult = m_pAcceptCallback->OnAcceptNewClient(xSockClientNew
						,iKeyCurrent,iKeyPrevious,m_pConnectTable);
					if(iResult == EM_TCP_ACTION_ACCEPT_NEXT){
						delete pConnectWorker;
						m_pConnectTable->erase(iKeyCurrent);
						continue;
					}
				}catch(...){
				}
			}

 			pConnectWorker->InitVariables();
 			pConnectWorker->Start();

		}catch(...){
			if(m_pAcceptCallback != NULL){
				try{
					iResult = m_pAcceptCallback->OnAcceptException(::WSAGetLastError());
				}catch(...){}
			}
			break;
		}
	}
	m_bIsAccepting = false;
	m_bNeedWorking = false;
}

void em::EmTcpServerAcceptWorker::ProcClean()
{
	string strTemp;
	m_bIsCleaning = true;
	while(true){
		if(m_pRunningStateNoticer->GetState() == 0){
			break;
		}

		if(!m_bNeedWorking){
			break;
		}
		
		::Sleep(100);

		//EmHandy::DebugTraceFile("c:/tcp.txt","EmTcpServerAcceptWorker::ProcClean 1");

		if(m_pConnectTable->size() == 0){
			//EmHandy::DebugTraceFile("c:/tcp.txt","EmTcpServerAcceptWorker::ProcClean 11");
			continue;
		}
		
		//EmHandy::DebugTraceFile("c:/tcp.txt","EmTcpServerAcceptWorker::ProcClean 2");

		std::vector<INT64> xKeyList;
		std::vector<INT64>::iterator itKey;
		std::map<INT64,EmTcpConnectWorker*>::iterator itWorker;
		
		for(itWorker = m_pConnectTable->begin(); itWorker != m_pConnectTable->end(); itWorker++){
			//EmHandy::DebugTraceFile("c:/tcp.txt","EmTcpServerAcceptWorker::ProcClean 22");
			INT64 iKeyTemp = itWorker->first;
			EmTcpConnectWorker* pWorker = itWorker->second;
			
			if(pWorker != NULL){
				if(pWorker->HasExit()){
					xKeyList.push_back(iKeyTemp);
				}else if(pWorker->IsHanging()){
					pWorker->Stop();
				}
				
			}
			//EmHandy::DebugTraceFile("c:/tcp.txt","EmTcpServerAcceptWorker::ProcClean 222");
		}

		//EmHandy::DebugTraceFile("c:/tcp.txt","EmTcpServerAcceptWorker::ProcClean 3");
		
		for(itKey = xKeyList.begin(); itKey != xKeyList.end(); itKey++){
			//EmHandy::DebugTraceFile("c:/tcp.txt","EmTcpServerAcceptWorker::ProcClean 33");
			INT64 iKeyTemp = *itKey;
			itWorker = m_pConnectTable->find(iKeyTemp);
			if(itWorker == m_pConnectTable->end()){
				//EmHandy::DebugTraceFile("c:/tcp.txt","EmTcpServerAcceptWorker::ProcClean 333");
				continue;
			}
			EmTcpConnectWorker* pWorker = itWorker->second;
			if(pWorker != NULL && pWorker->HasExit()){
				delete pWorker;
			}
			m_pConnectTable->erase(itWorker);
			//EmHandy::DebugTraceFile("c:/tcp.txt","EmTcpServerAcceptWorker::ProcClean 3333");
		}	
		//EmHandy::DebugTraceFile("c:/tcp.txt","EmTcpServerAcceptWorker::ProcClean 4");
	}
	//EmHandy::DebugTraceFile("c:/tcp.txt","EmTcpServerAcceptWorker::ProcClean 5");
	m_bIsCleaning = false;
	m_bNeedWorking = false;
}

int em::EmTcpServerAcceptWorker::SendCmdClose( INT64 iKey )
{
	if(!m_bBind){
		return 0;
	}
	if(m_pConnectTable->find(iKey) == m_pConnectTable->end()){
		return 0;
	}
	EmTcpConnectWorker* pConnectWorker = (*m_pConnectTable)[iKey];
	return pConnectWorker->SendCmdClose();
}

int em::EmTcpServerAcceptWorker::SendCmdText( const char* szSend, INT64 iKey )
{
	if(!m_bBind){
		return 0;
	}
	if(m_pConnectTable->find(iKey) == m_pConnectTable->end()){
		return 0;
	}
	EmTcpConnectWorker* pConnectWorker = (*m_pConnectTable)[iKey];
	return pConnectWorker->SendCmdText(szSend);
}

int em::EmTcpServerAcceptWorker::SendCmdFile( const char* szLocalFileName, const char* szRemoteFileName, INT64 iKey )
{
	if(!m_bBind){
		return 0;
	}
	if(m_pConnectTable->find(iKey) == m_pConnectTable->end()){
		return 0;
	}
	EmTcpConnectWorker* pConnectWorker = (*m_pConnectTable)[iKey];
	return pConnectWorker->SendCmdFile(szLocalFileName,szRemoteFileName);
}

int em::EmTcpServerAcceptWorker::SendCmdRaw( const char* bufRaw, int iSize, INT64 iKey )
{
	if(!m_bBind){
		return 0;
	}
	if(m_pConnectTable->find(iKey) == m_pConnectTable->end()){
		return 0;
	}
	EmTcpConnectWorker* pConnectWorker = (*m_pConnectTable)[iKey];
	return pConnectWorker->SendCmdRaw(bufRaw,iSize);
}

bool em::EmTcpServerAcceptWorker::HasBind()
{
	return m_bBind;
}

void em::EmTcpServerAcceptWorker::SetRecvTimeout( int iRecvTimeout )
{
	m_iRecvTimeout = iRecvTimeout;
}

void em::EmTcpServerAcceptWorker::SetRecvBufSize( int iRecvBufSize )
{
	m_iRecvBufSize = iRecvBufSize;
}

void em::EmTcpServerAcceptWorker::SetRecvMaxIdle( int iRecvMaxIdle )
{
	m_iRecvMaxIdle = iRecvMaxIdle;
}

void em::EmTcpServerAcceptWorker::SetSendTimeout( int iSendTimeout )
{
	m_iSendTimeout = iSendTimeout;
}

void em::EmTcpServerAcceptWorker::SetSendMaxIdle( int iSendMaxIdle )
{
	m_iSendMaxIdle = iSendMaxIdle;
}

void em::EmTcpServerAcceptWorker::SetSendFileSleep( int iSendFileSleep )
{
	m_iSendFileSleep = iSendFileSleep;
}

void em::EmTcpServerAcceptWorker::SetSendBufSize( int iSendBufSize )
{
	m_iSendBufSize = iSendBufSize;
}

void em::EmTcpServerAcceptWorker::SetAcceptMaxIdle( int iAcceptMaxIdle )
{
	m_iAcceptMaxIdle = iAcceptMaxIdle;
}

void em::EmTcpServerAcceptWorker::SetAcceptTimeout( int iAcceptTimeout )
{
	m_iAcceptTimeout = iAcceptTimeout;
}

void em::EmTcpServerAcceptWorker::SetAcceptCallback( IEmTcpServerAcceptCallback* pAcceptCallback )
{
	m_pAcceptCallback = pAcceptCallback;
}

void em::EmTcpServerAcceptWorker::SetRunnningStateNoticer( IEmStateNoticer* pRunningStateNoticer )
{
	m_pRunningStateNoticer = pRunningStateNoticer;
}

void em::EmTcpServerAcceptWorker::FinalClean()
{
	
}

void em::EmTcpServerAcceptWorker::DebugTraceFile( const char* szMsg )
{
	// 	if(m_iWorkSide != EM_TCP_SIDE_CLIENT){
	// 		return;
	// 	}
	//EmFile::AppendUtf8("c:/tcp.txt",szMsg,EmEncodeUtf8);
}

bool em::EmTcpServerAcceptWorker::IsHanging()
{
	if(m_bStart && !m_bStop && !m_bNeedWorking){
		return true;
	}

	return false;
}
