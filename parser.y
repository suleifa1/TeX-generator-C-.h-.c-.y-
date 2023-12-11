%{ 
	#include <stdio.h> 
	#include <string.h>
	#include <stdlib.h>
	#include "parserfuncs.h"
	#include "text.h"
	#include "func_param.h"
	#include "comment_block.h"

	/**
	 * Comment block variable
	 * 
	 * It's used for each special comment block
	 * 
	 * @version 2.0.0
	 * @author Faiz Suleimanov
	 */
	comment_t comment_block;

	/**
	 * List of all files to parse
	 * @version 2.0.0
	 * @author Faiz Suleimanov
	 */
	list_t* includes;

	/**
	 * Current directory variable
	 * @version 2.0.0
	 * @author Faiz Suleimanov
	 */
	char* current_directory;

	/**
	 * Error code variable
	 * @version 2.0.0
	 * @author Faiz Suleimanov
	 */
	int error_code;
	
%} 

%code requires {
	#include "list.h"
}


%token INFO_BEGIN INFO_END
%token LINE_BREAK

%union {
	int intval;
	char* str;
	list_t* list;
}

%token <str> INFO_TEXT TAG_BRIEF TAG_PARAM_NAME PARAM_DESC TAG_AUTHOR TAG_VERSION
%token <str> TAG_RETURN TAG_RETURN_DESC MODULE_TOKEN FUNCTION VAR STRUCT

%type <str> brief
%type <list> details
%type <intval> s
%type <comment> header


%%

start: s {
			if ($1 != 0) break;
			put_in_tex("Error: No useful information\n\\\\");
		}
	;

s:  /* empty */ { $$ = 0; }
	| s MODULE_TOKEN {
			char* s;
			printf("parser: ADD MODULE %s\n", $2);
			/* *.h files */
			s = make_full_path(current_directory, $2);
			if (list_add_unique(includes, s, (bool(*)(void*,void*))text_equal_func)) {
				s = text_copy(s);
			}

			/* *.c files */
			s[strlen(s)-1] = 'c'; 
			if (!list_add_unique(includes, s, (bool(*)(void*,void*))text_equal_func)) {
				free(s);
			}

			$$ = $1;
		}
	| s comment FUNCTION {
			process_function(&comment_block, yyout, $3);
			free($3);
			$$ = $$ + 1;
		}
	| s comment VAR {
			process_variable(&comment_block, yyout, $3);
			free($3);
			$$ = $$ + 1;
		}
	| s comment STRUCT {
			process_struct(&comment_block, yyout, $3);
			free($3);
			$$ = $$ + 1;
		}
	;

comment: INFO_BEGIN { clear_comment_block(&comment_block); }
	inf INFO_END
	;


inf: space_or_empty /* empty */
	| space_or_empty header space_or_empty
	| space_or_empty header space_or_empty tags
	| space_or_empty tags 
	;

header: details {
			comment_block.details = $1;
		}
	| brief space details {
			comment_block.brief = $1;
			comment_block.details = $3;
		}
	| brief details{
		comment_block.brief = $1;
		comment_block.details = $2;

	}
	;

tags: TAG space_or_empty
	| tags TAG space_or_empty
	;

space_or_empty: /* empty */
	| space
	;

space: LINE_BREAK
	| space LINE_BREAK
	;

brief: INFO_TEXT
	| TAG_BRIEF
	;

details: INFO_TEXT {
			$$ = list_new($1);
		}
	| details INFO_TEXT {
			list_add_object_back($1, $2);
			$$ = $1;  
		}
	;

TAG: TAG_BRIEF { 
			if (comment_block.brief != NULL) break;
			comment_block.brief = $1;
		}
	| TAG_PARAM_NAME PARAM_DESC {
			list_t* p;
			func_param_t* param;

			param = malloc(sizeof(func_param_t));
			
			param->signature = $1;
			param->description = $2;

			p = comment_block.params;
			if (p == NULL) {
				comment_block.params = list_new((void*)param);		
			} else {
				list_add_object_back(p, (void*)param);
			}
		}
	| TAG_RETURN TAG_RETURN_DESC  {
			if (comment_block.return_tag == NULL) {
				return_t* p = malloc(sizeof(return_t));
				comment_block.return_tag = p;
				p->type = $1;
				p->description = $2;
			}
		}
	| TAG_AUTHOR { 
			comment_block.author_tag = $1;
		}
	| TAG_VERSION { 
			comment_block.version_tag = $1;
		}
	;

%%

/**
 * Main function entry point for the program.
 *
 * This function serves as the main entry point of the program, handling command-line arguments
 * to process input files and generate LaTeX documentation. It expects a single source file name,
 * optionally followed by a destination file name. If no destination is provided, it appends '-doc.tex'
 * to the source file name to create one. The function initializes document structure for LaTeX,
 * processes the source file(s), and then finalizes the LaTeX document. 
 *
 * @param int argc Count of parameters passed to the program on the command line.
 * @param char* argv[] Array of strings corresponding to the individual parameters passed.
 * @return int Value returned to the operating system upon program termination.
 * @author Copyright(c) Faiz Suleimanov
 * @version 1.0.0
 */
int main(int argc, char **argv) { 
	++argv, --argc;  /* skip over program name */
	FILE* f; 
	char* filename;


	if (argc == 1) {
		/* make output filename */
		char* ptr = strrchr(argv[0], '.');
		int len = (int)(ptr - argv[0]);
		const char* ext = "-doc.tex";
		ptr = malloc(len + strlen(ext) + 1);
		strncpy(ptr, argv[0], len);
		strcpy(ptr + len, ext);
		yyout = fopen(ptr, "w");
		free(ptr);
	} else if (argc == 2) {
		yyout = fopen(argv[1], "w");
	} else {
		/* error */
		printf("Error. Format: ./ccdoc.exe {source file .h|.c|.y} {{destination file .tex}}\n");
		return 1;
	}

	f = fopen(argv[0], "r");

	if (f == NULL) {
		printf("I/O error: Can't open source file\n");
		if (yyout) fclose(yyout);
		return 2;
	}

	if (yyout == NULL) {
		printf("I/O error: Can't open destination file\n");
		if (f) fclose(f);
		return 2;
	}
	fclose(f);

	current_directory = NULL;
	includes = NULL;
	error_code = 0;

	fputs("\\documentclass{article}\n" 
		"\\usepackage[czech]{babel}\n" 
		"\\selectlanguage{czech}\n" 
		"\\catcode`\\_=12\n\n" 
		"\\title{TITLE}\n" 
		"\\author{AUTHOR}\n" 
		"\\date{\\today}\n\n" 
		"\\begin{document}\n" 
		"\\pagenumbering{roman}\n" 
		"\\begin{titlepage}\n" 
		"\\maketitle\n" 
		"\\tableofcontents\n" 
		"\\end{titlepage}\n" 
		"\\pagenumbering{arabic}\n" 
		"\\section{Programátorská dokumentace}\n", yyout);

	/* first source file */
	filename = text_copy(argv[0]);
	includes = list_new(filename);
	list_apply_foreach(includes, (void(*)(void*))module_parse_file);
	list_free(includes, free);
	includes = NULL;

	fputs("\\end{document}", yyout);
	fclose(yyout);

	yylex_destroy();
	if(!error_code) return error_code;
	return 0;
} 
  

int yyerror(char* message) { 
	fprintf(stderr, "\nError: It's invalid, line %d: %s\n", yylineno, message); 
	fprintf(stderr, "text: %s\n\n", yytext);
	return 0;
}


