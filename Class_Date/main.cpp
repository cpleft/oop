#include <iostream>
#include "date.hpp"
using namespace std;

int main() {
  Date d = Date();
  try{
    cout << d.day() <<" "<< d.month() <<" "<< d.year() << endl;
  }
  catch(Date::bad_date e) {
    cerr << "Error: " << e.p << endl;
  }
  return 0;
}
