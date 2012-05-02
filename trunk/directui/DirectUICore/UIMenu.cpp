#include "StdAfx.h"

namespace DirectUICore 
{

CMenuStripUI::CMenuStripUI() {}

LPCTSTR CMenuStripUI::GetClass() const
{
	return _T("CMenuStripUI");
}

LPVOID CMenuStripUI::GetInterface(LPCTSTR pstrName)
{
    if (_tcsicmp(pstrName, _T("MenuStrip")) == 0) return static_cast<CMenuStripUI*>(this);    
    return CHorizontalLayoutUI::GetInterface(pstrName);
}

/////////////////////////////////////////////////////////////////////////////////////

CMenuElementUI::CMenuElementUI() : m_pWindow(NULL)
{
	m_cxyFixed.cy = 25;
	m_bMouseChildEnabled = true;
	SetMouseChildEnabled(false);
}

CMenuElementUI::~CMenuElementUI() {}

LPCTSTR CMenuElementUI::GetClass() const
{
	return _T("CMenuElementUI");
}

LPVOID CMenuElementUI::GetInterface(LPCTSTR pstrName)
{
    if( _tcsicmp(pstrName, _T("MenuElement")) == 0 ) return static_cast<CMenuElementUI*>(this);    
    return CListContainerElementUI::GetInterface(pstrName);
}

void CMenuElementUI::DoPaint(HDC hDC, const RECT& rcPaint)
{
	int i;

    if (!::IntersectRect(&m_rcPaint, &rcPaint, &m_rcItem)) return;
	CMenuElementUI::DrawItemBk(hDC, m_rcItem);
	DrawItemText(hDC, m_rcItem);
	for (i = 0; i < GetCount(); ++i)
	{
		if (GetItemAt(i)->GetInterface(_T("MenuElement")) == NULL)
			GetItemAt(i)->DoPaint(hDC, rcPaint);
	}
}

void CMenuElementUI::DrawItemText(HDC hDC, const RECT& rcItem)
{
    if( m_sText.IsEmpty() ) return;

    if( m_pOwner == NULL ) return;
    TListInfoUI* pInfo = m_pOwner->GetListInfo();
    DWORD iTextColor = pInfo->dwTextColor;
    if( (m_uButtonState & UISTATE_HOT) != 0 ) {
        iTextColor = pInfo->dwHotTextColor;
    }
    if( IsSelected() ) {
        iTextColor = pInfo->dwSelectedTextColor;
    }
    if( !IsEnabled() ) {
        iTextColor = pInfo->dwDisabledTextColor;
    }
    int nLinks = 0;
    RECT rcText = rcItem;
    rcText.left += pInfo->rcTextPadding.left;
    rcText.right -= pInfo->rcTextPadding.right;
    rcText.top += pInfo->rcTextPadding.top;
    rcText.bottom -= pInfo->rcTextPadding.bottom;

    if( pInfo->bShowHtml )
        CRenderEngine::DrawHtmlText(hDC, m_pManager, rcText, m_sText, iTextColor, \
        NULL, NULL, nLinks, DT_SINGLELINE | pInfo->uTextStyle);
    else
        CRenderEngine::DrawText(hDC, m_pManager, rcText, m_sText, iTextColor, \
        pInfo->nFont, DT_SINGLELINE | pInfo->uTextStyle);
}


SIZE CMenuElementUI::EstimateSize(SIZE szAvailable)
{
	SIZE cXY = {0};
	for( int it = 0; it < GetCount(); it++ ) {
		CControlUI* pControl = static_cast<CControlUI*>(GetItemAt(it));
		if( !pControl->IsVisible() ) continue;
		SIZE sz = pControl->EstimateSize(szAvailable);
		cXY.cy += sz.cy;
		if( cXY.cx < sz.cx )
			cXY.cx = sz.cx;
	}
	if(cXY.cy == 0) {
		TListInfoUI* pInfo = m_pOwner->GetListInfo();

		DWORD iTextColor = pInfo->dwTextColor;
		if( (m_uButtonState & UISTATE_HOT) != 0 ) {
			iTextColor = pInfo->dwHotTextColor;
		}
		if( IsSelected() ) {
			iTextColor = pInfo->dwSelectedTextColor;
		}
		if( !IsEnabled() ) {
			iTextColor = pInfo->dwDisabledTextColor;
		}

		RECT rcText = { 0, 0, MAX(szAvailable.cx, m_cxyFixed.cx), 9999 };
		rcText.left += pInfo->rcTextPadding.left;
		rcText.right -= pInfo->rcTextPadding.right;
		if( pInfo->bShowHtml ) {   
			int nLinks = 0;
			CRenderEngine::DrawHtmlText(m_pManager->GetPaintDC(), m_pManager, rcText, m_sText, iTextColor, NULL, NULL, nLinks, DT_CALCRECT | pInfo->uTextStyle);
		}
		else {
			CRenderEngine::DrawText(m_pManager->GetPaintDC(), m_pManager, rcText, m_sText, iTextColor, pInfo->nFont, DT_CALCRECT | pInfo->uTextStyle);
		}
		cXY.cx = rcText.right - rcText.left + pInfo->rcTextPadding.left + pInfo->rcTextPadding.right + 20;
		cXY.cy = rcText.bottom - rcText.top + pInfo->rcTextPadding.top + pInfo->rcTextPadding.bottom;
	}

	if( m_cxyFixed.cy != 0 ) cXY.cy = m_cxyFixed.cy;
	return cXY;
}

void CMenuElementUI::DoEvent(TEventUI& event)
{
	if( event.Type == UIEVENT_MOUSEENTER )
	{
		CListContainerElementUI::DoEvent(event);
		if( m_pWindow ) return;
		bool hasSubMenu = false;
		for( int i = 0; i < GetCount(); ++i )
		{
			if( GetItemAt(i)->GetInterface(_T("MenuElement")) != NULL )
			{
				(static_cast<CMenuElementUI*>(GetItemAt(i)->GetInterface(_T("MenuElement"))))->SetVisible(true);
				(static_cast<CMenuElementUI*>(GetItemAt(i)->GetInterface(_T("MenuElement"))))->SetInternVisible(true);

				hasSubMenu = true;
			}
		}
		if( hasSubMenu )
		{
			m_pOwner->SelectItem(GetIndex(), true);
			CreateMenuWnd();
		}
		else
		{
			ContextMenuParam param;
			param.hWnd = m_pManager->GetPaintWindow();
			param.wParam = 2;
			//s_context_menu_observer.RBroadcast(param);
			m_pOwner->SelectItem(GetIndex(), true);
		}
		return;
	}

	if( event.Type == UIEVENT_BUTTONDOWN )
	{
		if( IsEnabled() ){
			CListContainerElementUI::DoEvent(event);

			if( m_pWindow ) return;

			bool hasSubMenu = false;
			for( int i = 0; i < GetCount(); ++i ) {
				if( GetItemAt(i)->GetInterface(_T("MenuElement")) != NULL ) {
					(static_cast<CMenuElementUI*>(GetItemAt(i)->GetInterface(_T("MenuElement"))))->SetVisible(true);
					(static_cast<CMenuElementUI*>(GetItemAt(i)->GetInterface(_T("MenuElement"))))->SetInternVisible(true);

					hasSubMenu = true;
				}
			}
			if( hasSubMenu )
			{
				CreateMenuWnd();
			}
			else
			{
				ContextMenuParam param;
				param.hWnd = m_pManager->GetPaintWindow();
				param.wParam = 1;
				//s_context_menu_observer.RBroadcast(param);
			}
        }
        return;
    }

    CListContainerElementUI::DoEvent(event);
}

bool CMenuElementUI::Activate()
{
	if (CListContainerElementUI::Activate() && m_bSelected)
	{
		if( m_pWindow ) return true;
		bool hasSubMenu = false;
		for (int i = 0; i < GetCount(); ++i)
		{
			if (GetItemAt(i)->GetInterface(_T("MenuElement")) != NULL)
			{
				(static_cast<CMenuElementUI*>(GetItemAt(i)->GetInterface(_T("MenuElement"))))->SetVisible(true);
				(static_cast<CMenuElementUI*>(GetItemAt(i)->GetInterface(_T("MenuElement"))))->SetInternVisible(true);

				hasSubMenu = true;
			}
		}
		if (hasSubMenu)
		{
			CreateMenuWnd();
		}
		else
		{
			ContextMenuParam param;
			param.hWnd = m_pManager->GetPaintWindow();
			param.wParam = 1;
			//s_context_menu_observer.RBroadcast(param);
		}

		return true;
	}
	return false;
}

CMenuWnd* CMenuElementUI::GetMenuWnd()
{
	return m_pWindow;
}

void CMenuElementUI::CreateMenuWnd()
{
	if (m_pWindow) return;

	m_pWindow = new CMenuWnd();
	ASSERT(m_pWindow);

	ContextMenuParam param;
	param.hWnd = m_pManager->GetPaintWindow();
	param.wParam = 2;
	//s_context_menu_observer.RBroadcast(param);

	m_pWindow->Init(static_cast<CMenuElementUI*>(this), CPoint());
}

/////////////////////////////////////////////////////////////////////////////////////
//
void CMenuWnd::Init(CMenuElementUI* pOwner, POINT point)
{
	m_BasedPoint = point;
    m_pOwner = pOwner;
    m_pLayout = NULL;

	//s_context_menu_observer.AddReceiver(this);

	Create(m_pOwner->GetManager()->GetPaintWindow(), NULL, WS_POPUP, WS_EX_TOOLWINDOW | WS_EX_TOPMOST, CRect());
    // HACK: Don't deselect the parent's caption
    HWND hWndParent = m_hWnd;
    while( ::GetParent(hWndParent) != NULL ) hWndParent = ::GetParent(hWndParent);
    ::ShowWindow(m_hWnd, SW_SHOW);
#if defined(WIN32) && !defined(UNDER_CE)
    ::SendMessage(hWndParent, WM_NCACTIVATE, TRUE, 0L);
#endif	
}


LPCTSTR CMenuWnd::GetWindowClassName() const
{
    return _T("CMenuWnd");
}

void CMenuWnd::OnFinalMessage(HWND hWnd)
{
    m_pOwner->m_pWindow = NULL;
    m_pOwner->m_uButtonState &= ~ UISTATE_PUSHED;
    m_pOwner->Invalidate();
    delete this;
}

LRESULT CMenuWnd::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	int i;

