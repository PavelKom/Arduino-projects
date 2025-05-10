/*
  Автоматическая работа внешнего насоса для ГВС с рециркуляцией.
  Оборудование:
    Ардуино УНО/НАНО
    Релейный модуль       - Включение насоса.
    Расходомер YF-S201    - В качестве датчика потока.
    Часы DS1302           - Если время не настроено, то прога его запишет (пишется время компиляции).
    Дисплей LCD1602A-I2C  - Если буквы неправильные, то юзаем rusLCD_Custom.h для кириллицы.
                            Если нужны другие буквы, то можно использовать char_draw_ui.py (требуется Python) для рисовки своих символов.
                            Примечание: LCDI2C_Multilingual хранит в своей памяти до 8 кастомных символов, если надо больше, то ищите CGRAM_SIZE и cgramLetters[CGRAM_SIZE]
*/
#include <Arduino.h>

// Настройки
//#define DEBUG // Включение Serial для связи с ПК
// Реле
#define RELAY_PIN 13          // Пин для реле
#define RELAY_NO true         // Нормально открыто?
// Расходомер
#define FLOW_WORK 100         // Литр/час (int)
#define FLOW_PIN 3            // Пин для расходомера (ШИМ)
#define FLOW_TYPE YFS201      // Тип расходомера
// Часы. DS1307 работают через A4() и A5() 
#define CLOCK_TYPE 1302       // 1302 1307
#define PIN_1302_DAT_IO 4     // DAT/IO пин
#define PIN_1302_CLK_SCLK 5   // CLK/SCLK пин
#define PIN_1302_RST_CE 2     // RST/CE пин
// Дисплей
#define LCD_FIX_CYRILLIC true // ИСТИНА если дисплей неправильно работает с кириллицей
#define LCD_ADDRESS 0x27      // Адрес дисплея
#define LCD_COLUMNS 16        // Количество столбцов
#define LCD_ROWS 2            // Количество строк
// Сообщения статуса, строки одинаковой длины
const static char* Status[] ={"ПРОГРЕВ ", "РАБОТА  ", "ПРОГРЕВ?", "РАБОТА? ", "ОЖИДАНИЕ", "ПРОВЕРКА"};
// Время. RtcDateTime(Год,Месяц,День,Час,Минута,Секунда), день должен быть больше нуля
#define TIME_IDLE RtcDateTime(0,0,1,1,0,0) // Время ожидания между прогревом (1 час)
#define TIME_WARM RtcDateTime(0,0,1,0,1,0) // Время прогрева (1 минута)
#define TIME_JOB  RtcDateTime(0,0,1,0,5,0) // Время работы (5 минут)
#define TIME_TEST RtcDateTime(0,0,1,0,0,5) // Время для набора потока (5 секунд)

// Библиотеки
#include <Relay.h>
#include <FlowSensor.h>
#if CLOCK_TYPE == 1302
  #include <RtcDS1302.h>
#elif CLOCK_TYPE == 1307
  #include <Wire.h>
  #include <RtcDS1307.h>
#endif
#if LCD_FIX_CYRILLIC
  #include "rusLCD_Custom.h"
#else
  #include <LCDI2C_Multilingual.h>
#endif

// Анстройка времени. 
int32_t idle_timer = TIME_IDLE.TotalSeconds();
int32_t warm_timer = TIME_WARM.TotalSeconds();
int32_t job_timer =  TIME_JOB.TotalSeconds();
int32_t test_timer = TIME_TEST.TotalSeconds();

// Классы
Relay pump(RELAY_PIN, RELAY_NO);
FlowSensor flow(FLOW_TYPE, FLOW_PIN);
#if CLOCK_TYPE == 1302
  ThreeWire myWire(PIN_1302_DAT_IO, PIN_1302_CLK_SCLK, PIN_1302_RST_CE);
  RtcDS1302<ThreeWire> rtc(myWire);
#elif CLOCK_TYPE == 1307
  RtcDS1307<TwoWire> rtc(Wire);
#endif
#if LCD_FIX_CYRILLIC
  LCDI2C_CustomRus lcd(LCD_ADDRESS, LCD_COLUMNS, LCD_ROWS);
#else
  LCDI2C_Russian lcd(LCD_ADDRESS, LCD_COLUMNS, LCD_ROWS);
#endif

RtcDateTime now;
RtcDateTime nextChange;
uint8_t status;
int flowH;

void set_warm(){
  status = 0;
  nextChange = now + warm_timer;
  lcd.clear();
}
void set_job(){
  status = 1;
  nextChange = now + job_timer;
  lcd.clear();
}
void set_warm_pre(){
  status = 2;
  pump.turnOn();
  lcd.backlight();
  nextChange = now + test_timer;
  lcd.clear();
}
void set_job_pre(){
  status = 3;
  pump.turnOn();
  lcd.backlight();
  nextChange = now + test_timer;
  lcd.clear();
}
void set_idle(){
  status = 4;
  pump.turnOff();
  lcd.noBacklight();
  nextChange = now + idle_timer;
  lcd.clear();
}
void set_test(){
  status = 5;
  pump.turnOff();
  nextChange = now + test_timer;
  lcd.clear();
}
void test_flow(){
  if (flowH >= FLOW_WORK)
    set_job_pre(); // Включаем насос на несколько секунд, проверяем поток
  else
    set_idle(); // Нет воды / закрыли кран
}
void test_time(){
  if (now < nextChange)
  {
    if (status == 4 && flowH >= FLOW_WORK) // Открыли кран
      set_job_pre();
    else if(status < 2 && flowH < FLOW_WORK) // Нет воды
      test_flow();
    return;
  }
  switch (status)
  {
    case 0: // Конец прогрева,
    case 1: // Конец работы. Пауза перед тестом.
      set_test();
      break;
    case 2: // Прогреваем
      set_warm();
      break;
    case 3:
      set_job();
      break;
    case 4: // Конец ожидания. Включаем тест на прогрев
      set_warm_pre();
      break;
    case 5: // Конец ожидания перед тестом. 
      test_flow();
      break;
    //default:
    //  break;
  }
}

void count()
{
	flow.count();
}

void setup() {
#ifdef DEBUG
  Serial.begin(57600);
#endif
  lcd.init();
  lcd.backlight();
  flow.begin(count);
  RtcDateTime compiled = RtcDateTime(__DATE__, __TIME__);
  now = rtc.GetDateTime();
  if (compiled > now)
    rtc.SetDateTime(compiled);
  set_warm_pre();
}


char buffer[32];
void loop() {
  flow.read();
  lcd.home();
  now = rtc.GetDateTime();
  flowH = int(flow.getFlowRate_h());
  test_time();

  //HH:MM:SS---MM:SS
  //STATUS++-----Flo
  RtcDateTime wait = nextChange - now.TotalSeconds();
#ifdef DEBUG
  Serial.print(status);
  Serial.print("  ");
  Serial.print(now.TotalSeconds());
  Serial.print("  ");
  Serial.print(nextChange.TotalSeconds());
  Serial.print("  ");
  Serial.println(wait.TotalSeconds());
#endif
  sprintf(buffer, "%.2i:%.2i:%.2i   %.2i:%.2i",
    now.Hour(), now.Minute(), now.Second(),
    wait.Minute(), wait.Second()); // Get time
  lcd.println(buffer);
  sprintf(buffer, "%s     %.3i", Status[status], flowH);
  lcd.println(buffer);
  delay(1000);
}
