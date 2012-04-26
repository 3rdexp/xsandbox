#define WIN32_LEAN_AND_MEAN	
#define _CRT_SECURE_NO_DEPRECATE

#include <windows.h>
#include <objbase.h>

#include "..\DirectUICore\UIlib.h"

using namespace DirectUICore;

#ifdef _DEBUG
#   ifdef _UNICODE
#       pragma comment(lib, "..\\bin\\Debug\\DirectUICore_u.lib")
#   else
#       pragma comment(lib, "..\\bin\\Debug\\DirectUICore.lib")
#   endif
#else
#   ifdef _UNICODE
#       pragma comment(lib, "..\\bin\\Release\\DirectUICore_u.lib")
#   else
#       pragma comment(lib, "..\\bin\\Release\\DirectUICore.lib")
#   endif
#endif

class CFrameWindowWnd : public CWindowWnd, public INotifyUI
{
public:
    CFrameWindowWnd() { };
    LPCTSTR GetWindowClassName() const { return _T("UIMainFrame"); };

    void Notify(TNotifyUI& msg)
    {
		if (msg.sType == _T("click")) 
		{
        }
    }

    LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
    {
        if (uMsg == WM_CREATE) 
		{
            m_pm.Init(m_hWnd);
            CDialogBuilder builder;
            CControlUI* pRoot = builder.Create(_T("control-factory1.xml"), (UINT)0, NULL, &m_pm);
            ASSERT(pRoot && "Failed to parse XML");
            m_pm.AttachDialog(pRoot);
            m_pm.AddNotifier(this);
            return 0;
        }
        else if (uMsg == WM_DESTROY) 
		{
            ::PostQuitMessage(0L);
        }
        LRESULT lRes = 0;
        if (m_pm.MessageHandler(uMsg, wParam, lParam, lRes)) return lRes;
        return CWindowWnd::HandleMessage(uMsg, wParam, lParam);
    }

public:
    CPaintManagerUI m_pm;
};

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE /*hPrevInstance*/, LPSTR /*lpCmdLine*/, int nCmdShow)
{
    CPaintManagerUI::SetInstance(hInstance);
    CPaintManagerUI::SetResourcePath(CPaintManagerUI::GetInstancePath());

    HRESULT Hr = ::CoInitialize(NULL);
    if (FAILED(Hr)) return 0;

    CFrameWindowWnd* pFrame = new CFrameWindowWnd();
    if (pFrame == NULL) return 0;
    pFrame->Create(NULL, _T("ControlFactory"), UI_WNDSTYLE_FRAME, WS_EX_WINDOWEDGE);
    pFrame->CenterWindow();
    pFrame->ShowWindow(true);
    CPaintManagerUI::MessageLoop();

    ::CoUninitialize();
    return 0;
}