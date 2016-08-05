#include "stdafx.h"
#ifndef EM_TURN_OFF_DEVICE_MODULE

#include "../inc/EmAdbScanner.h"
#include "../inc/EmUsbScanner.h"
#include "../inc/EmUsbDevice.h"
#include "../../Common/inc/EmStlStr.h"
#include "../../Common/inc/EmGuid.h"

using namespace em;

int em::EmAdbScanner::Scan()
{
	Clear();
	m_xUsbScanner.Scan(EmGuid::GuidZero);

	std::vector<EmUsbDevice> xAllList;
	m_xUsbScanner.ListAll(xAllList);
	std::string strAdbGuid = EmGuid::ObjToStr(EmGuid::GuidAdb);

	std::vector<EmUsbDevice>::iterator iterList;
	for(iterList = xAllList.begin(); iterList != xAllList.end(); iterList++)
	{
		EmAdbDevice xTempAdb;
		EmUsbDevice xTempUsb = *iterList;
		bool bInstalled = (xTempUsb.GetClassGuid().compare(strAdbGuid) == 0);

		if(!bInstalled){
			continue;
		}

		xTempAdb.SetInstalled(true);
		xTempAdb.SetVid(xTempUsb.FetchVid().c_str());
		xTempAdb.SetPid(xTempUsb.FetchPid().c_str());
		std::string strPlainSerial = xTempUsb.FetchPlainSerialNumber();
		if(strPlainSerial.length() == 0)
		{
			strPlainSerial = m_xUsbScanner.SearchPlainSerial(xTempUsb.FetchVid().c_str(),xTempUsb.FetchPid().c_str());
		}
		xTempAdb.SetName(xTempUsb.FetchName().c_str());
		xTempAdb.SetSerial(strPlainSerial.c_str());
		xTempAdb.SetHardwareId(xTempUsb.GetHardwareId().c_str());
		xTempAdb.SetInstanceId(xTempUsb.GetInstanceId().c_str());

		m_xAdbTable[xTempAdb.GetInstanceId()] = xTempAdb;
	}

	for(iterList = xAllList.begin(); iterList != xAllList.end(); iterList++)
	{
		EmUsbDevice xTempUsb = *iterList;
		
		bool bInstalled = (xTempUsb.GetClassGuid().compare(strAdbGuid) == 0);
		if(bInstalled)
		{
			continue;
		}

		bool bPossible = IsPossibleAdb(xTempUsb);
		if(!bPossible)
		{
			continue;
		}

		bool bInList = IsInList(xTempUsb);
		if(bInList){
			continue;
		}

		EmAdbDevice xTempAdb;
		xTempAdb.SetInstalled(false);
		xTempAdb.SetVid(xTempUsb.FetchVid().c_str());
		xTempAdb.SetPid(xTempUsb.FetchPid().c_str());
		std::string strPlainSerial = xTempUsb.FetchPlainSerialNumber();
		if(strPlainSerial.length() == 0)
		{
			strPlainSerial = m_xUsbScanner.SearchPlainSerial(xTempUsb.FetchVid().c_str(),xTempUsb.FetchPid().c_str());
		}
		xTempAdb.SetName(xTempUsb.FetchName().c_str());
		xTempAdb.SetSerial(strPlainSerial.c_str());
		xTempAdb.SetHardwareId(xTempUsb.GetHardwareId().c_str());
		xTempAdb.SetInstanceId(xTempUsb.GetInstanceId().c_str());

		m_xAdbTable[xTempAdb.GetInstanceId()] = xTempAdb;
	}

	//return m_xAdbTable.size();
	std::string strLastInstanceId = "";

	for(iterList = xAllList.begin(); iterList != xAllList.end(); iterList++)
	{
		
		EmUsbDevice xTempUsb = *iterList;
		
		bool bInstalled = (xTempUsb.GetClassGuid().compare(strAdbGuid) == 0);
		if(bInstalled)
		{
			continue;
		}

		
		bool bPossible = IsPossibleAdb(xTempUsb);
		if(!bPossible)
		{
			continue;
		}
		

		std::string strInstanceId = xTempUsb.GetInstanceId();
		if(m_xAdbTable.find(strInstanceId) != m_xAdbTable.end()){
			continue;
		}


// 		if(xTempUsb.GetServiceType().compare(_T("usbstor")) == 0)
// 		{
// 			continue;
// 		}
		
		//::AfxMessageBox(xTempUsb.FetchVid());
		strInstanceId = GetInstanceIdByVidPid(xTempUsb.FetchVid().c_str(),xTempUsb.FetchPid().c_str());
		if(strInstanceId.length() == 0){
			continue;
		}
		

		EmAdbDevice xTempAdb = m_xAdbTable[strInstanceId];
		xTempAdb.AppendHardwareId(xTempUsb.GetHardwareId().c_str());
		m_xAdbTable[xTempAdb.GetInstanceId()] = xTempAdb;

		//::AfxMessageBox(strInstanceId);
		strLastInstanceId = strInstanceId;
		//xTempAdb = m_xAdbTable[strLastInstanceId];
		//::AfxMessageBox(xTempAdb.ListHardwareIdToString());
		
	}

	//::AfxMessageBox(_T("total"));
	//EmAdbDevice xTempAdb = m_xAdbTable[strLastInstanceId];
	//::AfxMessageBox(xTempAdb.ListHardwareIdToString());
	
	return m_xAdbTable.size();
}

