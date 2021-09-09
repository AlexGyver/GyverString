![PROJECT_PHOTO](https://github.com/AlexGyver/gyverString/blob/master/proj_img.jpg)
# Бегущая строка с голосовым управлением
* [Описание проекта](#chapter-0)
* [Папки проекта](#chapter-1)
* [Схемы подключения](#chapter-2)
* [Материалы и компоненты](#chapter-3)
* [Как скачать и прошить](#chapter-4)
* [FAQ](#chapter-5)
* [Полезная информация](#chapter-6)
[![AlexGyver YouTube](http://alexgyver.ru/git_banner.jpg)](https://www.youtube.com/channel/UCgtAOyEQdAyjvm9ATCi_Aig?sub_confirmation=1)

<a id="chapter-0"></a>
## Описание проекта
- Система бегущей строки на матрице MAX7219 или адресных светодиодах
- Голосовое управление с Android
- Страница проекта на сайте: https://alexgyver.ru/gyverString/

<a id="chapter-1"></a>
## Папки
**ВНИМАНИЕ! Если это твой первый опыт работы с Arduino, читай [инструкцию](#chapter-4)**
- **libraries** - библиотеки проекта. Заменить имеющиеся версии
- **firmware** - прошивки для Arduino
	- GyverMatrixOS_v1.12 - портированный проект GyverMatrixBT
	- GyverString - версия бегущей строки с управлением по BT и адресной матрицей
	- GyverStringOffline - версия бегущей строки с адресной матрицей, без BT
	- GyverStringDotmatrix - версия бегущей строки с управлением по BT и матрицей MAX7219
- **schemes** - схемы подключения компонентов
- **3D print** - файлы для печати
- **Android** - исходник приложения

<a id="chapter-2"></a>
## Схемы
![SCHEME](https://github.com/AlexGyver/gyverString/blob/master/schemes/scheme0.jpg)
![SCHEME](https://github.com/AlexGyver/gyverString/blob/master/schemes/scheme1.jpg)
![SCHEME](https://github.com/AlexGyver/gyverString/blob/master/schemes/scheme2.jpg)

<a id="chapter-3"></a>
## Материалы и компоненты
### Ссылки оставлены на магазины, с которых я закупаюсь уже не один год
- Arduino NANO https://ali.ski/SQihi  https://ali.ski/kbhKuC
- Bluetooth HC-06 https://ali.ski/2ZSOok  https://ali.ski/FcMyMf
- Матрица MAX7219 https://ali.ski/kxwy2t  https://ali.ski/1SFtH
- Матрица 32x8 https://ali.ski/D8cerQ  https://ali.ski/iLxVM  https://ali.ski/ZTlyDf
- Лента адресная https://ali.ski/Fm17II  https://ali.ski/rnHw1E  https://ali.ski/E2T6HS
- БП 5V https://ali.ski/r0VKdK  https://ali.ski/c2-8D

## Вам скорее всего пригодится
* [Всё для пайки (паяльники и примочки)](http://alexgyver.ru/all-for-soldering/)
* [Недорогие инструменты](http://alexgyver.ru/my_instruments/)
* [Все существующие модули и сенсоры Arduino](http://alexgyver.ru/arduino_shop/)
* [Электронные компоненты](http://alexgyver.ru/electronics/)
* [Аккумуляторы и зарядные модули](http://alexgyver.ru/18650/)

<a id="chapter-4"></a>
## Как скачать и прошить
* [Первые шаги с Arduino](http://alexgyver.ru/arduino-first/) - ультра подробная статья по началу работы с Ардуино, ознакомиться первым делом!
* Скачать архив с проектом
> На главной странице проекта (где ты читаешь этот текст) вверху справа зелёная кнопка **Clone or download**, вот её жми, там будет **Download ZIP**
* **Подключить внешнее питание 5 Вольт**
* Подключить плату к компьютеру
* Файл/Настройки, вставить ссылку http://drazzy.com/package_drazzy.com_index.json в текст-бокс для дополнительных ссылок для менеджера плат
* Открыть Инструменты/Плата/Менеджер плат…, найти ATTinyCore by Spence Konde, установить актуальную версию
* Открыть Инструменты/Плата/ATtiny25/45/85
* Инструменты/Программатор/USBASP (или чем вы будете шить)
* Инструменты/Записать загрузчик
* Загрузить прошивку (Скетч/Загрузить через программатор)


## Настройки в коде

	
<a id="chapter-5"></a>
## FAQ
### Основные вопросы
В: Как скачать с этого грёбаного сайта?  
О: На главной странице проекта (где ты читаешь этот текст) вверху справа зелёная кнопка **Clone or download**, вот её жми, там будет **Download ZIP**

В: Скачался какой то файл .zip, куда его теперь?  
О: Это архив. Можно открыть стандартными средствами Windows, но думаю у всех на компьютере установлен WinRAR, архив нужно правой кнопкой и извлечь.

В: Я совсем новичок! Что мне делать с Ардуиной, где взять все программы?  
О: Читай и смотри видос http://alexgyver.ru/arduino-first/

В: Вылетает ошибка загрузки / компиляции!
О: Читай тут: https://alexgyver.ru/arduino-first/#step-5

В: Сколько стоит?  
О: Ничего не продаю.

### Вопросы по этому проекту

<a id="chapter-6"></a>
## Полезная информация
* [Мой сайт](http://alexgyver.ru/)
* [Основной YouTube канал](https://www.youtube.com/channel/UCgtAOyEQdAyjvm9ATCi_Aig?sub_confirmation=1)
* [YouTube канал про Arduino](https://www.youtube.com/channel/UC4axiS76D784-ofoTdo5zOA?sub_confirmation=1)
* [Мои видеоуроки по пайке](https://www.youtube.com/playlist?list=PLOT_HeyBraBuMIwfSYu7kCKXxQGsUKcqR)
* [Мои видеоуроки по Arduino](http://alexgyver.ru/arduino_lessons/)