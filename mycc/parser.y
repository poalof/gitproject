%{

#include "ast.h" // customized type defined in head file
#include <iostream>
#include <stdlib.h>
#include <string.h>

using namespace std;
Program* root;

int yyerror(const char* msg) {
    fprintf(stderr, "Error: %s\n", msg);
    return 0;
}
int yylex();

%}

// here is the type declaration
%union{
    int     ival; // built-in types
    double  dval;
    string*  sval;
    char    cval;
    bool    bval;
    Node *node; // custom types (ast node)
    Expr *expr;
    Stmt *stmt;
    Integer *integer;
    Real *real;
    Char *character;
    Boolean *boolean;
    ConstVal *constVal;
    Identifier *identifier;
    ConstDecl *constDecl;
    TypeDecl *typeDecl;
    VarDecl *varDecl;
    ASTType *type;
    EnumType *enumType;
    MyArrayType *arrayType;
    RecordType  *recordType;
    ConstRangeType *constRangeType;
    EnumSubrangeType *enumSubrangeType;
    FieldDecl *fieldDecl;
    FuncDecl *funcDecl;
    Parameter *parameter;
    Routine *routine;
    Program *program;
    AssignStmt *assignStmt;
    BinaryExpr *binaryExpr;
    ArrayRef *arrayAt;
    RecordRef *recordAt;
    FuncStmt *funcStmt;
    SysFunct *sysFunct;
    ProcStmt *procStmt;
    SysProc *sysProc;
    IfStmt *ifStmt;
    RepeatStmt *repeatStmt;
    WhileStmt *whileStmt;
    ForStmt *forStmt;
    CaseStmt *caseStmt;
    CaseExpr *caseExpr;
    GotoStmt *gotoStmt;
    vector<Expr*> *exprs;
    vector<Stmt*> *stmts;
    vector<ConstDecl*> *constDecls;
    vector<VarDecl*> *varDecls;
    vector<TypeDecl*> *typeDecls;
    vector<Identifier*> *names;
    vector<FieldDecl*> *fields;
    vector<Parameter*> *paras;
    vector<FuncDecl*> *routines;
    vector<Expr*> *args;
    vector<CaseExpr*> *caseExprs;
    CompoundStmt *compoundStmt;
}

%token  LP RP LB RB DOT COMMA COLON MUL DIV UNEQUAL NOT PLUS MINUS GE GT LE LT EQUAL ASSIGN MOD DOTDOT SEMI
        AND ARRAY lBEGIN CASE CONST DO DOWNTO ELSE END FOR FUNCTION GOTO IF OF OR PACKED PROCEDURE PROGRAM RECORD REPEAT THEN TO TYPE UNTIL VAR WHILE
%token<sval> ID SYS_CON SYS_FUNCT SYS_PROC SYS_TYPE READ
%token<ival> INTEGER
%token<dval> REAL
%token<cval> CHAR

%type<identifier>                       NAME
%type<program>                          program
%type<sval>                             program_head
%type<routine>                          routine routine_head sub_routine
%type<constDecls>                       const_part const_expr_list
%type<typeDecls>                     type_part type_decl_list
%type<typeDecl>                  type_definition
%type<varDecls>                      var_part var_decl_list
%type<varDecl>                   var_decl
%type<routines>                      routine_part
%type<constVal>                       const_value
%type<type>                             type_decl simple_type_decl array_type_decl record_type_decl
%type<names>                         name_list
%type<fields>                        field_decl_list
%type<fieldDecl>                 field_decl
%type<funcDecl>                  function_decl  function_head procedure_decl  procedure_head
%type<paras>                         parameters para_decl_list
%type<parameter>                        para_type_list var_para_list val_para_list
%type<stmt>                        stmt non_label_stmt else_clause
%type<assignStmt>                  assign_stmt
%type<stmt>                        proc_stmt     
%type<exprs>                   expression_list
%type<expr>                       expression expr term factor
%type<args>                         args_list 
%type<ifStmt>                      if_stmt 
%type<repeatStmt>                  repeat_stmt
%type<whileStmt>                   while_stmt
%type<forStmt>                     for_stmt
%type<bval>                             direction
%type<caseStmt>                    case_stmt
%type<caseExprs>                     case_expr_list
%type<caseExpr>                   case_expr
%type<gotoStmt>                    goto_stmt
%type<stmts>                    stmt_list
%type<compoundStmt>                routine_body compound_stmt

%start program
%%

program : program_head routine DOT { $$=new Program($1,$2); root=$$; }
        ;

program_head : PROGRAM ID SEMI { $$=$2; }
	     ;

routine : routine_head routine_body { $$=$1; $$->setRoutineBody($2); cout << "routine parsing done!" << endl; }
        ;

sub_routine : routine_head routine_body { $$=$1; $$->setRoutineBody($2); }
	    ;

routine_head : label_part const_part type_part var_part routine_part { $$=new Routine($2,$3,$4,$5); }
             ;

label_part : /* empty */
           ;

const_part : CONST const_expr_list { $$=$2; }
           | /* empty */           { $$=new vector<ConstDecl*>(); }
           ;

