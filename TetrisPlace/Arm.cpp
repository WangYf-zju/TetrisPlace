#include "Arm.h"
#include <string.h>
#include <stdio.h>
#include <math.h>
#include <Windows.h>
const double PI = atan(1) * 4;

#define X	0
#define Y	1
#define Z	2

Arm::Arm()
{
	for (int i = 0; i < 3; i++)
	{
		m_coor[i] = 0;
		m_cAngle[i] = 0;
		m_dAngle[i] = 0;
		m_angle[i] = 0;
	}
	m_w = new WzSerialPort;

	try
	{
		m_w->open(SERIAL_PORT, 115200, 0, 8, 1, 1);
		char initBuff[100] = 
			"G93 X0 Y0 Z0\r\nG95\r\nG1 X0 Y0 Z0\r\nM1004 R0\r\nM2005\r\nM1006\r\n";
		m_w->send(initBuff, strlen(initBuff));
	}
	catch (...)
	{
		printf("Cannot Open Port\n");
		exit(-1);
	}
	m_steerAngle = 0;
	InitAngle();
	double duration = GoSegTo(40, 60);
	Sleep(duration);
}


Arm::~Arm()
{
	delete m_w;
}


void Arm::Grab()
{
	char buff[100] = { 0 };
	sprintf_s(buff, "M2006\r\nM1005\r\n");
	m_w->send(buff, strlen(buff));
	double duration = GoTo(m_coor[X], m_coor[Y], GRAB_Z);
	Sleep((int)duration);
}

void Arm::Grab(double x, double y, double des_x, double des_y, double r/*=0*/)
{
	Calc_Angle(x, y, MOVE_Z);
	double angleZ1 = m_angle[Z];
	Calc_Angle(des_x, des_y, MOVE_Z);
	double dAngleZ = m_angle[Z] - angleZ1;
	double dSteerAngle = r - dAngleZ;
	if (dSteerAngle > 180)dSteerAngle -= 360;
	else if (dSteerAngle < -180)dSteerAngle += 360;

	double duration1 = 0;
	if (dSteerAngle < 0) duration1 = SteerEngineTo(180);
	else duration1 = SteerEngineTo(0);
	double duration2 = GoSegTo(x, y);
	double duration = (duration1 > duration2 ? duration1 : duration2);
	Sleep((int)duration);
	Grab();
	Disgrab(des_x, des_y, dSteerAngle);
}

void Arm::Disgrab()
{
	double duration = GoTo(m_coor[X], m_coor[Y], DISGRAB_Z);
	Sleep(duration);
	char buff[100] = { 0 };
	sprintf_s(buff, "M2005\r\nM1006\r\n");
	m_w->send(buff, strlen(buff));
}

void Arm::Disgrab(double x, double y, double r/*=0*/)
{
	double duration1 = GoSegTo(x, y);
	Sleep(1000);
	double duration2 = SteerEngineTo(m_steerAngle + r);
	double duration = (duration1-1000 > duration2 ? duration1-1000 : duration2);
	Sleep((int)duration);
	Disgrab();
}

double Arm::GoTo(double x, double y, double z/*=MOVE_Z*/)
{
	Calc_Angle(x + startX, y + startY, z + startZ);
	m_dAngle[X] = m_angle[X] - m_cAngle[X];
	m_dAngle[Y] = m_angle[Y] - m_cAngle[Y];
	m_dAngle[Z] = m_angle[Z] - m_cAngle[Z];
	double duration = 0;
	duration = (fabs(m_dAngle[X]) > fabs(m_dAngle[Y]) ? 
		fabs(m_dAngle[X]) / ARM_SPEED : fabs(m_dAngle[Y]) / ARM_SPEED);
	duration = (fabs(m_dAngle[Z]) / ARM_SPEED > duration ? 
		fabs(m_dAngle[Z]) / ARM_SPEED : duration);
	if (m_dAngle[Z] > 180)m_dAngle[Z] -= 360;
	else if (m_dAngle[Z] < -180)m_dAngle[Z] += 360;

	m_cAngle[X] = m_angle[X];
	m_cAngle[Y] = m_angle[Y];
	m_cAngle[Z] = m_angle[Z];
	char buff[100] = { 0 };
	sprintf_s(buff, "G1 X%.6f Y%.6f Z%.6f\r\n", 
		m_dAngle[X], m_dAngle[Y], m_dAngle[Z]);
	m_w->send(buff, strlen(buff));
	m_coor[X] = x;
	m_coor[Y] = y;
	m_coor[Z] = z;
	return duration*1000;
}

double Arm::GoSegTo(double x, double y, double z/*=MOVE_Z*/)
{
	double duration = 0;
	duration += GoTo(m_coor[X], m_coor[Y]);
	duration += GoTo(x, y);
	duration += GoTo(x, y, z);
	return duration;
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

void Arm::InitAngle()
{
	Calc_Angle(startX, startY, startZ);
	m_cAngle[X] = m_angle[X];
	m_cAngle[Y] = m_angle[Y];
	m_cAngle[Z] = m_angle[Z];
}

double Arm::RadToDeg(double rad)
{
	return rad / PI * 180;
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
	double Xf = x, Yf = y, Zf = z;
	const double OA = 30; const double BC = 30; const double AB = 135; const double OC = 135;
	const double BD = 190; const double DE = 75; const double EF = 60;
	m_angle[Z] = RadToDeg(atan(Yf / Xf));
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
	m_angle[X] = RadToDeg(ang_COX);

	double ang_AOB = calc_rad_angle_in_triangle_by_cosine_law(OA, OB, AB);
	double ang_AOX = ang_BOD + ang_DOX + ang_AOB;
	m_angle[Y] = RadToDeg(ang_AOX);
}
#undef X
#undef Y
#undef Z