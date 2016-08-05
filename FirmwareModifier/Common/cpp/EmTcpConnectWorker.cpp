#include "EmTcpConnectWorker.h"
#include "EmTcpSendThread.h"
#include "EmTcpRecvThread.h"
#include "EmTcpCmd.h"
#include "EmTcpPackWriter.h"
#include "IEmTcpConnectCallback.h"
#include "EmNet.h"
#include "EmSz.h"
#include "EmFile.h"
#include "EmStlStr.h"
#include "EmHandy.h"
#include "EmTime.h"
#include "EmStlStr.h"
#include "IEmStateNoticer.h"
#include <WinSock2.h>

using namespace em;
using namespace std;

em::EmTcpConnectWorker::EmTcpConnectWorker(SOCKET xSock, INT64 iKey)
{
	m_xSockClient = xSock;
	m_iKey = iKey;

	m_iRecvTimeout = EM_TCP_DEFAULT_RECV_TIMEOUT;
	m_iRecvBufSize = EM_TCP_DEFAULT_RECV_BUF_SIZE;
	m_iRecvMaxIdle = EM_TCP_DEFAULT_RECV_MAX_IDLE;

	m_iSendFileSleep = EM_TCP_DEFAULT_SEND_FILE_SLEEP;
	m_iSendTimeout = EM_TCP_DEFAULT_SEND_TIMEOUT;
	m_iSendBufSize = EM_TCP_DEFAULT_SEND_BUF_SIZE;
	m_iSendMaxIdle = EM_TCP_DEFAULT_SEND_MAX_IDLE;

	m_iRecvLastTime = 0;
	m_iSendLastTime = 0;
	m_iLastActionTime = 0;

	m_bNeedWorking = true;
	m_bStarted = false;
	m_bStoppped = false;
	m_bIsRecving = false;
	m_bIsSending = false;

	m_iWorkSide = EM_TCP_SIDE_CLIENT;
	m_bufRecv = NULL;
	m_bufSend = NULL;
	m_pConnectCallback = NULL;
	
	int iResult = 0;
	try{
		m_pRecvThread = new EmTcpRecvThread();
		m_pRecvThread->SetWorker(this);
		m_pSendThread = new EmTcpSendThread();
		m_pSendThread->SetWorker(this);
		m_pCommandList = new std::vector<EmTcpCmd*>();

	}catch(...){
	}
}

em::EmTcpConnectWorker::~EmTcpConnectWorker()
{
	Stop();
	EM_FREE_NULL(m_bufSend);
	EM_FREE_NULL(m_bufRecv);
	EM_DELETE_NULL(m_pRecvThread);
	EM_DELETE_NULL(m_pSendThread);
	EM_DELETE_NULL(m_pCommandList);
}

int em::EmTcpConnectWorker::InitVariables()
{
	try{
		EM_FREE_NULL(m_bufSend);
		EM_FREE_NULL(m_bufRecv);
		m_bufRecv = (char*)malloc(m_iRecvBufSize);
		m_bufSend = (char*)malloc(m_iSendBufSize);
		EmNet::SocketSetSendBufSize(m_xSockClient,m_iSendBufSize);
		EmNet::SocketSetRecvBufSize(m_xSockClient,m_iRecvBufSize);
		m_pCommandList->clear();
	}catch(...){
		m_bNeedWorking = false;
	}
	return 0;
}

int em::EmTcpConnectWorker::Start()
{
	int iResult = 0;
	if(m_pConnectCallback != NULL){
		try{
			iResult = m_pConnectCallback->OnConnect(m_xSockClient,m_iKey);
		}catch(...){
		}
	}
	m_bNeedWorking = true;
	m_bStarted = true;
	m_bStoppped = false;
	m_pRecvThread->Start();
	m_pSendThread->Start();
	return 0;
}

