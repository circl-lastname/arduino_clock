void ui_draw_number(uint8_t value) {
  if (value <= 99) {
    lcd.write(0x30 + value / 10);
    lcd.write(0x30 + value % 10);
  } else {
    lcd.print(F("??"));
  }
}

void ui_draw_time(uint8_t hours, uint8_t minutes, uint8_t seconds) {
  ui_draw_number(hours);
  lcd.write(':');
  ui_draw_number(minutes);
  lcd.write(':');
  ui_draw_number(seconds);
}

void ui_draw_week_day(uint8_t value) {
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

void ui_draw_week_days(uint8_t value) {
  #define TRY_DRAW(OFFSET, LETTER) if (bitRead(value, OFFSET)) lcd.write(LETTER); else lcd.write('_');

  TRY_DRAW(0, 'M')
  TRY_DRAW(1, 'T')
  TRY_DRAW(2, 'W')
  TRY_DRAW(3, 'T')
  TRY_DRAW(4, 'F')
  TRY_DRAW(5, 'S')
  TRY_DRAW(6, 'S')

  #undef TRY_DRAW
}

void ui_time_entry_draw(uint8_t hours, uint8_t minutes, uint8_t seconds, uint8_t stage) {
  lcd.setCursor(4, 0);
  ui_draw_time(hours, minutes, seconds);

  uint8_t offset = 0;

  if (stage < 6) {
    switch (stage) {
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

void ui_time_entry_update(uint8_t* hours, uint8_t* minutes, uint8_t* seconds, uint8_t stage) {
  switch (stage) {
    case 0:
      (*hours) += 10;

      if ((*hours) / 10 > 2) {
        (*hours) = 0;
      }
    break;
    case 1:
      if ((*hours) % 10 == 9) {
        (*hours) = (*hours) / 10 * 10;
      } else {
        (*hours)++;
      }

      if ((*hours) / 10 == 2 && (*hours) % 10 > 3) {
        (*hours) = (*hours) / 10 * 10;
      }
    break;
    case 2:
      (*minutes) += 10;

      if ((*minutes) / 10 > 5) {
        (*minutes) = 0;
      }
    break;
    case 3:
      if ((*minutes) % 10 == 9) {
        (*minutes) = (*minutes) / 10 * 10;
      } else {
        (*minutes)++;
      }
    break;
    case 4:
      (*seconds) += 10;

      if ((*seconds) / 10 > 5) {
        (*seconds) = 0;
      }
    break;
    case 5:
      if ((*seconds) % 10 == 9) {
        (*seconds) = (*seconds) / 10 * 10;
      } else {
        (*seconds)++;
      }
    break;
  }
}