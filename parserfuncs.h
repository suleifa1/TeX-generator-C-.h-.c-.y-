#if defined(_MSC_VER)
#define DIRECTORY_SEPARATOR '\\'
#elif defined(__GNUC__)
#define DIRECTORY_SEPARATOR '/'
#endif

#ifndef PARSER_FUNCS_H
#define PARSER_FUNCS_H

#include "list.h"
#include "func_param.h"
#include "comment_block.h"

extern size_t yyleng;

/**
 * External reference to the input file stream for the lexer.
 * This file stream is typically set to stdin by default, but can be reassigned to read from a file.
 */
extern FILE *yyin;

/**
 * External reference to the output file stream for the lexer.
 * This file stream is typically set to stdout by default, but can be reassigned to write to a file.
 */
extern FILE *yyout;

/**
 * External reference to the current lexeme.
 * After calling yylex(), yytext contains the lexeme that was recognized by the lexer.
 */
extern char *yytext;

/**
 * External reference to the lexical analysis function generated by Lex/Flex.
 * When invoked, it scans the input stream defined by yyin and returns the next token's integer code.
 * @return int The token's integer code as defined by the lexical rules.
 */
extern int yylex(void);

/**
 * External reference to the current line number.
 * The lexer increments this value as it reads new lines from the input, allowing for better error reporting.
 */
extern int yylineno;

/**
 * External reference to the function that deallocates any memory allocated by yylex().
 * This should be called when the lexing is complete to avoid memory leaks.
 * @return int A return code of 0 indicates successful destruction.
 */
extern int yylex_destroy(void);

/**
* Error function
*
* The function starts each time the 'yyparse' function
* finds an error of parsing
*
* @param char* message Error message
* @version 1.0.0
* @author \textcopyright{} Faiz Suleimanov
*/
int yyerror(char* message);

/**
 * Put text in Tex-file function
 * 
 * This function writes the specified text to the LaTeX file stream pointed to by 'yyout', followed by a newline.
 * It is used to sequentially add text content to the LaTeX document being generated. 
 * 
 * @param char* text The text to be written to the 'yyout' file.
 * @return void This function does not return a value.
 * @author \textcopyright{} Faiz Suleimanov
 * @version 1.0.0
 */
void put_in_tex(char* text);

/**
 * Process file
 * 
 * This function is responsible for parsing *.h and *.c files. It uses the filename provided to locate and
 * parse the file for documentation comments, converting them into a format suitable for LaTeX output.
 * 
 * @param char* filename The full name of the file to be parsed.
 * @return void This function does not return a value.
 * @author \textcopyright{} Faiz Suleimanov
 * @version 1.0.0
 */
void module_parse_file(char* filename);

/**
 * Clear Comment Block
 *
 * This function is used to clear an existing comment block of its current data, setting all pointers within
 * the structure to NULL. 
 * 
 * @param comment_t* block The comment block to be cleared.
 * @version 1.0.0 
 * @author \textcopyright{} Faiz Suleimanov 
 */
void clear_comment_block(comment_t* block);

/**
 * Process param of a function
 *
 * This function takes a single parameter object, which includes both the parameter's signature and description,
 * and writes it to the LaTeX output stream in a format suitable for documentation. The signature is wrapped with
 * LaTeX's verb command to format it as code, and it is followed by a description. This function is typically called
 * for each parameter in a function's documentation block when generating LaTeX-based software documentation.
 * The output format is designed to be clear and concise, providing the necessary detail for readers of the documentation.
 * 
 * @param func_param_t* param Function param object 
 * @version 1.0.0
 * @author \textcopyright{} Faiz Suleimanov
 */
void process_param(func_param_t* param);

/**
 * Concatenates directory path and filename to form a full file path.
 *
 * Allocates and returns a new string that combines the directory and filename,
 * handling the case where the directory may be NULL. 
 *
 * @param char* current_directory The directory path.
 * @param char* filename The filename to append to the directory.
 * @return char* A newly allocated string containing the full path.
 * @version 1.0.0
 * @author \textcopyright{} Faiz Suleimanov
 */
