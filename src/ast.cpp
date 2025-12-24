#include "ast.h"
#include "globals.h"

// LLVM includes ONLY in .cpp
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/Constants.h>
#include <llvm/IR/Verifier.h>
#include <llvm/IR/BasicBlock.h>
#include <llvm/IR/Function.h>
#include <llvm/IR/Type.h>

using namespace llvm;

//======================
// NumberExprAST
//======================

NumberExprAST::NumberExprAST(double Val) : Val(Val) {}

Value *NumberExprAST::codegen() {
  return ConstantFP::get(*TheContext, APFloat(Val));
}

//======================
// VariableExprAST
//======================

VariableExprAST::VariableExprAST(const std::string &Name) : Name(Name) {}

Value *VariableExprAST::codegen() {
  Value *V = NamedValues[Name];
  if (!V)
    return nullptr; // error handled elsewhere
  return V;
}

//======================
// BinaryExprAST
//======================

BinaryExprAST::BinaryExprAST(char Op,
                             std::unique_ptr<ExprAST> LHS,
                             std::unique_ptr<ExprAST> RHS)
    : Op(Op), LHS(std::move(LHS)), RHS(std::move(RHS)) {}

Value *BinaryExprAST::codegen() {
  Value *L = LHS->codegen();
  Value *R = RHS->codegen();
  if (!L || !R)
    return nullptr;

  switch (Op) {
  case '+':
    return Builder->CreateFAdd(L, R, "addtmp");
  case '-':
    return Builder->CreateFSub(L, R, "subtmp");
  case '*':
    return Builder->CreateFMul(L, R, "multmp");
  case '<': {
    Value *Cmp = Builder->CreateFCmpULT(L, R, "cmptmp");
    return Builder->CreateUIToFP(
        Cmp, Type::getDoubleTy(*TheContext), "booltmp");
  }
  default:
    return nullptr;
  }
}

//======================
// CallExprAST
//======================

CallExprAST::CallExprAST(const std::string &Callee,
                         std::vector<std::unique_ptr<ExprAST>> Args)
    : Callee(Callee), Args(std::move(Args)) {}

Value *CallExprAST::codegen() {
  Function *CalleeF = TheModule->getFunction(Callee);
  if (!CalleeF)
    return nullptr;

  if (CalleeF->arg_size() != Args.size())
    return nullptr;

  std::vector<Value *> ArgsV;
  for (auto &Arg : Args) {
    ArgsV.push_back(Arg->codegen());
    if (!ArgsV.back())
      return nullptr;
  }

  return Builder->CreateCall(CalleeF, ArgsV, "calltmp");
}

//======================
// PrototypeAST
//======================

PrototypeAST::PrototypeAST(const std::string &Name,
                           std::vector<std::string> Args)
    : Name(Name), Args(std::move(Args)) {}

const std::string &PrototypeAST::getName() const {
  return Name;
}

Function *PrototypeAST::codegen() {
  std::vector<Type *> Doubles(Args.size(),
                             Type::getDoubleTy(*TheContext));

  FunctionType *FT =
      FunctionType::get(Type::getDoubleTy(*TheContext),
                        Doubles, false);

  Function *F = Function::Create(
      FT, Function::ExternalLinkage, Name, TheModule.get());

  unsigned Idx = 0;
  for (auto &Arg : F->args())
    Arg.setName(Args[Idx++]);

  return F;
}

//======================
// FunctionAST
//======================

FunctionAST::FunctionAST(std::unique_ptr<PrototypeAST> Proto,
                         std::unique_ptr<ExprAST> Body)
    : Proto(std::move(Proto)), Body(std::move(Body)) {}

Function *FunctionAST::codegen() {
  Function *TheFunction = TheModule->getFunction(Proto->getName());

  if (!TheFunction)
    TheFunction = Proto->codegen();

  if (!TheFunction)
    return nullptr;

  BasicBlock *BB =
      BasicBlock::Create(*TheContext, "entry", TheFunction);
  Builder->SetInsertPoint(BB);

  NamedValues.clear();
  for (auto &Arg : TheFunction->args())
    NamedValues[std::string(Arg.getName())] = &Arg;

  if (Value *RetVal = Body->codegen()) {
    Builder->CreateRet(RetVal);
    verifyFunction(*TheFunction);
    return TheFunction;
  }

  TheFunction->eraseFromParent();
  return nullptr;
}
