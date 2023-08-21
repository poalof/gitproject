// IR生成
#include <iostream>
#include <vector>
#include <map>
#include <string>
#include "ast.h"
using namespace std;

extern unique_ptr<llvm::LLVMContext> TheContext; // 上下文环境
extern unique_ptr<llvm::IRBuilder<>> Builder;
extern unique_ptr<llvm::Module> TheModule;
//static map<string, llvm::AllocaInst*> NamedValues; // symbol table
vector<llvm::Function*> Functions; // 函数列表，用于嵌套的函数定义（找符号表）
map<string, MyArrayType*> arrayMap;
llvm::Function* mainFunction;
llvm::Function* print; // write/writeln
llvm::BasicBlock* labelBlock[500];

/// LogError* - These are little helper functions for error handling.
unique_ptr<Expr> LogError(const char *Str) 
{
    fprintf(stderr, "LogError: %s\n", Str);
    return nullptr;
}

llvm::Value *LogErrorV(const char *Str) 
{
    LogError(Str);
    return nullptr;
}

llvm::Function* createPrintf()
{
    vector<llvm::Type*> arg_types;
    arg_types.push_back(Builder->getInt8PtrTy());
    auto printf_type = llvm::FunctionType::get(Builder->getInt32Ty(), llvm::makeArrayRef(arg_types), true);
    auto func = llvm::Function::Create(printf_type, llvm::Function::ExternalLinkage, llvm::Twine("printf"), TheModule.get());
    func->setCallingConv(llvm::CallingConv::C);
    return func;
}

// function的符号表<name. Value>
llvm::Value *getValue(const string& name)
{
    // 先找function
    for(int i = Functions.size()-1; i >= 1; i--)
    {
        if(Functions[i]->getValueSymbolTable() != nullptr && Functions[i]->getValueSymbolTable()->lookup(name) != nullptr)
        {
			cout << name << " found in " << (string)Functions[i]->getName() << endl;
			//Functions[i]->getValueSymbolTable()->lookup(name)->getType()->print(llvm::outs()); cout << endl;
            return Functions[i]->getValueSymbolTable()->lookup(name);
        }
    }
    // 再找全局变量
    if(TheModule->getGlobalVariable(name)!=nullptr)
    {
		cout << name << " found in global" << endl;
		//TheModule->getGlobalVariable(name)->getType()->print(llvm::outs()); cout << endl;
        return TheModule->getGlobalVariable(name);
    }
    // 没找着
	cout << name << " not found"<<endl;
    return nullptr;
}

// 获取当前Function
llvm::Function* getCurrentFunction(llvm::IRBuilder<>& builder) 
{
    llvm::BasicBlock* currentBlock = builder.GetInsertBlock();
    llvm::Function* currentFunction = currentBlock->getParent();
    return currentFunction;
}

/// CreateEntryBlockAlloca - Create an alloca instruction in the entry block of
/// the function.  This is used for mutable variables etc.
llvm::AllocaInst *CreateEntryBlockAlloca(llvm::Function *TheFunction, llvm::StringRef VarName, llvm::Type* type) 
{
    llvm::IRBuilder<> TmpB(&TheFunction->getEntryBlock(), TheFunction->getEntryBlock().begin());
    return TmpB.CreateAlloca(type,nullptr,VarName);
}

// ast节点类型转llvm可表示类型
llvm::Type* ast2llvm(ASTType *astTy)
{
    llvm::Type* llvmTy = nullptr;
    switch (astTy->type)
    {
        case TYPE_ARRAY:
            if (astTy->arrayType->getRangeType()->type == TYPE_CONST_RANGE)
            {
                llvmTy = llvm::ArrayType::get(ast2llvm(astTy->arrayType->getElemType()), 
                        astTy->arrayType->getRangeType()->constRangeType->size());
            }
            else if(astTy->arrayType->getRangeType()->type == TYPE_ENUM_RANGE)
            {
                llvmTy = llvm::ArrayType::get(ast2llvm(astTy->arrayType->getElemType()), 
                        astTy->arrayType->getRangeType()->enumSubrangeType->size());
            }
            else
                LogErrorV("Invalid array range type");
            break;
        case TYPE_CONST_RANGE: 
        case TYPE_ENUM_RANGE: 
            llvmTy = Builder->getInt32Ty();
            break;
        case TYPE_BUILT_IN:
            switch (astTy->builtinType)
            {
                case TYPE_INTEGER: llvmTy = Builder->getInt32Ty(); break;
                case TYPE_REAL: llvmTy = Builder->getDoubleTy(); break;
                case TYPE_CHAR: llvmTy = Builder->getInt8Ty(); break;
                case TYPE_BOOLEAN: llvmTy = Builder->getInt1Ty(); break;
            }
            break;
        case TYPE_ENUM:
        case TYPE_RECORD:
        case TYPE_CUSTOM:
        case TYPE_NOTYPE: 
            llvmTy = Builder->getVoidTy();
            break;
        default:
            LogErrorV("Incorrect type");
    }
    return llvmTy;
}

