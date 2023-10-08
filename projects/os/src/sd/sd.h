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
	u32 Serial;

	/** SD card capacity in blocks */
	u32 Capacity;

	/** SD card OEM name: 2 characters + null-terminator */
	u8 OEM[3];

	/** SD card product name: 5 characters + null-terminator */
	u8 ProductName[6];

	/** SD manufacturer ID */
	u8 Manufacturer;

	/** SD card revision number as packed BCD */
	u8 Revision;

	/** SD card manufacturing year after 2000 */
	u8 ManufacturingYear;

	/** SD card manufacturing month number */
	u8 ManufacturingMonth;

	/** SD card type: SD1 / SD2 / SDHC */
	u8 CardType;
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
	return sd->Capacity;
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
