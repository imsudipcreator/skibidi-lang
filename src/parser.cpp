#include "parser.h"
#include "lexer.h"
#include <iostream>
#include <llvm-18/llvm/IR/IRBuilder.h>
#include <llvm-18/llvm/IR/Module.h>


int CurTok;
int getNextToken() { return CurTok = gettok(); }

static std::unique_ptr<ExprAST> ParseExpression();

static std::unique_ptr<ExprAST> ParsePrimary() {
  if (CurTok == tok_number) {
    auto V = std::make_unique<NumberExprAST>(NumVal);
    getNextToken();
    return V;
  }
  std::cerr << "❌ invalid expression\n";
  return nullptr;
}

static std::unique_ptr<ExprAST> ParseExpression() {
  auto LHS = ParsePrimary();
  if (!LHS) return nullptr;

  if (CurTok == '+') {
    getNextToken();
    auto RHS = ParsePrimary();
    return std::make_unique<BinaryExprAST>(
      '+', std::move(LHS), std::move(RHS));
  }
  return LHS;
}

extern llvm::LLVMContext TheContext;
extern llvm::IRBuilder<> Builder;
extern std::unique_ptr<llvm::Module> TheModule;

void ParseSkibidiFunction() {
  getNextToken(); // eat skibidi

  std::string FuncName = IdentifierStr;
  getNextToken(); // name

  getNextToken(); // (
  getNextToken(); // )

  getNextToken(); // {

  getNextToken(); // rizz
  auto Expr = ParseExpression();

  getNextToken(); // ;
  getNextToken(); // }

  auto *FT = llvm::FunctionType::get(
    llvm::Type::getDoubleTy(TheContext), false);

  auto *F = llvm::Function::Create(
    FT, llvm::Function::ExternalLinkage,
    FuncName, TheModule.get());

  auto *BB = llvm::BasicBlock::Create(TheContext, "entry", F);
  Builder.SetInsertPoint(BB);
  Builder.CreateRet(Expr->codegen());
}
