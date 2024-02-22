#include <LiquidCrystal.h>

#include "pins.h"

LiquidCrystal lcd(LCD_RS, LCD_E, LCD_B4, LCD_B5, LCD_B6, LCD_B7);

enum {
  MODE_CLOCK,
  MODE_SETTINGS,
  MODE_SET_TIME,
  MODE_SET_ALARM,
  MODE_SET_TONE,
  MODE_ALARM,
};

enum {
  TONE_LOW,
  TONE_MEDIUM,
  TONE_HIGH,
};

uint8_t mode = MODE_CLOCK;
uint32_t next_time_update = 2000;

void lcd_write_number(uint8_t value) {
  if (value <= 99) {
    lcd.write(0x30 + value / 10);
    lcd.write(0x30 + value % 10);
  }
}

void lcd_write_week_day(uint8_t value) {
  switch (value) {
    case 0:
      lcd.print(F("Monday"));
    break;
    case 1:
      lcd.print(F("Tuesday"));
    break;
    case 2:
      lcd.print(F("Wednesday"));
    break;
    case 3:
      lcd.print(F("Thursday"));
    break;
    case 4:
      lcd.print(F("Friday"));
    break;
    case 5:
      lcd.print(F("Saturday"));
    break;
    case 6:
      lcd.print(F("Sunday"));
    break;
  }
}

void beep() {
  tone(SPKR, 250, 25);
}

void beep_high() {
  tone(SPKR, 500, 25);
}

bool btn_1_pressed = false;
uint32_t btn_1_timeout = 0;
bool btn_2_pressed = false;
uint32_t btn_2_timeout = 0;

bool check_btn_1() {
  bool pressed = !digitalRead(BTN_1);

  if (pressed && !btn_1_pressed) {
    btn_1_pressed = true;
    btn_1_timeout = millis() + 250;
    return true;
  } else if (!pressed && btn_1_pressed && millis() >= btn_1_timeout) {
    btn_1_pressed = false;
  }

  return false;
}

bool check_btn_2() {
  bool pressed = !digitalRead(BTN_2);

  if (pressed && !btn_2_pressed) {
    btn_2_pressed = true;
    btn_2_timeout = millis() + 250;
    return true;
  } else if (!pressed && btn_2_pressed && millis() >= btn_2_timeout) {
    btn_2_pressed = false;
  }

  return false;
}

bool time_count = true;
uint8_t time_hours = 0;
uint8_t time_minutes = 0;
uint8_t time_seconds = 0;
uint8_t time_week_day = 0;
bool time_updated = false;

bool alarm_set = false;
uint8_t alarm_hours = 0;
uint8_t alarm_minutes = 0;
uint8_t alarm_seconds = 0;
uint8_t alarm_tone = TONE_MEDIUM;

void update_time() {
  if (time_count) {
    time_seconds++;

    if (time_seconds == 60) {
      time_minutes++;
      time_seconds = 0;
    }

    if (time_minutes == 60) {
      time_hours++;
      time_minutes = 0;
    }

    if (time_hours == 24) {
      time_week_day++;
      time_hours = 0;
    }

    if (time_week_day == 7) {
      time_week_day = 0;
    }

    time_updated = true;
  }
}

bool in_clock = false;
bool clock_backlight_on = true;

void loop_clock() {
  bool will_refresh = false;

  if (!in_clock) {
    digitalWrite(BKLT, clock_backlight_on);
    will_refresh = true;
    in_clock = true;
  }

  if (alarm_set &&
      time_hours == alarm_hours &&
      time_minutes == alarm_minutes &&
      time_seconds == alarm_seconds) {
    mode = MODE_ALARM;
    in_clock = false;
    return;
  }

  if (check_btn_1()) {
    clock_backlight_on = !clock_backlight_on;
    digitalWrite(BKLT, clock_backlight_on);
    beep();
  }

  if (check_btn_2()) {
    mode = MODE_SETTINGS;
    in_clock = false;
    beep();
    return;
  }

  if (time_updated) {
    will_refresh = true;
  }

  if (will_refresh) {
    lcd.clear();

    lcd.setCursor(4, 0);

    lcd_write_number(time_hours);
    lcd.write(':');
    lcd_write_number(time_minutes);
    lcd.write(':');
    lcd_write_number(time_seconds);

    lcd.setCursor(0, 1);

    lcd_write_week_day(time_week_day);

    if (alarm_set) {
      lcd.setCursor(15, 0);
      lcd.write('A');
    }
  }
}

