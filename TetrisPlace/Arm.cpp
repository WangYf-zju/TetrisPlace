#include "stdafx.h"
#include "Arm.h"
#include <string.h>
#include <stdio.h>
#include <math.h>
#include <Windows.h>
const double PI = atan(1) * 4;

#define X	0
#define Y	1
#define Z	2

Arm::Arm(WzSerialPort * w)
{
	for (int i = 0; i < 3; i++)
	{
		m_cCoor[i] = 0;
		m_cAngle[i] = 0;
		m_dAngle[i] = 0;
		m_tAngle[i] = 0;
		m_tCoor[i] = 0;
	}
	m_steerAngle = 0;
	m_w = w;
	InitAngle();

	if (w != nullptr)
	{
		InitArm();
	}
}


Arm::~Arm()
{
	delete m_w;
}

void Arm::BindSerialPort(WzSerialPort * w, BOOL bInit)
{
	m_w = w;
	if (bInit)
		InitArm();
}

void Arm::InitArm()
{
	InitAngle();
	m_steerAngle = 0;
	try
	{
		char initBuff[100] =
			"G95\r\nM1004 R0\r\nM2005\r\nM1006\r\n";
		m_w->send(initBuff, strlen(initBuff));
	}
	catch (...)
	{
		exit(-1);
	}
}


void Arm::Grab()
{
	OpenPump();
	double duration = GoTo(m_cCoor[X], m_cCoor[Y], GRAB_Z);
	Sleep((int)duration);
}

void Arm::Grab(double x, double y, double des_x, double des_y, double r/*=0*/)
{
	Calc_Angle(x, y, MOVE_Z);
	double angleZ1 = m_tAngle[Z];
	Calc_Angle(des_x, des_y, MOVE_Z);
	double dAngleZ = angleZ1 - m_tAngle[Z] ;
	double dSteerAngle = r - dAngleZ;
	if (dSteerAngle > 180)dSteerAngle -= 360;
	else if (dSteerAngle < -180)dSteerAngle += 360;

	double duration = 0;
	if (dSteerAngle < 0) duration = SteerEngineTo(180);
	else duration = SteerEngineTo(0);
	if (duration > 0)
		Sleep((int)duration);
	GoSegTo(x, y);
	Grab();
	GoSegTo(OFFSETX2, OFFSETY2);
	Disgrab(des_x, des_y, dSteerAngle);
}

void Arm::Disgrab()
{
	double duration = GoTo(m_cCoor[X], m_cCoor[Y], DISGRAB_Z);
	if (duration > 0)
		Sleep(duration);
	ClosePump();
}

void Arm::Disgrab(double x, double y, double r/*=0*/)
{
	GoSegTo(x, y);
	double duration = SteerEngineTo(m_steerAngle + r);
	if (duration > 0)
		Sleep((int)duration);
	Disgrab();
}

double Arm::GoTo(double x, double y, double z/*=MOVE_Z*/)
{
	Calc_Angle(x, y, z);
	m_dAngle[X] = m_tAngle[X] - m_cAngle[X];
	m_dAngle[Y] = m_tAngle[Y] - m_cAngle[Y];
	m_dAngle[Z] = m_tAngle[Z] - m_cAngle[Z];
	double duration = 0;
	duration = (fabs(m_dAngle[X]) > fabs(m_dAngle[Y]) ? 
		fabs(m_dAngle[X]) / ARM_SPEED : fabs(m_dAngle[Y]) / ARM_SPEED);
	duration = (fabs(m_dAngle[Z]) / ARM_SPEED > duration ? 
		fabs(m_dAngle[Z]) / ARM_SPEED : duration);
	if (m_dAngle[Z] > 180)m_dAngle[Z] -= 360;
	else if (m_dAngle[Z] < -180)m_dAngle[Z] += 360;

	m_cAngle[X] = m_tAngle[X];
	m_cAngle[Y] = m_tAngle[Y];
	m_cAngle[Z] = m_tAngle[Z];
	char buff[100] = { 0 };
	sprintf_s(buff, "G1 X%.6f Y%.6f Z%.6f\r\n", 
		m_dAngle[X], m_dAngle[Y], m_dAngle[Z]);
	m_w->send(buff, strlen(buff));
	m_cCoor[X] = x;
	m_cCoor[Y] = y;
	m_cCoor[Z] = z;
	return duration*1000;
}

