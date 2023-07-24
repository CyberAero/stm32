#include "SPI_FLASH.h"

struct _s_device{
	uint8_t	_manufacturer_id;
	uint8_t	_type_id;
	uint8_t	_device_id;
	uint16_t  _size;			// in Mbits
	uint32_t	_pages;			// highest page number available
	const char* 	_manufacturer_name;
	const char* 	_device_type;
	const char* 	_device_name;
	uint8_t	_cmd_s;		// command for page write
};
const struct _s_device _device[]= {
	{0x00, 0x00, 0x00, 0x0000, 0x00000000, "(Unknown manufacturer)", "(Unknown type)", "(Unknown device)",	0x00},
	{0xEF, 0x40, 0x14, 0x0008, 0x00000fff, "Winbond",			"SPI Serial Flash", "W25Q08BV",		0x01},
	{0xEF, 0x40, 0x15, 0x0010, 0x00001fff, "Winbond",			"SPI Serial Flash", "W25Q16BV",		0x01},
	{0xEF, 0x40, 0x16, 0x0020, 0x00003fff, "Winbond",			"SPI Serial Flash", "W25Q32BV",		0x01},
	{0xEF, 0x40, 0x17, 0x0040, 0x00007fff, "Winbond",			"SPI Serial Flash", "W25Q64BV",		0x01},
	{0xEF, 0x40, 0x18, 0x0080, 0x0000ffff, "Winbond",			"SPI Serial Flash", "W25Q128BV", 0x01},
	{0xC2, 0x20, 0x15, 0x0010, 0x00001fff, "MXIC",			"SPI Serial Flash",	"MX25L1605D",	0x02},
	{0xC2, 0x20, 0x16, 0x0020, 0x00003fff, "MXIC",			"SPI Serial Flash",	"MX25L3205D",	0x02},
	{0xC2, 0x20, 0x17, 0x0040, 0x00007fff, "MXIC",			"SPI Serial Flash",	"MX25L6405D",	0x02}
};

struct _s_device_cmd{
	uint8_t	WRITEENABLE;
	uint8_t	WRITEDISABLE;
	uint8_t	SECTORERASE4K;
	uint8_t	BLOCKERASE32K;
	uint8_t	BLOCKERASE64K;
	uint8_t CHIPERASE;
	uint8_t STATUSREADH;
	uint8_t STATUSREADL;
	uint8_t STATUSWRITE;
	uint8_t ARRAYREADFAST;
	uint8_t ARRAYREADLOW;
	uint8_t SLEEP;
	uint8_t WAKE;
	uint8_t BYTEPAGEPROGRAM;
	uint8_t JEDEC;
	uint8_t SERIAL;
};
const struct _s_device_cmd _device_cmd[]= {
	{0x06, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x9F, 0x00}, // none
	{0x06, 0x04, 0x20, 0x52, 0xD8, 0x60, 0x35, 0x05, 0x01, 0x0B, 0x03, 0xB9, 0xAB, 0x02, 0x9F, 0x4B}, // Winbond W25QxxBV
	{0x06, 0x04, 0x20, 0x52, 0xD8, 0x60, 0x00, 0x05, 0x01, 0x0B, 0x03, 0xB9, 0xAB, 0x02, 0x9F, 0x00}, // MXIC MX25Lxx05D
	{0x06, 0x04, 0x20, 0x52, 0xD8, 0x60, 0x00, 0x05, 0x01, 0x0B, 0x03, 0xB9, 0xAB, 0x02, 0x9F, 0x00}, // MXIC MX25Lxx05P
};

uint16_t _selected_device = 0x00;

