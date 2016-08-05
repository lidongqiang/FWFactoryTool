#include "EmTcpClientMonitorThread.h"
#include "EmTcpClient.h"

using namespace em;

void em::EmTcpClientMonitorThread::InitStopTag()
{
	((EmTcpClient*)m_pWorker)->m_bNeedMonitoring = true;
}

void em::EmTcpClientMonitorThread::StopSafely()
{
	((EmTcpClient*)m_pWorker)->m_bNeedMonitoring = false;
}

void em::EmTcpClientMonitorThread::Run()
{
	((EmTcpClient*)m_pWorker)->ProcMonitor();
}

void em::EmTcpClientMonitorThread::PreRun()
{
	EmThread::PreRun();
}

void em::EmTcpClientMonitorThread::PostRun()
{
	EmThread::PostRun();
}


