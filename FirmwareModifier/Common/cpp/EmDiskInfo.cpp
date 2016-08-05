#include "stdafx.h"
#include "../inc/EmDiskInfo.h"
#include "../inc/EmStlStr.h"
#include "../inc/EmTstr.h"
#include <afx.h>

using namespace em;
using namespace std;


EmDiskInfo::EmDiskInfo( const char* szVolume)
{
	m_iAvailable = 0;
	m_iTotal = 0;
	m_iFree = 0;
	m_strVolumeName = szVolume;
	
	ULARGE_INTEGER iAvailableSpace = {0};
	ULARGE_INTEGER iFreeSpace = {0};
	ULARGE_INTEGER iTotalSpace = {0};
	bool bResult = ::GetDiskFreeSpaceExA(szVolume
		,&iAvailableSpace,&iTotalSpace,&iFreeSpace);
	if(bResult){
		m_iAvailable = iAvailableSpace.QuadPart;
		m_iFree = iFreeSpace.QuadPart;
		m_iTotal = iTotalSpace.QuadPart;
		int iDriveType = ::GetDriveTypeA(szVolume);
		if(iDriveType == DRIVE_UNKNOWN){
			m_eVolumeType = EmDiskInfo::VtUnknown;
		}else if(iDriveType == DRIVE_NO_ROOT_DIR){
			m_eVolumeType = EmDiskInfo::VtNoRootDir;
		}else if(iDriveType == DRIVE_REMOVABLE){
			m_eVolumeType = EmDiskInfo::VtRemovable;
		}else if(iDriveType == DRIVE_FIXED){
			m_eVolumeType = EmDiskInfo::VtFixed;
		}else if(iDriveType == DRIVE_CDROM){
			m_eVolumeType = EmDiskInfo::VtCdRom;
		}else if(iDriveType == DRIVE_RAMDISK){
			m_eVolumeType = EmDiskInfo::VtRamDisk;
		}

	}
}

EmDiskInfo::~EmDiskInfo()
{
	
}

INT64 EmDiskInfo::GetTotal()
{
	return m_iTotal;
}

INT64 EmDiskInfo::GetFree()
{
	return m_iFree;
}

INT64 EmDiskInfo::GetAvailable()
{
	return m_iAvailable;
}

std::string EmDiskInfo::GetVolumeName()
{
	return m_strVolumeName;
}

EmDiskInfo::VolumeType EmDiskInfo::GetVolumeType()
{
	return m_eVolumeType;	
}


int EmDiskInfo::ListDiskVolumes(std::vector<std::string>& xVolumneList)
{
	xVolumneList.clear();
	TCHAR szDriverStrings[255] = {0};
	int iResult = ::GetLogicalDriveStrings(255,szDriverStrings);
	std::vector<EmTstr> xList;
	int iCount = EmTstr::ListInBuffer(xList,szDriverStrings,iResult);
	for(int i = 0; i < xList.size(); i++){
		EmTstr strTemp = xList.at(i);
		strTemp = EmTstr::Replace(strTemp.Sz(),_T("\\"),_T("/"));
		xVolumneList.push_back(EmTstr::ToUtf8(strTemp.Sz()).Sz());
	}
	return xVolumneList.size();
}


int EmDiskInfo::ListDiskWritableVolumes(std::vector<std::string>& xVolumneList)
{
	xVolumneList.clear();
	TCHAR szDriverStrings[255] = {0};
	int iResult = ::GetLogicalDriveStrings(255,szDriverStrings);
	std::vector<EmTstr> xList;
	int iCount = EmTstr::ListInBuffer(xList,szDriverStrings,iResult);
	for(int i = 0; i < xList.size(); i++){
		CString csTemp = xList.at(i).Sz();
		int iDriveType = ::GetDriveType(csTemp);
		csTemp = EmTstr::Replace(csTemp,_T("\\"),_T("/")).Sz();
		if( iDriveType == DRIVE_FIXED
			|| iDriveType == DRIVE_RAMDISK
			|| iDriveType == DRIVE_REMOVABLE
			|| iDriveType == DRIVE_REMOTE)
		{
			xVolumneList.push_back(EmTstr::ToUtf8(csTemp).Sz());
		}
	}
	return xVolumneList.size();
}