    if (uMsg == WM_CREATE) 
	{
        m_pm.Init(m_hWnd);
        // The trick is to add the items to the new container. Their owner gets
        // reassigned by this operation - which is why it is important to reassign
        // the items back to the righfull owner/manager when the window closes.
        m_pLayout = new CVerticalLayoutUI;
        m_pm.UseParentResource(m_pOwner->GetManager());
        m_pLayout->SetManager(&m_pm, NULL, true);
        LPCTSTR pDefaultAttributes = m_pOwner->GetManager()->GetDefaultAttributeList(_T("VerticalLayout"));
        if (pDefaultAttributes) 
		{
            m_pLayout->ApplyAttributeList(pDefaultAttributes);
        }
        m_pLayout->SetInset(CRect(1, 1, 1, 1));
        m_pLayout->SetBkColor(0xFFFFFFFF);
        m_pLayout->SetBorderColor(0xFFC6C7D2);
        m_pLayout->SetBorderSize(1);
        m_pLayout->SetAutoDestroy(false);
        m_pLayout->EnableScrollBar();
        for (i = 0; i < m_pOwner->GetCount(); i++) 
		{
            m_pLayout->Add(static_cast<CControlUI*>(m_pOwner->GetItemAt(i)));
        }
        m_pm.AttachDialog(m_pLayout);
        
        return 0;
    }
    else if (uMsg == WM_CLOSE) 
	{
        m_pOwner->SetManager(m_pOwner->GetManager(), m_pOwner->GetParent(), false);
        m_pOwner->SetPos(m_pOwner->GetPos());
        m_pOwner->SetFocus();
    }
    else if (uMsg == WM_LBUTTONUP) 
	{
        POINT pt = { 0 };
        ::GetCursorPos(&pt);
        ::ScreenToClient(m_pm.GetPaintWindow(), &pt);
        CControlUI* pControl = m_pm.FindControl(pt);
        if (pControl && _tcscmp(pControl->GetClass(), _T("ScrollBarUI")) != 0) PostMessage(WM_KILLFOCUS);
    }
    else if (uMsg == WM_KEYDOWN) 
	{
        switch( wParam ) {
        case VK_ESCAPE:
            EnsureVisible(m_iOldSel);
            // FALL THROUGH...
        case VK_RETURN:
            PostMessage(WM_KILLFOCUS);
            break;
        default:
            TEventUI event;
            event.Type = UIEVENT_KEYDOWN;
            event.chKey = (TCHAR)wParam;
            m_pOwner->DoEvent(event);
            return 0;
        }
    }
    else if (uMsg == WM_MOUSEWHEEL) 
	{
        int zDelta = (int) (short) HIWORD(wParam);
        TEventUI event = { 0 };
        event.Type = UIEVENT_SCROLLWHEEL;
        event.wParam = MAKELPARAM(zDelta < 0 ? SB_LINEDOWN : SB_LINEUP, 0);
        event.lParam = lParam;
        event.dwTimestamp = ::GetTickCount();
        m_pOwner->DoEvent(event);
        return 0;
    }
    else if (uMsg == WM_KILLFOCUS) 
	{
        if (m_hWnd != (HWND)wParam) PostMessage(WM_CLOSE);
    }

