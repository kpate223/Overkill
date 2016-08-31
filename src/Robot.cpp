#include "WPILib.h"

#include <cmath>
#include <iostream>

/**
 * This is a demo program showing the use of the RobotDrive class.
 * The SampleRobot class is the base of a robot application that will automatically call your
 * Autonomous and OperatorControl methods at the right time as controlled by the switches on
 * the driver station or the field controls.
 *
 * WARNING: While it may look like a good choice to use for your code if you're inexperienced,
 * don't. Unless you know what you are doing, complex code will be much more difficult under
 * this system. Use IterativeRobot or Command-Based instead if you're new.
 */
class Robot: public SampleRobot
{
	RobotDrive myRobot; // robot drive system
	Joystick controller; // only joystick
	Relay powerRelay;
	Victor leftVictor;
	Victor rightVictor;

public:
	Robot() :
			myRobot(0, 2, 3, 5),	// initialize the RobotDrive to use motor controllers on ports 0 and 1
			leftVictor(1),
			rightVictor(4),
			controller(1),
			powerRelay(0, Relay::Direction::kForwardOnly)
	{
		myRobot.SetExpiration(0.1);
		powerRelay.Set(Relay::Value::kForward);
	}

	void RobotInit()
	{
		powerRelay.Set(Relay::Value::kForward);
	}

	void Disabled() override
	{
		powerRelay.Set(Relay::Value::kForward);
	}

	void haloDrive()
	{
		const float maxPower = 1.0f;
		double forwardPower = -controller.GetRawAxis(1);
		double turnPower = -controller.GetRawAxis(4);

		std::cout << "forwardPower: " << forwardPower << std::endl;
		std::cout << "turnPower: " << turnPower << std::endl;

		double leftPower = forwardPower;
		double rightPower = forwardPower;

		const float turnReverseThreshold = 0.25f;
		double turnMod = abs(turnPower);
		double speedMod = 0.8f;

		if(turnMod >= turnReverseThreshold)
		{
			double reverseMod = (turnMod - turnReverseThreshold) / (1 - turnReverseThreshold);
			double reverseSpeed = -1.0;
			speedMod = reverseSpeed * reverseMod;
		}

		else
		{
			speedMod = (1 - turnMod);
		}

		if(turnPower > 0)
		{
			if(forwardPower > 0)
				leftPower *= speedMod;
			else
				rightPower *= speedMod;
		}
		else if(turnPower < 0)
		{
			if(forwardPower > 0)
				rightPower *= speedMod;
			else
				leftPower *= speedMod;
		}

		myRobot.TankDrive(leftPower * maxPower, rightPower * maxPower);
		leftVictor.Set(leftPower * maxPower);
		rightVictor.Set(-rightPower * maxPower);
	}

	/**
	 * Runs the motors with arcade steering.
	 */
	void OperatorControl()
	{
		while (IsOperatorControl() && IsEnabled())
		{
			haloDrive();
			Wait(0.005);				// wait for a motor update time
		}
	}





};

START_ROBOT_CLASS(Robot)
