#pragma once

#include <cstdint>
#include "io_expander.h"

#define BV(b) (1<<(b))

#define IO_STEPPER_EN1   0
#define IO_STEPPER_STEP1 1
#define IO_STEPPER_DIR1  2
#define IO_STEPPER_EN2   3
#define IO_STEPPER_STEP2 9
#define IO_STEPPER_DIR2  8
#define IO_ENDSTOP       7
#define IO_IOE_INT       10
#define IO_I2C_SDA       4
#define IO_I2C_SCL       5

#define IOE_MOTOR        BV(0)
#define IOE_BUZZER       BV(1)
#define IOE_FEEDER_SNS   BV(2)
#define IOE_SENSOR2      BV(3)
#define IOE_SENSOR3      BV(4)
#define IOE_BUT_A        BV(5)
#define IOE_BUT_B        BV(6)
#define IOE_BUT_C        BV(7)
#define IOE_SWITCH2      BV(8)
#define IOE_SWITCH1      BV(9)
#define IOE_M2MS3        BV(10)
#define IOE_M2MS2        BV(11)
#define IOE_M2MS1        BV(12)
#define IOE_M1MS3        BV(13)
#define IOE_M1MS2        BV(14)
#define IOE_M1MS1        BV(15)

void init_io(void);
void init_ioe(void);
void init_steppers(uint8_t microsteps1, uint8_t microsteps2);
extern void read_IOE(void);
void scan_twi(void);