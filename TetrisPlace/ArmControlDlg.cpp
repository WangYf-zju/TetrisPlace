// ArmControlDlg.cpp: 实现文件
//

#include "stdafx.h"
#include "TetrisPlace.h"
#include "ArmControlDlg.h"
#include "afxdialogex.h"


// CArmControlDlg 对话框

IMPLEMENT_DYNAMIC(CArmControlDlg, CDialogEx)

CArmControlDlg::CArmControlDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_ARMCONTROL, pParent)
	, m_X(0)
	, m_Y(0)
	, m_Z(0)
	, m_AX(0)
	, m_AY(0)
	, m_AZ(0)
	, m_steer(0)
	, m_moveX(0)
	, m_moveY(0)
	, m_moveToX(0)
	, m_moveToY(0)
	, m_moveR(0)
	, m_state(_T(""))
{

}

CArmControlDlg::~CArmControlDlg()
{
}

void CArmControlDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_X, m_X);
	DDX_Text(pDX, IDC_EDIT_Y, m_Y);
	DDX_Text(pDX, IDC_EDIT_Z, m_Z);
	DDX_Text(pDX, IDC_EDIT_ANGLEX, m_AX);
	DDX_Text(pDX, IDC_EDIT_ANGLEY, m_AY);
	DDX_Text(pDX, IDC_EDIT_ANGLEZ, m_AZ);
	DDX_Text(pDX, IDC_EDIT_STEER, m_steer);
	DDX_Text(pDX, IDC_EDIT_MOVEX, m_moveX);
	DDX_Text(pDX, IDC_EDIT_MOVEY, m_moveY);
	DDX_Text(pDX, IDC_EDIT_MOVEToX, m_moveToX);
	DDX_Text(pDX, IDC_EDIT_MOVEToY, m_moveToY);
	DDX_Text(pDX, IDC_EDIT_MOVER, m_moveR);
	DDX_Text(pDX, IDC_STATE, m_state);
}


BEGIN_MESSAGE_MAP(CArmControlDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_COORGO, &CArmControlDlg::OnBnClickedButtonCoorgo)
	ON_BN_CLICKED(IDC_BUTTON_GRAB, &CArmControlDlg::OnBnClickedButtonGrab)
	ON_BN_CLICKED(IDC_BUTTON_PLACE, &CArmControlDlg::OnBnClickedButtonPlace)
	ON_BN_CLICKED(IDC_BUTTON_GOANGLE, &CArmControlDlg::OnBnClickedButtonGoangle)
	ON_BN_CLICKED(IDC_BUTTON_OPENPUMP, &CArmControlDlg::OnBnClickedButtonOpenpump)
	ON_BN_CLICKED(IDC_BUTTON_CLOSEPUMP, &CArmControlDlg::OnBnClickedButtonClosepump)
	ON_BN_CLICKED(IDC_BUTTON_STEERGO, &CArmControlDlg::OnBnClickedButtonSteergo)
	ON_BN_CLICKED(IDC_BUTTON_UNLOCK, &CArmControlDlg::OnBnClickedButtonUnlock)
	ON_BN_CLICKED(IDC_RELATIVE, &CArmControlDlg::OnBnClickedRelative)
	ON_BN_CLICKED(IDC_ABSOLUTE, &CArmControlDlg::OnBnClickedAbsolute)
	ON_BN_CLICKED(IDC_SETTINGON, &CArmControlDlg::OnBnClickedSettingon)
	ON_BN_CLICKED(IDC_SETTINGOFF, &CArmControlDlg::OnBnClickedSettingoff)
	ON_BN_CLICKED(IDC_BUTTON_MOVE, &CArmControlDlg::OnBnClickedButtonMove)
	ON_BN_CLICKED(IDC_BUTTON_SEGGO, &CArmControlDlg::OnBnClickedButtonSeggo)
END_MESSAGE_MAP()


// CArmControlDlg 消息处理程序


BOOL CArmControlDlg::PreTranslateMessage(MSG* pMsg)
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


BOOL CArmControlDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_pA = new Arm();
	m_bRelative = TRUE;
	((CButton*)GetDlgItem(IDC_RELATIVE))->SetCheck(1);
	m_bSettingMode = FALSE;
	m_bLock = FALSE;
	((CButton*)GetDlgItem(IDC_SETTINGOFF))->SetCheck(1);
	UpdateState();
	hThread = CreateThread(NULL, 0, ArmCtrlThreadProc, this, 0, 0);
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}

