﻿#pragma once
#include "third-party/WzSerialPort.h"
#include "arm.h"
#include <vector>
#include <iterator>

#define AM_GOTO			1
#define AM_GRAB			2
#define AM_PLACE		3
#define AM_GOANGLETO	4
#define AM_STEERTO		5
#define AM_MOVETO		6
#define AM_SEGGOTO		7
#define AM_GOTOR		8
#define AM_GOANGLETOR	9


using std::vector;
using std::iterator;

typedef struct ArmMsg{
	int msg;
	vector<int> param;
}ArmMsg;

// CArmControlDlg 对话框

class CArmControlDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CArmControlDlg)

public:
	CArmControlDlg(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CArmControlDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_ARMCONTROL };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	void BindSerialPort(WzSerialPort * w, BOOL bArmInit = FALSE);
	void UpdateState();
	double m_X;
	double m_Y;
	double m_Z;
	double m_AX;
	double m_AY;
	double m_AZ;
	double m_moveX;
	double m_moveY;
	double m_moveToX;
	double m_moveToY;
	double m_moveR;
	double m_steer;
	CString m_state;
	Arm * m_pA;
	BOOL m_bRelative;
	BOOL m_bSettingMode;
	BOOL m_bLock;
	vector<ArmMsg> msgArray;
	HANDLE hThread;


private:
	WzSerialPort * m_pW;
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButtonCoorgo();
	afx_msg void OnBnClickedButtonGrab();
	afx_msg void OnBnClickedButtonPlace();
	afx_msg void OnBnClickedButtonGoangle();
	afx_msg void OnBnClickedButtonOpenpump();
	afx_msg void OnBnClickedButtonClosepump();
	afx_msg void OnBnClickedButtonSteergo();
	afx_msg void OnBnClickedButtonUnlock();
	afx_msg void OnBnClickedRelative();
	afx_msg void OnBnClickedAbsolute();
	afx_msg void OnBnClickedSettingon();
	afx_msg void OnBnClickedSettingoff();
	afx_msg void OnBnClickedButtonMove();
	void Grab(double x, double y, double des_x, double des_y, double r);
	void SetCoor(double x, double y, double z);
	void SetAngle(double x, double y, double z);
	void GoToR(int dx, int dy, int dz);
	void GoTo(int x, int y, int z);
	afx_msg void OnBnClickedButtonSeggo();
};

DWORD  WINAPI ArmCtrlThreadProc(LPVOID lpParam);