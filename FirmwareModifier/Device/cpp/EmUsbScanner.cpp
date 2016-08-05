#include "stdafx.h"
#ifndef EM_TURN_OFF_DEVICE_MODULE

#include "../inc/EmUsbScanner.h"
#include "../../Common/inc/EmStlStr.h"
#include "../../Common/inc/EmTstr.h"
#include "../../Common/inc/EmGuid.h"

using namespace em;
using namespace std;




int em::EmUsbScanner::Scan(const GUID& rGuid /*=NULL*/)
{
	m_xDeviceList.clear();


	int iCount = 0;
	bool bResult = 0;
	char *szEnumerator = NULL;
	DWORD dwFlag = DIGCF_PRESENT | DIGCF_DEVICEINTERFACE;
	GUID xGuid = rGuid;
	GUID *pGuid = &xGuid;

	if(EmGuid::Equal(EmGuid::GuidZero, rGuid) )
	{
		pGuid = NULL;
		szEnumerator = "USB";
		dwFlag = DIGCF_ALLCLASSES | DIGCF_PRESENT;
	}
	
	HDEVINFO hDevInfo;
	hDevInfo = ::SetupDiGetClassDevs(pGuid,EmTstr::FromUtf8(szEnumerator).Sz(),NULL,dwFlag);
	
	if(hDevInfo == INVALID_HANDLE_VALUE)
	{
		return 0;
	}
	
	SP_DEVINFO_DATA xDevInfoData;
	SP_DEVINFO_DATA xDevInfoData2;
	SP_DEVICE_INTERFACE_DATA xDevInterfaceData;
	SP_DEVICE_INTERFACE_DETAIL_DATA* pDevInterfaceDetail;
	
	pDevInterfaceDetail = (SP_DEVICE_INTERFACE_DETAIL_DATA*)malloc(1024);
	TCHAR tcsDetail[1024] = {0};
	char mbsDetail[1024] = {0};
	
	TCHAR tcsInstanceId[1024] = {0};
	TCHAR tcsDesc[1024] = {0};
	TCHAR tcsHardwareId[1024] = {0};
	TCHAR tstrServiceType[1024] = {0};
	TCHAR tcsFriendlyName[1024] = {0};
	TCHAR tcsLocationInfo[1024] = {0};
	TCHAR tcsClassGuid[1024] = {0};
	TCHAR tcsDriverMark[1024] = {0};
	TCHAR tcsManufacture[1024] = {0};
	TCHAR tcsPhysicalName[1024] = {0};
	TCHAR tcsDevicePath[1024] = {0};
	
	CONFIGRET xConfigRet = CR_FAILURE;
	DWORD dwRegDataType = 0;
	DWORD dwStrSize = 0;
	
	while(true)
	{
		memset(&xDevInfoData,0,sizeof(xDevInfoData));
		xDevInfoData.cbSize = sizeof(xDevInfoData);

		memset(&xDevInfoData2,0,sizeof(xDevInfoData2));
		xDevInfoData2.cbSize = sizeof(xDevInfoData2);

		memset(&xDevInterfaceData,0,sizeof(xDevInterfaceData));
		xDevInterfaceData.cbSize = sizeof(xDevInterfaceData);
		
		memset(pDevInterfaceDetail,0,1024);
		pDevInterfaceDetail->cbSize = sizeof(SP_DEVICE_INTERFACE_DETAIL_DATA);

		memset(tcsInstanceId,0,sizeof(TCHAR) * 1024);
		memset(tcsDesc,0,sizeof(TCHAR) * 1024);
		memset(tcsHardwareId,0,sizeof(TCHAR) * 1024);
		memset(tstrServiceType,0,sizeof(TCHAR) * 1024);
		memset(tcsFriendlyName,0,sizeof(TCHAR) * 1024);
		memset(tcsLocationInfo,0,sizeof(TCHAR) * 1024);
		memset(tcsClassGuid,0,sizeof(TCHAR) * 1024);
		memset(tcsDriverMark,0,sizeof(TCHAR) * 1024);
		memset(tcsManufacture,0,sizeof(TCHAR) * 1024);
		memset(tcsPhysicalName,0,sizeof(TCHAR) * 1024);
		memset(tcsDevicePath,0,sizeof(TCHAR) * 1024);

		if(pGuid != NULL)
		{
			bResult = ::SetupDiEnumDeviceInterfaces(hDevInfo,0,pGuid,iCount,&xDevInterfaceData);
			if(!bResult)
			{
				break;
			}
			::SetupDiGetDeviceInterfaceDetail(hDevInfo,&xDevInterfaceData,pDevInterfaceDetail,1024,NULL,&xDevInfoData2);
			_tcscpy(tcsDevicePath,pDevInterfaceDetail->DevicePath);
		}

		
		bResult = ::SetupDiEnumDeviceInfo(hDevInfo
			,(ULONG)iCount
			,&xDevInfoData);
		if(!bResult)
		{
			break;
		}

		xConfigRet = ::CM_Get_Device_ID(xDevInfoData.DevInst,tcsInstanceId,1024,0);
		
		bResult = ::SetupDiGetDeviceRegistryProperty(hDevInfo
			,&xDevInfoData
			,SPDRP_DEVICEDESC
			,&dwRegDataType
			,(BYTE*)tcsDesc
			,1024
			,&dwStrSize);
		
		
		bResult = ::SetupDiGetDeviceRegistryProperty(hDevInfo
			,&xDevInfoData
			,SPDRP_HARDWAREID
			,&dwRegDataType
			,(BYTE*)tcsHardwareId
			,1024
			,&dwStrSize);

		bResult = ::SetupDiGetDeviceRegistryProperty(hDevInfo
			,&xDevInfoData
			,SPDRP_SERVICE
			,&dwRegDataType
			,(BYTE*)tstrServiceType
			,1024
			,&dwStrSize);

		bResult = ::SetupDiGetDeviceRegistryProperty(hDevInfo
			,&xDevInfoData
			,SPDRP_FRIENDLYNAME
			,&dwRegDataType
			,(BYTE*)tcsFriendlyName
			,1024
			,&dwStrSize);

		
		bResult = ::SetupDiGetDeviceRegistryProperty(hDevInfo
			,&xDevInfoData
			,SPDRP_LOCATION_INFORMATION
			,&dwRegDataType
			,(BYTE*)tcsLocationInfo
			,1024
			,&dwStrSize);

		bResult = ::SetupDiGetDeviceRegistryProperty(hDevInfo
			,&xDevInfoData
			,SPDRP_CLASSGUID
			,&dwRegDataType
			,(BYTE*)tcsClassGuid
			,1024
			,&dwStrSize);

		bResult = ::SetupDiGetDeviceRegistryProperty(hDevInfo
			,&xDevInfoData
			,SPDRP_DRIVER
			,&dwRegDataType
			,(BYTE*)tcsDriverMark
			,1024
			,&dwStrSize);

		bResult = ::SetupDiGetDeviceRegistryProperty(hDevInfo
			,&xDevInfoData
			,SPDRP_MFG
			,&dwRegDataType
			,(BYTE*)tcsManufacture
			,1024
			,&dwStrSize);

		bResult = ::SetupDiGetDeviceRegistryProperty(hDevInfo
			,&xDevInfoData
			,SPDRP_PHYSICAL_DEVICE_OBJECT_NAME
			,&dwRegDataType
			,(BYTE*)tcsPhysicalName
			,1024
			,&dwStrSize);

		//SPDRP_NTDEVICEPATHS
		bResult = ::SetupDiGetDeviceRegistryProperty(hDevInfo
			,&xDevInfoData
			,0x00000003
			,&dwRegDataType
			,(BYTE*)tcsDevicePath
			,1024
			,&dwStrSize);

		EmUsbDevice xDevice;
		xDevice.SetDesc( EmStlStr::UnicodeToUtf8( tcsDesc).c_str() );
		xDevice.SetHardwareId( EmStlStr::UnicodeToUtf8( tcsHardwareId).c_str() );
		xDevice.SetInstanceId( EmStlStr::UnicodeToUtf8( tcsInstanceId).c_str() );
		xDevice.SetServiceType( EmStlStr::UnicodeToUtf8( tstrServiceType).c_str() );
		xDevice.SetFriendlyName( EmStlStr::UnicodeToUtf8( tcsFriendlyName).c_str() );
		xDevice.SetLocationInfo( EmStlStr::UnicodeToUtf8( tcsLocationInfo).c_str() );
		xDevice.SetClassGuid( EmStlStr::UnicodeToUtf8( tcsClassGuid).c_str() );
		xDevice.SetDriverMark( EmStlStr::UnicodeToUtf8( tcsDriverMark).c_str() );
		xDevice.SetDriverManufacture( EmStlStr::UnicodeToUtf8( tcsManufacture).c_str() );
		xDevice.SetPhysicalName( EmStlStr::UnicodeToUtf8( tcsPhysicalName).c_str() );
		//xDevice.SetDevicePath(tcsDevicePath);

		m_xDeviceList.push_back(xDevice);
		
		iCount++;
	}
	
	
	EM_FREE_NULL(pDevInterfaceDetail);
	::SetupDiDestroyDeviceInfoList(hDevInfo);

	return m_xDeviceList.size();
}

