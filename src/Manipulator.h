/*
 * Manipulator.h
 *
 *  Created on: Aug 11, 2016
 *      Author: Agoston
 */

#ifndef SRC_MANIPULATOR_H_
#define SRC_MANIPULATOR_H_

#include "WPILib.h"

class Manipulator
{
		// 8 motors
	public:
		Manipulator();
		virtual ~Manipulator();

	protected:

	private:
		std::shared_ptr<Victor> baseMotor;
		std::shared_ptr<Victor> shoulderMotor1;
		std::shared_ptr<Victor> shoulderMotor2;
		std::shared_ptr<Victor> elbowMotor;
		std::shared_ptr<Victor> pitchMotor;
		std::shared_ptr<Victor> yawMotor;
		std::shared_ptr<Victor> rollMotor;
		std::shared_ptr<Victor> gripperMotor;

		std::shared_ptr<AnalogPotentiometer> basePotentiometer;
		std::shared_ptr<AnalogPotentiometer> shoulderPotentiometer;
		std::shared_ptr<AnalogPotentiometer> elbowPotentiometer;
		std::shared_ptr<AnalogPotentiometer> pitchPotentiometer;
		std::shared_ptr<AnalogPotentiometer> yawPotentiometer;
		std::shared_ptr<AnalogPotentiometer> rollPotentiometer;
		std::shared_ptr<AnalogPotentiometer> gripperPotentiometers;
};

#endif /* SRC_MANIPULATOR_H_ */
