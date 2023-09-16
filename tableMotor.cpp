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

void TableMotor::move_angle(float angle, int rpm) {
    move_angle(angle, static_cast<float>(rpm));
}

void TableMotor::start_moving_angle(float angle, float rpm) {
    if (abs(angle) < 0.001F || rpm < 0.001F) {
        return;
    }
    move_angle(angle, static_cast<float>(rpm));
    float raw_angle = angle * m_reduction;
    float raw_rpm = rpm * m_rpm_to_deg * m_reduction;
    setRPM(raw_rpm);
    enable();
    startRotate(raw_angle);
}

void TableMotor::start_moving_angle(float angle, int rpm) {
    start_moving_angle(angle, static_cast<float>(rpm));
}

// void slider_move_angle(double angle, int rpm) {
//     // if (angle < 0.001F || rpm == 0) {
//     //     return;
//     // }
//     // Serial.println(angle);
//     // slider.setRPM(rpm);
//     // slider.enable();
//     // slider.rotate(angle);
//     // slider.disable();
// }

// void slider_move_distance(double distance_mm, int speed_mms) {
//     // if (distance_mm < 0.001F || speed_mms == 0) {
//     //     return;
//     // }
//     // float angle = distance_mm / SLIDER_CIRC * SLIDER_REDUCTION * 360.0F;
//     // float rpm = speed_mms * RPM_TO_DEGREE_PER_SEC * SLIDER_REDUCTION;
//     // slider_move_angle(angle, static_cast<int>(rpm));
// }

// void slider_start_moving_distance(double distance_mm, int speed_mms) {
//     // if (distance_mm < 0.001F || speed_mms == 0) {
//     //     return;
//     // }
//     // float angle = distance_mm / SLIDER_CIRC * SLIDER_REDUCTION * 360.0F;
//     // float rpm = speed_mms * RPM_TO_DEGREE_PER_SEC * SLIDER_REDUCTION;
//     // slider.setRPM(rpm);
//     // slider.enable();
//     // slider.startRotate(angle);
// }
