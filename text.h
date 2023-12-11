#ifndef TEXT_H
#define TEXT_H

#include <stdbool.h>


/**
 * Copy text
 *    
 * The function copies C-string (like 'strdup')
 *
 * @param char* s C-string to copy
 * @return char* Pointer to new c-string
 * @version 1.0.0
 * @author Faiz Suleimanov
 */
char* text_copy(char* s);

/**
 * Text equivalence function
 *
 * Details of the function
 *
 * @param char* s1 First C-string to compare
 * @param char* s2 Second C-string to compare
 * @return bool Result of comparison (1(true) - equal, 0(false) - no)
 * @version 1.0.0
 * @author Faiz Suleimanov
 */
bool text_equal_func(char* s1, char* s2);

#endif