void CArmControlDlg::BindSerialPort(WzSerialPort * w, BOOL bArmInit)
{
	m_pW = w;
	m_pA->BindSerialPort(w, bArmInit);
}

void CArmControlDlg::UpdateState()
{
	m_state.Format(_T("坐标：X %.6f Y%.6f Z%.6f\n角度：X %.6f Y%.6f Z%.6f\n舵机角度：%.6f"),
		m_pA->m_cCoor[0], m_pA->m_cCoor[1], m_pA->m_cCoor[2],
		m_pA->m_cAngle[0], m_pA->m_cAngle[1], m_pA->m_cAngle[2],
		m_pA->m_steerAngle);
	GetDlgItem(IDC_STATE)->SetWindowText(m_state);
}


void CArmControlDlg::OnBnClickedButtonCoorgo()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData();
	if (m_bSettingMode)
	{
		SetCoor(m_X, m_Y, m_Z);
		UpdateState();
	}
	else
	{
		m_bLock = TRUE;
		GetDlgItem(IDC_BUTTON_UNLOCK)->SetWindowTextW(_T("电机解锁"));
		if (m_bRelative)
			GoToR(m_X, m_Y, m_Z);
		else
			GoTo(m_X, m_Y, m_Z);
	}
}


void CArmControlDlg::OnBnClickedButtonGrab()
{
	// TODO: 在此添加控件通知处理程序代码
	m_bLock = TRUE;
	GetDlgItem(IDC_BUTTON_UNLOCK)->SetWindowTextW(_T("电机解锁"));
	UpdateData();
	ArmMsg msg;
	msg.msg = AM_GRAB;
	msgArray.push_back(msg);
}


void CArmControlDlg::OnBnClickedButtonPlace()
{
	// TODO: 在此添加控件通知处理程序代码
	m_bLock = TRUE;
	GetDlgItem(IDC_BUTTON_UNLOCK)->SetWindowTextW(_T("电机解锁"));
	UpdateData();
	ArmMsg msg;
	msg.msg = AM_PLACE;
	msgArray.push_back(msg);
}


void CArmControlDlg::OnBnClickedButtonGoangle()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData();
	if (m_bSettingMode)
	{
		SetAngle(m_AX, m_AY, m_AZ);
		UpdateState();
	}
	else
	{
		m_bLock = TRUE;
		GetDlgItem(IDC_BUTTON_UNLOCK)->SetWindowTextW(_T("电机解锁"));
		ArmMsg msg;
		if (m_bRelative)
			msg.msg = AM_GOANGLETOR;
		else
			msg.msg = AM_GOANGLETO;
		msg.param.push_back(m_AX);
		msg.param.push_back(m_AY);
		msg.param.push_back(m_AZ);
		msgArray.push_back(msg);

	}
}


void CArmControlDlg::OnBnClickedButtonOpenpump()
{
	// TODO: 在此添加控件通知处理程序代码
	m_pA->OpenPump();
	UpdateState();
}


void CArmControlDlg::OnBnClickedButtonClosepump()
{
	// TODO: 在此添加控件通知处理程序代码
	m_pA->ClosePump();
	UpdateState();
}


void CArmControlDlg::OnBnClickedButtonSteergo()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	ArmMsg msg;
	msg.msg = AM_STEERTO;
	msg.param.push_back(m_steer);
	msgArray.push_back(msg);
}


void CArmControlDlg::OnBnClickedButtonUnlock()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_bLock)
	{
		m_pA->UnlockMotor();
		m_bLock = FALSE;
		GetDlgItem(IDC_BUTTON_UNLOCK)->SetWindowTextW(_T("电机锁定"));
	}
	else
	{
		m_pA->LockMotor();
		m_bLock = TRUE;
		GetDlgItem(IDC_BUTTON_UNLOCK)->SetWindowTextW(_T("电机解锁"));
	}
	UpdateState();
}


void CArmControlDlg::OnBnClickedRelative()
{
	// TODO: 在此添加控件通知处理程序代码
	m_bRelative = TRUE;
}


void CArmControlDlg::OnBnClickedAbsolute()
{
	// TODO: 在此添加控件通知处理程序代码
	m_bRelative = FALSE;
}


