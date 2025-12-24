#pragma once

#include <map>
#include <memory>
#include <string>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Module.h>

extern llvm::LLVMContext TheContext;
extern llvm::Module TheModule;
extern llvm::IRBuilder<> Builder;
extern std::map<std::string, llvm::Value *> NamedValues;