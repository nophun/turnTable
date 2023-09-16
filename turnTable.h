#pragma once

#define STEPPER1_STEPS        200
#define STEPPER1_MICROSTEPS   8
#define STEPPER1_RPM          200

#define STEPPER2_STEPS        200
#define STEPPER2_MICROSTEPS   1
#define STEPPER2_RPM          100

#define STEPPER_ACCEL         1000
#define STEPPER_DECEL         1000

#define IOE_ADDRESS           0x20
#define OLED_ADDRESS          0x3C

#define LEDS                  1

#define LONG_PRESS_TIME       2000 /* ms */

#define TABLE_REDUCTION 		1.0F
#define RPM_TO_DEGREE_PER_SEC 	0.166666F
