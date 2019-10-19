#pragma once

#include "third-party/WzSerialPort.h"
#define GRAB_Z	20.0
#define DISGRAB_Z 5.0
#define MOVE_Z	40.0
#define ARM_SPEED 12 // degree per second
#define STEERING_SPEED 180


#define OFFSETX1	-13
#define OFFSETY1	75
#define OFFSETX2	-33.5
#define OFFSETY2	270

class Arm
{
public:
	Arm(WzSerialPort * w = nullptr);
	~Arm();
	void BindSerialPort(WzSerialPort * w, BOOL bInit = FALSE);
	void InitArm();
	void Grab();
	void Grab(double x, double y, double des_x, double des_y, double r = 0);
	void Disgrab();
	void Disgrab(double x, double y, double r = 0);
	double GoTo(double x, double y, double z = MOVE_Z);
	void GoToR(double dx, double dy, double dz);
	void GoSegTo(double x, double y, double z = MOVE_Z);
	double GoAngleTo(double x, double y, double z);
	void GoAngleToR(double dx, double dy, double dz);
	double SteerEngineTo(double r);
	void SetCoor(double x, double y, double z);
	void SetAngle(double x, double y, double z);
	void OpenPump();
	void ClosePump();
	void UnlockMotor();
	void LockMotor();

private:
	void InitAngle();
	double ArmRadToDeg(double rad);
	double ArmDegToRad(double deg);
	double calc_rad_angle_in_triangle_by_cosine_law(double adj_side1, double adj_side2, double opp_side);
	void Calc_Angle(double x, double y, double z);
	void Calc_Coor(double x, double y, double z);


public:
	static const enum startCoor { startX = 140, startY = -102, startZ = -105 };
	double m_cCoor[3];
	double m_cAngle[3];
	double m_steerAngle;
private:
	WzSerialPort * m_w;
	double m_tAngle[3];
	double m_dAngle[3];
	double m_tCoor[3];
};

