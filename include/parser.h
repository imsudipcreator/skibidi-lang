#pragma once

#include <map>

// tokens
extern int CurTok;
extern std::map<char, int> BinopPrecedence;

// parser API
int getNextToken();
void MainLoop();