// ast节点类型转llvm指针 用于传引用变量
llvm::Type* ast2llvmPtr(ASTType* type)
{
    if(type->type != TYPE_BUILT_IN)
    {
        LogErrorV("Unsupported type");
        return nullptr;
    }
    llvm::Type* llvmPtr = nullptr;
    switch (type->builtinType)
    {
        case TYPE_INTEGER: llvmPtr = llvm::Type::getInt32PtrTy(*TheContext); break;
        case TYPE_REAL: llvmPtr =  llvm::Type::getDoublePtrTy(*TheContext); break;
        case TYPE_CHAR: llvmPtr =  llvm::Type::getInt8PtrTy(*TheContext); break;
        case TYPE_BOOLEAN: llvmPtr =  llvm::Type::getInt1PtrTy(*TheContext); break;
    }
    return llvmPtr;
}

// 初始化常量
llvm::Constant* ASTType::initValue(ConstVal *v)
{
    vector<llvm::Constant*> element;
    llvm::ArrayType* arrayType;
    size_t size = 0;
    if (v != nullptr)
    {
        switch (type)
        {
            case TYPE_ARRAY:
                if (this->arrayType->getRangeType()->type == TYPE_CONST_RANGE)
                    size = this->arrayType->getRangeType()->constRangeType->size();
                else if (this->arrayType->getRangeType()->type == TYPE_ENUM_RANGE)
                    size = this->arrayType->getRangeType()->enumSubrangeType->size();
                else
                    LogErrorV("Invalid range type");
                for (int i = 0; i < size; i++) // size个元素
                {
                    element.push_back(this->arrayType->getElemType()->initValue(v));
                }
                arrayType = (llvm::ArrayType*)ast2llvm(this);
                return llvm::ConstantArray::get(arrayType, element);
            case TYPE_CONST_RANGE:
            case TYPE_ENUM_RANGE: return Builder->getInt32(v->getValue().ival);
            case TYPE_BUILT_IN:
                switch (builtinType)
                {
                    case TYPE_INTEGER: return Builder->getInt32(v->getValue().ival);
                    case TYPE_REAL: return llvm::ConstantFP::get(Builder->getDoubleTy(), v->getValue().dval);
                    case TYPE_CHAR: return Builder->getInt8(v->getValue().cval);
                    case TYPE_BOOLEAN: return Builder->getInt1(v->getValue().bval);
                }
                break;
            case TYPE_ENUM:
            case TYPE_RECORD:
            case TYPE_CUSTOM:
            case TYPE_NOTYPE: return nullptr;
        }
    }
    else
    {
        switch (type)
        {
            case TYPE_ARRAY:
                if (this->arrayType->getRangeType()->type == TYPE_CONST_RANGE)
                    size = this->arrayType->getRangeType()->constRangeType->size();
                else
                    size = this->arrayType->getRangeType()->enumSubrangeType->size();
                for (int i = 0; i < size; i++) // null
                {
                    element.push_back(this->arrayType->getElemType()->initValue());
                }
                arrayType = (llvm::ArrayType*)ast2llvm(this);
                return llvm::ConstantArray::get(arrayType, element);
            case TYPE_CONST_RANGE:
            case TYPE_ENUM_RANGE: return Builder->getInt32(0);
            case TYPE_BUILT_IN:
                switch (builtinType)
                {
                    case TYPE_INTEGER: return Builder->getInt32(0);
                    case TYPE_REAL: return llvm::ConstantFP::get(Builder->getDoubleTy(), 0.0);
                    case TYPE_CHAR: return Builder->getInt8(0);
                    case TYPE_BOOLEAN: return Builder->getInt1(0);
                }
                break;
            case TYPE_ENUM:
            case TYPE_RECORD:
            case TYPE_CUSTOM:
            case TYPE_NOTYPE: return nullptr;
        }
    }
}

llvm::Value *Routine::codeGen() 
{
    //Const part
    for (auto constDecl : *constPart)
        constDecl->codeGen();
    //Type part
    for (auto typeDecl : *typePart)
        typeDecl->codeGen();
    //Var part
    for (auto varDecl : *varPart)
        varDecl->codeGen();
    //Routine part
    for (auto routineDecl : *routinePart)
        routineDecl->codeGen();
    //Routine body
	cout << "start routine body: " << endl;
    routineBody->codeGen();
	cout << "end of routine body" << endl;
	return nullptr;
}

