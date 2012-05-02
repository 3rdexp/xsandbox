#include "StdAfx.h"

namespace DirectUICore 
{

CToolStripUI::CToolStripUI() {}

LPCTSTR CToolStripUI::GetClass() const
{
    return _T("ToolStripUI");
}

LPVOID CToolStripUI::GetInterface(LPCTSTR pstrName)
{
    if (_tcscmp(pstrName, _T("ToolStrip")) == 0) return static_cast<CToolStripUI*>(this);
    return CHorizontalLayoutUI::GetInterface(pstrName);
}

CToolStripButtonUI::CToolStripButtonUI() {}

LPCTSTR CToolStripButtonUI::GetClass() const
{
    return _T("ToolStripButtonUI");
}

LPVOID CToolStripButtonUI::GetInterface(LPCTSTR pstrName)
{
    if (_tcscmp(pstrName, _T("ToolStripButton")) == 0) return static_cast<CToolStripButtonUI*>(this);
    return COptionUI::GetInterface(pstrName);
}

}