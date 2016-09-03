/*
 * DozerDrive.cpp
 *
 *  Created on: Aug 4, 2016
 *      Author: Agoston
 */

#include <Drive.h>

Drive::Drive(Joystick *joystick)
{
	for(unsigned i = 0; i < DriveMotors::NUM_DRIVE_MOTORS; ++i)
	{
		motorControllers[i] = std::make_shared<Victor>(driveMotorPins[i]);
		encoders[i] = std::make_shared<Encoder>(driveEncoderPins[i][0], driveEncoderPins[i][1]);
	}

	lastRunTimestamp = getTimestampMicros() - drivePeriod;

	for(unsigned i = 0; i < DriveMotors::NUM_DRIVE_MOTORS; ++i)
	{
		lastPowerVals[i] = 0;
		lastEncoderVals[i] = 0;
	}

	this->joystick = joystick;
}

Drive::~Drive()
{
	// TODO Auto-generated destructor stub
}

void Drive::update()
{
	uint32_t timestampMicros = getTimestampMicros();

	if(timestampMicros - lastRunTimestamp < drivePeriod)
		return;

	lastRunTimestamp = timestampMicros;

	float forwardSpeed = -joystick->GetRawAxis(JoystickAxes::DriveForward);
	float turnSpeed = joystick->GetRawAxis(JoystickAxes::DriveTurn);

	if(abs(forwardSpeed) < 0.05)
		forwardSpeed = 0;

	if(abs(turnSpeed) < 0.05)
		turnSpeed = 0;

	float leftSpeed = constrain(forwardSpeed + turnSpeed, -1, 1);
	float rightSpeed = -constrain(forwardSpeed - turnSpeed, -1, 1);

	bool halfSpeed = joystick->GetRawButton(JoystickButtons::DriveHalfSpeed);

	if(halfSpeed)
	{
		leftSpeed = map(leftSpeed, -1, 1, -maxEncoderVal / 2, maxEncoderVal / 2);
		rightSpeed = map(rightSpeed, -1, 1, -maxEncoderVal / 2, maxEncoderVal / 2);
	}
	else
	{
		leftSpeed = map(leftSpeed, -1, 1, -maxEncoderVal, maxEncoderVal);
		rightSpeed = map(rightSpeed, -1, 1, -maxEncoderVal, maxEncoderVal);
	}

	std::cout << "leftSpeed " << leftSpeed << std::endl;
	std::cout << "rightSpeed" << rightSpeed << std::endl;

	int encoderVals[DriveMotors::NUM_DRIVE_MOTORS];
	int motorSpeeds[DriveMotors::NUM_DRIVE_MOTORS];

	for(unsigned i = 0; i < DriveMotors::NUM_DRIVE_MOTORS; ++i)
	{
		encoderVals[i] = encoders[i]->GetRaw();
//		std::cout << "enc[" << i << "] = " << encoderVals[i] << std::endl;
	}

	float speedErrorValues[DriveMotors::NUM_DRIVE_MOTORS];

	for(unsigned i = 0; i < DriveMotors::NUM_DRIVE_MOTORS; ++i)
	{
		motorSpeeds[i] = encoderVals[i] - lastEncoderVals[i];
//		std::cout << "speed[" << i << "] = " << motorSpeeds[i] << std::endl;

		lastEncoderVals[i] = encoderVals[i];

		if(i <= DriveMotors::RightRearMotor)
		{
			speedErrorValues[i] = rightSpeed - motorSpeeds[i];
		}
		else
		{
			speedErrorValues[i] = leftSpeed - motorSpeeds[i];
		}
	}

	if(!joystick->GetRawButton(JoystickButtons::DriveOverride))
	{

	}

	for(unsigned i = 0; i < DriveMotors::NUM_DRIVE_MOTORS; ++i)
	{
		lastPowerVals[i] += speedErrorValues[i] * kIntegral;
		lastPowerVals[i] = constrain(lastPowerVals[i], -1, 1);
		motorControllers[i]->Set(lastPowerVals[i]);
	}
}

void Drive::reset()
{
	for(unsigned i = 0; i < DriveMotors::NUM_DRIVE_MOTORS; ++i)
	{
		lastPowerVals[i] = 0;
	}
}