uint8_t FLASH_INIT()
{
	FLASH_COMMAND(_device_cmd[_device[_selected_device]._cmd_s].STATUSWRITE, 1);
	SPI_FLASH_SPI(0);
	//SPI_TRANSFER(0);
	FLASH_SELECT(0x00);
	uint32_t JEDEC = FLASH_JEDEC();
	for(uint16_t i = 0; i < (sizeof(_device) / sizeof(struct _s_device)); i++){
		if ((_device[i]._manufacturer_id == (uint8_t)(JEDEC>>16)) & (_device[i]._type_id == (uint8_t)(JEDEC>>8)) & (_device[i]._device_id == (uint8_t)JEDEC))
		{
			_selected_device = i;
			break;
		}
	}
	return 1;
}
void FLASH_PROTECT_MODE(uint8_t ProtectMode)
{
	uint16_t oldStatus = FLASH_READSTATUS();
	
	if(ProtectMode & 0x01) oldStatus |= (1 << 7);
	else oldStatus &= ~(1 << 7);
	
	if(ProtectMode & 0x02) oldStatus |= (1 << 8);
	else oldStatus &= ~(1 << 8);

	FLASH_WRITESTATUS(oldStatus);
}

uint32_t FLASH_JEDEC()
{
	FLASH_COMMAND(_device_cmd[_device[_selected_device]._cmd_s].JEDEC, 0);
	uint32_t jedecid = (uint32_t)SPI_FLASH_SPI(0) << 16;
	jedecid |=  (uint64_t)SPI_FLASH_SPI(0) << 8;
	jedecid |=  (uint64_t)SPI_FLASH_SPI(0);
	FLASH_SELECT(0x00);
	return jedecid;
}
uint64_t FLASH_GETSERIAL()
{
	FLASH_COMMAND(_device_cmd[_device[_selected_device]._cmd_s].SERIAL, 0);
	uint64_t jedecid = (uint64_t)SPI_FLASH_SPI(0) << 56;
	jedecid |=  (uint64_t)SPI_FLASH_SPI(0) << 48;
	jedecid |=  (uint64_t)SPI_FLASH_SPI(0) << 40;
	jedecid |=  (uint64_t)SPI_FLASH_SPI(0) << 32;
	jedecid |=  (uint64_t)SPI_FLASH_SPI(0) << 24;
	jedecid |=  (uint64_t)SPI_FLASH_SPI(0) << 16;
	jedecid |=  (uint64_t)SPI_FLASH_SPI(0) << 8;
	jedecid |=  (uint64_t)SPI_FLASH_SPI(0);
	FLASH_SELECT(0x00);
	return jedecid;
}

uint8_t FLASH_READBYTE(uint32_t addr)
{
	FLASH_COMMAND(_device_cmd[_device[_selected_device]._cmd_s].ARRAYREADLOW, 0);
	SPI_FLASH_SPI(addr >> 16);
	SPI_FLASH_SPI(addr >> 8);
	SPI_FLASH_SPI(addr);
	uint8_t result = SPI_FLASH_SPI(0);
	FLASH_SELECT(0x00);
	return result;
}
void FLASH_READBYTES(uint32_t addr, uint8_t* buf, uint16_t len)
{
	FLASH_COMMAND(_device_cmd[_device[_selected_device]._cmd_s].ARRAYREADFAST, 0);
	SPI_FLASH_SPI(addr >> 16);
	SPI_FLASH_SPI(addr >> 8);
	SPI_FLASH_SPI(addr);
	SPI_FLASH_SPI(0);  //"dont care"
	for (uint16_t i = 0; i < len; i++)
	buf[i] = SPI_FLASH_SPI(0);
	FLASH_SELECT(0x00);
}

void FLASH_WRITEBYTE(uint32_t addr, uint8_t byte)
{
	FLASH_COMMAND(_device_cmd[_device[_selected_device]._cmd_s].BYTEPAGEPROGRAM, 1);
	SPI_FLASH_SPI(addr >> 16);
	SPI_FLASH_SPI(addr >> 8);
	SPI_FLASH_SPI(addr);
	SPI_FLASH_SPI(byte);
	FLASH_SELECT(0x00);
}
void FLASH_WRITEBYTES(uint32_t addr, const uint8_t* buf, uint8_t len)
{
	FLASH_COMMAND(_device_cmd[_device[_selected_device]._cmd_s].BYTEPAGEPROGRAM, 1);  // Byte/Page Program
	SPI_FLASH_SPI(addr >> 16);
	SPI_FLASH_SPI(addr >> 8);
	SPI_FLASH_SPI(addr);
	for (uint8_t i = 0; i < len; i++)
		SPI_FLASH_SPI(buf[i]);
	FLASH_SELECT(0x00);
}

