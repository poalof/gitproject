#pragma once
// ast.h ast节点类型定义
#include <iostream>
#include <memory>
#include <string.h>
#include <stdlib.h>
#include <vector>
#include <llvm/IR/BasicBlock.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/Function.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/CallingConv.h>
#include <llvm/IR/IRPrintingPasses.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/GlobalVariable.h>
#include <llvm/IRReader/IRReader.h>
#include <llvm/IR/ValueSymbolTable.h>
#include <llvm/Support/SourceMgr.h>
#include <llvm/Support/ManagedStatic.h>
#include <llvm/Support/TargetSelect.h>
#include <llvm/Support/MemoryBuffer.h>
#include <llvm/Support/raw_ostream.h>
#include <llvm/Support/DynamicLibrary.h>
#include <llvm/Target/TargetMachine.h>
#include <llvm/IR/Value.h>
#include "misc.h"
#define NO_LABEL -1

using namespace std;

// ast的节点
class Node
{
public:
    virtual llvm::Value *codeGen() = 0;
    virtual ~Node() {}
};

// 返回值的表达式
class Expr: public Node
{ };

// 语句类，有些含有一个LABEL(address)
class Stmt: public Node
{
public:
    int getLabel() { return label; }
    void setLabel(int l) { label = l; }

    void setEntry();

private:
    int label = NO_LABEL;
};

// program 程序，最顶层，包含程序名ID和一个routine
class Program: public Node
{
public:
    Program(string *name, Routine * routine): name(name), routine(routine) {}
    virtual llvm::Value *codeGen() override;
private:
    string  *name;
    Routine *routine;
};

// ID 标识符，保存一个名称，继承自Expr，有一个值(VAR/CONST)
// 声明的变量或者常量或者函数名
class Identifier: public Expr
{
public:
    Identifier(string* name): name(name) {}
    string getName() { return *name; }
    virtual llvm::Value *codeGen() override;

private:
    string * name;
};

// ------------------------- const声明 ------------------------------------
// const_value 常量: INTEGER|REAL|CHAR|SYS_CON(bool,maxint)
// 子类继承自此类
class ConstVal: public Expr
{
public:
    virtual BuiltinType getType() = 0;
    virtual BuiltinValue getValue() = 0;

    virtual ConstVal *operator-() = 0;  //?????
    virtual bool isValidConstRangeType()
    {
        BuiltinType t = getType();
        return t == TYPE_INTEGER || t == TYPE_CHAR;
    }
};

// int
class Integer: public ConstVal
{
public:
    Integer(int val): val(val) {};
    virtual BuiltinType getType() { return TYPE_INTEGER; }
    virtual BuiltinValue getValue() override
    { 
        BuiltinValue bv;
        bv.ival = val;
        return bv;
    }
    virtual ConstVal* operator-() { return new Integer(-val); }
    virtual llvm::Value *codeGen() override;
private:
    int val;
};

// real
class Real: public ConstVal
{
public:
    Real(double val): val(val) {};
    virtual BuiltinType getType() override { return TYPE_REAL; }
    virtual BuiltinValue getValue() override
    { 
        BuiltinValue bv;
        bv.dval = val;
        return bv;
    }
    
    virtual ConstVal* operator-() { return new Real(-val); }
    virtual llvm::Value *codeGen() override;
private:
    double val;
};

// char
class Char: public ConstVal
{
public:
    Char(char val): val(val) {};
    virtual BuiltinType getType() override { return TYPE_CHAR; }
    virtual BuiltinValue getValue() override
    { 
        BuiltinValue bv;
        bv.cval = val;
        return bv;
    }
    
    virtual ConstVal* operator-() { return new Char(-val); }
    virtual llvm::Value *codeGen() override;
private:
    char val;
};

// bool
class Boolean: public ConstVal
{
public:
    Boolean(bool val): val(val) {};
    virtual BuiltinType getType() override { return TYPE_BOOLEAN; }
    virtual BuiltinValue getValue() override
    { 
        BuiltinValue bv;
        bv.bval = val;
        return bv;
    }
    
    virtual ConstVal* operator-() { return new Boolean(!val); }
    virtual llvm::Value *codeGen() override;
private:
    bool val;
};

// ------------------------- type声明 ----------------------------------------
// type_decl 单条类型声明 TYPE NAME = type;
class TypeDecl : public Stmt {
public:
    virtual llvm::Value *codeGen() override { return nullptr; }
    TypeDecl(Identifier *name, ASTType *type): name(name), type(type){};
    
private:
    Identifier *name;
    ASTType *type;
};

