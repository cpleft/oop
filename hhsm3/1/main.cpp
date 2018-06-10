#include <iostream>
#include <cstdio>
#include "lexer.hpp"
#include "parser.hpp"
#include "error.hpp"
#include "sym.hpp"
using namespace std;
using namespace lexer;
using namespace error;
using namespace parser;




int main(int argc, char* argv[]) {
  if (parseCommandLine(argc, argv) != 0) {
    cout <<"Failed to open "<< argv[2] <<endl;
    return 1;
  }

  if (input == &cin) 
    while (cin) {
      get_token();
      if (curr_tok == END) break;
      if (curr_tok == PRINT) continue;
      cout <<"="<< expr(false) <<endl;
    }

  else {
    for (int i = 0; i < argc-1; i++) {
      while (cin) {
        get_token();
        if (curr_tok == END) break;
        if (curr_tok == PRINT) continue;
        cout <<"="<< expr(false) <<endl;
      }
      int o = switch_input();
      if (o == 1) {
        cout <<"Failed to switch file."<< endl;
        return 1;
      }
      else if (o == 2) { cout <<"finished!"<<endl; return 0; }
    }
  }

  return 0;
}
