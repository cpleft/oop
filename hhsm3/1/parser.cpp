#include "parser.hpp"

map<string, double> parser::table;

double parser::expr(bool get) {
  double left = term(get);

  for (;;) 
    switch (curr_tok) {
      case PLUS:
        left += term(true);
        break;
      case MINUS:
        left -= term(true);
        break;
      default:
        return left;
    }
}


double parser::term(bool get) {
  double left = prim(get);

  for(;;) 
    switch (curr_tok) {
      case MUL:
        left *= prim(true);
        break;
      case DIV:
        if (double d = prim(true)) {
          left /= d;
          break;
        }
        return error_prompt("divide by 0.");
      default:
        return left;
    }
}


double parser::prim(bool get) {
  table["pi"] = 3.1415926535897932385;
  table["e"] = 2.7182818284590452354;
  if (get) get_token();

  switch (curr_tok) {
    case NUMBER:
    {
      double v = number_value;
      get_token();
      return v;
    }
    case NAME:
    {
      double& v = table[string_value];
      if (get_token() == ASSIGN) v = expr(true);
      return v;
    }
    case MINUS:
      return -prim(true);
    case LP:
    {
      double e = expr(true);
      if (curr_tok != RP) return error_prompt(") expected.");
      get_token();            // eat ')'
      return e;
    }
    default:
      return error_prompt("primmary expected.");
  }
}
