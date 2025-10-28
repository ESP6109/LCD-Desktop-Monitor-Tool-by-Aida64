#include <U8g2lib.h>
#include <Arduino.h>
U8G2_KS0108_ERM19264_F u8g2(U8G2_R2, 13, 12, 14, 27, 26, 25, 33, 32, 17, 16, 5, 18, 19, U8X8_PIN_NONE);  // Set R/W to low!
// U8G2_KS0108_ERM19264_F u8g2(U8G2_R2, D0, D1, D2, D3, D4, D5, D6, D7, EN, CS, CS0, CS1, CS2, RESET);  // Set R/W to low!
// U8G2_ST7920_128X64_F_SW_SPI u8g2(U8G2_R2, 5, 4, 14, U8X8_PIN_NONE);
//                          u8g2(U8G2_R0, E , RW , RS , RST);
//                                        D1  D2  D5


char inByte;
char Data[91];
int Yea = 0, Mon = 0, Day = 0;
int Hou = 0, Min = 0, Sec = 0;
int Wee = 0;
int CPUT = 0, GPUT = 0;
int MemP = 0;
int CPUF = 0, GPUF = 0;

void week() {
  if (Mon == 1 || Mon == 2)
    Wee = (Day + 2 * (Mon + 12) + 3 * (Mon + 12 + 1) / 5 + (Yea - 1) + (Yea - 1) / 4 - (Yea - 1) / 100 + (Yea - 1) / 400) % 7;
  else
    Wee = (Day + 2 * Mon + 3 * (Mon + 1) / 5 + Yea + Yea / 4 - Yea / 100 + Yea / 400) % 7;
  Wee++;
}


void getdata() {
  int tc = 0, tg = 0;
  if (Serial.available() > 0) {
    inByte = Serial.read();
    if (inByte == 0xFE) {
      Data[0] = inByte;
      inByte = Serial.read();
      if (inByte == 0x80) {
        Data[1] = inByte;
        for (int i = 2; i < 87; ++i) {
          while (Serial.available() == 0)
            ;
          Data[i] = Serial.read();
        }
      }
    }
  }
  for (int i = 10; i < 14; ++i)
    if (Data[i] >= '0' && Data[i] <= '9') {
      ++tc;
    }
  for (int i = 14; i < 18; ++i)
    if (Data[i] >= '0' && Data[i] <= '9') {
      ++tg;
    }
  if (tc == 4)
    CPUF = (Data[10] - 48) * 1000 + (Data[11] - 48) * 100 + (Data[12] - 48) * 10 + (Data[13] - 48);
  else if (tc == 3)
    CPUF = (Data[10] - 48) * 100 + (Data[11] - 48) * 10 + (Data[12] - 48);
  else if (tc == 2)
    CPUF = (Data[10] - 48) * 10 + (Data[11] - 48);
  else if (tc == 1)
    CPUF = (Data[10] - 48);
  else
    CPUF = 0;
  if (tg == 4)
    GPUF = (Data[14] - 48) * 1000 + (Data[15] - 48) * 100 + (Data[16] - 48) * 10 + (Data[18] - 48);
  else if (tg == 3)
    GPUF = (Data[14] - 48) * 100 + (Data[15] - 48) * 10 + (Data[16] - 48);
  else if (tg == 2)
    GPUF = (Data[14] - 48) * 10 + (Data[15] - 48);
  else if (tg == 1)
    GPUF = (Data[14] - 48);
  else
    GPUF = 0;
  CPUT = (Data[4] - 48) * 10 + (Data[5] - 48);
  GPUT = (Data[6] - 48) * 10 + (Data[7] - 48);
  MemP = (Data[8] - 48) * 10 + (Data[9] - 48);
  Yea = (Data[24] - 48) * 1000 + (Data[25] - 48) * 100 + (Data[26] - 48) * 10 + (Data[27] - 48);
  Mon = (Data[29] - 48) * 10 + (Data[30] - 48);
  Day = (Data[32] - 48) * 10 + (Data[33] - 48);
  Hou = (Data[34] - 48) * 10 + (Data[35] - 48);
  Min = (Data[37] - 48) * 10 + (Data[38] - 48);
  Sec = (Data[40] - 48) * 10 + (Data[41] - 48);
  week();
}

