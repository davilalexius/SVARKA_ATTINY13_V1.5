//       SVARKA_ATTINY13 v1.4                                                         //
//       Проект точечной сварки. Разработан Davilalexius. http://2137.ru              //
//       Свободное распространение с указанием автора кода и ссылкой на сайт!!!       //
#include <avr/io.h>
#include <avr/eeprom.h>
#define E_KEY_ADDR ((uint8_t*)0x3F)
 uint8_t EEPROM_KEY  =35;
 uint8_t EEPROM_SAVE_KEY  =53;
#define EEPROM_PRE_WT_ADDR  ((uint8_t*)0x00)
#define EEPROM_PAUSE_WT_ADDR  ((uint8_t*)0x01)
#define EEPROM_WT_ADDR  ((uint8_t*)0x02)
 uint8_t EEPROM_DEF_PRE_WT  =1;
 uint8_t EEPROM_DEF_PAUSE_WT  =3;
 uint8_t EEPROM_DEF_WT  =3;
#include <util/delay.h>
#define BUTTON 0  //НОГА PB0 ПОДТЯНУТА PULLUP К ПИТАНИЮ(+), КОГДА БУДЕТ ЧЕРЕЗ КНОПКУ К GND -СКРИПТ СРАБОТАЕТ
#define OPTION_BUTTON 1 //НАСТРОЕЧНАЯ НОГА PB! ПОДТЯНУТА PULLUP К ПИТАНИЮ(+)
#define START_PIN 2 //НОГА PB2 НА НЕЙ 5V, В МОМЕНТ РАБОТЫ ПОЯВЛЯЕТСЯ 0 НА ЗАДАННОЕ ВРЕМЯ 
#define INDICATOR 3 //НОГА PB3 для индикации
#define INDICATOR_TIME 150 //ВРЕМЯ ДЛЯ ПАУЗ ИНДИКАТОРА
#define _PAUSE 1000 //пауза в 1 секунду
#define TIME_PRE_MS 3000 //пауза перед прожигом
//Настройки режима сварки по дефолту
byte PRE_WORK_TIME=1,TIME_PAUSE_MS=3, WORK_TIME=3,MODE=0;
//Настройки опционального режима
byte OPT_MODE=0;
const uint16_t TIME[8]={10,20,50,100,200,250,500,1000};//ВРЕМЯ для паузы В МС
uint8_t k1=7; //Поправка на "ветер". Осциллографом замеряем реальные тайминги на выходе
//И в функции SVARKA() добавляем или отнимаем коэффициенты. в моем случае 1 коэффициент
void TIME_DELAY(int a){
for(int i=0;i<a;i++)_delay_ms(1);
}
void JOB(){
 switch (MODE){
    case 0:break;//Холостой ход
    case 1:SVARKA();break;//режим сварки
    case 2:OPTIONS();break;//режим опционального меню
   }}
void CHANGE_PIN(int TIME_IN_MS,byte PIN_NAME,bool ON_HIGH){
  TIME_DELAY(TIME_IN_MS);
  switch(ON_HIGH){
    case 0:PORTB&=~(1<<PIN_NAME);break;
    case 1:PORTB|=(1<<PIN_NAME);break;
    }}