int em::EmUsbScanner::ListByGuid( std::vector<EmUsbDevice>& rDeviceList, const GUID& rGuid)
{
	if(EmGuid::Equal(rGuid,EmGuid::GuidZero))
	{
		rDeviceList = m_xDeviceList;
		return rDeviceList.size();
	}

	std::string strGuidStr = EmGuid::ObjToStr(rGuid);

	vector<EmUsbDevice>::iterator iterDevice;
	for(iterDevice = m_xDeviceList.begin(); iterDevice != m_xDeviceList.end(); iterDevice++)
	{
		
		EmUsbDevice xTempDevice = *iterDevice;
		if(xTempDevice.GetClassGuid().compare(strGuidStr) != 0)
		{
			continue;
		}
		
		
		rDeviceList.push_back(xTempDevice);
	}
	
	return rDeviceList.size();
}

int em::EmUsbScanner::AppendDevice( EmUsbDevice& rDevice )
{
	m_xDeviceList.push_back(rDevice);

	return 1;
}

bool em::EmUsbScanner::ExistedDevice( EmUsbDevice& rDevice )
{
	bool bResult = false;
	vector<EmUsbDevice>::iterator iterDevice;
	for(iterDevice = m_xDeviceList.begin(); iterDevice != m_xDeviceList.end(); iterDevice++)
	{
		EmUsbDevice xTempDevice = *iterDevice;
		if(xTempDevice.GetInstanceId().compare(rDevice.GetInstanceId()) == 0)
		{
			bResult = true;
			break;
		}
	}
	return bResult;
}

