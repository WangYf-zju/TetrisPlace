#pragma once
#include "TetrisAI.h"

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

private:
	int m_type;
	TetrisAI * m_nextTR;

public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnBnClickedButtonPlace();
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	void DrawNext(int type, CDC *pDC);
	CComboBox m_comTetris;
	afx_msg void OnCbnSelchangeComboTetris();
};