bool in_settings = false;
uint8_t item = 0;

void loop_settings() {
  bool will_refresh = false;

  if (!in_settings) {
    digitalWrite(BKLT, HIGH);
    will_refresh = true;
    item = 0;
    in_settings = true;
  }

  if (check_btn_1()) {
    switch (item) {
      case 0:
        mode = MODE_SET_TIME;
      break;
      case 1:
        mode = MODE_SET_ALARM;
      break;
      case 2:
        mode = MODE_CLOCK;
        alarm_set = false;
      break;
      case 3:
        mode = MODE_SET_TONE;
      break;
    }

    in_settings = false;
    beep_high();
    return;
  }

  if (check_btn_2()) {
    beep();
    item++;

    if (item == 4) {
      mode = MODE_CLOCK;
      in_settings = false;
      return;
    }

    will_refresh = true;
  }

  if (will_refresh) {
    lcd.clear();

    lcd.setCursor(0, 0);
    lcd.print(F("Menu"));

    lcd.setCursor(0, 1);

    switch (item) {
      case 0:
        lcd.print(F("Set time"));
      break;
      case 1:
        lcd.print(F("Set alarm"));
      break;
      case 2:
        lcd.print(F("Clear alarm"));
      break;
      case 3:
        lcd.print(F("Set alarm tone"));
      break;
    }
  }
}

bool in_set_time = false;
uint8_t set_time_stage = 0;

void loop_set_time() {
  bool will_refresh = false;

  if (!in_set_time) {
    digitalWrite(BKLT, HIGH);
    will_refresh = true;
    set_time_stage = 0;
    in_set_time = true;

    time_count = false;
    time_hours = 0;
    time_minutes = 0;
    time_seconds = 0;
    time_week_day = 0;
  }

  if (check_btn_1()) {
    switch (set_time_stage) {
      case 0:
        time_hours += 10;

        if (time_hours / 10 > 2) {
          time_hours = 0;
        }
      break;
      case 1:
        if (time_hours % 10 == 9) {
          time_hours = time_hours / 10 * 10;
        } else {
          time_hours++;
        }

        if (time_hours / 10 == 2 && time_hours % 10 > 3) {
          time_hours = time_hours / 10 * 10;
        }
      break;
      case 2:
        time_minutes += 10;

        if (time_minutes / 10 > 5) {
          time_minutes = 0;
        }
      break;
      case 3:
        if (time_minutes % 10 == 9) {
          time_minutes = time_minutes / 10 * 10;
        } else {
          time_minutes++;
        }
      break;
      case 4:
        time_seconds += 10;

        if (time_seconds / 10 > 5) {
          time_seconds = 0;
        }
      break;
      case 5:
        if (time_seconds % 10 == 9) {
          time_seconds = time_seconds / 10 * 10;
        } else {
          time_seconds++;
        }
      break;
      case 6:
        time_week_day = (time_week_day + 1) % 7;
      break;
    }

    will_refresh = true;
    beep();
  }

  if (check_btn_2()) {
    set_time_stage++;

    if (set_time_stage == 6) {
      next_time_update = millis() + 1000;
      time_count = true;
    }

    if (set_time_stage == 7) {
      mode = MODE_CLOCK;
      in_set_time = false;
      beep_high();
      return;
    }

    beep_high();
    will_refresh = true;
  }

  if (time_updated) {
    will_refresh = true;
  }

  if (will_refresh) {
    lcd.clear();

    lcd.setCursor(4, 0);

    lcd_write_number(time_hours);
    lcd.write(':');
    lcd_write_number(time_minutes);
    lcd.write(':');
    lcd_write_number(time_seconds);

    if (set_time_stage <= 5) {
      uint8_t offset = 0;

      switch (set_time_stage) {
        case 0:
          offset = 0;
        break;
        case 1:
          offset = 1;
        break;
        case 2:
          offset = 3;
        break;
        case 3:
          offset = 4;
        break;
        case 4:
          offset = 6;
        break;
        case 5:
          offset = 7;
        break;
      }

      lcd.setCursor(4+offset, 1);
      lcd.write('^');
    }

    if (set_time_stage == 6) {
      lcd.setCursor(0, 1);
      lcd_write_week_day(time_week_day);
      lcd.setCursor(15, 1);
      lcd.write('>');
    }
  }
}

