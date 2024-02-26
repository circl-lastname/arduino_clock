bool clock_will_redraw = false;
bool clock_bklt = true;

void clock_enter() {
  bklt_set(clock_bklt);
  clock_will_redraw = true;
}

void clock_update() {
  if (clock_will_redraw) {
    lcd.clear();

    lcd.setCursor(4, 0);
    ui_draw_time(time_hours, time_minutes, time_seconds);
    lcd.setCursor(0, 1);
    ui_draw_week_day(time_week_day);

    clock_will_redraw = false;
  }
}

void clock_btn_ok() {
  spkr_beep();
  clock_bklt = !clock_bklt;
  bklt_set(clock_bklt);
}

void clock_btn_right() {
  spkr_beep();
  mode_switch(MODE_MENU);
}

void clock_time_updated() {
  clock_will_redraw = true;

  if (alarms_check()) {
    mode_switch(MODE_ALARM);
  }
}