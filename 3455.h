
// 3455.h : main header file for the 3455 application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols
#include "LeftView.h"
#include "RightView.h"


// CExcerciseApp:
// See 3455.cpp for the implementation of this class
//

class CExcerciseApp : public CWinAppEx
{
public:
	CExcerciseApp();


// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// Implementation
	UINT  m_nAppLook;
	BOOL  m_bHiColorIcons;

	virtual void PreLoadState();
	virtual void LoadCustomState();
	virtual void SaveCustomState();

	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CExcerciseApp theApp;
