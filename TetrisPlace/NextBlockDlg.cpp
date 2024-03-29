﻿// NextBlockDlg.cpp: 实现文件
//

#include "stdafx.h"
#include "TetrisPlace.h"
#include "NextBlockDlg.h"
#include "afxdialogex.h"
#include "TetrisAI.h"
#include "TetrisPlaceDlg.h"


// CNextBlockDlg 对话框

IMPLEMENT_DYNAMIC(CNextBlockDlg, CDialogEx)

CNextBlockDlg::CNextBlockDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_NEXTBLOCK, pParent)
	, m_type(0)
{

}

CNextBlockDlg::~CNextBlockDlg()
{
}

void CNextBlockDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	//  DDX_Text(pDX, IDC_EDIT_NEXT, m_type);
	//  DDV_MinMaxInt(pDX, m_type, 0, 6);
	DDX_Control(pDX, IDC_COMBO_TETRIS, m_comTetris);
}


BEGIN_MESSAGE_MAP(CNextBlockDlg, CDialogEx)
	ON_WM_PAINT()
//	ON_BN_CLICKED(IDC_BUTTON_NEXTOK, &CNextBlockDlg::OnBnClickedButtonNextok)
	ON_BN_CLICKED(IDC_BUTTON_PLACE, &CNextBlockDlg::OnBnClickedButtonPlace)
	ON_CBN_SELCHANGE(IDC_COMBO_TETRIS, &CNextBlockDlg::OnCbnSelchangeComboTetris)
END_MESSAGE_MAP()


// CNextBlockDlg 消息处理程序


BOOL CNextBlockDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_comTetris.AddString(_T("1"));
	m_comTetris.AddString(_T("2"));
	m_comTetris.AddString(_T("3"));
	m_comTetris.AddString(_T("4"));
	m_comTetris.AddString(_T("5"));
	m_comTetris.AddString(_T("6"));
	m_comTetris.AddString(_T("7"));
	m_comTetris.SetCurSel(0);
	m_type = 0;
	m_nextTR = nullptr;
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}


void CNextBlockDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
					   // TODO: 在此处添加消息处理程序代码
					   // 不为绘图消息调用 CDialogEx::OnPaint()
	DrawNext(m_type, &dc);
}


BOOL CNextBlockDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN)
	{
		OnBnClickedButtonPlace();
		return TRUE;
	}
	else if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_ESCAPE)
	{
		return TRUE;
	}
	return CDialogEx::PreTranslateMessage(pMsg);
}


void CNextBlockDlg::OnBnClickedButtonPlace()
{
	// TODO: 在此添加控件通知处理程序代码
	delete m_nextTR;
	m_nextTR = new TetrisAI(m_type);
	Position * pos = m_nextTR->PlaceToSupremePos();
	if (pos != nullptr)
	{
		::PostMessage(CBoardDlg::hBoardDlg, USER_WM_PAINTTETRIS,
			(WPARAM)m_type, (LPARAM)pos);
	}
}

void CNextBlockDlg::DrawNext(int type, CDC *pDC)
{

	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			if (TetrisAI::TetrisShape[type][0][j * 4 + i])
			{
				CRect rc(80 + i * 30, 150 + j * 30, 108 + i * 30, 178 + j * 30);
				pDC->FillSolidRect(&rc, RGB(0, 100, 50));
			}
		}
	}
}

//void CNextBlockDlg::OnBnClickedButtonNextok()
//{
//	// TODO: 在此添加控件通知处理程序代码
//	UpdateData();
//	if (m_type >= 0 && m_type < 7)
//	{
//		Invalidate();
//	}
//}


void CNextBlockDlg::OnCbnSelchangeComboTetris()
{
	// TODO: 在此添加控件通知处理程序代码
	m_type = m_comTetris.GetCurSel();
	if (m_type >= 0 && m_type <= 6)
	{
		Invalidate();
	}
}
