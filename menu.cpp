#include <cmath>
#include "menu.h"

Menu::Menu(OLED *oled) : m_oled{oled} {
    m_menu = this;
    new_page(Page::StartScreen);
}

void Menu::init(void) {

}

void Menu::new_page(Page new_page) {
    switch(new_page) {
        case Page::StartScreen:
            m_oled->set_header("SLIDER V1", Alignment::Center);
            m_oled->set_unit(UNIT_none);
            break;
        case Page::Speed:
            m_oled->set_header(" SPEED");
            m_oled->set_unit(UNIT_mm);
            break;
        case Page::Direction:
            m_oled->set_header(" DIRECTION");
            m_oled->set_unit(UNIT_none);
            break;
        case Page::Distance:
            m_oled->set_header(" DISTANCE");
            m_oled->set_unit(UNIT_mm);
            break;
        case Page::Rotation:
            m_oled->set_header(" ROTATION");
            m_oled->set_unit(UNIT_cm);
            break;
        case Page::Launch:
            m_oled->set_header(" LAUNCH");
            m_oled->set_unit(UNIT_none);
            break;
        case Page::Sliding:
            m_oled->set_header(" SLIDING");
            m_oled->set_unit(UNIT_none);
            break;
        default:
            return;
    }
    m_current_page = new_page;
    display_value(m_current_page);
}

void Menu::next_page(void) {
    switch(m_current_page) {
        case Page::StartScreen:
            new_page(Page::Speed);
            break;
        case Page::Speed:
            new_page(Page::Direction);
            break;
        case Page::Direction:
            new_page(Page::Distance);
            break;
        case Page::Distance:
            new_page(Page::Rotation);
            break;
        case Page::Rotation:
            new_page(Page::Launch);
            break;
        case Page::Launch:
            new_page(Page::Speed);
            break;
        default:
            return;
    }
}

Menu::Action Menu::enter(void) {
    if (m_current_page == Page::Launch && m_launch == true) {
        m_launch = false;
        new_page(Page::Sliding);
        return Action::Start;
    }

    next_page();
    return Action::NoAction;
}

void Menu::increase_value(void) {
    switch(m_current_page) {
        case Page::StartScreen:
            break;
        case Page::Speed:
            if (m_speed_idx < (speed_steps.size() - 1U)) {
                m_speed_idx++;
                m_speed = speed_steps[m_speed_idx];
            }
            break;
        case Page::Direction:
            m_direction = !m_direction;
            break;
        case Page::Distance:
            if (m_distance_idx < (distance_steps.size() - 1U)) {
                m_distance_idx++;
                m_distance = distance_steps[m_distance_idx];
            }
            break;
        case Page::Rotation:
            if (m_rotation < 100) {
                m_rotation += 10;
            }
            break;
        case Page::Launch:
            m_launch = !m_launch;
            break;
        default:
            return;
    }
    display_value(m_current_page);
}

void Menu::decrease_value(void) {
    switch(m_current_page) {
        case Page::StartScreen:
            break;
        case Page::Speed:
            if (m_speed_idx > 0U) {
                m_speed_idx--;
                m_speed = speed_steps[m_speed_idx];
            }
            break;
        case Page::Direction:
            increase_value();
            break;
        case Page::Distance:
            if (m_distance_idx > 0U) {
                m_distance_idx--;
                m_distance = distance_steps[m_distance_idx];
            }
            break;
        case Page::Rotation:
            if (m_rotation > -100) {
                m_rotation -= 10;
            }
            break;
        case Page::Launch:
            increase_value();
            break;
        default:
            return;
    }
    display_value(m_current_page);
}

void Menu::display_value(Page page) {
    switch(page) {
        case Page::StartScreen:
            m_oled->set_value("");
            break;
        case Page::Speed:
            m_oled->set_value(m_speed, 0);
            break;
        case Page::Direction:
            if (m_direction) {
                m_oled->set_value("->");
            } else {
                m_oled->set_value("<-");
            }
            break;
        case Page::Distance:
            if (m_distance_idx == distance_steps.size() - 1) {
                m_oled->set_value("MAX");
            } else {
                m_oled->set_value(m_distance, 0);
            }
            break;
        case Page::Rotation:
            if (m_rotation == 0) {
                m_oled->set_value("-");
            } else {
                m_oled->set_value(m_rotation, 0);
            }
            break;
        case Page::Launch:
            if (m_launch) {
                m_oled->set_value("YES");
            } else {
                m_oled->set_value("NO");
            }
            break;
        case Page::Sliding:
            m_oled->set_value("...");
            break;
        default:
            break;
    }
}

void Menu::reset(void) {
    new_page(Page::StartScreen);
}