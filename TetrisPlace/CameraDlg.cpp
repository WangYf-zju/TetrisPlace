// CameraDlg.cpp: 实现文件
//

#include "stdafx.h"
#include "afxdialogex.h"
#include "TetrisPlace.h"
#include "TetrisPlaceDlg.h"
#include "CameraDlg.h"

mutex cmrimgLock;

HWND CCameraDlg::hCameraDlg = nullptr;
CCameraDlg * CCameraDlg::instance = nullptr;

const int AngleOffset[TYPE_COUNT] = { 90,90,90,90,90,90,90 };
const double PictureXOffset[TYPE_COUNT][4] = {
	{9,0,-9,0},{9,0,-9,0},{-9,0,-9,0},{-9,0,-9,0},{-9,-9,-9,-9},{0,-9,0,-9},{9,0,-9,0}
};
const double PictureYOffset[TYPE_COUNT][4] = {
	{0,9,0,-9},{0,9,0,-9},{0,-9,0,-9},{0,-9,0,-9},{-9,-9,-9,-9},{-9,0,-9,0},{0,9,0,-9}
};
const double GrabXGridOffset[TYPE_COUNT][4] = {
	{0,0,0,0},{0,0,0,0},{0.5,0,0.5,0},{0.5,0,0.5,0},{0.5,0.5,0.5,0.5},{0,0.5,0,0.5},{0,0,0,0}
};
const double GrabYGridOffset[TYPE_COUNT][4] = {
	{0,0,0,0},{0,0,0,0},{0,-0.5,0,-0.5},{0,-0.5,0,-0.5},{-0.5,-0.5,-0.5,-0.5},{-0.5,0,-0.5,0},{0,0,0,0}
};
const double ToXGridOffset[TYPE_COUNT][4] = {
	{1,1,1,1},{1,1,1,1},{1,0.5,1,0.5},{1,1.5,1,1.5},{0.5,0.5,0.5,0.5},{1.5,1,1.5,1},{1,1,1,1}
};
const double ToYGridOffset[TYPE_COUNT][4] = {
	{1,1,1,1},{1,1,1,1},{0.5,1,0.5,1},{0.5,1,0.5,1},{0.5,0.5,0.5,0.5},{1,1.5,1,1.5},{1,1,1,1}
};

// CCameraDlg 对话框

IMPLEMENT_DYNAMIC(CCameraDlg, CDialogEx)

CCameraDlg::CCameraDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_CAMERA, pParent)
{

}

CCameraDlg::~CCameraDlg()
{
}

void CCameraDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CCameraDlg, CDialogEx)
	ON_WM_PAINT()
END_MESSAGE_MAP()


// CCameraDlg 消息处理程序


BOOL CCameraDlg::PreTranslateMessage(MSG* pMsg)
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


BOOL CCameraDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	hCameraDlg = this->m_hWnd;
	instance = this;

	m_bGrab = FALSE;
	m_bDistinguish = TRUE;
	m_bLoop = FALSE;
	m_bStart = FALSE;
	m_bCorrect = FALSE;
	m_bDrawBoundary = FALSE;
	m_bBoundaryChange = FALSE;
	try
	{
		for (int i = 0; i < TYPE_COUNT; i++)
		{
			char path[100] = { 0 };
			sprintf_s(path, "%s%d%s\0", "./models/model", i + 1, ".shm");
			ReadShapeModel(path, &hv_ModelID[i]);
		}
	}
	catch (...)
	{
		MessageBox(_T("打开模板失败"));
	}
	
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}