bool in_set_alarm = false;
uint8_t set_alarm_stage = 0;

void loop_set_alarm() {
  bool will_refresh = false;

  if (!in_set_alarm) {
    digitalWrite(BKLT, HIGH);
    will_refresh = true;
    set_alarm_stage = 0;
    in_set_alarm = true;

    alarm_hours = 0;
    alarm_minutes = 0;
    alarm_seconds = 0;
  }

  if (check_btn_1()) {
    switch (set_alarm_stage) {
      case 0:
        alarm_hours += 10;

        if (alarm_hours / 10 > 2) {
          alarm_hours = 0;
        }
      break;
      case 1:
        if (alarm_hours % 10 == 9) {
          alarm_hours = alarm_hours / 10 * 10;
        } else {
          alarm_hours++;
        }

        if (alarm_hours / 10 == 2 && alarm_hours % 10 > 3) {
          alarm_hours = alarm_hours / 10 * 10;
        }
      break;
      case 2:
        alarm_minutes += 10;

        if (alarm_minutes / 10 > 5) {
          alarm_minutes = 0;
        }
      break;
      case 3:
        if (alarm_minutes % 10 == 9) {
          alarm_minutes = alarm_minutes / 10 * 10;
        } else {
          alarm_minutes++;
        }
      break;
      case 4:
        alarm_seconds += 10;

        if (alarm_seconds / 10 > 5) {
          alarm_seconds = 0;
        }
      break;
      case 5:
        if (alarm_seconds % 10 == 9) {
          alarm_seconds = alarm_seconds / 10 * 10;
        } else {
          alarm_seconds++;
        }
      break;
    }

    will_refresh = true;
    beep();
  }

  if (check_btn_2()) {
    set_alarm_stage++;

    if (set_alarm_stage == 6) {
      alarm_set = true;

      mode = MODE_CLOCK;
      in_set_alarm = false;
      beep_high();
      return;
    }

    beep_high();
    will_refresh = true;
  }

  if (will_refresh) {
    lcd.clear();

    lcd.setCursor(4, 0);

    lcd_write_number(alarm_hours);
    lcd.write(':');
    lcd_write_number(alarm_minutes);
    lcd.write(':');
    lcd_write_number(alarm_seconds);

    uint8_t offset = 0;

    switch (set_alarm_stage) {
      case 0:
        offset = 0;
      break;
      case 1:
        offset = 1;
      break;
      case 2:
        offset = 3;
      break;
      case 3:
        offset = 4;
      break;
      case 4:
        offset = 6;
      break;
      case 5:
        offset = 7;
      break;
    }

    lcd.setCursor(4+offset, 1);
    lcd.write('^');
  }
}

bool in_set_tone = false;
uint32_t set_tone_next_beep = 0;
bool set_tone_even_beep = false;

