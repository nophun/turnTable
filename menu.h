#pragma once

#include <array>
#include <climits>
#include "oled.h"

static constexpr std::array<uint16_t, 6U> speed_steps = {1U, 2U, 5U, 10U, 25U, 50U};
static constexpr std::array<uint16_t, 5U> distance_steps = {50U, 100U, 200U, 300U, 1000U};
#define right true
#define left false

class Menu {
private:
    enum class Page {
        StartScreen,
        Speed,
        Direction,
        Distance,
        Rotation,
        Launch,
        Sliding
    };
public:
    enum class Action {
        NoAction,
        Start
    };

    Menu(OLED *oled);
    ~Menu() = default;
    Menu(const Menu &other) = delete;
    Menu(Menu &&other) = delete;
    Menu &operator=(const Menu &other) = delete;
    Menu &operator=(Menu &&other) = delete;

    void init(void);
    void new_page(Page new_page);
    void next_page(void);
    Action enter(void);
    void increase_value(void);
    void decrease_value(void);
    void reset(void);

    uint16_t get_speed(void) { return m_speed; }
    bool get_direction(void) { return m_direction; }
    uint16_t get_distance(void) { return m_distance; }
    int16_t get_rotation(void) { return m_rotation; }
    void set_direction(bool direction) { m_direction = direction; }

    Menu *get(void) { return m_menu; }

private:
    Page m_current_page = {Page::StartScreen};
    OLED *m_oled {nullptr};
    size_t m_speed_idx {speed_steps.size() / 2U};
    size_t m_distance_idx {distance_steps.size() - 1U};
    uint16_t m_speed {speed_steps[m_speed_idx]};
    bool m_direction {right};
    uint16_t m_distance {distance_steps[m_distance_idx]};
    int16_t m_rotation {0};
    bool m_launch {false};

    void display_value(Page page);

    static inline Menu *m_menu = {nullptr};
};