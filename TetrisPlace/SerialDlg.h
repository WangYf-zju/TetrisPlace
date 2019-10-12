#pragma once


// CSerialDlg 对话框

class CSerialDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CSerialDlg)

public:
	CSerialDlg(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CSerialDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_SERIAL };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CComboBox m_com;
private:
	void GetConnectedPort();
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
