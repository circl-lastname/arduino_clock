uint16_t alarms_amount() {
  uint16_t i = 0;

  for (; i < MAX_ALARMS; i++) {
    if (!bitRead(local_storage.alarms[i].days_flags, 7)) {
      break;
    }
  }

  return i;
}

void alarms_add(uint8_t hours, uint8_t minutes, uint8_t seconds, uint8_t days) {
  uint16_t i = 0;

  for (; i < MAX_ALARMS; i++) {
    if (!bitRead(local_storage.alarms[i].days_flags, 7)) {
      break;
    }
  }

  local_storage.alarms[i].hours = hours;
  local_storage.alarms[i].minutes = minutes;
  local_storage.alarms[i].seconds = seconds;
  local_storage.alarms[i].days_flags = days | 0x80;

  storage_modified();
}

void alarms_delete(uint16_t i) {
  i++;

  if (i == MAX_ALARMS) {
    i--;
    local_storage.alarms[i].days_flags = 0;
    return;
  }

  for (; i < MAX_ALARMS; i++) {
    local_storage.alarms[i-1].hours = local_storage.alarms[i].hours;
    local_storage.alarms[i-1].minutes = local_storage.alarms[i].minutes;
    local_storage.alarms[i-1].seconds = local_storage.alarms[i].seconds;
    local_storage.alarms[i-1].days_flags = local_storage.alarms[i].days_flags;
  }

  local_storage.alarms[i].days_flags = 0;

  storage_modified();
}

bool alarms_check() {
  for (uint16_t i = 0; i < MAX_ALARMS; i++) {
    if (!bitRead(local_storage.alarms[i].days_flags, 7)) {
      break;
    }

    if (bitRead(local_storage.alarms[i].days_flags, time_week_day) &&
        local_storage.alarms[i].hours == time_hours &&
        local_storage.alarms[i].minutes == time_minutes &&
        local_storage.alarms[i].seconds == time_seconds) {
      return true;
    }
  }

  return false;
}