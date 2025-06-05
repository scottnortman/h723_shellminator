/*
 * shell_cmds.h
 *
 *  Created on: Jun 2, 2025
 *      Author: snortman
 */

#ifndef INC_SHELL_CMDS_HPP_
#define INC_SHELL_CMDS_HPP_

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <errno.h>

#include "Shellminator.hpp"
#include "Shellminator-Commander-Interface.hpp"
#include "Commander-API.hpp"

#include "stm32h7xx_nucleo.h"
#include "stm32h7xx_hal_gpio.h"

#define COMMANDER_BUFFER_SIZE 128u
#define SHELL_CMDS_MAX_TOKENS 32u

//void execution_function( char *command, Shellminator* caller );

// COUNT_ARGS(args) returns the number of whitespace-separated tokens in args,
// ignoring any number of consecutive spaces. If args is NULL or empty, it yields 0.
//
// Uses a GNU “statement expression” to scan at runtime:
//   - Tracks whether we are currently “inside” a token (_in = 1) or “between” tokens (_in = 0).
//   - Whenever we see a non-space character while not already in a token, we bump the count.
//
// Example behavior:
//   COUNT_ARGS("")            ➔ 0
//   COUNT_ARGS("foo")         ➔ 1
//   COUNT_ARGS("foo bar")     ➔ 2
//   COUNT_ARGS("  foo   bar  ") ➔ 2
//   COUNT_ARGS("a  b   c")    ➔ 3
#define COUNT_ARGS(_args)                                      \
    ({                                                         \
        int _count = 0;                                        \
        if ((_args) != NULL) {                                 \
            int _in = 0;                                       \
            for (char* _p = (_args); *_p != '\0'; ++_p) {      \
                if (*_p != ' ' && !_in) {                      \
                    _in = 1;                                   \
                    ++_count;                                  \
                } else if (*_p == ' ') {                       \
                    _in = 0;                                   \
                }                                              \
            }                                                  \
        }                                                      \
        _count;                                                \
    })

// GET_ARG(args, idx) scans the NUL-terminated string `args`, finds the idx-th
// whitespace-separated token, replaces the first space after that token with '\0',
// and returns a pointer to the start of the token. Returns NULL if idx is out of range.
//
// Example tokens in args = "  foo   bar baz  "
//   GET_ARG(args, 0) ➔ points to "foo\0"
//   GET_ARG(args, 1) ➔ points to "bar\0"
//   GET_ARG(args, 2) ➔ points to "baz\0"
//   GET_ARG(args, 3) ➔ NULL
//
// Requirements/Assumptions:
//  • GNU “statement expression” extension (({ … })) is available.
//  • `args` may be modified (space after the chosen token is replaced with '\0').
//  • Tabs or other whitespace chars are not treated specially (only ASCII space ' ').
//
#define GET_ARG(_args, _idx)                                                        \
    ({                                                                              \
        char* _ret = NULL;                                                          \
        if ((_args) != NULL) {                                                      \
            char* _p = (_args);                                                     \
            int _in = 0;         /* 1 if currently inside a token, 0 otherwise */  \
            int _count = 0;      /* token index */                                  \
                                                                                     \
            /* Scan through each character */                                       \
            for (; *_p != '\0'; ++_p) {                                             \
                if (*_p != ' ' && !_in) {                                           \
                    /* Entering a new token */                                      \
                    _in = 1;                                                         \
                    if (_count == (_idx)) {                                         \
                        _ret = _p;  /* Found start of idx-th token */               \
                        break;                                                       \
                    }                                                                \
                    ++_count;                                                        \
                } else if (*_p == ' ') {                                            \
                    /* Exiting token */                                             \
                    _in = 0;                                                         \
                }                                                                    \
            }                                                                        \
            /* If found, null-terminate this token by replacing next space */       \
            if (_ret) {                                                              \
                char* _q = _ret;                                                    \
                while (*_q != '\0' && *_q != ' ') {                                  \
                    ++_q;                                                            \
                }                                                                    \
                if (*_q == ' ') {                                                    \
                    *_q = '\0';                                                      \
                }                                                                    \
            }                                                                        \
        }                                                                            \
        _ret;                                                                        \
    })



// Commander commands
bool shell_cmds_ping_func		( char * args, CommandCaller * caller );
bool shell_cmds_echo_func		( char * args, CommandCaller * caller );
bool shell_cmds_echo_args_func	( char * args, CommandCaller * caller );
bool shell_cmds_led_func		( char * args, CommandCaller * caller );


#endif /* INC_SHELL_CMDS_HPP_ */
