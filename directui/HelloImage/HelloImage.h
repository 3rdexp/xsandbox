// HelloImage.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// CHelloImageApp:
// See HelloImage.cpp for the implementation of this class
//

class CHelloImageApp : public CWinApp
{
public:
	CHelloImageApp();

// Overrides
	public:
	virtual BOOL InitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()
};

extern CHelloImageApp theApp;