    LRESULT lRes = 0;
    if (m_pm.MessageHandler(uMsg, wParam, lParam, lRes)) return lRes;
    return CWindowWnd::HandleMessage(uMsg, wParam, lParam);
}

void CMenuWnd::EnsureVisible(int iIndex)
{
    RECT rcItem = m_pLayout->GetItemAt(iIndex)->GetPos();
    RECT rcList = m_pLayout->GetPos();
    CScrollBarUI* pHorizontalScrollBar = m_pLayout->GetHorizontalScrollBar();
    if (pHorizontalScrollBar && pHorizontalScrollBar->IsVisible()) rcList.bottom -= pHorizontalScrollBar->GetFixedHeight();
    int iPos = m_pLayout->GetScrollPos().cy;
    if (rcItem.top >= rcList.top && rcItem.bottom < rcList.bottom) return;
    int dx = 0;
    if (rcItem.top < rcList.top) dx = rcItem.top - rcList.top;
    if (rcItem.bottom > rcList.bottom) dx = rcItem.bottom - rcList.bottom;
    Scroll(0, dx);
}

void CMenuWnd::Scroll(int dx, int dy)
{
    if (dx == 0 && dy == 0) return;
    SIZE sz = m_pLayout->GetScrollPos();
    m_pLayout->SetScrollPos(CSize(sz.cx + dx, sz.cy + dy));
}