// 
llvm::Value *Identifier::codeGen() 
{
    // Look this variable up in the function.
    //llvm::AllocaInst *A = NamedValues[*name];
    llvm::Value* A = getValue(*name);
    if (!A)
        return LogErrorV("Unknown symbol name");
    // Load the value.
//    return Builder->CreateLoad(A->getType(), A, (*name).c_str());
	return Builder->CreateLoad(getValue(*name), *name);
}

// 
llvm::Value *Program::codeGen()
{
    // main func
    vector<llvm::Type*> argTypes;
    llvm::FunctionType * funcType = llvm::FunctionType::get(Builder->getVoidTy(), makeArrayRef(argTypes), false);
    mainFunction = llvm::Function::Create(funcType, llvm::GlobalValue::InternalLinkage, "main", TheModule.get());
    llvm::BasicBlock * basicBlock = llvm::BasicBlock::Create(*TheContext, "entry", mainFunction, 0);
    Builder->SetInsertPoint(basicBlock);
    
    Functions.push_back(mainFunction);
    
    //Create System functions
    print = createPrintf();
    //Code generate
    routine->setGlobal(); // 主过程前
    routine->codeGen();
    Builder->CreateRetVoid(); // main func返回
    
    Functions.pop_back();
    return nullptr;
}

llvm::Value *BinaryExpr::codeGen()
{
    llvm::Value *L = lhs->codeGen();
    llvm::Value *R = rhs->codeGen();
    if (!L || !R)
        return nullptr;

    bool flag = L->getType()->isDoubleTy() || R->getType()->isDoubleTy();
	if(flag && !L->getType()->isDoubleTy())
	{
		L = Builder->CreateSIToFP(L, Builder->getDoubleTy());
	}
	else if(flag && !R->getType()->isDoubleTy())
	{
		R = Builder->CreateSIToFP(R, Builder->getDoubleTy());
	}

    switch (op)
    {
        case BINOP_PLUS: return flag ? Builder->CreateFAdd(L, R, "addtmpf") : Builder->CreateAdd(L, R, "addtmpi");
        case BINOP_MINUS: return flag ? Builder->CreateFSub(L, R, "subtmpf") : Builder->CreateSub(L, R, "subtmpi");
        case BINOP_MUL: return flag ? Builder->CreateFMul(L, R, "multmpf") : Builder->CreateMul(L, R, "multmpi");
        case BINOP_DIV: return Builder->CreateSDiv(L, R, "tmpdiv");
        case BINOP_GE: return Builder->CreateICmpSGE(L, R, "tmpsge");
        case BINOP_GT: return Builder->CreateICmpSGT(L, R, "tmpsgt");
        case BINOP_LT: return Builder->CreateICmpSLT(L, R, "tmpslt");
        case BINOP_LE: return Builder->CreateICmpSLE(L, R, "tmpsle");
        case BINOP_EQUAL: return Builder->CreateICmpEQ(L, R, "tmpeq");
        case BINOP_UNEQUAL: return Builder->CreateICmpNE(L, R, "tmpne");
        case BINOP_OR: return Builder->CreateOr(L, R, "tmpor");
        case BINOP_MOD: return Builder->CreateSRem(L, R, "tmpsrem");
        case BINOP_AND: return Builder->CreateAnd(L, R, "tmpand");
        case BINOP_XOR: return Builder->CreateXor(L, R, "tmpxor");
        default:
            return LogErrorV("invalid binary operator");
    }
}

llvm::Value *ConstDecl::codeGen()
{
    string str = name->getName();
    llvm::Type* llvmTy = ast2llvm(type);
    
    if (getGlobal()) // 全局常量
	{
		cout << "CONST " << str << " defined in global" << endl;
         return new llvm::GlobalVariable(*TheModule, llvmTy, true, llvm::GlobalValue::ExternalLinkage, type->initValue(value), str);				
	}
    else // local, 
    {
		cout << "CONST " << str << " defined in " << (string)getCurrentFunction(*Builder)->getName() << endl;
        llvm::AllocaInst *Alloca = CreateEntryBlockAlloca(getCurrentFunction(*Builder), str, ast2llvm(type));
        return Builder->CreateStore(this->value->codeGen(), Alloca);
    }
}

llvm::Value *ConstRangeType::mapIndex(Expr *index)
{
    BinaryExpr* off = new BinaryExpr(BINOP_MINUS, index, lowBound);
    return off->codeGen();
}

