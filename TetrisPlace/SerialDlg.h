﻿#pragma once

#include "third-party/WzSerialPort.h"

// CSerialDlg 对话框

class CSerialDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CSerialDlg)

public:
	CSerialDlg(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CSerialDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_SERIAL };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	void UpdateReceive();
	afx_msg void OnBnClickedButtonSend();
	void StartListenPort();
	void BindSerialPort(WzSerialPort * w);
	BOOL m_bOpen;
	WzSerialPort * m_w;
	CString m_receive;
	CString m_send;
	HANDLE hThread;
};

DWORD WINAPI SerialThreadProc(LPVOID lpParam);
