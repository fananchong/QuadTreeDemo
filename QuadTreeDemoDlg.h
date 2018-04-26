
// QuadTreeDemoDlg.h : header file
//

#pragma once
#include "QuadTreeRender.h"

// CQuadTreeDemoDlg dialog
class CQuadTreeDemoDlg : public CDialogEx
{
// Construction
public:
	CQuadTreeDemoDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_QUADTREEDEMO_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

	CRect m_Rect; //对话框的矩形区域
	math::quad_tree<Item>* m_quadTree;
	QuadTreeRender* m_Render;

	bool m_dragging = false;
	Rect  m_selectionRect;
	CPoint m_startPoint;

	std::vector<Item> m_selectdItems;

	void Invalidate();
	void Draw(CDC *pDC);
public:
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
};
