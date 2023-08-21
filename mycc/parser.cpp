/* A Bison parser, made by GNU Bison 3.0.4.  */

/* Bison implementation for Yacc-like parsers in C

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

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "3.0.4"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* Copy the first part of user declarations.  */
#line 1 "parser.y" /* yacc.c:339  */


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


#line 84 "parser.cpp" /* yacc.c:339  */

# ifndef YY_NULLPTR
#  if defined __cplusplus && 201103L <= __cplusplus
#   define YY_NULLPTR nullptr
#  else
#   define YY_NULLPTR 0
#  endif
# endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

/* In a future release of Bison, this section will be replaced
   by #include "parser.hpp".  */
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
#line 20 "parser.y" /* yacc.c:355  */

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

#line 242 "parser.cpp" /* yacc.c:355  */
};

typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_PARSER_HPP_INCLUDED  */

/* Copy the second part of user declarations.  */

#line 259 "parser.cpp" /* yacc.c:358  */

#ifdef short
# undef short
#endif

#ifdef YYTYPE_UINT8
typedef YYTYPE_UINT8 yytype_uint8;
#else
typedef unsigned char yytype_uint8;
#endif

#ifdef YYTYPE_INT8
typedef YYTYPE_INT8 yytype_int8;
#else
typedef signed char yytype_int8;
#endif

#ifdef YYTYPE_UINT16
typedef YYTYPE_UINT16 yytype_uint16;
#else
typedef unsigned short int yytype_uint16;
#endif

#ifdef YYTYPE_INT16
typedef YYTYPE_INT16 yytype_int16;
#else
typedef short int yytype_int16;
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif ! defined YYSIZE_T
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned int
# endif
#endif

#define YYSIZE_MAXIMUM ((YYSIZE_T) -1)

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(Msgid) dgettext ("bison-runtime", Msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(Msgid) Msgid
# endif
#endif

#ifndef YY_ATTRIBUTE
# if (defined __GNUC__                                               \
      && (2 < __GNUC__ || (__GNUC__ == 2 && 96 <= __GNUC_MINOR__)))  \
     || defined __SUNPRO_C && 0x5110 <= __SUNPRO_C
#  define YY_ATTRIBUTE(Spec) __attribute__(Spec)
# else
#  define YY_ATTRIBUTE(Spec) /* empty */
# endif
#endif

#ifndef YY_ATTRIBUTE_PURE
# define YY_ATTRIBUTE_PURE   YY_ATTRIBUTE ((__pure__))
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# define YY_ATTRIBUTE_UNUSED YY_ATTRIBUTE ((__unused__))
#endif

#if !defined _Noreturn \
     && (!defined __STDC_VERSION__ || __STDC_VERSION__ < 201112)
# if defined _MSC_VER && 1200 <= _MSC_VER
#  define _Noreturn __declspec (noreturn)
# else
#  define _Noreturn YY_ATTRIBUTE ((__noreturn__))
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(E) ((void) (E))
#else
# define YYUSE(E) /* empty */
#endif

#if defined __GNUC__ && 407 <= __GNUC__ * 100 + __GNUC_MINOR__
/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN \
    _Pragma ("GCC diagnostic push") \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")\
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# define YY_IGNORE_MAYBE_UNINITIALIZED_END \
    _Pragma ("GCC diagnostic pop")
#else
# define YY_INITIAL_VALUE(Value) Value
#endif
#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
#endif


#if ! defined yyoverflow || YYERROR_VERBOSE

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
      /* Use EXIT_SUCCESS as a witness for stdlib.h.  */
