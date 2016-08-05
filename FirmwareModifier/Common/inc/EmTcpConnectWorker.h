#ifndef _h_EmTcpConnectWorker
#define _h_EmTcpConnectWorker

#include "EmBase.h"
#include "EmTcpBase.h"

#include <string>
#include <vector>
#include <map>

namespace em
{
	class IEmTcpConnectCallback;
	class EmTcpRecvThread;
	class EmTcpSendThread;
	class EmTcpCmd;
	class IEmStateNoticer;

	class EmTcpConnectWorker
	{
	public:
		EmTcpConnectWorker(SOCKET xSock, INT64 iKey);
		virtual ~EmTcpConnectWorker();

		void DebugTraceFile(const char* szMsg);

		SOCKET GetClientSock();

		int SendCmdBeat();
		int SendCmdClose();
		int SendCmdText(const char* szText);
		int SendCmdFile(const char* szLocalFileName, const char* szRemoteFileName);
		int SendCmdRaw(const char* bufContent, int iSize);

		void SetSendFileSleep(int iSendFileSleep);
		void SetSendTimeout(int iSendTimeout);
		void SetSendBufSize(int iSendBufSize);
		void SetSendMaxIdle(int iSendMaxIdle);
		void SetRecvTimeout(int iRecvTimeout);
		void SetRecvBufSize(int iBufSize);
		void SetRecvMaxIdle(int iRecvMaxIdle);
		void SetRunnningStateNoticer(IEmStateNoticer* pRunningStateNoticer);
		
		void SetWorkSide(int iSide);
		void SetConnectCallback(IEmTcpConnectCallback* pConnectCallback);

		std::string FetchTargetAddr();
		int FetchTargetPort();
		
		int InitVariables();

		bool IsWorking();
		bool IsHanging();
		bool HasExit();
		int Start();
		void Stop();
		void NoticeStop();

		int SendByCmd(EmTcpCmd* pCmd);

		int SendOnce(const char* bufData, int iSize);
		int RecvOnce(char* bufData, int iSize);

		void ProcRecv();
		void ProcSend();
	public:
		IEmStateNoticer* m_pRunningStateNoticer;
		std::string m_strTargetAddr;
		int m_iTargetPort;
		SOCKET m_xSockClient;
		INT64 m_iKey;
		int m_iWorkSide;

		int m_iRecvBufSize;
		int m_iRecvMaxIdle;
		int m_iRecvTimeout;

		int m_iSendTimeout;
		int m_iSendBufSize;
		int m_iSendMaxIdle;
		int m_iSendFileSleep;

		INT64 m_iLastActionTime;
		INT64 m_iRecvLastTime;
		INT64 m_iSendLastTime;

		volatile bool m_bNeedWorking;
		volatile bool m_bIsRecving;
		volatile bool m_bIsSending;
		volatile bool m_bStoppped;
		volatile bool m_bStarted;
		
		IEmTcpConnectCallback *m_pConnectCallback;

		std::vector<EmTcpCmd*>* m_pCommandList;
		EmTcpRecvThread* m_pRecvThread;
		EmTcpSendThread* m_pSendThread;
		char* m_bufRecv;
		char* m_bufSend;
	};
}
#endif