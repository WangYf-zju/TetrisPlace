// CSerialDlg.cpp: 实现文件
//

#include "stdafx.h"
#include "TetrisPlace.h"
#include "SerialDlg.h"
#include "afxdialogex.h"
#include "TetrisPlaceDlg.h"


// CSerialDlg 对话框

IMPLEMENT_DYNAMIC(CSerialDlg, CDialogEx)

CSerialDlg::CSerialDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_SERIAL, pParent)
	, m_receive(_T(""))
	, m_send(_T(""))
{

}

CSerialDlg::~CSerialDlg()
{
}

void CSerialDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_COM, m_com);
	DDX_Text(pDX, IDC_EDIT_RX, m_receive);
	DDX_Text(pDX, IDC_EDIT_TX, m_send);
}


BEGIN_MESSAGE_MAP(CSerialDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_SEND, &CSerialDlg::OnBnClickedButtonSend)
	ON_BN_CLICKED(IDC_BUTTON_OPENPORT, &CSerialDlg::OnBnClickedButtonOpenport)
	ON_BN_CLICKED(IDC_BUTTON_REFRESH, &CSerialDlg::OnBnClickedButtonRefresh)
END_MESSAGE_MAP()


// CSerialDlg 消息处理程序


BOOL CSerialDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	// TODO:  在此添加额外的初始化
	m_bOpen = TRUE;
	m_w = &((CTetrisPlaceDlg*)GetParent())->m_w;
	//GetConnectedPort();
	//hThread = CreateThread(NULL, 0, SerialThreadProc, this, 0, 0);
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}


void CSerialDlg::GetConnectedPort()
{
	// TODO: 在此处添加实现代码.
	WzSerialPort w;
	for (int i = 0; i < 20; i++)
	{
		if (w.open(i, 115200, 0, 8, 1, 1))
		{
			CString str;
			str.Format(_T("COM%d"), i);
			m_com.AddString(str);
			w.close();
		}
	}
}

void CSerialDlg::UpdateReceive()
{
	GetDlgItem(IDC_EDIT_RX)->SetWindowTextW(m_receive);
}


BOOL CSerialDlg::PreTranslateMessage(MSG* pMsg)
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


void CSerialDlg::OnBnClickedButtonSend()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_w != nullptr && m_bOpen)
	{
		UpdateData();
		m_send += "\r\n";
		m_w->send(m_send.GetBuffer(m_send.GetLength()), m_send.GetLength());
	}
}


void CSerialDlg::OnBnClickedButtonOpenport()
{
	// TODO: 在此添加控件通知处理程序代码	
	if (!m_bOpen)
	{
		int iCurSel = m_com.GetCurSel();
		if (iCurSel >= 0)
		{
			CString str;
			m_com.GetLBText(m_com.GetCurSel(), str);
			int portNo = str[3] - '0';
			CTetrisPlaceDlg * parentDlg = (CTetrisPlaceDlg*)GetParent();
			if (parentDlg->m_w.open(portNo, BAUDRATE,
				PARITY, DATABIT, STOPBIT, SYNCHRONIZE))
			{
				m_bOpen = TRUE;
				m_w = &parentDlg->m_w;
				GetDlgItem(IDC_BUTTON_OPENPORT)->SetWindowTextW(_T("关闭串口"));
				((CTetrisPlaceDlg*)GetParent())->pArmCtrlDlg->EnableWindow(TRUE);
				if (MessageBox(_T("保留机械臂当前参数？"), _T("打开串口"),
					MB_YESNO) == IDYES)
				{
					parentDlg->pArmCtrlDlg->BindSerialPort(m_w, FALSE);
				}
				else
				{
					parentDlg->pArmCtrlDlg->BindSerialPort(m_w, TRUE);
				}
				hThread = CreateThread(NULL, 0, SerialThreadProc, this, 0, 0);
			}
			else
			{
				MessageBox(_T("串口连接失败"), _T("打开串口"));
			}
		}
		else
		{
			MessageBox(_T("请选择一个串口"), _T("打开串口"));
		}
	}
	else
	{
		m_w->close();
		GetDlgItem(IDC_BUTTON_OPENPORT)->SetWindowTextW(_T("打开串口"));
		((CTetrisPlaceDlg*)GetParent())->pArmCtrlDlg->EnableWindow(FALSE);
		m_bOpen = FALSE;
		TerminateThread(hThread, 0);
	}
}


void CSerialDlg::OnBnClickedButtonRefresh()
{
	// TODO: 在此添加控件通知处理程序代码
	GetConnectedPort();
}

DWORD WINAPI SerialThreadProc(LPVOID lpParam)
{
	CSerialDlg * dlg = (CSerialDlg*)lpParam;
	char buff[1000];
	while (1)
	{
		if (dlg->m_bOpen && dlg->m_w != nullptr)
		{
			int len = dlg->m_w->receive(buff, sizeof(buff) - 2);
			buff[len - 1] = '\n';
			buff[len] = 0;
			CString str(buff);
			dlg->m_receive += str;
			dlg->UpdateReceive();
		}
	}
	return 0;
}
