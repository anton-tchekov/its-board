#if 0

/**
 * @file    sd.c
 * @author  Anton Tchekov
 * @version 0.1
 * @date    2023-09-24
 */

#include "sd.h"
#include "sd_ll.h"
#include "spi_ll.h"
#include "delay.h"
#include <stdio.h>
#include <inttypes.h>

/** SD card block size in bytes */
#define SD_BLOCK_SIZE           512

/** SD card block size as a power of two */
#define SD_BLOCK_SIZE_POT         9

/** SD go to idle state command */
#define CMD_GO_IDLE_STATE      0x00

/** SD send IF cond */
#define CMD_SEND_IF_COND       0x08

/** SD read card-specific data register */
#define CMD_SEND_CSD           0x09

/** SD read card identification number */
#define CMD_SEND_CID           0x0A

/** SD set block length command */
#define CMD_SET_BLOCKLEN       0x10

/** SD read single block command */
#define CMD_READ_SINGLE_BLOCK  0x11

/** SD write single block command */
#define CMD_WRITE_SINGLE_BLOCK 0x18

/** SDHC set OP cond */
#define CMD_SEND_OP_COND       0x01

/** SD set OP cond */
#define CMD_SD_SEND_OP_COND    0x29

/** SD send app command */
#define CMD_APP                0x37

/** SD read operation conditions register */
#define CMD_READ_OCR           0x3A

/** SD idle */
#define IDLE_STATE             (1 << 0)

/** SD illegal command */
#define ILLEGAL_CMD            (1 << 2)

/** SD card type 1 */
#define SD_1                   (1 << 0)

/** SD card type 2 */
#define SD_2                   (1 << 1)

/** SDHC card type */
#define SD_HC                  (1 << 2)

/**
 * @brief Execute an SD command (panic if failed)
 *
 * @param cmd Command number
 * @param arg Address parameter
 * @return Output byte
 */
static u8 _sd_command(u8 cmd, u32 arg)
{
	u8 i, response;
	spi_ll_xchg(0xFF);
	spi_ll_xchg(0x40 | cmd);
	spi_ll_xchg((arg >> 24) & 0xFF);
	spi_ll_xchg((arg >> 16) & 0xFF);
	spi_ll_xchg((arg >> 8) & 0xFF);
	spi_ll_xchg((arg >> 0) & 0xFF);
	switch(cmd)
	{
	case CMD_GO_IDLE_STATE:
		spi_ll_xchg(0x95);
		break;

	case CMD_SEND_IF_COND:
		spi_ll_xchg(0x87);
		break;

	default:
		spi_ll_xchg(0xFF);
		break;
	}

	for(i = 0; i < 10 && ((response = spi_ll_xchg(0xFF)) == 0xFF); ++i) ;
	return response;
}

SD_Status _sd_init(SD *sd)
{
	shell_print("SD Detect\n");

	u8r response;
	u16r i;
	u32r arg;

	sd_ll_deselect();
	spi_ll_slow();
	sd->CardType = 0;
	for(i = 0; i < 10; ++i)
	{
		spi_ll_xchg(0xFF);
	}

	sd_ll_select();
	for(i = 0; ; ++i)
	{
		if(_sd_command(CMD_GO_IDLE_STATE, 0) == IDLE_STATE)
		{
			break;
		}

		if(i == 0x1FF)
		{
			sd_ll_deselect();
			return SD_TIMEOUT;
		}
	}

	if((_sd_command(CMD_SEND_IF_COND, 0x1AA) & ILLEGAL_CMD) == 0)
	{
		spi_ll_xchg(0xFF);
		spi_ll_xchg(0xFF);
		if(((spi_ll_xchg(0xFF) & 0x01) == 0) ||
			(spi_ll_xchg(0xFF) != 0xAA))
		{
			sd_ll_deselect();
			return SD_FAILURE;
		}

		sd->CardType |= SD_2;
	}
	else
	{
		_sd_command(CMD_APP, 0);
		if((_sd_command(CMD_SD_SEND_OP_COND, 0) & ILLEGAL_CMD) == 0)
		{
			sd->CardType |= SD_1;
		}
	}

	for(i = 0; ; ++i)
	{
		if(sd->CardType & (SD_1 | SD_2))
		{
			arg = 0;
			if(sd->CardType & SD_2)
			{
				arg = 0x40000000;
			}

			_sd_command(CMD_APP, 0);
			response = _sd_command(CMD_SD_SEND_OP_COND, arg);
		}
		else
		{
			response = _sd_command(CMD_SEND_OP_COND, 0);
		}

		if((response & IDLE_STATE) == 0)
		{
			break;
		}

		if(i == 0x7FFF)
		{
			sd_ll_deselect();
			return SD_TIMEOUT;
		}
	}

	if(sd->CardType & SD_2)
	{
		if(_sd_command(CMD_READ_OCR, 0))
		{
			sd_ll_deselect();
			return SD_FAILURE;
		}

		if(spi_ll_xchg(0xFF) & 0x40)
		{
			sd->CardType |= SD_HC;
		}

		spi_ll_xchg(0xFF);
		spi_ll_xchg(0xFF);
		spi_ll_xchg(0xFF);
	}

	if(_sd_command(CMD_SET_BLOCKLEN, SD_BLOCK_SIZE))
	{
		sd_ll_deselect();
		return SD_FAILURE;
	}

	sd_ll_deselect();
	spi_ll_fast();
	delay_ms(20);
	return SD_SUCCESS;
}

