#pragma once


// CNextBlockDlg 对话框

class CNextBlockDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CNextBlockDlg)

public:
	CNextBlockDlg(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CNextBlockDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_NEXTBLOCK };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

public:
	int m_type;
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();

private:
	void DrawNext(int type, CDC *pDC);
public:
	afx_msg void OnBnClickedButtonNextok();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnBnClickedButtonPlace();
};
