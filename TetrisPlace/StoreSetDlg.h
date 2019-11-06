#pragma once


// CStoreSetDlg 对话框

class CStoreSetDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CStoreSetDlg)

public:
	CStoreSetDlg(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CStoreSetDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_STORESETTING };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL OnInitDialog();
private:
	BOOL bIsometryMode;
	void IsometryMode();
	
	DECLARE_MESSAGE_MAP()
};
