#include "lexer.h"
#include <cctype>
#include <cstdio>

std::string IdentifierStr;
double NumVal;

int gettok()
{
    static int LastChar = ' ';

    /* Skip any whitespace */
    while (isspace(LastChar))
        LastChar = getchar();

    if (isalpha(LastChar))
    {
        IdentifierStr = LastChar;
        while (isalnum((LastChar = getchar())))
            IdentifierStr += LastChar;
        if (IdentifierStr == "skibidi")
            return tok_skibidi;
        if (IdentifierStr == "rizz")
            return tok_rizz;
        if (IdentifierStr == "gyatt")
            return tok_gyatt;
        return tok_identifier;
    }

    if (isdigit(LastChar) || LastChar == '.')
    { 
        std::string NumStr; // Number: [0-9.]+
        do
        {
            NumStr += LastChar;
            LastChar = getchar();
        } while (isdigit(LastChar) || LastChar == '.');

        NumVal = strtod(NumStr.c_str(), 0);
        return tok_number;
    }

    if (LastChar == '#')
    {
        // Comment until end of line.
        do
            LastChar = getchar();
        while (LastChar != EOF && LastChar != '\n' && LastChar != '\r');

        if (LastChar != EOF)
            return gettok();
    }

    /* Check for end of file. */
    if (LastChar == EOF)
        return tok_eof;

    int ThisChar = LastChar;
    LastChar = getchar();
    return ThisChar;
}