#ifndef UTILS_H
#define UTILS_H

/**
 * @breif Checks functions on error code
 * @param returnVal status from passed functions
 * @return EXIT_FAILURE on error with description of error or EXIT_SUCCESS if no
 * error occured
 * @note Maybe I shouldn't do this as separate function, but I just keep it
 */
int check_on_error(int returnVal);

#endif // !UTILS_H
