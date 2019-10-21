// ConnectDlg.cpp: 实现文件
//

#include "stdafx.h"
#include "TetrisPlace.h"
#include "ConnectDlg.h"
#include "afxdialogex.h"
#include "TetrisPlaceDlg.h"

// CConnectDlg 对话框

IMPLEMENT_DYNAMIC(CConnectDlg, CDialogEx)

CConnectDlg::CConnectDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_CONNECT, pParent)
{

}

CConnectDlg::~CConnectDlg()
{
}

void CConnectDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_PORT, m_comPort);
	DDX_Control(pDX, IDC_COMBO_CAMERA, m_comCamera);
}


BEGIN_MESSAGE_MAP(CConnectDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_REFRESH, &CConnectDlg::OnBnClickedButtonRefresh)
	ON_BN_CLICKED(IDC_BUTTON_CON, &CConnectDlg::OnBnClickedButtonCon)
	ON_BN_CLICKED(IDC_BUTTON_RUN, &CConnectDlg::OnBnClickedButtonRun)
	ON_BN_CLICKED(IDC_RADIO_LOOP, &CConnectDlg::OnBnClickedRadioLoop)
	ON_BN_CLICKED(IDC_RADIO_ONCE, &CConnectDlg::OnBnClickedRadioOnce)
	ON_BN_CLICKED(IDC_BUTTON_ARMCORRECT, &CConnectDlg::OnBnClickedButtonArmcorrect)
END_MESSAGE_MAP()


// CConnectDlg 消息处理程序


BOOL CConnectDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN)
	{
		return TRUE;
	}
	else if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_ESCAPE)
	{
		return TRUE;
	}
	return CDialogEx::PreTranslateMessage(pMsg);
}


BOOL CConnectDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_bPortOpen = FALSE;
	m_bCameraOpen = FALSE;
	m_OnceLoop = RUN_ONCE;
	((CButton*)GetDlgItem(IDC_RADIO_ONCE))->SetCheck(1);
	OnBnClickedButtonRefresh();
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}


int CConnectDlg::RefreshPort()
{
	// TODO: 在此处添加实现代码.
	EnumSerialPorts(m_portList);
	m_comPort.ResetContent();
	for (int i = 0; i < m_portList.GetSize(); i++)
	{
		int index = m_portList[i].strFriendlyName.Find(_T("COM"), 0);
		int length = m_portList[i].strFriendlyName.GetLength() - index - 1;
		m_comPort.AddString(m_portList[i].strFriendlyName.Mid(index, length));
	}
	return m_portList.GetSize();
}


int CConnectDlg::RefreshCamera()
{
	// TODO: 在此处添加实现代码.
	int count = listCamera(m_cameraList);
	m_comCamera.ResetContent();
	for (int i = 0; i < count; i++)
	{
		m_comCamera.AddString(m_cameraList[i].c_str());
	}
	return 0;
}


void CConnectDlg::OnBnClickedButtonRefresh()
{
	// TODO: 在此添加控件通知处理程序代码
	RefreshPort();
	RefreshCamera();
	m_comPort.SetCurSel(0);
	m_comCamera.SetCurSel(0);
}


void CConnectDlg::OnBnClickedButtonCon()
{
	// TODO: 在此添加控件通知处理程序代码
	Connect();
}


void CConnectDlg::Connect()
{
	// TODO: 在此处添加实现代码.
	CTetrisPlaceDlg * parentDlg = (CTetrisPlaceDlg*)(GetParent());
	int iPortSel = m_comPort.GetCurSel();
	if (!m_bPortOpen && iPortSel >= 0)
	{
		CString str;
		m_comPort.GetLBText(m_comPort.GetCurSel(), str);
		int portNo = _ttoi(str.Mid(3, str.GetLength() - 3));
		if (parentDlg->m_w.open(portNo, BAUDRATE,
			PARITY, DATABIT, STOPBIT, SYNCHRONIZE))
		{
			m_bPortOpen = TRUE;
			if (MessageBox(_T("重置机械臂参数？"), _T("连接"),
				MB_YESNO) == IDYES)
			{
				parentDlg->pArmCtrlDlg->BindSerialPort(&parentDlg->m_w, TRUE);
			}
			else
			{
				parentDlg->pArmCtrlDlg->BindSerialPort(&parentDlg->m_w, FALSE);
			}
			parentDlg->pSerialDlg->BindSerialPort(&parentDlg->m_w);
			//parentDlg->pSerialDlg->StartListenPort();
		}
		else
		{
			MessageBox(_T("串口连接失败"), _T("连接"));
		}
	}
	else
	{
		MessageBox(_T("请选择一个串口"), _T("连接"));
	}
	int iCameraSel = m_comCamera.GetCurSel();
	if (!m_bCameraOpen && iCameraSel >= 0)
	{
		if (((CTetrisPlaceDlg*)GetParent())->pCameraDlg->StartCamera(iCameraSel))
			m_bCameraOpen = TRUE;
	}
}


void CConnectDlg::OnBnClickedButtonRun()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_bPortOpen && m_bCameraOpen)
	{
		CTetrisPlaceDlg * parentDlg = (CTetrisPlaceDlg*)GetParent();
		if (m_OnceLoop == RUN_ONCE)
			parentDlg->pCameraDlg->StartDistinguishAndGrabOnce();
		else if (m_OnceLoop == RUN_LOOP)
			parentDlg->pCameraDlg->StartDistinguishAndGrabLoop();
	}
	else
		MessageBox(_T("串口或相机没有连接"), _T("运行"));
}


void CConnectDlg::OnBnClickedRadioLoop()
{
	// TODO: 在此添加控件通知处理程序代码
	m_OnceLoop = RUN_LOOP;
}


void CConnectDlg::OnBnClickedRadioOnce()
{
	// TODO: 在此添加控件通知处理程序代码
	m_OnceLoop = RUN_ONCE;
}


void CConnectDlg::OnBnClickedButtonArmcorrect()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_bPortOpen && m_bCameraOpen)
	{

	}
	else
		MessageBox(_T("串口或相机没有连接"), _T("运行"));
}
