interval_t btn_ok_interval = { 250 };
bool btn_ok_pressed = false;

interval_t btn_right_interval = { 250 };
bool btn_right_pressed = false;

bool btn_ok_check() {
  bool pressed = !digitalRead(BTN_OK);

  if (pressed && !btn_ok_pressed) {
    btn_ok_pressed = true;
    interval_reset(&btn_ok_interval);
    return true;
  } else if (!pressed && btn_ok_pressed && interval_check(&btn_ok_interval)) {
    btn_ok_pressed = false;
  }

  return false;
}

bool btn_right_check() {
  bool pressed = !digitalRead(BTN_RIGHT);

  if (pressed && !btn_right_pressed) {
    btn_right_pressed = true;
    interval_reset(&btn_right_interval);
    return true;
  } else if (!pressed && btn_right_pressed && interval_check(&btn_right_interval)) {
    btn_right_pressed = false;
  }

  return false;
}