#include <Arduino.h>
#include <Wire.h>
#include "shuffler_board.h"
#include "oled.h"
#include "rotaryEncoder.h"
#include "tableMotor.h"

#define SYS_VOL   3.3
OLED oled(OLED_ADDRESS, 5*60);
RotaryEncoder rotary(RotaryMode::FULL_STEP);
TableMotor table;

void init_io(void) {
  pinMode(IO_IOE_INT, INPUT_PULLUP);
  pinMode(IO_ENDSTOP, INPUT_PULLUP);
  pinMode(IO_STEPPER_DIR1, OUTPUT);
  pinMode(IO_STEPPER_EN1, OUTPUT);
  pinMode(IO_STEPPER_STEP1, OUTPUT);
  pinMode(IO_ROT_DAT, INPUT_PULLUP);
  pinMode(IO_ROT_CLK, INPUT_PULLUP);
  pinMode(IO_ROT_BUT, INPUT_PULLUP);

  digitalWrite(IO_STEPPER_DIR1, LOW);
  digitalWrite(IO_STEPPER_EN1, HIGH);
  digitalWrite(IO_STEPPER_STEP1, LOW);
}

void init_ioe(void) {
  IOE_init(IOE_ADDRESS,
    IOE_FEEDER_SNS |
    IOE_SENSOR2 |
    IOE_SENSOR3 |
    IOE_BUT_A |
    IOE_BUT_B |
    IOE_BUT_C |
    IOE_SWITCH2 |
    IOE_SWITCH1);
  Serial.println(
    IOE_FEEDER_SNS |
    IOE_SENSOR2 |
    IOE_SENSOR3 |
    IOE_BUT_A |
    IOE_BUT_B |
    IOE_BUT_C |
    IOE_SWITCH2 |
    IOE_SWITCH1);
  Serial.println(IOE_read_conf(IOE_ADDRESS));
  IOE_read(IOE_ADDRESS);
}

void init_steppers(uint8_t microsteps1, uint8_t microsteps2) {
  uint16_t ioe_settings = 0x0000;

  if (microsteps1 >= 32) {
    ioe_settings |= IOE_M1MS1 | IOE_M1MS3;
  } else if (microsteps1 >= 16) {
    ioe_settings |= IOE_M1MS3;
  } else if (microsteps1 >= 8) {
    ioe_settings |= IOE_M1MS1 | IOE_M1MS2;
  } else if (microsteps1 >= 4) {
    ioe_settings |= IOE_M1MS2;
  } else if (microsteps1 >= 2) {
    ioe_settings |= IOE_M1MS1;
  } else if (microsteps1 >= 1) {
    ioe_settings |= 0x0000;
  }

  if (microsteps2 >= 32) {
    ioe_settings |= IOE_M2MS1 | IOE_M2MS3;
  } else if (microsteps2 >= 16) {
    ioe_settings |= IOE_M2MS3;
  } else if (microsteps2 >= 8) {
    ioe_settings |= IOE_M2MS1 | IOE_M2MS2;
  } else if (microsteps2 >= 4) {
    ioe_settings |= IOE_M2MS2;
  } else if (microsteps2 >= 2) {
    ioe_settings |= IOE_M2MS1;
  } else if (microsteps2 >= 1) {
    ioe_settings |= 0x0000;
  }

  IOE_write(IOE_ADDRESS, ioe_settings);
}

void scan_twi(void) {
  byte error, address;
  int nDevices;
 
  Serial.println("Scanning...");
 
  nDevices = 0;
  for (address = 1; address < 127; address++) {
    // The i2c_scanner uses the return value of
    // the Write.endTransmisstion to see if
    // a device did acknowledge to the address.
    Wire.beginTransmission(address);
    error = Wire.endTransmission();
 
    if (error == 0) {
      Serial.print("I2C device found at address 0x");
      if (address < 16) {
        Serial.print("0");
      }
      Serial.print(address,HEX);
      Serial.println("  !");
 
      nDevices++;
    } else if (error == 4) {
      Serial.print("Unknown error at address 0x");
      if (address < 16) {
        Serial.print("0");
      }
      Serial.println(address,HEX);
    }    
  }
  if (nDevices == 0) {
    Serial.println("No I2C devices found\n");
  } else {
    Serial.println("done\n");
  }
}

uint16_t read_rotary(void) {
  uint16_t data = {};
  if (digitalRead(IO_ROT_DAT)) {
    data |= IOMASK_ROT_DAT;
  }
  if (digitalRead(IO_ROT_CLK)) {
    data |= IOMASK_ROT_CLK;
  }
  if (digitalRead(IO_ROT_BUT)) {
    data |= IOMASK_ROT_BUT;
  }
  return data;
}