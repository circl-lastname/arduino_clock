bool menu_will_redraw = false;
uint8_t menu_item = 0;

void menu_enter() {
  bklt_set(true);
  menu_will_redraw = true;
  menu_item = 0;
}

void menu_update() {
  if (menu_will_redraw) {
    lcd.clear();

    lcd.setCursor(0, 0);
    lcd.print(F("Menu"));

    lcd.setCursor(0, 1);

    switch (menu_item) {
      case 0:
        lcd.print(F("Set time"));
      break;
      case 1:
        lcd.print(F("Add alarm"));
      break;
      case 2:
        lcd.print(F("Edit alarm"));
      break;
      case 3:
        lcd.print(F("Alarm tone"));
      break;
      case 4:
        lcd.print(F("Button beeping"));
      break;
    }

    menu_will_redraw = false;
  }
}

void menu_btn_ok() {
  spkr_beep_confirm();

  switch (menu_item) {
    case 0:
      mode_switch(MODE_SET_TIME);
    return;
    case 1:
      mode_switch(MODE_ADD_ALARM);
    break;
    case 2:
      mode_switch(MODE_EDIT_ALARM);
    break;
    case 3:
      mode_switch(MODE_SET_TONE);
    break;
    case 4:
      mode_switch(MODE_SET_BEEP);
    break;
  }
}

void menu_btn_right() {
  spkr_beep();

  menu_item++;

  if (menu_item == 5) {
    mode_switch(MODE_CLOCK);
    return;
  }

  menu_will_redraw = true;
}

void menu_time_updated() {
  
}