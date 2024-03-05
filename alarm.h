interval_t alarm_interval = { 500 };
bool alarm_even_beep = true;

void alarm_enter() {
  interval_reset(&alarm_interval);
  alarm_even_beep = true;

  lcd.clear();

  lcd.setCursor(5, 0);
  lcd.print(F("Alarm!"));

  tone_play(false);
  bklt_set(true);
}

void alarm_update() {
  if (interval_check(&alarm_interval)) {
    tone_play(alarm_even_beep);
    bklt_set(!alarm_even_beep);

    alarm_even_beep = !alarm_even_beep;
  }
}

void alarm_btn_ok() {
  spkr_stop();
  mode_switch(MODE_CLOCK);
}

void alarm_btn_right() {
  spkr_stop();
  mode_switch(MODE_CLOCK);
}

void alarm_time_updated() {
  
}
