
// QuadTreeDemoDlg.cpp : implementation file
//

#include "stdafx.h"
#include "QuadTreeDemo.h"
#include "QuadTreeDemoDlg.h"
#include "afxdialogex.h"

#include <functional>
#include <unordered_map>
#include <random>
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CQuadTreeDemoDlg dialog

///[min.max]
int Rand(int min, int max)
{
	std::default_random_engine generator(time(NULL));
	std::uniform_int_distribution<int> dis(min, max);
	auto dice = std::bind(dis, generator);
	return dice();
}

///[min,max)
double Randf(double min, double max)
{
	std::default_random_engine generator(time(NULL));
	std::uniform_real_distribution<double> dis(min, max);
	auto dice = std::bind(dis, generator);
	return dice();
}


CQuadTreeDemoDlg::CQuadTreeDemoDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_QUADTREEDEMO_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CQuadTreeDemoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CQuadTreeDemoDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDOWN()
	ON_WM_RBUTTONUP()
	ON_WM_RBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_KEYDOWN()
END_MESSAGE_MAP()


// CQuadTreeDemoDlg message handlers

BOOL CQuadTreeDemoDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
	GetClientRect(m_Rect);

	m_quadTree = new math::quad_tree<Item>(math::rect(0, 0, m_Rect.Width(), m_Rect.Height()));
	m_Render = new QuadTreeRender(m_quadTree);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CQuadTreeDemoDlg::OnPaint()
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
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CQuadTreeDemoDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CQuadTreeDemoDlg::Invalidate()
{
	HDC hdc = ::GetDC(m_hWnd);
	CDC dc;
	dc.Attach(hdc);
	CDC memDC;
	CBitmap MemBitmap;
	// 设备描述表初始化
	memDC.CreateCompatibleDC(NULL);
	// 建立与屏幕显示兼容的内存显示设备
	MemBitmap.CreateCompatibleBitmap(&dc, m_Rect.Width(), m_Rect.Height());
	// 选取空白位图
	memDC.SelectObject(MemBitmap);
	memDC.FillSolidRect(0, 0, m_Rect.Width(), m_Rect.Height(), RGB(255, 255, 255));
	//画图
	Draw(&memDC);
	//拷贝到控件DC
	dc.BitBlt(0, 0, m_Rect.Width(), m_Rect.Height(), &memDC, 0, 0, SRCCOPY);
	MemBitmap.DeleteObject();
	memDC.DeleteDC();
	dc.Detach();
	::ReleaseDC(m_hWnd, hdc);
}

void CQuadTreeDemoDlg::Draw(CDC * pDC)
{
	Graphics graph(pDC->GetSafeHdc());

	m_Render->Render(&graph);

	if (!m_selectionRect.IsEmptyArea())
	{
		SolidBrush b(Color(128, 253, 236, 171));
		graph.FillRectangle(&b,m_selectionRect);
	}

	if (!m_selectdItems.empty())
	{
		for (auto& item : m_selectdItems)
		{
			Rect rect = QuadTreeRender::ToGDIRect(item.rect());

			rect.Inflate(1, 1);
			Pen p(Color(128,255,0,0), 2);
			graph.DrawEllipse(&p, rect);
		}
	}
}




void CQuadTreeDemoDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	srand(clock());
	int size = 30;// Rand(0, 25) + 4;
	//Item e(point.x, point.y,size);
	m_quadTree->insert(point.x, point.y, size);
	Invalidate();
	CDialogEx::OnLButtonUp(nFlags, point);
}


void CQuadTreeDemoDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default

	CDialogEx::OnLButtonDown(nFlags, point);
}


void CQuadTreeDemoDlg::OnRButtonUp(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	math::rect rect(m_selectionRect.X,m_selectionRect.Y, m_selectionRect.Width, m_selectionRect.Height);
	m_selectdItems = m_quadTree->query(rect);
	m_dragging = false;
	Invalidate();
	CDialogEx::OnRButtonUp(nFlags, point);
}


void CQuadTreeDemoDlg::OnRButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	m_dragging = true;
	m_startPoint = point;
	CDialogEx::OnRButtonDown(nFlags, point);
}


void CQuadTreeDemoDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	if (m_dragging)
	{
		Point pt(std::min(point.x, m_startPoint.x), std::min(point.y, m_startPoint.y));
		Point pt2(std::max(point.x, m_startPoint.x),std::max(point.y, m_startPoint.y));
		m_selectionRect = Rect(
			pt,
			Size( pt2.X - pt.X, pt2.Y - pt.Y)
			);

		Invalidate();
	}

	CDialogEx::OnMouseMove(nFlags, point);
}


void CQuadTreeDemoDlg::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (nChar == 'C')
	{
		m_quadTree->clear();
		Invalidate();
	};
	CDialogEx::OnKeyDown(nChar, nRepCnt, nFlags);
}
