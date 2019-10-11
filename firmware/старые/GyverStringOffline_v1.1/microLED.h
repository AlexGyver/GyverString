#pragma once	// отличная замена ifdef имя библиотеки

#ifndef COLOR_DEBTH
#define COLOR_DEBTH 3	// по умолчанию 24 бита
#endif

#ifdef REPLACE_FASTLED
#define CRGB mRGB
#define CHSV mHSV
#endif

#if defined(ORDER_GRB)
#define ORDER0	1
#define ORDER1	0
#define ORDER2	2
#elif defined(ORDER_RGB)
#define ORDER0	0
#define ORDER1	1
#define ORDER2	2
#else
#define ORDER0	0
#define ORDER1	1
#define ORDER2	2
#endif

// ====== DATA ======
#if (COLOR_DEBTH == 1)
typedef uint8_t LEDdata;
static byte bright = 32;
#elif (COLOR_DEBTH == 2)
typedef uint16_t LEDdata;
static float bright = 1.0;
#elif (COLOR_DEBTH == 3)
typedef struct LEDdata {
  byte r, g, b;
};
static float bright = 1.0;
#endif

// ===== COLOR UTILITY =====
LEDdata mRGB(byte r, byte g, byte b);	// RGB 255, 255, 255
LEDdata mWHEEL(int color);				// цвета 0-1530
LEDdata mHEX(uint32_t color);			// HEX цвет
LEDdata mHSV(byte h, byte s, byte v);	// HSV 255, 255, 255

// ======== CLASS ========
class microLED {
  public:
    microLED(LEDdata *LEDarray, int LEDamount, byte pin);		// имя буфера, количество ледов, пин
    void setRGB(int num, byte r, byte g, byte b);	// RGB
    void setHSV(int num, byte h, byte s, byte v); 	// HSV
    void setHEX(int num, uint32_t color);			// HEX
    void colorWheel(int num, int color);			// цвет 0-1530
    void setBrightness(byte newBright);				// яркость 0-255
    void fill(LEDdata color);						// заливка цветом (mRGB, mWHEEL, mHEX, mHSV)
    void clear();									// очистка
    void show();									// отправка

  private:
    int _numLEDs;
    LEDdata *LEDbuffer;
    byte _pin, _port;
    volatile uint8_t _mask;
};

// ================== microLED METHODS ==================
microLED::microLED(LEDdata *LEDarray, int LEDamount, byte pin) {
  _numLEDs = LEDamount;
  LEDbuffer = LEDarray;
  _pin = pin;
  pinMode(_pin, 1);

  _mask = digitalPinToBitMask(_pin);
  if (_pin < 8) _port = 0;
  else if (_pin < 14) _port = 1;
  else if (_pin < 20) _port = 2;
}

void microLED::setRGB(int num, byte r, byte g, byte b) {
  LEDbuffer[num] = mRGB(r, g, b);
}

void microLED::setHSV(int num, byte h, byte s, byte v) {
  LEDbuffer[num] = mHSV(h, s, v);
}

void microLED::setHEX(int num, uint32_t color) {
  LEDbuffer[num] = mHEX(color);
}

void microLED::colorWheel(int num, int color) {
  LEDbuffer[num] = mWHEEL(color);
}

void microLED::setBrightness(byte newBright) {
#if (COLOR_DEBTH == 1)
  bright = (255 - newBright) >> 5;
#elif (COLOR_DEBTH == 2)
  bright = (float)newBright / 255;
#elif (COLOR_DEBTH == 3)
  bright = (float)newBright / 255;
#endif
}

void microLED::clear() {
  for (int i = 0; i < _numLEDs; i++) {
#if (COLOR_DEBTH == 1)
    LEDbuffer[i] = 0;
#elif (COLOR_DEBTH == 2)
    LEDbuffer[i] = 0;
#elif (COLOR_DEBTH == 3)
    LEDbuffer[i].r = 0;
    LEDbuffer[i].g = 0;
    LEDbuffer[i].b = 0;
#endif
  }
}

void microLED::fill(LEDdata color) {
  for (int i = 0; i < _numLEDs; i++) {
    LEDbuffer[i] = color;
  }
}

// ============================== ВЫВОД =================================
// старый набор нопов
#define sendToPort(x);        \
  if ((thisLED[i] & (1 << b))) {  \
    (x) |= _mask;       \
    asm(            \
                    "nop \n\t"          \
                    "nop \n\t"          \
                    "nop \n\t"          \
                    "nop \n\t"          \
                    "nop \n\t"          \
                    "nop \n\t"          \
                    "nop \n\t"          \
                    "nop \n\t"          \
       );             \
    (x) &= ~_mask;        \
  } else {            \
    (x) |= _mask;       \
    asm ("nop");        \
    (x) &= ~_mask;        \
    asm(            \
                    "nop \n\t"          \
                    "nop \n\t"          \
                    "nop \n\t"          \
                    "nop \n\t"          \
                    "nop \n\t"          \
                    "nop \n\t"          \
       );             \
  }

