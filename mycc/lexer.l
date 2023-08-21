%{
#include "ast.h" // it shouldn't have to add this head file, normally???
#include "parser.hpp"
#include <stdio.h>
#include <string.h>
using namespace std;

%}

SYS_CON    "false"|"maxint"|"true"
SYS_FUNCT  "abs"|"chr"|"odd"|"ord"|"pred"|"sqr"|"sqrt"|"succ"
SYS_PROC   "write"|"writeln"
SYS_TYPE   "boolean"|"char"|"integer"|"real"
READ       "read"
INTEGER    [0-9]+
REAL       [0-9]+\.[0-9]+
CHAR       \'.\'
ID         [a-zA-Z][_a-zA-Z0-9]*

%%

"("                                                     { return LP; }
")"                                                     { return RP; }
"["                                                     { return LB; }
"]"                                                     { return RB; }
"."                                                     { return DOT; }
","                                                     { return COMMA; }
":"                                                     { return COLON; }
"*"                                                     { return MUL; }
"/"                                                     { return DIV; }
"<>"                                                    { return UNEQUAL; }
"NOT"|"not"                                             { return NOT; }
"+"                                                     { return PLUS; }
"-"                                                     { return MINUS; }
">="                                                    { return GE; }
">"                                                     { return GT; }
"<="                                                    { return LE; }
"<"                                                     { return LT; }
"="                                                     { return EQUAL; }
":="                                                    { return ASSIGN; }
"MOD"|"mod"                                             { return MOD; }
".."                                                    { return DOTDOT; }
";"                                                     { return SEMI; }
"and"                                                   { return AND; }
"array"                                                 { return ARRAY; }
"begin"                                                 { return lBEGIN; }
"case"                                                  { return CASE; }
"const"                                                 { return CONST; }
"div"                                                   { return DIV; }
"do"                                                    { return DO; }
"downto"                                                { return DOWNTO; }
"else"                                                  { return ELSE; }
"end"                                                   { return END; }
"for"                                                   { return FOR; }
"function"                                              { return FUNCTION; }
"goto"                                                  { return GOTO; }
"if"                                                    { return IF; }
"of"                                                    { return OF; }
"or"                                                    { return OR; }
"packed"                                                { return PACKED; }
"procedure"                                             { return PROCEDURE; }
"program"                                               { return PROGRAM; }
"record"                                                { return RECORD; }
"repeat"                                                { return REPEAT; }
"then"                                                  { return THEN; }
"to"                                                    { return TO; }
"type"                                                  { return TYPE; }
"until"                                                 { return UNTIL; }
"var"                                                   { return VAR; }
"while"                                                 { return WHILE; }
{SYS_CON}                   { yylval.sval=new string(yytext,yyleng); return SYS_TYPE; }
{SYS_FUNCT}                 { yylval.sval=new string(yytext,yyleng); return SYS_FUNCT; }
{SYS_PROC}                  { yylval.sval=new string(yytext,yyleng); return SYS_PROC; }
{SYS_TYPE}                  { yylval.sval=new string(yytext,yyleng); return SYS_TYPE; }
{READ}                      { yylval.sval=new string(yytext,yyleng); return READ; }
{INTEGER}                   { sscanf(yytext,"%d",&yylval.ival);      return INTEGER; }
{REAL}                      { sscanf(yytext,"%lf",&yylval.dval);     return REAL; }
{CHAR}                      { yylval.cval=yytext[1];                 return CHAR; }
{ID}                        { yylval.sval=new string(yytext,yyleng);    return ID; }
[ \t\n]
"{".*"}" 

%%

int yywrap() { return 1; }
