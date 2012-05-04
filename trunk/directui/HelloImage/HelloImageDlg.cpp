// HelloImageDlg.cpp : implementation file
//

#include "stdafx.h"
#include "HelloImage.h"
#include "HelloImageDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CHelloImageDlg dialog




CHelloImageDlg::CHelloImageDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CHelloImageDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	Gdiplus::GdiplusStartupInput gdiplusStartupInput;
    Gdiplus::GdiplusStartup(&m_gdiplusToken, &gdiplusStartupInput, NULL);
	m_MemoryGraphics = NULL;
}

CHelloImageDlg::~CHelloImageDlg() 
{
	if (m_MemoryGraphics) delete m_MemoryGraphics; m_MemoryGraphics = NULL;
	Gdiplus::GdiplusShutdown(m_gdiplusToken);
}

void CHelloImageDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CHelloImageDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_WM_SIZE()
END_MESSAGE_MAP()


// CHelloImageDlg message handlers

BOOL CHelloImageDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here

	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CHelloImageDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CPaintDC dc(this);
		CRect rect;

		Gdiplus::Graphics pGraphics(dc.m_hDC);

		GetClientRect(&rect);
		Gdiplus::Bitmap pBitmap(rect.Width(), rect.Height());
		m_MemoryGraphics = Gdiplus::Graphics::FromImage(&pBitmap);
		m_MemoryGraphics->SetSmoothingMode(Gdiplus::SmoothingModeAntiAlias);
		m_MemoryGraphics->SetTextRenderingHint(Gdiplus::TextRenderingHintAntiAliasGridFit);

		Gdiplus::SolidBrush pWndBg(Gdiplus::Color(255, 238, 243, 250));
		Gdiplus::Rect pWndRect(0, 0, rect.Width(), rect.Height());
		m_MemoryGraphics->FillRectangle(&pWndBg, pWndRect);

		// TODO: Draw your stuff

		pGraphics.DrawImage(&pBitmap, rect.left, rect.top);

		if (m_MemoryGraphics) 
		{
			delete m_MemoryGraphics; 
			m_MemoryGraphics = NULL;
		}

		CDialog::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CHelloImageDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CHelloImageDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	Invalidate(TRUE);
}