void loop_set_tone() {
  bool will_refresh = false;

  if (!in_set_tone) {
    digitalWrite(BKLT, HIGH);
    set_tone_next_beep = millis() + 500;
    set_tone_even_beep = false;

    will_refresh = true;
    in_set_tone = true;
  }

  if (check_btn_1()) {
    noTone(SPKR);
    beep_high();
    mode = MODE_CLOCK;
    in_set_tone = false;
    return;
  }

  if (check_btn_2()) {
    alarm_tone = (alarm_tone + 1) % 3;
    set_tone_next_beep = millis();
    set_tone_even_beep = false;
    will_refresh = true;
  }

  if (millis() >= set_tone_next_beep) {
    if (!set_tone_even_beep) {
      switch (alarm_tone) {
        case TONE_LOW:
          tone(SPKR, 125);
        break;
        case TONE_MEDIUM:
          tone(SPKR, 250);
        break;
        case TONE_HIGH:
          tone(SPKR, 500);
        break;
      }

      set_tone_even_beep = true;
    } else {
      noTone(SPKR);
      set_tone_even_beep = false;
    }

    set_tone_next_beep += 500;
  }

  if (will_refresh) {
    lcd.clear();

    lcd.setCursor(0, 0);
    lcd.print(F("Set alarm tone"));

    lcd.setCursor(0, 1);

    switch (alarm_tone) {
      case TONE_LOW:
        lcd.print(F("Low"));
      break;
      case TONE_MEDIUM:
        lcd.print(F("Medium"));
      break;
      case TONE_HIGH:
        lcd.print(F("High"));
      break;
    }
  }
}

bool in_alarm = false;
uint32_t alarm_next_beep = 0;
bool alarm_even_beep = false;

void loop_alarm() {
  if (!in_alarm) {
    in_alarm = true;
    alarm_next_beep = millis();
    alarm_even_beep = false;

    lcd.clear();
    
    lcd.setCursor(5, 0);
    lcd.print(F("Alarm!"));
  }

  if (check_btn_1() || check_btn_2()) {
    noTone(SPKR);
    mode = MODE_CLOCK;
    in_alarm = false;
    return;
  }

  if (millis() >= alarm_next_beep) {
    if (!alarm_even_beep) {
      digitalWrite(BKLT, HIGH);
      
      switch (alarm_tone) {
        case TONE_LOW:
          tone(SPKR, 125);
        break;
        case TONE_MEDIUM:
          tone(SPKR, 250);
        break;
        case TONE_HIGH:
          tone(SPKR, 500);
        break;
      }

      alarm_even_beep = true;
    } else {
      digitalWrite(BKLT, LOW);
      noTone(SPKR);
      alarm_even_beep = false;
    }

    alarm_next_beep += 500;
  }
}

void setup() {
  pinMode(BTN_1, INPUT_PULLUP);
  pinMode(BTN_2, INPUT_PULLUP);

  pinMode(SPKR, OUTPUT);

  pinMode(BKLT, OUTPUT);

  pinMode(LED, OUTPUT);

  digitalWrite(LED, LOW);
  digitalWrite(BKLT, HIGH);

  lcd.begin(16, 2);
  lcd.clear();

  lcd.setCursor(0, 0);
  lcd.print(F("Welcome"));
  lcd.setCursor(0, 1);
  lcd.print(F("Have a good day"));

  tone(SPKR, 500, 250);
  delay(2000);
}

void loop() {
  if (millis() >= next_time_update) {
    update_time();
    next_time_update += 1000;
  }

  switch (mode) {
    case MODE_CLOCK:
      loop_clock();
    break;
    case MODE_SETTINGS:
      loop_settings();
    break;
    case MODE_SET_TIME:
      loop_set_time();
    break;
    case MODE_SET_ALARM:
      loop_set_alarm();
    break;
    case MODE_SET_TONE:
      loop_set_tone();
    break;
    case MODE_ALARM:
      loop_alarm();
    break;
  }

  time_updated = false;
}
