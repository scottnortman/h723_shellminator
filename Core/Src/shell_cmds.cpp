/*
 * shell_cmds.cpp
 *
 *  Created on: Jun 2, 2025
 *      Author: snortman
 */

#include "shell_cmds.hpp"

/**
 * @brief  Step 1: In‐place sanitization only.
 *
 *   - Converts uppercase letters to lowercase.
 *   - Collapses any run of ' ' into exactly one ' '.
 *   - Trims leading and trailing spaces.
 *   - Leaves exactly one '\0' at the very end of the buffer.
 *
 * @param  str     Pointer to a mutable, null-terminated string.
 *                 On return, it is sanitized:
 *                    * no leading/trailing spaces
 *                    * no duplicate spaces
 *                    * all‐lowercase
 */
static void shell_sanitize(char *str)
{
    if (str == NULL) {
        return;
    }

    char *read_ptr  = str;
    char *write_ptr = str;
    bool  in_space  = true;   // assume “before start” is “in a space”

    // 1) Skip leading spaces
    while (*read_ptr == ' ') {
        read_ptr++;
    }

    // 2) Copy characters, collapsing runs of spaces
    while (*read_ptr != '\0') {
        char c = *read_ptr++;

        if (c == ' ') {
            if (in_space) {
                // we’re already “in a space run,” so skip
                continue;
            }
            // write exactly one space
            *write_ptr++ = ' ';
            in_space = true;
        }
        else {
            // convert uppercase → lowercase
            *write_ptr++ = (char) tolower((unsigned char)c);
            in_space = false;
        }
    }

    // 3) If the last written char is a space, trim it
    if (write_ptr > str && *(write_ptr - 1) == ' ') {
        write_ptr--;
    }

    // 4) Null‐terminate
    *write_ptr = '\0';

}// shell_sanitize

/**
 * @brief  Step 2: Tokenize the already-sanitized string in-place.
 *
 *   - Assumes 'str' has single spaces between tokens, no leading/trailing spaces,
 *     all lowercase, and a '\0' at the very end.
 *   - Walks through 'str', replacing each ' ' → '\0', and collects token pointers in argv[].
 *
 * @param  str     The sanitized string (single spaces, no leading/trailing space).
 * @param  argc    Pointer to uint8_t; on return, *argc = number of tokens found.
 * @param  argv    Array of char* pointers. Must have room for at least as many tokens
 *                 as will appear, plus one extra for the final NULL sentinel.
 *                 On return, argv[0..(*argc – 1)] each point to a null-terminated token in 'str',
 *                 and argv[*argc] == NULL.
 */
static void shell_tokenize(char *str, uint8_t *argc, char **argv)
{
    if (str == NULL || argc == NULL || argv == NULL) {
        if (argc) *argc = 0;
        return;
    }

    *argc = 0;
    char *scan = str;

    while (*scan != '\0') {
        // At start of a token
        argv[*argc] = scan;
        (*argc)++;

        // Advance until space or end
        while (*scan != '\0' && *scan != ' ') {
            scan++;
        }

        if (*scan == ' ') {
            // Replace space with '\0' to terminate this token
            *scan = '\0';
            scan++;  // move to start of next token
        }
    }

    // Put a NULL sentinel at the end
    argv[*argc] = NULL;

}//shell_tokenize

/**
 * @brief  Full wrapper: sanitize, allow you to inspect the full line,
 *         then tokenize in-place.
 *
 * @param  arg_in  The original input buffer (must be writable). On return,
 *                 the buffer has been modified twice:
 *                   1) sanitized (single spaces, lowercase, trimmed)
 *                   2) tokenized   (each internal space → '\0')
 * @param  argc    Pointer to uint8_t; on return, number of arguments found.
 * @param  argv    char* array. On return, argv[i] points to each token, argv[*argc] == NULL.
 *
 * @return The pointer to the (now‐sanitized) buffer, so you can print it *before* tokenization.
 */
static char *shell_argparse(char *arg_in, uint8_t *argc, char **argv)
{
    if (arg_in == NULL || argc == NULL || argv == NULL) {
        if (argc) *argc = 0;
        return arg_in;
    }

    // 1) Sanitize in-place (no '\0' inserted except final)
    shell_sanitize(arg_in);

    // At this point: arg_in is (for example) "arg1 arg2 arg3"
    // with exactly one space between tokens, no leading/trailing spaces.

    // (If you want to see the fully sanitized line, you can print it here.)

    // 2) Tokenize in-place: replace each single space → '\0' and fill argv[]
    shell_tokenize(arg_in, argc, argv);

    return arg_in;
}

//	$ ping
// ping
bool shell_cmds_ping_func( char * args, CommandCaller * caller ){
	(void)args;
	caller->print("ping\r\n");
	return true;
} // shell_cmds_ping_func

//	$ this is a test...
//	this is a test...
bool shell_cmds_echo_func( char * args, CommandCaller * caller ){
    if (args && *args) {
        caller->print(args);
    }
    caller->print("\r\n");
	return true;

} // shell_cmds_echo_func

// $ Cmd ARG1 arg2 arG3
//
bool shell_cmds_echo_args_func( char* args, CommandCaller* caller ) {

	uint8_t argc;
	char *argv[SHELL_CMDS_MAX_TOKENS + 1];

	shell_argparse(args, &argc, argv);

	caller->print("Found ");
	caller->print(argc);
	caller->print(" arguments:\n\r");

	for(uint8_t i=0; i<argc;i++){
		caller->print(i);
		caller->print(": [");
		caller->print(argv[i]);
		caller->print("]\n\r");
	}

	return true;


}// shell_cmds_echo_args_func

// Command: >>> led <i> <0|1>
// ex args:  0x31 0x20 0x31
// TODO:  better argument parsing, ability to read back
bool shell_cmds_led_func( char* args, CommandCaller* caller ) {

	uint8_t led = 0;
	uint8_t val = 0;

	shell_sanitize(args);

	led = 0x0F & args[0];
	val = 0x0F & args[2];

	GPIO_PinState pinState = (GPIO_PinState)(val==0?0u:1u);

    switch (led) {
        case 0:
            HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, pinState);
            break;
        case 1:
            HAL_GPIO_WritePin(GPIOE, GPIO_PIN_1, pinState);
            break;
        case 2:
            HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, pinState);
            break;
        default:
            // This should never happen due to earlier checks
            //caller->println("Error: invalid LED index.");
            return false;
    }

    return true;

}// shell_cmds_led_func