void em::EmTcpConnectWorker::Stop()
{
	
// 	if(m_iWorkSide == EM_TCP_SIDE_SERVER){
// 		EmHandy::DebugTraceFile("c:/tcp.txt","em::EmTcpConnectWorker::Stop 0 server");
// 	}
// 	if(m_iWorkSide == EM_TCP_SIDE_CLIENT){
// 		EmHandy::DebugTraceFile("c:/tcp.txt","em::EmTcpConnectWorker::Stop 0 client");
// 	}

	//EmHandy::DebugTraceFile("c:/tcp.txt","EmTcpConnectWorker::Stop 0");
	
	if(m_bStoppped){
		return;
	}

	//EmHandy::DebugTraceFile("c:/tcp.txt","EmTcpConnectWorker::Stop 1");

// 	if(m_iWorkSide == EM_TCP_SIDE_SERVER){
// 		EmHandy::DebugTraceFile("c:/tcp.txt","em::EmTcpConnectWorker::Stop 1 server");
// 	}
// 	if(m_iWorkSide == EM_TCP_SIDE_CLIENT){
// 		EmHandy::DebugTraceFile("c:/tcp.txt","em::EmTcpConnectWorker::Stop 1 client");
// 	}

	m_bStoppped = true;
	m_bStarted = false;

	int iResult = 0;
	string strTemp = "";

	m_bNeedWorking = false;
	
	//EmNet::SocketShutdown(m_xSockClient);

// 	if(m_iWorkSide == EM_TCP_SIDE_SERVER){
// 		EmHandy::DebugTraceFile("c:/tcp.txt","em::EmTcpConnectWorker::Stop 11 server");
// 	}
// 	if(m_iWorkSide == EM_TCP_SIDE_CLIENT){
// 		EmHandy::DebugTraceFile("c:/tcp.txt","em::EmTcpConnectWorker::Stop 11 client");
// 	}

	if(m_pConnectCallback != NULL){
		try{
			iResult = m_pConnectCallback->OnClose(m_xSockClient,m_iKey);
		}catch(...){
		}
	}

	//EmHandy::DebugTraceFile("c:/tcp.txt","EmTcpConnectWorker::Stop 2");

// 	if(m_iWorkSide == EM_TCP_SIDE_SERVER){
// 		EmHandy::DebugTraceFile("c:/tcp.txt","em::EmTcpConnectWorker::Stop 2 server");
// 	}
// 	if(m_iWorkSide == EM_TCP_SIDE_CLIENT){
// 		EmHandy::DebugTraceFile("c:/tcp.txt","em::EmTcpConnectWorker::Stop 2 client");
// 	}

	while(true){
		if(HasExit()){
			break;
		}else{
			::Sleep(10);
		}
	}

	//EmHandy::DebugTraceFile("c:/tcp.txt","EmTcpConnectWorker::Stop 3");

// 	if(m_iWorkSide == EM_TCP_SIDE_SERVER){
// 		EmHandy::DebugTraceFile("c:/tcp.txt","em::EmTcpConnectWorker::Stop 3 server");
// 	}
// 	if(m_iWorkSide == EM_TCP_SIDE_CLIENT){
// 		EmHandy::DebugTraceFile("c:/tcp.txt","em::EmTcpConnectWorker::Stop 3 client");
// 	}

	while(true){
		if(m_pSendThread->IsSafeEnd()){
			break;
		}else{
			::Sleep(10);
		}
	}

	//EmHandy::DebugTraceFile("c:/tcp.txt","EmTcpConnectWorker::Stop 4");

// 	if(m_iWorkSide == EM_TCP_SIDE_SERVER){
// 		EmHandy::DebugTraceFile("c:/tcp.txt","em::EmTcpConnectWorker::Stop 4 server");
// 	}
// 	if(m_iWorkSide == EM_TCP_SIDE_CLIENT){
// 		EmHandy::DebugTraceFile("c:/tcp.txt","em::EmTcpConnectWorker::Stop 4 client");
// 	}

	while(true){
		if(m_pRecvThread->IsSafeEnd()){
			break;
		}else{
			::Sleep(10);
		}
	}

	//EmHandy::DebugTraceFile("c:/tcp.txt","EmTcpConnectWorker::Stop 5");

// 	if(m_iWorkSide == EM_TCP_SIDE_SERVER){
// 		EmHandy::DebugTraceFile("c:/tcp.txt","em::EmTcpConnectWorker::Stop 5 server");
// 	}
// 	if(m_iWorkSide == EM_TCP_SIDE_CLIENT){
// 		EmHandy::DebugTraceFile("c:/tcp.txt","em::EmTcpConnectWorker::Stop 5 client");
// 	}

	EmNet::SocketClose(&m_xSockClient);

	m_pConnectCallback = NULL;

	while(true){
		if(m_pCommandList->size() == 0){
			break;
		}
		std::vector<EmTcpCmd*>::iterator itCmd;
		itCmd = m_pCommandList->begin();
		EmTcpCmd* pCmd = *itCmd;
		if(pCmd != NULL){
			delete pCmd;
			pCmd = NULL;
		}
		m_pCommandList->erase(itCmd);
	}

	//EmHandy::DebugTraceFile("c:/tcp.txt","EmTcpConnectWorker::Stop 6");

// 	if(m_iWorkSide == EM_TCP_SIDE_SERVER){
// 		EmHandy::DebugTraceFile("c:/tcp.txt","em::EmTcpConnectWorker::Stop 6 server");
// 	}
// 	if(m_iWorkSide == EM_TCP_SIDE_CLIENT){
// 		EmHandy::DebugTraceFile("c:/tcp.txt","em::EmTcpConnectWorker::Stop 6 client");
// 	}
}

