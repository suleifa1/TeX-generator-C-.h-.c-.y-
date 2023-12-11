#include <stdlib.h>
#include <string.h>
#include "text.h"


char* text_copy(char* s) {
	char* result = malloc(strlen(s) + 1);
	strncpy(result, s, strlen(s) + 1);
	return result;
}


bool text_equal_func(char* s1, char* s2) {
	return !strcmp(s1, s2);
}
