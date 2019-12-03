#pragma once

#include "SerialDlg.h"
#include "ArmControlDlg.h"
#include "ServerDlg.h"

// CLeftColDlg 对话框

class CLeftColDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CLeftColDlg)

public:
	CLeftColDlg(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CLeftColDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_LEFTCOL };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	CSerialDlg * pSerialDlg;
	CArmControlDlg * pArmCtrlDlg;
	CServerDlg * pServerDlg;
	CTabCtrl m_tab;
	afx_msg void OnTcnSelchangeTab(NMHDR *pNMHDR, LRESULT *pResult);
	void UpdateWindowPos();
};
