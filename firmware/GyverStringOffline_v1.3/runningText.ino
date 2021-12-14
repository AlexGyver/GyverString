// работа с бегущим текстом
// --------------------- ДЛЯ РАЗРАБОТЧИКОВ ----------------------

void fillString(ctext &text) {
  if (loadingFlag) {
    loadingFlag = false;
    fullTextFlag = false;
  }
  
  if (millis() - scrollTimer >= D_TEXT_SPEED) {
    strip.clear();
    int i = 0, j = 0;
    while (text.text[i] != '\0') {
      if ((byte)text.text[i] > 191) {    // работаем с русскими буквами!
        i++;
      } else {
        drawLetter(j, text.text[i], xoffset + j * (LET_WIDTH + SPACE), yoffset, setColor(text, j));
        i++;
        j++;
      }
    }
    if (!fullTextFlag) {
      xoffset=xoffset+cxoffset;
      yoffset=yoffset+cyoffset;
      if ((xoffset == stxpos) && (yoffset == stypos)) {    // строка убежала
        fullTextFlag = true;
      }
    }
    sticking = false;
    scrollTimer = millis();
    if ((xoffset == htxpos) && (yoffset == htypos)) {
      scrollTimer = millis() + D_TEXT_SPEED - text.sticktime;
      sticktimer = millis();
      sticking = true;
    }
    strip.show();
  }
}

int stringLength(String &thisString) {
  int i = 0, j = 0;
  while (thisString[i] != '\0') {
    if ((byte)thisString[i] > 191) {    // работаем с русскими буквами!
      i++;
    } else {
      i++;
      j++;
    }
  }
  return j;
}

uint32_t setColor(ctext &text, int index) {
  uint32_t col = text.color;
  if (text.rainbow > 0) {
    int cr,cg,cb;
    cr = (col >> 16) & 0xFF;
    cg = (col >> 8) & 0xFF;
    cb = col & 0xFF;
    int colr = index%5;
    uint8_t r,g,b;
    r=g=b=0;
    switch (colr) {
      case 0:
        r = 255;
        break;
      case 1:
        r = 255;
        g = 255;
        break;
      case 2:
        g = 255;
        break;
      case 3:
        g = 255;
        b = 255;
        break;
      case 4:
        b = 255;
        r = 255;
        break;
    }
    float fading = (uint8_t)(
      (float)text.sticktime /
      (float)(max(millis()-sticktimer,text.sticktime)));
    float alpha;
    switch (text.rainbowfade) {
      case RB_CONSTANT:
        alpha = (float)text.rainbow / 255;
        break;
      case RB_FADE_IN:
        alpha = fading * (float)(text.rainbow/255);
        break;
      case RB_FADE_OUT:
        alpha = (fading-1) * (float)(text.rainbow/255);
        break;
    }
    col = (
          ((uint32_t)((float)cr*(1-alpha)+(float)r*alpha) << 16) |
          ((uint32_t)((float)cg*(1-alpha)+(float)g*alpha) << 8 ) |
          (uint32_t)((float)cb*(1-alpha)+(float)b*alpha));
  }
  return col;
}

void drawLetter(int index, uint8_t letter, int offset, int yoffset, uint32_t letterColor) {
  int start_pos = 0, finish_pos = LET_WIDTH;
  int start_ypos = 0, finish_ypos = LET_HEIGHT;

  if (offset < -LET_WIDTH || offset > WIDTH) return;
  if (offset < 0) start_pos = -offset;
  if (offset > (WIDTH - LET_WIDTH)) finish_pos = WIDTH - offset;
  
  if (yoffset < -LET_HEIGHT || yoffset > HEIGHT) return;
  if (yoffset < 0) start_ypos = -yoffset;
  if (yoffset > (HEIGHT - LET_HEIGHT)) finish_ypos = HEIGHT - offset;

  for (int i = start_pos; i < finish_pos; i++) {
    int thisByte;
    if (MIRR_V) thisByte = getFont((byte)letter, LET_WIDTH - 1 - i);
    else thisByte = getFont((byte)letter, i);

    for (byte j = start_ypos; j < finish_ypos; j++) {
      boolean thisBit;

      if (MIRR_H) thisBit = thisByte & (1 << j);
      else thisBit = thisByte & (1 << (LET_HEIGHT - 1 - j));

      // рисуем столбец (i - горизонтальная позиция, j - вертикальная)
      if (TEXT_DIRECTION) {
        if (thisBit) leds[getPixelNumber(offset + i, yoffset + j)] = mHEX(letterColor);
        else drawPixelXY(offset + i, yoffset + j, 0x000000);
      } else {
        if (thisBit) leds[getPixelNumber(i, offset + yoffset + j)] = mHEX(letterColor);
        else drawPixelXY(i, offset + yoffset + j, 0x000000);
      }

    }
  }
}

// ------------- СЛУЖЕБНЫЕ ФУНКЦИИ --------------

// интерпретатор кода символа в массиве fontHEX (для Arduino IDE 1.8.* и выше)
uint8_t getFont(uint8_t font, uint8_t row) {
  font = font - '0' + 16;   // перевод код символа из таблицы ASCII в номер согласно нумерации массива
  if (font <= 90) return pgm_read_byte(&(fontHEX[font][row]));     // для английских букв и символов
  else if (font >= 112 && font <= 159) {    // и пизд*ц ждя русских
    return pgm_read_byte(&(fontHEX[font - 17][row]));
  } else if (font >= 96 && font <= 111) {
    return pgm_read_byte(&(fontHEX[font + 47][row]));
  }
}

/*
  // интерпретатор кода символа по ASCII в его номер в массиве fontHEX (для Arduino IDE до 1.6.*)
  uint8_t getFontOld(uint8_t font, uint8_t row) {
  font = font - '0' + 16;   // перевод код символа из таблицы ASCII в номер согласно нумерации массива
  if (font < 126) return pgm_read_byte(&(fontHEX[font][row]));   // для английских букв и символов
  else return pgm_read_byte(&(fontHEX[font - 65][row]));         // для русских букв и символов (смещение -65 по массиву)
  }
*/