char* make_full_path(char* current_directory, char* filename);

/**
 * Generates a LaTeX subsubsection for a given function with documentation details.
 *
 * This function takes a comment block associated with a function, an output file handle, and the name of the function.
 * It then generates a LaTeX subsubsection that includes a brief description of the function, detailed explanations,
 * arguments, and information about the return type, the author, and the version of the function's documentation.
 * The brief description is emphasized with the LaTeX `\textbf` command. 
 * Detailed text and parameter information are formatted and inserted into the LaTeX document using a function `put_in_tex`.
 * The function name is handled specifically to accommodate LaTeX formatting, breaking it into lines if it exceeds a certain length.
 * Memory management is handled carefully to clean up the dynamic allocations used during processing to avoid memory leaks.
 *
 * @param comment_t* block The comment block containing the documentation details for the function.
 * @param FILE* outfile The file handle for the LaTeX output file where the documentation will be written.
 * @param char* func The name of the function for which the documentation is generated.
 * @version 1.0.0
 * @author \textcopyright{} Faiz Suleimanov
 */
void process_function(comment_t* block, FILE* outfile, char* afc);

/**
 * Generates a LaTeX subsubsection for a given variable with documentation details.
 *
 * This function processes a comment block associated with a variable, an output file handle, and the name of the variable.
 * It outputs a LaTeX subsubsection that includes a brief description of the variable and detailed explanations if available.
 * The function ensures that the variable name is formatted correctly in the LaTeX document using the texttt command for code styling.
 * After processing the brief and detailed comments, it frees the associated memory to prevent memory leaks.
 * Although the function handles params and return tags, these are typically not used for variables and are thus cleaned up.
 *
 * @param comment_t* block The comment block containing the documentation details for the variable.
 * @param FILE* outfile The file handle for the LaTeX output file where the documentation will be written.
 * @param char* variable The name of the variable for which the documentation is generated.
 * @return void This function does not return a value but writes directly to the output file.
 * @version 1.0.0 
 * @author \textcopyright{} Faiz Suleimanov
 */
void process_variable(comment_t* block, FILE* outfile, char* variable);

/**
 * Generates a LaTeX subsubsection for a given structure with documentation details.
 *
 * This function takes a comment block associated with a structure, an output file handle, and the name of the structure.
 * It then generates a LaTeX subsubsection that includes a brief description of the structure, detailed explanations,
 * and information about the author and version of the structure's documentation.
 * The brief description is added with the LaTeX keyword `\textbf` to emphasize the brief. 
 * The details of the structure are formatted and inserted into the LaTeX document using a function `put_in_tex`.
 * Parameters and return value documentation, if present, are cleaned up to prevent memory leaks.
 * 
 * @param comment_t* block The comment block containing the documentation details for the structure.
 * @param FILE* outfile The file handle for the LaTeX output file where the documentation will be written.
 * @param char* struc The name of the structure for which the documentation is generated.
 * @version 1.0.0
 * @author \textcopyright{} Faiz Suleimanov.
 */
void process_struct(comment_t* block, FILE* outfile, char* struc);

/**
 * Text before last symbol function
 *
 * This function searches for the last occurrence of a given character in a text string and returns a new string
 * containing all the characters from the beginning of the original string up to and including the specified character.
 * If the character is not found, the function returns NULL.
 * @param char* text The input text where the search is performed.
 * @param char ch The character to find in the text.
 * @return char* A newly allocated substring from the beginning to the last occurrence of the character, or NULL if the character is not found.
 * @version 1.0.0 Initial version.
 * @author \textcopyright{} Faiz Suleimanov
 */
char* text_before_last_symbol(char* text, char ch);

/**
 * Current directory function
 *
 * The function takes the directory path from full path of a file.
 * 
 * @param char* filepath The full path of the file from which to extract the directory path.
 * @return char* A newly allocated string representing the directory path, or NULL if no directory separator is present.
 * @version 1.0.0 Initial version of the function to extract the directory from a full file path.
 * @author \textcopyright{} Faiz Suleimanov
 */
char* text_current_directory(char* filepath);

#endif