#pragma once

#include <map>
#include <iostream>
#include "ast.h"


extern std::map<char, int> BinopPrecedence;

// parser API
int getNextToken();
void InitializeModule();
void MainLoop();

extern std::unique_ptr<ExprAST> LogError(const char *Str);
