#pragma once

#include <memory>
#include <string>
#include <vector>

// Forward declare LLVM types (DO NOT include heavy LLVM headers here)
namespace llvm {
  class Value;
  class Function;
}

//======================
// Base AST class
//======================
class ExprAST {
public:
  virtual ~ExprAST() = default;
  virtual llvm::Value *codegen() = 0;
};

//======================
// Expression AST nodes
//======================

class NumberExprAST : public ExprAST {
  double Val;

public:
  explicit NumberExprAST(double Val);
  llvm::Value *codegen() override;
};

class VariableExprAST : public ExprAST {
  std::string Name;

public:
  explicit VariableExprAST(const std::string &Name);
  llvm::Value *codegen() override;
};

class BinaryExprAST : public ExprAST {
  char Op;
  std::unique_ptr<ExprAST> LHS, RHS;

public:
  BinaryExprAST(char Op,
                std::unique_ptr<ExprAST> LHS,
                std::unique_ptr<ExprAST> RHS);
  llvm::Value *codegen() override;
};

class CallExprAST : public ExprAST {
  std::string Callee;
  std::vector<std::unique_ptr<ExprAST>> Args;

public:
  CallExprAST(const std::string &Callee,
              std::vector<std::unique_ptr<ExprAST>> Args);
  llvm::Value *codegen() override;
};

//======================
// Function AST nodes
//======================

class PrototypeAST {
  std::string Name;
  std::vector<std::string> Args;

public:
  PrototypeAST(const std::string &Name,
               std::vector<std::string> Args);

  llvm::Function *codegen();
  const std::string &getName() const;
};

class FunctionAST {
  std::unique_ptr<PrototypeAST> Proto;
  std::unique_ptr<ExprAST> Body;

public:
  FunctionAST(std::unique_ptr<PrototypeAST> Proto,
              std::unique_ptr<ExprAST> Body);

  llvm::Function *codegen();
};
