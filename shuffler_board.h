#pragma once

#include <cstdint>
#include "io_expander.h"

#define BV(b) (1<<(b))

static constexpr int IO_STEPPER_EN1 = 0;
static constexpr int IO_STEPPER_STEP1 = 1;
static constexpr int IO_STEPPER_DIR1 = 2;
static constexpr int IO_ENDSTOP = 7;
static constexpr int IO_IOE_INT = 10;
static constexpr int IO_I2C_SDA = 4;
static constexpr int IO_I2C_SCL = 5;

static constexpr int IO_ROT_DAT = 3;
static constexpr int IO_ROT_CLK = 9;
static constexpr int IO_ROT_BUT = 8;
static constexpr int IOMASK_ROT_DAT = BV(0);
static constexpr int IOMASK_ROT_CLK = BV(1);
static constexpr int IOMASK_ROT_BUT = BV(2);

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
uint16_t read_rotary(void);
void scan_twi(void);