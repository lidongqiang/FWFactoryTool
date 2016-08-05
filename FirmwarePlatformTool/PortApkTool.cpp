// PortApkTool.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "PortApkTool.h"
#include "PortApkToolDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPortApkToolApp

BEGIN_MESSAGE_MAP(CPortApkToolApp, CWinApp)
	//{{AFX_MSG_MAP(CPortApkToolApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPortApkToolApp construction

CPortApkToolApp::CPortApkToolApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CPortApkToolApp object

CPortApkToolApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CPortApkToolApp initialization

BOOL CPortApkToolApp::InitInstance()
{
	// ��Ӧ�ó����������ź���
	CString m_pszExeName=_T("rkFactory");
    HANDLE hSem = CreateSemaphore(NULL, 1, 1, m_pszExeName);
	
    // �ź����Ѵ��ڣ�
    // �ź������ڣ����������һ��ʵ������
    if (GetLastError() == ERROR_ALREADY_EXISTS)
    {
        // �ر��ź������
        CloseHandle(hSem);
		
        // Ѱ����ǰʵ����������
        HWND hWndPrevious = ::GetWindow(::GetDesktopWindow(), GW_CHILD);
        while (::IsWindow(hWndPrevious))
        {
            // ��鴰���Ƿ���Ԥ��ı��?
            // �У���������Ѱ�ҵ�����
            if (::GetProp(hWndPrevious, m_pszExeName))
            {
                // ����������С������ָ����С
                if (::IsIconic(hWndPrevious))
                    ::ShowWindow(hWndPrevious, SW_RESTORE);
                // ����������
                ::SetForegroundWindow(hWndPrevious);
                // �������ĶԻ��򼤻�
                ::SetForegroundWindow(::GetLastActivePopup(hWndPrevious));
                // �˳���ʵ��
                return FALSE;
            }
			
            // ����Ѱ����һ������
            hWndPrevious = ::GetWindow(hWndPrevious, GW_HWNDNEXT);
        }
		
        // ǰһʵ���Ѵ��ڣ����Ҳ���������
        // ���ܳ�����
        // �˳���ʵ��
        return FALSE;
    }
	AfxEnableControlContainer();

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.

#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif

	CPortApkToolDlg dlg;
	m_pMainWnd = &dlg;
	int nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with OK
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with Cancel
	}

	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
}
