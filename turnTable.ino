#include <Wire.h>
#include <cmath>
#include "shuffler_board.h"
#include "turnTable.h"
#include "menu.h"
#include "oled.h"
#include "rotaryEncoder.h"
#include "tableMotor.h"

extern OLED oled;
extern RotaryEncoder rotary;
extern TableMotor table;

Menu menu(&oled);
byte incomingByte;
long startTime;
long time_until_next_move = {0};
long time_of_last_move = {0};
long stop_time = {0};
int direction = {1};
int speed = {0};

void check_serial(void) {
    if (Serial.available() > 0) {
        incomingByte = Serial.read();
        startTime = millis();
    }

    static int distance = 360;

    switch(incomingByte) {
        case 'a':
            speed = 10;
            print_configuration(distance, speed, direction);
            break;
        case 's':
            speed = 20;
            print_configuration(distance, speed, direction);
            break;
        case 'd':
            speed = 40;
            print_configuration(distance, speed, direction);
            break;
        case 'f':
            speed = 50;
            print_configuration(distance, speed, direction);
            break;
        case 'g':
            speed = 100;
            print_configuration(distance, speed, direction);
            break;
        case 'h':
            speed = 200;
            print_configuration(distance, speed, direction);
            break;
        case 'q':
            distance = 45;
            print_configuration(distance, speed, direction);
            break;
        case 'w':
            distance = 90;
            print_configuration(distance, speed, direction);
            break;
        case 'e':
            distance = 180;
            print_configuration(distance, speed, direction);
            break;
        case 'r':
            distance = 1*360;
            print_configuration(distance, speed, direction);
            break;
        case 't':
            distance = 2*360;
            print_configuration(distance, speed, direction);
            break;
        case 'y':
            distance = 4*360;
            print_configuration(distance, speed, direction);
            break;
        case 'u':
            distance = 8*360;
            print_configuration(distance, speed, direction);
            break;
        case 'z':
            direction *= -1;
            print_configuration(distance, speed, direction);
            break;
        case 'x':
            table.setup_move(direction * distance, abs(speed));
            break;
        case 'm':
            table.enable();
            green_light_to_motor();
            break;
        case 'n':
            table.setup_move(direction * distance, abs(speed));
            table.enable();
            green_light_to_motor();
            break;
        case ' ':
            table.move_angle(direction * distance, speed);
            break;
        case ',':
            table.move_angle(360, 50);
            break;
        case '.':
            table.move_angle(360*2, 50);
            break;
        case '-':
            table.move_angle(360*3, 50);
            break;
        default:
            break;
    }

    if (incomingByte != '\0' && incomingByte != '\n') {
        Serial.println("took " + String(millis() - startTime) + " ms");
    }

    incomingByte = '\0';
}

void setup() { 
    init_io();
    Wire.begin();
    Serial.begin(115200);

    table.init(TABLE_REDUCTION, RPM_TO_DEGREE_PER_SEC);
    oled.start();
    oled.set_header("SLIDER V1", Alignment::Center);
    oled.set_unit(UNIT_none);
    oled.refresh();
    rotary.init(IOMASK_ROT_DAT, IOMASK_ROT_CLK, IOMASK_ROT_BUT);
    menu.init();
}

static inline void green_light_to_motor(void) {
    table.enable();
    time_until_next_move = 1;
    time_of_last_move = micros();
}

static inline void red_light_to_motor(void) {
    time_until_next_move = 0;
    time_of_last_move = micros();
    table.stop();
    table.disable();
}

void loop() {
    static int start_time = {};
    static uint16_t distance = {720};
    unsigned int now = millis();
    unsigned int now_micros = micros();
    bool update_move = {false};
    static constexpr long cMidSteps = 8000; // 720 distance gives 15798 steps
    static constexpr long cRestartDelay = 1000; // time between stop and start

    if (speed == 0) {
        red_light_to_motor();
    } else
    /* Handle steps */
    if (time_until_next_move != 0 && (now_micros - time_of_last_move) > time_until_next_move) {
        time_until_next_move = table.nextAction();
        time_of_last_move = now_micros;
        if (time_until_next_move <= 0) {
            Serial.println("Stopped");
            table.stop();
            table.disable();
        }
    }

    /* Handle infinite rotary */
    if (true && table.getCurrentState() != BasicStepperDriver::STOPPED && table.getStepsRemaining() < cMidSteps) {
        Serial.println("Kick");
        table.setup_move((speed > 0 ? 1 : -1) * distance, abs(speed));
    }

    check_serial();
    uint16_t io_data = read_rotary();
    uint8_t rotary_state = rotary.read_encoder(io_data);
    uint8_t button_state = rotary.read_button(io_data);

    if (rotary_state == DIR_CW) {
        Serial.println("CW");
        bool need_to_start = (speed == 0);
        if (!need_to_start || (now - stop_time) > cRestartDelay) {
            speed -= 1;
            table.setup_move((speed > 0 ? 1 : -1) * distance, abs(speed));
            if (need_to_start) {
                green_light_to_motor();
            }
            Serial.println(table.getStepsCompleted());
            Serial.println(table.getStepsRemaining());
            print_configuration(distance, abs(speed), speed > 0 ? 1 : -1);
        }
        if (speed == 0) {
            stop_time = now;
        }
    } else if (rotary_state == DIR_CCW) {
        Serial.println("CCW");
        bool need_to_start = (speed == 0);
        if (!need_to_start || (now - stop_time) > cRestartDelay) {
            speed += 1;
            table.setup_move((speed > 0 ? 1 : -1) * distance, abs(speed));
            if (need_to_start) {
                green_light_to_motor();
            }
            print_configuration(distance, abs(speed), speed > 0 ? 1 : -1);
        }
        if (speed == 0) {
            stop_time = now;
        }
    }

    if (button_state == BUT_DOWN) {
        Serial.println("BUT");
        if (table.getCurrentState() == BasicStepperDriver::STOPPED) {
            table.setup_move((speed > 0 ? 1 : -1) * distance, abs(speed));
            green_light_to_motor();
            start_time = now;
        } else {
            speed = 0;
            Serial.println("Stopped at " + String(now - start_time));
        }
    }
}

void print_configuration(int distance, int speed, int direction) {
    Serial.print(direction > 0 ? "->" : "<-");
    Serial.print(" distance ");
    Serial.print(distance);
    Serial.print(", speed ");
    Serial.println(speed);
}

void print_configuration(float distance, float speed, int direction) {
    Serial.print(direction > 0 ? "->" : "<-");
    Serial.print(" angle ");
    Serial.print(distance);
    Serial.print(", speed ");
    Serial.println(speed);
}