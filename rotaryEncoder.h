#pragma once

#include <cstdint>

#define DIR_MASK        0x30
#define STEP_MASK       0x0F

/* Direction flags */
static constexpr uint8_t R_START    = 0x00;
static constexpr uint8_t DIR_NONE   = 0x00;
static constexpr uint8_t DIR_CW     = 0x10;
static constexpr uint8_t DIR_CCW    = 0x20;

/* Full step step flags */
#define FS_R_CW_FINAL   0x01
#define FS_R_CW_BEGIN   0x02
#define FS_R_CW_NEXT    0x03
#define FS_R_CCW_BEGIN  0x04
#define FS_R_CCW_FINAL  0x05
#define FS_R_CCW_NEXT   0x06

/* Half step step flags */
#define HS_R_CCW_BEGIN   0x1
#define HS_R_CW_BEGIN    0x2
#define HS_R_START_M     0x3
#define HS_R_CW_BEGIN_M  0x4
#define HS_R_CCW_BEGIN_M 0x5

static constexpr uint8_t BUT_DOWN   = 0x01;
static constexpr uint8_t BUT_UP     = 0x02;

enum class RotaryMode {
    HALF_STEP = 0,
    FULL_STEP = 1
};

class RotaryEncoder {
public:
    explicit RotaryEncoder(RotaryMode mode = RotaryMode::HALF_STEP);
    RotaryEncoder(const RotaryEncoder &other) = delete;
    RotaryEncoder(RotaryEncoder &&other) = delete;
    RotaryEncoder &operator=(const RotaryEncoder &other) = delete;
    RotaryEncoder &operator=(RotaryEncoder &&other) = delete;
    void init(void);
    uint8_t read_encoder(uint16_t data);
    uint8_t read_button(uint16_t data);
    uint8_t read(uint16_t data);
private:
    uint8_t m_input_last_state;
    RotaryMode m_mode;

    uint32_t m_last_changed_time;
    uint16_t m_last_io_data;
    bool m_change_expired;
};