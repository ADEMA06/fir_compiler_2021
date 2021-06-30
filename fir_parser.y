%{
//-- don't change *any* of these: if you do, you'll break the compiler.
#include <algorithm>
#include <memory>
#include <cstring>
#include <cdk/compiler.h>
#include <cdk/types/types.h>
#include "ast/all.h"
#define LINE                         compiler->scanner()->lineno()
#define yylex()                      compiler->scanner()->scan()
#define yyerror(compiler, s)         compiler->scanner()->error(s)
//-- don't change *any* of these --- END!
%}

%parse-param {std::shared_ptr<cdk::compiler> compiler}

%union {
  //--- don't change *any* of these: if you do, you'll break the compiler.
  YYSTYPE() : type(cdk::primitive_type::create(0, cdk::TYPE_VOID)) {}
  ~YYSTYPE() {}
  YYSTYPE(const YYSTYPE &other) { *this = other; }
  YYSTYPE& operator=(const YYSTYPE &other) { type = other.type; return *this; }

  std::shared_ptr<cdk::basic_type> type;        /* expression type */
  //-- don't change *any* of these --- END!

  int                   i;	/* integer value */
  double                d;  /* integer value */
  std::string          *s;	/* symbol name or string literal */
  cdk::basic_node      *node;	/* node pointer */
  cdk::sequence_node   *sequence;
  cdk::expression_node *expression; /* expression nodes */
  cdk::lvalue_node     *lvalue;

  fir::block_node               *block;
  fir::prologue_node               *prologue;
  fir::function_body_node       *function_body;
  std::vector<std::string> *ids;
};


%token tAND tOR tNE tLE tGE tSIZEOF
%token tWRITE tWRITELN
%token tTYPE_STRING tTYPE_INT tTYPE_REAL
%token tIF tTHEN tELSE
%token tWHILE tDO tFINALLY
%token tLEAVE tRESTART tRETURN
%token tTYPE_VOID tNULL
%token tEPILOGUE
%token tDEFAULTRETVAL
%token tNULLPTR

%token <i> tINTEGER
%token <d> tREAL
%token <s> tID tSTRING

%nonassoc tELSE
%right '='
%left tOR
%left tAND
%right '~'
%left tNE tEQ
%left '<' tLE tGE '>'
%left '+' '-'
%left '*' '/' '%'
%nonassoc tUNARY

%type <node>        declaration vardec fundec fundef argdec instruction
%type <sequence>    file declarations opt_declarations argdecs instructions opt_instructions expressions opt_expressions
%type <expression>  opt_initializer expression literal defaultretval
%type <lvalue>      lvalue
%type <block>       block body epilogue
%type <prologue>    prologue
%type <function_body>       funbody
%type <type>        data_type
%type <s>           string

%{
//-- The rules below will be included in yyparse, the main parsing function.
%}
%%

file            : declarations  { compiler->ast($$ = $1); }
                ;

declaration     : vardec ';'      { $$ = $1; }
                | fundec         { $$ = $1; }
                | fundef         { $$ = $1; }
                ;

declarations    : declaration               { $$ = new cdk::sequence_node(LINE, $1);     }
                | declarations declaration  { $$ = new cdk::sequence_node(LINE, $2, $1); }
                ;

opt_declarations  : /* empty */  { $$ = new cdk::sequence_node(LINE); }
                  | declarations { $$ = $1; }
                  ;

vardec          : data_type '*' tID opt_initializer              { $$ = new fir::variable_declaration_node(LINE, '*',  $1, *$3, $4); }
                | data_type '?' tID                              { $$ = new fir::variable_declaration_node(LINE, '?',  $1, *$3, nullptr); }
                | data_type     tID opt_initializer              { $$ = new fir::variable_declaration_node(LINE, '/',  $1, *$2, $3); }
                ;

opt_initializer : /* empty */           { $$ = nullptr; /* must be nullptr, not NIL */ }
                | '=' expression        { $$ = $2; }
                ;

fundec   :  data_type      tID '(' argdecs ')' { $$ = new fir::function_declaration_node(LINE, '/', $1, *$2, $4); }
         |  data_type  '?' tID '(' argdecs ')' { $$ = new fir::function_declaration_node(LINE, '?', $1, *$3, $5); }
         |  data_type  '*' tID '(' argdecs ')' { $$ = new fir::function_declaration_node(LINE, '*', $1, *$3, $5); }
         |  tTYPE_VOID     tID '(' argdecs ')' { $$ = new fir::function_declaration_node(LINE, '/', *$2, $4); }
         |  tTYPE_VOID '?' tID '(' argdecs ')' { $$ = new fir::function_declaration_node(LINE, '?', *$3, $5); }
         |  tTYPE_VOID '*' tID '(' argdecs ')' { $$ = new fir::function_declaration_node(LINE, '*', *$3, $5); }
         ;