#     ifndef EXIT_SUCCESS
#      define EXIT_SUCCESS 0
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's 'empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined EXIT_SUCCESS \
       && ! ((defined YYMALLOC || defined malloc) \
             && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef EXIT_SUCCESS
#    define EXIT_SUCCESS 0
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined EXIT_SUCCESS
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* ! defined yyoverflow || YYERROR_VERBOSE */


#if (! defined yyoverflow \
     && (! defined __cplusplus \
         || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yytype_int16 yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (yytype_int16) + sizeof (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)                           \
    do                                                                  \
      {                                                                 \
        YYSIZE_T yynewbytes;                                            \
        YYCOPY (&yyptr->Stack_alloc, Stack, yysize);                    \
        Stack = &yyptr->Stack_alloc;                                    \
        yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / sizeof (*yyptr);                          \
      }                                                                 \
    while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, (Count) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYSIZE_T yyi;                         \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (0)
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  5
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   419

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  60
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  57
/* YYNRULES -- Number of rules.  */
#define YYNRULES  127
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  261

/* YYTRANSLATE[YYX] -- Symbol number corresponding to YYX as returned
   by yylex, with out-of-bounds checking.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   314

#define YYTRANSLATE(YYX)                                                \
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, without out-of-bounds checking.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59
};

#if YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   124,   124,   127,   130,   133,   136,   139,   142,   143,
     146,   147,   150,   151,   152,   153,   158,   159,   162,   163,
     166,   169,   170,   171,   174,   178,   179,   180,   181,   182,
     183,   186,   189,   192,   193,   196,   199,   200,   203,   204,
     207,   208,   211,   214,   215,   216,   217,   218,   221,   224,
     227,   230,   233,   234,   237,   238,   241,   242,   245,   248,
     251,   254,   257,   258,   261,   262,   265,   266,   267,   268,
     269,   270,   271,   272,   273,   276,   277,   278,   281,   282,
     283,   284,   285,   288,   291,   292,   295,   298,   301,   304,
     305,   308,   311,   312,   315,   316,   319,   322,   323,   326,
     327,   328,   329,   330,   331,   332,   335,   336,   337,   338,
     341,   342,   343,   344,   345,   348,   349,   350,   351,   352,
     353,   354,   355,   356,   357,   360,   361,   364
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || 0
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "LP", "RP", "LB", "RB", "DOT", "COMMA",
  "COLON", "MUL", "DIV", "UNEQUAL", "NOT", "PLUS", "MINUS", "GE", "GT",
  "LE", "LT", "EQUAL", "ASSIGN", "MOD", "DOTDOT", "SEMI", "AND", "ARRAY",
  "lBEGIN", "CASE", "CONST", "DO", "DOWNTO", "ELSE", "END", "FOR",
  "FUNCTION", "GOTO", "IF", "OF", "OR", "PACKED", "PROCEDURE", "PROGRAM",
  "RECORD", "REPEAT", "THEN", "TO", "TYPE", "UNTIL", "VAR", "WHILE", "ID",
  "SYS_CON", "SYS_FUNCT", "SYS_PROC", "SYS_TYPE", "READ", "INTEGER",
  "REAL", "CHAR", "$accept", "program", "program_head", "routine",
  "sub_routine", "routine_head", "label_part", "const_part",
  "const_expr_list", "const_value", "type_part", "type_decl_list",
  "type_definition", "type_decl", "simple_type_decl", "array_type_decl",
  "record_type_decl", "field_decl_list", "field_decl", "name_list",
  "var_part", "var_decl_list", "var_decl", "routine_part", "function_decl",
  "function_head", "procedure_decl", "procedure_head", "parameters",
  "para_decl_list", "para_type_list", "var_para_list", "val_para_list",
  "routine_body", "compound_stmt", "stmt_list", "stmt", "non_label_stmt",
  "assign_stmt", "proc_stmt", "if_stmt", "else_clause", "repeat_stmt",
  "while_stmt", "for_stmt", "direction", "case_stmt", "case_expr_list",
  "case_expr", "goto_stmt", "expression_list", "expression", "expr",
  "term", "factor", "args_list", "NAME", YY_NULLPTR
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[NUM] -- (External) token number corresponding to the
   (internal) symbol number NUM (which must be that of a token).  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292,   293,   294,
     295,   296,   297,   298,   299,   300,   301,   302,   303,   304,
     305,   306,   307,   308,   309,   310,   311,   312,   313,   314
};
# endif

#define YYPACT_NINF -168

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-168)))

#define YYTABLE_NINF -1

#define yytable_value_is_error(Yytable_value) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
     -26,   -30,    49,  -168,    31,  -168,    64,    66,    61,  -168,
    -168,  -168,  -168,  -168,    45,    48,   221,  -168,    45,    85,
      45,    60,   170,  -168,    45,    41,   170,  -168,   170,   107,
     119,   110,  -168,   104,  -168,  -168,  -168,  -168,  -168,  -168,
    -168,  -168,  -168,    63,   109,   -15,    45,  -168,   111,    45,
      16,   170,   170,   170,  -168,   129,  -168,  -168,  -168,  -168,
     309,    -1,    14,  -168,   101,   120,  -168,   225,   232,   326,
     170,   170,   268,  -168,   170,   170,    45,   170,   -15,   121,
    -168,   143,     0,    45,  -168,  -168,    45,    45,    16,  -168,
     123,  -168,   124,   350,  -168,  -168,   170,   170,   170,   170,
     170,   170,   170,   354,   170,   170,   170,   170,   170,   170,
     170,   170,   170,    45,   170,   257,   170,   257,    44,   384,
     139,  -168,   384,    78,   366,   131,   384,   126,  -168,    45,
     -15,   149,    45,  -168,   132,   138,  -168,  -168,  -168,   133,
      45,   143,  -168,   161,   161,  -168,  -168,  -168,  -168,  -168,
      79,    -1,    -1,    -1,    -1,    -1,    -1,   159,   282,  -168,
     165,    14,    14,    14,  -168,  -168,  -168,  -168,   113,   375,
    -168,    10,   144,   384,  -168,  -168,   170,  -168,  -168,   170,
     154,   170,  -168,   116,   147,   108,   -18,  -168,    52,   -15,
    -168,    45,  -168,   155,    25,   169,  -168,   157,    66,   158,
    -168,   257,  -168,  -168,   257,  -168,  -168,  -168,  -168,   170,
     257,  -168,   384,   384,   170,   384,  -168,   -12,   178,  -168,
    -168,   143,  -168,  -168,  -168,    45,   180,     7,  -168,   182,
     188,   108,  -168,  -168,  -168,   175,   181,   341,  -168,   384,
     -15,  -168,   166,   184,   180,  -168,    25,   108,   108,  -168,
    -168,  -168,   257,  -168,   143,  -168,  -168,  -168,  -168,  -168,
    -168
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       0,     0,     0,     7,     0,     1,     0,     0,     9,     3,
       2,    63,     4,    60,     0,    17,     0,   127,     8,     0,
       0,    39,     0,    61,     0,     0,     0,    63,     0,    80,
       0,     0,    68,     0,    65,    66,    67,    69,    70,    71,
      72,    73,    74,    78,     0,     0,    16,    19,     0,     0,
      47,     0,     0,     0,    15,   117,    12,    13,    14,   119,
       0,   105,   109,   114,   115,     0,    96,     0,     0,     0,
       0,     0,     0,    62,     0,     0,     0,     0,     0,     0,
      18,     0,     0,    38,    41,    37,     0,     0,     6,    45,
       0,    46,     0,     0,   121,   122,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    98,
       0,    64,   126,     0,     0,     0,    75,     0,    11,     0,
       0,     0,     0,    24,     0,     0,    21,    22,    23,    25,
       0,     0,    40,    53,    53,    43,    44,     7,     7,   120,
       0,   104,    99,   100,   101,   102,   103,     0,     0,    93,
       0,   106,   107,   108,   110,   111,   112,   113,     0,     0,
     124,     0,    85,    86,    87,    81,     0,    82,    79,     0,
       0,     0,    10,     0,     0,     0,     0,    34,     0,     0,
      20,     0,    36,     0,     0,     0,    51,     0,     0,     0,
     118,     0,    91,    92,     0,   116,   123,    90,    89,     0,
       0,    83,    97,   125,     0,    77,    26,     0,     0,    32,
      33,     0,    27,    30,    42,     0,    59,     0,    55,     0,
       0,     0,    48,     5,    50,     0,     0,     0,    84,    76,
       0,    28,     0,     0,    58,    52,     0,     0,     0,    49,
      94,    95,     0,    29,     0,    35,    54,    56,    57,    88,
      31
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -168,  -168,  -168,  -168,    58,   206,  -168,  -168,  -168,   -28,
    -168,  -168,   164,  -129,  -167,  -168,  -168,  -168,    28,  -109,
    -168,  -168,   141,  -168,   127,  -168,   137,  -168,    86,  -168,
     -11,  -168,  -168,    33,    -6,   209,  -110,   167,  -168,  -168,
    -168,  -168,  -168,  -168,  -168,  -168,  -168,  -168,    89,  -168,
    -168,    37,   317,    34,    26,   -77,   -14
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     2,     3,     6,   197,   198,     8,    15,    18,    59,
      21,    46,    47,   135,   136,   137,   138,   186,   187,    82,
      50,    83,    84,    88,    89,    90,    91,    92,   195,   227,
     228,   229,   230,    12,    32,    16,    33,    34,    35,    36,
      37,   211,    38,    39,    40,   209,    41,   158,   159,    42,
     118,   122,    61,    62,    63,   123,    64
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_uint16 yytable[] =
{
      19,    13,    43,   240,    44,   172,    48,   174,   140,   141,
      65,   245,   193,   104,   105,   219,     1,    79,   218,   150,
     183,     4,    97,   188,   107,   108,    98,    99,   100,   101,
     102,   246,    48,    17,   168,    85,   109,    54,   106,   110,
      54,   207,    56,    57,    58,    56,    57,    58,   175,     5,
     127,    86,   176,   134,    43,     9,   208,    87,    43,    60,
     140,   221,   125,    67,   249,    69,    74,   139,    75,    85,
      76,    10,   143,   144,   225,   157,    17,   188,    94,    95,
     257,   258,   178,   200,    77,   226,   179,   179,    93,   160,
      14,   235,   243,    11,   236,    20,    17,   120,    66,   170,
     238,    43,   184,    43,   111,    45,   112,   119,   113,    49,
      70,   129,   124,   134,   126,    85,   244,   205,    85,    72,
     216,   179,    71,   130,   140,   260,   192,   139,    73,    78,
     157,    81,    96,   164,   165,   166,   167,   226,   161,   162,
     163,   114,   259,   177,   160,   128,   129,   147,   148,   169,
     182,   171,   181,   173,   185,   189,   191,   134,   130,    17,
      54,   222,   190,   133,   194,    56,    57,    58,   201,   131,
     217,   139,    85,    51,   204,   214,   210,   223,   231,   224,
      85,   232,   234,    52,   242,    53,   132,    43,   140,   241,
      43,   247,    13,   134,    17,    54,    43,   248,   133,   250,
      56,    57,    58,   134,   254,   251,   199,   139,   255,     7,
      80,    85,   253,   212,   220,   145,   213,   139,   215,   134,
     134,    17,    54,    55,   142,   146,   134,    56,    57,    58,
     196,   233,    85,   139,   139,   256,    68,    97,    43,   121,
     139,    98,    99,   100,   101,   102,   237,   203,    11,    22,
       0,   239,     0,     0,    23,    24,     0,    25,    26,    11,
      22,     0,     0,     0,     0,    27,    24,     0,    25,    26,
     115,    28,    17,     0,     0,    29,    27,    30,    31,     0,
     116,     0,    28,    17,    11,    22,    29,     0,    30,    31,
       0,    24,     0,    25,    26,    11,    22,     0,     0,     0,
       0,    27,    24,     0,    25,    26,     0,    28,    17,     0,
       0,    29,    27,    30,    31,   202,     0,     0,    28,    17,
       0,    97,    29,     0,    30,    98,    99,   100,   101,   102,
       0,     0,     0,    17,    54,     0,     0,     0,    97,    56,
      57,    58,    98,    99,   100,   101,   102,   103,     0,     0,
       0,     0,     0,    97,   149,     0,   117,    98,    99,   100,
     101,   102,    97,     0,     0,     0,    98,    99,   100,   101,
     102,   252,   180,     0,     0,     0,     0,     0,    97,     0,
       0,   206,    98,    99,   100,   101,   102,    97,     0,     0,
       0,    98,    99,   100,   101,   102,    97,     0,     0,     0,
      98,    99,   100,   101,   102,    17,    54,     0,     0,     0,
       0,    56,    57,    58,   151,   152,   153,   154,   155,   156
};

static const yytype_int16 yycheck[] =
{
      14,     7,    16,    15,    18,   115,    20,   117,     8,     9,
      24,     4,   141,    14,    15,    33,    42,    45,   185,    96,
     129,    51,    12,   132,    10,    11,    16,    17,    18,    19,
      20,    24,    46,    51,   111,    49,    22,    52,    39,    25,
      52,    31,    57,    58,    59,    57,    58,    59,     4,     0,
      78,    35,     8,    81,    68,    24,    46,    41,    72,    22,
       8,     9,    76,    26,   231,    28,     3,    81,     5,    83,
       7,     7,    86,    87,    49,   103,    51,   186,    52,    53,
     247,   248,     4,     4,    21,   194,     8,     8,    51,   103,
      29,   201,   221,    27,   204,    47,    51,    71,    57,   113,
     210,   115,   130,   117,     3,    20,     5,    70,     7,    49,
       3,     3,    75,   141,    77,   129,   225,     4,   132,     9,
       4,     8,     3,    15,     8,   254,   140,   141,    24,    20,
     158,    20,     3,   107,   108,   109,   110,   246,   104,   105,
     106,    21,   252,     4,   158,    24,     3,    24,    24,   112,
      24,   114,    21,   116,     5,    23,    23,   185,    15,    51,
      52,   189,    24,    55,     3,    57,    58,    59,     9,    26,
      23,   185,   186,     3,     9,    21,    32,   191,     9,    24,
     194,    24,    24,    13,     6,    15,    43,   201,     8,   217,
     204,     9,   198,   221,    51,    52,   210,     9,    55,    24,
      57,    58,    59,   231,    38,    24,   148,   221,    24,     3,
      46,   225,   240,   176,   186,    88,   179,   231,   181,   247,
     248,    51,    52,    53,    83,    88,   254,    57,    58,    59,
     144,   198,   246,   247,   248,   246,    27,    12,   252,    72,
     254,    16,    17,    18,    19,    20,   209,   158,    27,    28,
      -1,   214,    -1,    -1,    33,    34,    -1,    36,    37,    27,
      28,    -1,    -1,    -1,    -1,    44,    34,    -1,    36,    37,
      45,    50,    51,    -1,    -1,    54,    44,    56,    57,    -1,
      48,    -1,    50,    51,    27,    28,    54,    -1,    56,    57,
      -1,    34,    -1,    36,    37,    27,    28,    -1,    -1,    -1,
      -1,    44,    34,    -1,    36,    37,    -1,    50,    51,    -1,
      -1,    54,    44,    56,    57,    33,    -1,    -1,    50,    51,
      -1,    12,    54,    -1,    56,    16,    17,    18,    19,    20,
      -1,    -1,    -1,    51,    52,    -1,    -1,    -1,    12,    57,
      58,    59,    16,    17,    18,    19,    20,    38,    -1,    -1,
      -1,    -1,    -1,    12,     4,    -1,    30,    16,    17,    18,
      19,    20,    12,    -1,    -1,    -1,    16,    17,    18,    19,
      20,    30,     6,    -1,    -1,    -1,    -1,    -1,    12,    -1,
      -1,     6,    16,    17,    18,    19,    20,    12,    -1,    -1,
      -1,    16,    17,    18,    19,    20,    12,    -1,    -1,    -1,
      16,    17,    18,    19,    20,    51,    52,    -1,    -1,    -1,
      -1,    57,    58,    59,    97,    98,    99,   100,   101,   102
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,    42,    61,    62,    51,     0,    63,    65,    66,    24,
       7,    27,    93,    94,    29,    67,    95,    51,    68,   116,
      47,    70,    28,    33,    34,    36,    37,    44,    50,    54,
      56,    57,    94,    96,    97,    98,    99,   100,   102,   103,
     104,   106,   109,   116,   116,    20,    71,    72,   116,    49,
      80,     3,    13,    15,    52,    53,    57,    58,    59,    69,
     111,   112,   113,   114,   116,   116,    57,   111,    95,   111,
       3,     3,     9,    24,     3,     5,     7,    21,    20,    69,
      72,    20,    79,    81,    82,   116,    35,    41,    83,    84,
      85,    86,    87,   111,   114,   114,     3,    12,    16,    17,
      18,    19,    20,    38,    14,    15,    39,    10,    11,    22,
      25,     3,     5,     7,    21,    45,    48,    30,   110,   111,
     114,    97,   111,   115,   111,   116,   111,    69,    24,     3,
      15,    26,    43,    55,    69,    73,    74,    75,    76,   116,
       8,     9,    82,   116,   116,    84,    86,    24,    24,     4,
     115,   112,   112,   112,   112,   112,   112,    69,   107,   108,
     116,   113,   113,   113,   114,   114,   114,   114,   115,   111,
     116,   111,    96,   111,    96,     4,     8,     4,     4,     8,
       6,    21,    24,    79,    69,     5,    77,    78,    79,    23,
      24,    23,   116,    73,     3,    88,    88,    64,    65,    64,
       4,     9,    33,   108,     9,     4,     6,    31,    46,   105,
      32,   101,   111,   111,    21,   111,     4,    23,    74,    33,
      78,     9,    69,   116,    24,    49,    79,    89,    90,    91,
      92,     9,    24,    93,    24,    96,    96,   111,    96,   111,
      15,    69,     6,    73,    79,     4,    24,     9,     9,    74,
      24,    24,    30,    69,    38,    24,    90,    74,    74,    96,
      73
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    60,    61,    62,    63,    64,    65,    66,    67,    67,
      68,    68,    69,    69,    69,    69,    70,    70,    71,    71,
      72,    73,    73,    73,    74,    74,    74,    74,    74,    74,
      74,    75,    76,    77,    77,    78,    79,    79,    80,    80,
      81,    81,    82,    83,    83,    83,    83,    83,    84,    85,
      86,    87,    88,    88,    89,    89,    90,    90,    91,    92,
      93,    94,    95,    95,    96,    96,    97,    97,    97,    97,
      97,    97,    97,    97,    97,    98,    98,    98,    99,    99,
      99,    99,    99,   100,   101,   101,   102,   103,   104,   105,
     105,   106,   107,   107,   108,   108,   109,   110,   110,   111,
     111,   111,   111,   111,   111,   111,   112,   112,   112,   112,
     113,   113,   113,   113,   113,   114,   114,   114,   114,   114,
     114,   114,   114,   114,   114,   115,   115,   116
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     3,     3,     2,     2,     5,     0,     2,     0,
       5,     4,     1,     1,     1,     1,     2,     0,     2,     1,
       4,     1,     1,     1,     1,     1,     3,     3,     4,     5,
       3,     6,     3,     2,     1,     4,     3,     1,     2,     0,
       2,     1,     4,     2,     2,     1,     1,     0,     4,     5,
       4,     3,     3,     0,     3,     1,     3,     3,     2,     1,
       1,     3,     3,     0,     3,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     3,     6,     5,     1,     4,
       1,     4,     4,     5,     2,     0,     4,     4,     8,     1,
       1,     5,     2,     1,     4,     4,     2,     3,     1,     3,
       3,     3,     3,     3,     3,     1,     3,     3,     3,     1,
       3,     3,     3,     3,     1,     1,     4,     1,     4,     1,
       3,     2,     2,     4,     3,     3,     1,     1
};


#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)
#define YYEMPTY         (-2)
#define YYEOF           0

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                  \
do                                                              \
  if (yychar == YYEMPTY)                                        \
    {                                                           \
      yychar = (Token);                                         \
      yylval = (Value);                                         \
      YYPOPSTACK (yylen);                                       \
      yystate = *yyssp;                                         \
      goto yybackup;                                            \
    }                                                           \
  else                                                          \
    {                                                           \
      yyerror (YY_("syntax error: cannot back up")); \
      YYERROR;                                                  \
    }                                                           \
while (0)

/* Error token number */
#define YYTERROR        1
#define YYERRCODE       256



/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)                        \
do {                                            \
  if (yydebug)                                  \
    YYFPRINTF Args;                             \
} while (0)

/* This macro is provided for backward compatibility. */
#ifndef YY_LOCATION_PRINT
# define YY_LOCATION_PRINT(File, Loc) ((void) 0)
#endif


# define YY_SYMBOL_PRINT(Title, Type, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Type, Value); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*----------------------------------------.
| Print this symbol's value on YYOUTPUT.  |
`----------------------------------------*/

static void
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
{
  FILE *yyo = yyoutput;
  YYUSE (yyo);
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# endif
  YYUSE (yytype);
}


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

static void
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
{
  YYFPRINTF (yyoutput, "%s %s (",
             yytype < YYNTOKENS ? "token" : "nterm", yytname[yytype]);

  yy_symbol_value_print (yyoutput, yytype, yyvaluep);
  YYFPRINTF (yyoutput, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
yy_stack_print (yytype_int16 *yybottom, yytype_int16 *yytop)
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)                            \
do {                                                            \
  if (yydebug)                                                  \
    yy_stack_print ((Bottom), (Top));                           \
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

static void
yy_reduce_print (yytype_int16 *yyssp, YYSTYPE *yyvsp, int yyrule)
{
  unsigned long int yylno = yyrline[yyrule];
  int yynrhs = yyr2[yyrule];
  int yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %lu):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr,
                       yystos[yyssp[yyi + 1 - yynrhs]],
                       &(yyvsp[(yyi + 1) - (yynrhs)])
                                              );
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, Rule); \
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args)
# define YY_SYMBOL_PRINT(Title, Type, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif


#if YYERROR_VERBOSE

# ifndef yystrlen
#  if defined __GLIBC__ && defined _STRING_H
#   define yystrlen strlen
#  else
/* Return the length of YYSTR.  */
static YYSIZE_T
yystrlen (const char *yystr)
{
  YYSIZE_T yylen;
  for (yylen = 0; yystr[yylen]; yylen++)
    continue;
  return yylen;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
static char *
yystpcpy (char *yydest, const char *yysrc)
{
  char *yyd = yydest;
  const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
#  endif
# endif

# ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYSIZE_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYSIZE_T yyn = 0;
      char const *yyp = yystr;

      for (;;)
        switch (*++yyp)
          {
          case '\'':
          case ',':
            goto do_not_strip_quotes;

          case '\\':
            if (*++yyp != '\\')
              goto do_not_strip_quotes;
            /* Fall through.  */
          default:
            if (yyres)
              yyres[yyn] = *yyp;
            yyn++;
            break;

          case '"':
            if (yyres)
              yyres[yyn] = '\0';
            return yyn;
          }
    do_not_strip_quotes: ;
    }

  if (! yyres)
    return yystrlen (yystr);

  return yystpcpy (yyres, yystr) - yyres;
}
# endif

/* Copy into *YYMSG, which is of size *YYMSG_ALLOC, an error message
   about the unexpected token YYTOKEN for the state stack whose top is
   YYSSP.

   Return 0 if *YYMSG was successfully written.  Return 1 if *YYMSG is
   not large enough to hold the message.  In that case, also set
   *YYMSG_ALLOC to the required number of bytes.  Return 2 if the
   required number of bytes is too large to store.  */
static int
yysyntax_error (YYSIZE_T *yymsg_alloc, char **yymsg,
                yytype_int16 *yyssp, int yytoken)
{
  YYSIZE_T yysize0 = yytnamerr (YY_NULLPTR, yytname[yytoken]);
  YYSIZE_T yysize = yysize0;
  enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
  /* Internationalized format string. */
  const char *yyformat = YY_NULLPTR;
  /* Arguments of yyformat. */
  char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
  /* Number of reported tokens (one for the "unexpected", one per
     "expected"). */
  int yycount = 0;

  /* There are many possibilities here to consider:
     - If this state is a consistent state with a default action, then
       the only way this function was invoked is if the default action
       is an error action.  In that case, don't check for expected
       tokens because there are none.
     - The only way there can be no lookahead present (in yychar) is if
       this state is a consistent state with a default action.  Thus,
       detecting the absence of a lookahead is sufficient to determine
       that there is no unexpected or expected token to report.  In that
       case, just report a simple "syntax error".
     - Don't assume there isn't a lookahead just because this state is a
       consistent state with a default action.  There might have been a
       previous inconsistent state, consistent state with a non-default
       action, or user semantic action that manipulated yychar.
     - Of course, the expected token list depends on states to have
       correct lookahead information, and it depends on the parser not
       to perform extra reductions after fetching a lookahead from the
       scanner and before detecting a syntax error.  Thus, state merging
       (from LALR or IELR) and default reductions corrupt the expected
       token list.  However, the list is correct for canonical LR with
       one exception: it will still contain any token that will not be
       accepted due to an error action in a later state.
  */
  if (yytoken != YYEMPTY)
    {
      int yyn = yypact[*yyssp];
      yyarg[yycount++] = yytname[yytoken];
      if (!yypact_value_is_default (yyn))
        {
          /* Start YYX at -YYN if negative to avoid negative indexes in
             YYCHECK.  In other words, skip the first -YYN actions for
             this state because they are default actions.  */
          int yyxbegin = yyn < 0 ? -yyn : 0;
          /* Stay within bounds of both yycheck and yytname.  */
          int yychecklim = YYLAST - yyn + 1;
          int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
          int yyx;

          for (yyx = yyxbegin; yyx < yyxend; ++yyx)
            if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR
                && !yytable_value_is_error (yytable[yyx + yyn]))
              {
                if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
                  {
                    yycount = 1;
                    yysize = yysize0;
                    break;
                  }
                yyarg[yycount++] = yytname[yyx];
                {
                  YYSIZE_T yysize1 = yysize + yytnamerr (YY_NULLPTR, yytname[yyx]);
                  if (! (yysize <= yysize1
                         && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
                    return 2;
                  yysize = yysize1;
                }
              }
        }
    }

  switch (yycount)
    {
# define YYCASE_(N, S)                      \
      case N:                               \
        yyformat = S;                       \
      break
      YYCASE_(0, YY_("syntax error"));
      YYCASE_(1, YY_("syntax error, unexpected %s"));
      YYCASE_(2, YY_("syntax error, unexpected %s, expecting %s"));
      YYCASE_(3, YY_("syntax error, unexpected %s, expecting %s or %s"));
      YYCASE_(4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
      YYCASE_(5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
# undef YYCASE_
    }

  {
    YYSIZE_T yysize1 = yysize + yystrlen (yyformat);
    if (! (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
      return 2;
    yysize = yysize1;
  }

  if (*yymsg_alloc < yysize)
    {
      *yymsg_alloc = 2 * yysize;
      if (! (yysize <= *yymsg_alloc
             && *yymsg_alloc <= YYSTACK_ALLOC_MAXIMUM))
        *yymsg_alloc = YYSTACK_ALLOC_MAXIMUM;
      return 1;
    }

  /* Avoid sprintf, as that infringes on the user's name space.
     Don't have undefined behavior even if the translation
     produced a string with the wrong number of "%s"s.  */
  {
    char *yyp = *yymsg;
    int yyi = 0;
    while ((*yyp = *yyformat) != '\0')
      if (*yyp == '%' && yyformat[1] == 's' && yyi < yycount)
        {
          yyp += yytnamerr (yyp, yyarg[yyi++]);
          yyformat += 2;
        }
      else
        {
          yyp++;
          yyformat++;
        }
  }
  return 0;
}
#endif /* YYERROR_VERBOSE */

/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep)
{
  YYUSE (yyvaluep);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YYUSE (yytype);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}




/* The lookahead symbol.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;
/* Number of syntax errors so far.  */
int yynerrs;


/*----------.
| yyparse.  |
`----------*/

int
yyparse (void)
{
    int yystate;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus;

    /* The stacks and their tools:
       'yyss': related to states.
       'yyvs': related to semantic values.

       Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* The state stack.  */
    yytype_int16 yyssa[YYINITDEPTH];
    yytype_int16 *yyss;
    yytype_int16 *yyssp;

    /* The semantic value stack.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs;
    YYSTYPE *yyvsp;

    YYSIZE_T yystacksize;

  int yyn;
  int yyresult;
  /* Lookahead token as an internal (translated) token number.  */
  int yytoken = 0;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;

#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYSIZE_T yymsg_alloc = sizeof yymsgbuf;
#endif

#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  yyssp = yyss = yyssa;
  yyvsp = yyvs = yyvsa;
  yystacksize = YYINITDEPTH;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY; /* Cause a token to be read.  */
  goto yysetstate;

/*------------------------------------------------------------.
| yynewstate -- Push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
 yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;

 yysetstate:
  *yyssp = yystate;

  if (yyss + yystacksize - 1 <= yyssp)
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T yysize = yyssp - yyss + 1;

#ifdef yyoverflow
      {
        /* Give user a chance to reallocate the stack.  Use copies of
           these so that the &'s don't force the real ones into
           memory.  */
        YYSTYPE *yyvs1 = yyvs;
        yytype_int16 *yyss1 = yyss;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * sizeof (*yyssp),
                    &yyvs1, yysize * sizeof (*yyvsp),
                    &yystacksize);

        yyss = yyss1;
        yyvs = yyvs1;
      }
