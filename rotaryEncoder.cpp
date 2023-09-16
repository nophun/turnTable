#include <Arduino.h>
#include "rotaryEncoder.h"
#include "shuffler_board.h"

static const unsigned char fullStepsTable[7][4] = {
    // 00           01              10              11
    {R_START,       FS_R_CW_BEGIN,  FS_R_CCW_BEGIN, R_START},           // R_START
    {FS_R_CW_NEXT,  R_START,        FS_R_CW_FINAL,  R_START | DIR_CW},  // R_CW_FINAL
    {FS_R_CW_NEXT,  FS_R_CW_BEGIN,  R_START,        R_START},           // R_CW_BEGIN
    {FS_R_CW_NEXT,  FS_R_CW_BEGIN,  FS_R_CW_FINAL,  R_START},           // R_CW_NEXT
    {FS_R_CCW_NEXT, R_START,        FS_R_CCW_BEGIN, R_START},           // R_CCW_BEGIN
    {FS_R_CCW_NEXT, FS_R_CCW_FINAL, R_START,        R_START | DIR_CCW}, // R_CCW_FINAL
    {FS_R_CCW_NEXT, FS_R_CCW_FINAL, FS_R_CCW_BEGIN, R_START}            // R_CCW_NEXT
};

static const unsigned char halfStepsTable[6][4] = {
    // 00                  01              10                       11
    {HS_R_START_M,           HS_R_CW_BEGIN,     HS_R_CCW_BEGIN,     R_START},           // R_START (00)
    {HS_R_START_M | DIR_CCW, R_START,           HS_R_CCW_BEGIN,     R_START},           // R_CCW_BEGIN
    {HS_R_START_M | DIR_CW,  HS_R_CW_BEGIN,     R_START,            R_START},           // R_CW_BEGIN
    {HS_R_START_M,           HS_R_CCW_BEGIN_M,  HS_R_CW_BEGIN_M,    R_START},           // R_START_M (11)
    {HS_R_START_M,           HS_R_START_M,      HS_R_CW_BEGIN_M,    R_START | DIR_CW},  // R_CW_BEGIN_M 
    {HS_R_START_M,           HS_R_CCW_BEGIN_M,  HS_R_START_M,       R_START | DIR_CCW}  // R_CCW_BEGIN_M
};

RotaryEncoder::RotaryEncoder(RotaryMode mode) : m_mode(mode) {
    m_input_last_state = 0;
    m_last_io_data = 0;
}

void RotaryEncoder::init() {

}

uint8_t RotaryEncoder::read_encoder(uint16_t data) {
    uint8_t direction;

    bool dat = data & IOE_BUT_B;
    bool clk = data & IOE_BUT_C;

    if (m_mode == RotaryMode::HALF_STEP) {
        m_input_last_state = halfStepsTable[m_input_last_state & STEP_MASK][(dat << 1) | clk];
    } else {
        m_input_last_state = fullStepsTable[m_input_last_state & STEP_MASK][(dat << 1) | clk];
    }

    direction = (m_input_last_state & DIR_MASK);

    return direction;
}

uint8_t RotaryEncoder::read_button(uint16_t data) {
    if ((data ^ m_last_io_data) & IOE_BUT_A) {
        m_last_io_data = data;
        if (data & IOE_BUT_A) {
            return BUT_UP;
        } else {
            return BUT_DOWN;
        }
    }

    return 0x00;
}