void FLASH_COMMAND(uint8_t _CMD, uint8_t _ISWRITE)
{
	if(_ISWRITE)
	{
		FLASH_COMMAND(_device_cmd[_device[_selected_device]._cmd_s].WRITEENABLE, 0);
		//USART_SEND('a');
		FLASH_SELECT(0x00);
	}
	while(FLASH_BUSY());
	FLASH_SELECT(0x01);
	SPI_FLASH_SPI(_CMD);
}

void FLASH_SELECT(uint8_t _SELECTED)
{
	if (_SELECTED == 0x01) SPI_FLASH_CE_DISABLE;
	else  SPI_FLASH_CE_ENABLE;
}

uint8_t FLASH_BUSY()
{
	return FLASH_READSTATUS() & 0x0001;
}

uint16_t FLASH_READSTATUS()
{
	FLASH_SELECT(0x01);
	SPI_FLASH_SPI(_device_cmd[_device[_selected_device]._cmd_s].STATUSREADL);
	uint16_t status = SPI_FLASH_SPI(0);
	FLASH_SELECT(0x00);
	FLASH_SELECT(0x01);
	SPI_FLASH_SPI(_device_cmd[_device[_selected_device]._cmd_s].STATUSREADH);
	status |= SPI_FLASH_SPI(0) << 8;
	FLASH_SELECT(0x00);
	return status;
}

void FLASH_WRITESTATUS(uint16_t byte)
{
	FLASH_COMMAND(_device_cmd[_device[_selected_device]._cmd_s].STATUSWRITE, 1);
	SPI_FLASH_SPI(byte);
	SPI_FLASH_SPI(byte >> 8);
	FLASH_SELECT(0x00);
}

void FLASH_CHIP_ERASE()
{
	FLASH_COMMAND(_device_cmd[_device[_selected_device]._cmd_s].CHIPERASE, 1);
	FLASH_SELECT(0x00);
}

void FLASH_SECTORERASE4K(uint16_t sector) {
	uint32_t sectorWrite = sector * 4096;
	FLASH_COMMAND(_device_cmd[_device[_selected_device]._cmd_s].SECTORERASE4K, 1);
	SPI_FLASH_SPI(sectorWrite >> 16);
	SPI_FLASH_SPI(sectorWrite >> 8);
	SPI_FLASH_SPI(sectorWrite);
	FLASH_SELECT(0x00);
}

void FLASH_BLOCKERASE32K(uint16_t block) {
	uint32_t blockWrite = block * 32768;
	FLASH_COMMAND(_device_cmd[_device[_selected_device]._cmd_s].BLOCKERASE32K, 1);
	SPI_FLASH_SPI(blockWrite >> 16);
	SPI_FLASH_SPI(blockWrite >> 8);
	SPI_FLASH_SPI(blockWrite);
	FLASH_SELECT(0x00);
}
void FLASH_BLOCKERASE64K(uint16_t block) {
	uint32_t blockWrite = block * 65536;
	FLASH_COMMAND(_device_cmd[_device[_selected_device]._cmd_s].BLOCKERASE64K, 1);
	SPI_FLASH_SPI(blockWrite >> 16);
	SPI_FLASH_SPI(blockWrite >> 8);
	SPI_FLASH_SPI(blockWrite);
	FLASH_SELECT(0x00);
}

void FLASH_SLEEP()
{
	FLASH_COMMAND(_device_cmd[_device[_selected_device]._cmd_s].SLEEP, 0);
	FLASH_SELECT(0x00);
}
void FLASH_WAKE()
{
	FLASH_COMMAND(_device_cmd[_device[_selected_device]._cmd_s].WAKE, 0);
	FLASH_SELECT(0x00);
}