// const value  ConstantInt getInt32(uint32_t)
llvm::Value *Integer::codeGen()
{
    return Builder->getInt32(val);
}

llvm::Value *Char::codeGen()
{
    return Builder->getInt8(val);
}

llvm::Value *Real::codeGen()
{
    return llvm::ConstantFP::get(*TheContext, llvm::APFloat(val)); //???
}

llvm::Value *Boolean::codeGen()
{
    return Builder->getInt1(val);
}

llvm::Value *EnumSubrangeType::codeGen() 
{
    lowValue  = low->codeGen();
    highValue = high->codeGen();
    lowValueAddr  = getValue(low->getName()); // 不是地址，得到的是low写入符号表中的Value，Value中可以得到？？
    highValueAddr = getValue(high->getName());
    size(); // 是否越界
    return nullptr;
}

llvm::Value *EnumSubrangeType::mapIndex(Expr *index)
{
    BinaryExpr* off = new BinaryExpr(BINOP_MINUS, index, low);
    return off->codeGen();
}

llvm::Value *ASTType::codeGen()
{
    switch (type)
    {
        case TYPE_ARRAY: this->arrayType->codeGen(); break;
        case TYPE_CONST_RANGE: this->constRangeType->codeGen(); break;
        case TYPE_ENUM_RANGE: this->enumSubrangeType->codeGen(); break;
        case TYPE_RECORD: this->recordType->codeGen(); break;
        case TYPE_ENUM: this->enumType->codeGen(); break;
        case TYPE_CUSTOM: this->customType->codeGen(); break;
        case TYPE_BUILT_IN:
        case TYPE_NOTYPE: break;
    }
    return (llvm::Value*)ast2llvm(this);
}

// ??
llvm::Value *FuncStmt::codeGen()
{
    setEntry();

    // Look up the name in the global module table.
    llvm::Function *F = TheModule->getFunction(name->getName());
    if (!F)
        return LogErrorV("Unknown function referenced");
    vector<llvm::Value *> ArgsV;
	llvm::Function::arg_iterator argIt = F->arg_begin();
	if(argIt != F->arg_end())
		for (auto arg : *args)
		{
		    if (argIt->getType()->isPointerTy())
		    {
		        llvm::Value *addr = getValue(dynamic_cast<Identifier*>(arg)->getName());
		        ArgsV.push_back(addr);
		    }
		    else
		        ArgsV.push_back(arg->codeGen());
		    argIt++;
		}
/*
    for (unsigned int i = 0, e = args->size(); i != e; ++i) 
    {
		if((*args)[i]->)
        ArgsV.push_back((*args)[i]->codeGen());
        if (!ArgsV.back())
            return nullptr;
    }
*/
	cout << "Call function " << (string)F->getName() << endl;
    llvm::Value* call = Builder->CreateCall(F, ArgsV, "calltmp");


    return call;
}

llvm::Value *ProcStmt::codeGen()
{
    setEntry();

    llvm::Function* function = TheModule->getFunction(name->getName()); // 根据函数名称获取对应的 LLVM 函数对象
    if (!function)
        return LogErrorV("Procedure undefine");

    vector<llvm::Value*> argsV; // 存储参数值的向量
    // 生成每个参数的 IR 代码
    llvm::Function::arg_iterator argIt = function->arg_begin();
	if(argIt != function->arg_end())
		for (auto arg : *args)
		{
		    if (argIt->getType()->isPointerTy())
		    {
		        llvm::Value *addr = getValue(dynamic_cast<Identifier*>(arg)->getName());
		        argsV.push_back(addr);
		    }
		    else
		        argsV.push_back(arg->codeGen());
		    argIt++;
		}

    // 创建过程调用指令
	cout << "Call procedure " << (string)function->getName() << endl;
    llvm::Value* result = Builder->CreateCall(function, argsV);

    return result;
}

// 没写
llvm::Value *SysFunct::codeGen()
{
    return nullptr;
}

llvm::Value *SysProc::codeGen()
{
    setEntry();

    llvm::Value *res = nullptr;
    switch (this->procedure) 
    {
        case SYS_READ: res = this->SysProcRead(); break;
        case SYS_WRITE: res = this->SysProcWrite(false); break;
        case SYS_WRITELN: cout << "Call printf" << endl; res = this->SysProcWrite(true); break;
        case SYS_ERROR_PROC: return LogErrorV("Unknown System Procedure");
    }


    return res;
}

