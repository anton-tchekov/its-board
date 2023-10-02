/**
 * @file    ffstatus.h
 * @author  Anton Tchekov
 * @version 0.1
 * @date    2023-10-02
 * @brief   FATFS status code to string
 */

#ifndef __FFSTATUS_H__
#define __FFSTATUS_H__

#include "ff.h"

/**
 * @brief Get a human-readable error message for a FATFS error code
 *
 * @param status FRESULT status code
 * @return Status string
 */
const char *f_status_str(FRESULT status);

#endif /* __FFSTATUS_H__ */
