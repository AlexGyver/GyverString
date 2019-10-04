/*
  Скетч к проекту "Бегущая строка"
  Страница проекта (схемы, описания): https://alexgyver.ru/GyverString/
  Исходники на GitHub: https://github.com/AlexGyver/GyverString/
  Нравится, как написан код? Поддержи автора! https://alexgyver.ru/support_alex/
  Автор: AlexGyver, AlexGyver Technologies, 2019
  https://AlexGyver.ru/

  Версия 1.1: прошивка оптимизирована под широкие матрицы (до 80 пикс)
*/

// ================ НАСТРОЙКИ ================
#define BRIGHTNESS 50         // стандартная яркость (0-255)
#define CURRENT_LIMIT 2000    // лимит по току в миллиамперах, автоматически управляет яркостью (пожалей свой блок питания!) 0 - выключить лимит

#define WIDTH 42              // ширина матрицы
#define HEIGHT 8              // высота матрицы
#define SEGMENTS 1            // диодов в одном "пикселе" (для создания матрицы из кусков ленты)

#define COLOR_ORDER GRB       // порядок цветов на ленте. Если цвет отображается некорректно - меняйте. Начать можно с RGB

#define MATRIX_TYPE 0         // тип матрицы: 0 - зигзаг, 1 - параллельная
#define CONNECTION_ANGLE 0    // угол подключения: 0 - левый нижний, 1 - левый верхний, 2 - правый верхний, 3 - правый нижний
#define STRIP_DIRECTION 0     // направление ленты из угла: 0 - вправо, 1 - вверх, 2 - влево, 3 - вниз
// при неправильной настрйоке матрицы вы получите предупреждение "Wrong matrix parameters! Set to default"
// шпаргалка по настройке матрицы здесь! https://alexgyver.ru/matrix_guide/

#define D_TEXT_SPEED 100      // скорость бегущего текста по умолчанию (мс)
// ============ ДЛЯ РАЗРАБОТЧИКОВ ============
// ПИНЫ
#define BT_RX 3
#define BT_TX 2
#define LED_PIN 4

// БИБЛИОТЕКИ
#include <SoftwareSerial.h>
#include <avr/eeprom.h>

#define COLOR_DEBTH 2   // цветовая глубина: 1, 2, 3 (в байтах)
#define ORDER_GRB       // ORDER_GRB / ORDER_RGB
#include "microLED.h"

#include "fonts.h"
#include "timerMinim.h"

const int NUM_LEDS = WIDTH * HEIGHT * SEGMENTS;
LEDdata leds[NUM_LEDS];
microLED strip(leds, NUM_LEDS, LED_PIN);  // объект лента
SoftwareSerial btSerial(BT_TX, BT_RX); // RX, TX
timerMinim scrollTimer(D_TEXT_SPEED);
timerMinim eepromTimer(5000);

String runningText = "";
boolean loadingFlag, fullTextFlag, runningState = true, eepromFlag = false, autoFlag = true, showText = true;
byte thisSpeed, thisBright, thisMode, thisColor, thisLength;

void setup() {
  //Serial.begin(9600);
  btSerial.begin(9600);
  btSerial.setTimeout(100);
  randomSeed(analogRead(0));
  if (eeprom_read_byte((uint8_t*)1023) != 100) {
    eeprom_write_byte((uint8_t*)1023, 100);
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

  if (thisLength > 0) {
    for (byte i = 10; i < thisLength + 10; i++) {
      runningText += (char)eeprom_read_byte((uint8_t*)i);
    }
  }

  // настройки ленты
  //strip.addLeds<WS2812, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );
  strip.setBrightness(thisBright);
  //if (CURRENT_LIMIT > 0) strip.setMaxPowerInVoltsAndMilliamps(5, CURRENT_LIMIT);
  strip.clear();
  strip.show();

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
    strip.clear();
    strip.show();
  }
}
