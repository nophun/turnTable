#include <cstdint>
#include <Wire.h>

#define IOE_CMD_INPUT     0
#define IOE_CMD_OUTPUT    2
#define IOE_CMD_CONF      6

uint16_t IOE_read_conf(uint8_t address);
void IOE_init(uint8_t address, uint16_t bitmap);
uint16_t IOE_read(uint8_t address);
uint16_t IOE_read_out(uint8_t address);
void IOE_write(uint8_t address, uint16_t bitmap);
void IOE_set(uint8_t address, uint16_t bit);
void IOE_clear(uint8_t address, uint16_t bit);
