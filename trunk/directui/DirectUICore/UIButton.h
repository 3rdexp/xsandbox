#ifndef __UIBUTTON_H__
#define __UIBUTTON_H__

namespace DirectUICore 
{

class UILIB_API CButtonUI : public CLabelUI
{
public:
    CButtonUI();

    LPCTSTR GetClass() const;
    LPVOID GetInterface(LPCTSTR pstrName);
    UINT GetControlFlags() const;

    bool Activate();
    void SetEnabled(bool bEnable = true);
    void DoEvent(TEventUI& event);

    LPCTSTR GetNormalImage();
    void SetNormalImage(LPCTSTR pStrImage);
    LPCTSTR GetHotImage();
    void SetHotImage(LPCTSTR pStrImage);
    LPCTSTR GetPushedImage();
    void SetPushedImage(LPCTSTR pStrImage);
    LPCTSTR GetFocusedImage();
    void SetFocusedImage(LPCTSTR pStrImage);
    LPCTSTR GetDisabledImage();
    void SetDisabledImage(LPCTSTR pStrImage);

    void SetHotTextColor(DWORD dwColor);
	DWORD GetHotTextColor() const;
    void SetPushedTextColor(DWORD dwColor);
	DWORD GetPushedTextColor() const;
    void SetFocusedTextColor(DWORD dwColor);
	DWORD GetFocusedTextColor() const;
    SIZE EstimateSize(SIZE szAvailable);
    void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);

	void PaintText(HDC hDC);
    void PaintStatusImage(HDC hDC);

protected:
    UINT m_uButtonState;

	DWORD m_dwHotTextColor;
	DWORD m_dwPushedTextColor;
	DWORD m_dwFocusedTextColor;

    CStdString m_sNormalImage;
    CStdString m_sHotImage;
    CStdString m_sPushedImage;
    CStdString m_sFocusedImage;
    CStdString m_sDisabledImage;
};

}

#endif // __UIBUTTON_H__