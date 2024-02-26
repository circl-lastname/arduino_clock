// Example mode
bool dummy_will_redraw = false;

void dummy_enter() {
  bklt_set(true);
  dummy_will_redraw = true;
}

void dummy_update() {
  if (dummy_will_redraw) {
    lcd.clear();

    dummy_will_redraw = false;
  }
}

void dummy_btn_ok() {

}

void dummy_btn_right() {

}

void dummy_time_updated() {
  
}