void microLED::show() {
  //*ws2812_port_reg |= pinMask; // Enable DDR
  //ws2812_sendarray_mask((uint8_t *)LEDbuffer, COLOR_DEBTH * _numLEDs, pinMask, (uint8_t*) ws2812_port, (uint8_t*) ws2812_port_reg);

  byte thisLED[3];
  for (uint16_t c = 0; c < _numLEDs; c++) {
    // тут задаётся GRB (порядок 1 0 2)
#if (COLOR_DEBTH == 1)
    // 8 бит
    thisLED[ORDER0] = (LEDbuffer[c] & 0b11100000) >> bright;
    thisLED[ORDER1] = ((LEDbuffer[c] & 0b00011000) << 3) >> bright;
    thisLED[ORDER2] = ((LEDbuffer[c] & 0b00000111) << 5) >> bright;
#elif (COLOR_DEBTH == 2)
    // 16 бит
    thisLED[ORDER0] = (LEDbuffer[c] & 0b1111100000000000) >> 8;
    thisLED[ORDER1] = (LEDbuffer[c] & 0b0000011111100000) >> 3;
    thisLED[ORDER2] = (LEDbuffer[c] & 0b0000000000011111) << 3;
#elif (COLOR_DEBTH == 3)
    // 32 бит
    thisLED[ORDER0] = LEDbuffer[c].r;
    thisLED[ORDER1] = LEDbuffer[c].g;
    thisLED[ORDER2] = LEDbuffer[c].b;
#endif

    for (uint8_t i = 0; i < 3; i++) {
      for (uint8_t b = 8; b; b--) {
        cli();
#if defined(__AVR_ATmega328P__)
        switch (_port) {
          case 0:
            sendToPort(PORTD);
            break;
          case 1:
            sendToPort(PORTB);
            break;
          case 2:
            sendToPort(PORTC);
            break;
        }
#else
        sendToPort(PORTB);	// для тини
#endif
        sei();
      }
    }
  }
}


// ================== COLOR UTILITY ==================
LEDdata mRGB(byte r, byte g, byte b) {
#if (COLOR_DEBTH == 1)
  return ((r & 0b11100000) >> bright) | (((g & 0b11000000) >> 3) >> bright) | (((b & 0b11100000) >> 5) >> bright);

#elif (COLOR_DEBTH == 2)
  r = (float) r * bright;
  g = (float) g * bright;
  b = (float) b * bright;
  return ((r & 0b11111000) << 8) | ((g & 0b11111100) << 3) | ((b & 0b11111000) >> 3);
#elif (COLOR_DEBTH == 3)

  LEDdata bufferData;
  bufferData.r = r * bright;
  bufferData.g = g * bright;
  bufferData.b = b * bright;
  return bufferData;
#endif
}

LEDdata mHSV(byte h, byte s, byte v) {
  float r, g, b;

  float H = (float)h / 255;
  float S = (float)s / 255;
  float V = (float)v / 255;

  int i = int(H * 6);
  float f = H * 6 - i;
  float p = V * (1 - S);
  float q = V * (1 - f * S);
  float t = V * (1 - (1 - f) * S);

  switch (i % 6) {
    case 0: r = V, g = t, b = p; break;
    case 1: r = q, g = V, b = p; break;
    case 2: r = p, g = V, b = t; break;
    case 3: r = p, g = q, b = V; break;
    case 4: r = t, g = p, b = V; break;
    case 5: r = V, g = p, b = q; break;
  }
  return mRGB(r * 255, g * 255, b * 255);
}

LEDdata mHEX(uint32_t color) {
  return mRGB( (color >> 16) & 0xFF, (color >> 8) & 0xFF, color & 0xFF );
}

LEDdata mWHEEL(int color) {
  byte _r, _g, _b;
  if (color <= 255) {           // красный макс, зелёный растёт
    _r = 255;
    _g = color;
    _b = 0;
  }
  else if (color > 255 && color <= 510) {   // зелёный макс, падает красный
    _r = 510 - color;
    _g = 255;
    _b = 0;
  }
  else if (color > 510 && color <= 765) {   // зелёный макс, растёт синий
    _r = 0;
    _g = 255;
    _b = color - 510;
  }
  else if (color > 765 && color <= 1020) {  // синий макс, падает зелёный
    _r = 0;
    _g = 1020 - color;
    _b = 255;
  }
  else if (color > 1020 && color <= 1275) {   // синий макс, растёт красный
    _r = color - 1020;
    _g = 0;
    _b = 255;
  }
  else if (color > 1275 && color <= 1530) { // красный макс, падает синий
    _r = 255;
    _g = 0;
    _b = 1530 - color;
  }
  return mRGB(_r, _g, _b);
}
