#include "stdafx.h"
#include "../inc/EmVariantConvert.h"
#include "../inc/EmCs.h"
#include "../inc/EmStlStr.h"
       
#include <afxdtctl.h>

CString em::EmVariantConvert::ToCs(const _variant_t& xVar)  
{  
	CString strValue;  
	switch (xVar.vt)  
	{  
	case VT_BSTR: //strings
	case VT_LPSTR:  
	case VT_LPWSTR:  
		strValue = (LPCTSTR)(_bstr_t)xVar;  
		break;  

	case VT_I1:   //unsigned char
	case VT_UI1:  
		strValue.Format(_T("%d"), xVar.bVal);  
		break;  

	case VT_I2:   //short
		strValue.Format(_T("%d"), xVar.iVal);  
		break;  

	case VT_UI2:   //unsigned short 
		strValue.Format(_T("%u"), xVar.uiVal);  
		break; 

	case VT_INT: 
		strValue.Format(_T("%d"), xVar.intVal);  
		break;  

	case VT_I4:   //int 32 
	case VT_I8:   //int 64
		strValue.Format(_T("%d"), xVar.lVal);  
		break;  

	case VT_UINT:   //unsigned int
		strValue.Format(_T("%d"), xVar.uintVal);  
		break;  

	case VT_UI4:    //UINT32
	case VT_UI8:    //UINT64
		strValue.Format(_T("%d"), xVar.ulVal);  
		break;  
	case VT_VOID:  
		strValue.Format(_T("%08x"), xVar.byref);  
		break;  

	case VT_R4:   //float32 
		strValue.Format(_T("%f"), xVar.fltVal);  
		break;  

	case VT_R8:   //float64
		strValue.Format(_T("%f"), xVar.dblVal);  
		break;  

	case VT_DECIMAL:
		strValue.Format(_T("%f"), (double)xVar);  
		break;  

	//case VT_CY:  
	//	{  
	//		COleCurrency cy = xVar.cyVal;  
	//		strValue = cy.Format();
	//		break; 
	//	}  

	case VT_BLOB:  
	case VT_BLOB_OBJECT:  
	case 0x2011:  
		strValue = _T("[BLOB]");  
		break;  

	case VT_BOOL:   
		strValue = xVar.boolVal ? _T("TRUE") : _T("FALSE");  
		break; 

	case VT_DATE:
		{  
			DATE dt = xVar.date;  
			COleDateTime da = COleDateTime(dt);  
			strValue = da.Format(_T("%Y-%m-%d %H:%M:%S")); 
			break;
		}  

	case VT_NULL:
	case VT_EMPTY:
		strValue = _T("");  
		break;

	case VT_UNKNOWN:
		strValue = _T("VT_UNKNOW");  
		break; 

	default:  
		strValue = _T("VT_UNKNOW");  
		break;  
	}  

	return strValue;  
}

