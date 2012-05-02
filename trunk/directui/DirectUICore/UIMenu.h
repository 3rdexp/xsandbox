#ifndef __UIMENU_H__
#define __UIMENU_H__

#pragma once

namespace DirectUICore 
{

class UILIB_API CMenuStripUI : public CHorizontalLayoutUI 
{
public:
	CMenuStripUI();
	LPCTSTR GetClass() const;
    LPVOID GetInterface(LPCTSTR pstrName);
};

/////////////////////////////////////////////////////////////////////////////////////
//

class CMenuWnd;

class UILIB_API CMenuUI : public CListUI 
{
    friend class CMenuWnd;
public:
    CMenuUI();

    LPCTSTR GetClass() const;
    LPVOID GetInterface(LPCTSTR pstrName);

    CStdString GetText() const;
    void SetEnabled(bool bEnable = true);

    bool SetItemIndex(CControlUI* pControl, int iIndex);
    bool Add(CControlUI* pControl);
    bool AddAt(CControlUI* pControl, int iIndex);
    bool Remove(CControlUI* pControl);
    bool RemoveAt(int iIndex);
    void RemoveAll();

    bool Activate();

	void SetTextStyle(UINT uStyle);
	UINT GetTextStyle() const;
	void SetTextColor(DWORD dwTextColor);
	DWORD GetTextColor() const;
    void SetDisabledTextColor(DWORD dwTextColor);
	DWORD GetDisabledTextColor() const;
    void SetFont(int index);
	int GetFont() const;
	UINT m_uTextStyle;

    RECT GetTextPadding() const;
    void SetTextPadding(RECT rc);
    LPCTSTR GetNormalImage() const;
    void SetNormalImage(LPCTSTR pStrImage);
    LPCTSTR GetHotImage() const;
    void SetHotImage(LPCTSTR pStrImage);
    LPCTSTR GetPushedImage() const;
    void SetPushedImage(LPCTSTR pStrImage);
    LPCTSTR GetFocusedImage() const;
    void SetFocusedImage(LPCTSTR pStrImage);
    LPCTSTR GetDisabledImage() const;
    void SetDisabledImage(LPCTSTR pStrImage);

    SIZE EstimateSize(SIZE szAvailable);
    void SetPos(RECT rc);
    void DoEvent(TEventUI& event);
    void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);
    
    void DoPaint(HDC hDC, const RECT& rcPaint);
    void PaintText(HDC hDC);
    void PaintStatusImage(HDC hDC);

protected:
    CMenuWnd* m_pWindow;

    RECT m_rcTextPadding;
    UINT m_uButtonState;

	DWORD m_dwTextColor;
    DWORD m_dwDisabledTextColor;
    int m_iFont;

    CStdString m_sNormalImage;
    CStdString m_sHotImage;
    CStdString m_sPushedImage;
    CStdString m_sFocusedImage;
    CStdString m_sDisabledImage;
};

class CMenuElementUI;

class CMenuWnd : public CWindowWnd
{
public:
	void Init(CMenuElementUI* pOwner, POINT point);
    LPCTSTR GetWindowClassName() const;
    void OnFinalMessage(HWND hWnd);

    LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);

    void EnsureVisible(int iIndex);
    void Scroll(int dx, int dy);

#if(_WIN32_WINNT >= 0x0501)
	virtual UINT GetClassStyle() const;
#endif

public:
    CPaintManagerUI m_pm;
	CMenuElementUI* m_pOwner;
    CVerticalLayoutUI* m_pLayout;
	POINT m_BasedPoint;
    int m_iOldSel;
};

struct ContextMenuParam
{
	// 1: remove all
	// 2: remove the sub menu
	WPARAM wParam;
	HWND hWnd;
};

class CMenuElementUI : public CListContainerElementUI
{
	friend CMenuWnd;
public:
    CMenuElementUI();
	~CMenuElementUI();

    LPCTSTR GetClass() const;
    LPVOID GetInterface(LPCTSTR pstrName);

    void DoPaint(HDC hDC, const RECT& rcPaint);

	void DrawItemText(HDC hDC, const RECT& rcItem);

	SIZE EstimateSize(SIZE szAvailable);

	bool Activate();

	void DoEvent(TEventUI& event);

	CMenuWnd* GetMenuWnd();

	void CreateMenuWnd();

protected:
	CMenuWnd* m_pWindow;
};

} // namespace DirectUICore

#endif // __UIMENU_H__
