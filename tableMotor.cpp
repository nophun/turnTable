#include "shuffler_board.h"
#include "tableMotor.h"

TableMotor::TableMotor() : StepperCtrl(STEPPER1_STEPS, IO_STEPPER_DIR1, IO_STEPPER_STEP1, IO_STEPPER_EN1) {
    
}

void TableMotor::init(float reduction, float rpm_conversion) {
    m_reduction = reduction;
    m_rpm_to_deg = rpm_conversion;
    StepperCtrl::init();
}

void TableMotor::move_angle(float angle, float rpm) {
    if (abs(angle) < 0.001F || rpm < 0.001F) {
        return;
    }
    float raw_angle = angle * m_reduction;
    float raw_rpm = rpm * m_rpm_to_deg * m_reduction;
    move_raw_angle(raw_angle, raw_rpm);
}

void TableMotor::start_moving_angle(float angle, float rpm) {
    if (abs(angle) < 0.001F || rpm < 0.001F) {
        return;
    }
    float raw_angle = angle * m_reduction;
    float raw_rpm = rpm * m_rpm_to_deg * m_reduction;
    Serial.println(String(angle) + "@" + String(rpm));
    setRPM(raw_rpm);
    enable();
    setRPM(raw_rpm);
    startRotate(raw_angle);
}

void TableMotor::setup_move(float angle, float rpm) {
    if (abs(angle) < 0.001F || rpm < 0.001F) {
        return;
    }
    float raw_angle = angle * m_reduction;
    float raw_rpm = rpm * m_rpm_to_deg * m_reduction;
    setRPM(raw_rpm);
    startRotate(raw_angle);
}
