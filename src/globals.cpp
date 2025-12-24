#include "globals.h"

llvm::LLVMContext TheContext;
llvm::Module TheModule;
llvm::IRBuilder<> Builder;
std::map<std::string, llvm::Value *> NamedValues;