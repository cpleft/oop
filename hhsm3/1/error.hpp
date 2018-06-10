#ifndef _ERROR_H_
#define _ERROR_H_

#include <cstring>
#include <iostream>
using namespace std;


// developer interface
// user interface
namespace error {
  
  // globle variables
  extern int no_of_errors;
  extern int lines;

  // declare functions
  double error_prompt(const string& s);
}


#endif
