bool set_tone_will_redraw = false;
interval_t set_tone_interval = { 500 };
bool set_tone_even_beep = false;

void set_tone_enter() {
  bklt_set(true);
  set_tone_will_redraw = true;
  interval_reset(&set_tone_interval);
  set_tone_even_beep = false;
}

void set_tone_update() {
  if (interval_check(&set_tone_interval)) {
    tone_play(set_tone_even_beep);
    set_tone_even_beep = !set_tone_even_beep;
  }

  if (set_tone_will_redraw) {
    lcd.clear();
    
    lcd.setCursor(0, 0);
    lcd.print(F("Alarm tone"));

    lcd.setCursor(0, 1);

    switch (tone_get()) {
      case 0:
        lcd.print(F("Low"));
      break;
      case 1:
        lcd.print(F("Medium"));
      break;
      case 2:
        lcd.print(F("High"));
      break;
    }

    set_tone_will_redraw = false;
  }
}

void set_tone_btn_ok() {
  spkr_stop();
  spkr_beep_confirm();
  mode_switch(MODE_CLOCK);
}

void set_tone_btn_right() {
  tone_set((tone_get()+1) % 3);

  set_tone_interval.next_trigger = millis();
  set_tone_even_beep = false;

  set_tone_will_redraw = true;
}

void set_tone_time_updated() {
  
}
