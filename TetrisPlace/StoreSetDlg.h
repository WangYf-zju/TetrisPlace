#pragma once

// CStoreSetDlg 对话框
#include "CameraDlg.h"

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

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL OnInitDialog();
private:
	void IsometryMode();
	void FreeMode();
	void Default();
	void SaveData();
	void UpdateDisplay();
	void CalcByIso();
public:
	static BOOL bIsometryMode;
	afx_msg void OnBnClickedRadioStore1();
	afx_msg void OnBnClickedRadioStore2();
	LPARAM * m_boundary;
private:
	int m_storeRowDis;
	int m_storeColDis;
	int m_storeStartX;
	int m_storeStartY;
	int m_storeX[STORE_COL_BOUNDARY_COUNT];
	int m_storeY[STORE_ROW_BOUNDARY_COUNT];

public:
	afx_msg void OnDestroy();
	afx_msg void OnEnKillfocusStoreRowdis();
	afx_msg void OnEnKillfocusStoreColdis();
	afx_msg void OnEnKillfocusStoreX();
	afx_msg void OnKillfocusStoreY();
	afx_msg void OnKillfocusStoreX0();
	afx_msg void OnKillfocusStoreX1();
	afx_msg void OnKillfocusStoreX2();
	afx_msg void OnKillfocusStoreX3();
	afx_msg void OnKillfocusStoreX4();
	afx_msg void OnKillfocusStoreX5();
	afx_msg void OnKillfocusStoreX6();
	afx_msg void OnKillfocusStoreX7();
	afx_msg void OnKillfocusStoreX8();
	afx_msg void OnKillfocusStoreX9();
	afx_msg void OnKillfocusStoreX10();
	afx_msg void OnKillfocusStoreX11();
	afx_msg void OnKillfocusStoreX12();
	afx_msg void OnKillfocusStoreY0();
	afx_msg void OnKillfocusStoreY1();
	afx_msg void OnKillfocusStoreY2();
	afx_msg void OnKillfocusStoreY3();
	afx_msg void OnKillfocusStoreY4();
	afx_msg void OnKillfocusStoreY5();
	afx_msg void OnKillfocusStoreY6();
	afx_msg void OnKillfocusStoreY7();
	afx_msg void OnKillfocusStoreY8();
	afx_msg void OnKillfocusStoreY9();
	afx_msg void OnKillfocusStoreY10();
};
