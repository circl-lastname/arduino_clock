bool display_state = false;

void display_set(bool value) {
  if (value) {
    if (!display_state) {
      digitalWrite(LCD_VDD, HIGH);
      lcd.begin(16, 2);
      display_state = true;
    }
  } else {
    digitalWrite(LCD_VDD, LOW);
    display_state = false;
  }
}