int em::EmTcpConnectWorker::SendByCmd( EmTcpCmd* pCmd )
{
	int iTotalSize = 0;
	int iResult = 0;
	int iSendSize = 0;
	INT64 iTimeStamp = EmTime::CurrentStamp13();
	if(pCmd->m_iType == EM_TCP_CMD_FILE){

		int iFailCount = 0;
		int iFileSize = 0;
		int iOffset = 0;
		int iPartSize = 0;
		int iPackSize = 0;
		int iBasePartSize = m_iSendBufSize - EM_TCP_PACK_MIN_SIZE - EM_TCP_PACK_FILE_META_SIZE;
		if(iBasePartSize < 0){
			return 0;
		}

		int iCompleteFileSize = 0;

		if(!EmFile::IsFile(pCmd->m_pstrLocalFileName->c_str())){
			return EM_ERR_FILE_LOST;
		}
		iFileSize = EmFile::Size(pCmd->m_pstrLocalFileName->c_str());

		while(true){

			if(m_pRunningStateNoticer != NULL){
				if(m_pRunningStateNoticer->GetState() == 0){
					break;
				}
			}

			if(!m_bNeedWorking){
				break;
			}
			if(iCompleteFileSize >= iFileSize){
				break;
			}
			
			iPartSize = (iFileSize - iCompleteFileSize) > iBasePartSize ? iBasePartSize : (iFileSize - iCompleteFileSize);

			iPackSize = EmTcpPackWriter::MakeFileBytes(m_bufSend
				,m_iRecvBufSize
				,pCmd->m_pstrLocalFileName->c_str()
				,pCmd->m_pstrRemoteFileName->c_str()
				,iFileSize
				,iOffset
				,iPartSize);
			
			if(iPackSize < 0){
				return iPackSize;
			}

			iSendSize = SendOnce((const char *)m_bufSend,iPackSize);
			if(iSendSize > 0){
				iFailCount = 0;
				iCompleteFileSize = iOffset + iPartSize;
				if(m_pConnectCallback != NULL){
					try{
						if(m_iWorkSide == EM_TCP_SIDE_CLIENT){
							//EmHandy::DebugTraceFile("c:/tcp.txt","OnSendBytesOnce file");
						}
						iResult = m_pConnectCallback->OnSendBytesOnce(m_xSockClient,m_iKey
							,m_bufSend,iCompleteFileSize,iFileSize,EM_TCP_CMD_FILE);
					}catch(...){
					}
				}
				iOffset = iCompleteFileSize;
				::Sleep(m_iSendFileSleep);
			}else if(iSendSize == 0){
				if(iFailCount++ >= EM_TCP_DEFAULT_SEND_MAX_FAIL){
					break;
				}
				continue;
			}else if(iSendSize < 0){
				return iSendSize;
			}
		}
	}else {
		char* bufHolder = NULL;
		if(pCmd->m_iType == EM_TCP_CMD_BEAT){
			iResult = SendOnce((const char *)EmTcpPackWriter::GetBeatBytes(),EM_TCP_PACK_MIN_SIZE);
			if(iResult < 0){
				return iResult;
			}
		}else if(pCmd->m_iType == EM_TCP_CMD_CLOSE){
			iResult = SendOnce((const char *)EmTcpPackWriter::GetCloseBytes(),EM_TCP_PACK_MIN_SIZE);
			if(iResult < 0){
				return iResult;
			}
		}else if(pCmd->m_iType == EM_TCP_CMD_TEXT){
			bufHolder = pCmd->m_bufContent;

			iResult = EmTcpPackWriter::MakeTextBytes(bufHolder,pCmd->m_iContentSize);
			if(iResult < 0){
				return iResult;
			}
			iResult = SendOnce((const char *)bufHolder,iResult);
			if(iResult < 0){
				return iResult;
			}
		}else if(pCmd->m_iType == EM_TCP_CMD_RAW){
			bufHolder = pCmd->m_bufContent;
			iResult = EmTcpPackWriter::MakeRawBytes(bufHolder,pCmd->m_iContentSize);
			if(iResult < 0){
				return iResult;
			}
			iResult = SendOnce((const char *)bufHolder,iResult);
			if(iResult < 0){
				return iResult;
			}
		}

		if(iResult > 0){
			iTotalSize = iResult;
			if(m_pConnectCallback != NULL){
				try{
					iResult = m_pConnectCallback->OnSendBytesOnce(m_xSockClient,m_iKey
						,bufHolder,iResult,iTotalSize,pCmd->m_iType);
				}catch(...){
				}
			}
		}
	}
	return iTotalSize;
}

