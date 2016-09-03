#include <Constants.h>
#include <Drive.h>
#include <Manipulator.h>
#include <Safety.h>

class Robot: public SampleRobot
{
	Joystick controller;
	Drive drive;
	Manipulator manipulator;
	Safety safety;

public:
	Robot() :
			controller(1),
			drive(&controller),
			manipulator(&controller),
			safety(&controller)
	{
	}

	void RobotInit()
	{
	}

	void Disabled()
	{
		drive.reset();
	}

	void OperatorControl()
	{
		while (IsOperatorControl() && IsEnabled())
		{
			safety.update();
			drive.update();
			//manipulator.update();
		}
	}
};

START_ROBOT_CLASS(Robot)
