#ifndef _h_EmDiskInfo
#define _h_EmDiskINfo

#include "EmBase.h"
#include <string>
#include <vector>
#include <basetsd.h>

namespace em{

class EmDiskInfo
{
public:

	enum VolumeType{
		VtUnknown
		,VtNoRootDir
		,VtRemovable
		,VtFixed
		,VtRemote
		,VtCdRom
		,VtRamDisk
	};

	EmDiskInfo(const char* szVolume);
	virtual ~EmDiskInfo();
	INT64 GetTotal();
	INT64 GetFree();
	INT64 GetAvailable();
	std::string GetVolumeName();
	VolumeType GetVolumeType();

	static int ListDiskVolumes(std::vector<std::string>& xVolumneList);
	static int ListDiskWritableVolumes(std::vector<std::string>& xVolumneList);

protected:
	
	
	INT64 m_iTotal;
	INT64 m_iFree;
	INT64 m_iAvailable;
	std::string m_strVolumeName;
	VolumeType m_eVolumeType;
};

}



#endif