std::string em::EmVariantConvert::ToUtf8( const _variant_t& xVar )
{
	std::string strValue;  
	switch (xVar.vt)  
	{  
	case VT_BSTR: //strings
	case VT_LPSTR:  
	case VT_LPWSTR:  
		//::AfxMessageBox(_T("00"));
		//::AfxMessageBox((LPCTSTR)(_bstr_t)xVar);
		strValue = (LPCSTR)(_bstr_t)xVar;  
		break;  

	case VT_I1:   //unsigned char
	case VT_UI1:  
		strValue = EmStlStr::Int2Str(xVar.bVal);  
		break;  

	case VT_I2:   //short
		strValue = EmStlStr::Int2Str(xVar.iVal);   
		break;  

	case VT_UI2:   //unsigned short 
		strValue = EmStlStr::Int2Str(xVar.uiVal); 
		break; 

	case VT_INT: 
		strValue = EmStlStr::Int2Str(xVar.intVal);  
		break;  

	case VT_I4:   //int 32 
		strValue = EmStlStr::Int2Str(xVar.lVal);  
		break;
	case VT_I8:   //int 64
		strValue = EmStlStr::Long2Str(xVar.lVal);  
		break;  

	case VT_UINT:   //unsigned int
		strValue = EmStlStr::Int2Str(xVar.uiVal); 
		break;  

	case VT_UI4:    //UINT32
	case VT_UI8:    //UINT64
		strValue = EmStlStr::Long2Str(xVar.ulVal);  
		break;  
	case VT_VOID:  
		strValue = EmStlStr::Long2Str((INT64)xVar.byref);
		break;  

	case VT_R4:   //float32 
		strValue = EmStlStr::Float2Str(xVar.fltVal);
		break;  

	case VT_R8:   //float64
		strValue = EmStlStr::Float2Str(xVar.dblVal);
		break;  

	case VT_DECIMAL:

		break;  

		//case VT_CY:  
		//	{  
		//		COleCurrency cy = xVar.cyVal;  
		//		strValue = cy.Format();
		//		break; 
		//	}  

	case VT_BLOB:  
	case VT_BLOB_OBJECT:  
	case 0x2011:  

		break;  

	case VT_BOOL:   
		strValue = xVar.boolVal ? "TRUE" : "FALSE";  
		break; 

	case VT_DATE:
		{  
			DATE dt = xVar.date;  
			COleDateTime da = COleDateTime(dt);  
			strValue = EmStlStr::UnicodeToUtf8(da.Format(_T("%Y-%m-%d %H:%M:%S"))); 
			break;
		}  

	case VT_NULL:
	case VT_EMPTY:
		strValue = "";  
		break;

	case VT_UNKNOWN:
		strValue = "VT_UNKNOW";  
		break; 

	default:  
		strValue = "VT_UNKNOW";  
		break;  
	}  

	return strValue;  
}

em::EmTstr em::EmVariantConvert::ToTstr( const _variant_t& xVar )
{
	EmTstr strValue;  
	switch (xVar.vt)  
	{  
	case VT_BSTR: //strings
	case VT_LPSTR:  
	case VT_LPWSTR:  
		strValue = (LPCTSTR)(_bstr_t)xVar;  
		break;  

	case VT_I1:   //unsigned char
	case VT_UI1:  
		strValue = EmTstr::Int2Str(xVar.bVal);  
		break;  

	case VT_I2:   //short
		strValue = EmTstr::Int2Str(xVar.iVal);  
		break;  

	case VT_UI2:   //unsigned short 
		strValue = EmTstr::Int2Str(xVar.uiVal);  
		break; 

	case VT_INT: 
		strValue = EmTstr::Int2Str(xVar.intVal);  
		break;  

	case VT_I4:   //int 32 
	case VT_I8:   //int 64
		strValue = EmTstr::Long2Str(xVar.lVal);   
		break;  

	case VT_UINT:   //unsigned int
		strValue = EmTstr::Int2Str(xVar.uintVal);  
		break;  

	case VT_UI4:    //UINT32
	case VT_UI8:    //UINT64
		strValue = EmTstr::Long2Str(xVar.ulVal);    
		break;  
	case VT_VOID:  
		strValue = EmTstr::Long2Str((INT64)xVar.byref);    
		break;  

	case VT_R4:   //float32 
		strValue = EmTstr::Float2Str(xVar.fltVal);     
		break;  

	case VT_R8:   //float64
		strValue = EmTstr::Float2Str(xVar.dblVal); 
		break;  

	case VT_DECIMAL:
		strValue = EmTstr::Float2Str(xVar.dblVal);  
		break;  

		//case VT_CY:  
		//	{  
		//		COleCurrency cy = xVar.cyVal;  
		//		strValue = cy.Format();
		//		break; 
		//	}  

	case VT_BLOB:  
	case VT_BLOB_OBJECT:  
	case 0x2011:  
		strValue = _T("[BLOB]");  
		break;  

	case VT_BOOL:   
		strValue = xVar.boolVal ? _T("TRUE") : _T("FALSE");  
		break; 

	case VT_DATE:
		{  
			DATE dt = xVar.date;  
			COleDateTime da = COleDateTime(dt);  
			strValue = da.Format(_T("%Y-%m-%d %H:%M:%S")); 
			break;
		}  

	case VT_NULL:
	case VT_EMPTY:
		strValue = _T("");  
		break;

	case VT_UNKNOWN:
		strValue = _T("VT_UNKNOW");  
		break; 

	default:  
		strValue = _T("VT_UNKNOW");  
		break;  
	}  

	return strValue;  
}


