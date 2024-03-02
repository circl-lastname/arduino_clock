bool set_time_will_redraw = false;
uint8_t set_time_stage = 0;

void set_time_enter() {
  bklt_set(true);
  set_time_will_redraw = true;
  set_time_stage = 0;

  time_count = false;
  time_hours = 0;
  time_minutes = 0;
  time_seconds = 0;
  time_week_day = 0;
}

void set_time_update() {
  if (set_time_will_redraw) {
    lcd.clear();

    ui_time_entry_draw(time_hours, time_minutes, time_seconds, set_time_stage);

    if (set_time_stage > 5) {
      lcd.setCursor(0, 1);
      ui_draw_week_day(time_week_day);

      lcd.setCursor(15, 1);
      lcd.write('>');
    }

    set_time_will_redraw = false;
  }
}

void set_time_btn_ok() {
  spkr_beep();

  if (set_time_stage < 6) {
    ui_time_entry_update(&time_hours, &time_minutes, &time_seconds, set_time_stage);
  } else {
    time_week_day = (time_week_day+1) % 7;
  }

  set_time_will_redraw = true;
}

void set_time_btn_right() {
  spkr_beep_confirm();

  set_time_stage++;

  if (set_time_stage == 6) {
    interval_reset(&time_update_interval);
    time_count = true;
  } else if (set_time_stage == 7) {
    mode_switch(MODE_CLOCK);
    return;
  }

  set_time_will_redraw = true;
}

void set_time_time_updated() {
  set_time_will_redraw = true;
}
