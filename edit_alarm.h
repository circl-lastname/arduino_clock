bool edit_alarm_will_redraw = false;
uint16_t edit_alarm_item = 0;
bool edit_alarm_end = false;
uint8_t edit_alarm_action = 0;
bool edit_alarm_action_select = false;

void edit_alarm_enter() {
  bklt_set(true);
  edit_alarm_will_redraw = true;
  edit_alarm_item = 0;
  edit_alarm_action = 0;
  edit_alarm_action_select = false;

  if (alarms_amount() == 0) {
    edit_alarm_end = true;
  } else {
    edit_alarm_end = false;
  }
}

void edit_alarm_update() {
  if (edit_alarm_will_redraw) {
    lcd.clear();

    if (edit_alarm_action_select) {
      lcd.setCursor(0, 0);
      lcd.print(F("Select action"));

      lcd.setCursor(0, 1);

      switch (edit_alarm_action) {
        case 0:
          lcd.print(F("Change"));
        break;
        case 1:
          lcd.print(F("Delete"));
        break;
      }
    } else if (!edit_alarm_end) {
      lcd.setCursor(4, 0);
      ui_draw_time(local_storage.alarms[edit_alarm_item].hours, local_storage.alarms[edit_alarm_item].minutes, local_storage.alarms[edit_alarm_item].seconds);
      lcd.setCursor(0, 1);
      ui_draw_week_days(local_storage.alarms[edit_alarm_item].days_flags);
    } else {
      lcd.setCursor(0, 0);
      lcd.print(F("End of list"));
    }

    edit_alarm_will_redraw = false;
  }
}

void edit_alarm_btn_ok() {
  if (edit_alarm_action_select) {
    spkr_beep_confirm();
    
    switch (edit_alarm_action) {
      case 0:
        storage_commit_enabled = false;
        local_storage.alarms[edit_alarm_item].days_flags = 0;
        mode_switch(MODE_ADD_ALARM);
      return;
      case 1:
        alarms_delete(edit_alarm_item);
        mode_switch(MODE_CLOCK);
      return;
    }

    edit_alarm_will_redraw = true;
  } else if (!edit_alarm_end) {
    spkr_beep_confirm();
    edit_alarm_action_select = true;

    edit_alarm_will_redraw = true;
  }
}

void edit_alarm_btn_right() {
  spkr_beep();

  if (edit_alarm_action_select) {
    edit_alarm_action = (edit_alarm_action+1) % 2;
  } else if (!edit_alarm_end) {
    edit_alarm_item++;

    if (alarms_amount() == edit_alarm_item) {
      edit_alarm_end = true;
    }
  } else {
    mode_switch(MODE_CLOCK);
    return;
  }

  edit_alarm_will_redraw = true;
}

void edit_alarm_time_updated() {
  
}