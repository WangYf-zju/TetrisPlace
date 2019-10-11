// CSerialDlg.cpp: 实现文件
//

#include "stdafx.h"
#include "TetrisPlace.h"
#include "SerialDlg.h"
#include "afxdialogex.h"

#include "WzSerialPort.h"


// CSerialDlg 对话框

IMPLEMENT_DYNAMIC(CSerialDlg, CDialogEx)

CSerialDlg::CSerialDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_SERIAL, pParent)
{

}

CSerialDlg::~CSerialDlg()
{
}

void CSerialDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_COM, m_com);
}


BEGIN_MESSAGE_MAP(CSerialDlg, CDialogEx)
END_MESSAGE_MAP()


// CSerialDlg 消息处理程序


BOOL CSerialDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	// TODO:  在此添加额外的初始化
	GetConnectedPort();

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}


void CSerialDlg::GetConnectedPort()
{
	// TODO: 在此处添加实现代码.
	WzSerialPort w;
	if (w.open("COM1", 9600, 0, 8, 1))
	{
		w.close();
		m_com.AddString(_T("COM1"));
	}
	if (w.open("COM2", 9600, 0, 8, 1))
	{
		w.close();
		m_com.AddString(_T("COM2"));
	}
	if (w.open("COM3", 9600, 0, 8, 1))
	{
		w.close();
		m_com.AddString(_T("COM3"));
	}
	if (w.open("COM4", 9600, 0, 8, 1))
	{
		w.close();
		m_com.AddString(_T("COM4"));
	}
	if (w.open("COM5", 9600, 0, 8, 1))
	{
		w.close();
		m_com.AddString(_T("COM5"));
	}
	if (w.open("COM6", 9600, 0, 8, 1))
	{
		w.close();
		m_com.AddString(_T("COM6"));
	}
	if (w.open("COM7", 9600, 0, 8, 1))
	{
		w.close();
		m_com.AddString(_T("COM7"));
	}
	if (w.open("COM8", 9600, 0, 8, 1))
	{
		w.close();
		m_com.AddString(_T("COM8"));
	}
	if (w.open("COM9", 9600, 0, 8, 1))
	{
		w.close();
		m_com.AddString(_T("COM9"));
	}
}
