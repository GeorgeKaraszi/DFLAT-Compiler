%option noyywrap

%{
	#include "dflat.h"

%}

DIGIT [-]?[0-9]+
FLOAT [-]?[0-9]+\.[0-9]+
ID    [_a-zA-Z][_a-zA-Z0-9]*

%%

[ \t\n\r]+ 	;
{DIGIT}					{
							yylval.value.x.ival = atoi(yytext); 
							yylval.value.valtype = INT_T;
							return INTEGER;
						}
{FLOAT} 				{
							yylval.value.x.fval = atof(yytext); 
							yylval.value.valtype = FLOAT_T;
							return FLOAT;
					  	}
"?"						{return SEMICOLON;}
"("						{return LBRACE;}
")"						{return RBRACE;}
"="						{return EQUALS;}
"{"						{return LCURB;}
"}"						{return RCURB;}
","						{return COMMA;}
">="					{return GE_TOK;}
"<="					{return LE_TOK;}
"!="					{return NE_TOK;}
"=="					{return ET_TOK;}
"Playlist"				{return PLAYLIST;}
"Pick"					{return IF;}
"Other"					{return ELSE;}
"Call"					{return CALL;}
"&"						{return IF_END;}
"#"						{return FUNC_END;}
(?i:HORN)				{return PRINT_TOKEN;}
{ID}					{
							yylval.string = strdup(yytext);

							return ID;
						}
\"(\\.|[^\"])*\" 		{ 	/* This one identifies strings in qoutes eg: "HELLO WORLD"" */
							yylval.value.valtype = STR_T;

							/* Remove Qoutes (") from the ends of the string*/
							yylval.value.x.sval  = strndup(yytext, strlen(yytext)-1);
							++yylval.value.x.sval;

							return STRING;
						}
.						{return yytext[0];}

%%