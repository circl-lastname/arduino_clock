typedef struct {
  uint16_t time;
  uint32_t next_trigger;
} interval_t;

void interval_reset(interval_t* interval) {
  interval->next_trigger = millis() + interval->time;
}

bool interval_check(interval_t* interval) {
  if (millis() >= interval->next_trigger) {
    interval->next_trigger += interval->time;
    return true;
  } else {
    return false;
  }
}
