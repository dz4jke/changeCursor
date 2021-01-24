/*все что отностися к дисплею I2C*/
#include <LiquidCrystal_I2C.h>           // подключаем библиотеку для работы с ЖК-дисплеем через I2C
#define cols 20                          // длинна строки в дисплее
#define rows 4                           // количестов строк в дисплее
LiquidCrystal_I2C lcd(0x27, cols, rows); // опледеляем адрес дисплея, длинну строки, количество строк(пины подключения А4,А5)

#define ButtPin 5
#include "GyverButton.h"
GButton butt1(ButtPin);

//положение курсора
byte CursorPos = 0;
byte PrevPos = 0;

//энкодер
#define CLK 4
#define DT 3
#define SW 2
#include "GyverEncoder.h"
Encoder enc1(CLK, DT, SW); // для работы c кнопкой

byte speed = 0;
bool fan = false;

//функция вывода в терминал
void menuStrokPORT()
{
  Serial.print(">");
  Serial.println("Speed");
  Serial.println(" Fan");
  Serial.println(" Temperature");
  Serial.println(" etc.");
}

//функция вывода на дисплей
void menuStrok_LCD()
{

  lcd.setCursor(1, 0); //выводим первый символ на перву строку нулевую клетку
  lcd.print("Speed");
  lcd.setCursor(7, 0); //выводим первый символ на перву строку нулевую клетку
  lcd.print(speed);
  lcd.setCursor(1, 1);
  lcd.print("Fan");
  lcd.setCursor(7, 1);
  lcd.print(fan);
  lcd.setCursor(1, 2);
  lcd.print("Temperature");
  lcd.setCursor(1, 3);
  lcd.print("etc.");
}

// очистка первого столбца на дисплее
void ClearFirstColomn()
{
  for (byte i = 0; i <= 1; i++)
  {
    lcd.setCursor(0, i);
    lcd.print(" ");
  }
}

// функция получения позиции курсора
void ChangePos()
{
  PrevPos = CursorPos;
  if (enc1.isClick())
  {

    if (CursorPos >= 1)
      CursorPos = 0;
    else
      CursorPos++;
  }
}

// функция отрисовки курсора
void SetCursor(byte x)
{
  if (PrevPos != CursorPos)
    ClearFirstColomn();
  switch (x)
  {
  case 0:
  enc1.tick();
  if (enc1.isLeft()) speed++;
  if (enc1.isRight())speed--;
  if (enc1.isRightH())speed-=50;
  if (enc1.isLeftH())speed+=50;
    lcd.setCursor(0, 0);
    lcd.print(">");
    break;

  case 1:
  enc1.tick();
  if (enc1.isTurn())  fan=!fan;
    lcd.setCursor(0, 1);
    lcd.print(">");
    break;
  }
}

// обработка энкодера
/*void EncoderState()
{
  enc1.tick();
  if (enc1.isLeft() && CursorPos == 0) speed++;
  if (enc1.isRight() && CursorPos == 0)speed--;
  if (enc1.isLeft() && CursorPos == 1)  fan++;
  if (enc1.isRight() && CursorPos == 1)  fan--;
  
}*/

void setup()
{
  enc1.setType(TYPE2);
  lcd.init();      // инициализируем дисплей (если использовать дисплей без I2C, то в скобках необходимо указать его размер(20,4))
  lcd.backlight(); // подсветка дисплея
  Serial.begin(9600);
  butt1.setDebounce(80);      // настройка антидребезга (по умолчанию 80 мс)
  butt1.setTimeout(500);      // настройка таймаута на удержание (по умолчанию 500 мс)
  butt1.setClickTimeout(300); // настройка таймаута между кликами (по умолчанию 300 мс)
  // HIGH_PULL - кнопка подключена к GND, пин подтянут к VCC (BTN_PIN --- КНОПКА --- GND)
  // LOW_PULL  - кнопка подключена к VCC, пин подтянут к GND
  // по умолчанию стоит HIGH_PULL
  butt1.setType(HIGH_PULL);
  // NORM_OPEN - нормально-разомкнутая кнопка
  // NORM_CLOSE - нормально-замкнутая кнопка
  // по умолчанию стоит NORM_OPEN
  butt1.setDirection(NORM_OPEN);

  //menuStrokPORT();
}

void loop()
{

  //Serial.println(CursorPos);

// EncoderState();
enc1.tick();
  ChangePos();
  SetCursor(CursorPos);
  menuStrok_LCD();
  //Serial.print(PrevPos);
  //Serial.print("      ");
  //Serial.print(speed);
  //Serial.print("      ");
  //Serial.println(fan);
}