void Arm::GoToR(double dx, double dy, double dz)
{
	double duration = GoTo(m_cCoor[X] + dx, m_cCoor[Y] + dy, m_cCoor[Z] + dz);
	if (duration > 0)
		Sleep(duration);
}

void Arm::GoSegTo(double x, double y, double z/*=MOVE_Z*/)
{
	double duration = 0;
	duration = GoTo(m_cCoor[X], m_cCoor[Y]);
	if (duration > 0)
		Sleep((int)duration);
	duration = GoTo(x, y);
	if (duration > 0)
		Sleep((int)duration);
	duration = GoTo(x, y, z);
	if (duration > 0)
		Sleep((int)duration);
}

double Arm::GoAngleTo(double x, double y, double z)
{
	m_dAngle[X] = x - m_cAngle[X];
	m_dAngle[Y] = y - m_cAngle[Y];
	m_dAngle[Z] = z - m_cAngle[Z];
	double duration = 0;
	duration = (fabs(m_dAngle[X]) > fabs(m_dAngle[Y]) ?
		fabs(m_dAngle[X]) / ARM_SPEED : fabs(m_dAngle[Y]) / ARM_SPEED);
	duration = (fabs(m_dAngle[Z]) / ARM_SPEED > duration ?
		fabs(m_dAngle[Z]) / ARM_SPEED : duration);
	if (m_dAngle[Z] > 180)m_dAngle[Z] -= 360;
	else if (m_dAngle[Z] < -180)m_dAngle[Z] += 360;

	m_cAngle[X] = x;
	m_cAngle[Y] = y;
	m_cAngle[Z] = z;
	char buff[100] = { 0 };
	sprintf_s(buff, "G1 X%.6f Y%.6f Z%.6f\r\n",
		m_dAngle[X], m_dAngle[Y], m_dAngle[Z]);
	m_w->send(buff, strlen(buff));
	
	Calc_Coor(x, y, z);
	m_cCoor[X] = m_tCoor[X];
	m_cCoor[Y] = m_tCoor[Y];
	m_cCoor[Z] = m_tCoor[Z];
	return duration;
}

void Arm::GoAngleToR(double dx, double dy, double dz)
{
	double duration = GoAngleTo(dx + m_cAngle[X], dy + m_cAngle[Y], dz + m_cAngle[Z]);
	if (duration > 0)
		Sleep(duration);
}

double Arm::SteerEngineTo(double r)
{
	char buff[100] = { 0 };
	sprintf_s(buff, "M1004 R%.1f\r\n", r);
	m_w->send(buff, strlen(buff));
	double duration = fabs(m_steerAngle - r) / STEERING_SPEED;
	m_steerAngle = r;
	return duration*1000;
}

void Arm::SetCoor(double x, double y, double z)
{
	m_cCoor[X] = x;
	m_cCoor[Y] = y;
	m_cCoor[Z] = z;
	Calc_Angle(x, y, z);
	m_cAngle[X] = m_tAngle[X];
	m_cAngle[Y] = m_tAngle[Y];
	m_cAngle[Z] = m_tAngle[Z];
}

void Arm::SetAngle(double x, double y, double z)
{
	m_cAngle[X] = x;
	m_cAngle[Y] = y;
	m_cAngle[Z] = z;
	Calc_Coor(x, y, z);
	m_cCoor[X] = m_tCoor[X];
	m_cCoor[Y] = m_tCoor[Y];
	m_cCoor[Z] = m_tCoor[Z];
}

void Arm::OpenPump()
{
	char buff[100] = { 0 };
	sprintf_s(buff, "M2006\r\nM1005\r\n");
	m_w->send(buff, strlen(buff));
}

void Arm::ClosePump()
{
	char buff[100] = { 0 };
	sprintf_s(buff, "M2005\r\nM1006\r\n");
	m_w->send(buff, strlen(buff));
}

void Arm::UnlockMotor()
{
	char buff[100] = { 0 };
	sprintf_s(buff, "M84\r\n");
	m_w->send(buff, strlen(buff));
}

