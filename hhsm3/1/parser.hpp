#ifndef _PARSER_H_
#define _PARSER_H_

#include <iostream>
#include <cstring>
#include <cstdio>
#include <map>
#include "lexer.hpp"
#include "error.hpp"
using namespace std;
using namespace lexer;
using namespace error;

// developer interface
namespace parser {
  // globle variables
  extern map<string, double> table;

  // declare functions
  double expr(bool get);
  double term(bool get);
  double prim(bool get);
}

/*
// user interface
namespace parser {
  // declare functions
  double expr(bool get);
}
*/

#endif
