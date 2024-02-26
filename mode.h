#define ENUM_MODES(F, D) \
  F(CLOCK, clock, D) \
  F(MENU, menu, D) \
  F(SET_TIME, set_time, D) \
  F(ADD_ALARM, add_alarm, D) \
  F(EDIT_ALARM, edit_alarm, D) \
  F(SET_TONE, set_tone, D) \
  F(SET_BEEP, set_beep, D) \
  F(ALARM, alarm, D)

enum {
  #define E(NAME, name, D) \
    MODE_ ## NAME,
  ENUM_MODES(E, ())
  #undef E
};

uint8_t mode = MODE_CLOCK;

void mode_switch(uint8_t new_mode);