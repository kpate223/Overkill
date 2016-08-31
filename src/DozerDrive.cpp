/*
 * DozerDrive.cpp
 *
 *  Created on: Aug 4, 2016
 *      Author: Agoston
 */

#include <DozerDrive.h>

#include "WPILib.h"
#include "HAL/HAL.hpp"

#include <DozerPins.h>

DozerDrive::DozerDrive(Joystick &joystick, const Timer &timer)
{
	initDrive();

	override = false;

	for(unsigned i = 0; i < Motors::NUM_MOTORS; ++i)
	{
		motorControllers[i] = std::make_shared<Victor>(drivePins[i]);
		encoders[i] = std::make_shared<Encoder>(encoderPins[i][0], encoderPins[i][1]);
	}

	lastRunTime = timer.GetFPGATimestamp();

	for(unsigned i = 0; i < NUM_MOTORS; ++i)
	{
		lastPowerVals[i] = 0;
		accumulators[i] = 0;
		lastEncoderVals[i] = 0;
	}

	this->joystick = &joystick;
}

DozerDrive::~DozerDrive()
{
	// TODO Auto-generated destructor stub
}

float DozerDrive::clip(float n, float lower, float upper)
{
	return std::max(lower, std::min(n, upper));
}

void DozerDrive::updateMotors(const Timer &timer)
{
	// update every 50 seconds
	if(lastRunTime - (timer.GetFPGATimestamp() * 1000) < 50)
		return;

	lastRunTime = timer.GetFPGATimestamp() * 1000;

	float forwardSpeed = joystick->GetRawAxis(1);
	float turnSpeed = joystick->GetRawAxis(4);

	float leftSpeed = clip(forwardSpeed + turnSpeed, -1, 1);
	float rightSpeed = clip(forwardSpeed - turnSpeed, -1, 1);

	bool halfSpeed = joystick->GetRawButton(1);

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

	int encoderVals[Motors::NUM_MOTORS];
	int motorSpeeds[Motors::NUM_MOTORS];

	for(unsigned i = 0; i < Motors::NUM_MOTORS; ++i)
	{
		encoderVals[i] = encoders[i]->GetRaw();

	}

	float speedErrorValues[NUM_MOTORS];

	for(unsigned i = 0; i < Motors::NUM_MOTORS; ++i)
	{
		motorSpeeds[i] = lastEncoderVals[i] - encoderVals[i];
		lastEncoderVals[i] = encoderVals[i];

		if(i <= Motors::RightRearMotor)
		{
			speedErrorValues[i] = rightSpeed - motorSpeeds[i];
		}
		else
		{
			speedErrorValues[i] = leftSpeed - motorSpeeds[i];
		}
	}

	if(!override)
	{

	}

	for(unsigned i = 0; i < Motors::NUM_MOTORS; ++i)
	{
		accumulators[i] += speedErrorValues[i] * kIntegral;
	}

	for(unsigned i = 0; i < Motors::NUM_MOTORS; ++i)
	{
		lastPowerVals[i] = accumulators[i] + speedErrorValues[i] * kProportional;
		lastPowerVals[i] = clip(lastPowerVals[i], -1, 1);
		motorControllers[i]->Set(lastPowerVals[i]);
	}
}

float DozerDrive::map(float x, float in_min, float in_max, float out_min, float out_max)
{
	return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

void DozerDrive::initDrive()
{

}
