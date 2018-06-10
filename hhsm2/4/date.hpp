#include <iostream>
#include <cstdio>
#include <algorithm>
using namespace std;

class date {
public:
  enum Month {jan=1, feb, mar, apr, may, jun, jul, aug, sep, oct, nov, dec};
  date(int dd = 0, Month mm = Month(0), int yy = 0); // 0 means pick a default.

  class bad_date {};

  // functions for examining the date:
  int day() const;
  int month() const;
  int year() const;
  static void set_default(int , Month , int );

  // functions for changing the date:
  date& add_day(int n);
  date& add_month(int n);
  date& add_year(int n);

private:
  int d, m, y;
  static date default_date;

  date& add_one_day();
};

// overloading operators
bool operator==(date, date);
bool operator!=(date, date);

// friends functions
int leapyear(int y);
int week(date a);
date first_monday(date a);