#else /* no yyoverflow */
# ifndef YYSTACK_RELOCATE
      goto yyexhaustedlab;
# else
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
        goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yytype_int16 *yyss1 = yyss;
        union yyalloc *yyptr =
          (union yyalloc *) YYSTACK_ALLOC (YYSTACK_BYTES (yystacksize));
        if (! yyptr)
          goto yyexhaustedlab;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
#  undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif
#endif /* no yyoverflow */

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YYDPRINTF ((stderr, "Stack size increased to %lu\n",
                  (unsigned long int) yystacksize));

      if (yyss + yystacksize - 1 <= yyssp)
        YYABORT;
    }

  YYDPRINTF ((stderr, "Entering state %d\n", yystate));

  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;

/*-----------.
| yybackup.  |
`-----------*/
yybackup:

  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yypact_value_is_default (yyn))
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid lookahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = yylex ();
    }

  if (yychar <= YYEOF)
    {
      yychar = yytoken = YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yytable_value_is_error (yyn))
        goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);

  /* Discard the shifted token.  */
  yychar = YYEMPTY;

  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- Do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     '$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
        case 2:
#line 124 "parser.y" /* yacc.c:1646  */
    { (yyval.program)=new Program((yyvsp[-2].sval),(yyvsp[-1].routine)); root=(yyval.program); }