BOOL CCameraDlg::StartCamera(int iCamera)
{
	HWND hImgWnd = GetDlgItem(IDC_PICTURE)->m_hWnd;
	CRect rc;
	GetDlgItem(IDC_PICTURE)->GetClientRect(&rc);
	OpenWindow(0, 0, rc.Width(), rc.Height(), (Hlong)hImgWnd, "visible", "", &hv_WindowHandle);
	char sCamera[5] = { 0 };
	sprintf(sCamera, _T("%d"), iCamera);
	try
	{
		OpenFramegrabber("DirectShow", 1, 1, 0, 0, 0, 0, "default", 8, "gray", -1, "false",
			"default", sCamera, 0, -1, &hv_AcqHandle);
	}
	catch (HException & exception)
	{
		MessageBox(_T("无法打开相机"));
		return FALSE;
	}

	GrabImageStart(hv_AcqHandle, -1);
	GrabImageAsync(&ho_Image, hv_AcqHandle, -1);
	GetImageSize(ho_Image, &hl_width, &hl_height);


	hThread = CreateThread(NULL, 0, CameraThreadProc, this, 0, 0);
	m_bStart = TRUE;
	m_bDistinguish = TRUE;
	m_cameraIndex = iCamera;
	return TRUE;
}

void CCameraDlg::CloseCamera()
{
	m_bStart = FALSE;
	m_bCorrect = FALSE;
	m_bGrab = FALSE;
	m_bLoop = FALSE;
	CloseHandle(hThread);
}

#define SCALE_PICTURE 39/18
int CCameraDlg::GetGridX(int type, int r, double col)
{
	int x = (int)(col + PictureXOffset[type][r] * SCALE_PICTURE);
	for (int i = 0; i < 12; i++)
	{
		if (x > CParameter::pStoreColBoundary[i] && x < CParameter::pStoreColBoundary[i + 1])
		{
			return i;
		}
	}
	return -1;
}
int CCameraDlg::GetGridY(int type, int r, double row)
{
	int y = (int)(row + PictureYOffset[type][r] * SCALE_PICTURE);
	for (int i = 0; i < 10; i++)
	{
		if (y > CParameter::pStoreRowBoundary[i] && y < CParameter::pStoreRowBoundary[i + 1])
		{
			return 9 - i;
		}
	}
	return -1;
}

int CCameraDlg::GetGridR(int type, double rot)
{
	int r = (int)(-rot*180/3.1415926 + AngleOffset[type]);
	r += 45;
	if (r < 0)r += 360;
	else if (r > 360)r -= 360;
	r /= 90;
	return r;
}

void CCameraDlg::UpdateBoardDlg()
{
	((CTetrisPlaceDlg*)GetParent())->PostMessage(USER_WM_PAINTBOARD, 0, 0);
}

void CCameraDlg::DrawTetrisOnBoard(int type, Position * pos)
{
	WPARAM wParam = (WPARAM)type;
	LPARAM lParam = (LPARAM)pos;
	((CTetrisPlaceDlg*)GetParent())->PostMessage(USER_WM_PAINTTETRIS, wParam, lParam);
}

void CCameraDlg::DrawContours(int type)
{
	HTuple  hv_RefColumn, hv_HomMat2D, hv_TestImages, hv_T;
	HObject  ho_TemplateImage, ho_ModelContours, ho_TransContours;

	GetShapeModelContours(&ho_ModelContours, hv_ModelID[type], 1);

	HomMat2dIdentity(&hv_HomMat2D);
	HomMat2dScale(hv_HomMat2D, (hv_Scale[type])[0], (hv_Scale[type])[0],
		0, 0, &hv_HomMat2D);
	HomMat2dRotate(hv_HomMat2D, (hv_Angle[type])[0], 0, 0, &hv_HomMat2D);
	HomMat2dTranslate(hv_HomMat2D, (hv_Row[type])[0], (hv_Column[type])[0],
		&hv_HomMat2D);
	AffineTransContourXld(ho_ModelContours, &ho_TransContours, hv_HomMat2D);

	SetColor(hv_WindowHandle, "red");
	SetLineWidth(hv_WindowHandle, 5);

	DispObj(ho_TransContours, hv_WindowHandle);
}