int em::EmAdbScanner::ScanByVid(const char *vid)
{
    Clear();
    m_xUsbScanner.Scan(EmGuid::GuidZero);
    
    std::vector<EmUsbDevice> xAllList;
    m_xUsbScanner.ListAll(xAllList);
    std::string strAdbGuid = EmGuid::ObjToStr(EmGuid::GuidAdb);
    
    std::vector<EmUsbDevice>::iterator iterList;    

    for(iterList = xAllList.begin(); iterList != xAllList.end(); iterList++)
    {
        EmAdbDevice xTempAdb;
        EmUsbDevice xTempUsb = *iterList;
        bool bInstalled = (xTempUsb.GetClassGuid().compare(strAdbGuid) == 0);
        
        if(!bInstalled){
            continue;
        }
        if(xTempUsb.FetchVid().compare(vid) == 0)
        {
            xTempAdb.SetInstalled(true);
            xTempAdb.SetVid(xTempUsb.FetchVid().c_str());
            xTempAdb.SetPid(xTempUsb.FetchPid().c_str());
            std::string strPlainSerial = xTempUsb.FetchPlainSerialNumber();
            if(strPlainSerial.length() == 0)
            {
                strPlainSerial = m_xUsbScanner.SearchPlainSerial(xTempUsb.FetchVid().c_str(),xTempUsb.FetchPid().c_str());
            }
            xTempAdb.SetName(xTempUsb.FetchName().c_str());
            xTempAdb.SetSerial(strPlainSerial.c_str());
            xTempAdb.SetHardwareId(xTempUsb.GetHardwareId().c_str());
            xTempAdb.SetInstanceId(xTempUsb.GetInstanceId().c_str());
        
            m_xAdbTable[xTempAdb.GetInstanceId()] = xTempAdb;
        }
	}

    return m_xAdbTable.size();
}

void em::EmAdbScanner::Clear()
{
	m_xAdbTable.clear();
}

int em::EmAdbScanner::ListInstalled( std::vector<EmAdbDevice>& rList, bool bInstalled )
{
	rList.clear();
	int iResult = 0;
	std::map<std::string,EmAdbDevice>::iterator iterInstalled;
	for(iterInstalled = m_xAdbTable.begin(); iterInstalled != m_xAdbTable.end(); iterInstalled++)
	{
		EmAdbDevice xTempAdb = iterInstalled->second;
		if(xTempAdb.GetInstalled() == bInstalled)
		{
			rList.push_back(xTempAdb);
		}
	}
	iResult = rList.size();
	return iResult;
}


bool em::EmAdbScanner::IsPossibleAdb( EmUsbDevice& xDevice )
{
	if( EmStlStr::Find(xDevice.FetchVid().c_str(),"_") == -1)
	{
		return false;
	}

	if(EmStlStr::Find(xDevice.GetServiceType().c_str(),"hidusb") != -1)
	{
		return false;
	}
	return true;
}

bool em::EmAdbScanner::IsInList( EmUsbDevice& xDevice )
{
	if(m_xAdbTable.find(xDevice.GetInstanceId()) != m_xAdbTable.end()){
		return true;
	}

	std::map<std::string,EmAdbDevice>::iterator iterList;
	for(iterList = m_xAdbTable.begin(); iterList != m_xAdbTable.end(); iterList++)
	{
		EmAdbDevice xTempAdb = iterList->second;

		if(xTempAdb.GetVid().compare(xDevice.FetchVid()) == 0
			&& xTempAdb.GetPid().compare(xDevice.FetchPid()) == 0)
		{
			return true;
		}
	}

	return false;
}

int em::EmAdbScanner::GetDeviceBySerial( const char* szSerial, EmAdbDevice& xDevice )
{
	std::string strSerial = EmStlStr::Lower(szSerial);
	std::map<std::string,EmAdbDevice>::iterator iterList;
	for(iterList = m_xAdbTable.begin(); iterList != m_xAdbTable.end(); iterList++)
	{
		EmAdbDevice xTempAdb = iterList->second;
		if(xTempAdb.GetSerial().compare(strSerial) == 0)
		{
			xDevice = xTempAdb;
			return 1;
		}
	}
	return 0;
}

std::string em::EmAdbScanner::GetInstanceIdByVidPid( const char* szVid, const char* szPid )
{
	std::map<std::string,EmAdbDevice>::iterator iterList;
	for(iterList = m_xAdbTable.begin(); iterList != m_xAdbTable.end(); iterList++)
	{
		EmAdbDevice xTempAdb = iterList->second;

		if(xTempAdb.GetVid().compare(szVid) == 0
			&& xTempAdb.GetPid().compare(szPid) == 0)
		{
			return xTempAdb.GetInstanceId();
		}
	}

	return "";
}

em::EmAdbScanner::EmAdbScanner()
{
}

em::EmAdbScanner::~EmAdbScanner()
{
}

#endif