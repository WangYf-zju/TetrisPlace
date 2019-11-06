// CStoreSetDlg.cpp: 实现文件
//

#include "stdafx.h"
#include "TetrisPlace.h"
#include "StoreSetDlg.h"
#include "afxdialogex.h"


// CStoreSetDlg 对话框
BOOL CStoreSetDlg::bIsometryMode = TRUE;

IMPLEMENT_DYNAMIC(CStoreSetDlg, CDialogEx)

const int ID_X[] = {
	IDC_STORE_X0,IDC_STORE_X1,IDC_STORE_X2,IDC_STORE_X3,IDC_STORE_X4,IDC_STORE_X5,
	IDC_STORE_X6,IDC_STORE_X7,IDC_STORE_X8,IDC_STORE_X9,IDC_STORE_X10,IDC_STORE_X11,
	IDC_STORE_X12
};
const int ID_Y[] = {
	IDC_STORE_Y0,IDC_STORE_Y1,IDC_STORE_Y2,IDC_STORE_Y3,IDC_STORE_Y4,IDC_STORE_Y5,
	IDC_STORE_Y6,IDC_STORE_Y7,IDC_STORE_Y8,IDC_STORE_Y9,IDC_STORE_Y10
};

CStoreSetDlg::CStoreSetDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_STORESETTING, pParent)
	, m_storeRowDis(0)
	, m_storeColDis(0)
	, m_storeStartX(0)
	, m_storeStartY(0)
{

}

CStoreSetDlg::~CStoreSetDlg()
{
}

void CStoreSetDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_STORE_ROWDIS, m_storeRowDis);
	DDV_MinMaxInt(pDX, m_storeRowDis, 0, CAMERA_Y_PX);
	DDX_Text(pDX, IDC_STORE_COLDIS, m_storeColDis);
	DDV_MinMaxInt(pDX, m_storeColDis, 0, CAMERA_X_PX);
	DDX_Text(pDX, IDC_STORE_X, m_storeStartX);
	DDV_MinMaxInt(pDX, m_storeStartX, 0, CAMERA_X_PX);
	DDX_Text(pDX, IDC_STORE_Y, m_storeStartY);
	DDV_MinMaxInt(pDX, m_storeStartY, 0, CAMERA_Y_PX);
	for (int i = 0; i < STORE_COL_BOUNDARY_COUNT; i++)
	{
		DDX_Text(pDX, ID_X[i], m_storeX[i]);
		DDV_MinMaxInt(pDX, m_storeX[i], 0, CAMERA_X_PX);
	}
	for (int i = 0; i < STORE_ROW_BOUNDARY_COUNT; i++)
	{
		DDX_Text(pDX, ID_Y[i], m_storeY[i]);
		DDV_MinMaxInt(pDX, m_storeY[i], 0, CAMERA_Y_PX);
	}
}


