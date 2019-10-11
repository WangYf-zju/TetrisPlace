#pragma once

#include "WzSerialPort.h"
#define GRAB_Z	25.0
#define DISGRAB_Z 5.0
#define MOVE_Z	40.0
#define SERIAL_PORT 3
#define ARM_SPEED 15 // degree per second
#define STEERING_SPEED 180


class Arm
{
public:
	Arm();
	~Arm();
	void Grab();
	void Grab(double x, double y, double des_x, double des_y, double r = 0);
	void Disgrab();
	void Disgrab(double x, double y, double r = 0);
	double GoTo(double x, double y, double z = MOVE_Z);
	double GoSegTo(double x, double y, double z = MOVE_Z);
	double SteerEngineTo(double r);

private:
	void InitAngle();
	double RadToDeg(double rad);
	double calc_rad_angle_in_triangle_by_cosine_law(double adj_side1, double adj_side2, double opp_side);
	void Calc_Angle(double x, double y, double z);


public:
	static const enum startCoor { startX = 133, startY = -100, startZ = -106 };
	double m_coor[3];
	double m_cAngle[3];
	double m_steerAngle;
private:
	WzSerialPort * m_w;
	double m_angle[3];
	double m_dAngle[3];
};

