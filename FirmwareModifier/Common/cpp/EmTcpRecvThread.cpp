#include "EmTcpRecvThread.h"
#include "EmTcpConnectWorker.h"

using namespace em;

void em::EmTcpRecvThread::InitStopTag()
{
	((EmTcpConnectWorker*)m_pWorker)->m_bNeedWorking = true;
}

void em::EmTcpRecvThread::StopSafely()
{
	((EmTcpConnectWorker*)m_pWorker)->m_bNeedWorking = false;
}

void em::EmTcpRecvThread::Run()
{
	((EmTcpConnectWorker*)m_pWorker)->ProcRecv();
}

void em::EmTcpRecvThread::PostRun()
{
	EmThread::PostRun();
}

void em::EmTcpRecvThread::PreRun()
{
	EmThread::PreRun();
}
