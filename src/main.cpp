#include "parser.h"
#include "lexer.h"
#include <llvm-18/llvm/Support/raw_ostream.h>
#include <llvm-18/llvm/IR/Module.h>


extern std::unique_ptr<llvm::Module> TheModule;
extern llvm::LLVMContext TheContext;

int main() {
  TheModule = std::make_unique<llvm::Module>("skibidi", TheContext);

  getNextToken();

  while (CurTok != tok_eof) {
    if (CurTok == tok_skibidi)
      ParseSkibidiFunction();
    else
      getNextToken();
  }

  TheModule->print(llvm::outs(), nullptr);
  return 0;
}
