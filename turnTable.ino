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
bool nextAction;

void read_IOE(void) {
    static uint16_t last_data = 0xFFFF;
    uint16_t data = IOE_read(IOE_ADDRESS);
    uint16_t diff = last_data ^ data;

    if (diff & IOE_SWITCH1) {
        if ((data & IOE_SWITCH1) != 0U) {
            table.stop();
            table.disable();
        }
    }

    last_data = data;
}

void check_serial(void) {
    if (Serial.available() > 0) {
        incomingByte = Serial.read();
        startTime = millis();
    }

    static int speed = 360;
    static int distance = 360;
    static int direction = 1;

    switch(incomingByte) {
        case 'a':
            speed = 75;
            print_configuration(distance, speed, direction);
            break;
        case 's':
            speed = 150;
            print_configuration(distance, speed, direction);
            break;
        case 'd':
            speed = 300;
            print_configuration(distance, speed, direction);
            break;
        case 'f':
            speed = 600;
            print_configuration(distance, speed, direction);
            break;
        case 'g':
            speed = 900;
            print_configuration(distance, speed, direction);
            break;
        case 'h':
            speed = 1200;
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
            table.start_moving_angle(direction * distance, speed);
            break;
        case 'm':
            break;
        case 'n':
            table.start_moving_angle(direction * distance, speed);
            nextAction = true;
            break;
        case ' ':
            table.move_angle(direction * distance, speed);
            // slider_move_distance(direction * distance, speed);
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

    // while (nextAction) {
    //     nextAction = table.nextAction();
    //     if (digitalRead(IO_ENDSTOP) == 0) {
    //         table.stop();
    //     }
    // }
    // table.disable();

    while (incomingByte != '\0' && table.getCurrentState() != table.STOPPED) {}

    if (incomingByte != '\0' && incomingByte != '\n') {
        Serial.println("took " + String(millis() - startTime) + " ms");
    }

    incomingByte = '\0';
}

void setup() { 
    init_io();
    Wire.begin();
    Serial.begin(115200);

    // init_ioe();
    // init_steppers(STEPPER1_MICROSTEPS, STEPPER2_MICROSTEPS);
    table.init(TABLE_REDUCTION, RPM_TO_DEGREE_PER_SEC);
    oled.start();
    oled.set_header("SLIDER V1", Alignment::Center);
    oled.set_unit(UNIT_none);
    oled.refresh();
    rotary.init();
    menu.init();
} 

void loop() {
    volatile bool nextAction;
    static long duration = 0U;
    check_serial();
    static int start_time = {};
    unsigned int now = millis();
    uint16_t speed;
    bool direction;
    uint16_t distance;
    int16_t rotation;
    float anglular_speed;

    // uint16_t io_data = IOE_read(IOE_ADDRESS);
    // uint8_t rotary_state = rotary.read_encoder(io_data);
    // uint8_t button_state = rotary.read_button(io_data);

    // if (rotary_state == DIR_CW) {
    //     menu.increase_value();
    // } else if (rotary_state == DIR_CCW) {
    //     menu.decrease_value();
    // }

    // if (button_state == BUT_DOWN) {
    //     if (menu.enter() == Menu::Action::Start) {
    //         oled.refresh();
    //         speed = menu.get_speed();
    //         direction = menu.get_direction();
    //         distance = menu.get_distance();
    //         rotation = menu.get_rotation();
            
    //         print_configuration((direction ? -1 : 1) * distance, speed, direction);
    //         slider_start_moving_distance((direction ? -1 : 1) * distance, speed);
    //         if (rotation != 0) {
    //             anglular_speed = abs(atan(speed / (rotation * 10.0F))) * 180.0F / PI;
    //             print_configuration((direction ? 1 : -1) * (rotation >= 0 ? 1 : -1) * 1000.0F, anglular_speed, rotation >= 0);
    //         } else {
    //             anglular_speed = 0.0F;
    //         }
    //         start_time = now;
    //         nextAction = true;

    //         /* Move until all steps are done or endstop is reached */
    //         while (nextAction) {
    //             if (check_away_endstop()) {
    //                 slider.stop();
    //                 nextAction = false;
    //                 break;
    //             }
    //             nextAction = slider.nextAction();
    //         }
    //         slider.disable();
    //         Serial.println(millis() - start_time);

    //         /* Flip direction if either endstop is reached */
    //         if (check_away_endstop()) {
    //             menu.set_direction(!direction);
    //         }
    //         menu.reset();
    //     }
    // }

    oled.refresh();
}

bool check_endstop(bool side) {
    uint16_t io_data = IOE_read(IOE_ADDRESS);
    uint16_t mask;
    if (side == right) {
        mask = IOE_SWITCH1;
    } else {
        mask = IOE_SWITCH2;
    }
    return (io_data & mask) == 0U;
}

bool check_away_endstop(void) {
    if (menu.get_direction() == right) {
        return check_endstop(right);
    } else {
        return check_endstop(left);
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