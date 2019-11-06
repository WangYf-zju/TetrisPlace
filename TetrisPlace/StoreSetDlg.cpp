// CStoreSetDlg.cpp: 实现文件
//

#include "stdafx.h"
#include "TetrisPlace.h"
#include "StoreSetDlg.h"
#include "afxdialogex.h"


// CStoreSetDlg 对话框

IMPLEMENT_DYNAMIC(CStoreSetDlg, CDialogEx)

CStoreSetDlg::CStoreSetDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_STORESETTING, pParent)
{

}

CStoreSetDlg::~CStoreSetDlg()
{
}

void CStoreSetDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CStoreSetDlg, CDialogEx)
END_MESSAGE_MAP()


// CStoreSetDlg 消息处理程序

BOOL CStoreSetDlg::PreTranslateMessage(MSG* pMsg)
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


BOOL CStoreSetDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	bIsometryMode = TRUE;
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}

void CStoreSetDlg::IsometryMode()
{
	GetDlgItem(IDC_STORE_ROWDIS)->EnableWindow(TRUE);
	GetDlgItem(IDC_STORE_COLDIS)->EnableWindow(TRUE);
	GetDlgItem(IDC_STORE_X)->EnableWindow(TRUE);
	GetDlgItem(IDC_STORE_Y)->EnableWindow(TRUE);
}
