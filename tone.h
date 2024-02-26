uint8_t tone_get() {
  return (local_storage.flags >> 1) & 0x03;
}

void tone_set(uint8_t value) {
  if (value <= 3) {
    local_storage.flags = (local_storage.flags & 0xF9) | (value << 1);
    storage_modified();
  }
}

bool tone_update(interval_t* interval, bool* even) {
  bool check = interval_check(interval);

  if (check) {
    if (!(*even)) {
      switch (tone_get()) {
        case 0:
          tone(SPKR, 125);
        break;
        case 1:
          tone(SPKR, 250);
        break;
        case 2:
          tone(SPKR, 500);
        break;
      }

      *even = true;
    } else {
      spkr_stop();
      *even = false;
    }
  }

  return check;
}