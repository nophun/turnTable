#pragma once

#include "DRV8825.h"

class StepperCtrl : public DRV8825 {
public:
    explicit StepperCtrl(short steps, short dir_pin, short step_pin, short enable_pin) : 
        DRV8825(steps, dir_pin, step_pin, enable_pin) {
    }
    StepperCtrl() = delete;
    ~StepperCtrl() = default;
    StepperCtrl(const StepperCtrl &other) = delete;
    StepperCtrl(StepperCtrl &&other) = delete;
    StepperCtrl &operator=(const StepperCtrl &other) = delete;
    StepperCtrl &operator=(StepperCtrl &&other) = delete;

    void init() {
        begin(STEPPER1_RPM, STEPPER1_MICROSTEPS);
        setEnableActiveState(LOW);
        setSpeedProfile(BasicStepperDriver::LINEAR_SPEED, STEPPER_ACCEL, STEPPER_DECEL);
        disable();
    }

    void move_raw_angle(float angle, float rpm) {
        setRPM(rpm);
        enable();
        rotate(angle);
        disable();
    }
};