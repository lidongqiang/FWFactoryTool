#ifndef _h_EmTcpServerAcceptWorker
#define _h_EmTcpServerAcceptWorker

#include "EmBase.h"
#include "IEmTcpServerAcceptCallback.h"
#include "IEmStateNoticer.h"

#include <map>
#include <string>
#include <vector>



#ifndef SOCKET
typedef unsigned int SOCKET;
#endif 

namespace em
{
	class EmTcpConnectWorker;
	class IEmTcpServerAcceptCallback;
	class IEmStateNoticer;
	class EmTcpServerAcceptThread;
	class EmTcpServerCleanThread;

	class EmTcpServerAcceptWorker
	{
	public:

		void DebugTraceFile(const char* szMsg);

		EmTcpServerAcceptWorker();
		virtual ~EmTcpServerAcceptWorker();

		int SendCmdClose(INT64 iKey);
		int SendCmdText(const char* szSend, INT64 iKey);
		int SendCmdFile(const char* szLocalFileName, const char* szRemoteFileName, INT64 iKey);
		int SendCmdRaw(const char* bufRaw, int iSize, INT64 iKey);

		void SetRecvTimeout(int iRecvTimeout);
		void SetRecvBufSize(int iRecvBufSize);
		void SetRecvMaxIdle(int iRecvMaxIdle);
		void SetSendFileSleep(int iSendFileSleep);
		void SetSendTimeout(int iSendTimeout);
		void SetSendMaxIdle(int iSendMaxIdle);
		void SetSendBufSize(int iSendBufSize);
		void SetAcceptMaxIdle(int iAcceptMaxIdle);
		void SetAcceptTimeout(int iAcceptTimeout);
		void SetAcceptCallback(IEmTcpServerAcceptCallback* pAcceptCallback);
		void SetRunnningStateNoticer(IEmStateNoticer* pRunningStateNoticer);
		int Bind(const char* szHostName, int iHostPort);
		int Start();
		void Stop();
		bool IsHanging();
		bool IsWorking();
		bool HasBind();
		void ProcAccept();
		void ProcClean();
		void FinalClean();

	public:
		volatile bool m_bIsAccepting;
		volatile bool m_bIsCleaning;
		volatile bool m_bNeedWorking;
		volatile bool m_bBind;
		volatile bool m_bStop;
		volatile bool m_bStart;

	private:

		IEmStateNoticer* m_pRunningStateNoticer;
		std::string m_strHostName;
		int m_iHostPort;
		SOCKET m_xSockServer;
		
		int m_iRecvTimeout;
		int m_iRecvBufSize;
		int m_iRecvMaxIdle;
		int m_iSendTimeout;
		int m_iSendMaxIdle;
		int m_iSendBufSize;
		int m_iSendFileSleep;

		int m_iAcceptTimeout;
		int m_iAcceptMaxIdle;
		INT64 m_iAcceptLastTime;

		std::map<INT64,EmTcpConnectWorker*>* m_pConnectTable;
		IEmTcpServerAcceptCallback* m_pAcceptCallback;
		EmTcpServerAcceptThread* m_pAcceptTh;
		EmTcpServerCleanThread* m_pCleanTh;
	};
}
#endif