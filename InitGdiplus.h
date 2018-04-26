#pragma once

#include <GdiPlus.h>
using namespace Gdiplus;

class CInitGdiplus
{
public:
	CInitGdiplus();
	~CInitGdiplus();

private:
	ULONG_PTR m_GdiplusToken;
	GdiplusStartupInput gdiplusStartupInput;
};

