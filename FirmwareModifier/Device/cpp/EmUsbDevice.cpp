#include "stdafx.h"
#ifndef EM_TURN_OFF_DEVICE_MODULE

#include "../inc/EmUsbDevice.h"
#include "../../Common/inc/EmGuid.h"
#include "../../Common/inc/EmStlStr.h"

using namespace em;


std::string em::EmUsbDevice::GetHardwareId()
{
	return m_strHardwareId;
}

std::string em::EmUsbDevice::GetInstanceId()
{
	return m_strInstanceId;
}

std::string em::EmUsbDevice::GetDesc()
{
	return m_strDesc;
}

void em::EmUsbDevice::SetHardwareId( const char* szHardwareId )
{
	m_strHardwareId = EmStlStr::Lower(szHardwareId);	
}

void em::EmUsbDevice::SetInstanceId( const char* szInstanceId )
{
	m_strInstanceId = szInstanceId;
}

void em::EmUsbDevice::SetDesc( const char* szDesc )
{	
	m_strDesc = EmStlStr::Lower(szDesc);
}

std::string em::EmUsbDevice::FetchSerialNumber()
{
	std::string strResult;

	if(m_strInstanceId.length() == 0)
	{
		return "";
	}

	int iSlashPos = EmStlStr::FindReverse(m_strInstanceId.c_str(),"\\");
	if(iSlashPos == -1)
	{
		return "";
	}

	strResult = EmStlStr::SubStr(m_strInstanceId.c_str(),iSlashPos + 1,-1);

	return strResult;
}

std::string em::EmUsbDevice::FetchPid()
{
	return FetchSubProperty("pid_");
}

std::string em::EmUsbDevice::FetchVid()
{
	return FetchSubProperty("vid_");
}

std::string em::EmUsbDevice::FetchMi()
{
	return FetchSubProperty("mi");
}

std::string em::EmUsbDevice::FetchSubProperty( const char* szSubKey )
{
	std::string strResult;
	
	if(m_strHardwareId.length() == 0)
	{	
		return "";
	}
	
	std::string strSubKey = szSubKey;
	//strSubKey += "_";
	int iPidPos = EmStlStr::Find(m_strHardwareId.c_str(),strSubKey.c_str());
	if(iPidPos == -1)
	{
		return "";
	}
	
	int iBeginPos = iPidPos + strSubKey.length();
	int iAndPos = EmStlStr::FindFrom(m_strHardwareId.c_str(),"&",iPidPos);
	if(iAndPos == -1)
	{
		strResult = EmStlStr::SubStr(m_strHardwareId.c_str(),iBeginPos,-1);
	}
	else
	{
		strResult = EmStlStr::SubStr(m_strHardwareId.c_str(),iBeginPos,iAndPos - iBeginPos);
	}

	return strResult;
}

std::string em::EmUsbDevice::FetchPlainSerialNumber()
{
	std::string strResult;
	std::string strSerial = FetchSerialNumber();
	if(strSerial.length() == 0)
	{
		return "";
	}
	
	if(EmStlStr::Find(strSerial.c_str(),"&") != -1)
	{
		return "";
	}

	int iUnderscorePos = EmStlStr::Find(strSerial.c_str(),"_");
	if(iUnderscorePos != -1)
	{
		strResult = EmStlStr::SubStr(strSerial.c_str(),0,iUnderscorePos);
	}else
	{
		strResult = strSerial;
	}

	return strResult;
}

std::string em::EmUsbDevice::GetServiceType()
{
	return m_strServiceType;
}

void em::EmUsbDevice::SetServiceType( const char* szServiceType )
{
	m_strServiceType = EmStlStr::Lower(szServiceType);
}

void em::EmUsbDevice::SetFriendlyName( const char* szFriendlyName )
{
	m_strFriendlyName = EmStlStr::Lower(szFriendlyName);
}

std::string em::EmUsbDevice::GetFriendlyName()
{
	return m_strFriendlyName;
}

void em::EmUsbDevice::SetLocationInfo( const char* szLocationInfo )
{
	m_strLocationInfo = EmStlStr::Lower(szLocationInfo);
}

std::string em::EmUsbDevice::GetLocationInfo()
{
	return m_strLocationInfo;
}

void em::EmUsbDevice::SetClassGuid( const char* szClassGuid )
{
	m_strClassGuid = EmStlStr::Lower(szClassGuid);
	m_strClassGuid = EmStlStr::Replace(m_strClassGuid.c_str(),"{","");
	m_strClassGuid = EmStlStr::Replace(m_strClassGuid.c_str(),"}","");
}

std::string em::EmUsbDevice::GetClassGuid()
{
	return m_strClassGuid;
}

void em::EmUsbDevice::SetDriverMark( const char* szDriverMark )
{
	m_strDriverMark = EmStlStr::Lower(szDriverMark);
}

std::string em::EmUsbDevice::GetDriverMark()
{
	return m_strDriverMark;
}

void em::EmUsbDevice::SetDriverManufacture( const char* szDriverManufacture )
{
	m_strDriverManufacture = EmStlStr::Lower(szDriverManufacture);
}

std::string em::EmUsbDevice::GetDriverManufacture()
{
	return m_strDriverManufacture;
}

void em::EmUsbDevice::SetPhysicalName( const char* szPhysicalName )
{
	m_strPhysicalName = EmStlStr::Lower(szPhysicalName);
}

std::string em::EmUsbDevice::GetPhysicalName()
{
	return m_strPhysicalName;
}

em::EmUsbDevice::EmUsbDevice()
{
}

em::EmUsbDevice::~EmUsbDevice()
{
	
}

std::string em::EmUsbDevice::GetDevicePath()
{
	return m_strDevicePath;
}

void em::EmUsbDevice::SetDevicePath( const char* szDevicePath )
{
	m_strDevicePath = szDevicePath;
}

std::string em::EmUsbDevice::FetchName()
{
	std::string strResult = m_strDesc;
	if(strResult.length() == 0)
	{
		return strResult;
	}

	int iSpacePos = EmStlStr::Find(strResult.c_str()," ");
	strResult = EmStlStr::SubStr(strResult.c_str(),0,iSpacePos);
	return strResult;
}

#endif