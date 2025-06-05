/*
 * printf_retarget.hpp
 *
 *  Created on: Jun 4, 2025
 *      Author: snortman
 */

#ifndef INC_PRINTF_RETARGET_HPP_
#define INC_PRINTF_RETARGET_HPP_

// retarget.cpp
#include <errno.h>
#include <sys/unistd.h>        // STDOUT_FILENO, STDERR_FILENO
#include <sys/stat.h>

extern Serial usart3;

extern "C" int _write(int file, char* ptr, int len)
{
    // file:  STDOUT_FILENO (1) or STDERR_FILENO (2); you could check if file==STDOUT_FILENO etc.
    if ((file != STDOUT_FILENO) && (file != STDERR_FILENO)) {
        errno = EBADF;
        return -1;
    }

    usart3.write((const uint8_t *)ptr, (size_t)len);

    return len; // number of bytes written
}

#endif /* INC_PRINTF_RETARGET_HPP_ */
