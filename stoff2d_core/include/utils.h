#pragma once

#include <defines.h>

/* list_files_in_dir
 * -----------------
 * Returns an array of all the filenames in a directory terminated by NULL.
 *
 * User is responsible for freeing all the strings in the array and the array 
 * itself.
 */
char** list_files_in_dir(const char* dirPath);

/* utils_sleep
 * -----------
 * Sleep the current thread.
 *
 * seconds:
 *     amound of time in seconds to sleep.
 */
void utils_sleep(f32 seconds);

