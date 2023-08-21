
class Node;
class Expr;
class Stmt;
class Integer;
class Real;
class Char;
class Boolean;
class ConstVal;
class Identifier;
class ConstDecl;
class TypeDecl;
class VarDecl;
class ASTType;
class EnumType;
class MyArrayType;
class RecordType;
class ConstRangeType;
class EnumSubrangeType;
class FieldDecl;
class FuncDecl;
class Parameter;
class Routine;
class Program;
class AssignStmt;
class BinaryExpr;
class ArrayRef;
class RecordRef;
class FuncStmt;
class SysFunct;
class ProcStmt;
class SysProc;
class IfStmt;
class RepeatStmt;
class WhileStmt;
class ForStmt;
class CaseStmt;
class CaseExpr;
class GotoStmt;
class CompoundStmt;

// 二元运算符
enum BINOP
{
    BINOP_PLUS,
    BINOP_MINUS,
    BINOP_MUL,
    BINOP_DIV,
    BINOP_GE,
    BINOP_GT,
    BINOP_LT,
    BINOP_LE,
    BINOP_EQUAL,
    BINOP_UNEQUAL,
    BINOP_AND,
    BINOP_OR,
    BINOP_XOR,
    BINOP_MOD
};

// sys funct
enum SysFunctName 
{
    SYS_ABS, 
    SYS_CHR, 
    SYS_ODD, 
    SYS_ORD, 
    SYS_PRED, 
    SYS_SQR, 
    SYS_SQRT, 
    SYS_SUCC,
    SYS_ERROR_FUNCT
};

// sys procedure
enum SysProcName
{
    SYS_WRITE,
    SYS_WRITELN,
    SYS_READ,
    SYS_ERROR_PROC
};

// assign操作的类型
enum AssignType
{
    ASSIGN_SIMPLE,
    ASSIGN_ARRAY,
    ASSIGN_RECORD
};

// 语法规则中的所有可定义类型
enum Types 
{
    TYPE_BUILT_IN,
    TYPE_ENUM,
    TYPE_ENUM_RANGE,
    TYPE_ARRAY,
    TYPE_CONST_RANGE,
    TYPE_RECORD,
    TYPE_CUSTOM, // 用户自定义 name = costom_type(NAME)
    TYPE_NOTYPE,  // 类型为空
    TYPE_ERROR
};

// 内置类型，为const token赋值
enum BuiltinType
{
    TYPE_INTEGER,
    TYPE_REAL,
    TYPE_CHAR,
    TYPE_BOOLEAN
};

// 内置类型的值
union BuiltinValue
{
    int ival;
    double dval;
    char cval;
    bool bval;
};

