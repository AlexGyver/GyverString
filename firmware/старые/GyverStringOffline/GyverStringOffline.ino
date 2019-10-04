/*
  Скетч к проекту "Бегущая строка"
  Страница проекта (схемы, описания): https://alexgyver.ru/GyverString/
  Исходники на GitHub: https://github.com/AlexGyver/GyverString/
  Нравится, как написан код? Поддержи автора! https://alexgyver.ru/support_alex/
  Автор: AlexGyver, AlexGyver Technologies, 2019
  https://AlexGyver.ru/
*/

String text1 = "продам ";
#define COLOR1 CRGB::White

String text2 = "гараж ";
#define COLOR2 CRGB::Red

String text3 = "тел. ";
#define COLOR3 CRGB::White

String text4 = "+7(945)259-63-54 ";
#define COLOR4 CRGB::Blue

String text5 = "состояние ";
#define COLOR5 CRGB::White

String text6 = "ОТЛИЧНОЕ, ";
#define COLOR6 CRGB::Green

String text7 = "";
#define COLOR7 CRGB::Yellow

String text8 = "";
#define COLOR8 CRGB::White

String text9 = "";
#define COLOR9 CRGB::White

String text10 = "";
#define COLOR10 CRGB::White

// ================ НАСТРОЙКИ ================
#define BRIGHTNESS 150         // стандартная яркость (0-255)
#define D_TEXT_SPEED 50      // скорость бегущего текста по умолчанию (мс)

#define CURRENT_LIMIT 2500    // лимит по току в миллиамперах, автоматически управляет яркостью (пожалей свой блок питания!) 0 - выключить лимит

#define WIDTH 60              // ширина матрицы
#define HEIGHT 8              // высота матрицы
#define SEGMENTS 1            // диодов в одном "пикселе" (для создания матрицы из кусков ленты)

#define COLOR_ORDER GRB       // порядок цветов на ленте. Если цвет отображается некорректно - меняйте. Начать можно с RGB

#define MATRIX_TYPE 0         // тип матрицы: 0 - зигзаг, 1 - параллельная
#define CONNECTION_ANGLE 0    // угол подключения: 0 - левый нижний, 1 - левый верхний, 2 - правый верхний, 3 - правый нижний
#define STRIP_DIRECTION 0     // направление ленты из угла: 0 - вправо, 1 - вверх, 2 - влево, 3 - вниз
// при неправильной настрйоке матрицы вы получите предупреждение "Wrong matrix parameters! Set to default"
// шпаргалка по настройке матрицы здесь! https://alexgyver.ru/matrix_guide/

// ============ ДЛЯ РАЗРАБОТЧИКОВ ============
// ПИНЫ
#define LED_PIN 4

// БИБЛИОТЕКИ
#include <FastLED.h>
#include "fonts.h"

const int NUM_LEDS = WIDTH * HEIGHT * SEGMENTS;
CRGB leds[NUM_LEDS];

uint32_t scrollTimer;
String runningText = "";
boolean loadingFlag, fullTextFlag;
const uint32_t textColors[] PROGMEM = {
  COLOR1,
  COLOR2,
  COLOR3,
  COLOR4,
  COLOR5,
  COLOR6,
  COLOR7,
  COLOR8,
  COLOR9,
  COLOR10,
};
int colorChange[10];

void setup() {
  Serial.begin(9600);
  randomSeed(analogRead(0));

  // настройки ленты
  FastLED.addLeds<WS2812, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );
  FastLED.setBrightness(BRIGHTNESS);
  if (CURRENT_LIMIT > 0) FastLED.setMaxPowerInVoltsAndMilliamps(5, CURRENT_LIMIT);
  FastLED.clear();
  FastLED.show();

  runningText = String(text1) + text2 + text3 + text4 + text5 + text6 + text7 + text8 + text9 + text10;
  colorChange[0] = stringLength(text1);
  colorChange[1] = colorChange[0] + stringLength(text2);
  colorChange[2] = colorChange[1] + stringLength(text3);
  colorChange[3] = colorChange[2] + stringLength(text4);
  colorChange[4] = colorChange[3] + stringLength(text5);
  colorChange[5] = colorChange[4] + stringLength(text6);
  colorChange[6] = colorChange[5] + stringLength(text7);
  colorChange[7] = colorChange[6] + stringLength(text8);
  colorChange[8] = colorChange[7] + stringLength(text9);
  colorChange[9] = colorChange[8] + stringLength(text10);
}

void loop() {
  fillString(runningText);
}
