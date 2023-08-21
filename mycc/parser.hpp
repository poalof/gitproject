/* A Bison parser, made by GNU Bison 3.0.4.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015 Free Software Foundation, Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

#ifndef YY_YY_PARSER_HPP_INCLUDED
# define YY_YY_PARSER_HPP_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    LP = 258,
    RP = 259,
    LB = 260,
    RB = 261,
    DOT = 262,
    COMMA = 263,
    COLON = 264,
    MUL = 265,
    DIV = 266,
    UNEQUAL = 267,
    NOT = 268,
    PLUS = 269,
    MINUS = 270,
    GE = 271,
    GT = 272,
    LE = 273,
    LT = 274,
    EQUAL = 275,
    ASSIGN = 276,
    MOD = 277,
    DOTDOT = 278,
    SEMI = 279,
    AND = 280,
    ARRAY = 281,
    lBEGIN = 282,
    CASE = 283,
    CONST = 284,
    DO = 285,
    DOWNTO = 286,
    ELSE = 287,
    END = 288,
    FOR = 289,
    FUNCTION = 290,
    GOTO = 291,
    IF = 292,
    OF = 293,
    OR = 294,
    PACKED = 295,
    PROCEDURE = 296,
    PROGRAM = 297,
    RECORD = 298,
    REPEAT = 299,
    THEN = 300,
    TO = 301,
    TYPE = 302,
    UNTIL = 303,
    VAR = 304,
    WHILE = 305,
    ID = 306,
    SYS_CON = 307,
    SYS_FUNCT = 308,
    SYS_PROC = 309,
    SYS_TYPE = 310,
    READ = 311,
    INTEGER = 312,
    REAL = 313,
    CHAR = 314
  };
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED

union YYSTYPE
{
#line 20 "parser.y" /* yacc.c:1909  */

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

#line 172 "parser.hpp" /* yacc.c:1909  */
};

typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_PARSER_HPP_INCLUDED  */
