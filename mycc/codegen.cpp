#include <fstream>
#include <llvm/Support/FileSystem.h>
#include "ast.h"
#include "parser.hpp"
using namespace std;

extern int yyparse();
extern Program *root;
extern llvm::Function *mainFunction;

unique_ptr<llvm::LLVMContext> TheContext; // 上下文环境
unique_ptr<llvm::IRBuilder<>> Builder; // IR生成器
unique_ptr<llvm::Module> TheModule; // Module 顶级模块

int main(int argc, char **argv) 
{
    cout << "Parsing..." << endl;
    yyparse();
    cout << "Parsing done!" << endl;
    TheContext = make_unique<llvm::LLVMContext>(); // 上下文环境
    Builder = make_unique<llvm::IRBuilder<>>(*TheContext); // IR生成器
    TheModule = make_unique<llvm::Module>("main", *TheContext); // Module 顶级模块
    
    llvm::InitializeNativeTarget();
    llvm::InitializeNativeTargetAsmPrinter();
    llvm::InitializeNativeTargetAsmParser();
    cout << "Generating IR..." << endl;
    root->codeGen();
    // 将模块的 IR 代码输出到标准错误流
    TheModule->print(llvm::errs(), nullptr);
    cout << "Generating done!" << endl;
    
    return 0;
}