void CCameraDlg::DrawBoundary()
{
	//m_bDrawBoundary = FALSE;
	HObject hBoundary;
	for (int i = 0; i < STORE_COL_BOUNDARY_COUNT; i++)
	{
		GenRegionLine(&hBoundary, 0, m_xBoundary[i], CAMERA_Y_PX, m_xBoundary[i]);
		DispObj(hBoundary, hv_WindowHandle);
	}
	for (int i = 0; i < STORE_ROW_BOUNDARY_COUNT; i++)
	{
		GenRegionLine(&hBoundary, m_yBoundary[i], 0, m_yBoundary[i], CAMERA_X_PX);
		DispObj(hBoundary, hv_WindowHandle);
	}
}


void CCameraDlg::Distinguish()
{
	HTuple ScaleMin[TYPE_COUNT] = { 0.95,0.95,0.95,0.95,0.95,0.95,0.95 };
	HTuple ScaleMax[TYPE_COUNT] = { 1.1,1.1,1.1,1.1,1.1,1.1,1.1 };
	HTuple MinScore[TYPE_COUNT] = { 0.9,0.9,0.9,0.9,0.9,0.9,0.9 };
	HTuple NumMatches[TYPE_COUNT] = { 5,5,5,5,5,5,5 };
	HTuple MaxOverlap[TYPE_COUNT] = { 0.5,0.5,0.5,0.5,0.5,0.5,0.5 };
	HTuple Greediness[TYPE_COUNT] = { 0.8,0.8,0.8,0.8,0.8,0.8,0.8 };

	try
	{
		double max_rank = -1e3;
		int supreme_type = -1;
		Position pos;
		for (int i = 0; i < TYPE_COUNT; i++)
		{
			FindScaledShapeModel(ho_Image, hv_ModelID[i], HTuple(0).TupleRad(), HTuple(360).TupleRad(),
				ScaleMin[i], ScaleMax[i], MinScore[i], NumMatches[i], MaxOverlap[i],
				"least_squares", (HTuple(5).Append(1)), Greediness[i],
				&hv_Row[i], &hv_Column[i], &hv_Angle[i], &hv_Scale[i], &hv_Score[i]);

			int length = hv_Row[i].Length();
			m_typeInfo[i].count = length;
			if (length <= 0)
			{
				m_typeInfo[i].bExist = FALSE;
			}
			else
			{
				m_typeInfo[i].bExist = TRUE;
				TetrisAI AI(i);
				double rank = AI.GetSupremeRank();
				m_typeInfo[i].rank = AI.GetSupremeRank();
				m_typeInfo[i].grid_toX = AI.GetSupremePos()->x;
				m_typeInfo[i].grid_toY = AI.GetSupremePos()->y;
				m_typeInfo[i].grid_toR = AI.GetSupremePos()->r;
				m_typeInfo[i].grid_r = GetGridR(i, (hv_Angle[i])[0].D());
				m_typeInfo[i].grid_x = GetGridX(i, m_typeInfo[i].grid_r, (hv_Column[i])[0].D());
				double test = (hv_Row[i])[0].D();
				m_typeInfo[i].grid_y = GetGridY(i, m_typeInfo[i].grid_r, (hv_Row[i])[0].D());
				m_typeInfo[i].x = 
					(m_typeInfo[i].grid_x + GrabXGridOffset[i][m_typeInfo[i].grid_r]) 
					* GRID_DISTANCE + CParameter::offsetX1;
				m_typeInfo[i].y = 
					(m_typeInfo[i].grid_y + GrabYGridOffset[i][m_typeInfo[i].grid_r]) 
					* GRID_DISTANCE + CParameter::offsetY1;
				// 放置仓从下向上放，需旋转180°
				m_typeInfo[i].toX = -m_typeInfo[i].grid_toY * GRID_DISTANCE
					+ CParameter::offsetX2;
				m_typeInfo[i].toY = (9-m_typeInfo[i].grid_toX) * GRID_DISTANCE
					+ CParameter::offsetY2;

				if (max_rank < rank && AI.m_canPlace)
				{
					max_rank = rank;
					supreme_type = i;
					pos.x = AI.GetSupremePos()->x;
					pos.y = AI.GetSupremePos()->y;
					pos.r = AI.GetSupremePos()->r;
				}
#ifndef NONE_UI
				DrawContours(i);
#endif // !NONE_UI
			}
		}
		UpdateInfo(supreme_type);
		if (m_bGrab && !CArmControlDlg::bArmBusy)
		{
			if (supreme_type >= 0)
			{
				int r = GetGridR(supreme_type, (hv_Angle[supreme_type])[0].D());
				int grid_x = GetGridX(supreme_type, r, (hv_Column[supreme_type])[0].D());
				int grid_y = GetGridY(supreme_type, r, (hv_Row[supreme_type])[0].D());
				double grid_toX = pos.x + ToXGridOffset[supreme_type][pos.r];
				double grid_toY = pos.y + ToYGridOffset[supreme_type][pos.r];
				int dr = pos.r - r;
				dr -= 2;
				if (dr < 0)dr += 4;
				else if (dr > 4)dr -= 4;
				int symmetry = 0;
				if (supreme_type == 2 || supreme_type == 3 || supreme_type == 5)
					symmetry = 1;
				else if (supreme_type == 4)
					symmetry = 2;
				
				double x_cpst = 0, y_cpst = 0, toy_cpst = 0;
				// if (grid_x > 3)
					x_cpst = (grid_x - 3) * 2 / 3;
				if (grid_toX < 6)
					toy_cpst = 1.0 * (6 - grid_toX);
				double x = (grid_x + GrabXGridOffset[supreme_type][r]) * 18 + CParameter::offsetX1 + x_cpst;
				double y = (grid_y + GrabYGridOffset[supreme_type][r]) * 18 + CParameter::offsetY1 + y_cpst;
				double toX = -grid_toY * 18 + CParameter::offsetX2;
				double toY = -grid_toX * 18 + CParameter::offsetY2 + 18 * 9 + toy_cpst;

				CArmControlDlg::instance->Grab(x, y, toX, toY, dr * 90.0, symmetry);
				if (!m_bLoop) m_bGrab = FALSE;
				TetrisAI::PlaceTetris(supreme_type, &pos);
				DrawTetrisOnBoard(supreme_type, &pos);
			}
			else
			{
				m_bLoop = FALSE;
				m_bGrab = FALSE;
				MessageBox(_T("当前已经没有可以放置的方块了"));
			}
		}
	}
	catch (HException & exception)
	{
		fprintf(stderr, "  Error #%u in %s: %s\n", exception.ErrorCode(),
			(const char*)exception.ProcName(),
			(const char*)exception.ErrorMessage());
	}
}