// enum枚举类型，(name1,name2,namen)
class EnumType: public Stmt
{
public:
    EnumType(vector<Identifier*> *names) : enumVal(names) { }
    virtual llvm::Value *codeGen() override { return nullptr; }
    
private:
    vector<Identifier*>* enumVal;
};

// enum子界类型，name1..namen
class EnumSubrangeType: public Stmt
{
public:
    EnumSubrangeType(Identifier *lb, Identifier *ub): low(lb), high(ub) { }
    virtual llvm::Value *codeGen() override;
    llvm::Value *mapIndex(Expr* index);
    
    // load value from memory address
    int64_t getActualValue(llvm::Value *v)
    {
        llvm::Constant *constValue = llvm::cast<llvm::GlobalVariable>(v)->getInitializer();
        llvm::ConstantInt *constInt = llvm::cast<llvm::ConstantInt>(constValue);
        return constInt->getSExtValue();
    }
    
    size_t size()
    {
        int64_t lowIdx = 1, highIdx = 0;
        if (lowValue->getType() == highValue->getType())
        {
            lowIdx  = getActualValue(lowValueAddr); // enum num
            highIdx = getActualValue(highValueAddr); // 
            if (lowIdx > highIdx)
                throw std::range_error("[ERROR]low > highIdx.");
        }
        else
            throw std::domain_error("[ERROR]Invalid range type.");
        return highIdx - lowIdx + 1;
    }

private:
    Identifier *low; // 上下界标签
    Identifier *high;
    llvm::Value *lowValue, *highValue, *lowValueAddr, *highValueAddr;
};

// const range, int | double | char
class ConstRangeType : public Stmt
{
public:
    ConstRangeType(ConstVal *lowBound, ConstVal *upBound) : lowBound(lowBound), upBound(upBound) { }
    virtual llvm::Value *codeGen() override { size(); return nullptr; }
    size_t size()
    {
        int s;
        if (lowBound->getType() == upBound->getType() && lowBound->isValidConstRangeType())
        {
            if (lowBound->getType() == TYPE_INTEGER)
                s = upBound->getValue().ival - lowBound->getValue().ival + 1;
            else // char / byte
                s = upBound->getValue().cval - lowBound->getValue().cval + 1;
            if (s <= 0)
                throw range_error("ERROR::Lower bound exceeds upper bound");
        }
        else
            throw domain_error("ERROR::Invalid range type");
        return s;
    }
    // index偏移量
    llvm::Value *mapIndex(Expr* indexValue);

private:
    ConstVal *lowBound;
    ConstVal *upBound;
};

// array 数组 array[..] of type_decl
// range可以是除real之外的所有类型
class MyArrayType : public Stmt 
{
public:
    MyArrayType(ASTType *range, ASTType *type): range(range),type(type){ }
    ASTType* getRangeType() { return range; }
    ASTType* getElemType() { return type; }
    virtual llvm::Value *codeGen() override;

private:
    ASTType *range; // const range or enum subrange
    ASTType *type;
};

// record 结构 name1, name2: type_decl; name3: type_decl;
class RecordType : public Stmt
{
public:
    RecordType(vector<FieldDecl*>* fl) : fields(fl) { }
    virtual llvm::Value *codeGen() override { return nullptr; }
    
private:
    vector<FieldDecl*>* fields;
};

// record 的类型声明 name,name1: type_decl;
class FieldDecl: public Stmt
{
public:
    FieldDecl(vector<Identifier*> *nameList, ASTType *td) : names(nameList), type(td) { }
    virtual llvm::Value *codeGen() override { return nullptr; }
    
private:
    vector<Identifier*>* names;
    ASTType *type;
};

// ast 可以定义的所有类型，包括 
// built-in(int|double|char|bool) | enum/enum_subrange | array(enumrange/constrange) | record | name
class ASTType: public Stmt
{
public:
    MyArrayType *arrayType;
    RecordType *recordType;
    EnumType *enumType;
    ConstRangeType *constRangeType;
    EnumSubrangeType *enumSubrangeType;
    BuiltinType builtinType;
    Identifier *customType;

    Types type; // 该对象的实际类型，访问时根据枚举值得到实际类型的指针