const_expr_list : const_expr_list NAME EQUAL const_value SEMI { $$=$1; $$->push_back(new ConstDecl($2,$4)); }
                | NAME EQUAL const_value SEMI                 { $$=new vector<ConstDecl*>(); $$->push_back(new ConstDecl($1,$3)); }
                ;

const_value : INTEGER { $$=new Integer($1); }
            | REAL { $$=new Real($1); }
            | CHAR { $$=new Char($1); }
            | SYS_CON { if(*$1=="maxint") $$=new Integer(INT_MAX);
                        else if(*$1=="true") $$=new Boolean(true);
                        else $$=new Boolean(false); }
            ;

type_part : TYPE type_decl_list { $$=$2; }
          | /* empty */ { $$=new vector<TypeDecl*>(); }
          ;

type_decl_list : type_decl_list type_definition { $$=$1; $$->push_back($2); }
               | type_definition { $$=new vector<TypeDecl*>(); $$->push_back($1); }
               ;

type_definition : NAME EQUAL type_decl SEMI { $$=new TypeDecl($1,$3); }
                ;

type_decl : simple_type_decl { $$=$1; }
          | array_type_decl { $$=$1; }
          | record_type_decl { $$=$1; }
          ;

simple_type_decl : SYS_TYPE { if(*$1=="boolean") $$=new ASTType(TYPE_BOOLEAN); 
                              else if(*$1=="char") $$=new ASTType(TYPE_CHAR);
                              else if(*$1=="integer") $$=new ASTType(TYPE_INTEGER); 
                              else if(*$1=="real") $$=new ASTType(TYPE_REAL); }
                 | NAME { $$=new ASTType($1); }
                 | LP name_list RP { $$=new ASTType(new EnumType($2)); }
                 | const_value DOTDOT const_value { $$=new ASTType(new ConstRangeType($1,$3)); }
                 | MINUS const_value DOTDOT const_value { $$=new ASTType(new ConstRangeType(-*$2,$4)); }
                 | MINUS const_value DOTDOT MINUS const_value  { $$=new ASTType(new ConstRangeType(-*$2,-*$5)); }
                 | NAME DOTDOT NAME  { $$=new ASTType(new EnumSubrangeType($1,$3)); }
                 ;

array_type_decl : ARRAY LB simple_type_decl RB OF type_decl { $$=new ASTType(new MyArrayType($3,$6)); }
                ;

record_type_decl : RECORD field_decl_list END { $$=new ASTType(new RecordType($2)); }
                 ;

field_decl_list : field_decl_list field_decl { $$=$1; $$->push_back($2); }
                | field_decl { $$=new vector<FieldDecl*>(); $$->push_back($1); }
                ;

field_decl : name_list COLON type_decl SEMI { $$=new FieldDecl($1,$3); }
           ;

name_list : name_list COMMA NAME { $$=$1; $$->push_back($3); }
          | NAME { $$=new vector<Identifier*>(); $$->push_back($1); }
          ;

var_part : VAR var_decl_list { $$=$2; }
         | /* empty */ { $$=new vector<VarDecl*>(); }
         ;

var_decl_list : var_decl_list var_decl { $$=$1; $$->push_back($2);}
              | var_decl { $$=new vector<VarDecl*>(); $$->push_back($1); }
              ;

var_decl : name_list COLON type_decl SEMI { $$=new VarDecl($1,$3); }
         ;

routine_part : routine_part function_decl { $$=$1; $$->push_back($2); }
	     | routine_part procedure_decl { $$=$1; $$->push_back($2); }
	     | function_decl { $$=new vector<FuncDecl*>(); $$->push_back($1); }
	     | procedure_decl { $$=new vector<FuncDecl*>(); $$->push_back($1); }
	     | { $$=new vector<FuncDecl*>(); }
	     ;

function_decl : function_head SEMI sub_routine SEMI { $$=$1; $$->setRoutine($3); }
	      ;

function_head : FUNCTION NAME parameters COLON simple_type_decl { $$=new FuncDecl($2,$3,$5); }
	      ;

procedure_decl : procedure_head SEMI sub_routine SEMI { $$=$1; $$->setRoutine($3); }
	       ;

procedure_head : PROCEDURE NAME parameters { $$=new FuncDecl($2,$3); }
	       ;

parameters : LP para_decl_list RP { $$=$2; }
	   | { $$=new vector<Parameter*>(); }
	   ;

para_decl_list : para_decl_list SEMI para_type_list { $$=$1; $$->push_back($3); }
               | para_type_list { $$=new vector<Parameter*>(); $$->push_back($1); }
	           ;

para_type_list : var_para_list COLON simple_type_decl { $$=$1; $1->setType($3); }
	       | val_para_list COLON simple_type_decl { $$=$1; $1->setType($3); }
	       ;

var_para_list : VAR name_list { $$=new Parameter($2,true); }
	      ;

val_para_list : name_list { $$=new Parameter($1,false); }
	      ;

routine_body : compound_stmt { $$=$1; }
             ;

compound_stmt : lBEGIN stmt_list END { $$=new CompoundStmt($2); }
              ;

