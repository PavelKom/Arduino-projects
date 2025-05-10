
#include <LCDI2C_Multilingual.h>

#ifndef rusLCD_Custom_h
#define rusLCD_Custom_h


const uint8_t SymbolCustomRusNum = 66;
CustomCharacterType SymbolCustomRus [SymbolCustomRusNum] = {
  {0x0410, {126, 136, 136, 136, 126}},  // А
  {0x0411, {254, 146, 146, 146, 204}},  // Б
  {0x0412, {254, 146, 146, 146, 108}},   // В
  {0x0413, {254, 128, 128, 128, 192}},   // Г
  {0x0414, {7, 138, 242, 130, 255}},   // Д
  {0x0415, {254, 146, 146, 146, 130}},   // Е
  {0x0401, {62, 170, 42, 170, 34}},   // Ё
  {0x0416, {238, 16, 254, 16, 238}},   // Ж
  {0x0417, {130, 146, 146, 146, 108}},   // З
  {0x0418, {254, 8, 16, 32, 254}},   // И
  {0x0419, {62, 132, 72, 144, 62}},   // Й
  {0x041A, {254, 16, 40, 68, 130}},   // К
  {0x041B, {4, 130, 252, 128, 254}},   // Л
  {0x041C, {254, 64, 48, 64, 254}},   // М
  {0x041D, {254,16,16,16,254}},   // Н
  {0x041E, {124,130,130,130,124}},   // О
  {0x041F, {254,128,128,128,254}},   // П
  {0x0420, {254,144,144,144,96}},   // Р
  {0x0421, {252,130,130,130,68}},   // С
  {0x0422, {128,128,254,128,128}},   // Т
  {0x0423, {226,20,8,16,224}},   // У
  {0x0424, {56,68,254,68,56}},   // Ф
  {0x0425, {198,40,16,40,198}},   // Х
  {0x0426, {254,2,2,2,255}},   // Ц
  {0x0427, {224,16,16,16,254}},   // Ч
  {0x0428, {254,2,254,2,254}},   // Ш
  {0x0429, {254,2,254,2,255}},   // Щ
  {0x042A, {128,254,18,18,12}},   // Ъ
  {0x042B, {254,18,12,0,254}},   // Ы
  {0x042C, {238,2,18,12,0}},   // Ь (b)
  {0x042D, {130,146,146,146,124}},   // Э
  {0x042E, {254,16,124,130,124}},   // Ю
  {0x042F, {98,148,152,144,254}},   // Я
  
  {0x0430, {4,42,42,42,30}},   // а
  {0x0431, {60,82,82,146,140}},   // б
  {0x0432, {62,42,42,20,0}},   // в
  {0x0433, {62,32,32,32,48}},   // г
  {0x0434, {39,42,50,34,63}},   // д
  {0x0435, {28,42,42,42,24}},   // е
  {0x0451, {28,170,42,170,24}},    // ё
  {0x0436, {54,8,62,8,54}},   // ж
  {0x0437, {34,34,42,42,20}},   // з
  {0x0438, {62,4,8,16,62}},   // и
  {0x0439, {62,132,72,144,62}},   // й
  {0x043A, {62,8,20,34,0}},   // к
  {0x043B, {4,34,60,32,62}},   // л
  {0x043C, {62,16,8,16,62}},   // м
  {0x043D, {62,8,8,8,62}},   // н
  {0x043E, {28,34,34,34,28}},   // о
  {0x043F, {62,32,32,32,62}},   // п
  {0x0440, {62,40,40,40,16}},   // р
  {0x0441, {28,34,34,34,20}},   // с
  {0x0442, {32,32,62,32,32}},   // т
  {0x0443, {48,10,10,10,60}},   // у
  {0x0444, {24,36,126,36,24}},   // ф
  {0x0445, {34,20,8,20,34}},   // х
  {0x0446, {62,2,2,2,63}},   // ц
  {0x0447, {48,8,8,8,62}},   // ч
  {0x0448, {62,2,62,2,62}},   // ш
  {0x0449, {62,2,62,2,63}},   // щ
  {0x044A, {32,62,10,10,4}},   // ъ
  {0x044B, {62,10,4,0,62}},   // ы
  {0x044C, {62,10,10,4,0}},   // ь
  {0x044D, {20,34,42,42,28}},   // э
  {0x044E, {62,8,28,34,28}},   // ю
  {0x044F, {16,42,44,40,62}}   // я*/
};

class CustomRusSymbols : public CustomizedLanguage {
public:
  CustomRusSymbols(LCDI2C_UTF8* lcd) : CustomizedLanguage(lcd) {
    CustomLetterNum = SymbolCustomRusNum;
    CustomLetters = SymbolCustomRus;
  }

protected:
  uint8_t getAlternativeLetter(uint16_t c) override {
    switch (c) {
      case 0x0410: // А
        return 0x41;
      case 0x0412: // В
        return 0x42;
      case 0x0415: // Е
        return 0x45;
      case 0x041C: // М
        return 0x4D;
      case 0x041D: // Н
        return 0x48;
      case 0x041E: // О
        return 0x4F;
      case 0x0420: // Р
        return 0x50;
      case 0x0421: // С
        return 0x43;
      case 0x0422: // Т
        return 0x54;
      case 0x0425: // Х
        return 0x58;
      case 0x042C: // Ь
        return 0x62;
      case 0x0430: // а
        return 0x61;
      case 0x0435: // е
        return 0x65;
      case 0x043E: // о
        return 0x6F;
      case 0x0440: // р
        return 0x70;
      case 0x0441: // с
        return 0x63;
      case 0x0443: // у
        return 0x79;
      case 0x0445: // х
        return 0x78;
      default:
        return '*';
    }
  }
};

class LCDI2C_CustomRus : public LCDI2C_Russian {
public:
  LCDI2C_CustomRus(uint8_t lcd_addr, uint8_t lcd_cols, uint8_t lcd_rows)
  : LCDI2C_Russian(lcd_addr, lcd_cols, lcd_rows) {
	customizedLanguage = new CustomRusSymbols(static_cast<LCDI2C_UTF8*>(this));
  }
};


unsigned int utf8GetCharSize(unsigned char bt) {
  if (bt < 128) return 1;
  else if ((bt & 0xE0) == 0xC0) return 2;
  else if ((bt & 0xF0) == 0xE0) return 3;
  else if ((bt & 0xF8) == 0xF0) return 4;
  else if ((bt & 0xFC) == 0xF8) return 5;
  else if ((bt & 0xFE) == 0xFC) return 6;

  return 1;
}

String getChar(String& data, int& pos){
  if (pos >= data.length())
    return "";
  int next = utf8GetCharSize(data[pos]);
  String buf = data.substring(pos, pos+next);
  pos += next;
  Serial.println(buf);
  return buf;
}


#endif rusLCD_Custom_h