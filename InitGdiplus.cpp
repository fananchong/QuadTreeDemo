#include "stdafx.h"
#include "InitGdiplus.h"


CInitGdiplus::CInitGdiplus()
{
	GdiplusStartup(&m_GdiplusToken, &gdiplusStartupInput, NULL);
}


CInitGdiplus::~CInitGdiplus()
{
	GdiplusShutdown(m_GdiplusToken);
}
