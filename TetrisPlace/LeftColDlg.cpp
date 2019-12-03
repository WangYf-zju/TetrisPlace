// LeftColDlg.cpp: 实现文件
//

#include "stdafx.h"
#include "TetrisPlace.h"
#include "LeftColDlg.h"
#include "afxdialogex.h"


// CLeftColDlg 对话框

IMPLEMENT_DYNAMIC(CLeftColDlg, CDialogEx)

CLeftColDlg::CLeftColDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_LEFTCOL, pParent)
{

}

CLeftColDlg::~CLeftColDlg()
{
}

void CLeftColDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB, m_tab);
}


BEGIN_MESSAGE_MAP(CLeftColDlg, CDialogEx)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB, &CLeftColDlg::OnTcnSelchangeTab)
END_MESSAGE_MAP()


// CLeftColDlg 消息处理程序


BOOL CLeftColDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_tab.InsertItem(0, _T("电机控制"));
	m_tab.InsertItem(1, _T("串口"));	
	m_tab.InsertItem(2, _T("HTTP服务"));
	pArmCtrlDlg = new CArmControlDlg;
	pArmCtrlDlg->Create(IDD_DIALOG_ARMCONTROL, &m_tab);
	pArmCtrlDlg->ShowWindow(SW_SHOW);
	pSerialDlg = new CSerialDlg;
	pSerialDlg->Create(IDD_DIALOG_SERIAL, &m_tab);
	pSerialDlg->ShowWindow(SW_HIDE);
	pServerDlg = new CServerDlg;
	pServerDlg->Create(IDD_DIALOG_HTTP, &m_tab);
	pServerDlg->ShowWindow(SW_HIDE);
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}


BOOL CLeftColDlg::PreTranslateMessage(MSG* pMsg)
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


void CLeftColDlg::OnTcnSelchangeTab(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 在此添加控件通知处理程序代码
	int curPage = m_tab.GetCurSel();
	if (curPage == 0)
	{
		pArmCtrlDlg->ShowWindow(SW_SHOW);
		pSerialDlg->ShowWindow(SW_HIDE);
		pServerDlg->ShowWindow(SW_HIDE);
	}
	else if(curPage == 1)
	{
		pArmCtrlDlg->ShowWindow(SW_HIDE);
		pSerialDlg->ShowWindow(SW_SHOW);
		pServerDlg->ShowWindow(SW_HIDE);
	}
	else if (curPage == 2)
	{
		pArmCtrlDlg->ShowWindow(SW_HIDE);
		pSerialDlg->ShowWindow(SW_HIDE);
		pServerDlg->ShowWindow(SW_SHOW);
	}
	*pResult = 0;
}

void CLeftColDlg::UpdateWindowPos()
{
	int cx = GetSystemMetrics(SM_CXFULLSCREEN) - GetSystemMetrics(SM_CXVSCROLL);
	int cy = GetSystemMetrics(SM_CYFULLSCREEN) - GetSystemMetrics(SM_CYSIZE)
		- GetSystemMetrics(SM_CYVSCROLL);
	CRect rc(0, 0, cx, cy);
	int margin = 10;

	m_tab.MoveWindow(0, 0, rc.Width()*0.27, rc.Height()*0.9);
	pArmCtrlDlg->MoveWindow(10, 20, rc.Width()*0.27 - 20, rc.Height()*0.9 - 30);
	pSerialDlg->MoveWindow(10, 20, rc.Width()*0.27 - 20, rc.Height()*0.9 - 30);
	pServerDlg->MoveWindow(10, 20, rc.Width()*0.27 - 20, rc.Height()*0.9 - 30);
}