void em::EmTcpConnectWorker::ProcRecv()
{
	string strTemp = "";
	int iReadSize = 0;
	int iResult = 0;
	INT64 iTimeStamp = EmTime::CurrentStamp13();
	m_iRecvLastTime = iTimeStamp;
	m_iLastActionTime = m_iRecvLastTime;
	m_bIsRecving = true;

	try{
		while(true)
		{
			if(m_pRunningStateNoticer->GetState() == 0){

				break;
			}


			if(!m_bNeedWorking
				|| m_xSockClient == INVALID_SOCKET
			){
				break;
			}


			iReadSize = 0;
			iTimeStamp = EmTime::CurrentStamp13();

			if(iTimeStamp - m_iLastActionTime >= m_iRecvMaxIdle){
				if(m_pConnectCallback != NULL){
					try{
						iResult = m_pConnectCallback->OnRecvMaxIdle(m_xSockClient,m_iKey);
					}catch(...){
					}
				}
				break;
			}

			iReadSize = RecvOnce((char*)m_bufRecv,m_iRecvBufSize);

			if(iReadSize > 0){
				if(m_pConnectCallback != NULL){
					try{
						iResult = m_pConnectCallback->OnRecvBytesOnce(m_xSockClient,m_iKey,m_bufRecv,iReadSize);
						if(iResult == EM_TCP_ACTION_CONNECTION_CLOSE){
							break;
						}
					}catch(...){
					}
				}
			}else if(iReadSize == 0){
				continue;
			}else{
				break;
			}

		}
	}catch(...){
	}
	m_bIsRecving = false;
	m_bNeedWorking = false;

}

