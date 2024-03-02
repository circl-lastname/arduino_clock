bool add_alarm_will_redraw = false;
uint8_t add_alarm_stage = 0;
bool add_alarm_week = false;
bool add_alarm_error = false;

uint8_t add_alarm_hours = 0;
uint8_t add_alarm_minutes = 0;
uint8_t add_alarm_seconds = 0;
uint8_t add_alarm_days = 0x7f;

void add_alarm_enter() {
  bklt_set(true);
  add_alarm_will_redraw = true;
  add_alarm_stage = 0;
  add_alarm_week = false;

  add_alarm_hours = 0;
  add_alarm_minutes = 0;
  add_alarm_seconds = 0;
  add_alarm_days = 0x7f;

  add_alarm_error = (alarms_amount() == MAX_ALARMS);
}

void add_alarm_update() {
  if (add_alarm_will_redraw) {
    lcd.clear();

    if (add_alarm_error) {
      lcd.setCursor(0, 0);
      lcd.print(F("Error"));
      lcd.setCursor(0, 1);
      lcd.print(F("Memory full"));

      add_alarm_will_redraw = false;
      return;
    }

    if (!add_alarm_week) {
      ui_time_entry_draw(add_alarm_hours, add_alarm_minutes, add_alarm_seconds, add_alarm_stage);
    } else {
      lcd.setCursor(0, 0);
      ui_draw_week_days(add_alarm_days);

      lcd.setCursor(add_alarm_stage, 1);
      lcd.write('^');
    }

    add_alarm_will_redraw = false;
  }
}

void add_alarm_btn_ok() {
  if (add_alarm_error) {
    spkr_beep_confirm();
    mode_switch(MODE_CLOCK);
    return;
  }

  spkr_beep();

  if (!add_alarm_week) {
    ui_time_entry_update(&add_alarm_hours, &add_alarm_minutes, &add_alarm_seconds, add_alarm_stage);
  } else {
    if (bitRead(add_alarm_days, add_alarm_stage)) {
      add_alarm_days = bitClear(add_alarm_days, add_alarm_stage);
    } else {
      add_alarm_days = bitSet(add_alarm_days, add_alarm_stage);
    }
  }

  add_alarm_will_redraw = true;
}

void add_alarm_btn_right() {
  if (!add_alarm_error) {
    spkr_beep_confirm();

    add_alarm_stage++;

    if (!add_alarm_week && add_alarm_stage == 6) {
      add_alarm_week = true;
      add_alarm_stage = 0;
    }

    if (add_alarm_week && add_alarm_stage == 7) {
      alarms_add(add_alarm_hours, add_alarm_minutes, add_alarm_seconds, add_alarm_days);
      storage_commit_enabled = true;
      mode_switch(MODE_CLOCK);
      return;
    }

    add_alarm_will_redraw = true;
  }
}

void add_alarm_time_updated() {
  
}
