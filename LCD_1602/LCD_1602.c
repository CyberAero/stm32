#include <LCD_1602.h>


uint8_t _addr = DEFAULT_ADDRESS;
uint8_t _backlightval = LIGHT;

void LCD1602_Begin(uint8_t lcd_addr) {
	_addr = lcd_addr;
	LCD1602_Write4bits(0x30);
	Common_Delay(Micros(4500));
	LCD1602_Write4bits(0x30);
	Common_Delay(Micros(150));
	LCD1602_Write4bits(0x20);
  
	// Параметры дисплея
	LCD1602_Send(LCD_FUNCTIONSET | (LCD_4BITMODE | LCD_2LINE | LCD_5x8DOTS), 0);
  
	// Включить дисплей
	LCD1602_Send(LCD_DISPLAYCONTROL | (LCD_DISPLAYON | LCD_CURSOROFF | LCD_BLINKOFF), 0);
  
	// Очистить
	LCD1602_Send(LCD_CLEARDISPLAY, 0);
  
	// set the entry mode
	LCD1602_Send(LCD_ENTRYMODESET | (LCD_ENTRYLEFT | LCD_ENTRYSHIFTDECREMENT), 0);
}

void LCD1602_Clear() {
	LCD1602_Send(LCD_CLEARDISPLAY, 0);
}

void LCD1602_Cursor(uint8_t Position, uint8_t Line) {
	int row_offsets[] = {0x00, 0x40};
	LCD1602_Send(LCD_SETDDRAMADDR | (Position + row_offsets[Line]), 0);
}

void LCD1602_WriteStr(uint8_t* _str, uint8_t Length) {
	for (uint8_t i = 0; i < Length; i++) LCD1602_Send(_str[i], RS);
}

void LCD1602_Send(uint8_t _data, uint8_t _mode) {
	LCD1602_Write4bits((_data & 0xf0) | _mode);
	LCD1602_Write4bits(((_data << 4) & 0xf0) | _mode);
}

void LCD1602_Write4bits(uint8_t _data) {
	LCD_1602_Write(_data | EN);
	Common_Delay(Micros(1));

	LCD_1602_Write(_data & ~EN);
	Common_Delay(Micros(50));
}

void LCD_1602_Write(uint8_t _data) {
	//LCD_1602_SEND_DATA((_data) | _backlightval);
	//LCD_1602_SEND_DATA((uint16_t)(_data) | _backlightval);
	uint8_t __data = (_data) | _backlightval;
	I2C2_Transmit(_addr, &__data, 0);
}

void LCD1602_SetBacklight(uint8_t _backlight) {
  if (_backlight) _backlightval = LIGHT;
  else _backlightval = 0x00;
	LCD_1602_Write(0);
}

void LCD1602_CreateChar(uint8_t location, uint8_t charmap[]) {
	LCD1602_Send(LCD_SETCGRAMADDR | ((location & 0x7) << 3), 0);
	for (uint8_t i=0; i<8; i++) {
		LCD1602_Send(charmap[i], RS);
	}
}