// 19264
void display() {
  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_helvB12_te);
  u8g2.setCursor(50, 14);
  u8g2.printf("%4d.%02d.%02d-%d", Yea, Mon, Day, Wee);
  u8g2.setCursor(65, 30);
  u8g2.printf("%02d:%02d:%02d", Hou, Min, Sec);
  u8g2.setCursor(33, 46);
  u8g2.printf("%d°C", CPUT);
  if (CPUF >= 1000)
    u8g2.setCursor(33, 62);
  else if (CPUF >= 100 && CPUF <= 999)
    u8g2.setCursor(42, 62);
  u8g2.printf("%d", CPUF);
  u8g2.setCursor(123, 46);
  u8g2.printf("%02d°C", GPUT);
  if (GPUF >= 1000)
    u8g2.setCursor(124, 62);
  else if (GPUF >= 100 && GPUF <= 999)
    u8g2.setCursor(133, 62);
  else if (GPUF >= 10 && GPUF <= 99)
    u8g2.setCursor(142, 62);
  else
    u8g2.setCursor(151, 62);
  u8g2.printf("%d", GPUF);
  u8g2.setFont(u8g2_font_logisoso24_tr);
  u8g2.setCursor(73, 60);
  u8g2.printf("%2d%%", MemP);
  u8g2.sendBuffer();
}

// 12864
/*void display() {
  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_helvB12_te);
  u8g2.setCursor(18, 14);
  u8g2.printf("%4d.%02d.%02d-%d", Yea, Mon, Day, Wee);
  u8g2.setCursor(33, 30);
  u8g2.printf("%02d:%02d:%02d", Hou, Min, Sec);
  u8g2.setCursor(1, 46);
  u8g2.printf("%d°C", CPUT);
  if (CPUF >= 1000)
    u8g2.setCursor(1, 62);
  else if (CPUF >= 100 && CPUF <= 999)
    u8g2.setCursor(10, 62);
  else if (GPUF >= 10 && GPUF <= 99)
    u8g2.setCursor(19, 62);
  else
    u8g2.setCursor(28, 62);
  u8g2.printf("%d", CPUF);
  u8g2.setCursor(91, 46);
  u8g2.printf("%02d°C", GPUT);
  if (GPUF >= 1000)
    u8g2.setCursor(92, 62);
  else if (GPUF >= 100 && GPUF <= 999)
    u8g2.setCursor(101, 62);
  else if (GPUF >= 10 && GPUF <= 99)
    u8g2.setCursor(110, 62);
  else
    u8g2.setCursor(119, 62);
  u8g2.printf("%d", GPUF);
  u8g2.setFont(u8g2_font_logisoso24_tr);
  u8g2.setCursor(41, 60);
  u8g2.printf("%2d%%", MemP);
  u8g2.sendBuffer();
}*/

void setup() {
  pinMode(2, OUTPUT);
  u8g2.begin();
  Serial.begin(9600);
  u8g2.enableUTF8Print();
  while (Serial.available() == 0) {
    u8g2.clearBuffer();
    u8g2.setFont(u8g2_font_logisoso16_tr);
    u8g2.setCursor(57, 24);
    u8g2.printf("Wait for");
    u8g2.setCursor(47, 56);
    u8g2.printf("Connecting");
    u8g2.sendBuffer();
  }
}

void loop() {
  if (Serial.available())
    digitalWrite(2, 1);
  else
    digitalWrite(2, 0);
  delay(1);
  getdata();
  display();
}

/*35, 34,

int Yea = 0, Mon = 0, Day = 0;
int Hou = 0, Min = 0, Sec = 0;
int Wee = 0;
*/
// int button();

/*
#define BU 0

int button() {
  static bool bu_st = 1;
  delay(5);
  if (digitalRead(BU) && !bu_st) {
    bu_st = digitalRead(BU);
    return 1;
  } else {
    bu_st = digitalRead(BU);
    return 0;
  }
}*/




