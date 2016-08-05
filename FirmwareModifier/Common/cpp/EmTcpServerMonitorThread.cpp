#include "EmTcpServerMonitorThread.h"
#include "EmTcpServer.h"

using namespace em;

void em::EmTcpServerMonitorThread::InitStopTag()
{
	((EmTcpServer*)m_pWorker)->m_bNeedMonitoring = true;
}

void em::EmTcpServerMonitorThread::StopSafely()
{
	((EmTcpServer*)m_pWorker)->m_bNeedMonitoring = false;
}

void em::EmTcpServerMonitorThread::Run()
{
	((EmTcpServer*)m_pWorker)->ProcMonitor();
}

void em::EmTcpServerMonitorThread::PreRun()
{
	EmThread::PreRun();
}

void em::EmTcpServerMonitorThread::PostRun()
{
	EmThread::PostRun();
}