    ASTType(MyArrayType *at) : arrayType(at), type(TYPE_ARRAY) {}
    ASTType(RecordType *rt) : recordType(rt), type(TYPE_RECORD) {}
    ASTType(EnumType *et) : enumType(et), type(TYPE_ENUM) { }
    ASTType(ConstRangeType *crt) : constRangeType(crt), type(TYPE_CONST_RANGE) { }
    ASTType(EnumSubrangeType *ert) : enumSubrangeType(ert), type(TYPE_ENUM_RANGE) { }
    ASTType(BuiltinType bt) : builtinType(bt), type(TYPE_BUILT_IN) { }
    ASTType(Identifier *udt) : customType(udt), type(TYPE_CUSTOM) { }
    ASTType() : type(TYPE_NOTYPE) {}

    virtual llvm::Value *codeGen() override;
    llvm::Constant* initValue(ConstVal *v = nullptr);
};

// const_decl 单条常量声明 CONST ID = value;
class ConstDecl: public Stmt
{
public:
    ConstDecl(Identifier *name, ConstVal *val) : name(name), value(val) 
    {
        type = new ASTType(value->getType());
        isGlobal = false; 
    }
    void setGlobal(bool flag) { isGlobal = flag; }
    bool getGlobal() { return isGlobal; }

    virtual llvm::Value *codeGen() override;
private:
    Identifier* name;
    ConstVal* value;
    ASTType* type; // 可能的类型只有 built-in 的 int char bool real
    bool isGlobal;
};

// ------------------------ var声明 -----------------------------------
// val_decl 单条变量类型声明，可能有多个变量为同一类型 VAR NAME1, NAME2: type;
// NameList: 声明类型的变量名集合
class VarDecl: public Stmt
{
public:
    VarDecl(vector<Identifier*>* names, ASTType *type) : names(names), type(type) {}
    void setGlobal(bool flag) { isGlobal = flag; }
    bool getGlobal() { return isGlobal; }
    virtual llvm::Value *codeGen() override;
private:
    vector<Identifier*>* names;
    ASTType*  type;
    bool isGlobal = false;
};

// --------------------------- function/procedure声明 -----------------------
// routine_part: 函数声明 head; sub_routine;
// head: FUNCTION NAME (parameters): type
// sub_routine: Routine类

class FuncDecl: public Stmt
{
public:
    FuncDecl(Identifier* name, vector<Parameter*>* paras, ASTType* type): name(name), paras(paras), returnType(type){}
    FuncDecl(Identifier* name, vector<Parameter*>* paras): name(name), paras(paras), returnType(new ASTType()){}
    void setRoutine(Routine *routine) { subRoutine = routine; }
    virtual llvm::Function *codeGen() override;
    
private:
    Identifier* name;
    vector<Parameter*>*  paras;
    ASTType*    returnType;
    Routine*    subRoutine;
};
/*
// 过程声明，和function不同的是没有返回值
class ProcDecl: public Stmt
{
public:
    ProcDecl(Identifier* name, vector<Parameter*>* paras): name(name), paras(paras) {}
    void setRoutine(Routine *routine) { subRoutine = routine; }
    virtual llvm::Value *codeGen(CodeGenerator & generator) override;
    
private:
    Identifier* name;
    vector<Parameter*>*   paras;
    Routine*    subRoutine;
};
*/
// 一个参数声明 VAR name: type; | name1, name2: type
// VAR 传入变量地址，name传入变量值
class Parameter : public Stmt
{
public:
    Parameter(vector<Identifier*> *nl, bool isRef, ASTType *t=nullptr) : names(nl), type(t), isRef(isRef) { }
    void setType(ASTType *t) { type = t; }
    bool getRef() { return isRef; }
    ASTType* getType() { return type; }
    size_t getParaCnt() { return names->size(); }
    virtual llvm::Value *codeGen() override { return nullptr; }
    vector<Identifier*>* names;

private:
    bool isRef;

    ASTType *type;
};

// --------------------------- routine_body stmt部分 ----------------------
// compound_stmt stmt列表，构成routine_body
// 实际上复合语句看作一个stmt，有作用域BEGIN-END
class CompoundStmt: public Stmt
{
public:
    CompoundStmt(vector<Stmt*>* s): stmts(s) {}
    virtual llvm::Value *codeGen() override;

private:
    vector<Stmt*>* stmts;
};

