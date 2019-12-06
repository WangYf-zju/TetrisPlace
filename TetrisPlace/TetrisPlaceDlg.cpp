
// TetrisPlaceDlg.cpp: 实现文件
//

#include "stdafx.h"
#include "TetrisPlace.h"
#include "TetrisPlaceDlg.h"
#include "afxdialogex.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define SERIAL_PORT 3


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CTetrisPlaceDlg 对话框

CTetrisPlaceDlg::CTetrisPlaceDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_TETRISPLACE_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_bCreate = FALSE;
}

void CTetrisPlaceDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CTetrisPlaceDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_COMMAND(ID_MENU_STORESET, &CTetrisPlaceDlg::OnMenuStoreset)
	ON_WM_SIZE()
	ON_WM_CLOSE()
	ON_WM_HSCROLL()
	ON_WM_VSCROLL()
	ON_COMMAND(ID_MENU_ARMSET, &CTetrisPlaceDlg::OnMenuArmset)
	ON_STN_DBLCLK(IDC_EMERGENCY, &CTetrisPlaceDlg::OnStnDblclickEmergency)
	ON_COMMAND(ID_MENU_DISCAMERA, &CTetrisPlaceDlg::OnMenuDiscamera)
	ON_COMMAND(ID_MENU_SCREENSHOT, &CTetrisPlaceDlg::OnMenuScreenshot)
END_MESSAGE_MAP()


// CTetrisPlaceDlg 消息处理程序

BOOL CTetrisPlaceDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	ShowWindow(SW_MAXIMIZE);


	pCntDlg = new CConnectDlg;
	pCntDlg->Create(IDD_DIALOG_CONNECT, this);
	pCntDlg->ShowWindow(SW_SHOW);
	pLeftColDlg = new CLeftColDlg;
	pLeftColDlg->Create(IDD_LEFTCOL);
	pLeftColDlg->ShowWindow(SW_SHOW);
	pCameraDlg = new CCameraDlg;
	pCameraDlg->Create(IDD_DIALOG_CAMERA, this);
	pCameraDlg->ShowWindow(SW_SHOW);
	pBoardDlg = new CBoardDlg;
	pBoardDlg->Create(IDD_DIALOG_BOARD, this);
	pBoardDlg->ShowWindow(SW_SHOW);
	pInfoDlg = new CInfoDlg;
	pInfoDlg->Create(IDD_DIALOG_INFO, this);
	pInfoDlg->ShowWindow(SW_SHOW);
	m_bCreate = TRUE;
	UpdateWindowPos();

	SetScrollRange(SB_VERT, 0, 50, FALSE);
	SetScrollRange(SB_HORZ, 0, 50, FALSE);
	SCROLLINFO info;
	info.cbSize = sizeof(SCROLLINFO);
	info.fMask = SIF_PAGE;
	info.nPage = 50;
	SetScrollInfo(SB_VERT, &info, FALSE);
	SetScrollInfo(SB_HORZ, &info, FALSE);
	SetScrollPos(SB_VERT, 0);
	SetScrollPos(SB_HORZ, 0);

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}


void CTetrisPlaceDlg::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	int oper = MessageBox(_T("是否当前设定的保存参数？"), _T("俄罗斯方块"), MB_YESNOCANCEL);
	switch (oper)
	{
	case IDYES:
		// TODO: 
		m_w.close();
		break;
	case IDNO:
		// TODO: 
		m_w.close();
		break;
	case IDCANCEL:
	default:
		return;
	}

	CDialogEx::OnClose();
}


void CTetrisPlaceDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CTetrisPlaceDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CTetrisPlaceDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



BOOL CTetrisPlaceDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN)
		return TRUE;
	else if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_ESCAPE)
		return TRUE;
	return CDialogEx::PreTranslateMessage(pMsg);
}


