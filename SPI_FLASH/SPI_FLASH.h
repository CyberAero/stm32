#ifndef SPI_FLASH_H_
#define SPI_FLASH_H_

#include "Common.h"
#include "SPI.h"

#define SPI_FLASH_SPI(a) SPI1_Transmit(a)
#define SPI_FLASH_CE_ENABLE  GPIOA->ODR |= GPIO_ODR_ODR3
#define SPI_FLASH_CE_DISABLE GPIOA->ODR &= ~GPIO_ODR_ODR3

#define SPIFLASH_WRITEENABLE      0x06        // write enable
#define SPIFLASH_WRITEDISABLE     0x04        // write disable

#define SPIFLASH_BLOCKERASE_4K    0x20        // erase one 4K block of flash memory
#define SPIFLASH_BLOCKERASE_32K   0x52        // erase one 32K block of flash memory
#define SPIFLASH_BLOCKERASE_64K   0xD8        // erase one 64K block of flash memory
#define SPIFLASH_CHIPERASE        0x60        // chip erase (may take several seconds depending on size)
// but no actual need to wait for completion (instead need to check the status register BUSY bit)
#define SPIFLASH_STATUSREADL      0x05        // read status register
#define SPIFLASH_STATUSREADH      0x35        // read status register
#define SPIFLASH_STATUSWRITE      0x01        // write status register
#define SPIFLASH_ARRAYREAD        0x0B        // read array (fast, need to add 1 dummy byte after 3 address bytes)
#define SPIFLASH_ARRAYREADLOWFREQ 0x03        // read array (low frequency)

#define SPIFLASH_SLEEP            0xB9        // deep power down
#define SPIFLASH_WAKE             0xAB        // deep power wake up
#define SPIFLASH_BYTEPAGEPROGRAM  0x02        // write (1 to 256bytes)
#define SPIFLASH_JEDEC            0x9F        // read JEDEC manufacturer and device ID (2 bytes, specific bytes for each manufacturer and device)
#define SPIFLASH_SERIAL           0x4B        // read serial (only winbond)

#define FLASH_WP_NO_PROTECT 0x00
#define FLASH_WP_PIN_TO_RESET 0x01
#define FLASH_WP_PROTECTION_TO_RESET 0x02
#define FLASH_WP_PERMANENT_PROTECTION 0x03

#define FLASH_WRITETYPE_WORD	0x01
#define FLASH_WRITETYPE_CONT	0x02

uint8_t FLASH_INIT();
void FLASH_PROTECT_MODE(uint8_t);

uint32_t FLASH_JEDEC();
uint64_t FLASH_GETSERIAL();

uint8_t FLASH_READBYTE(uint32_t);
void FLASH_READBYTES(uint32_t, uint8_t*, uint16_t);

void FLASH_WRITEBYTE(uint32_t, uint8_t);
void FLASH_WRITEBYTES(uint32_t, const uint8_t*, uint8_t);

void FLASH_COMMAND(uint8_t, uint8_t);
void FLASH_SELECT(uint8_t);
uint8_t FLASH_BUSY();

uint16_t FLASH_READSTATUS();
void FLASH_WRITESTATUS(uint16_t);

void FLASH_CHIP_ERASE();
void FLASH_SECTORERASE4K(uint16_t);
void FLASH_BLOCKERASE32K(uint16_t);
void FLASH_BLOCKERASE64K(uint16_t);

void FLASH_SLEEP();
void FLASH_WAKE();

#endif
