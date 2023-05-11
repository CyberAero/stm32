#ifndef __LCD_1602__H
#define __LCD_1602__H
/**************************INCLUDES***************************/
#include "Common.h"
#include "I2C.h"

/***************************DEFINES***************************/
#define LCD_1602_SEND_DATA(Addr, Data) I2C1_BeginTransmission(); I2C2_Write(Addr, Data); I2C1_EndTransmission();

// commands
#define LCD_CLEARDISPLAY 0x01
#define LCD_ENTRYMODESET 0x04
#define LCD_DISPLAYCONTROL 0x08
#define LCD_FUNCTIONSET 0x20
#define LCD_SETCGRAMADDR 0x40
#define LCD_SETDDRAMADDR 0x80

// flags for display entry mode
#define LCD_ENTRYLEFT 0x02
#define LCD_ENTRYSHIFTDECREMENT 0x00

// flags for display on/off control
#define LCD_DISPLAYON 0x04
#define LCD_CURSOROFF 0x00
#define LCD_BLINKOFF 0x00

// flags for function set
#define LCD_4BITMODE 0x00
#define LCD_2LINE 0x08
#define LCD_5x8DOTS 0x00

#define DEFAULT_ADDRESS 0x27

#define RS 0x01
#define RW 0x02
#define EN 0x04
#define LIGHT 0x08

void LCD1602_Begin(uint8_t lcd_addr);
void LCD1602_Clear();
void LCD1602_Cursor(uint8_t, uint8_t);
void LCD1602_WriteStr(uint8_t* _str, uint8_t Length);
void LCD1602_Send(uint8_t, uint8_t);
void LCD1602_SetBacklight(uint8_t);
void LCD1602_CreateChar(uint8_t, uint8_t[]);
void LCD1602_Write4bits(uint8_t);
void LCD_1602_Write(uint8_t);

#endif // !__LCD_1602__H
