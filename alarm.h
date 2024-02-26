interval_t alarm_interval = { 500 };
bool alarm_even_beep = false;

void alarm_enter() {
  alarm_interval.next_trigger = millis();
  alarm_even_beep = false;

  lcd.clear();

  lcd.setCursor(5, 0);
  lcd.print(F("Alarm!"));
}

void alarm_update() {
  if (tone_update(&alarm_interval, &alarm_even_beep)) {
    bklt_set(alarm_even_beep);
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