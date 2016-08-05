#ifndef _h_EmTcpServer
#define _h_EmTcpServer

#include "EmBase.h"
#include "EmTcpBase.h"
#include "IEmStateNoticer.h"

namespace em
{
	class EmTcpServerAcceptWorker;
	class IEmTcpServerAcceptCallback;
	class EmTcpServerMonitorThread;

	class EmTcpServer : public virtual IEmStateNoticer
	{
	public:
		EmTcpServer();
		virtual ~EmTcpServer();

		void ProcMonitor();

		int SendCmdClose(INT64 iKey);
		int SendCmdText(const char* szSend, INT64 iKey);
		int SendCmdFile(const char* szLocalFileName, const char* szRemoteFileName, INT64 iKey);
		int SendCmdRaw(const char* bufRaw, int iSize, INT64 iKey);

		void SetRecvTimeout(int iRecvTimeout);
		void SetRecvBufSize(int iRecvBufSize);
		void SetRecvMaxIdle(int iRecvMaxIdle);
		void SetSendTimeout(int iSendTimeout);
		void SetSendBufSize(int iSendBufSize);
		void SetSendMaxIdle(int iSendMaxIdle);
		void SetSendFileSleep(int iSendFileSleep);

		void SetAcceptMaxIdle(int iAcceptMaxIdle);
		void SetAcceptTimeout(int iAcceptTimeout);

		int Start(const char* szHost, int iPort
			, IEmTcpServerAcceptCallback* pAcceptCallback);
		void Stop();
		
		

		virtual int GetState();
		virtual void SetState(int iState);
		
	public:
		volatile int m_iRunningState;
		volatile bool m_bNeedMonitoring;

	private:
		
		int m_iSendTimeout;
		int m_iSendBufSize;
		int m_iSendMaxIdle;
		int m_iSendFileSleep;
		int m_iRecvTimeout;
		int m_iRecvBufSize;
		int m_iRecvMaxIdle;
		int m_iAcceptTimeout;
		int m_iAcceptMaxIdle;
		EmTcpServerAcceptWorker *m_pAcceptWorker;
		EmTcpServerMonitorThread* m_pMonitorThread;
	};
}
#endif