#line 1559 "parser.cpp" /* yacc.c:1646  */
    break;

  case 3:
#line 127 "parser.y" /* yacc.c:1646  */
    { (yyval.sval)=(yyvsp[-1].sval); }
#line 1565 "parser.cpp" /* yacc.c:1646  */
    break;

  case 4:
#line 130 "parser.y" /* yacc.c:1646  */
    { (yyval.routine)=(yyvsp[-1].routine); (yyval.routine)->setRoutineBody((yyvsp[0].compoundStmt)); cout << "routine parsing done!" << endl; }
#line 1571 "parser.cpp" /* yacc.c:1646  */
    break;

  case 5:
#line 133 "parser.y" /* yacc.c:1646  */
    { (yyval.routine)=(yyvsp[-1].routine); (yyval.routine)->setRoutineBody((yyvsp[0].compoundStmt)); }
#line 1577 "parser.cpp" /* yacc.c:1646  */
    break;

  case 6:
#line 136 "parser.y" /* yacc.c:1646  */
    { (yyval.routine)=new Routine((yyvsp[-3].constDecls),(yyvsp[-2].typeDecls),(yyvsp[-1].varDecls),(yyvsp[0].routines)); }
#line 1583 "parser.cpp" /* yacc.c:1646  */
    break;

  case 8:
#line 142 "parser.y" /* yacc.c:1646  */
    { (yyval.constDecls)=(yyvsp[0].constDecls); }
#line 1589 "parser.cpp" /* yacc.c:1646  */
    break;

  case 9:
#line 143 "parser.y" /* yacc.c:1646  */
    { (yyval.constDecls)=new vector<ConstDecl*>(); }
#line 1595 "parser.cpp" /* yacc.c:1646  */
    break;

  case 10:
#line 146 "parser.y" /* yacc.c:1646  */
    { (yyval.constDecls)=(yyvsp[-4].constDecls); (yyval.constDecls)->push_back(new ConstDecl((yyvsp[-3].identifier),(yyvsp[-1].constVal))); }
#line 1601 "parser.cpp" /* yacc.c:1646  */
    break;

  case 11:
#line 147 "parser.y" /* yacc.c:1646  */
    { (yyval.constDecls)=new vector<ConstDecl*>(); (yyval.constDecls)->push_back(new ConstDecl((yyvsp[-3].identifier),(yyvsp[-1].constVal))); }
#line 1607 "parser.cpp" /* yacc.c:1646  */
    break;

  case 12:
#line 150 "parser.y" /* yacc.c:1646  */
    { (yyval.constVal)=new Integer((yyvsp[0].ival)); }
#line 1613 "parser.cpp" /* yacc.c:1646  */
    break;

  case 13:
#line 151 "parser.y" /* yacc.c:1646  */
    { (yyval.constVal)=new Real((yyvsp[0].dval)); }
#line 1619 "parser.cpp" /* yacc.c:1646  */
    break;

  case 14:
#line 152 "parser.y" /* yacc.c:1646  */
    { (yyval.constVal)=new Char((yyvsp[0].cval)); }
#line 1625 "parser.cpp" /* yacc.c:1646  */
    break;

  case 15:
#line 153 "parser.y" /* yacc.c:1646  */
    { if(*(yyvsp[0].sval)=="maxint") (yyval.constVal)=new Integer(INT_MAX);
                        else if(*(yyvsp[0].sval)=="true") (yyval.constVal)=new Boolean(true);
                        else (yyval.constVal)=new Boolean(false); }
#line 1633 "parser.cpp" /* yacc.c:1646  */
    break;

  case 16:
#line 158 "parser.y" /* yacc.c:1646  */
    { (yyval.typeDecls)=(yyvsp[0].typeDecls); }
#line 1639 "parser.cpp" /* yacc.c:1646  */
    break;

  case 17:
#line 159 "parser.y" /* yacc.c:1646  */
    { (yyval.typeDecls)=new vector<TypeDecl*>(); }
#line 1645 "parser.cpp" /* yacc.c:1646  */
    break;

  case 18:
#line 162 "parser.y" /* yacc.c:1646  */
    { (yyval.typeDecls)=(yyvsp[-1].typeDecls); (yyval.typeDecls)->push_back((yyvsp[0].typeDecl)); }
#line 1651 "parser.cpp" /* yacc.c:1646  */
    break;

  case 19:
#line 163 "parser.y" /* yacc.c:1646  */
    { (yyval.typeDecls)=new vector<TypeDecl*>(); (yyval.typeDecls)->push_back((yyvsp[0].typeDecl)); }
#line 1657 "parser.cpp" /* yacc.c:1646  */
    break;

  case 20:
#line 166 "parser.y" /* yacc.c:1646  */
    { (yyval.typeDecl)=new TypeDecl((yyvsp[-3].identifier),(yyvsp[-1].type)); }
#line 1663 "parser.cpp" /* yacc.c:1646  */
    break;

  case 21:
#line 169 "parser.y" /* yacc.c:1646  */
    { (yyval.type)=(yyvsp[0].type); }
#line 1669 "parser.cpp" /* yacc.c:1646  */
    break;

  case 22:
#line 170 "parser.y" /* yacc.c:1646  */
    { (yyval.type)=(yyvsp[0].type); }
#line 1675 "parser.cpp" /* yacc.c:1646  */
    break;

  case 23:
#line 171 "parser.y" /* yacc.c:1646  */
    { (yyval.type)=(yyvsp[0].type); }
#line 1681 "parser.cpp" /* yacc.c:1646  */
    break;

  case 24:
#line 174 "parser.y" /* yacc.c:1646  */
    { if(*(yyvsp[0].sval)=="boolean") (yyval.type)=new ASTType(TYPE_BOOLEAN); 
                              else if(*(yyvsp[0].sval)=="char") (yyval.type)=new ASTType(TYPE_CHAR);
                              else if(*(yyvsp[0].sval)=="integer") (yyval.type)=new ASTType(TYPE_INTEGER); 
                              else if(*(yyvsp[0].sval)=="real") (yyval.type)=new ASTType(TYPE_REAL); }
#line 1690 "parser.cpp" /* yacc.c:1646  */
    break;

  case 25:
#line 178 "parser.y" /* yacc.c:1646  */
    { (yyval.type)=new ASTType((yyvsp[0].identifier)); }
#line 1696 "parser.cpp" /* yacc.c:1646  */
    break;

  case 26:
#line 179 "parser.y" /* yacc.c:1646  */
    { (yyval.type)=new ASTType(new EnumType((yyvsp[-1].names))); }
#line 1702 "parser.cpp" /* yacc.c:1646  */
    break;

  case 27:
#line 180 "parser.y" /* yacc.c:1646  */
    { (yyval.type)=new ASTType(new ConstRangeType((yyvsp[-2].constVal),(yyvsp[0].constVal))); }
#line 1708 "parser.cpp" /* yacc.c:1646  */
    break;

  case 28:
#line 181 "parser.y" /* yacc.c:1646  */
    { (yyval.type)=new ASTType(new ConstRangeType(-*(yyvsp[-2].constVal),(yyvsp[0].constVal))); }
#line 1714 "parser.cpp" /* yacc.c:1646  */
    break;

  case 29:
#line 182 "parser.y" /* yacc.c:1646  */
    { (yyval.type)=new ASTType(new ConstRangeType(-*(yyvsp[-3].constVal),-*(yyvsp[0].constVal))); }
#line 1720 "parser.cpp" /* yacc.c:1646  */
    break;

  case 30:
#line 183 "parser.y" /* yacc.c:1646  */
    { (yyval.type)=new ASTType(new EnumSubrangeType((yyvsp[-2].identifier),(yyvsp[0].identifier))); }
#line 1726 "parser.cpp" /* yacc.c:1646  */
    break;

  case 31:
#line 186 "parser.y" /* yacc.c:1646  */
    { (yyval.type)=new ASTType(new MyArrayType((yyvsp[-3].type),(yyvsp[0].type))); }
#line 1732 "parser.cpp" /* yacc.c:1646  */
    break;

  case 32:
#line 189 "parser.y" /* yacc.c:1646  */
    { (yyval.type)=new ASTType(new RecordType((yyvsp[-1].fields))); }
#line 1738 "parser.cpp" /* yacc.c:1646  */
    break;

  case 33:
#line 192 "parser.y" /* yacc.c:1646  */
    { (yyval.fields)=(yyvsp[-1].fields); (yyval.fields)->push_back((yyvsp[0].fieldDecl)); }
#line 1744 "parser.cpp" /* yacc.c:1646  */
    break;

  case 34:
#line 193 "parser.y" /* yacc.c:1646  */
    { (yyval.fields)=new vector<FieldDecl*>(); (yyval.fields)->push_back((yyvsp[0].fieldDecl)); }
#line 1750 "parser.cpp" /* yacc.c:1646  */
    break;

  case 35:
#line 196 "parser.y" /* yacc.c:1646  */
    { (yyval.fieldDecl)=new FieldDecl((yyvsp[-3].names),(yyvsp[-1].type)); }
#line 1756 "parser.cpp" /* yacc.c:1646  */
    break;

  case 36:
#line 199 "parser.y" /* yacc.c:1646  */
    { (yyval.names)=(yyvsp[-2].names); (yyval.names)->push_back((yyvsp[0].identifier)); }
#line 1762 "parser.cpp" /* yacc.c:1646  */
    break;

  case 37:
#line 200 "parser.y" /* yacc.c:1646  */
    { (yyval.names)=new vector<Identifier*>(); (yyval.names)->push_back((yyvsp[0].identifier)); }
#line 1768 "parser.cpp" /* yacc.c:1646  */
    break;

  case 38:
#line 203 "parser.y" /* yacc.c:1646  */
    { (yyval.varDecls)=(yyvsp[0].varDecls); }
#line 1774 "parser.cpp" /* yacc.c:1646  */
    break;

  case 39:
#line 204 "parser.y" /* yacc.c:1646  */
    { (yyval.varDecls)=new vector<VarDecl*>(); }
#line 1780 "parser.cpp" /* yacc.c:1646  */
    break;

  case 40:
#line 207 "parser.y" /* yacc.c:1646  */
    { (yyval.varDecls)=(yyvsp[-1].varDecls); (yyval.varDecls)->push_back((yyvsp[0].varDecl));}
#line 1786 "parser.cpp" /* yacc.c:1646  */
    break;

  case 41:
#line 208 "parser.y" /* yacc.c:1646  */
    { (yyval.varDecls)=new vector<VarDecl*>(); (yyval.varDecls)->push_back((yyvsp[0].varDecl)); }
#line 1792 "parser.cpp" /* yacc.c:1646  */
    break;

  case 42:
#line 211 "parser.y" /* yacc.c:1646  */
    { (yyval.varDecl)=new VarDecl((yyvsp[-3].names),(yyvsp[-1].type)); }
#line 1798 "parser.cpp" /* yacc.c:1646  */
    break;

  case 43:
#line 214 "parser.y" /* yacc.c:1646  */
    { (yyval.routines)=(yyvsp[-1].routines); (yyval.routines)->push_back((yyvsp[0].funcDecl)); }
#line 1804 "parser.cpp" /* yacc.c:1646  */
    break;

  case 44:
#line 215 "parser.y" /* yacc.c:1646  */
    { (yyval.routines)=(yyvsp[-1].routines); (yyval.routines)->push_back((yyvsp[0].funcDecl)); }
#line 1810 "parser.cpp" /* yacc.c:1646  */
    break;

  case 45:
#line 216 "parser.y" /* yacc.c:1646  */
    { (yyval.routines)=new vector<FuncDecl*>(); (yyval.routines)->push_back((yyvsp[0].funcDecl)); }
#line 1816 "parser.cpp" /* yacc.c:1646  */
    break;

  case 46:
#line 217 "parser.y" /* yacc.c:1646  */
    { (yyval.routines)=new vector<FuncDecl*>(); (yyval.routines)->push_back((yyvsp[0].funcDecl)); }
#line 1822 "parser.cpp" /* yacc.c:1646  */
    break;

  case 47:
#line 218 "parser.y" /* yacc.c:1646  */
    { (yyval.routines)=new vector<FuncDecl*>(); }
#line 1828 "parser.cpp" /* yacc.c:1646  */
    break;

  case 48:
#line 221 "parser.y" /* yacc.c:1646  */
    { (yyval.funcDecl)=(yyvsp[-3].funcDecl); (yyval.funcDecl)->setRoutine((yyvsp[-1].routine)); }
#line 1834 "parser.cpp" /* yacc.c:1646  */
    break;

  case 49:
#line 224 "parser.y" /* yacc.c:1646  */
    { (yyval.funcDecl)=new FuncDecl((yyvsp[-3].identifier),(yyvsp[-2].paras),(yyvsp[0].type)); }
#line 1840 "parser.cpp" /* yacc.c:1646  */
    break;

  case 50:
#line 227 "parser.y" /* yacc.c:1646  */
    { (yyval.funcDecl)=(yyvsp[-3].funcDecl); (yyval.funcDecl)->setRoutine((yyvsp[-1].routine)); }
#line 1846 "parser.cpp" /* yacc.c:1646  */
    break;

  case 51:
#line 230 "parser.y" /* yacc.c:1646  */
    { (yyval.funcDecl)=new FuncDecl((yyvsp[-1].identifier),(yyvsp[0].paras)); }
#line 1852 "parser.cpp" /* yacc.c:1646  */
    break;

  case 52:
#line 233 "parser.y" /* yacc.c:1646  */
    { (yyval.paras)=(yyvsp[-1].paras); }
#line 1858 "parser.cpp" /* yacc.c:1646  */
    break;

  case 53:
#line 234 "parser.y" /* yacc.c:1646  */
    { (yyval.paras)=new vector<Parameter*>(); }
#line 1864 "parser.cpp" /* yacc.c:1646  */
    break;

  case 54:
#line 237 "parser.y" /* yacc.c:1646  */
    { (yyval.paras)=(yyvsp[-2].paras); (yyval.paras)->push_back((yyvsp[0].parameter)); }
#line 1870 "parser.cpp" /* yacc.c:1646  */
    break;

  case 55:
#line 238 "parser.y" /* yacc.c:1646  */
    { (yyval.paras)=new vector<Parameter*>(); (yyval.paras)->push_back((yyvsp[0].parameter)); }
#line 1876 "parser.cpp" /* yacc.c:1646  */
    break;

  case 56:
#line 241 "parser.y" /* yacc.c:1646  */
    { (yyval.parameter)=(yyvsp[-2].parameter); (yyvsp[-2].parameter)->setType((yyvsp[0].type)); }
#line 1882 "parser.cpp" /* yacc.c:1646  */
    break;

  case 57:
#line 242 "parser.y" /* yacc.c:1646  */
    { (yyval.parameter)=(yyvsp[-2].parameter); (yyvsp[-2].parameter)->setType((yyvsp[0].type)); }
#line 1888 "parser.cpp" /* yacc.c:1646  */
    break;

  case 58:
#line 245 "parser.y" /* yacc.c:1646  */
    { (yyval.parameter)=new Parameter((yyvsp[0].names),true); }
#line 1894 "parser.cpp" /* yacc.c:1646  */
    break;

  case 59:
#line 248 "parser.y" /* yacc.c:1646  */
    { (yyval.parameter)=new Parameter((yyvsp[0].names),false); }
#line 1900 "parser.cpp" /* yacc.c:1646  */
    break;

  case 60:
#line 251 "parser.y" /* yacc.c:1646  */
    { (yyval.compoundStmt)=(yyvsp[0].compoundStmt); }
#line 1906 "parser.cpp" /* yacc.c:1646  */
    break;

  case 61:
#line 254 "parser.y" /* yacc.c:1646  */
    { (yyval.compoundStmt)=new CompoundStmt((yyvsp[-1].stmts)); }
#line 1912 "parser.cpp" /* yacc.c:1646  */
    break;

  case 62:
#line 257 "parser.y" /* yacc.c:1646  */
    { (yyval.stmts)=(yyvsp[-2].stmts); (yyval.stmts)->push_back((yyvsp[-1].stmt)); }
#line 1918 "parser.cpp" /* yacc.c:1646  */
    break;

  case 63:
#line 258 "parser.y" /* yacc.c:1646  */
    { (yyval.stmts)=new vector<Stmt*>(); }
#line 1924 "parser.cpp" /* yacc.c:1646  */
    break;

  case 64:
#line 261 "parser.y" /* yacc.c:1646  */
    { (yyval.stmt)=(yyvsp[0].stmt); (yyval.stmt)->setLabel((yyvsp[-2].ival)); }
#line 1930 "parser.cpp" /* yacc.c:1646  */
    break;

  case 65:
#line 262 "parser.y" /* yacc.c:1646  */
    { (yyval.stmt)=(yyvsp[0].stmt); }
#line 1936 "parser.cpp" /* yacc.c:1646  */
    break;

  case 66:
#line 265 "parser.y" /* yacc.c:1646  */
    { (yyval.stmt)=(yyvsp[0].assignStmt); }
#line 1942 "parser.cpp" /* yacc.c:1646  */
    break;

  case 67:
#line 266 "parser.y" /* yacc.c:1646  */
    { (yyval.stmt)=(yyvsp[0].stmt); }
#line 1948 "parser.cpp" /* yacc.c:1646  */
    break;

  case 68:
#line 267 "parser.y" /* yacc.c:1646  */
    { (yyval.stmt)=(yyvsp[0].compoundStmt); }
#line 1954 "parser.cpp" /* yacc.c:1646  */
    break;

  case 69:
#line 268 "parser.y" /* yacc.c:1646  */
    { (yyval.stmt)=(yyvsp[0].ifStmt); }
#line 1960 "parser.cpp" /* yacc.c:1646  */
    break;

  case 70:
#line 269 "parser.y" /* yacc.c:1646  */
    { (yyval.stmt)=(yyvsp[0].repeatStmt); }
#line 1966 "parser.cpp" /* yacc.c:1646  */
    break;

  case 71:
#line 270 "parser.y" /* yacc.c:1646  */
    { (yyval.stmt)=(yyvsp[0].whileStmt); }
#line 1972 "parser.cpp" /* yacc.c:1646  */
    break;

  case 72:
#line 271 "parser.y" /* yacc.c:1646  */
    { (yyval.stmt)=(yyvsp[0].forStmt); }