/*static int i = 64;
  while (button())
    i += 4;
  
  u8g2.clearBuffer();
  u8g2.drawVLine(0, 0, 64);
  u8g2.drawVLine(17, 0, 64);
  u8g2.setFont(u8g2_font_open_iconic_all_2x_t);
  u8g2.drawGlyph(1, 16, i);
  u8g2.drawGlyph(1, 32, i + 1);
  u8g2.drawGlyph(1, 48, i + 2);
  u8g2.drawGlyph(1, 64, i + 3);
  // u8g2.drawVLine(100, 0, 64);
  // u8g2.drawVLine(133, 0, 64);
  // u8g2.drawVLine(166, 0, 64);
  u8g2.setFont(u8g2_font_open_iconic_all_8x_t);
  u8g2.drawGlyph(101, 64, i);
  // u8g2.drawGlyph(101, 64, i + 1);
  // u8g2.drawGlyph(134, 32, i + 2);
  // u8g2.drawGlyph(134, 64, i + 3);
  u8g2.setFont(u8g2_font_helvB24_te);
  u8g2.setCursor(20, 28);
  u8g2.printf("%d", i);
  u8g2.sendBuffer();
  if (i > 284) i = 64;
  // delay(500);

*/


/*
void setup() {
  pinMode(10, OUTPUT);
  Serial.begin(9600);
  u8g2.begin();
  u8g2.enableUTF8Print();
  while (Serial.available() == 0) {
    u8g2.clearBuffer();
    u8g2.setFont(u8g2_font_logisoso16_tr);
    u8g2.setCursor(20, 24);
    u8g2.printf("Wait for");
    u8g2.setCursor(10, 56);
    u8g2.printf("Connecting");
    u8g2.sendBuffer();
  }
}
*/









/*
void getdata() {
  int tc = 0, tg = 0;
  if (Serial.available() > 0) {
    inByte = Serial.read();
    if (inByte == 'C') {
      Data[0] = inByte;

      for (int i = 1; i < 40; i++) {
        while (Serial.available() == 0)
          ;
        Data[i] = Serial.read();
      }
      // Serial.println(Data);
    }
  }
  for (int i = 8; i < 12; ++i)
    if (Data[i] >= '0' && Data[i] <= '9') {
      ++tc;
    }
  for (int i = 12; i < 16; ++i)
    if (Data[i] >= '0' && Data[i] <= '9') {
      ++tg;
    }
  CPUT = (Data[2] - 48) * 10 + (Data[3] - 48);
  GPUT = (Data[4] - 48) * 10 + (Data[5] - 48);
  MemP = (Data[6] - 48) * 10 + (Data[7] - 48);
  Yea = (Data[20] - 48) * 1000 + (Data[21] - 48) * 100 + (Data[22] - 48) * 10 + (Data[23] - 48);
  Mon = (Data[25] - 48) * 10 + (Data[26] - 48);
  Day = (Data[28] - 48) * 10 + (Data[29] - 48);
  Hou = (Data[30] - 48) * 10 + (Data[31] - 48);
  Min = (Data[33] - 48) * 10 + (Data[34] - 48);
  Sec = (Data[36] - 48) * 10 + (Data[37] - 48);
  if (tc == 4)
    CPUF = (Data[8] - 48) * 1000 + (Data[9] - 48) * 100 + (Data[10] - 48) * 10 + (Data[11] - 48);
  else if (tc == 3)
    CPUF = (Data[8] - 48) * 100 + (Data[9] - 48) * 10 + (Data[10] - 48);
  else if (tc == 2)
    CPUF = (Data[8] - 48) * 10 + (Data[9] - 48);
  else if (tc == 1)
    CPUF = Data[8] - 48;
  else
    CPUF = 0;
  if (tg == 4)
    GPUF = (Data[12] - 48) * 1000 + (Data[13] - 48) * 100 + (Data[14] - 48) * 10 + (Data[15] - 48);
  else if (tg == 3)
    GPUF = (Data[12] - 48) * 100 + (Data[13] - 48) * 10 + (Data[14] - 48);
  else if (tg == 2)
    GPUF = (Data[12] - 48) * 10 + (Data[13] - 48);
  else if (tg == 1)
    GPUF = Data[12] - 48;
  else
    GPUF = 0;
  week();
}*/
/*
void display() {
  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_helvB12_te);
  u8g2.setCursor(50, 14);
  u8g2.printf("%4d.%02d.%02d-%d", Yea, Mon, Day, Wee);
  u8g2.setCursor(65, 30);
  u8g2.printf("%02d:%02d:%02d", Hou, Min, Sec);
  u8g2.setCursor(33, 46);
  u8g2.printf("%d°C", CPUT);
  if (CPUF >= 1000)
    u8g2.setCursor(33, 62);
  else if (CPUF >= 100 && CPUF <= 999)
    u8g2.setCursor(42, 62);
  u8g2.printf("%d", CPUF);
  u8g2.setCursor(123, 46);
  u8g2.printf("%02d°C", GPUT);
  if (GPUF >= 1000)
    u8g2.setCursor(124, 62);
  else if (GPUF >= 100 && GPUF <= 999)
    u8g2.setCursor(133, 62);
  else if (GPUF >= 10 && GPUF <= 99)
    u8g2.setCursor(142, 62);
  else
    u8g2.setCursor(151, 62);
  u8g2.printf("%d", GPUF);
  u8g2.setFont(u8g2_font_logisoso24_tr);
  u8g2.setCursor(73, 60);
  u8g2.printf("%2d%%", MemP);
  u8g2.sendBuffer();
}
*/






