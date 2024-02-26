bool time_count = true;
uint8_t time_hours = 0;
uint8_t time_minutes = 0;
uint8_t time_seconds = 0;
uint8_t time_week_day = 0;
bool time_updated = false;

void time_update() {
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
      time_week_day = (time_week_day+1) % 7;
      time_hours = 0;
    }

    time_updated = true;
  }
}