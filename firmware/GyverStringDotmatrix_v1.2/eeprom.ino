// --------------- РАБОТА С EEPROM --------------
// EEPROM MAP
// 0 - скорость
// 1 - яркость
// 2 - режим
// 3 - цвет
// 4 - длина строки
// 10-300 - строка


void eeprom_update_byte(int addr, byte value) {
  byte buf = eeprom_read_byte((uint8_t*)addr);
  if (value != buf) eeprom_write_byte((uint8_t*)addr, value);
}

void updateSettings() {
  eepromFlag = true;    // ставим галочку, что данные изменились
  eepromTimer.reset();
}

void eepromTick() {
  eepromFlag = false;
  eeprom_update_byte(0, thisSpeed);
  eeprom_update_byte(1, thisBright);
  eeprom_update_byte(2, thisMode);
  eeprom_update_byte(3, thisColor);
  eeprom_update_byte(4, thisLength);

  for (byte i = 10; i < thisLength + 10; i++) {
    eeprom_update_byte(i, runningText[i - 10]);
  }
}
