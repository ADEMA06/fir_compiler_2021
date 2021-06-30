%option c++ prefix="fir_scanner_" outfile="fir_scanner.cpp"
%option stack noyywrap yylineno 8bit debug
%{
// make relevant includes before including the parser's tab file
#include <string>
#include <cdk/ast/sequence_node.h>
#include <cdk/ast/expression_node.h>
#include <cdk/ast/lvalue_node.h>
#include "fir_parser.tab.h"

// don't change this
#define yyerror LexerError
%}
%x X_STRING X_COMMENT X_TILDE X_CLEARSTR
%%


"void"                  return tTYPE_VOID;
"int"                   return tTYPE_INT;
"float"                 return tTYPE_REAL;
"string"                return tTYPE_STRING;
"null"                  return tNULL;
"sizeof"                return tSIZEOF;

"while"                 return tWHILE;
"do"                    return tDO;
"finally"               return tFINALLY;
"leave"                 return tLEAVE;
"restart"               return tRESTART;
"return"                return tRETURN;
"if"                    return tIF;
"else"                  return tELSE;
"then"                  return tTHEN;
"write"                 return tWRITE;
"writeln"               return tWRITELN;

">>"					return tEPILOGUE;
"->"					return tDEFAULTRETVAL;



"!!".*$                /* match the whole line (it is ignored) */

"(*"                   yy_push_state(X_COMMENT);
<X_COMMENT>"*)"        yy_pop_state();
<X_COMMENT>.|"\n"      ;  /* ignore inside comments */



">="                   return tGE;
"<="                   return tLE;
"=="                   return tEQ;
"!="                   return tNE;

"&&"                   return tAND;
"||"                   return tOR;

[A-Za-z][A-Za-z0-9_]*  yylval.s = new std::string(yytext); return tID;

\'                     yy_push_state(X_STRING); yylval.s = new std::string("");
<X_STRING>\'           yy_pop_state(); return tSTRING;
<X_STRING>~						 yy_pop_state(); yy_push_state(X_TILDE);
<X_STRING>.            *yylval.s += yytext;
<X_STRING>\n           yyerror("Newline in string");

<X_TILDE>n					*yylval.s += "\n"; yy_pop_state(); yy_push_state(X_STRING);
<X_TILDE>r					*yylval.s += "\r"; yy_pop_state(); yy_push_state(X_STRING);
<X_TILDE>t					*yylval.s += "\t"; yy_pop_state(); yy_push_state(X_STRING);
<X_TILDE>a					*yylval.s += "\a"; yy_pop_state(); yy_push_state(X_STRING);
<X_TILDE>\'					*yylval.s += "\'"; yy_pop_state(); yy_push_state(X_STRING);
<X_TILDE>~					*yylval.s += "~"; yy_pop_state(); yy_push_state(X_STRING);
<X_TILDE>"0"						yy_pop_state(); yy_push_state(X_CLEARSTR);
<X_TILDE>"00"						yy_pop_state(); yy_push_state(X_CLEARSTR);
<X_TILDE>[A-Fa-f0-9][A-Fa-f0-9]? *yylval.s +=std::string(1,strtol(yytext, nullptr, 16)); yy_pop_state(); yy_push_state(X_STRING);
<X_TILDE>.					*yylval.s += yytext; yy_pop_state(); yy_push_state(X_STRING);

<X_CLEARSTR>\'			yy_pop_state(); return tSTRING ;
<X_CLEARSTR>.				;

[1-9][0-9]*            yylval.i = strtol(yytext, nullptr, 10);  if(errno == ERANGE) yyerror("Overflow..."); return tINTEGER;
"0"[0-7]*              yylval.i = strtol(yytext, nullptr, 8); if(errno == ERANGE) yyerror("Overflow..."); return tINTEGER;
"0"[0-9a-zA-Z]+        yyerror("Invalid octal");

([0-9]*\.[0-9]+|[0-9]+\.[0-9]*)([eE][-+]?[0-9]+)? yylval.d = strtod(yytext, NULL); if(errno == ERANGE) yyerror("Overflow..."); return tREAL;
[0-9]+[eE][-+]?[0-9]+ 														yylval.d = strtod(yytext, NULL); if(errno == ERANGE) yyerror("Overflow..."); return tREAL;

[-,()<>=+*/%;{}.?@~\[\]]     return *yytext;

[ \t\n]+               ; /* ignore whitespace */

.                      yyerror("Unknown character");

%%
