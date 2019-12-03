// ServerDlg.cpp: 实现文件
//

#include "stdafx.h"
#include "TetrisPlace.h"
#include "ServerDlg.h"
#include "afxdialogex.h"


// CServerDlg 对话框

extern mutex cmrimgLock;

IMPLEMENT_DYNAMIC(CServerDlg, CDialogEx)

CServerDlg::CServerDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_HTTP, pParent)
{

}

CServerDlg::~CServerDlg()
{
}

void CServerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CServerDlg, CDialogEx)
	ON_BN_CLICKED(IDC_OPENHTTP, &CServerDlg::OnBnClickedOpenhttp)
END_MESSAGE_MAP()


// CServerDlg 消息处理程序


BOOL CServerDlg::PreTranslateMessage(MSG* pMsg)
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


BOOL CServerDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_bStart = FALSE;
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}

void CServerDlg::ev_handler(struct mg_connection * nc, int ev, void * ev_data)
{
	struct http_message *hm = (struct http_message *) ev_data;

	switch (ev) {
	case MG_EV_HTTP_REQUEST:
		if (mg_vcmp(&hm->uri, "/favicon.ico") == 0)
		{
			mg_http_serve_file(nc, hm, "./http/favicon.ico", mg_mk_str("x-icon"), mg_mk_str(""));
		}
		else if (mg_vcmp(&hm->uri, "/res/dropdown.png") == 0)
		{
			mg_http_serve_file(nc, hm, "./http/res/dropdown.png", mg_mk_str("x-icon"), mg_mk_str(""));
		}
		else if (mg_vcmp(&hm->uri, "/res/cmr.jpg") == 0)
		{
			cmrimgLock.lock();
			mg_http_serve_file(nc, hm, "./temp/cmr-temp.jpg", mg_mk_str("x-icon"), mg_mk_str(""));
			cmrimgLock.unlock();
		}
		else if (mg_vcmp(&hm->uri, "/") == 0 || 
			mg_vcasecmp(&hm->uri, "/index.html") == 0)
		{
			mg_http_serve_file(nc, hm, "./http/index.html", mg_mk_str("text/html"), mg_mk_str(""));
		}
		
		break;
	default:
		break;
	}
}


void CServerDlg::StartServer()
{
	m_bStart = TRUE;
	hThread = CreateThread(NULL, 0, ServerThreadProc, this, 0, 0);
}

void CServerDlg::CloseServer()
{
	m_bStart = FALSE;
	CloseHandle(hThread);
}

DWORD WINAPI ServerThreadProc(LPVOID lpParam)
{
	CServerDlg * dlg = (CServerDlg*)lpParam;
	int i;
	char *cp;
	const char *err_str;
#if MG_ENABLE_SSL
	const char *ssl_cert = NULL;
#endif
	mg_mgr_init(&(dlg->mgr), NULL);

	/* Set document root directory */
	dlg->s_http_server_opts.document_root = ".";

	/* Set HTTP server options */
	memset(&(dlg->bind_opts), 0, sizeof(dlg->bind_opts));
	dlg->bind_opts.error_string = &err_str;
#if MG_ENABLE_SSL
	if (ssl_cert != NULL) {
		bind_opts.ssl_cert = ssl_cert;
	}
#endif
	dlg->nc = mg_bind_opt(&(dlg->mgr), HTTP_PORT, &(CServerDlg::ev_handler), dlg->bind_opts);
	if (dlg->nc == NULL) {
		CString str;
		str.Format("Error starting server on port %s: %s\n", HTTP_PORT,
			*(dlg->bind_opts).error_string);
		MessageBox(dlg->m_hWnd, str, "Error", MB_OK);
		return 1;
	}

	mg_set_protocol_http_websocket(dlg->nc);
	dlg->s_http_server_opts.enable_directory_listing = "yes";

	while (dlg->m_bStart)
	{
		mg_mgr_poll(&(dlg->mgr), 1000);
		Sleep(10);
	}
	mg_mgr_free(&(dlg->mgr));
	return 0;
}


void CServerDlg::OnBnClickedOpenhttp()
{
	// TODO: 在此添加控件通知处理程序代码
	if (!m_bStart)
	{
		StartServer();
		GetDlgItem(IDC_OPENHTTP)->SetWindowText(_T("关闭服务"));
	}
	else
	{
		CloseServer();
		GetDlgItem(IDC_OPENHTTP)->SetWindowText(_T("开启服务"));
	}
}
