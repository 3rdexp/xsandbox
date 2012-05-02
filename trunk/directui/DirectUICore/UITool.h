#ifndef __UITOOL_H__
#define __UITOOL_H__

namespace DirectUICore 
{

class UILIB_API CToolStripUI : public CHorizontalLayoutUI 
{
public:
	CToolStripUI();
	LPCTSTR GetClass() const;
    LPVOID GetInterface(LPCTSTR pstrName);
};

class UILIB_API CToolStripButtonUI : public COptionUI 
{
public:
	CToolStripButtonUI();
	LPCTSTR GetClass() const;
    LPVOID GetInterface(LPCTSTR pstrName);
};

}

#endif // __UITOOL_H__