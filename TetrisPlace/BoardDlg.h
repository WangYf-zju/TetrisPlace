#pragma once


// CBoardDlg 对话框
#include "TetrisAI.h"

class CBoardDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CBoardDlg)

public:
	CBoardDlg(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CBoardDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_BOARD };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL OnInitDialog();
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	void DrawTetris(CDC *pDC, int type, int x, int y, int r);
	void DrawTetris(CDC *pDC, int type, Position * pos);
	void DrawClient();
	void InitDlg();

private:
	void DrawBorder(CDC *pDC);
	void DrawBoard(CDC *pDC);
	void DrawBlock(CDC *pDC, int x, int y, COLORREF rgb);
	int GetGridX(CPoint * pt);
	int GetGridX(double x);
	int GetGridY(CPoint * pt);
	int GetGridY(double y);

private:
	CBitmap m_memBmp;
	CBitmap * m_pOldBmp;
	int m_sideLength;
	BOOL m_bModify;
public:
	CDC m_memDC;
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnBnClickedButtonModify();
	afx_msg void OnBnClickedButtonClearboard();
	afx_msg void OnBnClickedButtonRefreshboard();
};
