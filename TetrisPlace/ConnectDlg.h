#pragma once
#include "third-party/EnumSerial.h"
#include "third-party/CameraList.h"
#include <vector>
#include <string>
using std::vector;
using std::string;

#define RUN_ONCE 0
#define RUN_LOOP 1

// CConnectDlg 对话框

class CConnectDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CConnectDlg)

public:
	CConnectDlg(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CConnectDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_CONNECT };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL OnInitDialog();
	int RefreshPort();
	int RefreshCamera();

private:
	CArray< SSerInfo, SSerInfo&> m_portList;
	CameraList m_cameraList;
public:
	CComboBox m_comPort;
	CComboBox m_comCamera;
	BOOL m_bCameraOpen;
	BOOL m_bPortOpen;
	int m_OnceLoop;
	afx_msg void OnBnClickedButtonRefresh();
	afx_msg void OnBnClickedButtonCon();
	void Connect();
	afx_msg void OnBnClickedButtonRun();
	afx_msg void OnBnClickedRadioLoop();
	afx_msg void OnBnClickedRadioOnce();
};
