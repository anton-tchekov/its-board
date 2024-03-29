/*-----------------------------------------------------------------------*/
/* Low level disk I/O module SKELETON for FatFs     (C)ChaN, 2019        */
/*-----------------------------------------------------------------------*/
/* If a working storage control module is available, it should be        */
/* attached to the FatFs via a glue function rather than modifying it.   */
/* This is an example of glue functions to attach various exsisting      */
/* storage control modules to the FatFs module with a defined API.       */
/*-----------------------------------------------------------------------*/

#include "ff.h"			/* Obtains integer types */
#include "diskio.h"		/* Declarations of disk functions */
#include "ramdisk.h"
#include "sd/sd.h"

/* Definitions of physical drive number for each drive */
#define DEV_RAM		0	/* Example: Map Ramdisk to physical drive 0 */
#define DEV_MMC		1	/* Example: Map MMC/SD card to physical drive 1 */

/*-----------------------------------------------------------------------*/
/* Get Drive Status                                                      */
/*-----------------------------------------------------------------------*/

extern SD sd;

DSTATUS disk_status (
	BYTE pdrv		/* Physical drive nmuber to identify the drive */
)
{
	switch (pdrv)
	{
	case DEV_RAM:
		return 0;

	case DEV_MMC:
		return 0;
	}

	return STA_NOINIT;
}

/*-----------------------------------------------------------------------*/
/* Initialize a Drive                                                    */
/*-----------------------------------------------------------------------*/

DSTATUS disk_initialize (
	BYTE pdrv				/* Physical drive nmuber to identify the drive */
)
{
	switch(pdrv)
	{
	case DEV_RAM:
		return 0;

	case DEV_MMC:
		return sd_init(&sd) ? STA_NOINIT : 0;
	}

	return STA_NOINIT;
}

/*-----------------------------------------------------------------------*/
/* Read Sector(s)                                                        */
/*-----------------------------------------------------------------------*/

DRESULT disk_read (
	BYTE pdrv,		/* Physical drive nmuber to identify the drive */
	BYTE *buff,		/* Data buffer to store read data */
	LBA_t sector,	/* Start sector in LBA */
	UINT count		/* Number of sectors to read */
)
{
	switch(pdrv)
	{
#ifdef ENABLE_RAMDISK
	case DEV_RAM:
		ramdisk_read(buff, sector, count);
		return RES_OK;
#endif

	case DEV_MMC:
		return sd_read(&sd, sector, count, buff) ? RES_ERROR : RES_OK;
	}

	return RES_PARERR;
}

/*-----------------------------------------------------------------------*/
/* Write Sector(s)                                                       */
/*-----------------------------------------------------------------------*/

#if FF_FS_READONLY == 0

DRESULT disk_write (
	BYTE pdrv,			/* Physical drive nmuber to identify the drive */
	const BYTE *buff,	/* Data to be written */
	LBA_t sector,		/* Start sector in LBA */
	UINT count			/* Number of sectors to write */
)
{
	switch(pdrv)
	{
#ifdef ENABLE_RAMDISK
	case DEV_RAM:
		ramdisk_write(buff, sector, count);
		return RES_OK;
#endif

	case DEV_MMC:
		return sd_write(&sd, sector, count, buff) ? RES_ERROR : RES_OK;
	}

	return RES_PARERR;
}

#endif


/*-----------------------------------------------------------------------*/
/* Miscellaneous Functions                                               */
/*-----------------------------------------------------------------------*/

DRESULT disk_ioctl (
	BYTE pdrv,		/* Physical drive nmuber (0..) */
	BYTE cmd,		/* Control code */
	void *buff		/* Buffer to send/receive control data */
)
{
	switch(pdrv)
	{
	case DEV_RAM:
		switch(cmd)
		{
		case GET_SECTOR_COUNT:
			*(LBA_t *)buff = 192;
			break;

		case GET_BLOCK_SIZE:
			*(DWORD *)buff = 1;
			break;
		}
		return RES_OK;

	case DEV_MMC:
		switch(cmd)
		{
		case GET_SECTOR_COUNT:
			*(LBA_t *)buff = sd.Capacity;
			break;

		case GET_BLOCK_SIZE:
			*(DWORD *)buff = 1;
			break;
		}
		return RES_ERROR;
	}

	return RES_PARERR;
}
