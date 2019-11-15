// BoardDlg.cpp: 实现文件
//

#include "stdafx.h"
#include "TetrisPlace.h"
#include "afxdialogex.h"
#include "TetrisAI.h"
#include "BoardDlg.h"
#include "NextBlockDlg.h"

// CBoardDlg 对话框

IMPLEMENT_DYNAMIC(CBoardDlg, CDialogEx)

HWND CBoardDlg::hBoardDlg = 0;
CBoardDlg::CBoardDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_BOARD, pParent)
{

}

CBoardDlg::~CBoardDlg()
{
}

void CBoardDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CBoardDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_BN_CLICKED(IDC_BUTTON_MODIFY, &CBoardDlg::OnBnClickedButtonModify)
	ON_BN_CLICKED(IDC_BUTTON_REFRESHBOARD, &CBoardDlg::OnBnClickedButtonRefreshboard)
	ON_BN_CLICKED(IDC_BUTTON_ADDTETRIS, &CBoardDlg::OnBnClickedButtonAddtetris)
END_MESSAGE_MAP()


// CBoardDlg 消息处理程序


void CBoardDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
					   // TODO: 在此处添加消息处理程序代码
					   // 不为绘图消息调用 CDialogEx::OnPaint()
	CRect rc;
	GetClientRect(&rc);
	CDC *pDC = GetDC();
	pDC->BitBlt(0, 0, rc.Width(), rc.Height(), &m_memDC, 0, 0, SRCCOPY);
}


BOOL CBoardDlg::PreTranslateMessage(MSG* pMsg)
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


LRESULT CBoardDlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	// TODO: 在此添加专用代码和/或调用基类
	switch (message)
	{
	case USER_WM_PAINTBOARD:
		DrawClient();
		Invalidate(FALSE);
		break;
	case USER_WM_PAINTTETRIS:
		DrawClient();
		DrawTetris(&m_memDC, (int)wParam, (Position*)lParam);
		Invalidate(FALSE);
		break;
	}
	return CDialogEx::WindowProc(message, wParam, lParam);
}


void CBoardDlg::DrawBorder(CDC *pDC)
{
	CPen pen(PS_SOLID, 2, RGB(204, 204, 204));
	CPen *pOldPen = pDC->SelectObject(&pen);
	for (int i = 0; i < 11; i++)
	{
		pDC->MoveTo(10 + m_sideLength * i, 10);
		pDC->LineTo(10 + m_sideLength * i, 10 + ROW * m_sideLength);
	}
	for (int j = 0; j < 13; j++)
	{
		pDC->MoveTo(10, 10 + m_sideLength * j);
		pDC->LineTo(10 + COL * m_sideLength, 10 + m_sideLength * j);
	}
	pDC->SelectObject(pOldPen);
	pen.DeleteObject();
}


void CBoardDlg::DrawBoard(CDC *pDC)
{
	for (int x = 0; x < COL; x++)
	{
		for (int y = 0; y < ROW; y++)
		{
			if ((*TetrisAI::m_pBoard)[y][x])
			{
				DrawBlock(pDC, x, y, RGB(180, 180, 180));
			}
		}
	}
}

void CBoardDlg::DrawBlock(CDC *pDC, int x, int y, COLORREF rgb)
{
	CRect rc(12 + m_sideLength * x, 12 + m_sideLength * y,
		8 + m_sideLength * (x + 1), 8 + m_sideLength * (y + 1));
	pDC->FillSolidRect(&rc, rgb);
}


void CBoardDlg::DrawTetris(CDC *pDC, int type, int x, int y, int r)
{
	if (pDC == nullptr)
		pDC = GetDC();
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			if (TetrisAI::TetrisShape[type][r][j * 4 + i])
			{
				DrawBlock(pDC, x + i, y + j, RGB(0, 255, 0));
			}
		}
	}
}

void CBoardDlg::DrawTetris(CDC *pDC, int type, Position * pos)
{
	DrawTetris(pDC, type, pos->x, pos->y, pos->r);
}

void CBoardDlg::DrawClient()
{
	CRect rc;
	GetClientRect(&rc);
	m_memDC.FillSolidRect(&rc, RGB(255, 255, 255));

	DrawBorder(&m_memDC);
	DrawBoard(&m_memDC);
}


BOOL CBoardDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_bModify = FALSE;
	hBoardDlg = this->m_hWnd;
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}

void CBoardDlg::InitDlg()
{
	CRect rc;
	GetClientRect(&rc);
	m_sideLength = (rc.Width() - 20) / COL;
	m_memDC.DeleteDC();
	m_memDC.CreateCompatibleDC(this->GetDC());
	m_memBmp.DeleteObject();
	m_memBmp.CreateCompatibleBitmap(GetDC(), rc.Width(), rc.Height());
	m_memDC.SelectObject(&m_memBmp);
	DrawClient();
	Invalidate(FALSE);
}


void CBoardDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (m_bModify)
	{
		int x = GetGridX(&point);
		int y = GetGridY(&point);
		TetrisAI::ReverseBlock(x, y);
		DrawClient();
		Invalidate(FALSE);
	}
	CDialogEx::OnLButtonDown(nFlags, point);
}

int CBoardDlg::GetGridX(CPoint * pt)
{
	return GetGridX((double)pt->x);
}

int CBoardDlg::GetGridX(double x)
{
	return (int)((x - 5) / m_sideLength);
}

int CBoardDlg::GetGridY(CPoint * pt)
{
	return GetGridY((double)pt->y);
}

int CBoardDlg::GetGridY(double y)
{
	return (int)((y - 5) / m_sideLength);
}


void CBoardDlg::OnBnClickedButtonModify()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_bModify)
	{
		m_bModify = FALSE;
		GetDlgItem(IDC_BUTTON_MODIFY)->SetWindowText(_T("修改盘面"));
		GetDlgItem(IDC_BUTTON_REFRESHBOARD)->SetWindowTextA(_T("刷新"));
	}
	else
	{
		m_bModify = TRUE;
		GetDlgItem(IDC_BUTTON_MODIFY)->SetWindowText(_T("确定"));
		GetDlgItem(IDC_BUTTON_REFRESHBOARD)->SetWindowTextA(_T("清空"));
	}
}


void CBoardDlg::OnBnClickedButtonRefreshboard()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_bModify)
	{
		TetrisAI::ClearBoard();
	}
	DrawClient();
	Invalidate(FALSE);
}


void CBoardDlg::OnBnClickedButtonAddtetris()
{
	// TODO: 在此添加控件通知处理程序代码
	CNextBlockDlg NextDlg;
	NextDlg.DoModal();
}