fundef   : data_type       tID '(' argdecs ')' funbody                        { $$ = new fir::function_definition_node(LINE, '/', $1, *$2, $4, $6); }
         | data_type  '*'  tID '(' argdecs ')' funbody                        { $$ = new fir::function_definition_node(LINE, '*', $1, *$3, $5, $7); }
         | tTYPE_VOID      tID '(' argdecs ')' funbody                        { $$ = new fir::function_definition_node(LINE, '/', *$2, $4, $6); }
         | tTYPE_VOID '*'  tID '(' argdecs ')' funbody                        { $$ = new fir::function_definition_node(LINE, '*', *$3, $5, $7); }
         | data_type       tID '(' argdecs ')' defaultretval funbody          { $$ = new fir::function_definition_node(LINE, '/', $1, *$2, $4, $7, $6); }
         | data_type  '*'  tID '(' argdecs ')' defaultretval funbody          { $$ = new fir::function_definition_node(LINE, '*', $1, *$3, $5, $8, $7); }
         | tTYPE_VOID      tID '(' argdecs ')' defaultretval funbody          { yyerror(compiler, "error: can't have default return value if function is type void"); }
         | tTYPE_VOID '*'  tID '(' argdecs ')' defaultretval funbody          { yyerror(compiler, "error: can't have default return value if function is type void"); }
         ; // might need to check if the body isn't totally empty

argdecs   : /* empty */         { $$ = new cdk::sequence_node(LINE);  }
          |             argdec  { $$ = new cdk::sequence_node(LINE, $1);     }
          | argdecs ',' argdec  { $$ = new cdk::sequence_node(LINE, $3, $1); }
          ;

argdec    : data_type tID { $$ = new fir::variable_declaration_node(LINE, '/', $1, *$2, nullptr); }
          ;

defaultretval : tDEFAULTRETVAL literal    { $$ = $2; }
              | tDEFAULTRETVAL string        { $$ = new cdk::string_node(LINE, $2); }
              ;

prologue  : '@' '{' opt_declarations opt_instructions '}'                     { $$ = new fir::prologue_node(LINE, $3, $4); }
          ;

body      : block                         { $$ = $1; }
          ;

epilogue  : tEPILOGUE block               { $$ = $2; }
          ;

funbody   : prologue                      { $$ = new fir::function_body_node(LINE, $1, new fir::block_node(LINE, new cdk::sequence_node(LINE), new cdk::sequence_node(LINE)), new fir::block_node(LINE, new cdk::sequence_node(LINE), new cdk::sequence_node(LINE))); }
            |          body                 { $$ = new fir::function_body_node(LINE, new fir::prologue_node(LINE, new cdk::sequence_node(LINE), new cdk::sequence_node(LINE)), $1, new fir::block_node(LINE, new cdk::sequence_node(LINE), new cdk::sequence_node(LINE))); }
            |               epilogue        { $$ = new fir::function_body_node(LINE, new fir::prologue_node(LINE, new cdk::sequence_node(LINE), new cdk::sequence_node(LINE)), new fir::block_node(LINE, new cdk::sequence_node(LINE), new cdk::sequence_node(LINE)),$1); }
          | prologue body epilogue        { $$ = new fir::function_body_node(LINE, $1, $2, $3); }
          | prologue body                 { $$ = new fir::function_body_node(LINE, $1, $2, new fir::block_node(LINE, new cdk::sequence_node(LINE), new cdk::sequence_node(LINE))); }
          | prologue      epilogue        { $$ = new fir::function_body_node(LINE, $1, new fir::block_node(LINE, new cdk::sequence_node(LINE), new cdk::sequence_node(LINE)), $2); }
          |          body epilogue        { $$ = new fir::function_body_node(LINE, new fir::prologue_node(LINE, new cdk::sequence_node(LINE), new cdk::sequence_node(LINE)), $1, $2); }
          ;


instruction     : tIF expression tTHEN instruction                                              { $$ = new fir::if_node(LINE, $2, $4); }
                | tIF expression tTHEN instruction tELSE instruction                            { $$ = new fir::if_else_node(LINE, $2, $4, $6); }
                | tWHILE expression tDO instruction tFINALLY instruction                        { $$ = new fir::while_node(LINE, $2, $4, $6); }
                | tWHILE expression tDO instruction                                             { $$ = new fir::while_node(LINE, $2, $4); }
                | expression ';'                                                                { $$ = new fir::evaluation_node(LINE, $1); }
                | tWRITE   expressions ';'                                                      { $$ = new fir::print_node(LINE, $2, false); }
                | tWRITELN expressions ';'                                                      { $$ = new fir::print_node(LINE, $2, true); }
                | tLEAVE tINTEGER ';'                                                           { $$ = new fir::leave_node(LINE, $2); }
                | tLEAVE ';'                                                                    { $$ = new fir::leave_node(LINE); }
                | tRESTART tINTEGER ';'                                                         { $$ = new fir::restart_node(LINE, $2); }
                | tRESTART ';'                                                                  { $$ = new fir::restart_node(LINE); }
                | tRETURN                                                                       { $$ = new fir::return_node(LINE); }
                | block                                                                         { $$ = $1; }
                ;

