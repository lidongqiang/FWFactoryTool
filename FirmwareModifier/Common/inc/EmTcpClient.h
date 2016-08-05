#ifndef _h_EmTcpClient
#define _h_EmTcpClient

#include "EmBase.h"
#include "EmTcpBase.h"
#include "IEmStateNoticer.h"

#include <string>

namespace em
{
	class EmTcpConnectWorker;
	class IEmTcpConnectCallback;
	class EmTcpClientMonitorThread;

	class EmTcpClient : public virtual IEmStateNoticer
	{
	public:
		EmTcpClient();
		virtual ~EmTcpClient();

		void ProcMonitor();

		int GetClientPort();
		std::string GetServerName();
		int GetServerPort();

		EmTcpConnectWorker* GetConnectWorker();

		int SendCmdFile(const char* szLocalFileName, const char* szRemoteFileName);
		int SendCmdRaw(const char* bufSend, int iSize);
		int SendCmdText(const char* szText);
		
		int Connect(const char* szServerName
			, int iServerPort
			, IEmTcpConnectCallback* pConnectCallback);
		void Close();
		bool IsConnected();
		
		void SetRecvTimeout(int iTimeout);
		void SetRecvBufSize(int iSize);
		void SetRecvMaxIdle(int iMaxIdle);
		void SetSendFileSleep(int iSendFileSleep);
		void SetSendTimeout(int iTimeout);
		void SetSendBufSize(int iSize);
		void SetSendMaxIdle(int iMaxIdle);

		virtual int GetState();
		virtual void SetState(int iState);

	public:
		volatile int m_iRunningState;
		bool m_bNeedMonitoring;

		int m_iRecvBufSize;
		int m_iRecvMaxIdle;
		int m_iRecvTimeout;
		int m_iSendTimeout;
		int m_iSendBufSize;
		int m_iSendMaxIdle;
		int m_iSendFileSleep;
		int m_iServerPort;

		SOCKET m_xSock;
		std::string* m_pstrServerName;
		EmTcpConnectWorker* m_pConnectWorker;
		EmTcpClientMonitorThread* m_pMonitorThread;
		
	};
}
#endif