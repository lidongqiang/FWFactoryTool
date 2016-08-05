#include "stdafx.h"
#include "../inc/EmThread.h"
#include <afx.h>

using namespace em;

std::map<INT64,EmThread*> EmThread::s_xTable;

EmThread::EmThread(){
	m_fCreateFlag = 0;
	m_iStackSize = 0;
	m_iThId = 0;
	m_hTh = INVALID_HANDLE_VALUE;
	m_bStarted = false;
	m_bNaturalEnd = true;
	m_bForcibleEnd = false;
}

EmThread::~EmThread(){
	StopSafely();
	::CloseHandle(m_hTh);
}

unsigned long EmThread::SRun(void* pvParam){
	EmThread* pTh = (EmThread*)pvParam;
	pTh->RunWrapper();
	return 0;
}

void EmThread::Start(){
	
	if(IsAlive()){
		return;
	}
	m_bNaturalEnd = false;
	InitStopTag();
	m_hTh = ::CreateThread(NULL,m_iStackSize,EmThread::SRun,this,m_fCreateFlag,&m_iThId);
}

void EmThread::StopForcibly(){
	if(m_hTh != INVALID_HANDLE_VALUE){
		::TerminateThread(m_hTh,-1);
	}
	m_bForcibleEnd = true;
}

void* em::EmThread::GetHandle()
{
	return m_hTh;
}

unsigned long em::EmThread::GetId()
{
	return m_iThId;
}

void em::EmThread::SetCreateFlag( unsigned long fCreateFlag )
{
	m_fCreateFlag = fCreateFlag;
}

bool em::EmThread::IsAlive()
{
	if(!m_bStarted){
		return false;
	}

	if(m_bNaturalEnd){
		return false;
	}

	if(m_bForcibleEnd){
		return false;
	}

	unsigned long iExitCode = 0;
	bool bResult = ::GetExitCodeThread(m_hTh,&iExitCode);
	if(bResult){
		if(iExitCode == STILL_ACTIVE){
			return true;
		}
	}

	return false;
}

bool em::EmThread::IsStarted()
{
	return m_bStarted;
}

void em::EmThread::RunWrapper()
{
	PreRun();
	Run();
	PostRun();
}

void em::EmThread::PreRun()
{
	m_bStarted = true;
	m_bNaturalEnd = false;
	m_bForcibleEnd = false;
}

void em::EmThread::PostRun()
{
	m_bNaturalEnd = true;
}

void em::EmThread::StopSafely()
{
	
}

void em::EmThread::InitStopTag()
{
	
}

void em::EmThread::SetWorker( void* pWorker )
{
	m_pWorker = pWorker;
}

bool em::EmThread::IsSafeEnd()
{
	return m_bNaturalEnd;
}

void em::EmThread::AddToTable( INT64 iKey, EmThread* pTh )
{
	s_xTable[iKey] = pTh;
}

void em::EmThread::CleanInTable()
{
	std::vector<INT64> xKeyList;
	std::map<INT64,EmThread*>::iterator itTh;
	for(itTh = s_xTable.begin(); itTh != s_xTable.end(); itTh++){
		EmThread* pTh = itTh->second;
		if(pTh != NULL){
			if(pTh->IsSafeEnd()){
				xKeyList.push_back(itTh->first);
			}
		}
	}

	std::vector<INT64>::iterator itKey;
	for(itKey = xKeyList.begin(); itKey != xKeyList.end(); itKey++){
		INT64 iKey = *itKey;
		itTh = s_xTable.find(iKey);
		if(itTh == s_xTable.end()){
			continue;
		}
		EmThread* pTh = itTh->second;
		if(pTh == NULL){
			continue;
		}
		delete pTh;
		s_xTable.erase(itTh);
	}
}