llvm::Value *SysProc::SysProcWrite(bool isLineBreak)
{
    string formatStr = "";
    vector<llvm::Value*> params;
    for (auto arg : *(this->args))
    {
        llvm::Value* argValue = arg->codeGen();
    	//argValue->getType()->print(llvm::outs()); cout << endl;
		//Builder->getInt32Ty()->print(llvm::outs()); cout << endl;
		//getValue("k")->getType()->print(llvm::outs()); cout << endl;
        if (argValue->getType() == Builder->getInt32Ty()) // i32 -> i32* ??????
            formatStr += "%d";
        else if (argValue->getType() == Builder->getInt8Ty())
            formatStr += "%c";
        else if (argValue->getType() == Builder->getInt1Ty())
            formatStr += "%d";
        else if (argValue->getType()->isDoubleTy())
            formatStr += "%lf";
        else
            LogErrorV("Invalid type");
        params.push_back(argValue);
    }
    if (isLineBreak)
        formatStr += "\n";
    auto formatConst = llvm::ConstantDataArray::getString(*TheContext, formatStr.c_str());
    auto formatStrVar = new llvm::GlobalVariable(*(TheModule), llvm::ArrayType::get(Builder->getInt8Ty(), formatStr.size() + 1), true, llvm::GlobalValue::ExternalLinkage, formatConst, ".str");
    auto zero = llvm::Constant::getNullValue(Builder->getInt32Ty());
    llvm::Constant* indices[] = {zero, zero};
    auto varRef = llvm::ConstantExpr::getGetElementPtr(formatStrVar->getType()->getElementType(), formatStrVar, indices);
    params.insert(params.begin(), varRef);
    return Builder->CreateCall(print, llvm::makeArrayRef(params), "printf");
}

// 这个语言函数原型和定义放在一起
// Function 插入Module 传函数name和FunctionType
// FunctionType 传入返回值类型、参数列表、是否变参
llvm::Function *FuncDecl::codeGen() 
{
    if(TheModule->getFunction(name->getName()) != nullptr)
    {
        LogErrorV("Function redefine");
        return nullptr;
    }
	if(returnType->type == TYPE_NOTYPE)
		cout << "Declare procedure " << name->getName() << ":" << endl;
	else 
		cout << "Declare function " << name->getName() << ":" << endl;
    // 设置函数的参数类型
    vector<llvm::Type*> paramTypes;
    for (auto param : *paras) 
    {
        llvm::Type* paramType = nullptr;
        if(param->getRef()) // 引用
            paramType = ast2llvmPtr(param->getType());  // 根据 AST 类型获取 LLVM 类型
        else
            paramType = ast2llvm(param->getType());  // 根据 AST 类型获取 LLVM 类型
        paramTypes.insert(paramTypes.end(), param->getParaCnt(), paramType);
    }

    // 创建函数类型 
    llvm::Type* retType = ast2llvm(returnType);  // 根据 AST 类型获取 LLVM 类型
    llvm::FunctionType* funcType = llvm::FunctionType::get(retType, paramTypes, false);

    // 在模块中创建函数 添加符号表
    llvm::Function* function = llvm::Function::Create(funcType, llvm::Function::ExternalLinkage, name->getName(), TheModule.get());
    Functions.push_back(function);

    // 创建函数的基本块
    llvm::BasicBlock* entryBlock = llvm::BasicBlock::Create(*TheContext, "entry", function);
    Builder->SetInsertPoint(entryBlock);
    /*
    // Record the function arguments in the NamedValues map.
    NamedValues.clear();
    for (auto &Arg : TheFunction->args())
    {
        // Create an alloca for this variable.
        llvm::AllocaInst *Alloca = CreateEntryBlockAlloca(TheFunction, Arg.getName());

        // Store the initial value into the alloca.
        Builder->CreateStore(&Arg, Alloca);

        // Add arguments to variable symbol table.
        NamedValues[std::string(Arg.getName())] = Alloca;
    }

    // 将表达式计算到输入块中，并返回计算出的值
    if (Value *RetVal = Body->codegen()) {
    // Finish off the function.
    // 创建 LLVM ret instruction
    Builder->CreateRet(RetVal);
    */
    // 为参数分配空间
    llvm::Function::arg_iterator it =  function->arg_begin();
    for (auto & args : *paras) // para
    {
        for (auto & arg : *(args->names)) // id
        {
            if (args->getRef())
                llvm::Value* alloc = Builder->CreateGEP(it, Builder->getInt32(0), arg->getName());
            else
            {
                llvm::Value* alloc = CreateEntryBlockAlloca(function, arg->getName(), ast2llvm(args->getType()));
                Builder->CreateStore(it, alloc);
            }
            it++;
			cout << "arg name: " << arg->getName() << endl;
        }
    }
    // ret val 分配一个内存返回时读入
    llvm::Value *res = nullptr;
    if (returnType->type != TYPE_NOTYPE)
        res = CreateEntryBlockAlloca(function, name->getName(), ast2llvm(returnType));

    // 生成函数体的 LLVM IR 代码
    subRoutine->codeGen();

    // 在函数末尾添加返回语句
    if(returnType->type != TYPE_NOTYPE)
        Builder->CreateRet(name->codeGen());
    else
        Builder->CreateRetVoid();

    // 插入上一级bb末尾
    Functions.pop_back();
    Builder->SetInsertPoint(&Functions.back()->getBasicBlockList().back());
	cout << "End of declaration of " << name->getName() << endl;
    return function;
}