instructions    : instruction                { $$ = new cdk::sequence_node(LINE, $1);     }
                | instructions instruction   { $$ = new cdk::sequence_node(LINE, $2, $1); }
                ;

opt_instructions: /* empty */  { $$ = new cdk::sequence_node(LINE); }
                | instructions { $$ = $1; }
                ;


block     : '{' opt_declarations opt_instructions '}' { $$ = new fir::block_node(LINE, $2, $3);}
          ;

data_type     : tTYPE_STRING                     { $$ = cdk::primitive_type::create(4, cdk::TYPE_STRING);  }
              | tTYPE_INT                        { $$ = cdk::primitive_type::create(4, cdk::TYPE_INT);     }
              | tTYPE_REAL                       { $$ = cdk::primitive_type::create(8, cdk::TYPE_DOUBLE);  }
              | '<' data_type '>'                { $$ = cdk::reference_type::create(4, $2); }
              ;


literal : tINTEGER      { $$ = new cdk::integer_node(LINE, $1); }
        | tREAL         { $$ = new cdk::double_node(LINE, $1); }
        | tNULL         { $$ = new fir::nullptr_node(LINE); }
        ;

string          : tSTRING                       { $$ = $1; }
                | string tSTRING                { $$ = $1; $$->append(*$2); delete $2; }
                ;

expression      : literal                       { $$ = $1; }
                | string        { $$ = new cdk::string_node(LINE, $1); }
                | tNULLPTR                      { $$ = new fir::nullptr_node(LINE); }
                /* LEFT VALUES */
                | lvalue                        { $$ = new cdk::rvalue_node(LINE, $1); }
                /* ASSIGNMENTS */
                | lvalue '=' expression         { $$ = new cdk::assignment_node(LINE, $1, $3); }
                /* ARITHMETIC EXPRESSIONS */
                | expression '+' expression     { $$ = new cdk::add_node(LINE, $1, $3); }
                | expression '-' expression     { $$ = new cdk::sub_node(LINE, $1, $3); }
                | expression '*' expression     { $$ = new cdk::mul_node(LINE, $1, $3); }
                | expression '/' expression     { $$ = new cdk::div_node(LINE, $1, $3); }
                | expression '%' expression     { $$ = new cdk::mod_node(LINE, $1, $3); }
                /* LOGICAL EXPRESSIONS */
                | expression  '<' expression    { $$ = new cdk::lt_node(LINE, $1, $3); }
                | expression tLE  expression    { $$ = new cdk::le_node(LINE, $1, $3); }
                | expression tEQ  expression    { $$ = new cdk::eq_node(LINE, $1, $3); }
                | expression tGE  expression    { $$ = new cdk::ge_node(LINE, $1, $3); }
                | expression  '>' expression    { $$ = new cdk::gt_node(LINE, $1, $3); }
                | expression tNE  expression    { $$ = new cdk::ne_node(LINE, $1, $3); }
                /* LOGICAL EXPRESSIONS */
                | expression tAND  expression   { $$ = new cdk::and_node(LINE, $1, $3); }
                | expression tOR   expression   { $$ = new cdk::or_node (LINE, $1, $3); }
                /* UNARY EXPRESSION */
                | '-' expression %prec tUNARY   { $$ = new cdk::neg_node(LINE, $2); }
                | '+' expression %prec tUNARY   { $$ = new fir::identity_node(LINE, $2); }
                | '~' expression                { $$ = new cdk::not_node(LINE, $2); }
                /* OTHER EXPRESSION */
                | '@'                           { $$ = new fir::read_node(LINE); }
                /* OTHER EXPRESSION */
                | tID '(' opt_expressions ')'   { $$ = new fir::function_call_node(LINE, *$1, $3); delete $1; }
                | tSIZEOF '(' expression ')'    { $$ = new fir::sizeof_node(LINE, $3); }
                /* OTHER EXPRESSION */
                | '(' expression ')'            { $$ = $2; }
                | '[' expression ']'            { $$ = new fir::stack_alloc_node(LINE, $2); }
                | lvalue '?'                    { $$ = new fir::address_of_node(LINE, $1); }
                ;

expressions     : expression                     { $$ = new cdk::sequence_node(LINE, $1);     }
                | expressions ',' expression     { $$ = new cdk::sequence_node(LINE, $3, $1); }
                ;

opt_expressions : /* empty */         { $$ = new cdk::sequence_node(LINE); }
                | expressions         { $$ = $1; }
                ;

lvalue          : tID                                            { $$ = new cdk::variable_node(LINE, *$1); delete $1; }
                | lvalue             '[' expression ']'          { $$ = new fir::index_node(LINE, new cdk::rvalue_node(LINE, $1), $3); }
                | '(' expression ')' '[' expression ']'          { $$ = new fir::index_node(LINE, $2, $5); }
                | tID '(' opt_expressions ')' '[' expression ']' { $$ = new fir::index_node(LINE, new fir::function_call_node(LINE, *$1, $3), $6); }
                ;

%%
