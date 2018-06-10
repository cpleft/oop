#include "error.hpp"

// define globle variables
int error::no_of_errors = 0;
int error::lines = 0;

// implement functions
double error::error_prompt(const string& s) {
  ++no_of_errors;
  cerr <<"error at line"<< lines <<":"<< s <<endl;
  return 1;
}
