#ifndef COMMENT_BLOCK_H
#define COMMENT_BLOCK_H

#include "list.h"

/**
 * Struct return_t
 *
 * It collects information of the return type of a function
 *
 * @version 2.2.2
 * @author Faiz Suleimanov
 */
typedef struct return_t {
	char* type;
	char* description;
} return_t;

/**
 * @brief Struct comment_t
 * 
 * @details It collects all information inside a special comment
 * 
 * @version 2.2.2
 * @author Faiz Suleimanov
 */
typedef struct comment_t {
	list_t* params;
	char* brief;
	list_t* details;
	return_t* return_tag;
	char* version_tag;
	char* author_tag;
} comment_t;

#endif