void em::EmUsbScanner::Clear()
{
	m_xDeviceList.clear();
	
}

int em::EmUsbScanner::FilterByClassGuid( const GUID& rGuid )
{
	std::string strGuidStr = EmGuid::ObjToStr(rGuid);

	vector<EmUsbDevice> xDeviceListCopy;
	int iSrcSize = m_xDeviceList.size();
	
	vector<EmUsbDevice>::iterator iterDevice;
	for(iterDevice = m_xDeviceList.begin(); iterDevice != m_xDeviceList.end(); iterDevice++)
	{
		EmUsbDevice xTempDevice = *iterDevice;
		
		if( xTempDevice.GetClassGuid().compare(strGuidStr) == 0 )
		{
			continue;
		}
		
		xDeviceListCopy.push_back(xTempDevice);
	}
	
	m_xDeviceList = xDeviceListCopy;
	int iDestSize = m_xDeviceList.size();
	
	return  iSrcSize - iDestSize;
}

int em::EmUsbScanner::FilterByVid( const char* szVid )
{
	std::string strVid = EmStlStr::Lower(szVid);
	std::string strVidPrefix = "_" + strVid;

	vector<EmUsbDevice> xDeviceListCopy;
	int iSrcSize = m_xDeviceList.size();
	
	vector<EmUsbDevice>::iterator iterDevice;
	for(iterDevice = m_xDeviceList.begin(); iterDevice != m_xDeviceList.end(); iterDevice++)
	{
		EmUsbDevice xTempDevice = *iterDevice;

		if(xTempDevice.FetchVid().compare(strVid) == 0
			|| xTempDevice.FetchVid().compare(strVidPrefix) == 0)
		{
			continue;
		}
		
		xDeviceListCopy.push_back(xTempDevice);
	}
	
	m_xDeviceList = xDeviceListCopy;
	int iDestSize = m_xDeviceList.size();
	
	return  iSrcSize - iDestSize;
}

int em::EmUsbScanner::Count()
{
	return m_xDeviceList.size();
}


int em::EmUsbScanner::FilterByServiceType( const char* szServiceType )
{
	std::string strServiceType = EmStlStr::Lower(szServiceType);
	vector<EmUsbDevice> xDeviceListCopy;
	int iSrcSize = m_xDeviceList.size();
	
	vector<EmUsbDevice>::iterator iterDevice;
	for(iterDevice = m_xDeviceList.begin(); iterDevice != m_xDeviceList.end(); iterDevice++)
	{
		EmUsbDevice xTempDevice = *iterDevice;
		
		if(xTempDevice.GetServiceType().compare(strServiceType) == 0)
		{
			continue;
		}
		
		xDeviceListCopy.push_back(xTempDevice);
	}
	
	m_xDeviceList = xDeviceListCopy;
	int iDestSize = m_xDeviceList.size();
	
	return  iSrcSize - iDestSize;
}

int em::EmUsbScanner::ListAll( std::vector<EmUsbDevice>& rDeviceList )
{
	rDeviceList = m_xDeviceList;
	return rDeviceList.size();
}

std::string em::EmUsbScanner::SearchPlainSerial( const char* szVid, const char* szPid )
{
	std::string strResult;

	vector<EmUsbDevice>::iterator iterDevice;
	for(iterDevice = m_xDeviceList.begin(); iterDevice != m_xDeviceList.end(); iterDevice++)
	{
		EmUsbDevice xTempDevice = *iterDevice;
		
		if(xTempDevice.FetchVid().compare(szVid) == 0 
			&& xTempDevice.FetchPid().compare(szPid) == 0
			&& xTempDevice.FetchPlainSerialNumber().length() > 0)
		{
			strResult = xTempDevice.FetchPlainSerialNumber();
			break;
		}
	}

	return strResult;
}

#endif