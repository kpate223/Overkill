/*
 * DozerDrive.h
 *
 *  Created on: Aug 4, 2016
 *      Author: Agoston
 */

#ifndef SRC_DOZERDRIVE_H_
#define SRC_DOZERDRIVE_H_

#include "WPILib.h"
#include <Timer.h>

#include <memory>
#include <type_traits>

#include <DozerPins.h>

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

class DozerDrive : public ErrorBase, public MotorSafety
{
	public:


		/**
		 * Constructor for the RobotDrive.
		 * @param frontRight PWM port for the front right motor controller
		 * @param frontLeft PWM port for the front left motor controller
		 * @param middleRight PWM port for the middle left motor controller
		 * @param middleLeft PWM port for the middle right motor controller
		 * @param rearRight PWM port for the rear right motor controller
		 * @param rearLeft PWM port for the rear left motor controller
		 */
		DozerDrive(Joystick &joystick, const Timer &timer);
		virtual ~DozerDrive();

		DozerDrive(const DozerDrive &drive) = delete;
		DozerDrive& operator=(const DozerDrive &drive) = delete;

		void updateMotors(const Timer &time);
		float clip(float n, float lower, float upper);

	private:
		void initDrive();
		/**
		 * Limit motor values to the -1.0 to +1.0 range.
		 */
		float Limit(float num);

		float map(float x, float in_min, float in_max, float out_min, float out_max);

		std::shared_ptr<Victor> motorControllers[Motors::NUM_MOTORS];
		std::shared_ptr<Encoder> encoders[Motors::NUM_MOTORS];
		uint32_t lastEncoderVals[Motors::NUM_MOTORS];
		float lastPowerVals[Motors::NUM_MOTORS];
		float accumulators[Motors::NUM_MOTORS];

		const float maxEncoderVal = 125;

		bool override;

		const double kProportional = 1;
		const double kIntegral = 1;

		/**
		 * Last time the motors were updated.
		 */
		double lastRunTime;

		float sensitivity = 0.5;
		double m_maxOutput = 1.0;

		static const int32_t kMaxNumberOfMotors = 6;
		int32_t invertedMotors[kMaxNumberOfMotors] = {1,1,1,1,1,1};

		Joystick *joystick;
};

#endif /* SRC_DOZERDRIVE_H_ */
