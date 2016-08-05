#include "EmTcpSendThread.h"
#include "EmTcpConnectWorker.h"

using namespace em;

void em::EmTcpSendThread::InitStopTag()
{
	((EmTcpConnectWorker*)m_pWorker)->m_bNeedWorking = true;
}

void em::EmTcpSendThread::StopSafely()
{
	((EmTcpConnectWorker*)m_pWorker)->m_bNeedWorking = false;
}

void em::EmTcpSendThread::Run()
{
	((EmTcpConnectWorker*)m_pWorker)->ProcSend();
}

void em::EmTcpSendThread::PostRun()
{
	EmThread::PostRun();
}

void em::EmTcpSendThread::PreRun()
{
	EmThread::PreRun();
}