void Arm::LockMotor()
{
	GoAngleToR(0.1, 0, 0);
	GoAngleToR(-0.1, 0, 0);
}

void Arm::InitAngle()
{
	Calc_Angle(0, 0, 0);
	m_cAngle[X] = m_tAngle[X];
	m_cAngle[Y] = m_tAngle[Y];
	m_cAngle[Z] = m_tAngle[Z];
}

double Arm::RadToDeg(double rad)
{
	return rad / PI * 180.0;
}

double Arm::DegToRad(double deg)
{
	return deg / 180.0 * PI;
}

double Arm::calc_rad_angle_in_triangle_by_cosine_law(double adj_side1, double adj_side2, double opp_side)
{
	double angle = acos((adj_side1*adj_side1 + adj_side2 * adj_side2 - opp_side * opp_side) / (2 * adj_side1*adj_side2));
	return angle >= 0 ? angle : angle + PI;
}

//        B
//y^     / \
// |    /   C
// |   /   / \
// |  /   /   D-----E
// | /   /          |
// |A   /           F
// | \ /
// ---O--------------------->X axis
void Arm::Calc_Angle(double x, double y, double z)
{
	double Xf = x + startX, Yf = y + startY, Zf = z + startZ;
	const double OA = 30; const double BC = 30; const double AB = 135; const double OC = 135;
	const double BD = 190; const double DE = 75; const double EF = 60;
	m_tAngle[Z] = RadToDeg(atan(Yf / Xf));
	double Xd = Xf - DE * Xf / sqrt(Xf * Xf + Yf * Yf);
	double Yd = Yf - DE * Yf / sqrt(Xf * Xf + Yf * Yf);
	double Zd = Zf + EF;
	double OD = sqrt(Xd * Xd + Yd * Yd + Zd * Zd);
	// Using Cosine Law in triangle OBC & triangle OBD
	// ］OBC = ］OBD = (OB^2+BD^2-OD^2)/(2，OB，BD) = (OB^2+BC^2-OC^2)/(2，OB，BC)
	double OB = sqrt(((BC*BC - OC * OC) / BC - (BD*BD - OD * OD) / BD) / (1 / BD - 1 / BC));

	double ang_BOD = calc_rad_angle_in_triangle_by_cosine_law(OB, OD, BD);
	double ang_BOC = calc_rad_angle_in_triangle_by_cosine_law(OB, OC, BC);
	double ang_DOX = atan(Zd / sqrt(Xd * Xd + Yd * Yd));
	double ang_COX = ang_BOD + ang_DOX - ang_BOC;
	m_tAngle[X] = RadToDeg(ang_COX);

	double ang_AOB = calc_rad_angle_in_triangle_by_cosine_law(OA, OB, AB);
	double ang_AOX = ang_BOD + ang_DOX + ang_AOB;
	m_tAngle[Y] = RadToDeg(ang_AOX);
}
//        B
//y^     / \
// |    /   C
// |   /   / \
// |  /   /   D-----E
// | /   /          |
// |A   /           F
// | \ /
// ---O--------------------->X axis

void Arm::Calc_Coor(double ax, double ay, double az)
{
	double ang_COX = DegToRad(ax);
	double ang_AOX = DegToRad(ay);
	double ang_Z = DegToRad(az);
	const double OA = 30; const double BC = 30; const double AB = 135; const double OC = 135;
	const double BD = 190; const double DE = 75; const double EF = 60;
	double Xa = OA * cos(ang_AOX); double Ya = OA * sin(ang_AOX);
	double Xc = OC * cos(ang_COX); double Yc = OC * sin(ang_COX);
	double Xb = Xa + Xc; double Yb = Ya + Yc;
	double Xd = Xb + (Xc - Xb) / BC * BD;
	double Yd = Yb + (Yc - Yb) / BC * BD;
	double Xf = Xd + DE; double Yf = Yd - EF;
	m_tCoor[X] = cos(ang_Z) * Xf - startX;
	m_tCoor[Y] = sin(ang_Z) * Xf - startY;
	m_tCoor[Z] = Yf - startZ;
}

#undef X
#undef Y
#undef Z