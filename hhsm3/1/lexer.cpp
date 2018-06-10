#include "lexer.hpp"


// define globle variables
lexer::Token_value lexer::curr_tok = PRINT;
double lexer::number_value;
string lexer::string_value;
istream* lexer::input;
vector<string> lexer::fileNames;


// implement member functions
int lexer::parseCommandLine(int argc, char* argv[]){
  if (argc == 1) {lexer::input = &cin; return 0;}
  else {
    for (int i = 1; i < argc; ++i) 
      fileNames.push_back(argv[i]);
    // open the first file
    input = new ifstream(fileNames[0].c_str());
    // judge whether the file opened successfully
    if (lexer::input->good() == 1) {
      fileNames.erase(fileNames.begin());
      return 0;
    }
    else return 1; // failed to open the file
  }
}

int lexer::switch_input() {
  if (fileNames.empty() == 1) return 2; // no files remained
  else {
    delete lexer::input; // close last file
    lexer::input = new ifstream(fileNames[0].c_str()); // open next file
    if (input->good() == 1) {
      fileNames.erase(fileNames.begin());
      return 0;
    }
    else return 1; // failed to switch the file
  }
}

lexer::Token_value lexer::get_token(){
  char ch;
  do {
    if (!(input->get(ch))) return curr_tok = END;
  } while (ch!='\n' && isspace(ch));

  switch (ch) {
    case '\n': case ';':
      { lines++; return curr_tok = PRINT; }
    
    case '+': case '-': case '*': case '/':
    case '(': case ')': case '=':
      return curr_tok = Token_value(ch);

    case '0': case '1': case '2': case '3': case '4': 
    case '5': case '6': case '7': case '8': case '9': 
      input->putback(ch);
      (*input) >> number_value;
      return curr_tok = NUMBER;

    default:
      if (isalpha(ch)) { 
        string_value = ch;
        while (input->get(ch) && isalnum(ch)) string_value.push_back(ch);
        input->putback(ch);
        return curr_tok = NAME;
      }
      error_prompt("Bad Token!");
      return curr_tok = PRINT;
  }

}




