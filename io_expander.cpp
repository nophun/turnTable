#include <Arduino.h>
#include <Wire.h>
#include "io_expander.h"

uint16_t IOE_read_conf(uint8_t address) {
  union {
    uint16_t data = 0x0000;
    uint8_t buff[2];
  };

  Wire.beginTransmission(address);
  Wire.write(IOE_CMD_CONF);
  Wire.endTransmission();

  Wire.requestFrom(address, 2);
  Wire.readBytes(buff, 2);

  return data;
}

void IOE_init(uint8_t address, uint16_t bitmap) {
  Wire.beginTransmission(address);
  Wire.write(IOE_CMD_CONF);
  Wire.write(bitmap & 0xFF);
  Wire.write((bitmap >> 8) & 0xFF);
  Wire.endTransmission();
}

uint16_t IOE_read(uint8_t address) {
  union {
    uint16_t data = 0x0000;
    uint8_t buff[2];
  };

  Wire.beginTransmission(address);
  Wire.write(IOE_CMD_INPUT);
  Wire.endTransmission();

  Wire.requestFrom(address, 2);
  Wire.readBytes(buff, 2);

  return data;
}

uint16_t IOE_read_out(uint8_t address) {
  union {
    uint16_t data = 0x0000;
    uint8_t buff[2];
  };

  Wire.beginTransmission(address);
  Wire.write(IOE_CMD_OUTPUT);
  Wire.endTransmission();

  Wire.requestFrom(address, 2);
  Wire.readBytes(buff, 2);

  return data;
}

void IOE_write(uint8_t address, uint16_t bitmap) {
  Wire.beginTransmission(address);
  Wire.write(IOE_CMD_OUTPUT);
  Wire.write(bitmap & 0xFF);
  Wire.write((bitmap >> 8) & 0xFF);
  Wire.endTransmission();
}

void IOE_set(uint8_t address, uint16_t bit) {
  union {
    uint16_t data = 0x0000;
    uint8_t buff[2];
  };

  Wire.beginTransmission(address);
  Wire.write(IOE_CMD_OUTPUT);
  Wire.endTransmission();

  Wire.requestFrom(address, 2);
  Wire.readBytes(buff, 2);

  IOE_write(address, data |= bit);
}

void IOE_clear(uint8_t address, uint16_t bit) {
  union {
    uint16_t data = 0x0000;
    uint8_t buff[2];
  };

  Wire.beginTransmission(address);
  Wire.write(IOE_CMD_OUTPUT);
  Wire.endTransmission();

  Wire.requestFrom(address, 2);
  Wire.readBytes(buff, 2);

  IOE_write(address, data &= ~bit);
}