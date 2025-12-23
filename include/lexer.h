#pragma once
#include <string>

enum Token {
  tok_eof = -1,

  // keywords
  tok_skibidi = -2, // function
  tok_rizz    = -3, // return
  tok_gyatt   = -4, // variable

  // primary
  tok_identifier = -5,
  tok_number     = -6
};

static std::string IdentifierStr;
static double NumVal;

int gettok();