#if(_WIN32_WINNT >= 0x0501)
UINT CMenuWnd::GetClassStyle() const
{
	return __super::GetClassStyle() | CS_DROPSHADOW;
}
#endif
////////////////////////////////////////////////////////


CMenuUI::CMenuUI() : m_pWindow(NULL), m_uButtonState(0), 
	m_uTextStyle(DT_VCENTER), m_dwTextColor(0), m_dwDisabledTextColor(0), m_iFont(-1)
{
    ::ZeroMemory(&m_rcTextPadding, sizeof(m_rcTextPadding));
}

LPCTSTR CMenuUI::GetClass() const
{
    return _T("CMenuUI");
}

LPVOID CMenuUI::GetInterface(LPCTSTR pstrName)
{
	if (_tcscmp(pstrName, _T("Menu")) == 0) return static_cast<CMenuUI*>(this);
    return CListUI::GetInterface(pstrName);
}

bool CMenuUI::SetItemIndex(CControlUI* pControl, int iIndex)
{
    CMenuElementUI* pMenuItem = static_cast<CMenuElementUI*>(pControl->GetInterface("MenuElement"));
	if (pMenuItem == NULL) return false;
	return __super::SetItemIndex(pControl, iIndex);
}

void CMenuUI::SetTextStyle(UINT uStyle)
{
    m_uTextStyle = uStyle;
    Invalidate();
}

UINT CMenuUI::GetTextStyle() const
{
	return m_uTextStyle;
}

void CMenuUI::SetTextColor(DWORD dwTextColor)
{
    m_dwTextColor = dwTextColor;
}

DWORD CMenuUI::GetTextColor() const
{
	return m_dwTextColor;
}

void CMenuUI::SetDisabledTextColor(DWORD dwTextColor)
{
    m_dwDisabledTextColor = dwTextColor;
}

DWORD CMenuUI::GetDisabledTextColor() const
{
	return m_dwDisabledTextColor;
}

void CMenuUI::SetFont(int index)
{
    m_iFont = index;
}

int CMenuUI::GetFont() const
{
	return m_iFont;
}