BEGIN_MESSAGE_MAP(CStoreSetDlg, CDialogEx)
	ON_BN_CLICKED(IDC_RADIO_STORE1, &CStoreSetDlg::OnBnClickedRadioStore1)
	ON_BN_CLICKED(IDC_RADIO_STORE2, &CStoreSetDlg::OnBnClickedRadioStore2)
	ON_WM_DESTROY()
	ON_EN_KILLFOCUS(IDC_STORE_ROWDIS, &CStoreSetDlg::OnEnKillfocusStoreRowdis)
	ON_EN_KILLFOCUS(IDC_STORE_COLDIS, &CStoreSetDlg::OnEnKillfocusStoreColdis)
	ON_EN_KILLFOCUS(IDC_STORE_X, &CStoreSetDlg::OnEnKillfocusStoreX)
	ON_EN_KILLFOCUS(IDC_STORE_Y, &CStoreSetDlg::OnKillfocusStoreY)
	ON_EN_KILLFOCUS(IDC_STORE_X0, &CStoreSetDlg::OnKillfocusStoreX0)
	ON_EN_KILLFOCUS(IDC_STORE_X1, &CStoreSetDlg::OnKillfocusStoreX1)
	ON_EN_KILLFOCUS(IDC_STORE_X2, &CStoreSetDlg::OnKillfocusStoreX2)
	ON_EN_KILLFOCUS(IDC_STORE_X3, &CStoreSetDlg::OnKillfocusStoreX3)
	ON_EN_KILLFOCUS(IDC_STORE_X4, &CStoreSetDlg::OnKillfocusStoreX4)
	ON_EN_KILLFOCUS(IDC_STORE_X5, &CStoreSetDlg::OnKillfocusStoreX5)
	ON_EN_KILLFOCUS(IDC_STORE_X6, &CStoreSetDlg::OnKillfocusStoreX6)
	ON_EN_KILLFOCUS(IDC_STORE_X7, &CStoreSetDlg::OnKillfocusStoreX7)
	ON_EN_KILLFOCUS(IDC_STORE_X8, &CStoreSetDlg::OnKillfocusStoreX8)
	ON_EN_KILLFOCUS(IDC_STORE_X9, &CStoreSetDlg::OnKillfocusStoreX9)
	ON_EN_KILLFOCUS(IDC_STORE_X10, &CStoreSetDlg::OnKillfocusStoreX10)
	ON_EN_KILLFOCUS(IDC_STORE_X11, &CStoreSetDlg::OnKillfocusStoreX11)
	ON_EN_KILLFOCUS(IDC_STORE_X12, &CStoreSetDlg::OnKillfocusStoreX12)
	ON_EN_KILLFOCUS(IDC_STORE_Y0, &CStoreSetDlg::OnKillfocusStoreY0)
	ON_EN_KILLFOCUS(IDC_STORE_Y1, &CStoreSetDlg::OnKillfocusStoreY1)
	ON_EN_KILLFOCUS(IDC_STORE_Y2, &CStoreSetDlg::OnKillfocusStoreY2)
	ON_EN_KILLFOCUS(IDC_STORE_Y3, &CStoreSetDlg::OnKillfocusStoreY3)
	ON_EN_KILLFOCUS(IDC_STORE_Y4, &CStoreSetDlg::OnKillfocusStoreY4)
	ON_EN_KILLFOCUS(IDC_STORE_Y5, &CStoreSetDlg::OnKillfocusStoreY5)
	ON_EN_KILLFOCUS(IDC_STORE_Y6, &CStoreSetDlg::OnKillfocusStoreY6)
	ON_EN_KILLFOCUS(IDC_STORE_Y7, &CStoreSetDlg::OnKillfocusStoreY7)
	ON_EN_KILLFOCUS(IDC_STORE_Y8, &CStoreSetDlg::OnKillfocusStoreY8)
	ON_EN_KILLFOCUS(IDC_STORE_Y9, &CStoreSetDlg::OnKillfocusStoreY9)
	ON_EN_KILLFOCUS(IDC_STORE_Y10, &CStoreSetDlg::OnKillfocusStoreY10)
END_MESSAGE_MAP()


// CStoreSetDlg 消息处理程序

BOOL CStoreSetDlg::PreTranslateMessage(MSG* pMsg)
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


BOOL CStoreSetDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	for (int i = 0; i < STORE_COL_BOUNDARY_COUNT; i++)
	{
		m_storeX[i] = CParameter::pStoreColBoundary[i];
	}
	for (int i = 0; i < STORE_ROW_BOUNDARY_COUNT; i++)
	{
		m_storeY[i] = CParameter::pStoreRowBoundary[i];
	}
	m_storeColDis = CParameter::pStoreColBoundary[1] - CParameter::pStoreColBoundary[0];
	m_storeRowDis = CParameter::pStoreRowBoundary[1] - CParameter::pStoreRowBoundary[0];
	m_storeStartX = CParameter::pStoreColBoundary[0];
	m_storeStartY = CParameter::pStoreRowBoundary[0];
	UpdateData(FALSE);
	if (bIsometryMode)
	{
		IsometryMode();
	}
	else
	{
		FreeMode();
	}
	((CButton*)GetDlgItem(IDC_RADIO_STORE1))->SetCheck(1);
	m_boundary = new LPARAM[2];
	m_boundary[0] = (LPARAM)m_storeX;
	m_boundary[1] = (LPARAM)m_storeY;
	UpdateDisplay();
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}

