typedef struct {
  char signature[3];
  uint8_t flags;

  alarm_t alarms[MAX_ALARMS];
} storage_t;

bool local_storage_modified = false;
storage_t local_storage = {
  .signature = {'C', 'L', 'K'},
  .flags = 0x03, // Button beeping = On
                 // Alarm tone = Medium
};

interval_t storage_commit_interval = { 60*1000 };
bool storage_commit_enabled = true;

bool storage_check_signature() {
  return (EEPROM.read(0) == 'C' &&
          EEPROM.read(1) == 'L' &&
          EEPROM.read(2) == 'K');
}

void storage_commit() {
  digitalWrite(LED, HIGH);
  EEPROM.put(0, local_storage);
  digitalWrite(LED, LOW);
  local_storage_modified = false;
}

void storage_maybe_commit() {
  if (storage_commit_enabled && local_storage_modified && interval_check(&storage_commit_interval)) {
    storage_commit();
  }
}

void storage_load() {
  EEPROM.get(0, local_storage);
}

void storage_modified() {
  local_storage_modified = true;
  interval_reset(&storage_commit_interval);
}