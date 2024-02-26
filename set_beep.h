bool set_beep_will_redraw = false;

void set_beep_enter() {
  bklt_set(true);
  set_beep_will_redraw = true;
}

void set_beep_update() {
  if (set_beep_will_redraw) {
    lcd.clear();

    lcd.setCursor(0, 0);
    lcd.print(F("Button beeping"));

    lcd.setCursor(0, 1);

    if (bitRead(local_storage.flags, 0)) {
      lcd.print(F("On"));
    } else {
      lcd.print(F("Off"));
    }

    set_beep_will_redraw = false;
  }
}

void set_beep_btn_ok() {
  spkr_beep_confirm();
  mode_switch(MODE_CLOCK);
}

void set_beep_btn_right() {
  if (bitRead(local_storage.flags, 0)) {
    local_storage.flags = bitClear(local_storage.flags, 0);
  } else {
    local_storage.flags = bitSet(local_storage.flags, 0);
  }

  storage_modified();

  spkr_beep();

  set_beep_will_redraw = true;
}

void set_beep_time_updated() {
  
}