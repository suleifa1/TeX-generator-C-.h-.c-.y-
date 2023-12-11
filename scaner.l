%{
#include <stdio.h>
#include "y.tab.h"
#include "text.h"

int fileno(FILE *stream);

%}

%option yylineno

%s COMMENT
%s AFTER_COMMENT
%s INCLUDE
%s PARAM
%s RETURN


LETTER [a-zA-Z]
%%

"/**"	|
"/*!"	{
	BEGIN(COMMENT);
	printf("COMMENT_BEGIN\n");
	return INFO_BEGIN;
}

"#include"[ \t]+["] {
	printf("INCLUDE\n");
	BEGIN(INCLUDE);
}

<INCLUDE>{

	[^"]+ {
		BEGIN(INITIAL);
		printf("INCLUDE NAME\n");
		yylval.str = yytext;
		return MODULE_TOKEN;
	}

}

<COMMENT>{

	"@brief".* { 
		printf("TAG BRIEF\n");
		yylval.str = text_copy(yytext + 7);
		return TAG_BRIEF;
	}

	"@details".* { 
		printf("TAG DETAILS\n");
		yylval.str = text_copy(yytext + 9);
		return INFO_TEXT;
	}

	"@param"[^A-Z]+ { 
		printf("TAG PARAM\n");
		BEGIN(PARAM);
		yytext[yyleng-1] = '\0'; /* remove space in the end */
		yylval.str = text_copy(yytext + 7);
		return TAG_PARAM_NAME;
	}

	"@author".* { 
		printf("TAG AUTOR\n");
		yylval.str = text_copy(yytext + 8);
		return TAG_AUTHOR;
	}

	"@version".* { 
		printf("TAG VERSION\n");
		yylval.str = text_copy(yytext + 9);
		return TAG_VERSION;
	}

	"@return"[^A-Z]+ { 
		printf("TAG RETuRN\n");
		BEGIN(RETURN);
		yytext[yyleng-1] = '\0'; /* remove space in the end */
		yylval.str = text_copy(yytext + 8);
		return TAG_RETURN;
	}

	{LETTER}.* {
		printf("TEXT\n");
		yylval.str = text_copy(yytext);
		return INFO_TEXT;
	}

	"*/" { 
		printf("END_COMMENT\n");
		BEGIN(AFTER_COMMENT);
		return INFO_END;
	}

	^(" "|"\t")*"*"[ \t]*\n {
		printf("LINE_BREAK\n");
		return LINE_BREAK;
	}

}

<AFTER_COMMENT>{

	[[:alpha:]_]+[ \t]*"(".*")" {
		printf("AFTER_COMMENT 1\n");
		BEGIN(INITIAL);
		yylval.str = text_copy(yytext);
		return FUNCTION;
	}

	[[:alpha:]_]+[\t ]*[=;] {
		printf("AFTER_COMMENT 2\n");
		BEGIN(INITIAL);
		yytext[yyleng-1] = '\0'; /* remove last symbol */
		yylval.str = text_copy(yytext);
		return VAR;
	}

	"struct"[ \t]*[[:alpha:]_]* {
		printf("AFTER_COMMENT 3 struct\n");
		BEGIN(INITIAL);
		yylval.str = text_copy(yytext+7);
		return STRUCT;
	}

}

<PARAM>{
	.* {
		printf("INSIDE PARAM\n");
		BEGIN(COMMENT);
		yylval.str = text_copy(yytext);
		return PARAM_DESC;
	}
}

<RETURN>{
	.* {
		printf("INSIDE RETURN\n");
		BEGIN(COMMENT);
		yylval.str = text_copy(yytext);
		return TAG_RETURN_DESC;
	}
}

.|"\n" ;

%%

int yywrap() {
	return 1;
}

