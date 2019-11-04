#pragma once
#include "HalconCpp.h"
#include "TetrisAI.h"
using namespace HalconCpp;
#define TYPE_COUNT 7
#define GRID_DISTANCE	18

typedef struct {
	BOOL bExist;
	int count;
	int rank;
	int grid_x;
	int grid_y;
	int grid_r;
	int grid_toX;
	int grid_toY;
	int grid_toR;
	double x;
	double y;
	double toX;
	double toY;
	double dx;
	double dy;
	double dr;
}TypeInfo;

// CCameraDlg 对话框

class CCameraDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CCameraDlg)

public:
	CCameraDlg(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CCameraDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_CAMERA };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL OnInitDialog();
	BOOL StartCamera(int iCamera);
	void CloseCamera();
	void Distinguish();
	void StartDistinguishAndGrabOnce();
	void StartDistinguishAndGrabLoop();
	void CorrectArm();
	void UpdateBoardDlg();
	void DrawTetrisOnBoard(int type, Position * pos);

private:
	int GetGridX(int type, int r, double col);
	int GetGridY(int type, int r, double row);
	int GetGridR(int type, double rot);
	void DrawContours(int type);
public:
	BOOL m_bDistinguish;
	BOOL m_bGrab;
	BOOL m_bStart;
	BOOL m_bLoop;
	BOOL m_bCorrect;
	int m_cameraIndex;
	TypeInfo m_typeInfo[TYPE_COUNT];
	HANDLE hThread;
	HObject  ho_Image;//, ho_Region, ho_ConnectedRegions, ho_SelectedRegions;
	HObject  ho_SelectedRegions;
	HTuple hl_width, hl_height;
	HTuple  hv_WindowHandle;
	HTuple  hv_AcqHandle;
	HTuple hv_Row[TYPE_COUNT],
		hv_Column[TYPE_COUNT],
		hv_Angle[TYPE_COUNT],
		hv_Scale[TYPE_COUNT],
		hv_Score[TYPE_COUNT],
		hv_ModelID[TYPE_COUNT];

	void UpdateInfo(int supreme);
};

DWORD WINAPI CameraThreadProc(LPVOID lpParam);