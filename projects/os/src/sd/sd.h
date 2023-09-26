/**
 * @file    sd.h
 * @author  Anton Tchekov
 * @version 0.1
 * @date    2023-09-24
 * @brief   SD-card driver
 */

#ifndef __SD_H__
#define __SD_H__

#include "types.h"

/** SD card data */
typedef struct
{
	/** SD card serial number */
	u32 _serial = 0;

	/** SD card capacity in blocks */
	u32 _capacity;

	/** SD card OEM name: 2 characters + null-terminator */
	u8 OEM[3];

	/** SD card product name: 5 characters + null-terminator */
	u8 ProductName[6];

	/** SD manufacturer ID */
	u8 Manufacturer = 0;

	/** SD card revision number as packed BCD */
	u8 Revision = 0;

	/** SD card manufacturing year after 2000 */
	u8 _manufacturing_year = 0;

	/** SD card manufacturing month number */
	u8 _manufacturing_month = 0;

	/** SD card copy/original flag */
	u8 _flag_copy;

	/** SD card write protected flag */
	u8 _flag_write_protect;

	/** SD card temporarily write protected flag */
	u8 _flag_write_protect_temp;

	/** SD card format ID */
	u8 _format;

	/** SD card type: SD1 / SD2 / SDHC */
	u8 _card_type;
} SD;

enum
{
	SD_SUCCESS,
	SD_FAILURE,
	SD_TIMEOUT
};

typedef u8r SD_Status;

/**
 * @brief Get SD card capacity
 *
 * @param sd Pointer to SD card struct
 * @return Capacity in 512-byte blocks
 */
static inline u32r sd_size(SD *sd)
{
	return sd->_capacity;
}

/**
 * @brief Initialize SD card
 *
 * @return Status code
 */
SD_Status sd_init(SD *sd);

/**
 * @brief Read multiple 512-byte blocks from SD-card
 *
 * @param start Block number to start at
 * @param count Number of blocks to read
 * @param buf Pointer to buffer for read data
 * @return Status code
 */
SD_Status sd_read(SD *sd, u32r start, u32r count, void *data);

/**
 * @brief Write multiple 512-byte blocks to SD-card
 *
 * @param start Block number to start at
 * @param count Number of blocks to write
 * @param buf Pointer to buffer with data to write
 * @return Status code
 */
SD_Status sd_write(SD *sd, u32r start, u32r count, const void *data);

#endif /* __SD_H__ */
