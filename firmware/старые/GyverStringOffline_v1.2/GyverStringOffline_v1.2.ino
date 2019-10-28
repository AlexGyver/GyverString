/*
  Скетч к проекту "Бегущая строка"
  Страница проекта (схемы, описания): https://alexgyver.ru/GyverString/
  Исходники на GitHub: https://github.com/AlexGyver/GyverString/
  Нравится, как написан код? Поддержи автора! https://alexgyver.ru/support_alex/
  Автор: AlexGyver, AlexGyver Technologies, 2019
  https://AlexGyver.ru/

  Версия 1.1: прошивка оптимизирована под широкие матрицы (до 80 пикс)
*/

#define C_BLACK 0x000000
#define C_GREEN 0x00FF00
#define C_RED 0xFF0000
#define C_BLUE 0x0000FF
#define C_PURPLE 0xFF00FF
#define C_YELLOW 0xFFFF00
#define C_CYAN 0x00FFFF
#define C_WHITE 0xFFFFFF

String text1 = "продам ";
#define COLOR1 C_WHITE

String text2 = "гараж ";
#define COLOR2 C_RED

String text3 = "тел. ";
#define COLOR3 C_WHITE

String text4 = "+7(945)259-63-54 ";
#define COLOR4 C_BLUE

String text5 = "состояние ";
#define COLOR5 C_WHITE

String text6 = "ОТЛИЧНОЕ, ";
#define COLOR6 C_GREEN

String text7 = "";
#define COLOR7 C_YELLOW

String text8 = "";
#define COLOR8 C_WHITE

String text9 = "";
#define COLOR9 C_WHITE

String text10 = "";
#define COLOR10 C_WHITE

// ================ НАСТРОЙКИ ================
#define BRIGHTNESS 150        // стандартная яркость (0-255)
#define D_TEXT_SPEED 50       // скорость бегущего текста по умолчанию (мс)

#define CURRENT_LIMIT 2500    // лимит по току в миллиамперах, автоматически управляет яркостью (пожалей свой блок питания!) 0 - выключить лимит

#define WIDTH 42              // ширина матрицы
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
//#include <FastLED.h>
#define COLOR_DEBTH 2   // цветовая глубина: 1, 2, 3 (в байтах)
#define ORDER_GRB       // ORDER_GRB / ORDER_RGB

#include "microLED.h"
#include "fonts.h"

const int NUM_LEDS = WIDTH * HEIGHT * SEGMENTS;
LEDdata leds[NUM_LEDS];
microLED strip(leds, NUM_LEDS, LED_PIN);  // объект лента

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
  //Serial.begin(9600);
  randomSeed(analogRead(0));

  // настройки ленты
  //strip.addLeds<WS2812, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );
  strip.setBrightness(BRIGHTNESS);
  //if (CURRENT_LIMIT > 0) strip.setMaxPowerInVoltsAndMilliamps(5, CURRENT_LIMIT);
  strip.clear();
  strip.show();

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
