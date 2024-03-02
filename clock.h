bool clock_will_redraw = false;
bool clock_display = true;

void clock_enter() {
  bklt_set(clock_display);
  display_set(clock_display);
  clock_will_redraw = true;
}

void clock_update() {
  if (clock_will_redraw) {
    if (clock_display) {
      lcd.clear();

      lcd.setCursor(4, 0);
      ui_draw_time(time_hours, time_minutes, time_seconds);
      lcd.setCursor(0, 1);
      ui_draw_week_day(time_week_day);
    }

    clock_will_redraw = false;
  }
}

void clock_btn_ok() {
  spkr_beep();
  clock_display = !clock_display;
  bklt_set(clock_display);
  display_set(clock_display);

  if (clock_display) {
    clock_will_redraw = true;
  }
}

void clock_btn_right() {
  spkr_beep();
  display_set(true);
  mode_switch(MODE_MENU);
}

void clock_time_updated() {
  clock_will_redraw = true;

  if (alarms_check()) {
    display_set(true);
    mode_switch(MODE_ALARM);
  }
}
