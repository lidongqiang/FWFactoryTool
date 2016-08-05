#include "stdafx.h"
#ifndef EM_TURN_OFF_DEVICE_MODULE

#include "../inc/EmAdbDevice.h"
#include "../../Common/inc/EmStlStr.h"


using namespace em;

em::EmAdbDevice::EmAdbDevice():m_bInstalled(false)
{
}

em::EmAdbDevice::~EmAdbDevice()
{
	
}

std::string em::EmAdbDevice::GetName()
{
	return m_strName;
}

void em::EmAdbDevice::SetName( const char* szName )
{
	m_strName = EmStlStr::Lower(szName);
}

std::string em::EmAdbDevice::GetSerial()
{
	return m_strSerial;
}

void em::EmAdbDevice::SetSerial( const char* szSerial )
{
	m_strSerial = szSerial;
}

std::string em::EmAdbDevice::GetVid()
{
	return m_strVid;
}

void em::EmAdbDevice::SetVid( const char* szVid )
{
	m_strVid = EmStlStr::Lower(szVid);
}

std::string em::EmAdbDevice::GetPid()
{
	return m_strPid;
}

void em::EmAdbDevice::SetPid( const char* szPid )
{
	m_strPid = EmStlStr::Lower(szPid);
}

std::string em::EmAdbDevice::GetHardwareId()
{
	return m_strHardwareId;
}

void em::EmAdbDevice::SetHardwareId( const char* szHardwareId )
{
	m_strHardwareId = EmStlStr::Lower(szHardwareId);
	AppendHardwareId(m_strHardwareId.c_str());
}

std::string em::EmAdbDevice::GetInstanceId()
{
	return m_strInstanceId;
}

void em::EmAdbDevice::SetInstanceId( const char* szInstanceId )
{
	m_strInstanceId = EmStlStr::Lower(szInstanceId);
}

bool em::EmAdbDevice::GetInstalled()
{
	return m_bInstalled;
}

void em::EmAdbDevice::SetInstalled( bool bInstalled )
{
	m_bInstalled = bInstalled;
}

int em::EmAdbDevice::AppendHardwareId( const char* szHardwardId )
{
	bool bExisted = false;
	std::string strHadwareId = EmStlStr::Lower(szHardwardId);
	std::vector<std::string>::iterator iterList;
	for(iterList = m_xHardwareIdList.begin(); iterList != m_xHardwareIdList.end(); iterList++)
	{
		std::string csTempId = *iterList;
		if(csTempId.compare(strHadwareId) == 0)
		{
			bExisted = true;
			return 0;
		}
	}
	m_xHardwareIdList.push_back(strHadwareId);
	return 1;
}

int em::EmAdbDevice::ListHardwareId( std::vector<std::string>& xList )
{
	xList.clear();

	xList = m_xHardwareIdList;

	return xList.size();
}

std::string em::EmAdbDevice::ListHardwareIdToString()
{
	std::string strResult;
	std::vector<std::string>::iterator iterList;
	for(iterList = m_xHardwareIdList.begin(); iterList != m_xHardwareIdList.end(); iterList++)
	{
		strResult.append(*iterList);
		strResult.append(";");
	}
	return strResult;
}

#endif


