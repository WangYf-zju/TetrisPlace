
// TetrisPlaceDlg.h: 头文件
//

#pragma once

#include "SerialDlg.h"
#include "ArmControlDlg.h"
#include "NextBlockDlg.h"
#include "BoardDlg.h"
#include "third-party/WzSerialPort.h"
#include "Arm.h"

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
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

public:
	CNextBlockDlg * pNextBlockDlg;
	CSerialDlg * pSerialDlg;
	CArmControlDlg *pArmCtrlDlg;
	CBoardDlg * pBoardDlg;
	WzSerialPort m_w;
	Arm m_a;

public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
