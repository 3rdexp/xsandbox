#ifndef __UIMENU_H__
#define __UIMENU_H__

#pragma once

namespace DirectUICore {
/////////////////////////////////////////////////////////////////////////////////////
//

class CMenuWnd;

class UILIB_API CMenuUI : public CContainerUI, public IListOwnerUI
{
    friend class CMenuWnd;
public:
    CMenuUI();

    LPCTSTR GetClass() const;
    LPVOID GetInterface(LPCTSTR pstrName);

    void DoInit();
    UINT GetControlFlags() const;

    CStdString GetText() const;
    void SetEnabled(bool bEnable = true);

    CStdString GetDropBoxAttributeList();
    void SetDropBoxAttributeList(LPCTSTR pstrList);
    SIZE GetDropBoxSize() const;
    void SetDropBoxSize(SIZE szDropBox);

    int GetCurSel() const;  
    bool SelectItem(int iIndex, bool bTakeFocus = false);

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

    TListInfoUI* GetListInfo();
    void SetItemFont(int index);
    void SetItemTextStyle(UINT uStyle);
	RECT GetItemTextPadding() const;
    void SetItemTextPadding(RECT rc);
	DWORD GetItemTextColor() const;
    void SetItemTextColor(DWORD dwTextColor);
	DWORD GetItemBkColor() const;
    void SetItemBkColor(DWORD dwBkColor);
	LPCTSTR GetItemBkImage() const;
    void SetItemBkImage(LPCTSTR pStrImage);
    bool IsAlternateBk() const;
    void SetAlternateBk(bool bAlternateBk);
	DWORD GetSelectedItemTextColor() const;
    void SetSelectedItemTextColor(DWORD dwTextColor);
	DWORD GetSelectedItemBkColor() const;
    void SetSelectedItemBkColor(DWORD dwBkColor);
	LPCTSTR GetSelectedItemImage() const;
    void SetSelectedItemImage(LPCTSTR pStrImage);
	DWORD GetHotItemTextColor() const;
    void SetHotItemTextColor(DWORD dwTextColor);
	DWORD GetHotItemBkColor() const;
    void SetHotItemBkColor(DWORD dwBkColor);
	LPCTSTR GetHotItemImage() const;
    void SetHotItemImage(LPCTSTR pStrImage);
	DWORD GetDisabledItemTextColor() const;
    void SetDisabledItemTextColor(DWORD dwTextColor);
	DWORD GetDisabledItemBkColor() const;
    void SetDisabledItemBkColor(DWORD dwBkColor);
	LPCTSTR GetDisabledItemImage() const;
    void SetDisabledItemImage(LPCTSTR pStrImage);
	DWORD GetItemLineColor() const;
    void SetItemLineColor(DWORD dwLineColor);
    bool IsItemShowHtml();
    void SetItemShowHtml(bool bShowHtml = true);

    SIZE EstimateSize(SIZE szAvailable);
    void SetPos(RECT rc);
    void DoEvent(TEventUI& event);
    void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);
    
    void DoPaint(HDC hDC, const RECT& rcPaint);
    void PaintText(HDC hDC);
    void PaintStatusImage(HDC hDC);

protected:
    CMenuWnd* m_pWindow;

    int m_iCurSel;
    RECT m_rcTextPadding;
    CStdString m_sDropBoxAttributes;
    SIZE m_szDropBox;
    UINT m_uButtonState;

	DWORD m_dwTextColor;
    DWORD m_dwDisabledTextColor;
    int m_iFont;

    CStdString m_sNormalImage;
    CStdString m_sHotImage;
    CStdString m_sPushedImage;
    CStdString m_sFocusedImage;
    CStdString m_sDisabledImage;

    TListInfoUI m_ListInfo;
};

class CMenuElementUI;

class CMenuWnd : public CWindowWnd
{
public:
    void Init(CMenuUI* pOwner);
	void Init(CMenuElementUI* pSubOwner, POINT point);
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
    CMenuUI* m_pOwner;
	CMenuElementUI* m_pSubOwner;
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
