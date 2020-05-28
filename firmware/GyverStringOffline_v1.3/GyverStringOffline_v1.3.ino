/*
  Скетч к проекту "Бегущая строка"
  Страница проекта (схемы, описания): https://alexgyver.ru/GyverString/
  Исходники на GitHub: https://github.com/AlexGyver/GyverString/
  Нравится, как написан код? Поддержи автора! https://alexgyver.ru/support_alex/
  Автор: AlexGyver, AlexGyver Technologies, 2019
  https://AlexGyver.ru/

  Версия 1.1: прошивка оптимизирована под широкие матрицы (до 80 пикс)
  Версия 1.3: исправлен баг с красным цветом
*/
#include "ctext.h"
#define ostr(n,s) const char n[] PROGMEM = s

const int strcount = 6;

ostr(string_1,"продам ");
ostr(string_2,"гараж ");
ostr(string_3,"тел. ");
ostr(string_4,"+7(945)259-63-54 ");
ostr(string_5,"состояние ");
ostr(string_6,"ОТЛИЧНОЕ, ");

const char *const strings[] PROGMEM = {
  string_1,
  string_2,
  string_3,
  string_4,
  string_5,
  string_6,
};
const uint32_t scolors[] PROGMEM = {
  C_WHITE,
  C_RED,
  C_WHITE,
  C_BLUE,
  C_WHITE,
  C_GREEN,
};
const uint8_t sanims[] PROGMEM = {
  RIGHT_TO_LEFT,
  LEFT_TO_RIGHT,
  UP_TO_DOWN,
  DOWN_TO_UP,
  RIGHT_TO_LEFT,
  LEFT_TO_RIGHT,
};
const uint16_t sticktimes[] PROGMEM = {
  0,
  0,
  0,
  1000,
  200,
  1000
};
const uint8_t rainbows[] PROGMEM = {
  0,
  0,
  0,
  0,
  0,
  255,
};
const uint8_t srainbowfades[] PROGMEM = {
  RB_CONSTANT,
  RB_CONSTANT,
  RB_CONSTANT,
  RB_CONSTANT,
  RB_FADE_IN,
  RB_FADE_OUT,
};

// ================ НАСТРОЙКИ ================
#define BRIGHTNESS 150        // стандартная яркость (0-255)
#define D_TEXT_SPEED 50       // скорость бегущего текста по умолчанию (мс/пиксель)

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
#include "textcfg.h"

const int NUM_LEDS = WIDTH * HEIGHT * SEGMENTS;
LEDdata leds[NUM_LEDS];
microLED strip(leds, NUM_LEDS, LED_PIN);  // объект лента

uint32_t scrollTimer;
boolean loadingFlag, fullTextFlag;

ctext runningText;
int current_text = 0;

int xoffset;
int yoffset;

int dxoffset; // default offset
int dyoffset;

int cxoffset; // offset direction
int cyoffset;

int stxpos; // stop position
int stypos;

int htxpos; // stick position
int htypos;
int sticktimer;
int sticking = false;

void setctext() {
  runningText = get_ctext(current_text);
  int len = stringLength(runningText.text)*(LET_WIDTH+SPACE);
  dxoffset=dyoffset=cxoffset=cyoffset=stxpos=stypos=htxpos=htypos=0;
  switch(runningText.anim) {
    case RIGHT_TO_LEFT:
      dxoffset = WIDTH;
      cxoffset = -1;
      stxpos = -(len);
      htxpos = 0;
      break;
    case LEFT_TO_RIGHT:
      dxoffset = -len;
      cxoffset = 1;
      htxpos = WIDTH-(len);
      stxpos = WIDTH;
      break;
    case UP_TO_DOWN:
      dyoffset = HEIGHT;
      cyoffset = -1;
      stypos = -LET_HEIGHT+TEXT_HEIGHT;
      htypos = 0+TEXT_HEIGHT;
      break;
    case DOWN_TO_UP:
      dyoffset = -LET_HEIGHT;
      cyoffset = 1;
      htypos = HEIGHT-LET_HEIGHT;
      stypos = HEIGHT;
      break;
  }
  xoffset = dxoffset;
  yoffset = dyoffset;
}

void setup() {
  //Serial.begin(9600);
  randomSeed(analogRead(0));
  
  // настройки ленты
  //strip.addLeds<WS2812, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );
  strip.setBrightness(BRIGHTNESS);
  //if (CURRENT_LIMIT > 0) strip.setMaxPowerInVoltsAndMilliamps(5, CURRENT_LIMIT);
  strip.clear();
  strip.show();
  
  setctext();
}

void loop() {
  fillString(runningText);
  if (fullTextFlag) {
    current_text = (current_text+1)%strcount;
    setctext();
    fullTextFlag = false;
  }
}

ctext get_ctext(int i) {
  String text = String((char *)pgm_read_word(&(strings[i])));
  uint8_t anim = pgm_read_byte_near(sanims+i);
  uint16_t sticktime = pgm_read_byte_near(sticktimes+i);
  uint32_t color = pgm_read_dword_near(scolors+i);
  uint8_t rainbow = pgm_read_byte_near(rainbows+i);
  uint8_t rainbowfade = pgm_read_byte_near(srainbowfades+i);
  return ctext {
    color,
    anim,
    sticktime,
    rainbow,
    rainbowfade,
    text
  };
}
