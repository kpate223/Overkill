/*
 * DozersPins.h
 *
 *  Created on: Aug 18, 2016
 *      Author: Agoston
 */

#ifndef SRC_DOZERPINS_H_
#define SRC_DOZERPINS_H_

#include <type_traits>

enum Motors
{
	RightFrontMotor = 0,
	RightMiddleMotor,
	RightRearMotor,
	LeftFrontMotor,
	LeftMiddleMotor,
	LeftRearMotor,
	NUM_MOTORS,
};

const uint32_t drivePins[Motors::NUM_MOTORS] =
{
	5,
	4,
	3,
	2,
	1,
	0
};

const uint32_t encoderPins[Motors::NUM_MOTORS][2] =
{
	{0, 1},
	{2, 3},
	{4, 5},
	{6, 7},
	{8, 9},
	{10, 11}
};


#endif /* SRC_DOZERPINS_H_ */