/*

int i = 0;
void loop() {
  // while(Serial.available())++i;
  // for (i = 64; i < 288; ++i) {}
  u8g2.clearBuffer();
u8g2.setFont(u8g2_font_open_iconic_all_8x_t);
    u8g2.drawGlyph(0, 64, i);
    u8g2.setFont(u8g2_font_open_iconic_all_8x_t);
    u8g2.drawGlyph(64, 64, i + 1);
    u8g2.setFont(u8g2_font_open_iconic_all_8x_t);
    u8g2.drawGlyph(128, 64, i + 2);
  u8g2.setFont(u8g2_font_logisoso62_tn);
  u8g2.setCursor(0, 61);
  u8g2.printf("%04d.%02d.%02d-%d\n", Yea, Mon, Day, Wee);
  u8g2.setCursor(30, 30);
  u8g2.printf("%02d:%02d:%02d\n", Hou, Min, Sec);
  u8g2.setFontDirection(0);
  u8g2.setFont(u8g2_font_inb24_mf);
  u8g2.drawStr(0, 30, "U");

  u8g2.setFontDirection(1);
  u8g2.setFont(u8g2_font_inb30_mn);
  u8g2.drawStr(21, 8, "8");

  u8g2.setFontDirection(0);
  u8g2.setFont(u8g2_font_inb24_mf);
  u8g2.drawStr(51, 30, "g");
  u8g2.drawStr(67, 30, "\xb2");

  u8g2.drawHLine(2, 35, 47);
  u8g2.drawHLine(3, 36, 47);
  u8g2.drawVLine(45, 32, 12);
  u8g2.drawVLine(46, 33, 12);
  u8g2.setFont(u8g2_font_4x6_tr);
  u8g2.drawStr(1,54,"github.com/olikraus/u8g2");
  u8g2.sendBuffer();
  // Serial.println(i);
  delay(500);
}
*/





/*int angle = 0;
 {
  put your main code here, to run repeatedly:
  int dt = analogRead(DT);
  int clk = analogRead(CLK);
  int sw = analogRead(SW);
  int t1, t2;
  /*if(!clk)
  {
    delay(100);
      if(!dt)
        angle+=18;
  }
  if(!dt)
  {
    delay(100);
      if(!clk)
        angle-=18;
  }
  if(!clk)t1=millis();
  if(!dt) t2=millis();
  if(!clk)
  if(!clk&&!dt)
  angle=angle+18;
  if (!dt && clk)
  if (!clk && !dt)
  delay(10);
  if (!clk && dt)
  angle = angle - 18;
  while ((!analogRead(CLK)) || (!analogRead(DT)))
    Serial.printf("(%d)(%d)\n", dt, clk);
  if (!(clk && dt)) {
    dt ? angle += 18 : angle -= 18;
    angle += 360;
    Serial.printf("%d\n", angle % 360);
  }
  angle %= 360;
  delay(1);

  delay(1);
  if (!digitalRead(0)) {
    angle = 0;
    Serial.printf("%d\n", angle % 360);
  }
  Serial.printf("(%d)(%d)>%d\n", dt, clk, angle);
  Serial.printf("(%d)-(%d)=%d\n",t1,t2,t1-t2);
  digitalWrite(2,1);
}*/