SD_Status _sd_info(SD *sd)
{
	shell_print("SD Info\n");

	u8r i, b, csd_read_bl_len, csd_c_size_mult, csd_structure;
	u16r j, csd_c_size;

	sd->Serial = 0;
	sd->Manufacturer = 0;
	sd->Revision = 0;
	sd->ManufacturingYear = 0;
	sd->ManufacturingMonth = 0;

	sd_ll_select();

	/* Read CID register */
	_sd_command(CMD_SEND_CID, 0);
	j = 0;
	while(spi_ll_xchg(0xFF) != 0xFE)
	{
		if(++j > 0x7FFF)
		{
			sd_ll_deselect();
			return SD_TIMEOUT;
		}
	}

	for(i = 0; i < 18; ++i)
	{
		b = spi_ll_xchg(0xFF);
		switch(i)
		{
			case 0:
				sd->Manufacturer = b;
				break;

			case 1:
			case 2:
				sd->OEM[i - 1] = b;
				break;

			case 3:
			case 4:
			case 5:
			case 6:
			case 7:
				sd->ProductName[i - 3] = b;
				break;

			case 8:
				sd->Revision = b;
				break;

			case 9:
			case 10:
			case 11:
			case 12:
				sd->Serial |= (u32)b << ((12 - i) * 8);
				break;

			case 13:
				sd->ManufacturingYear = b << 4;
				break;

			case 14:
				sd->ManufacturingYear |= b >> 4;
				sd->ManufacturingMonth = b & 0x0f;
				break;
		}
	}

	/* Read CSD register */
	csd_read_bl_len = 0;
	csd_c_size_mult = 0;
	csd_structure = 0;
	csd_c_size = 0;

	_sd_command(CMD_SEND_CSD, 0);
	j = 0;
	while(spi_ll_xchg(0xFF) != 0xFE)
	{
		if(++j > 0x7FFF)
		{
			sd_ll_deselect();
			return SD_TIMEOUT;
		}
	}

	for(i = 0; i < 18; ++i)
	{
		b = spi_ll_xchg(0xFF);
		if(i == 0)
		{
			csd_structure = b >> 6;
		}
		else if(i == 14)
		{
			if(b & 0x40)
			{
				sd->FlagCopy = 1;
			}

			if(b & 0x20)
			{
				sd->FlagWriteProtect = 1;
			}

			if(b & 0x10)
			{
				sd->FlagTemporaryWriteProtect = 1;
			}

			sd->Format = (b & 0x0C) >> 2;
		}
		else
		{
			if(csd_structure == 0x01)
			{
				switch(i)
				{
					case 7:
						b &= 0x3f;
						/* fall through */

					case 8:
					case 9:
						csd_c_size <<= 8;
						csd_c_size |= b;
						++csd_c_size;
						sd->Capacity = (u32)csd_c_size << 10;
				}
			}
			else if(csd_structure == 0x00)
			{
				switch(i)
				{
					case 5:
						csd_read_bl_len = b & 0x0F;
						break;

					case 6:
						csd_c_size = b & 0x03;
						csd_c_size <<= 8;
						break;

					case 7:
						csd_c_size |= b;
						csd_c_size <<= 2;
						break;

					case 8:
						csd_c_size |= b >> 6;
						++csd_c_size;
						break;

					case 9:
						csd_c_size_mult = b & 0x03;
						csd_c_size_mult <<= 1;
						break;

					case 10:
						csd_c_size_mult |= b >> 7;
						sd->Capacity = ((u32)csd_c_size <<
							(csd_c_size_mult + csd_read_bl_len + 2)) >> 9;
						break;
				}
			}
		}
	}

	sd_ll_deselect();
	return SD_SUCCESS;
}

