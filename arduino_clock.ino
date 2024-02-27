#include <EEPROM.h>
#include <LiquidCrystal.h>

#include "config.h"

LiquidCrystal lcd(LCD_RS, LCD_E, LCD_B4, LCD_B5, LCD_B6, LCD_B7);

typedef struct {
  uint8_t days_flags;
  uint8_t hours;
  uint8_t minutes;
  uint8_t seconds;
} alarm_t;

#include "interval.h"

interval_t time_update_interval = { 1000 };

#include "storage.h"
#include "display.h"
#include "bklt.h"
#include "spkr.h"
#include "btn.h"
#include "ui.h"
#include "time.h"
#include "alarms.h"
#include "tone.h"
#include "mode.h"

#include "clock.h"
#include "menu.h"
#include "set_time.h"
#include "add_alarm.h"
#include "edit_alarm.h"
#include "set_tone.h"
#include "set_beep.h"
#include "alarm.h"

#include "mode_funcs.h"

void setup() {
  pinMode(BTN_OK, INPUT_PULLUP);
  pinMode(BTN_RIGHT, INPUT_PULLUP);
  pinMode(SPKR, OUTPUT);
  pinMode(BKLT, OUTPUT);
  pinMode(LCD_VDD, OUTPUT);
  pinMode(LED, OUTPUT);

  digitalWrite(LED, LOW);

  display_set(true);
  bklt_set(true);

  lcd.clear();

  if (EEPROM.length() < sizeof(storage_t)) {
    lcd.setCursor(0, 0);
    lcd.print(F("Error"));
    lcd.setCursor(0, 1);
    lcd.print(F("Bad MAX_ALARMS"));

    while (true) {}
  }

  if (!storage_check_signature() || btn_right_check()) {
    spkr_beep_confirm();

    storage_commit();

    lcd.setCursor(0, 0);
    lcd.print(F("Factory settings"));

    delay(500);
  }

  lcd.clear();

  lcd.setCursor(4, 0);
  lcd.print(F("Welcome!"));

  spkr_beep();

  delay(2000);

  storage_load();

  interval_reset(&time_update_interval);
  mode_enter();
}

void loop() {
  if (interval_check(&time_update_interval)) {
    time_update();
  }

  if (time_updated) {
    mode_time_updated();
  }

  if (btn_ok_check()) {
    mode_btn_ok();
  }

  if (btn_right_check()) {
    mode_btn_right();
  }

  mode_update();

  storage_maybe_commit();
  time_updated = false;
}