// CameraDlg.cpp: 实现文件
//

#include "stdafx.h"
#include "TetrisPlace.h"
#include "CameraDlg.h"
#include "afxdialogex.h"
#include <stdio.h>


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
	HWND hImgWnd = GetDlgItem(IDC_PICTURE)->m_hWnd;
	OpenWindow(0, 0, 640, 480, (Hlong)hImgWnd, "visible", "", &hv_WindowHandle);
	CreateThread(NULL, 0, CameraThreadProc, this, 0, 0);
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

DWORD WINAPI CameraThreadProc(LPVOID lpParam)
{
	CCameraDlg * dlg = (CCameraDlg*)lpParam;
	HWND hImgWnd = dlg->GetDlgItem(IDC_PICTURE)->m_hWnd;

	HObject  ho_MapFixed;
	HObject  ho_Region, ho_RegionClosing, ho_ConnectedRegions;
	HTuple  hv_CamParVirtualFixed, hv_CamParOriginal;
	HTuple width, height;

	try
	{
		OpenFramegrabber("DirectShow", 1, 1, 0, 0, 0, 0, "default", 8, "gray", -1, "false",
			"default", "1", 0, -1, &(dlg->hv_AcqHandle));
	}
	catch (HException & exception)
	{
		AfxMessageBox(_T("无法打开相机"));
		return 0;
	}
	GrabImageStart(dlg->hv_AcqHandle, -1);
	GrabImageAsync(&(dlg->ho_Image), dlg->hv_AcqHandle, -1);
	GetImageSize(dlg->ho_Image, &width, &height);

	int i = 0;
	while (1)
	{
		GrabImageAsync(&dlg->ho_Image, dlg->hv_AcqHandle, -1);

		hv_CamParOriginal.Clear();
		hv_CamParOriginal[0] = 0.0384924;
		hv_CamParOriginal[1] = -5311.75;
		hv_CamParOriginal[2] = 8.43797e-006;
		hv_CamParOriginal[3] = 8.3e-006;
		hv_CamParOriginal[4] = 425.258;
		hv_CamParOriginal[5] = 301.19;
		hv_CamParOriginal[6] = 800;
		hv_CamParOriginal[7] = 600;
		hv_CamParVirtualFixed = hv_CamParOriginal;
		hv_CamParVirtualFixed[1] = 0;
		GenRadialDistortionMap(&ho_MapFixed, hv_CamParOriginal, hv_CamParVirtualFixed,
			"bilinear");
		MapImage(dlg->ho_Image, ho_MapFixed, &dlg->ho_Image);//纠正相机畸变

		Threshold(dlg->ho_Image, &ho_Region, 155, 255);

		OpeningCircle(ho_Region, &ho_RegionClosing, 1.2);
		Connection(ho_RegionClosing, &ho_ConnectedRegions);
		SelectShape(ho_ConnectedRegions, &dlg->ho_SelectedRegions, "area", "and", 9000, 20000);
		SetPart(dlg->hv_WindowHandle, 0, 0, height, width);
		DispObj(dlg->ho_Image, dlg->hv_WindowHandle);
		dlg->Distinguish();
		Sleep(1000);
	}
	return 0;
}

void CCameraDlg::Distinguish()
{
	HTuple ScaleMin[TYPE_COUNT] = { 0.95,0.95,0.95,0.95,0.95,0.95,0.95 };
	HTuple ScaleMax[TYPE_COUNT] = { 1.05,1.05,1.05,1.05,1.05,1.05,1.05 };
	HTuple MinScore[TYPE_COUNT] = { 0.9,0.9,0.9,0.9,0.9,0.9,0.9 };
	HTuple NumMatches[TYPE_COUNT] = { 5,5,5,5,5,5,5 };
	HTuple MaxOverlap[TYPE_COUNT] = { 0.5,0.5,0.5,0.5,0.5,0.5,0.5 };
	HTuple Greediness[TYPE_COUNT] = { 0.8,0.8,0.8,0.8,0.8,0.8,0.8 };

	try
	{
		for (int i = 0; i < TYPE_COUNT; i++)
		{
			FindScaledShapeModel(ho_Image, hv_ModelID[i], HTuple(0).TupleRad(), HTuple(360).TupleRad(),
				ScaleMin[i], ScaleMax[i], MinScore[i], NumMatches[i], MaxOverlap[i],
				"least_squares", (HTuple(5).Append(1)), Greediness[i],
				&hv_Row[i], &hv_Column[i], &hv_Angle[i], &hv_Scale[i], &hv_Score[i]);

#ifndef NONE_UI
			if (hv_Row[i].Length() > 0)
			{
				HTuple  hv_RefColumn, hv_HomMat2D, hv_TestImages, hv_T;
				HObject  ho_TemplateImage, ho_ModelContours, ho_TransContours;

				GetShapeModelContours(&ho_ModelContours, hv_ModelID[i], 1);

				HomMat2dIdentity(&hv_HomMat2D);
				HomMat2dScale(hv_HomMat2D, hv_Scale[i], hv_Scale[i],
					0, 0, &hv_HomMat2D);
				HomMat2dRotate(hv_HomMat2D, hv_Angle[i], 0, 0, &hv_HomMat2D);
				HomMat2dTranslate(hv_HomMat2D, hv_Row[i], hv_Column[i],
					&hv_HomMat2D);
				AffineTransContourXld(ho_ModelContours, &ho_TransContours, hv_HomMat2D);

				SetColor(hv_WindowHandle, "green");
				SetLineWidth(hv_WindowHandle, 3.5);

				DispObj(ho_TransContours, hv_WindowHandle);
			}
#endif // !NONE_UI

		}
	}
	catch (HException & exception)
	{
		fprintf(stderr, "  Error #%u in %s: %s\n", exception.ErrorCode(),
			(const char*)exception.ProcName(),
			(const char*)exception.ErrorMessage());
	}
}