// assign 赋值语句 ID ASSIGN Expr | ID[expr] ASSIGN expr | ID.ID ASSIGN expr
class AssignStmt: public Stmt
{
public:
    AssignStmt(Identifier* name, Expr* exp): name(name), exp(exp), type(ASSIGN_SIMPLE) {}
    AssignStmt(Identifier* name, Expr* idx, Expr* exp): name(name), arrayIndex(idx),exp(exp), type(ASSIGN_ARRAY){}
    AssignStmt(Identifier* name, Identifier* elem, Expr* exp): name(name), recordElem(elem),exp(exp), type(ASSIGN_RECORD) {}
    virtual llvm::Value *codeGen() override;

private:
    AssignType  type;
    Identifier* name;
    Expr*       arrayIndex;
    Identifier* recordElem;
    Expr*       exp;
};

// if 条件分支语句 IF Expr THEN stmt [ELSE stmt]
class IfStmt: public Stmt
{
public:
    IfStmt(Expr* cond, Stmt* t, Stmt* e): cond(cond), thenStmt(t), elseStmt(e){}
    virtual llvm::Value *codeGen() override;
    
private:
    Expr* cond;
    Stmt* thenStmt;
    Stmt* elseStmt;
};

// repeat 循环语句 REPEAT stmt_list UNTIL expr
class RepeatStmt: public Stmt
{
public:
    RepeatStmt(vector<Stmt*>* stmts, Expr* cond): stmts(stmts), cond(cond){}
    virtual llvm::Value *codeGen() override;
    
private:
    vector<Stmt*>* stmts;
    Expr*     cond;
};

// while 循环语句 WHILE Expr DO stmt
class WhileStmt: public Stmt
{
public:
    WhileStmt(Expr* cond, Stmt* stmt): cond(cond), stmt(stmt){}
    virtual llvm::Value *codeGen() override;
    
private:
    Stmt*     stmt;
    Expr*     cond;
};

// for 循环语句 FOR ID ASSIGN Expr direction Expr DO stmt
// FOR i := 1 TO 10 DO ...
class ForStmt: public Stmt
{
public:
    ForStmt(Identifier* id, Expr* b, Expr* e, bool in, Stmt* stmt)
    : name(id), begin(b), end(e), isAsc(in), stmt(stmt) {}
    virtual llvm::Value *codeGen() override;
    
private:
    Identifier* name;
    Expr*       begin;
    Expr*       end;
    bool        isAsc; // to or downto
    Stmt*       stmt;
};

// case 选择语句 CASE Expr OF const: stmt; ID: stmt; END
class CaseStmt: public Stmt
{
public:
    CaseStmt(Expr* cond, vector<CaseExpr*>* cases): cond(cond),cases(cases) {}
    virtual llvm::Value *codeGen() override;
    
private:
    Expr*              cond;
    vector<CaseExpr*>* cases;
};

class CaseExpr: public Stmt
{
public:
    CaseExpr(Expr *value, Stmt* stmt) : value(value), stmt(stmt) { }
    Expr* getValue() { return value; }
    virtual llvm::Value *codeGen() override { return stmt->codeGen(); }

private:
    Expr* value;
    Stmt* stmt;
};

// goto 跳转语句 GOTO INTEGER(label)
class GotoStmt: public Stmt
{
public:
    GotoStmt(int label): label(label) {}
    virtual llvm::Value *codeGen() override;
    
private:
    int label;
};

// ---------------------------- proc_stmt 函数/过程调用 --------------------
/* 
   ID | ID(args_list) | SYS_PROC | SYS_PROC(expr_list) | READ(factor)
*/
// 函数调用
class FuncStmt: public Stmt, public Expr
{
public:
    FuncStmt(Identifier *name) : name(name), args(nullptr) { }
    FuncStmt(Identifier *name, vector<Expr*>* args) : name(name), args(args) { }
    virtual llvm::Value *codeGen() override;
private:
    Identifier* name;
    vector<Expr*>* args;
};

// 过程调用
class ProcStmt : public Stmt
{
public:
    ProcStmt(Identifier *name): name(name), args(nullptr) { }
    ProcStmt(Identifier *name, vector<Expr*>* args): name(name), args(args) { }
    virtual llvm::Value *codeGen() override;
    
private:
    Identifier *name;
    vector<Expr*>* args;
};

