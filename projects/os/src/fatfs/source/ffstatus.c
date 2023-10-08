/**
 * @file    ffstatus.h
 * @author  Anton Tchekov
 * @version 0.1
 * @date    2023-10-02
 */

#include "ffstatus.h"

const char *f_status_str(FRESULT status)
{
	const char *msgs[] =
	{
		"Ok",                        /* FR_OK = 0,              (0) Succeeded */
		"I/O error",                 /* FR_DISK_ERR,            (1) A hard error occurred in the low level disk I/O layer */
		"Internal error",            /* FR_INT_ERR,             (2) Assertion failed */
		"Not ready",                 /* FR_NOT_READY,           (3) The physical drive cannot work */
		"No such file or directory", /* FR_NO_FILE,             (4) Could not find the file */
		"No such path",              /* FR_NO_PATH,             (5) Could not find the path */
		"Invalid path format",       /* FR_INVALID_NAME,        (6) The path name format is invalid */
		"Access denied",             /* FR_DENIED,              (7) Access denied due to prohibited access or directory full */
		"Already exists",            /* FR_EXIST,               (8) Access denied due to prohibited access */
		"Invalid object",            /* FR_INVALID_OBJECT,      (9) The file/directory object is invalid */
		"Write protected",           /* FR_WRITE_PROTECTED,     (10) The physical drive is write protected */
		"Invalid drive number",      /* FR_INVALID_DRIVE,       (11) The logical drive number is invalid */
		"Not mounted",               /* FR_NOT_ENABLED,         (12) The volume has no work area */
		"No filesystem",             /* FR_NO_FILESYSTEM,       (13) There is no valid FAT volume */
		"MKFS unknown error",        /* FR_MKFS_ABORTED,        (14) The f_mkfs() aborted due to any problem */
		"Timeout",                   /* FR_TIMEOUT,             (15) Could not get a grant to access the volume within defined period */
		"Locked",                    /* FR_LOCKED,              (16) The operation is rejected according to the file sharing policy */
		"Not enough memory",         /* FR_NOT_ENOUGH_CORE,     (17) LFN working buffer could not be allocated */
		"Too many open files",       /* FR_TOO_MANY_OPEN_FILES, (18) Number of open files > FF_FS_LOCK */
		"Invalid parameter",         /* FR_INVALID_PARAMETER    (19) Given parameter is invalid */
	};

	return msgs[status];
}