void em::EmTcpConnectWorker::ProcSend()
{
	string strTemp;
	int iSendSize = 0;
	int iResult = 0;
	INT64 iTimeStamp = EmTime::CurrentStamp13();
	m_iSendLastTime = iTimeStamp;
	m_iLastActionTime = m_iSendLastTime;
	m_bIsSending = true;

	try{
		while(true){

			if(m_pRunningStateNoticer->GetState() == 0){

				break;
			}

			if(!m_bNeedWorking
				|| m_xSockClient == INVALID_SOCKET
			){
				break;
			}
			
			iSendSize = 0;
			iTimeStamp = EmTime::CurrentStamp13();
			
			if(m_pCommandList->size() > 0){

				EmTcpCmd* pCmd = *m_pCommandList->begin();
				

				iResult = SendByCmd(pCmd);

				delete pCmd;
				pCmd = NULL;
				m_pCommandList->erase(m_pCommandList->begin());
				if(iResult < 0){
					break;
				}
			}else{
				if(iTimeStamp - m_iSendLastTime >= m_iSendMaxIdle){
					SendCmdBeat();
				}else{
					::Sleep(100);
				}
			}
		}
	}catch(...){
	}
	m_bIsSending = false;
	m_bNeedWorking = false;

}

int em::EmTcpConnectWorker::SendOnce(const char* bufData, int iSize)
{
	int iResult = 0;
	int iError = 0;
	try{
		iResult = send(m_xSockClient,bufData,iSize,0);
		if(iResult < 0){
			iError = ::WSAGetLastError();
		}

		if(iResult < 0){
			if(iError == WSAETIMEDOUT || iError == 0){
				return 0;
			}
			if(m_pConnectCallback != NULL){
				try{
					iResult = m_pConnectCallback->OnSendException(m_xSockClient,m_iKey,iError);
				}catch(...){
				}
			}
			m_bNeedWorking = false;
			return iResult;
		}
	}catch(...){
		if(m_pConnectCallback != NULL){
			try{
				iResult = m_pConnectCallback->OnSendException(m_xSockClient,m_iKey,0);
			}catch(...){
			}
		}
		m_bNeedWorking = false;
		iResult = -1;
	}
	m_iSendLastTime = EmTime::CurrentStamp13();
	m_iLastActionTime = m_iSendLastTime;
	return iResult;
}

int em::EmTcpConnectWorker::RecvOnce(char* bufData, int iSize)
{
	int iResult = 0;
	int iError = 0;
	try{
		iResult = recv(m_xSockClient,bufData,iSize,0);
		if(iResult < 0){
			iError = ::WSAGetLastError();
		}

		if(iResult < 0){
			if(iError == WSAETIMEDOUT || iError == 0){
				return 0;
			}
			if(m_pConnectCallback != NULL){
				try{
					iResult = m_pConnectCallback->OnRecvException(m_xSockClient,m_iKey,iError);
				}catch(...){
				}
			}
			m_bNeedWorking = false;
			return iResult;
		}else{
			m_iRecvLastTime = EmTime::CurrentStamp13();
			m_iLastActionTime = m_iRecvLastTime;
		}
	}catch(...){
		if(m_pConnectCallback != NULL){
			try{
				iResult = m_pConnectCallback->OnRecvException(m_xSockClient,m_iKey,0);
			}catch(...){
			}
		}
		m_bNeedWorking = false;
		iResult = -1;
	}
	return iResult;
}

std::string em::EmTcpConnectWorker::FetchTargetAddr()
{
	if(m_xSockClient == INVALID_SOCKET){
		return "";
	}
	return EmNet::SocketPeerAddress(m_xSockClient);
}

int em::EmTcpConnectWorker::FetchTargetPort()
{
	if(m_xSockClient == INVALID_SOCKET){
		return 0;
	}
	return EmNet::SocketPeerPort(m_xSockClient);
}

bool em::EmTcpConnectWorker::HasExit()
{
	if(!m_bIsRecving && !m_bIsSending && !m_bNeedWorking){
		return true;
	}
	return false;
}

