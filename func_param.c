#include <stdlib.h>
#include "func_param.h"

void func_param_free(func_param_t* param) {
	if(param== NULL) return;
	if (param->signature != NULL)   free(param->signature);
	if (param->description != NULL) free(param->description);
	free(param);
}
