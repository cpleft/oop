#ifndef _LEXER_H_
#define _LEXER_H_

#include <iostream>
#include <string>
#include <vector>
#include <cctype>
#include <fstream>
#include "error.hpp"
using namespace std;
using namespace error;

// developer interface
namespace lexer {

  // globle variables
  enum Token_value {
    NAME,       NUMBER,       END, 
    PLUS='+',   MINUS='-',    MUL='*',    DIV='/',
    PRINT=';',  ASSIGN='=',   LP='(',     RP=')'
  };
  extern Token_value curr_tok;
  extern double number_value;
  extern string string_value;

  extern std::istream *input;
  extern vector<std::string> fileNames;

  // declare functions
  Token_value get_token();
  int parseCommandLine(int argc, char* argv[]);
  int switch_input();
}


// user interface
namespace lexer {
  // globle variables
  extern Token_value curr_tok;
  extern double number_value;
  extern std::istream *input;

  // declare functions
  Token_value get_token();
  int parseCommandLine(int argc, char* argv[]);
  int switch_input();
}

#endif