bool CMenuUI::Add(CControlUI* pControl)
{
    CMenuElementUI* pMenuItem = static_cast<CMenuElementUI*>(pControl->GetInterface(_T("MenuItem")));
    if (pMenuItem) 
    {
        pMenuItem->SetOwner(this);
        pMenuItem->SetIndex(m_items.GetSize());
    }
    return CContainerUI::Add(pControl);
}

bool CMenuUI::AddAt(CControlUI* pControl, int iIndex)
{
	int i;

    if (!CContainerUI::AddAt(pControl, iIndex)) return false;

    CMenuElementUI* pMenuItem = static_cast<CMenuElementUI*>(pControl->GetInterface(_T("MenuItem")));
    if (pMenuItem) 
	{
        pMenuItem->SetOwner(this);
        pMenuItem->SetIndex(iIndex);
    }

    for (i = iIndex + 1; i < GetCount(); ++i) 
	{
        CControlUI* p = GetItemAt(i);
        pMenuItem = static_cast<CMenuElementUI*>(p->GetInterface(_T("MenuItem")));
        if (pMenuItem) 
		{
            pMenuItem->SetIndex(i);
        }
    }
    if( m_iCurSel >= iIndex ) m_iCurSel += 1;
    return true;
}

bool CMenuUI::Remove(CControlUI* pControl)
{
    int iIndex = GetItemIndex(pControl);
	int i;

    if (iIndex == -1) return false;

    if (!CContainerUI::RemoveAt(iIndex)) return false;

    for (i = iIndex; i < GetCount(); ++i) 
	{
        CControlUI* p = GetItemAt(i);
        CMenuElementUI* pMenuItem = static_cast<CMenuElementUI*>(p->GetInterface(_T("MenuItem")));
        if (pMenuItem) 
		{
            pMenuItem->SetIndex(i);
        }
    }

    if (iIndex == m_iCurSel && m_iCurSel >= 0 ) 
	{
        int iSel = m_iCurSel;
        m_iCurSel = -1;
        SelectItem(FindSelectable(iSel, false));
    }
    else if (iIndex < m_iCurSel) m_iCurSel -= 1;
    return true;
}

bool CMenuUI::RemoveAt(int iIndex)
{
	int i;

    if (!CContainerUI::RemoveAt(iIndex)) return false;

    for (i = iIndex; i < GetCount(); ++i) 
	{
        CControlUI* p = GetItemAt(i);
        CMenuElementUI* pMenuItem = static_cast<CMenuElementUI*>(p->GetInterface(_T("MenuItem")));
        if (pMenuItem) pMenuItem->SetIndex(i);
    }

    if (iIndex == m_iCurSel && m_iCurSel >= 0) 
	{
        int iSel = m_iCurSel;
        m_iCurSel = -1;
        SelectItem(FindSelectable(iSel, false));
    }
    else if (iIndex < m_iCurSel) m_iCurSel -= 1;
    return true;
}

void CMenuUI::RemoveAll()
{
    m_iCurSel = -1;
    CContainerUI::RemoveAll();
}

