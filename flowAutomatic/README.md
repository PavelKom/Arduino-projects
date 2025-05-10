# flowAutomatic 
Скрипт для автоматизации работы ГВС с внешним рециркуляционным насосом. 
## Компоненты (в скобках те, которые использовал я)
- Arduino (*Nano*)
- Макетная плата (*17x10*)
- Релейный модуль (*CW-019* с реле *SRD-05VDC-SL-C* на **10A 252VAC**) - включение насоса.
- Расходомер (*YF-S201*) - в качестве датчика потока.
- Часы реального времени (*DS1302*)
- Дисплей (*LCD1602A-I2C*)
## Библиотеки
- **Relay.h** (https://github.com/rafaelnsantos/Relay) - реле.
- **FlowSensor.h** (https://github.com/hafidhh/FlowSensor-Arduino) - расходомер.
- **RtcDS1302.h** (https://github.com/Makuna/Rtc) - часы.
- **LCDI2C_Multilingual.h** (https://github.com/locple/LCDI2C_Multilingual) - дисплей. Если у вас не работают русские буквы (за место них японские или ещё что), то используйте **rusLCD_Custom.h** (идёт в комплекте). Также есть скрипт на Python для рисовки кастомных символов.




