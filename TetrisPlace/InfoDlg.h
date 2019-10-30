#pragma once
#include "Message.h"
#include "CameraDlg.h"
#include "TetrisAI.h"

// CInfoDlg 对话框

class CInfoDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CInfoDlg)

public:
	CInfoDlg(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CInfoDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_INFO };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	CListCtrl m_listRank;
	static HWND hInfoDlg;
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	void UpdateInfo(int supreme, TypeInfo * info);
	afx_msg void OnLvnItemchangedListRank(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnPaint();
};
