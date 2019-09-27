/*
  Скетч к проекту "Бегущая строка"
  Страница проекта (схемы, описания): https://alexgyver.ru/GyverString/
  Исходники на GitHub: https://github.com/AlexGyver/GyverString/
  Нравится, как написан код? Поддержи автора! https://alexgyver.ru/support_alex/
  Автор: AlexGyver, AlexGyver Technologies, 2019
  https://AlexGyver.ru/
*/

//  Версия 1.2 - исправлена ошибка с количеством матриц
// Версия 1.3 - исправлена скорость

// ================ НАСТРОЙКИ ================
#define MATR_NUM 4            // количество матриц последовательно

// ============ ДЛЯ РАЗРАБОТЧИКОВ ============
// ПИНЫ
#define BT_RX 3
#define BT_TX 2

// БИБЛИОТЕКИ
#include <SoftwareSerial.h>
#include <avr/eeprom.h>
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Max72xxPanel.h>
#include "fonts.h"
#include "timerMinim.h"

#define WIDTH MATR_NUM*8      // ширина матрицы
#define HEIGHT 8              // высота матрицы

Max72xxPanel matrix = Max72xxPanel(10, MATR_NUM, 1);
SoftwareSerial btSerial(BT_TX, BT_RX); // RX, TX
timerMinim scrollTimer(100);
timerMinim eepromTimer(5000);

String runningText = "";
boolean loadingFlag, fullTextFlag, runningState = true, eepromFlag = false, autoFlag = true, showText = true;
byte thisSpeed, thisBright, thisMode, thisColor, thisLength;

void setup() {
  //Serial.begin(9600);
  btSerial.begin(9600);
  btSerial.setTimeout(100);
  randomSeed(analogRead(0));
  if (eeprom_read_byte((uint8_t*)1023) != 123) {
    eeprom_write_byte((uint8_t*)1023, 123);
    for (int i = 10; i < 300; i++) eeprom_write_byte((uint8_t*)i, 0);
    eeprom_write_byte((uint8_t*)0, 60);
    eeprom_write_byte((uint8_t*)1, 60);
    eeprom_write_byte((uint8_t*)2, 0);
    eeprom_write_byte((uint8_t*)3, 0);
    eeprom_write_byte((uint8_t*)4, 0);
  }
  thisSpeed = eeprom_read_byte((uint8_t*)0);
  thisBright = eeprom_read_byte((uint8_t*)1);
  thisMode = eeprom_read_byte((uint8_t*)2);
  thisColor = eeprom_read_byte((uint8_t*)3);
  thisLength = eeprom_read_byte((uint8_t*)4);

  for (byte i = 10; i < thisLength + 10; i++) {
    runningText += (char)eeprom_read_byte((uint8_t*)i);
  }

  matrix.setIntensity(50);
  for (byte i = 0; i < MATR_NUM; i++) {
    // матрицы расположены криво, здесь поворачиваем
    matrix.setRotation(i, 1);
  }
  matrix.fillScreen(LOW);
  matrix.write();

  scrollTimer.setInterval(thisSpeed);
}

void loop() {
  bluetoothTick();  // парсинг блютус
  if (runningState) fillString(runningText, thisMode);
  eepromTick();
}

void toggleText(boolean state) {
  if (state) {
    runningState = true;
    loadingFlag = true;
  } else {
    runningState = false;
    matrix.fillScreen(LOW);
    matrix.write();
  }
}
