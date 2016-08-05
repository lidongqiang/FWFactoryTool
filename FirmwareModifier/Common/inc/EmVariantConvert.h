#ifndef _h_EmVariantConvert
#define _h_EmVariantConvert

#include "EmBase.h"
#include "EmTstr.h"

#include <string>

#include <afx.h>
#include <comdef.h>  




namespace em
{

class EmVariantConvert
{
private:
	EmVariantConvert(void);
	~EmVariantConvert(void);

public:  
  
	static CString ToCs(const _variant_t& xVar);  
	static EmTstr ToTstr(const _variant_t& xVar);  
	static std::string ToUtf8(const _variant_t& xVar);

};//class EmVariantConvert


}//namespace em

#endif