// 变量，entry block分配空间
llvm::Value *VarDecl::codeGen()
{
    llvm::Value* alloc = nullptr;
    llvm::Type* varType;
    for (auto id : *names)
    {
        if (type->type == TYPE_ARRAY)
            arrayMap[id->getName()] = type->arrayType;
        varType = (llvm::Type*)(type->codeGen());
        if (this->getGlobal())
		{
			cout << "VAR " << id->getName() << " defined in global, type: " << endl;
			varType->print(llvm::outs());
			cout << endl;
            alloc = new llvm::GlobalVariable(*TheModule, varType, false, llvm::GlobalValue::ExternalLinkage, type->initValue(), id->getName());
		}
        else
		{
			cout << "VAR " << id->getName() << " defined in " << (string)getCurrentFunction(*Builder)->getName() << ", type: " << endl;
			varType->print(llvm::outs());
			cout << endl;
            alloc = CreateEntryBlockAlloca(getCurrentFunction(*Builder), id->getName(), varType);
		}
    }

    return alloc;
}

llvm::Value *IfStmt::codeGen() 
{   
    setEntry();

    llvm::Value *CondV = cond->codeGen();
    llvm::Value *ThenV, *ElseV;
    if (!CondV)
        return nullptr;
    // 创建icmp one 指令，CondV = (CondV != 0.0)
    CondV = Builder->CreateICmpNE(CondV, llvm::ConstantInt::get(llvm::Type::getInt1Ty(*TheContext), 0, true), "ifcond");
    // 在每个 function 内，我们会创建一个 block，这里一定在这个 block 内
    // 根据block 得到对应的上层 function
    llvm::Function *TheFunction = getCurrentFunction(*Builder);
    // Create blocks for the then and else cases.  Insert the 'then' block at the
    // end of the function.
    // 为 then else 以及最后的 Merge 创建 block
    llvm::BasicBlock *ThenBB = llvm::BasicBlock::Create(*TheContext, "then", TheFunction);
    llvm::BasicBlock *ElseBB = llvm::BasicBlock::Create(*TheContext, "else", TheFunction);
    llvm::BasicBlock *MergeBB = llvm::BasicBlock::Create(*TheContext, "ifcont", TheFunction);
    // 创建跳转指令，根据 CondV 选择 Then的block或者Else的block
    llvm::Value *br = Builder->CreateCondBr(CondV, ThenBB, ElseBB);
    // Emit then value
    Builder->SetInsertPoint(ThenBB);
    // codegen Then block
    ThenV = thenStmt->codeGen();
    if (!ThenV) return nullptr;
    // 增加跳转 if(Merge)指令
    Builder->CreateBr(MergeBB);
    // Codegen of 'Then' can change the current block, update ThenBB for the PHI.
    // then 语句内可能会有嵌套的 if/then/else ，在嵌套的 codegen 时，
    // 会改变当前的 InsertBlock，我们需要有最终结果的那个 block 作为这里的then_block
    ThenBB = Builder->GetInsertBlock();
    // Emit else block.
    // 在这里才加入是为了让这个 block 位于上面的 then 里面嵌套 block 的后面
    //TheFunction->getBasicBlockList().push_back(ElseBB);
    // 与 then 类似
    Builder->SetInsertPoint(ElseBB);
    if(elseStmt!=nullptr) ElseV = this->elseStmt->codeGen();
    Builder->CreateBr(MergeBB);
    // codegen of 'Else' can change the current block, update ElseBB for the PHI.
    ElseBB = Builder->GetInsertBlock();
    // Emit merge block.
    // codegen Merge Block
    Builder->SetInsertPoint(MergeBB);
    return br;
//    TheFunction->getBasicBlockList().push_back(MergeBB);
}