SD_Status sd_init(SD *sd)
{
	shell_print("SD Init\n");
	return _sd_init(sd) && _sd_info(sd);
}

void sd_info_print(SD *sd)
{
	char buf[128];

	/* Print disk info */
	sprintf(buf, "Card Type        : %s\n",
		sd->CardType & SD_HC ? "SDHC" : "SD");
	shell_print(buf);

	sprintf(buf, "Manufacturer ID  : %02X\n",
		sd->Manufacturer);
	shell_print(buf);

	sprintf(buf, "OEM              : %s\n",
		sd->OEM);
	shell_print(buf);

	sprintf(buf, "Product Name     : %s\n",
		sd->ProductName);
	shell_print(buf);

	sprintf(buf, "Revision         : %c.%c\n",
		(sd->Revision >> 4) + '0', (sd->Revision & 0x0F) + '0');
	shell_print(buf);

	sprintf(buf, "Serial Number    : 0x%08"PRIx32"\n",
		sd->Serial);
	shell_print(buf);

	sprintf(buf, "Manufacture Date : %02d-%d\n",
		sd->ManufacturingMonth, 2000 + sd->ManufacturingYear);
	shell_print(buf);

	sprintf(buf, "Capacity         : %"PRIu32" blocks\n",
		sd->Capacity);
	shell_print(buf);

	sprintf(buf, "                   (%"PRIu32" bytes)\n",
		sd->Capacity * SD_BLOCK_SIZE);
	shell_print(buf);

	sprintf(buf, "Format           : 0x%02X\n",
		sd->Format);
	shell_print(buf);

	sprintf(buf, "Flags            : %s%s\n",
		sd->FlagCopy ? "Copy, " : "Original, ",
		sd->FlagTemporaryWriteProtect ? "Temporarily Write Protected" :
			(sd->FlagWriteProtect ? "Write Protected" :
			"Rewritable"));
	shell_print(buf);
}

/**
 * @brief Convert block number to internal SD address
 *        since SD uses byte address but SDHC uses block number
 *
 * @param block Block number
 * @return Internal Address
 */
static u32r _sd_block_addr(SD *sd, u32r block)
{
	return sd->CardType & SD_HC ? block : (block << SD_BLOCK_SIZE_POT);
}

SD_Status sd_read(SD *sd, u32r start, u32r count, void *data)
{
	u16 i;
	u8 *data8, response;

	data8 = data;
	sd_ll_select();

	/* Start read */
	RETURN_IF(_sd_command_try(CMD_READ_SINGLE_BLOCK,
		_sd_block_addr(block)));

	/* Wait for ready */
	for(i = 0; ; ++i)
	{
		response = spi_ll_xchg(0xFF);
		if(response == 0xFE)
		{
			break;
		}

		if(i == 0xFFFF)
		{
			sd_ll_deselect();
			return SD_TIMEOUT;
		}
	}

	/* Read data */
	for(i = 0; i < SD_BLOCK_SIZE; ++i)
	{
		data8[i] = spi_ll_xchg(0xFF);
	}

	spi_ll_xchg(0xFF);
	spi_ll_xchg(0xFF);
	spi_ll_xchg(0xFF);
	sd_ll_deselect();
	return SD_SUCCESS;
}

SD_Status sd_write(SD *sd, u32r start, u32r count, const void *data)
{
	u16 i;
	u8 response;
	const u8 *data8;

	data8 = data;
	sd_ll_select();

	/* Start write */
	RETURN_IF(_sd_command_try(CMD_WRITE_SINGLE_BLOCK,
		_sd_block_addr(block)))
	spi_ll_xchg(0xFE);
	for(i = 0; i < SD_BLOCK_SIZE; ++i)
	{
		spi_ll_xchg(*data8++);
	}

	spi_ll_xchg(0xFF);
	spi_ll_xchg(0xFF);

	/* Wait for ready */
	for(i = 0; ; ++i)
	{
		response = spi_ll_xchg(0xFF);
		if(response == 0xFF)
		{
			break;
		}

		if(i == 0xFFFF)
		{
			sd_ll_deselect();
			return SD_TIMEOUT;
		}
	}

	spi_ll_xchg(0xFF);
	sd_ll_deselect();
	return SD_SUCCESS;
}
#endif