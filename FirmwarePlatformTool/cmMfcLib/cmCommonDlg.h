#ifndef CM_COMMONDLG_H
#define CM_COMMONDLG_H

namespace cm
{
	class cmCommonDlg
	{
	public:
		static BOOL OpenDialog(CString &strSelectFile,CString strFilter,CString strInitialDir=_T(""),LPCTSTR strTitle=NULL);
		static BOOL SaveDialog(CString &strSelectFile,CString strFilter,CString strInitialDir=_T(""),LPCTSTR strTitle=NULL);
		static BOOL BrowseDialog(CString &strSelectFile,CString strInitialDir=_T(""),LPCTSTR strTitle=NULL,BOOL bIncludeFile=FALSE);
	};
}


#endif