#pragma once

#include "Arm.h"

// CArmSetDlg 对话框

class CArmSetDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CArmSetDlg)

public:
	CArmSetDlg(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CArmSetDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_ARMSETTING };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
private:
	int m_startX;
	int m_startY;
	int m_startZ;
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
};
