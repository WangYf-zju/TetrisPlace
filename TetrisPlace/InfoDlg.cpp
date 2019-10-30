// InfoDlg.cpp: 实现文件
//

#include "stdafx.h"
#include "TetrisPlace.h"
#include "InfoDlg.h"
#include "afxdialogex.h"


// CInfoDlg 对话框
HWND CInfoDlg::hInfoDlg = 0;

IMPLEMENT_DYNAMIC(CInfoDlg, CDialogEx)

CInfoDlg::CInfoDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_INFO, pParent)
{

}

CInfoDlg::~CInfoDlg()
{
}

void CInfoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_RANK, m_listRank);
}


BEGIN_MESSAGE_MAP(CInfoDlg, CDialogEx)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_RANK, &CInfoDlg::OnLvnItemchangedListRank)
	ON_WM_PAINT()
END_MESSAGE_MAP()


// CInfoDlg 消息处理程序


BOOL CInfoDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	hInfoDlg = this->m_hWnd;
	// TODO:  在此添加额外的初始化
	m_listRank.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	m_listRank.InsertColumn(0, _T("序号"), LVCFMT_LEFT, 40);
	m_listRank.InsertColumn(1, _T("数量"), LVCFMT_LEFT, 40);
	m_listRank.InsertColumn(2, _T("分数"), LVCFMT_LEFT, 40);
	m_listRank.InsertColumn(3, _T("位置(x,y)"), LVCFMT_LEFT, 80);
	m_listRank.InsertColumn(4, _T("放置(x,y)"), LVCFMT_LEFT, 80);
	m_listRank.InsertColumn(5, _T("实际位置(x,y)"), LVCFMT_LEFT, 100);
	m_listRank.InsertColumn(6, _T("实际放置(x,y)"), LVCFMT_LEFT, 100);
	m_listRank.InsertColumn(7, _T("行程(x,y,r)"), LVCFMT_LEFT, 100);
	m_listRank.InsertItem(0, _T("1"));
	m_listRank.InsertItem(1, _T("2"));
	m_listRank.InsertItem(2, _T("3"));
	m_listRank.InsertItem(3, _T("4"));
	m_listRank.InsertItem(4, _T("5"));
	m_listRank.InsertItem(5, _T("6"));
	m_listRank.InsertItem(6, _T("7"));
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}


BOOL CInfoDlg::PreTranslateMessage(MSG* pMsg)
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


LRESULT CInfoDlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	// TODO: 在此添加专用代码和/或调用基类
	switch (message)
	{
	case USER_WM_UPDATEINFO:
		UpdateInfo((int)wParam, (TypeInfo*)lParam);
		break;
	default:
		break;
	}
	return CDialogEx::WindowProc(message, wParam, lParam);
}


void CInfoDlg::UpdateInfo(int supreme, TypeInfo * info)
{
	if (info != nullptr)
	{
		for (int i = 0; i < TYPE_COUNT; i++)
		{
			CString str;
			str.Format(_T("%d", info[i].count));
			m_listRank.SetItemText(i, 1, str);
			if (info[i].count > 0)
			{
				str.Format(_T("%d", info[i].rank));
				m_listRank.SetItemText(i, 2, str);
				str.Format("(%d,%d)", info[i].grid_x, info[i].grid_y);
				m_listRank.SetItemText(i, 3, str);
				str.Format("(%d,%d)", info[i].grid_toX, info[i].grid_toY);
				m_listRank.SetItemText(i, 4, str);
				str.Format("(%.1f,%.1f)", info[i].x, info[i].y);
				m_listRank.SetItemText(i, 5, str);
				str.Format("(%.1f,%.1f)", info[i].toX, info[i].toY);
				m_listRank.SetItemText(i, 6, str);
			}
			else
			{
				m_listRank.SetItemText(i, 2, "");
				m_listRank.SetItemText(i, 3, "");
				m_listRank.SetItemText(i, 4, "");
				m_listRank.SetItemText(i, 5, "");
				m_listRank.SetItemText(i, 6, "");
				m_listRank.SetItemText(i, 7, "");
			}
		}
		m_listRank.SetCheck(supreme);
	}
}


void CInfoDlg::OnLvnItemchangedListRank(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	if (pNMLV->uChanged == LVIF_STATE && pNMLV->uNewState & LVIS_SELECTED)
	{
		Invalidate();
	}
	*pResult = 0;
}


void CInfoDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
					   // TODO: 在此处添加消息处理程序代码
					   // 不为绘图消息调用 CDialogEx::OnPaint()
	CRect cRC;
	GetClientRect(&cRC);
	POSITION p = m_listRank.GetFirstSelectedItemPosition();
	int type = m_listRank.GetNextSelectedItem(p);
	if (type >= 0 && type < 7)
	{
		int sideLength = cRC.Width() * 0.15 / 4;
		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				if (TetrisAI::TetrisShape[type][0][j * 4 + i])
				{
					CRect rc(cRC.Width() * 0.84 + i * sideLength + 1,
						cRC.Height() * 0.3 + j * sideLength + 1,
						cRC.Width() * 0.84 + (i + 1) * sideLength - 2,
						cRC.Height() * 0.3 + (j + 1) * sideLength - 2);
					dc.FillSolidRect(&rc, RGB(192, 192, 192));
				}
			}
		}
	}
}
