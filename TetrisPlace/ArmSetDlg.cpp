// ArmSetDlg.cpp: 实现文件
//

#include "stdafx.h"
#include "TetrisPlace.h"
#include "ArmSetDlg.h"
#include "afxdialogex.h"


// CArmSetDlg 对话框

IMPLEMENT_DYNAMIC(CArmSetDlg, CDialogEx)

CArmSetDlg::CArmSetDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_ARMSETTING, pParent)
	, m_startX(0)
	, m_startY(0)
	, m_startZ(0)
{

}

CArmSetDlg::~CArmSetDlg()
{
}

void CArmSetDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_ARM_STARTX, m_startX);
	DDX_Text(pDX, IDC_ARM_STARTY, m_startY);
	DDX_Text(pDX, IDC_ARM_STARTZ, m_startZ);
}


BEGIN_MESSAGE_MAP(CArmSetDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CArmSetDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CArmSetDlg::OnBnClickedCancel)
END_MESSAGE_MAP()


// CArmSetDlg 消息处理程序


BOOL CArmSetDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_startX = CParameter::startX;
	m_startY = CParameter::startY;
	m_startZ = CParameter::startZ;
	UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}


BOOL CArmSetDlg::PreTranslateMessage(MSG* pMsg)
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


void CArmSetDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData();
	CParameter::startX = m_startX;
	CParameter::startY = m_startY;
	CParameter::startZ = m_startZ;

	CDialogEx::OnOK();
}


void CArmSetDlg::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	CDialogEx::OnCancel();
}