void CCameraDlg::StartDistinguishAndGrabOnce()
{
	m_bDistinguish = TRUE;
	m_bGrab = TRUE;
	m_bLoop = FALSE;
}

void CCameraDlg::StartDistinguishAndGrabLoop()
{
	m_bDistinguish = TRUE;
	m_bGrab = TRUE;
	m_bLoop = TRUE;
}

void CCameraDlg::CorrectArm()
{
	HTuple ScaleMin = 0.95;
	HTuple ScaleMax = 1.05;
	HTuple MinScore = 0.9;
	HTuple NumMatches = 1;
	HTuple MaxOverlap = 0.5;
	HTuple Greediness = 0.8;

	try
	{
		//FindScaledShapeModel(ho_Image, hv_ModelID[i], HTuple(0).TupleRad(), HTuple(360).TupleRad(),
		//	ScaleMin, ScaleMax, MinScore, NumMatches, MaxOverlap,
		//	"least_squares", (HTuple(5).Append(1)), Greediness,
		//	&hv_Row, &hv_Column, &hv_Angle, &hv_Scale, &hv_Score);
		double d_x = 0, d_y = 0;


	}
	catch (HException & expection)
	{

	}
}


DWORD WINAPI CameraThreadProc(LPVOID lpParam)
{
	CCameraDlg * dlg = (CCameraDlg*)lpParam;
	HWND hImgWnd = dlg->GetDlgItem(IDC_PICTURE)->m_hWnd;

	HObject  ho_MapFixed;
	HObject  ho_Region, ho_RegionClosing, ho_RegionOpening, ho_ConnectedRegions, ho_Gauss, ho_ROI;
	HObject  ho_ImageReduce, ho_ImageReduce1;
	HTuple  hv_CamParVirtualFixed, hv_CamParOriginal;
	
	while (1 && dlg->m_bStart)
	{
		GrabImageAsync(&dlg->ho_Image, dlg->hv_AcqHandle, -1);

		hv_CamParOriginal.Clear();
		hv_CamParOriginal[0] = 0.00652862;
		hv_CamParOriginal[1] = -9224.01;
		hv_CamParOriginal[2] = 8.30004e-006;
		hv_CamParOriginal[3] = 8.3e-006;
		hv_CamParOriginal[4] = 323.226;
		hv_CamParOriginal[5] = 226.988;
		hv_CamParOriginal[6] = 640;
		hv_CamParOriginal[7] = 480;
		hv_CamParVirtualFixed = hv_CamParOriginal;
		hv_CamParVirtualFixed[1] = 0;
		GenRadialDistortionMap(&ho_MapFixed, hv_CamParOriginal, hv_CamParVirtualFixed,"bilinear");
		MapImage(dlg->ho_Image, ho_MapFixed, &dlg->ho_Image);//纠正相机畸变

		GaussFilter(dlg->ho_Image, &ho_Gauss, 5);
	
		GenRectangle1(&ho_ROI, 58.3, 61.5, 438.3, 511.1);
		ReduceDomain(ho_Gauss, ho_ROI, &ho_ImageReduce);

		Threshold(ho_ImageReduce, &ho_Region, 145, 255);
		OpeningCircle(ho_Region, &ho_RegionOpening, 1.5);
		ClosingCircle(ho_RegionOpening, &ho_RegionClosing, 1.5);

		Connection(ho_RegionClosing, &ho_ConnectedRegions);
		ReduceDomain(ho_ImageReduce, ho_ConnectedRegions, &ho_ImageReduce1);

		SetPart(dlg->hv_WindowHandle, 0, 0, dlg->hl_height, dlg->hl_width);

		cmrimgLock.lock();
		WriteImage((dlg->ho_Image), "jpg", 0, "./temp/cmr-temp.jpg");
		cmrimgLock.unlock();

		if (!dlg->m_bDrawBoundary || dlg->m_bBoundaryChange)
		{
			DispObj(dlg->ho_Image, dlg->hv_WindowHandle);
			if (dlg->m_bDistinguish)
			{
				dlg->Distinguish();
			}
			dlg->m_bBoundaryChange = FALSE;
		}
		else
		{
			dlg->DrawBoundary();
		}
		if (dlg->m_bCorrect)
		{
			dlg->CorrectArm();
			Sleep(1000);
		}
		Sleep(500);
	}
	CloseFramegrabber(dlg->hv_AcqHandle);
	CloseWindow(dlg->hv_WindowHandle);
	return 0;
}



void CCameraDlg::UpdateInfo(int supreme)
{
	::PostMessage(CInfoDlg::hInfoDlg, USER_WM_UPDATEINFO,
		(WPARAM)supreme, (LPARAM)&m_typeInfo);
}


LRESULT CCameraDlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	// TODO: 在此添加专用代码和/或调用基类
	switch (message)
	{
	case USER_WM_DRAWBOUNDARY:
		m_xBoundary = (int*)(((LPARAM*)lParam)[0]);
		m_yBoundary = (int*)(((LPARAM*)lParam)[1]);
		m_bDrawBoundary = TRUE;
		m_bBoundaryChange = TRUE;
		break;
	case USER_WM_DISDRAWBOUNDARY:
		m_bDrawBoundary = FALSE;
		m_bBoundaryChange = FALSE;
		break;
	}
	return CDialogEx::WindowProc(message, wParam, lParam);
}


void CCameraDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
					   // TODO: 在此处添加消息处理程序代码
					   // 不为绘图消息调用 CDialogEx::OnPaint()

}
