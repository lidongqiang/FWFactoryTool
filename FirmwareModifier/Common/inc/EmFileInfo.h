#ifndef _h_EmFileInfo
#define _h_EmDiskINfo

#include "EmBase.h"
#include "EmTime.h"

#include <string>
#include <vector>
#include <basetsd.h>

namespace em{

class EmFileInfo
{
public:
	static EmTime GetAccessTime(const char* szFileName);
	static EmTime GetModifyTime(const char* szFileName);
	static EmTime GetCreateTime(const char* szFileName);

	static int UpdateAccessTime(const char* szFileName, EmTime& xTime);
	static int UpdateModifyTime(const char* szFileName, EmTime& xTime);
	static int UpdateCreateTime(const char* szFileName, EmTime& xTime);
};

}



#endif