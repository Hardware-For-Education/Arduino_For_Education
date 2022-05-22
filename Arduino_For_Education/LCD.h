const int PIN_SCE = 10;    // Chip Select
const int PIN_DC = 12;     // Data/Command
const int PIN_SDIN = 11;   // Data in
const int PIN_SCLK = 13;   // Clk

#include "ASCII.h"

const int LCD_LOW = LOW;
const int LCD_HIGH = HIGH;
const int LCD_X = 84;
const int LCD_Y = 48;

// Enviar byte a la pantalla
void LcdWrite(byte dc, byte data)
{
  digitalWrite(PIN_DC, dc);
  digitalWrite(PIN_SCE, LOW);
  shiftOut(PIN_SDIN, PIN_SCLK, MSBFIRST, data);
  digitalWrite(PIN_SCE, HIGH);
}

// Mostrar caracter por pantalla
void LcdCharacter(char character)
{
  for (int index = 0; index < 5; index++)
  {
    LcdWrite(LCD_HIGH, ASCII[character - 0x20][index]);
  }
  LcdWrite(LCD_HIGH, 0x00);
}

// Inicializar el LCD
void LcdInitialise(void)
{
  pinMode(PIN_SCE, OUTPUT);
  pinMode(PIN_DC, OUTPUT);
  pinMode(PIN_SDIN, OUTPUT);
  pinMode(PIN_SCLK, OUTPUT);


  LcdWrite(LCD_LOW, 0x21);  // LCD Extended Commands
  LcdWrite(LCD_LOW, 0xBF);  // Set LCD Vop (Contrast)
  LcdWrite(LCD_LOW, 0x04);  // Set Temp coefficent
  LcdWrite(LCD_LOW, 0x14);  // LCD bias mode 1:40

  LcdWrite(LCD_LOW, 0x20);  // LCD basic commands
  LcdWrite(LCD_LOW, 0x0C);  // LCD normal mode
}

// Posicionar cursor en x,y
void gotoXY(int x, int y)
{
  LcdWrite(0, 0x80 | x);  // Column.
  LcdWrite(0, 0x40 | (y - 1)); // Row.
}

// Borrar pantalla
void LcdClear(void)
{
  for (int index = 0; index < LCD_X * LCD_Y / 8; index++)
  {
    LcdWrite(LCD_HIGH, 0x00);
  }
}

// Mostrar string por pantalla
void LcdString(char const *characters)
{
  while (*characters)
  {
    LcdCharacter(*characters++);
  }
}

// Dibujar caja (dos lineas horizontales y dos verticales)
void rectangle(int pos_x, int pos_y, int length_rec, int width_rec)
{
  unsigned char j;
  for (j = (pos_x * 6) - 6; j < ((width_rec + 2) * 6); j++) // top
  {
    gotoXY(j, pos_y);
    LcdWrite(1, 0x01);
  }
  for (j = (pos_x * 6) - 6; j < ((width_rec + 2) * 6); j++) //Bottom
  {
    gotoXY(j, length_rec + 1);
    LcdWrite(1, 0x80);
  }

  for (j = pos_y; j < length_rec + 2; j++) // Right
  {
    gotoXY(((width_rec + 2) * 6), j);
    LcdWrite(1, 0xff);
  }
  for (j = pos_y; j < length_rec + 2; j++) // Left
  {
    gotoXY((pos_x * 6) - 6, j);
    LcdWrite(1, 0xff);
  }
}

void circle(int pos_x, int pos_y, int length_rec, int width_rec)
{
  unsigned char j;
  ///
  gotoXY(36, 1);
  LcdWrite(1, 0x01);
  LcdWrite(1, 0x01);
  LcdWrite(1, 0x01);


  gotoXY(39, 1);
  LcdWrite(1, 0x02);
  LcdWrite(1, 0x02);

  gotoXY(41, 1);
  LcdWrite(1, 0x04);

  gotoXY(42, 1);
  LcdWrite(1, 0x08);

  gotoXY(43, 1);
  LcdWrite(1, 0x30);

  gotoXY(44, 1);
  LcdWrite(1, 0xC0);

  ///

  gotoXY(33, 1);
  LcdWrite(1, 0x01);
  LcdWrite(1, 0x01);
  LcdWrite(1, 0x01);

  gotoXY(31, 1);
  LcdWrite(1, 0x02);
  LcdWrite(1, 0x02);

  gotoXY(30, 1);
  LcdWrite(1, 0x04);

  gotoXY(29, 1);
  LcdWrite(1, 0x08);

  gotoXY(28, 1);
  LcdWrite(1, 0x30);

  gotoXY(27, 1);
  LcdWrite(1, 0xC0);

  ///

  gotoXY(27, 2);
  LcdWrite(1, 0x03);

  gotoXY(28, 2);
  LcdWrite(1, 0x0C);

  gotoXY(29, 2);
  LcdWrite(1, 0x10);

  gotoXY(30, 2);
  LcdWrite(1, 0x20);

  gotoXY(33, 2);
  LcdWrite(1, 0x80);
  LcdWrite(1, 0x80);
  LcdWrite(1, 0x80);

  gotoXY(31, 2);
  LcdWrite(1, 0x40);
  LcdWrite(1, 0x40);

  ///

  gotoXY(44, 2);
  LcdWrite(1, 0x03);

  gotoXY(43, 2);
  LcdWrite(1, 0x0C);

  gotoXY(42, 2);
  LcdWrite(1, 0x10);

  gotoXY(41, 2);
  LcdWrite(1, 0x20);

  gotoXY(39, 2);
  LcdWrite(1, 0x40);
  LcdWrite(1, 0x40);

  gotoXY(36, 2);
  LcdWrite(1, 0x80);
  LcdWrite(1, 0x80);
  LcdWrite(1, 0x80);

}

void triangle(int pos_x, int pos_y, int alt)
{
  unsigned char j;
  for (j = (3 + (6 * (pos_x - 1))) - (8 * alt); j <= (3 + (6 * (pos_x - 1))) + (8 * alt); j++) //Bottom
  {
    gotoXY(j, pos_y + alt);
    LcdWrite(1, 0x01);
  }
  unsigned char i;
  byte data;
  int cont = 0;

  for (j = 0; j < alt * 8 ; j = j + 8) {
    for (i = j, data = 0x01; i < j + 8; i++, data = data * 2) //Bottom
    {
      gotoXY((3 + (6 * (pos_x - 1))) + i, pos_y + cont);
      LcdWrite(1, data);

      gotoXY((3 + (6 * (pos_x - 1))) - i, pos_y + cont);
      LcdWrite(1, data);
    }
    cont++;
  }
}

void write_line(char const *characters, int y)
{
  gotoXY(0, y);
  LcdString(characters);
}