void CStoreSetDlg::IsometryMode()
{
	GetDlgItem(IDC_STORE_ROWDIS)->EnableWindow(TRUE);
	GetDlgItem(IDC_STORE_COLDIS)->EnableWindow(TRUE);
	GetDlgItem(IDC_STORE_X)->EnableWindow(TRUE);
	GetDlgItem(IDC_STORE_Y)->EnableWindow(TRUE);
	for (int i = 0; i < STORE_COL_BOUNDARY_COUNT; i++)
	{
		GetDlgItem(ID_X[i])->EnableWindow(FALSE);
	}
	for (int i = 0; i < STORE_ROW_BOUNDARY_COUNT; i++)
	{
		GetDlgItem(ID_Y[i])->EnableWindow(FALSE);
	}
}

void CStoreSetDlg::FreeMode()
{
	GetDlgItem(IDC_STORE_ROWDIS)->EnableWindow(FALSE);
	GetDlgItem(IDC_STORE_COLDIS)->EnableWindow(FALSE);
	GetDlgItem(IDC_STORE_X)->EnableWindow(FALSE);
	GetDlgItem(IDC_STORE_Y)->EnableWindow(FALSE);
	for (int i = 0; i < STORE_COL_BOUNDARY_COUNT; i++)
	{
		GetDlgItem(ID_X[i])->EnableWindow(TRUE);
	}
	for (int i = 0; i < STORE_ROW_BOUNDARY_COUNT; i++)
	{
		GetDlgItem(ID_Y[i])->EnableWindow(TRUE);
	}
}

void CStoreSetDlg::Default()
{
	for (int i = 0; i < STORE_COL_BOUNDARY_COUNT; i++)
	{
		m_storeX[i] = DEFAULT_STORE_COL_START + i * DEFAULT_STORE_COL_DISTANCE;
	}
	for (int i = 0; i < STORE_ROW_BOUNDARY_COUNT; i++)
	{
		m_storeX[i] = DEFAULT_STORE_ROW_START + i * DEFAULT_STORE_ROW_DISTANCE;
	}
	UpdateData(FALSE);
	IsometryMode();
	((CButton*)GetDlgItem(IDC_RADIO_STORE1))->SetCheck(1);
}

void CStoreSetDlg::SaveData()
{
	UpdateData();
	for (int i = 0; i < STORE_COL_BOUNDARY_COUNT; i++)
	{
		CParameter::pStoreColBoundary[i] = m_storeX[i];
	}
	for (int i = 0; i < STORE_ROW_BOUNDARY_COUNT; i++)
	{
		CParameter::pStoreRowBoundary[i] = m_storeY[i];
	}
}

void CStoreSetDlg::UpdateDisplay()
{
	::SendMessage(CCameraDlg::hCameraDlg, USER_WM_DRAWBOUNDARY, 0, (LPARAM)m_boundary);
}

void CStoreSetDlg::CalcByIso()
{
	for (int i = 0; i < STORE_COL_BOUNDARY_COUNT; i++)
	{
		m_storeX[i] = m_storeStartX + i * m_storeColDis;
	}
	for (int i = 0; i < STORE_ROW_BOUNDARY_COUNT; i++)
	{
		m_storeY[i] = m_storeStartY + i * m_storeRowDis;
	}
}


void CStoreSetDlg::OnBnClickedRadioStore1()
{
	IsometryMode();
	CalcByIso();
	UpdateData(FALSE);
	bIsometryMode = TRUE;
	UpdateDisplay();
}


