
// TetrisPlaceDlg.h: 头文件
//

#pragma once

#include "ConnectDlg.h"
#include "SerialDlg.h"
#include "ArmControlDlg.h"
#include "NextBlockDlg.h"
#include "BoardDlg.h"
#include "CameraDlg.h"
#include "InfoDlg.h"
#include "third-party/WzSerialPort.h"
#include "Arm.h"
#include "StoreSetDlg.h"
#include "ArmSetDlg.h"

#define BAUDRATE	115200
#define PARITY		0
#define DATABIT		8
#define STOPBIT		0
#define SYNCHRONIZEFLAG	1


// CTetrisPlaceDlg 对话框
class CTetrisPlaceDlg : public CDialogEx
{
// 构造
public:
	CTetrisPlaceDlg(CWnd* pParent = nullptr);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_TETRISPLACE_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnClose();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	DECLARE_MESSAGE_MAP()

public:
	CConnectDlg * pCntDlg;
	CSerialDlg * pSerialDlg;
	CArmControlDlg *pArmCtrlDlg;
	CBoardDlg * pBoardDlg;
	CCameraDlg * pCameraDlg;
	CInfoDlg * pInfoDlg;
	WzSerialPort m_w;
	BOOL m_bCreate;

public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	CTabCtrl m_tab;
	afx_msg void OnTcnSelchangeTabSerial(NMHDR *pNMHDR, LRESULT *pResult);
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	afx_msg void OnMenuStoreset();
	void UpdateWindowPos();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnMenuArmset();
	afx_msg void OnStnDblclickEmergency();
};