#line 1978 "parser.cpp" /* yacc.c:1646  */
    break;

  case 73:
#line 272 "parser.y" /* yacc.c:1646  */
    { (yyval.stmt)=(yyvsp[0].caseStmt); }
#line 1984 "parser.cpp" /* yacc.c:1646  */
    break;

  case 74:
#line 273 "parser.y" /* yacc.c:1646  */
    { (yyval.stmt)=(yyvsp[0].gotoStmt); }
#line 1990 "parser.cpp" /* yacc.c:1646  */
    break;

  case 75:
#line 276 "parser.y" /* yacc.c:1646  */
    { (yyval.assignStmt)=new AssignStmt((yyvsp[-2].identifier),(yyvsp[0].expr)); }
#line 1996 "parser.cpp" /* yacc.c:1646  */
    break;

  case 76:
#line 277 "parser.y" /* yacc.c:1646  */
    { (yyval.assignStmt)=new AssignStmt((yyvsp[-5].identifier),(yyvsp[-3].expr),(yyvsp[0].expr)); }
#line 2002 "parser.cpp" /* yacc.c:1646  */
    break;

  case 77:
#line 278 "parser.y" /* yacc.c:1646  */
    { (yyval.assignStmt)=new AssignStmt((yyvsp[-4].identifier),(yyvsp[-2].identifier),(yyvsp[0].expr)); }
#line 2008 "parser.cpp" /* yacc.c:1646  */
    break;

  case 78:
#line 281 "parser.y" /* yacc.c:1646  */
    { (yyval.stmt)=new ProcStmt((yyvsp[0].identifier)); }
#line 2014 "parser.cpp" /* yacc.c:1646  */
    break;

  case 79:
#line 282 "parser.y" /* yacc.c:1646  */
    { (yyval.stmt)=new ProcStmt((yyvsp[-3].identifier),(yyvsp[-1].args)); }
#line 2020 "parser.cpp" /* yacc.c:1646  */
    break;

  case 80:
#line 283 "parser.y" /* yacc.c:1646  */
    { (yyval.stmt)=new SysProc((yyvsp[0].sval)); }
#line 2026 "parser.cpp" /* yacc.c:1646  */
    break;

  case 81:
#line 284 "parser.y" /* yacc.c:1646  */
    { (yyval.stmt)=new SysProc((yyvsp[-3].sval),(yyvsp[-1].exprs)); }
#line 2032 "parser.cpp" /* yacc.c:1646  */
    break;

  case 82:
#line 285 "parser.y" /* yacc.c:1646  */
    { (yyval.stmt)=new SysProc((yyvsp[-3].sval),(yyvsp[-1].expr)); }
#line 2038 "parser.cpp" /* yacc.c:1646  */
    break;

  case 83:
#line 288 "parser.y" /* yacc.c:1646  */
    { (yyval.ifStmt)=new IfStmt((yyvsp[-3].expr),(yyvsp[-1].stmt),(yyvsp[0].stmt)); }
#line 2044 "parser.cpp" /* yacc.c:1646  */
    break;

  case 84:
#line 291 "parser.y" /* yacc.c:1646  */
    { (yyval.stmt)=(yyvsp[0].stmt); }
#line 2050 "parser.cpp" /* yacc.c:1646  */
    break;

  case 85:
#line 292 "parser.y" /* yacc.c:1646  */
    { (yyval.stmt)=nullptr; }
#line 2056 "parser.cpp" /* yacc.c:1646  */
    break;

  case 86:
#line 295 "parser.y" /* yacc.c:1646  */
    { (yyval.repeatStmt)=new RepeatStmt((yyvsp[-2].stmts),(yyvsp[0].expr)); }
#line 2062 "parser.cpp" /* yacc.c:1646  */
    break;

  case 87:
#line 298 "parser.y" /* yacc.c:1646  */
    { (yyval.whileStmt)=new WhileStmt((yyvsp[-2].expr),(yyvsp[0].stmt)); }
#line 2068 "parser.cpp" /* yacc.c:1646  */
    break;

  case 88:
#line 301 "parser.y" /* yacc.c:1646  */
    { (yyval.forStmt)=new ForStmt((yyvsp[-6].identifier),(yyvsp[-4].expr),(yyvsp[-2].expr),(yyvsp[-3].bval),(yyvsp[0].stmt)); }
#line 2074 "parser.cpp" /* yacc.c:1646  */
    break;

  case 89:
#line 304 "parser.y" /* yacc.c:1646  */
    { (yyval.bval)=true; }
#line 2080 "parser.cpp" /* yacc.c:1646  */
    break;

  case 90:
#line 305 "parser.y" /* yacc.c:1646  */
    { (yyval.bval)=false; }
#line 2086 "parser.cpp" /* yacc.c:1646  */
    break;

  case 91:
#line 308 "parser.y" /* yacc.c:1646  */
    { (yyval.caseStmt)=new CaseStmt((yyvsp[-3].expr),(yyvsp[-1].caseExprs)); }
#line 2092 "parser.cpp" /* yacc.c:1646  */
    break;

  case 92:
#line 311 "parser.y" /* yacc.c:1646  */
    { (yyval.caseExprs)=(yyvsp[-1].caseExprs); (yyval.caseExprs)->push_back((yyvsp[0].caseExpr)); }
#line 2098 "parser.cpp" /* yacc.c:1646  */
    break;

  case 93:
#line 312 "parser.y" /* yacc.c:1646  */
    { (yyval.caseExprs)=new vector<CaseExpr*>(); (yyval.caseExprs)->push_back((yyvsp[0].caseExpr)); }
#line 2104 "parser.cpp" /* yacc.c:1646  */
    break;

  case 94:
#line 315 "parser.y" /* yacc.c:1646  */
    { (yyval.caseExpr)=new CaseExpr((yyvsp[-3].constVal),(yyvsp[-1].stmt)); }
#line 2110 "parser.cpp" /* yacc.c:1646  */
    break;

  case 95:
#line 316 "parser.y" /* yacc.c:1646  */
    { (yyval.caseExpr)=new CaseExpr((yyvsp[-3].identifier),(yyvsp[-1].stmt)); }
#line 2116 "parser.cpp" /* yacc.c:1646  */
    break;

  case 96:
#line 319 "parser.y" /* yacc.c:1646  */
    { (yyval.gotoStmt)=new GotoStmt((yyvsp[0].ival)); }
#line 2122 "parser.cpp" /* yacc.c:1646  */
    break;

  case 97:
#line 322 "parser.y" /* yacc.c:1646  */
    { (yyval.exprs)=(yyvsp[-2].exprs); (yyval.exprs)->push_back((yyvsp[0].expr)); }
#line 2128 "parser.cpp" /* yacc.c:1646  */
    break;

  case 98:
#line 323 "parser.y" /* yacc.c:1646  */
    { (yyval.exprs)=new vector<Expr*>(); (yyval.exprs)->push_back((yyvsp[0].expr)); }
#line 2134 "parser.cpp" /* yacc.c:1646  */
    break;

  case 99:
#line 326 "parser.y" /* yacc.c:1646  */
    { (yyval.expr)=new BinaryExpr(BINOP_GE,(yyvsp[-2].expr),(yyvsp[0].expr)); }
#line 2140 "parser.cpp" /* yacc.c:1646  */
    break;

  case 100:
#line 327 "parser.y" /* yacc.c:1646  */
    { (yyval.expr)=new BinaryExpr(BINOP_GT,(yyvsp[-2].expr),(yyvsp[0].expr)); }
#line 2146 "parser.cpp" /* yacc.c:1646  */
    break;

  case 101:
#line 328 "parser.y" /* yacc.c:1646  */
    { (yyval.expr)=new BinaryExpr(BINOP_LE,(yyvsp[-2].expr),(yyvsp[0].expr)); }
#line 2152 "parser.cpp" /* yacc.c:1646  */
    break;

  case 102:
#line 329 "parser.y" /* yacc.c:1646  */
    { (yyval.expr)=new BinaryExpr(BINOP_LT,(yyvsp[-2].expr),(yyvsp[0].expr)); }
#line 2158 "parser.cpp" /* yacc.c:1646  */
    break;

  case 103:
#line 330 "parser.y" /* yacc.c:1646  */
    { (yyval.expr)=new BinaryExpr(BINOP_EQUAL,(yyvsp[-2].expr),(yyvsp[0].expr)); }
#line 2164 "parser.cpp" /* yacc.c:1646  */
    break;

  case 104:
#line 331 "parser.y" /* yacc.c:1646  */
    { (yyval.expr)=new BinaryExpr(BINOP_UNEQUAL,(yyvsp[-2].expr),(yyvsp[0].expr)); }
#line 2170 "parser.cpp" /* yacc.c:1646  */
    break;

  case 105:
#line 332 "parser.y" /* yacc.c:1646  */
    { (yyval.expr)=(yyvsp[0].expr); }
#line 2176 "parser.cpp" /* yacc.c:1646  */
    break;

  case 106:
#line 335 "parser.y" /* yacc.c:1646  */
    { (yyval.expr)=new BinaryExpr(BINOP_PLUS,(yyvsp[-2].expr),(yyvsp[0].expr)); }
#line 2182 "parser.cpp" /* yacc.c:1646  */
    break;

  case 107:
#line 336 "parser.y" /* yacc.c:1646  */
    { (yyval.expr)=new BinaryExpr(BINOP_MINUS,(yyvsp[-2].expr),(yyvsp[0].expr)); }
#line 2188 "parser.cpp" /* yacc.c:1646  */
    break;

  case 108:
#line 337 "parser.y" /* yacc.c:1646  */
    { (yyval.expr)=new BinaryExpr(BINOP_OR,(yyvsp[-2].expr),(yyvsp[0].expr)); }
#line 2194 "parser.cpp" /* yacc.c:1646  */
    break;

  case 109:
#line 338 "parser.y" /* yacc.c:1646  */
    { (yyval.expr)=(yyvsp[0].expr); }
#line 2200 "parser.cpp" /* yacc.c:1646  */
    break;

  case 110:
#line 341 "parser.y" /* yacc.c:1646  */
    { (yyval.expr)=new BinaryExpr(BINOP_MUL,(yyvsp[-2].expr),(yyvsp[0].expr)); }
#line 2206 "parser.cpp" /* yacc.c:1646  */
    break;

  case 111:
#line 342 "parser.y" /* yacc.c:1646  */
    { (yyval.expr)=new BinaryExpr(BINOP_DIV,(yyvsp[-2].expr),(yyvsp[0].expr)); }
#line 2212 "parser.cpp" /* yacc.c:1646  */
    break;

  case 112:
#line 343 "parser.y" /* yacc.c:1646  */
    { (yyval.expr)=new BinaryExpr(BINOP_MOD,(yyvsp[-2].expr),(yyvsp[0].expr)); }
#line 2218 "parser.cpp" /* yacc.c:1646  */
    break;

  case 113:
#line 344 "parser.y" /* yacc.c:1646  */
    { (yyval.expr)=new BinaryExpr(BINOP_AND,(yyvsp[-2].expr),(yyvsp[0].expr)); }
#line 2224 "parser.cpp" /* yacc.c:1646  */
    break;

  case 114:
#line 345 "parser.y" /* yacc.c:1646  */
    { (yyval.expr)=(yyvsp[0].expr); }
#line 2230 "parser.cpp" /* yacc.c:1646  */
    break;

  case 115:
#line 348 "parser.y" /* yacc.c:1646  */
    { (yyval.expr)=(yyvsp[0].identifier); }
#line 2236 "parser.cpp" /* yacc.c:1646  */
    break;

  case 116:
#line 349 "parser.y" /* yacc.c:1646  */
    { (yyval.expr)=new FuncStmt((yyvsp[-3].identifier),(yyvsp[-1].args)); }
#line 2242 "parser.cpp" /* yacc.c:1646  */
    break;

  case 117:
#line 350 "parser.y" /* yacc.c:1646  */
    { (yyval.expr)=new SysFunct((yyvsp[0].sval)); }
#line 2248 "parser.cpp" /* yacc.c:1646  */
    break;

  case 118:
#line 351 "parser.y" /* yacc.c:1646  */
    { (yyval.expr)=new SysFunct((yyvsp[-3].sval),(yyvsp[-1].args)); }
#line 2254 "parser.cpp" /* yacc.c:1646  */
    break;

  case 119:
#line 352 "parser.y" /* yacc.c:1646  */
    { (yyval.expr)=(yyvsp[0].constVal); }
#line 2260 "parser.cpp" /* yacc.c:1646  */
    break;

  case 120:
#line 353 "parser.y" /* yacc.c:1646  */
    { (yyval.expr)=(yyvsp[-1].expr); }
#line 2266 "parser.cpp" /* yacc.c:1646  */
    break;

  case 121:
#line 354 "parser.y" /* yacc.c:1646  */
    { (yyval.expr)=new BinaryExpr(BINOP_XOR,new Boolean(true),(yyvsp[0].expr)); }
#line 2272 "parser.cpp" /* yacc.c:1646  */
    break;

  case 122:
#line 355 "parser.y" /* yacc.c:1646  */
    { (yyval.expr)=new BinaryExpr(BINOP_MINUS,new Integer(0),(yyvsp[0].expr)); }
#line 2278 "parser.cpp" /* yacc.c:1646  */
    break;

  case 123:
#line 356 "parser.y" /* yacc.c:1646  */
    { (yyval.expr)=new ArrayRef((yyvsp[-3].identifier),(yyvsp[-1].expr)); }
#line 2284 "parser.cpp" /* yacc.c:1646  */
    break;

  case 124:
#line 357 "parser.y" /* yacc.c:1646  */
    { (yyval.expr)=new RecordRef((yyvsp[-2].identifier),(yyvsp[0].identifier)); }
#line 2290 "parser.cpp" /* yacc.c:1646  */
    break;

  case 125:
#line 360 "parser.y" /* yacc.c:1646  */
    { (yyval.args)=(yyvsp[-2].args); (yyval.args)->push_back((yyvsp[0].expr)); }
#line 2296 "parser.cpp" /* yacc.c:1646  */
    break;

  case 126:
#line 361 "parser.y" /* yacc.c:1646  */
    { (yyval.args)=new vector<Expr*>(); (yyval.args)->push_back((yyvsp[0].expr)); }
#line 2302 "parser.cpp" /* yacc.c:1646  */
    break;

  case 127:
#line 364 "parser.y" /* yacc.c:1646  */
    { (yyval.identifier)=new Identifier((yyvsp[0].sval)); }
#line 2308 "parser.cpp" /* yacc.c:1646  */
    break;


#line 2312 "parser.cpp" /* yacc.c:1646  */
      default: break;
    }
  /* User semantic actions sometimes alter yychar, and that requires
     that yytoken be updated with the new translation.  We take the
     approach of translating immediately before every use of yytoken.
     One alternative is translating here after every semantic action,
     but that translation would be missed if the semantic action invokes
     YYABORT, YYACCEPT, or YYERROR immediately after altering yychar or
     if it invokes YYBACKUP.  In the case of YYABORT or YYACCEPT, an
     incorrect destructor might then be invoked immediately.  In the
     case of YYERROR or YYBACKUP, subsequent parser actions might lead
     to an incorrect destructor call or verbose syntax error message
     before the lookahead is translated.  */
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;

  /* Now 'shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
  if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTOKENS];

  goto yynewstate;


/*--------------------------------------.
| yyerrlab -- here on detecting error.  |
`--------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYEMPTY : YYTRANSLATE (yychar);

  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (YY_("syntax error"));
#else
# define YYSYNTAX_ERROR yysyntax_error (&yymsg_alloc, &yymsg, \
                                        yyssp, yytoken)
      {
        char const *yymsgp = YY_("syntax error");
        int yysyntax_error_status;
        yysyntax_error_status = YYSYNTAX_ERROR;
        if (yysyntax_error_status == 0)
          yymsgp = yymsg;
        else if (yysyntax_error_status == 1)
          {
            if (yymsg != yymsgbuf)
              YYSTACK_FREE (yymsg);
            yymsg = (char *) YYSTACK_ALLOC (yymsg_alloc);
            if (!yymsg)
              {
                yymsg = yymsgbuf;
                yymsg_alloc = sizeof yymsgbuf;
                yysyntax_error_status = 2;
              }
            else
              {
                yysyntax_error_status = YYSYNTAX_ERROR;
                yymsgp = yymsg;
              }
          }
        yyerror (yymsgp);
        if (yysyntax_error_status == 2)
          goto yyexhaustedlab;
      }
# undef YYSYNTAX_ERROR
#endif
    }



  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
         error, discard it.  */

      if (yychar <= YYEOF)
        {
          /* Return failure if at end of input.  */
          if (yychar == YYEOF)
            YYABORT;
        }
      else
        {
          yydestruct ("Error: discarding",
                      yytoken, &yylval);
          yychar = YYEMPTY;
        }
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:

  /* Pacify compilers like GCC when the user code never invokes
     YYERROR and the label yyerrorlab therefore never appears in user
     code.  */
  if (/*CONSTCOND*/ 0)
     goto yyerrorlab;

  /* Do not reclaim the symbols of the rule whose action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;      /* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
        {
          yyn += YYTERROR;
          if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYTERROR)
            {
              yyn = yytable[yyn];
              if (0 < yyn)
                break;
            }
        }

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
        YYABORT;


      yydestruct ("Error: popping",
                  yystos[yystate], yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", yystos[yyn], yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturn;

/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturn;

#if !defined yyoverflow || YYERROR_VERBOSE
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif

yyreturn:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  yystos[*yyssp], yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
#if YYERROR_VERBOSE
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
#endif
  return yyresult;
}
#line 367 "parser.y" /* yacc.c:1906  */




