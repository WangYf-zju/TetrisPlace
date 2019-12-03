#pragma once
#include "third-party/mongoose.h"
#include <mutex>

#include "ConnectDlg.h"

using std::mutex;

#define HTTP_PORT "3000"


// CServerDlg 对话框

class CServerDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CServerDlg)

public:
	CServerDlg(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CServerDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_HTTP };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	HANDLE hThread;
	BOOL m_bStart;
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL OnInitDialog();
	static void ev_handler(struct mg_connection *nc, int ev, void *ev_data);
	void StartServer();
	void CloseServer();

public:
	struct mg_serve_http_opts s_http_server_opts;
	struct mg_mgr mgr;
	struct mg_connection *nc;
	struct mg_bind_opts bind_opts;
	afx_msg void OnBnClickedOpenhttp();
	int m_port;
	CString m_serverPrint;
};

DWORD WINAPI ServerThreadProc(LPVOID lpParam);
