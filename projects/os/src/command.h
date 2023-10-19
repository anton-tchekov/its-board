/**
 * @file    command.h
 * @author  Anton Tchekov
 * @version 0.1
 * @date    2023-10-02
 * @brief   Run shell command
 */

#ifndef __COMMAND_H__
#define __COMMAND_H__

/**
 * @brief Run a shell command
 *
 * @param cmd Command to run
 * @param len Length of the string
 */
void command_run(const char *cmd, int len);

/**
 * @brief Print a file system error message
 *
 * @param ret Error code
 */
void fserror(int ret);

#endif /* __COMMAND_H__ */
