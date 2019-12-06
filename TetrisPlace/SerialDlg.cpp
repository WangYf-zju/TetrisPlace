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
	DDX_Text(pDX, IDC_EDIT_RX, m_receive);
	DDX_Text(pDX, IDC_EDIT_TX, m_send);
}


BEGIN_MESSAGE_MAP(CSerialDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_SEND, &CSerialDlg::OnBnClickedButtonSend)
END_MESSAGE_MAP()


// CSerialDlg 消息处理程序


BOOL CSerialDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	// TODO:  在此添加额外的初始化
	m_bOpen = TRUE;
	m_w = &((CTetrisPlaceDlg*)GetParent())->m_w;
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}


void CSerialDlg::UpdateReceive()
{
	GetDlgItem(IDC_EDIT_RX)->SetWindowText(m_receive);
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
		m_send += "\n";
		m_w->send(m_send.GetBuffer(m_send.GetLength()), m_send.GetLength());
	}
}



void CSerialDlg::StartListenPort()
{
	//hThread = CreateThread(NULL, 0, SerialThreadProc, this, 0, 0);
}

void CSerialDlg::BindSerialPort(WzSerialPort * w)
{
	m_w = w;
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
			if (len > 0)
			{
				buff[len - 1] = '\n';
				buff[len] = 0;
				CString str(buff);
				dlg->m_receive += str;
				dlg->UpdateReceive();
			}			
		}
	}
	return 0;
}
