void spkr_beep() {
  if (bitRead(local_storage.flags, 0)) {
    tone(SPKR, 250, 50);
  }
}

void spkr_beep_confirm() {
  if (bitRead(local_storage.flags, 0)) {
    tone(SPKR, 500, 50);;
  }
}

void spkr_stop() {
  noTone(SPKR);
}
