
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
}

void CTetrisPlaceDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB_SERIAL, m_tab);
}

BEGIN_MESSAGE_MAP(CTetrisPlaceDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB_SERIAL, &CTetrisPlaceDlg::OnTcnSelchangeTabSerial)
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
	CRect cRC;
	GetClientRect(&cRC);
	int margin = 10;

	m_tab.InsertItem(0, _T("电机控制"));
	m_tab.InsertItem(1, _T("串口"));
	m_tab.MoveWindow(20, cRC.Height()*0.1, cRC.Width()*0.27, cRC.Height()*0.9);
	pArmCtrlDlg = new CArmControlDlg;
	pArmCtrlDlg->Create(IDD_DIALOG_ARMCONTROL, &m_tab);
	pArmCtrlDlg->MoveWindow(10, 20, cRC.Width()*0.27 - 20, cRC.Height()*0.9 - 30);
	pArmCtrlDlg->ShowWindow(SW_SHOW);
	pSerialDlg = new CSerialDlg;
	pSerialDlg->Create(IDD_DIALOG_SERIAL, &m_tab);
	pSerialDlg->MoveWindow(10, 20, cRC.Width()*0.27 - 20, cRC.Height()*0.9 - 30);
	pSerialDlg->ShowWindow(SW_HIDE);
	pCntDlg = new CConnectDlg;
	pCntDlg->Create(IDD_DIALOG_CONNECT, this);
	pCntDlg->MoveWindow(20, 10, cRC.Width() - 40, cRC.Height()*0.073);
	pCntDlg->ShowWindow(SW_SHOW);
	pCameraDlg = new CCameraDlg;
	pCameraDlg->Create(IDD_DIALOG_CAMERA, this);
	pCameraDlg->MoveWindow(cRC.Width()*0.29, cRC.Height()*0.1, 
		cRC.Width()*0.45, cRC.Height()*0.66);
	pCameraDlg->GetDlgItem(IDC_PICTURE)->MoveWindow(5, 5,
		cRC.Width()*0.45 - 12, cRC.Height()*0.66 - 12);
	pCameraDlg->ShowWindow(SW_SHOW);
	pBoardDlg = new CBoardDlg;
	pBoardDlg->Create(IDD_DIALOG_BOARD, this);
	pBoardDlg->MoveWindow(cRC.Width()*0.75, cRC.Height()*0.1, 
		cRC.Width()*0.24, cRC.Height()*0.66);
	pBoardDlg->InitDlg();
	pBoardDlg->ShowWindow(SW_SHOW);
	pInfoDlg = new CInfoDlg;
	pInfoDlg->Create(IDD_DIALOG_INFO, this);
	pInfoDlg->MoveWindow(cRC.Width()*0.29, cRC.Height()*0.77,
		cRC.Width()*0.7, cRC.Height()*0.225);
	pInfoDlg->ShowWindow(SW_SHOW);

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
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


void CTetrisPlaceDlg::OnTcnSelchangeTabSerial(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 在此添加控件通知处理程序代码
	int curPage = m_tab.GetCurSel();
	if (curPage == 0)
	{
		pArmCtrlDlg->ShowWindow(SW_SHOW);
		pSerialDlg->ShowWindow(SW_HIDE);
	}
	else
	{
		pArmCtrlDlg->ShowWindow(SW_HIDE);
		pSerialDlg->ShowWindow(SW_SHOW);
	}
	*pResult = 0;
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
