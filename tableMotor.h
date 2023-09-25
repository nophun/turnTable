#pragma once

#include "turnTable.h"
#include "stepperCtrl.h"

class TableMotor : public StepperCtrl {
public:
    TableMotor();
    ~TableMotor() = default;
    TableMotor(const TableMotor &other) = delete;
    TableMotor(TableMotor &&other) = delete;
    TableMotor &operator=(const TableMotor &other) = delete;
    TableMotor &operator=(TableMotor &&other) = delete;

    void init(float reduction, float rpm_conversion);
    void move_angle(float angle, float rpm = STEPPER1_RPM);
    void start_moving_angle(float angle, float rpm = STEPPER1_RPM);
    void setup_move(float angle, float rpm = STEPPER1_RPM);
private:
    float m_reduction {1.0F};
    float m_rpm_to_deg {1.0F};
};