void CStoreSetDlg::OnBnClickedRadioStore2()
{
	FreeMode();
	bIsometryMode = FALSE;
	UpdateDisplay();
}


void CStoreSetDlg::OnDestroy()
{
	CDialogEx::OnDestroy();

	// TODO: 在此处添加消息处理程序代码
	::SendMessage(CCameraDlg::hCameraDlg, USER_WM_DISDRAWBOUNDARY, 0, 0);
	delete m_boundary;
}


void CStoreSetDlg::OnEnKillfocusStoreRowdis()
{
	UpdateData();
	CalcByIso();
	UpdateData(FALSE);
	UpdateDisplay();
}


void CStoreSetDlg::OnEnKillfocusStoreColdis()
{
	UpdateData();
	CalcByIso();
	UpdateData(FALSE);
	UpdateDisplay();
}


void CStoreSetDlg::OnEnKillfocusStoreX()
{
	UpdateData();
	CalcByIso();
	UpdateData(FALSE);
	UpdateDisplay();
}


void CStoreSetDlg::OnKillfocusStoreY()
{
	UpdateData();
	CalcByIso();
	UpdateData(FALSE);
	UpdateDisplay();
}


void CStoreSetDlg::OnKillfocusStoreX0()
{
	UpdateData();
	UpdateDisplay();
}


void CStoreSetDlg::OnKillfocusStoreX1()
{
	UpdateData();
	UpdateDisplay();
}


void CStoreSetDlg::OnKillfocusStoreX2()
{
	UpdateData();
	UpdateDisplay();
}


void CStoreSetDlg::OnKillfocusStoreX3()
{
	UpdateData();
	UpdateDisplay();
}


void CStoreSetDlg::OnKillfocusStoreX4()
{
	UpdateData();
	UpdateDisplay();
}


void CStoreSetDlg::OnKillfocusStoreX5()
{
	UpdateData();
	UpdateDisplay();
}


void CStoreSetDlg::OnKillfocusStoreX6()
{
	UpdateData();
	UpdateDisplay();
}


void CStoreSetDlg::OnKillfocusStoreX7()
{
	UpdateData();
	UpdateDisplay();
}


void CStoreSetDlg::OnKillfocusStoreX8()
{
	UpdateData();
	UpdateDisplay();
}


void CStoreSetDlg::OnKillfocusStoreX9()
{
	UpdateData();
	UpdateDisplay();
}


void CStoreSetDlg::OnKillfocusStoreX10()
{
	UpdateData();
	UpdateDisplay();
}


void CStoreSetDlg::OnKillfocusStoreX11()
{
	UpdateData();
	UpdateDisplay();
}


void CStoreSetDlg::OnKillfocusStoreX12()
{
	UpdateData();
	UpdateDisplay();
}


void CStoreSetDlg::OnKillfocusStoreY0()
{
	UpdateData();
	UpdateDisplay();
}


void CStoreSetDlg::OnKillfocusStoreY1()
{
	UpdateData();
	UpdateDisplay();
}


void CStoreSetDlg::OnKillfocusStoreY2()
{
	UpdateData();
	UpdateDisplay();
}


void CStoreSetDlg::OnKillfocusStoreY3()
{
	UpdateData();
	UpdateDisplay();
}


void CStoreSetDlg::OnKillfocusStoreY4()
{
	UpdateData();
	UpdateDisplay();
}


void CStoreSetDlg::OnKillfocusStoreY5()
{
	UpdateData();
	UpdateDisplay();
}


void CStoreSetDlg::OnKillfocusStoreY6()
{
	UpdateData();
	UpdateDisplay();
}


void CStoreSetDlg::OnKillfocusStoreY7()
{
	UpdateData();
}


void CStoreSetDlg::OnKillfocusStoreY8()
{
	UpdateData();
}


void CStoreSetDlg::OnKillfocusStoreY9()
{
	UpdateData();
}


void CStoreSetDlg::OnKillfocusStoreY10()
{
	UpdateData();
}
