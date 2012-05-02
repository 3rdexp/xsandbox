#ifndef __UITEXT_H__
#define __UITEXT_H__

namespace DirectUICore 
{

class UILIB_API CTextUI : public CLabelUI
{
public:
    CTextUI();
    ~CTextUI();

    LPCTSTR GetClass() const;
    UINT GetControlFlags() const;
    LPVOID GetInterface(LPCTSTR pstrName);

    CStdString* GetLinkContent(int iIndex);

    void DoEvent(TEventUI& event);
    SIZE EstimateSize(SIZE szAvailable);

    void PaintText(HDC hDC);

protected:
    enum { MAX_LINK = 8 };
    int m_nLinks;
    RECT m_rcLinks[MAX_LINK];
    CStdString m_sLinks[MAX_LINK];
    int m_nHoverLink;
};

}

#endif // __UITEXT_H__