llvm::Value *RepeatStmt::codeGen()
{
    this->setEntry();
    
    llvm::Function *TheFunction = getCurrentFunction(*Builder);    
    llvm::BasicBlock *loopBB = llvm::BasicBlock::Create(*TheContext, "loop", TheFunction);
	llvm::BasicBlock *condBB = llvm::BasicBlock::Create(*TheContext, "cond", TheFunction);
    llvm::BasicBlock *afterBB = llvm::BasicBlock::Create(*TheContext, "afterLoop", TheFunction);
    
    //Loop
    Builder->CreateBr(loopBB);
    Builder->SetInsertPoint(loopBB);
    for (auto stmt : *(this->stmts))
    {
        stmt->codeGen();
    }
    Builder->CreateBr(condBB);
    loopBB = Builder->GetInsertBlock();
    
    //Cond
    Builder->SetInsertPoint(condBB);
    llvm::Value *condValue = this->cond->codeGen();
    condValue = Builder->CreateICmpNE(condValue, llvm::ConstantInt::get(llvm::Type::getInt1Ty(*TheContext), 0, true), "repeateCond");
    auto branch = Builder->CreateCondBr(condValue, loopBB, afterBB);
    
    //After
    Builder->SetInsertPoint(afterBB);

    return branch;
}

// Output for-loop as:
//   ...
//   start = startexpr
//   goto loop
// loop:
//   variable = phi [start, loopheader], [nextvariable, loopend]
//   ...
//   bodyexpr
//   ...
// loopend:
//   step = stepexpr
//   nextvariable = variable + step
//   endcond = endexpr
//   br endcond, loop, endloop
// outloop:
llvm::Value *ForStmt::codeGen() 
{
    setEntry();

    // Make the new basic block for the loop header, inserting after current block.
    llvm::Function *TheFunction = Builder->GetInsertBlock()->getParent();
    // Create an alloca for the variable in the entry block.
    //llvm::AllocaInst *Alloca = CreateEntryBlockAlloca(TheFunction, name->getName(), ast2llvm(n)); 
    llvm::Value* startValue = this->begin->codeGen();
    llvm::Value* endValue = this->end->codeGen();
    llvm::Value *condValue = nullptr, *curValue = nullptr, *varValue = getValue(name->getName());
    Builder->CreateStore(startValue, varValue); // initial

    llvm::BasicBlock *condBB = llvm::BasicBlock::Create(*TheContext, "cond", TheFunction);
    llvm::BasicBlock *loopBB = llvm::BasicBlock::Create(*TheContext, "loop", TheFunction);
    llvm::BasicBlock *afterBB = llvm::BasicBlock::Create(*TheContext, "afterLoop", TheFunction);
    //Cond
    Builder->CreateBr(condBB);
    Builder->SetInsertPoint(condBB);
    curValue = name->codeGen();
    if (this->isAsc)
        condValue = Builder->CreateICmpSLE(curValue, endValue);
    else
        condValue = Builder->CreateICmpSGE(curValue, endValue);
    condValue = Builder->CreateICmpNE(condValue, llvm::ConstantInt::get(llvm::Type::getInt1Ty(*TheContext), 0, true), "forCond");
    auto branch = Builder->CreateCondBr(condValue, loopBB, afterBB);
    condBB = Builder->GetInsertBlock();
    //Loop
    Builder->SetInsertPoint(loopBB);
    this->stmt->codeGen();
    llvm::Value *nextValue = Builder->CreateAdd(curValue, Builder->getInt32(this->isAsc ? 1 : -1), "nextvar"); // update value
    Builder->CreateStore(nextValue, varValue);
    Builder->CreateBr(condBB);
    loopBB = Builder->GetInsertBlock();
    //After
    Builder->SetInsertPoint(afterBB);

    return branch;
}

llvm::Value *CaseStmt::codeGen()
{
    this->setEntry();
    
    llvm::Value *cmpValue = cond->codeGen(), *condValue = nullptr;
    llvm::Function *TheFunction = getCurrentFunction(*Builder);
    llvm::BasicBlock *afterBB = llvm::BasicBlock::Create(*TheContext, "afterCase", TheFunction);
    vector<llvm::BasicBlock*> switchBBs, caseBBs;
    for (int i = 1; i <= cases->size(); i++)
    {
        switchBBs.push_back(llvm::BasicBlock::Create(*TheContext, "switch", TheFunction));
        caseBBs.push_back(llvm::BasicBlock::Create(*TheContext, "case", TheFunction));
    }
    Builder->CreateBr(switchBBs[0]);
    for (int i = 0; i < this->cases->size(); i++)
    {
        //Switch
        Builder->SetInsertPoint(switchBBs[i]);
        BinaryExpr* cmp = new BinaryExpr(BINOP_EQUAL, cond, (*cases)[i]->getValue());
        condValue = cmp->codeGen();
        if (i < this->cases->size() - 1)
        {
            Builder->CreateCondBr(condValue, caseBBs[i], switchBBs[i + 1]);
        }
        else
        {
            Builder->CreateCondBr(condValue, caseBBs[i], afterBB);
        }
        
        //Case
        Builder->SetInsertPoint(caseBBs[i]);
        (*cases)[i]->codeGen();
        Builder->CreateBr(afterBB);
    }
    
    //After
    Builder->SetInsertPoint(afterBB);

    return nullptr;
}

