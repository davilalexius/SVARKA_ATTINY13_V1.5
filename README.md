ПРОГРАММА: 		Svarka Attiny13
ВЕРСИЯ: 		1.5
ПРОИЗВОДИТЕЛЬ: 	Davilalexius
Сайт:			http://2137.ru
ДАТА:			14/08/2020
E-MAIL:			davilalexius@mail.ru

МИКРОКОНТРОЛЛЕР: Attiny13 
ПРИМЕЧАНИЕ МК: 	Можно использовать другие микроконтроллеры, если их программная память(FLASH) не менее 700 байт,
				динамическая память(RAM) не менее 50 байт, и используются ножки указанного порта.
ПОРТЫ: 			(PORTB)

ИНСТРУКЦИЯ
//***********************************************************************************************************//
РАБОЧИЙ РЕЖИМ-СВАРКА:
АКТИВАЦИЯ: Нажатие кнопки, подключенной из пина BUTTON на пин GROUND(к минусу питания)
**на пин START_PIN подаются сварочные импульсы(сюда подключаем реле управления сварочным током)

!!! НЕ ЗАБЫВАЙТЕ, ATTINY13 РАБОТАЕТ ОТ 1,8 ДО 5,5в НАПРЯЖЕНИЯ. ТАКОГО ЖЕ НОМИНАЛА СИГНАЛ БУДЕТ И НА ВЫХОДЕ ПИНА!!!
	!!УЧИТЫВАЙТЕ ЭТО ПРИ ВЫБОРЕ УПРАВЛЯЮЩЕГО РЕЛЕ!!
ДЕАКТИВАЦИЯ: Автоматическая

РЕЖИМ НАСТРОЙКИ ВРЕМЕНИ РЕЖИМОВ СВАРКИ:
АКТИВАЦИЯ: Нажатие кнопки, подключенной из пина OPTION_BUTTON на пин GROUND(к минусу питания)
**На пине INDICATOR появится 5 коротких сигналов
//***********************************************************************************************************//
МЕНЮ НАСТРОЕК:
0.ХОЛОСТОЙ ХОД <=
1.ПЕРВЫЙ ИМПУЛЬС(РАЗОГРЕВАЮЩИЙ)
2.ПАУЗА МЕЖДУ ИМПУЛЬСАМИ
3.ВТОРОЙ ИМПУЛЬС(СВАРКА)

	BUTTON - смена пункта меню в любом режиме
	**На пине INDICATOR появятся короткие сигналы в количестве(номер меню или 4 раза в меню=0)

=>ХОЛОСТОЙ ХОД (По умолчанию-стартовая страница)
	OPTION_BUTTON - выход из режима настроек + сохранение ваших настроек в память EEPROM
**На пине INDICATOR появится 5 коротких сигналов

=>ПЕРВЫЙ ИМПУЛЬС,ПАУЗА МЕЖДУ ИМПУЛЬСАМИ,ВТОРОЙ ИМПУЛЬС(СВАРКА)
	OPTION_BUTTON(каждое нажатие) - смена времени из массива TIME[8] -8 вариантов {10,20,50,100,200,250,500,1000};
**На пине INDICATOR появятся короткие сигналы в количестве(номер из массива 1-8) //не путать с программной нумирацией(0-7)
//***********************************************************************************************************//
Примечание:
Кнопки подтянуты к питанию+программно защищены паузами, поэтому не бойтесь тримминга.
							 ______
					PIN5	-|	  |-	VCC(1.8-5.5V)
  ВЫХОД INDICATOR<- PIN3	-|    |-	PIN2 ->ВЫХОД К РЕЛЕ START_PIN
					PIN4	-|    |-	PIN1 <-КНОПКА OPTION_BUTTON
					GND(-) 	-|    |-	PIN0 <-КНОПКА BUTTON
							 ------

На выход INDICATOR(положительный сигнал) можете повесить светодиод или зуммер для ориентирования в меню опций.

СПАСИБО ПО ЖЕЛАНИЮ:
Яндекс кошелек: 410012738934950
PayPal:davilalexius@yahoo.com
Beeline:+7 903 763 19 92