stmt_list : stmt_list stmt SEMI { $$=$1; $$->push_back($2); }
          | /* empty */ { $$=new vector<Stmt*>(); }
          ;

stmt : INTEGER COLON non_label_stmt { $$=$3; $$->setLabel($1); }
     | non_label_stmt { $$=$1; }
     ;

non_label_stmt : assign_stmt { $$=$1; }
               | proc_stmt { $$=$1; }
               | compound_stmt { $$=$1; }
               | if_stmt { $$=$1; }
               | repeat_stmt { $$=$1; }
               | while_stmt { $$=$1; }
               | for_stmt { $$=$1; }
               | case_stmt { $$=$1; }
               | goto_stmt { $$=$1; }
               ;

assign_stmt : NAME ASSIGN expression { $$=new AssignStmt($1,$3); }
            | NAME LB expression RB ASSIGN expression { $$=new AssignStmt($1,$3,$6); }
            | NAME DOT NAME ASSIGN expression { $$=new AssignStmt($1,$3,$5); }
            ;

proc_stmt : NAME { $$=new ProcStmt($1); }
          | NAME LP args_list RP { $$=new ProcStmt($1,$3); }
          | SYS_PROC { $$=new SysProc($1); }
          | SYS_PROC LP expression_list RP { $$=new SysProc($1,$3); }
          | READ LP factor RP { $$=new SysProc($1,$3); }
          ;

if_stmt : IF expression THEN stmt else_clause { $$=new IfStmt($2,$4,$5); }
        ;

else_clause : ELSE stmt { $$=$2; }
            | /* empty */ { $$=nullptr; }
            ;

repeat_stmt : REPEAT stmt_list UNTIL expression { $$=new RepeatStmt($2,$4); }
            ;

while_stmt : WHILE expression DO stmt { $$=new WhileStmt($2,$4); }
           ;

for_stmt : FOR NAME ASSIGN expression direction expression DO stmt { $$=new ForStmt($2,$4,$6,$5,$8); }
         ;

direction : TO { $$=true; }
          | DOWNTO { $$=false; }
          ;

case_stmt : CASE expression OF case_expr_list END { $$=new CaseStmt($2,$4); }
          ;

case_expr_list : case_expr_list case_expr { $$=$1; $$->push_back($2); }
               | case_expr { $$=new vector<CaseExpr*>(); $$->push_back($1); }
               ;

case_expr : const_value COLON stmt SEMI { $$=new CaseExpr($1,$3); }
          | NAME COLON stmt SEMI { $$=new CaseExpr($1,$3); }
          ;

goto_stmt : GOTO INTEGER { $$=new GotoStmt($2); }
          ;

expression_list : expression_list COMMA expression { $$=$1; $$->push_back($3); }
                | expression { $$=new vector<Expr*>(); $$->push_back($1); }
                ;

expression : expression GE expr { $$=new BinaryExpr(BINOP_GE,$1,$3); }
           | expression GT expr { $$=new BinaryExpr(BINOP_GT,$1,$3); }
           | expression LE expr { $$=new BinaryExpr(BINOP_LE,$1,$3); }
           | expression LT expr { $$=new BinaryExpr(BINOP_LT,$1,$3); }
           | expression EQUAL expr { $$=new BinaryExpr(BINOP_EQUAL,$1,$3); }
           | expression UNEQUAL expr { $$=new BinaryExpr(BINOP_UNEQUAL,$1,$3); }
           | expr { $$=$1; }
           ;

expr : expr PLUS term { $$=new BinaryExpr(BINOP_PLUS,$1,$3); }
     | expr MINUS term { $$=new BinaryExpr(BINOP_MINUS,$1,$3); }
     | expr OR term { $$=new BinaryExpr(BINOP_OR,$1,$3); }
     | term { $$=$1; }
     ;

term : term MUL factor { $$=new BinaryExpr(BINOP_MUL,$1,$3); }
     | term DIV factor { $$=new BinaryExpr(BINOP_DIV,$1,$3); }
     | term MOD factor { $$=new BinaryExpr(BINOP_MOD,$1,$3); }
     | term AND factor { $$=new BinaryExpr(BINOP_AND,$1,$3); }
     | factor { $$=$1; }
     ;

factor : NAME { $$=$1; }
       | NAME LP args_list RP { $$=new FuncStmt($1,$3); }
       | SYS_FUNCT { $$=new SysFunct($1); }
       | SYS_FUNCT LP args_list RP { $$=new SysFunct($1,$3); }
       | const_value { $$=$1; }
       | LP expression RP { $$=$2; }
       | NOT factor { $$=new BinaryExpr(BINOP_XOR,new Boolean(true),$2); }
       | MINUS factor { $$=new BinaryExpr(BINOP_MINUS,new Integer(0),$2); }
       | NAME LB expression RB { $$=new ArrayRef($1,$3); }
       | NAME DOT NAME { $$=new RecordRef($1,$3); }
       ;

args_list : args_list COMMA expression { $$=$1; $$->push_back($3); }
          | expression { $$=new vector<Expr*>(); $$->push_back($1); }
          ;
          
NAME : ID { $$=new Identifier($1); }
     ;

%%