void CArmControlDlg::OnBnClickedSettingon()
{
	// TODO: 在此添加控件通知处理程序代码
	m_bSettingMode = TRUE;
	GetDlgItem(IDC_BUTTON_COORGO)->SetWindowTextW(_T("设定"));
	GetDlgItem(IDC_BUTTON_GOANGLE)->SetWindowTextW(_T("设定"));
}


void CArmControlDlg::OnBnClickedSettingoff()
{
	// TODO: 在此添加控件通知处理程序代码
	m_bSettingMode = FALSE;
	GetDlgItem(IDC_BUTTON_COORGO)->SetWindowTextW(_T("GO"));
	GetDlgItem(IDC_BUTTON_GOANGLE)->SetWindowTextW(_T("GO"));
}


void CArmControlDlg::OnBnClickedButtonMove()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData();
	m_bLock = TRUE;
	GetDlgItem(IDC_BUTTON_UNLOCK)->SetWindowTextW(_T("电机解锁"));
	Grab(m_moveX, m_moveY, m_moveToX, m_moveToY, m_moveR);
}

void CArmControlDlg::Grab(double x, double y, double des_x, double des_y, double r)
{
	ArmMsg msg;
	msg.msg = AM_MOVETO;
	msg.param.push_back(x);
	msg.param.push_back(y);
	msg.param.push_back(des_x);
	msg.param.push_back(des_y);
	msg.param.push_back(r);
	msgArray.push_back(msg);
}

void CArmControlDlg::SetCoor(double x, double y, double z)
{
	m_pA->SetCoor(x, y, z);
}

void CArmControlDlg::SetAngle(double x, double y, double z)
{
	m_pA->SetAngle(x, y, z);
}

void CArmControlDlg::GoToR(int dx, int dy, int dz)
{
	ArmMsg msg;
	msg.msg = AM_GOTOR;
	msg.param.push_back(dx);
	msg.param.push_back(dy);
	msg.param.push_back(dz);
	msgArray.push_back(msg);
}

void CArmControlDlg::GoTo(int x, int y, int z)
{
	ArmMsg msg;
	msg.msg = AM_GOTO;
	msg.param.push_back(x);
	msg.param.push_back(y);
	msg.param.push_back(z);
	msgArray.push_back(msg);
}

DWORD WINAPI ArmCtrlThreadProc(LPVOID lpParam)
{
	CArmControlDlg * dlg = (CArmControlDlg*)lpParam;
	vector<ArmMsg>::iterator itMsg;
	while (1)
	{
		itMsg = dlg->msgArray.begin();
		if (itMsg != dlg->msgArray.end())
		{
			switch (itMsg->msg)
			{
			case AM_GOTO:
				dlg->m_pA->GoTo(itMsg->param[0], itMsg->param[1], itMsg->param[2]);				
				break;
			case AM_GRAB:
				dlg->m_pA->Grab();
				break;
			case AM_PLACE:
				dlg->m_pA->Disgrab();
				break;
			case AM_GOANGLETO:
				dlg->m_pA->GoAngleTo(itMsg->param[0], itMsg->param[1], itMsg->param[2]);
				break;
			case AM_STEERTO:
				dlg->m_pA->SteerEngineTo(itMsg->param[0]);
				break;
			case AM_MOVETO:
				dlg->m_pA->Grab(itMsg->param[0], itMsg->param[1], itMsg->param[2], 
					itMsg->param[3], itMsg->param[4]);
				break;
			case AM_SEGGOTO:
				dlg->m_pA->GoSegTo(itMsg->param[0], itMsg->param[1], itMsg->param[2]);
				break;
			case AM_GOTOR:
				dlg->m_pA->GoToR(itMsg->param[0], itMsg->param[1], itMsg->param[2]);
				break;
			case AM_GOANGLETOR:
				dlg->m_pA->GoAngleToR(itMsg->param[0], itMsg->param[1], itMsg->param[2]);
				break;
			default:
				break;
			}
			itMsg->param.clear();
			itMsg = dlg->msgArray.erase(itMsg);
			dlg->UpdateState();
		}
		else
			Sleep(10);
	}
	return 0;
}


void CArmControlDlg::OnBnClickedButtonSeggo()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData();
	ArmMsg msg;
	msg.msg = AM_SEGGOTO;
	msg.param.push_back(m_X);
	msg.param.push_back(m_Y);
	msg.param.push_back(m_Z);
	msgArray.push_back(msg);
}
