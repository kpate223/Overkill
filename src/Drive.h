/*
 * DozerDrive.h
 *
 *  Created on: Aug 4, 2016
 *      Author: Agoston
 */

#ifndef SRC_DRIVE_H_
#define SRC_DRIVE_H_

#include <Constants.h>

/*
 * Encoder pulses per revolution = 7
 * x4 = 28
 * Gear ratio = 71
 * Pulses per rotation of output shaft = 28 * 71 = 1988
 * Wheel circumference = 0.67 m
 * Pulses per meter = 2970 pulses/m
 * By measuring pulses from encoder, we can determine speed and distance.
 *
 * Motor no load speed = 75 rpm
 * Wheel no load velocity = (75 / 60) * 0.67 m = 0.84 m/s <- MAX SPEED = 2500 pulses/s
 * Most likely velocity is 0.6 m/s
 *
 */

class Drive
{
	public:
		Drive(Joystick *controller);
		virtual ~Drive();
		void update();
		void reset();

	private:
		std::shared_ptr<Victor> motorControllers[DriveMotors::NUM_DRIVE_MOTORS];
		std::shared_ptr<Encoder> encoders[DriveMotors::NUM_DRIVE_MOTORS];

		uint32_t lastEncoderVals[DriveMotors::NUM_DRIVE_MOTORS];

		float lastPowerVals[DriveMotors::NUM_DRIVE_MOTORS];

		const float maxEncoderVal = 125;

		const double kIntegral = 0.002;

		uint32_t lastRunTimestamp;

		Joystick *joystick;
};

#endif /* SRC_DRIVE_H_ */