llvm::Value *GotoStmt::codeGen()
{
    this->setEntry();

    llvm::Value *res = nullptr;
    if (labelBlock[label] == nullptr)
    {
        labelBlock[label] = llvm::BasicBlock::Create(*TheContext, "Label_" + to_string(label), getCurrentFunction(*Builder));
    }
    res = Builder->CreateBr(labelBlock[label]);

    return res;
}

llvm::Value *AssignStmt::codeGen()
{
    this->setEntry();

    llvm::Value *res = nullptr;
    switch (this->type)
    {
        case ASSIGN_SIMPLE: res = Builder->CreateStore(this->exp->codeGen(), getValue(this->name->getName())); break;
        case ASSIGN_ARRAY:  res = Builder->CreateStore(this->exp->codeGen(), (new ArrayRef(name, arrayIndex))->getReference()); break;
        case ASSIGN_RECORD: res = nullptr; break;
    }

    return res;
}

llvm::Value *WhileStmt::codeGen()
{
    this->setEntry();

    llvm::Function *TheFunction = getCurrentFunction(*Builder);
    llvm::BasicBlock *condBB = llvm::BasicBlock::Create(*TheContext, "cond", TheFunction);
    llvm::BasicBlock *loopBB = llvm::BasicBlock::Create(*TheContext, "loop", TheFunction);
    llvm::BasicBlock *afterBB = llvm::BasicBlock::Create(*TheContext, "afterLoop", TheFunction);
    
    //Cond
    Builder->CreateBr(condBB);
    Builder->SetInsertPoint(condBB);
    llvm::Value *condValue = this->cond->codeGen();
    condValue = Builder->CreateICmpNE(condValue, llvm::ConstantInt::get(llvm::Type::getInt1Ty(*TheContext), 0, true), "whileCond");
    auto branch = Builder->CreateCondBr(condValue, loopBB, afterBB);
    condBB = Builder->GetInsertBlock();
    
    //Loop
    Builder->SetInsertPoint(loopBB);
    stmt->codeGen();
    Builder->CreateBr(condBB); // loop body 执行完回到条件判断
    
    //After
    Builder->SetInsertPoint(afterBB);

    return branch;
}

llvm::Value *CompoundStmt::codeGen()
{
    this->setEntry();

    llvm::Value *lastValue = nullptr;
    for (auto stmt : *(this->stmts))
        lastValue = stmt->codeGen();

    return lastValue;
}

llvm::Value *ArrayRef::codeGen()
{
    return Builder->CreateLoad(this->getReference(), "arrref");
}

llvm::Value *ArrayRef::getReference()
{
    string name = array->getName();
    llvm::Value* arrayValue = getValue(name), *indexValue;
    if (arrayMap[name]->getRangeType()->type == TYPE_CONST_RANGE)
        indexValue = arrayMap[name]->getRangeType()->constRangeType->mapIndex(this->index);
    else
        indexValue = arrayMap[name]->getRangeType()->enumSubrangeType->mapIndex(this->index);
    
    vector<llvm::Value*> indexList;
    indexList.push_back(Builder->getInt32(0));
    indexList.push_back(indexValue);
    return Builder->CreateInBoundsGEP(arrayValue, llvm::ArrayRef<llvm::Value*>(indexList));
}

// has label block, add a label
// 将builder指向label，进行当前stmt指令生成
void Stmt::setEntry()
{
    llvm::Function *TheFunction = getCurrentFunction(*Builder);
    if (this->label >= 0)
    {
        if (labelBlock[label] == nullptr)
        {
            labelBlock[label] = llvm::BasicBlock::Create(*TheContext, "Label_" + to_string(label), TheFunction);
        }
        Builder->CreateBr(labelBlock[this->label]);
        Builder->SetInsertPoint(labelBlock[this->label]);
    }
}

llvm::Value *MyArrayType::codeGen()
{ 
	return range->codeGen(); 
}
