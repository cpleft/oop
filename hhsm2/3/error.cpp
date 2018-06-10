#include <iostream>
#include <cstdarg>
using namespace std;

int error(const char* fmt, ...);

int main() {
  error("\tHello, my name is %s, I'm %d years old, I perform good at school and I got an %c last semester.\n", "Ann", 9, 'A');
  return 0;
}

int error(const char* fmt, ...){
  va_list ap;
  va_start(ap, fmt);
  while (*fmt) {
    if (*fmt!='%' && *fmt!='\\') {cout << *fmt; ++fmt;}
    else if (*fmt == '\\') {
      ++fmt;
      switch (*fmt) {
        case 't': {cout<<char(9); ++fmt; break;}
        case 'n': {cout<<char(10); ++fmt; break;}
        case 'b': {cout<<char(8); ++fmt; break;}
        default: {cout<<*fmt; ++fmt; break;}
      }
    }
    else if (*fmt == '%') {
      ++fmt;
      switch (*fmt) {
        case 'd':
        {
          int p = va_arg(ap, int);
          cout << p;
          ++fmt;
          break;
        }
        case 'c':
        {
          char p = va_arg(ap, int);
          cout << p;
          ++fmt;
          break;
        }
        case 's':
        {
          char* p = va_arg(ap, char*);
          cout << p;
          ++fmt;
          break;
        }
        default:
        {
          cout << *fmt;
          break;
        }
      }  
    }
  }
  va_end(ap);
  return 0;
}