// sys_funct sys函数
class SysFunct: public Expr, public Stmt
{
public:
    SysFunct(string *name) : function(getFunctName(name)), name(name) { }
    SysFunct(string *name, vector<Expr*> *args) : function(getFunctName(name)), args(args), name(name) { }
    virtual llvm::Value *codeGen() override;
    
private:
    SysFunctName getFunctName(string *name) 
    {
        string &sname = *name;
        if(sname == "abs")
            return SYS_ABS;
        else if(sname == "chr")
            return SYS_CHR;
        else if(sname == "odd")
            return SYS_ODD;
        else if(sname == "ord")
            return SYS_ORD;
        else if(sname == "pred")
            return SYS_PRED;
        else if(sname == "sqr")
            return SYS_SQR;
        else if(sname == "sqrt")
            return SYS_SQRT;
        else if(sname == "succ")
            return SYS_SUCC;
        else
            return SYS_ERROR_FUNCT;
    }
    SysFunctName function;
    vector<Expr*>* args;
    string *name;
};

// SYS_PROC write | writeln | read    (expr_list)
class SysProc: public Stmt
{
public:
    SysProc(string *name) : procedure(getProcName(name)), name(name) { }
    SysProc(string *name, vector<Expr*>* args) : procedure(getProcName(name)), args(args), name(name) { }
    SysProc(string *name, Expr* expr) : procedure(getProcName(name)), args(new vector<Expr*>()), name(name) 
    {
        args->push_back(expr); // 一个参数
    }
    virtual llvm::Value *codeGen() override;
    
    llvm::Value *SysProcWrite(bool isLineBreak) ;
    llvm::Value *SysProcRead() { setEntry(); return nullptr; }
private:
    SysProcName getProcName(string *name) 
    {
        string &sname = *name;
        if(sname == "write")
            return SYS_WRITE;
        else if(sname == "writeln")
            return SYS_WRITELN;
        else if(sname == "read")
            return SYS_READ;
        else
            return SYS_ERROR_PROC;
    }
    SysProcName procedure; // sys过程名类型
    vector<Expr*> *args; 
    string *name; 
};

// ------------------------------- 其余expr定义 ------------------------------
// BINOP 二元expr LHS OP RHS
class BinaryExpr: public Expr
{
public:
    BinaryExpr(BINOP op, Expr* lhs, Expr* rhs): op(op), lhs(lhs), rhs(rhs) {}
    virtual llvm::Value *codeGen() override;
private:
    Expr* lhs;
    Expr* rhs;
    BINOP op;
};

// array下标访问，或assign
class ArrayRef: public Expr
{
public:
    ArrayRef(Identifier *array, Expr *index) : array(array), index(index) { }
    virtual llvm::Value *codeGen() override;
    
    llvm::Value *getReference();
    llvm::Type *getElementType();
private:
    Identifier *array;
    Expr *index;
};

// record成员访问，或assign
class RecordRef: public Expr
{
public:
    RecordRef(Identifier *record, Identifier *field) : record(record), field(field) { }
    virtual llvm::Value *codeGen() override { return nullptr; }
    
private:
    Identifier *record;
    Identifier *field;
};

/* routine 过程，program主体，包括：
head部分
 const_part   常量声明 CONST NAME = value;
 type_part    类型声明 TYPE 
 var_part     变量声明
 routine_part 函数声明和过程声明，包括sub_routine
这些声明貌似是严格按顺序排列的，可以直接parse得到
body部分
 stmt_list    语句列表

主函数中定义的变量为全局变量，局部过程为局部变量
*/
class Routine: public Node
{
public:
    Routine(vector<ConstDecl*>* constPart, vector<TypeDecl*>* typePart, vector<VarDecl*>* varPart, 
    vector<FuncDecl*>* funcPart)
    : constPart(constPart), typePart(typePart), varPart(varPart), routinePart(funcPart)
    { routineBody = nullptr; }
    void setRoutineBody(CompoundStmt* stmt) { routineBody = stmt; }
    virtual llvm::Value *codeGen() override;
    void setGlobal()
    {
        for (auto &constDecl : *constPart)
            constDecl->setGlobal(true);
        for (auto &varDecl : *varPart)
            varDecl->setGlobal(true);
    }

private:
    vector<ConstDecl*>*    constPart; // decl
    vector<TypeDecl*>*     typePart;
    vector<VarDecl*>*      varPart;
    vector<FuncDecl*>*     routinePart; // 方便parse
    //vector<ProcDecl*>*     procPart;
    CompoundStmt* routineBody; // body
};
