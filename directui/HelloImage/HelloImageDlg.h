// HelloImageDlg.h : header file
//

#pragma once

#include "GifImage.h"

// CHelloImageDlg dialog
class CHelloImageDlg : public CDialog
{
// Construction
public:
	CHelloImageDlg(CWnd* pParent = NULL);	// standard constructor
	~CHelloImageDlg();

// Dialog Data
	enum { IDD = IDD_HELLOIMAGE_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	DECLARE_MESSAGE_MAP()

private:
	ULONG_PTR			m_gdiplusToken;
	Gdiplus::Graphics*	m_MemoryGraphics;
	GifImage*			m_GifImage;
};