bool GET_PIN(byte PIN_NAME){return PINB&(1<<PIN_NAME);}
bool CHECK_BUTTON(byte LOW_PIN_NAME, byte HIGH_PIN_NAME){return (!(GET_PIN(LOW_PIN_NAME))&& GET_PIN(HIGH_PIN_NAME))?1:0;}
void SVARKA(){
  CHANGE_PIN(TIME_PRE_MS,START_PIN,1);
  CHANGE_PIN(TIME[PRE_WORK_TIME]+k1,START_PIN,0);
  CHANGE_PIN(TIME[TIME_PAUSE_MS]-k1,START_PIN,1);
  CHANGE_PIN(TIME[WORK_TIME]+k1,START_PIN,0);
  MODE=0;//выход из режима сварки
}
void OPTIONS(){
  if(CHECK_BUTTON(BUTTON, OPTION_BUTTON)){//смена менюшек
    OPT_MODE+=1;
      if(OPT_MODE>3)OPT_MODE=0;
    if (OPT_MODE==0){INDICATION(4);}
    else {INDICATION(OPT_MODE);}
      TIME_DELAY(_PAUSE);
   }  
  if(CHECK_BUTTON(OPTION_BUTTON,BUTTON)){//Не нажата BUTTON, нажата OPTION_BUTTON
         switch (OPT_MODE){ 
          case 0://Выход из меню настроек
            OPT_MODE=0;
            MODE=0;
      eeprom_update_byte(E_KEY_ADDR,EEPROM_SAVE_KEY); 
      UPDATE_EEPROM();
            INDICATION(5);
            break;
          case 1://Изменение времени препрожига
            PRE_WORK_TIME++;
            if(PRE_WORK_TIME>7){PRE_WORK_TIME=0;}
      INDICATION(PRE_WORK_TIME+1);
            break;
      case 2:
      TIME_PAUSE_MS++;
      if(TIME_PAUSE_MS>7){TIME_PAUSE_MS=0;}
      INDICATION(TIME_PAUSE_MS+1);
      break;
          case 3: //Изменение времени основного прожига
            WORK_TIME++;
            if(WORK_TIME>7){WORK_TIME=0;}
      INDICATION(WORK_TIME+1);
            break;
        }
        TIME_DELAY(_PAUSE);
  }
}
void INDICATION(int8_t b){//Звуковая или световая индикация
  for(int8_t i=0;i<b;i++ ){
    CHANGE_PIN(INDICATOR_TIME,INDICATOR,1);
    CHANGE_PIN(INDICATOR_TIME,INDICATOR,0);
    }
  }
 void UPDATE_EEPROM(){
  if(eeprom_read_byte(E_KEY_ADDR)==EEPROM_SAVE_KEY){//Ключ сохранения опций в память EEPROM
  eeprom_update_byte(EEPROM_PRE_WT_ADDR,PRE_WORK_TIME); 
    eeprom_update_byte(EEPROM_PAUSE_WT_ADDR,TIME_PAUSE_MS); 
    eeprom_update_byte(EEPROM_WT_ADDR,WORK_TIME); 
  eeprom_update_byte(E_KEY_ADDR,EEPROM_KEY); 
  }
  else if(eeprom_read_byte(E_KEY_ADDR)==EEPROM_KEY){//Ключ проверки установленных опций
  PRE_WORK_TIME=eeprom_read_byte(EEPROM_PRE_WT_ADDR);
    TIME_PAUSE_MS=eeprom_read_byte(EEPROM_PAUSE_WT_ADDR);
    WORK_TIME=eeprom_read_byte(EEPROM_WT_ADDR);
   }else{//Установка  DEFAULT значений в память EEPROM - программный сброс настроек
    eeprom_update_byte(E_KEY_ADDR,EEPROM_KEY); 
    eeprom_update_byte(EEPROM_PRE_WT_ADDR,EEPROM_DEF_PRE_WT); 
    eeprom_update_byte(EEPROM_PAUSE_WT_ADDR,EEPROM_DEF_PAUSE_WT); 
    eeprom_update_byte(EEPROM_WT_ADDR,EEPROM_DEF_WT); 
    }
 }
void INIT_0(){
DDRB|=(1<<START_PIN); //ПИН ВКЛЮЧЕНИЯ РЕЛЕ В ПОЗИЦИЮ ВЫХОДА
PORTB&=~(1<<START_PIN); //ПИН ВКЛЮЧЕНИЯ РЕЛЕ (ЛОГ 0) ДЛЯ РЕЛЕ С АКТИВАТОРОМ ПО ЛОГ 1
DDRB&=~(1<<BUTTON);  //КНОПКА ВКЛЮЧЕНИЯ В ПОЗИЦИЮ ВХОДА
PORTB|=(1<<BUTTON);  //ПОДТЯГИВАЕМ КНОПКУ ВКЛЮЧЕНИЯ К ПИТАНИЮ 5V(+)
DDRB&=~(1<<OPTION_BUTTON);  //КНОПКА ОПЦИЙ В ПОЗИЦИЮ ВХОДА
PORTB|=(1<<OPTION_BUTTON);  //ПОДТЯГИВАЕМ КНОПКУ НАСТРОЕК К ПИТАНИЮ 5V(+)
//Выход индикации
DDRB|=(1<<INDICATOR); //ПИН ИНДИКАЦИИ->ВЫХОД
PORTB&=~(1<<INDICATOR); //ИНДИКАТОР->ЛОГ 0
UPDATE_EEPROM();
  }
int main(void)
{
INIT_0();
  while(1){
    if(!(GET_PIN(BUTTON)) && MODE==0){MODE=1;}
    else if(!(GET_PIN(OPTION_BUTTON)) && MODE==0){//Если нажата кнопка опций в холостом режиме
    MODE=2;//Режим меню настроек  
    INDICATION(5);
  }   
  JOB();//проверка режима работы и действие от состояния регистра MODE
  }
  return 0;
}
