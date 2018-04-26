
// QuadTreeDemo.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols
#include "InitGdiplus.h"

// CQuadTreeDemoApp:
// See QuadTreeDemo.cpp for the implementation of this class
//

class CQuadTreeDemoApp : public CWinApp
{
public:
	CQuadTreeDemoApp();

// Overrides
public:
	virtual BOOL InitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()

	CInitGdiplus m_InitGdiplus;
};

extern CQuadTreeDemoApp theApp;