// BoardDlg.cpp: 实现文件
//

#include "stdafx.h"
#include "TetrisPlace.h"
#include "BoardDlg.h"
#include "afxdialogex.h"
#include "TetrisAI.h"


// CBoardDlg 对话框

IMPLEMENT_DYNAMIC(CBoardDlg, CDialogEx)

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

void CBoardDlg::DrawBorder(CDC *pDC)
{
	CPen pen(PS_SOLID, 2, RGB(204, 204, 204));
	CPen *pOldPen = pDC->SelectObject(&pen);
	for (int i = 0; i < 11; i++)
	{
		pDC->MoveTo(10 + 30 * i, 10);
		pDC->LineTo(10 + 30 * i, 370);
	}
	for (int j = 0; j < 13; j++)
	{
		pDC->MoveTo(10, 10 + 30 * j);
		pDC->LineTo(310, 10 + 30 * j);
	}
	pDC->SelectObject(pOldPen);
	pen.DeleteObject();
}


void CBoardDlg::DrawBoard(CDC *pDC)
{
	for (int x = 0; x < COL; x++)
	{
		for (int y = 0; y < RAW; y++)
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
	CRect rc(12 + 30 * x, 12 + 30 * y, 38 + 30 * x, 38 + 30 * y);
	pDC->FillSolidRect(&rc, rgb);
}


void CBoardDlg::DrawTetris(CDC *pDC, int type, int x, int y, int r)
{
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
	CRect rc;
	GetClientRect(&rc);
	m_memDC.CreateCompatibleDC(this->GetDC());
	m_memBmp.CreateCompatibleBitmap(GetDC(), rc.Width(), rc.Height());
	m_memDC.SelectObject(&m_memBmp);
	DrawClient();
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}
