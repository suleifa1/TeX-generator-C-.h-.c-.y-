#include <stdio.h> 
#include <string.h>
#include <stdlib.h>
#include "parserfuncs.h"
#include "text.h"
#include "func_param.h"
#include "comment_block.h"

extern comment_t comment_block;
extern list_t* includes;
extern char* current_directory;
extern int error_code;
extern int yyparse (void);


void put_in_tex(char* text) {
	fprintf(yyout, "%s\n", text);
}



void module_parse_file(char* filename) {
	printf("Parsing: %s\n", filename); 

	/* set current directory */
	current_directory = text_current_directory(filename);

	yyin = fopen(filename, "r");
	if (yyin == NULL)  {
		/* error */
		free(current_directory);
		printf("Parsing error: can't open file %s\n\n", filename);
		return;
	}

	fprintf(yyout, "\\subsection{Modul \\texttt{%s}}\n", filename);

	if(!yyparse()) {
		printf("Parsing complete %s\n\n", filename); 
	} else {
		printf("Parsing failed %s\n\n", filename); 
		error_code = 3;
	}

	fclose(yyin);

	if (current_directory != NULL) {
		free(current_directory);
	}
}


void process_variable(comment_t* block, FILE* outfile, char* variable) {
	fprintf(outfile, "\\subsubsection {Proměnná \\texttt{%s}}\n", variable);

	if (block->brief) {
	fprintf(outfile, "\\par\\noindent\n\\textbf {Brief:} %s\\\n",
		comment_block.brief);
	fputs("\\\\\n", outfile); 
	free(block->brief);
	}

	/* process params */
	if (block->params) {
		list_free(block->params, (void(*)(void*))func_param_free);
		block->params = NULL;
	}

	/* process details (TEXT) */
	if (block->details) {
		fputs("\\par\\noindent\n\\textbf{Popis:} ", outfile);
		list_apply_foreach(block->details, (void(*)(void*))put_in_tex);
		list_free(block->details, free);
		fputs("\\\\\n", outfile);
		block->details = NULL;
	}



	if (block->author_tag) {
		fprintf(outfile, "\\par\\noindent\n\\textbf{Autor:} %s\\\\\n",
			block->author_tag);
		free(block->author_tag);
	}

	if (block->version_tag) {
		fprintf(outfile, "\\par\\noindent\n\\textbf{Verze:} %s\\\\\n",
			block->version_tag);
		free(block->version_tag);
	}

	if (block->return_tag) {
		return_t* p = block->return_tag;
		free(p->type);
		free(p->description);
		free(p);
	}
}


void process_struct(comment_t* block, FILE* outfile, char* struc) {
	fprintf(outfile, "\\subsubsection {Struktura \\texttt{%s}}\n", struc);

	if (block->brief) {
		fprintf(outfile, "\\par\\noindent\n\\textbf {Brief:} %s\\\n",
		comment_block.brief); 
	fputs("\\\\\n", outfile);	
	free(block->brief);
	}

	/* process details (TEXT) */
	if (block->details) {
		fputs("\\par\\noindent\n\\textbf{Popis:} ", outfile);
		list_apply_foreach(block->details, (void(*)(void*))put_in_tex);
		list_free(block->details, free);
		fputs("\\\\\n", outfile);
		block->details = NULL;
	}

	/* process params */
	if (block->params) {
		list_free(block->params, (void(*)(void*))func_param_free);
		block->params = NULL;
	}

	if (block->return_tag) {
		return_t* p = block->return_tag;
		free(p->type);
		free(p->description);
		free(p);
	}

	


	if (block->author_tag) {
		fprintf(outfile, "\\par\\noindent\n\\textbf{Autor:} %s\\\\\n",
			block->author_tag);
		free(block->author_tag);
	}

	if (block->version_tag) {
		fprintf(outfile, "\\par\\noindent\n\\textbf{Verze:} %s\\\\\n",
			block->version_tag);
		free(block->version_tag);
	}
}


void process_function(comment_t* block, FILE* outfile, char* func) {

	/* print in outfile */
	char c;
	char* ptr;
	int n;
	const int LIMIT = 40;


	n = strlen(func);
	ptr = func;
	fprintf(outfile, "\\subsubsection {Funkce \\texttt{");
	
	while (n > LIMIT) {
		ptr += LIMIT;
		c = *ptr;
		*ptr = '\0';
		fprintf(outfile, "%s\\newline ", ptr-LIMIT);
		*ptr = c;
		n -= LIMIT;
	}
	fprintf(outfile, "%s}}", ptr);


	if (block->brief) {
		fprintf(outfile, "\\par\\noindent\n\\textbf {Brief:} %s\\\\\n",
		comment_block.brief); 
	fputs("\\\\\n", outfile);
	free(block->brief);
	}

	/* process params */
	if (block->params) {
		fputs("\\textbf{Argumenty:}\n", outfile);
		list_apply_foreach(block->params, (void(*)(void*))process_param);
		fputs("\\\\\n", outfile);
		list_free(block->params, (void(*)(void*))func_param_free);
		block->params = NULL;
	}

	if (block->return_tag) {
		return_t* p = block->return_tag;
		fprintf(outfile, "\\par\\noindent\n\\textbf{Návratová hodnota:} \\verb\"%s\" -- %s \\\\\n",
			p->type, p->description);
		free(p->type);
		free(p->description);
		free(p);
	}

	/* process details (TEXT) */
	if (block->details) {
		fputs("\\par\\noindent\n\\textbf{Popis:} ", outfile);
		list_apply_foreach(block->details, (void(*)(void*))put_in_tex);
		list_free(block->details, free);
		fputs("\\\\\n", outfile);
		block->details = NULL;
	}



	if (block->author_tag) {
		fprintf(outfile, "\\par\\noindent\n\\textbf{Autor:} %s\\\\\n",
			block->author_tag);
		free(block->author_tag);
	}

	if (block->version_tag) {
		fprintf(outfile, "\\par\\noindent\n\\textbf{Verze:} %s\\\\\n",
			block->version_tag);
		free(block->version_tag);
	}
}


void clear_comment_block(comment_t* block) {
	block->brief = NULL;
	block->author_tag = NULL;
	block->version_tag = NULL;
	block->return_tag = NULL;
	block->params = NULL;
	block->details = NULL;
}


void process_param(func_param_t* param) {
	fprintf(yyout, "\\verb\"%s\" -- %s\n", param->signature, param->description);
}


char* make_full_path(char* current_directory, char* filename) {
	char* s;
	if (filename == NULL) return NULL;

	if (current_directory == NULL) {
		s = text_copy(filename);
	} else {
		s = malloc(strlen(filename) + strlen(current_directory) + 1);
		strcpy(s, current_directory);
		strcat(s, filename);
	}
	return s;
}


char* text_before_last_symbol(char* text, char ch) {
	char* ptr;
	int len;

	ptr = strrchr(text, ch);

	if (ptr == NULL) return ptr;
	len = (int)(ptr - text) + 1; /* including ch */
	ptr = malloc(len + 1); /* including 0 */
	strncpy(ptr, text, len);
	ptr[len] = '\0';
	return ptr;
}




char* text_current_directory(char* filepath) {
	return text_before_last_symbol(filepath, (DIRECTORY_SEPARATOR));
}
