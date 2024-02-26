#define E(NAME, name, FUNC_NAME) \
  case MODE_ ## NAME: \
    name ## _ ## FUNC_NAME(); \
  break;

#define MODE_FUNC(FUNC_NAME) \
  void mode_ ## FUNC_NAME() { \
    switch (mode) { \
      ENUM_MODES(E, FUNC_NAME) \
    } \
  }

MODE_FUNC(enter)
MODE_FUNC(update)
MODE_FUNC(btn_ok)
MODE_FUNC(btn_right)
MODE_FUNC(time_updated)

#undef E

void mode_switch(uint8_t new_mode) {
  mode = new_mode;
  mode_enter();
}