LRESULT CTetrisPlaceDlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	// TODO: 在此添加专用代码和/或调用基类
	switch (message)
	{
	case USER_WM_PAINTBOARD:
		pBoardDlg->PostMessage(USER_WM_PAINTBOARD);
		break;
	case USER_WM_PAINTTETRIS:
		pBoardDlg->PostMessage(USER_WM_PAINTTETRIS, wParam, lParam);
		break;
	}
	return CDialogEx::WindowProc(message, wParam, lParam);
}



void CTetrisPlaceDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);
	if (m_bCreate)
	{
		UpdateWindowPos();
		int sx = GetSystemMetrics(SM_CXFULLSCREEN);
		int sy = GetSystemMetrics(SM_CYFULLSCREEN);
		
		SCROLLINFO v_info, h_info;
		v_info.cbSize = sizeof(SCROLLINFO);
		v_info.fMask = SIF_PAGE;
		v_info.nPage = 50 * cy / sy;
		SetScrollInfo(SB_VERT, &v_info);
		h_info.cbSize = sizeof(SCROLLINFO);
		h_info.fMask = SIF_PAGE;
		h_info.nPage = 50 * cx / sx;
		SetScrollInfo(SB_HORZ, &h_info);
	}
}


void CTetrisPlaceDlg::OnMenuStoreset()
{
	// TODO: 在此添加命令处理程序代码
	CStoreSetDlg storeSetDlg;
	storeSetDlg.DoModal();
}

void CTetrisPlaceDlg::UpdateWindowPos()
{
	int cx = GetSystemMetrics(SM_CXFULLSCREEN) - GetSystemMetrics(SM_CXVSCROLL);
	int cy = GetSystemMetrics(SM_CYFULLSCREEN) - GetSystemMetrics(SM_CYSIZE)
		- GetSystemMetrics(SM_CYVSCROLL);
	CRect rc(0,0,cx,cy);
	int margin = 10;

	pLeftColDlg->MoveWindow(20, rc.Height()*0.1, rc.Width()*0.27, rc.Height()*0.9);
	pLeftColDlg->UpdateWindowPos();
	pCntDlg->MoveWindow(20, 10, rc.Width() - 40, rc.Height()*0.073);
	pCameraDlg->MoveWindow(rc.Width()*0.29, rc.Height()*0.1,
		rc.Width()*0.45, rc.Height()*0.66);
	pCameraDlg->GetDlgItem(IDC_PICTURE)->MoveWindow(5, 5,
		rc.Width()*0.45 - 12, rc.Height()*0.66 - 12);
	pBoardDlg->MoveWindow(rc.Width()*0.75, rc.Height()*0.1,
		rc.Width()*0.24, rc.Height()*0.66);
	pBoardDlg->InitDlg();
	pInfoDlg->MoveWindow(rc.Width()*0.29, rc.Height()*0.77,
		rc.Width()*0.57, rc.Height()*0.225);
	GetDlgItem(IDC_EMERGENCY)->MoveWindow(rc.Width()*0.87, rc.Height()*0.78, 
		rc.Width()*0.12, rc.Height()*0.21);
}


void CTetrisPlaceDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	SetScrollPos(SB_HORZ, nPos, TRUE);
	CDialogEx::OnHScroll(nSBCode, nPos, pScrollBar);
}


void CTetrisPlaceDlg::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	SetScrollPos(SB_VERT, nPos, TRUE);
	CDialogEx::OnVScroll(nSBCode, nPos, pScrollBar);
}


void CTetrisPlaceDlg::OnStnDblclickEmergency()
{
	// TODO: 在此添加控件通知处理程序代码
	pLeftColDlg->pArmCtrlDlg->m_pA->EmgerencyStop();
	MessageBox(_T("机械臂以紧急停止，请重新连接"));
}


void CTetrisPlaceDlg::OnMenuArmset()
{
	CArmSetDlg ArmSetDlg;
	ArmSetDlg.DoModal();
}


void CTetrisPlaceDlg::OnMenuDiscamera()
{
	CConnectDlg::instance->DisconnectCamera();
}


void CTetrisPlaceDlg::OnMenuScreenshot()
{
	CCameraDlg::instance->SaveImage();
}
