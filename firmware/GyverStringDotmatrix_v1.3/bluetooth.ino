#define PARSE_AMOUNT 8    // максимальное количество значений в массиве, который хотим получить
#define header '$'        // стартовый символ
#define divider ','       // разделительный символ
#define ending ';'        // завершающий символ
int intData[PARSE_AMOUNT];     // массив численных значений после парсинга
boolean recievedFlag;
String request = "1234567891011121314151617181920";
boolean getStarted;
byte index;
String string_convert = "";
boolean parseString, getString;

void bluetoothTick() {
  parsing();              // функция парсинга
  if (recievedFlag) {     // если получены данные
    recievedFlag = false;
    if (getString) {
      getString = false;
      loadingFlag = true;
      thisLength = runningText.length();
      showText = true;
      updateSettings();
      return;
    }

    switch (intData[0]) {
      case 0:   // запрос онлайна
        request = "OK ";
        request += String(random(0, 100));
        btSerial.print(request);
        break;
      case 1: // вкл/выкл
        toggleText(intData[1]);
        break;
      case 2: // скорость
        thisSpeed = 110 - intData[1];
        scrollTimer.setInterval(thisSpeed);
        break;
      case 3: // яркость
        thisBright = intData[1];
        matrix.setIntensity(thisBright);
        break;
      case 4: // режим
        thisMode = intData[1];
        break;
      case 5: // цвет
        thisColor = intData[1];
        break;
      case 6: // авто
        autoFlag = intData[1];
        if (autoFlag) showText = true;
        break;
    }
    if (intData[0] > 0) updateSettings();
  }
}

void parsing() {
  if (btSerial.available() > 0) {
    char incomingByte;
    if (parseString) {
      runningText = "";
      runningText = btSerial.readString();  // принимаем всю
      incomingByte = ending;              // сразу завершаем парс
    } else {
      incomingByte = btSerial.read();      // обязательно ЧИТАЕМ входящий символ
    }
    if (getStarted) {                         // если приняли начальный символ (парсинг разрешён)
      if (incomingByte != divider && incomingByte != ending) {  // если это не пробел И не конец
        string_convert += incomingByte;       // складываем в строку
      } else {                                // если это пробел или ; конец пакета
        intData[index] = string_convert.toInt();  // преобразуем строку в int и кладём в массив
        string_convert = "";                  // очищаем строку
        index++;                              // переходим к парсингу следующего элемента массива
      }
    }
    if (incomingByte == header) {             // если это $
      getStarted = true;                      // поднимаем флаг, что можно парсить
      index = 0;                              // сбрасываем индекс
      string_convert = "";                    // очищаем строку
      parseString = false;
    }
    if (incomingByte == '#') {                // если это #
      getStarted = true;                      // поднимаем флаг, что можно парсить
      index = 0;                              // сбрасываем индекс
      string_convert = "";                    // очищаем строку
      parseString = true;
      getString = true;
    }
    if (incomingByte == ending) {             // если таки приняли ; - конец парсинга
      getStarted = false;                     // сброс
      recievedFlag = true;                    // флаг на принятие
      parseString = false;
    }
  }
}
