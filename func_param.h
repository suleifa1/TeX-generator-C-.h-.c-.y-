#ifndef FUNC_PARAM_H
#define FUNC_PARAM_H

/**
 * Struct func_param_t
 *
 * This structure represents the metadata of a function's parameter for documentation purposes. It contains a 'signature' field, 
 * which should hold the data type and the name of the parameter as it appears in a function's definition, and a 'description' field,
 * which should contain an explanation of what the parameter is used for. The structure is intended for use with documentation
 * generation tools that output LaTeX formatted documentation, as evidenced by the 'process_param' function which formats these fields
 * into LaTeX syntax.
 *
 * @param signature A char pointer to a string that represents the parameter's declaration, including its data type and name.
 * @param description A char pointer to a string that provides a description of the parameter's purpose and usage.
 * @version 2.2.2  
 * @author Faiz Suleimanov
 */
typedef struct func_param_t {
	char* signature;
	char* description;
} func_param_t;


/**
 * Frees the memory allocated for a function parameter object.
 *
 * This function safely deallocates memory assigned to a function parameter object,
 * including its signature and description. It checks for NULL pointers before
 * attempting to free to avoid undefined behavior.
 * 
 * @param func_param_t* param The function parameter object to be freed. If NULL, the function performs no operation.
 * @version 1.0.0
 * @author Faiz Suleimanov
 */ 
void func_param_free(func_param_t* param);


#endif