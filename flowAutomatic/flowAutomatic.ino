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
// Библиотеки
#include <Arduino.h>
// Реле
#include <Relay.h>
// Расходомер
#include <FlowSensor.h>
// Часы
#include <RtcDS1302.h>
// Дисплей. Если у вас за место некоторых 
// букв отображаются буквы других языков, к примеру, японские, то используйте кастомный рус-яз
//#include <LCDI2C_Multilingual.h>
#include "rusLCD_Custom.h"

// Реле
// 3 пина, 1 управляющий. 5V, SIGNAL (OUTPUT), GND
Relay pump(13, true);
// Расходомер
// 3 пина, 1 управляющий. 5V, SIGNAL (INPUT с ШИМ), GND
#define FLOW_WORK 100 // Литр/час
FlowSensor flow(YFS201, 3);
// Часы DS1302
// 5 пинов, 3 управляющих. 5V, GND, CLK (PWN), DAT, RST
ThreeWire myWire(4,5,2); // DAT/IO, CLK/SCLK, RST/CE
RtcDS1302<ThreeWire> rtc(myWire);
int32_t idle_timer = RtcDateTime(0,0,1,1,0,0).TotalSeconds(); //1 hour
int32_t warm_timer = RtcDateTime(0,0,1,0,1,0).TotalSeconds(); //1 min
int32_t job_timer = RtcDateTime(0,0,1,0,5,0).TotalSeconds(); //5 min
int32_t test_timer = RtcDateTime(0,0,1,0,0,5).TotalSeconds(); //5 sec
// Дисплей по I2C (A4,A5)
LCDI2C_CustomRus lcd(0x27, 16, 2);
const static char* Status[] ={"ПРОГРЕВ ", "РАБОТА  ", "ПРОГРЕВ?", "РАБОТА? ", "ОЖИДАНИЕ", "ПРОВЕРКА"};

RtcDateTime now;
RtcDateTime nextChange;
uint8_t status;
int flowH;

//#define DEBUG

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
