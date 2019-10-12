#pragma once


// CBoardDlg 对话框

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
	void DrawTetris(CDC *pDC, int type, int x, int y, int r);
	void DrawClient();

private:
	void DrawBorder(CDC *pDC);
	void DrawBoard(CDC *pDC);
	void DrawBlock(CDC *pDC, int x, int y, COLORREF rgb);

private:
	CBitmap m_memBmp;
	CBitmap * m_pOldBmp;
public:
	CDC m_memDC;
};