void CMenuUI::DoEvent(TEventUI& event)
{
    if( !IsMouseEnabled() && event.Type > UIEVENT__MOUSEBEGIN && event.Type < UIEVENT__MOUSEEND ) {
        if( m_pParent != NULL ) m_pParent->DoEvent(event);
        else CContainerUI::DoEvent(event);
        return;
    }

    if( event.Type == UIEVENT_SETFOCUS ) 
    {
        Invalidate();
    }
    if( event.Type == UIEVENT_KILLFOCUS ) 
    {
        Invalidate();
    }
    if( event.Type == UIEVENT_BUTTONDOWN )
    {
        if( IsEnabled() ) {
            Activate();
            m_uButtonState |= UISTATE_PUSHED | UISTATE_CAPTURED;
        }
        return;
    }
    if( event.Type == UIEVENT_BUTTONUP )
    {
        if( (m_uButtonState & UISTATE_CAPTURED) != 0 ) {
            m_uButtonState &= ~ UISTATE_CAPTURED;
            Invalidate();
        }
        return;
    }
    if( event.Type == UIEVENT_MOUSEMOVE )
    {
        return;
    }
    if( event.Type == UIEVENT_KEYDOWN )
    {
        switch( event.chKey ) {
        case VK_F4:
            Activate();
            return;
        case VK_UP:
            SelectItem(FindSelectable(m_iCurSel - 1, false));
            return;
        case VK_DOWN:
            SelectItem(FindSelectable(m_iCurSel + 1, true));
            return;
        case VK_PRIOR:
            SelectItem(FindSelectable(m_iCurSel - 1, false));
            return;
        case VK_NEXT:
            SelectItem(FindSelectable(m_iCurSel + 1, true));
            return;
        case VK_HOME:
            SelectItem(FindSelectable(0, false));
            return;
        case VK_END:
            SelectItem(FindSelectable(GetCount() - 1, true));
            return;
        }
    }
    if( event.Type == UIEVENT_SCROLLWHEEL )
    {
        bool bDownward = LOWORD(event.wParam) == SB_LINEDOWN;
        SelectItem(FindSelectable(m_iCurSel + (bDownward ? 1 : -1), bDownward));
        return;
    }
    if( event.Type == UIEVENT_CONTEXTMENU )
    {
        return;
    }
    if( event.Type == UIEVENT_MOUSEENTER )
    {
        if( ::PtInRect(&m_rcItem, event.ptMouse ) ) {
            if( (m_uButtonState & UISTATE_HOT) == 0  )
                m_uButtonState |= UISTATE_HOT;
            Invalidate();
        }
        return;
    }
    if( event.Type == UIEVENT_MOUSELEAVE )
    {
        if( (m_uButtonState & UISTATE_HOT) != 0 ) {
            m_uButtonState &= ~UISTATE_HOT;
            Invalidate();
        }
        return;
    }
    CControlUI::DoEvent(event);
}

SIZE CMenuUI::EstimateSize(SIZE szAvailable)
{
    if (m_cxyFixed.cy == 0) return CSize(m_cxyFixed.cx, m_pManager->GetDefaultFontInfo()->tm.tmHeight + 12);
    return CControlUI::EstimateSize(szAvailable);
}

bool CMenuUI::Activate()
{
    if (!CControlUI::Activate()) return false;
    if (m_pWindow) return true;
    m_pWindow = new CMenuWnd();
    ASSERT(m_pWindow);
    m_pWindow->Init(NULL, CPoint());
    Invalidate();
    return true;
}

CStdString CMenuUI::GetText() const
{
    return m_sText;
}

void CMenuUI::SetEnabled(bool bEnable)
{
    CContainerUI::SetEnabled(bEnable);
    if( !IsEnabled() ) m_uButtonState = 0;
}

RECT CMenuUI::GetTextPadding() const
{
    return m_rcTextPadding;
}

void CMenuUI::SetTextPadding(RECT rc)
{
    m_rcTextPadding = rc;
    Invalidate();
}

LPCTSTR CMenuUI::GetNormalImage() const
{
    return m_sNormalImage;
}

void CMenuUI::SetNormalImage(LPCTSTR pStrImage)
{
    m_sNormalImage = pStrImage;
    Invalidate();
}

LPCTSTR CMenuUI::GetHotImage() const
{
    return m_sHotImage;
}

void CMenuUI::SetHotImage(LPCTSTR pStrImage)
{
    m_sHotImage = pStrImage;
    Invalidate();
}

LPCTSTR CMenuUI::GetPushedImage() const
{
    return m_sPushedImage;
}

void CMenuUI::SetPushedImage(LPCTSTR pStrImage)
{
    m_sPushedImage = pStrImage;
    Invalidate();
}

LPCTSTR CMenuUI::GetFocusedImage() const
{
    return m_sFocusedImage;
}

void CMenuUI::SetFocusedImage(LPCTSTR pStrImage)
{
    m_sFocusedImage = pStrImage;
    Invalidate();
}

LPCTSTR CMenuUI::GetDisabledImage() const
{
    return m_sDisabledImage;
}

void CMenuUI::SetDisabledImage(LPCTSTR pStrImage)
{
    m_sDisabledImage = pStrImage;
    Invalidate();
}


void CMenuUI::SetPos(RECT rc)
{
    // Put all elements out of sight
    RECT rcNull = { 0 };
    for( int i = 0; i < m_items.GetSize(); i++ ) static_cast<CControlUI*>(m_items[i])->SetPos(rcNull);
    // Position this control
    CControlUI::SetPos(rc);
}