SOCKET em::EmTcpConnectWorker::GetClientSock()
{
	return m_xSockClient;
}

int em::EmTcpConnectWorker::SendCmdBeat()
{
	EmTcpCmd* pCmd = new EmTcpCmd();
	pCmd->MakeBeatCmd();
	m_pCommandList->push_back(pCmd);
	return 0;
}

int em::EmTcpConnectWorker::SendCmdClose()
{
	EmTcpCmd* pCmd = new EmTcpCmd();
	pCmd->MakeCloseCmd();
	m_pCommandList->push_back(pCmd);
	return 0;
}

int em::EmTcpConnectWorker::SendCmdText( const char* szText )
{
	EmTcpCmd* pCmd = new EmTcpCmd();
	pCmd->MakeTextCmd(szText);
	m_pCommandList->push_back(pCmd);
	return 0;
}

int em::EmTcpConnectWorker::SendCmdFile(const char* szLocalFileName, const char* szRemoteFileName)
{
	EmTcpCmd* pCmd = new EmTcpCmd();
	pCmd->MakeFileCmd(szLocalFileName,szRemoteFileName);
	m_pCommandList->push_back(pCmd);
	return 0;
}

int em::EmTcpConnectWorker::SendCmdRaw(const char* bufContent, int iSize )
{
	EmTcpCmd* pCmd = new EmTcpCmd();
	pCmd->MakeRawCmd(bufContent,iSize);
	m_pCommandList->push_back(pCmd);
	return 0;
}

void em::EmTcpConnectWorker::SetSendTimeout( int iSendTimeout )
{
	m_iSendTimeout = iSendTimeout;
}

void em::EmTcpConnectWorker::SetSendBufSize( int iSendBufSize )
{
	m_iSendBufSize = iSendBufSize;
}

void em::EmTcpConnectWorker::SetSendFileSleep( int iSendFileSleep )
{
	m_iSendFileSleep = iSendFileSleep;
}

void em::EmTcpConnectWorker::SetSendMaxIdle( int iSendMaxIdle )
{
	m_iSendMaxIdle = iSendMaxIdle;	
}

void em::EmTcpConnectWorker::SetRecvBufSize( int iBufSize )
{
	m_iRecvBufSize = iBufSize;
}

void em::EmTcpConnectWorker::SetRecvMaxIdle( int iRecvMaxIdle )
{
	m_iRecvMaxIdle = iRecvMaxIdle;
}

void em::EmTcpConnectWorker::SetRecvTimeout( int iRecvTimeout )
{
	m_iRecvTimeout = iRecvTimeout;
}

void em::EmTcpConnectWorker::SetWorkSide( int iSide )
{
	m_iWorkSide = iSide;
}

void em::EmTcpConnectWorker::SetConnectCallback( IEmTcpConnectCallback* pConnectCallback )
{
	m_pConnectCallback = pConnectCallback;
}

bool em::EmTcpConnectWorker::IsWorking()
{
	if(m_bNeedWorking && m_bIsSending && m_bIsRecving && m_xSockClient != INVALID_SOCKET){
		return true;
	}
	return false;
}

void em::EmTcpConnectWorker::NoticeStop()
{
	m_bNeedWorking = false;
}

void em::EmTcpConnectWorker::DebugTraceFile( const char* szMsg )
{
// 	if(m_iWorkSide != EM_TCP_SIDE_CLIENT){
// 		return;
// 	}
	EmFile::AppendUtf8("c:/tcp.txt",szMsg,EmEncodeUtf8);
}

void em::EmTcpConnectWorker::SetRunnningStateNoticer( IEmStateNoticer* pRunningStateNoticer )
{
	m_pRunningStateNoticer = pRunningStateNoticer;
}

bool em::EmTcpConnectWorker::IsHanging()
{
	if(m_bStarted && !m_bStoppped && !m_bNeedWorking){
		return true;
	}
	return false;
}