void CMenuUI::SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue)
{
    if( _tcscmp(pstrName, _T("textpadding")) == 0 ) {
        RECT rcTextPadding = { 0 };
        LPTSTR pstr = NULL;
        rcTextPadding.left = _tcstol(pstrValue, &pstr, 10);  ASSERT(pstr);    
        rcTextPadding.top = _tcstol(pstr + 1, &pstr, 10);    ASSERT(pstr);    
        rcTextPadding.right = _tcstol(pstr + 1, &pstr, 10);  ASSERT(pstr);    
        rcTextPadding.bottom = _tcstol(pstr + 1, &pstr, 10); ASSERT(pstr);    
        SetTextPadding(rcTextPadding);
    }
    else if( _tcscmp(pstrName, _T("normalimage")) == 0 ) SetNormalImage(pstrValue);
    else if( _tcscmp(pstrName, _T("hotimage")) == 0 ) SetHotImage(pstrValue);
    else if( _tcscmp(pstrName, _T("pushedimage")) == 0 ) SetPushedImage(pstrValue);
    else if( _tcscmp(pstrName, _T("focusedimage")) == 0 ) SetFocusedImage(pstrValue);
    else if( _tcscmp(pstrName, _T("disabledimage")) == 0 ) SetDisabledImage(pstrValue);
    else CContainerUI::SetAttribute(pstrName, pstrValue);
}

void CMenuUI::DoPaint(HDC hDC, const RECT& rcPaint)
{
    CControlUI::DoPaint(hDC, rcPaint);
}

void CMenuUI::PaintStatusImage(HDC hDC)
{
    if( IsFocused() ) m_uButtonState |= UISTATE_FOCUSED;
    else m_uButtonState &= ~ UISTATE_FOCUSED;
    if( !IsEnabled() ) m_uButtonState |= UISTATE_DISABLED;
    else m_uButtonState &= ~ UISTATE_DISABLED;

    if( (m_uButtonState & UISTATE_DISABLED) != 0 ) {
        if( !m_sDisabledImage.IsEmpty() ) {
            if( !DrawImage(hDC, (LPCTSTR)m_sDisabledImage) ) m_sDisabledImage.Empty();
            else return;
        }
    }
    else if( (m_uButtonState & UISTATE_PUSHED) != 0 ) {
        if( !m_sPushedImage.IsEmpty() ) {
            if( !DrawImage(hDC, (LPCTSTR)m_sPushedImage) ) m_sPushedImage.Empty();
            else return;
        }
    }
    else if( (m_uButtonState & UISTATE_HOT) != 0 ) {
        if( !m_sHotImage.IsEmpty() ) {
            if( !DrawImage(hDC, (LPCTSTR)m_sHotImage) ) m_sHotImage.Empty();
            else return;
        }
    }
    else if( (m_uButtonState & UISTATE_FOCUSED) != 0 ) {
        if( !m_sFocusedImage.IsEmpty() ) {
            if( !DrawImage(hDC, (LPCTSTR)m_sFocusedImage) ) m_sFocusedImage.Empty();
            else return;
        }
    }

    if( !m_sNormalImage.IsEmpty() ) {
        if( !DrawImage(hDC, (LPCTSTR)m_sNormalImage) ) m_sNormalImage.Empty();
        else return;
    }
}

void CMenuUI::PaintText(HDC hDC)
{
	if (m_dwTextColor == 0) m_dwTextColor = m_pManager->GetDefaultFontColor();
    if (m_dwDisabledTextColor == 0) m_dwDisabledTextColor = m_pManager->GetDefaultDisabledColor();

    if (m_sText.IsEmpty()) return;

	RECT rcText = m_rcItem;
    rcText.left += m_rcTextPadding.left;
    rcText.right -= m_rcTextPadding.right;
    rcText.top += m_rcTextPadding.top;
    rcText.bottom -= m_rcTextPadding.bottom;
	if (IsEnabled()) 
	{
		CRenderEngine::DrawText(hDC, m_pManager, rcText, m_sText, m_dwTextColor, 
			m_iFont, DT_SINGLELINE | m_uTextStyle);
    }
    else 
	{
		CRenderEngine::DrawText(hDC, m_pManager, rcText, m_sText, m_dwDisabledTextColor, 
			m_iFont, DT_SINGLELINE | m_uTextStyle);
    }
}

} // namespace DirectUICore
