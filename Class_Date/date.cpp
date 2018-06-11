#include "date.hpp"
#include <iostream>         // for cin, cout, endl
#include <cstdlib>          // for atoi()
#include <ctime>            // for time_t, tm, mktime(), gmtime(), strftime()
#include <cstring>          // for strcmp()

/* definations */
// default date
Date Date::default_date = Date(Date::Jan, 1, 1970);
void Date::set_default(Date::Month mm, int dd, int yy) {
  default_date = Date(mm, dd, yy);
}

// constructor
Date::Date(Date::Month mm, int dd, int yy) {
  // pick a default
  if (dd==0) dd = default_date.day();
  if (mm==0) mm = (Month)default_date.month();
  if (yy==0) yy = default_date.year();

  // check the argument inputed
  int max;
  switch (mm) {
    case Feb:
      max = 28+leapyear(yy);
      break;
    case Apr: case Jun: case Sep: case Nov:
      max = 30;
      break;
    case Jan: case Mar: case May: case Jul: case Aug: case Oct: case Dec:
      max = 31;
      break;
    default:
      throw bad_date("invalid month inputed"); // someone cheated
  }

  if (dd<1 || dd>max) throw bad_date("invalid day inputed");

  // init
  struct std::tm the_date;   // std::tm 
  the_date.tm_mon = (int)mm-1;  // month since Jan - [0, 11]
  the_date.tm_mday = dd;   // day of the month
  the_date.tm_year = yy-1900;   // years since 1900
  the_date.tm_sec = 0;
  the_date.tm_min = 0;
  the_date.tm_hour = 0;

  N = std::mktime(&the_date) / 86400;
}


// functions for examing the date
int Date::day() const {
  std::time_t sec = N * 86400;
  char buf[10];         // to store char of days of month
  std::strftime(buf, sizeof(buf), "%d", std::gmtime(&sec));  // output days >> buf
  return atoi(buf);
}
Date::Month Date::month() const {
  std::time_t sec = N * 86400;
  char buf[10];         // to store char of days of month
  std::strftime(buf, sizeof(buf), "%b", std::gmtime(&sec));  // output days >> buf

  if (strcmp(buf, "Jan") == 0) return Jan;
  else if (strcmp(buf, "Feb") == 0) return Feb;
  else if (strcmp(buf, "Mar") == 0) return Mar;
  else if (strcmp(buf, "Apr") == 0) return Apr;
  else if (strcmp(buf, "May") == 0) return May;
  else if (strcmp(buf, "Jun") == 0) return Jun;
  else if (strcmp(buf, "Jul") == 0) return Jul;
  else if (strcmp(buf, "Aug") == 0) return Aug;
  else if (strcmp(buf, "Sep") == 0) return Sep;
  else if (strcmp(buf, "Oct") == 0) return Oct;
  else if (strcmp(buf, "Nov") == 0) return Nov;
  else if (strcmp(buf, "Dec") == 0) return Dec;
  else throw bad_date("error in strftime() or gmtime().");
}
int Date::year() const {
  std::time_t sec = N * 86400;
  char buf[10];         // to store char of days of month
  std::strftime(buf, sizeof(buf), "%Y", std::gmtime(&sec));  // output days >> buf
  return atoi(buf);
}


// functions for changing the date
Date& Date::add_day(int n) {
  N += n;
  return *this;
}
Date& Date::add_month(int n) {
  if (n == 0) return *this;
  if (n < 0) return *this;   // do not support minus.

  int days = 0;
  for (int i = 0; i < n; i++) {
    days += addOneMonth(); // return days betweem months. 
  }
  
  // update N
  N += days;
  return *this;
}
Date& Date::add_year(int n) {
  int days = 0;
  for (int i = 0; i < n; i++) {
    days += addOneYear();
  }
  
  N += days;
  return *this;
}

// define private functions 
int Date::addOneMonth() {
  // convert the date from by representing as epoch type to as mon/day/year 
  // type to make it easier to manipulate.
  Month m = month();
  int d = day();
  int y = year();

  int ret;   // days between two months.
  int daysOf_curMonth = maxDaysofMonth(m, y);
  int daysOf_nexMonth;
  // the month year number of "next month"
  if ((int)m == 12) daysOf_nexMonth = maxDaysofMonth(Jan, y+1);
  else daysOf_nexMonth = maxDaysofMonth(m, y);
  ret = daysOf_curMonth;
  if (d > daysOf_nexMonth) ret -= d - daysOf_nexMonth;
  return ret;
}

int Date::addOneYear() {
  // convert the date from by representing as epoch type to as mon/day/year 
  // type to make it easier to manipulate.
  Month m = month();
  int d = day();
  int y = year();

  // if next year is a leapyear && the date > Feb 28, then plus 366.
  // if this year is a leapyear && the date <= Feb 29, then plus 366.
  // else plus 365.
  if (leapyear(y+1) && (int)m > 2)    return 366;
  else if (leapyear(y) && (int)m < 3) return 366;
  else return 365;
}


/* define friend functions */
int leapyear(int y) {
  if (y%100 == 0) {
    if (y/100 % 4 == 0) return 1;
    else return 0;
  }
  else if (y%4 == 0) return 1;
  else return 0;
}
int maxDaysofMonth(Date::Month mm, int y){
    int max;
    switch (mm) {
      case Date::Feb:
        max = 28+leapyear(y);
        break;
      case Date::Apr: case Date::Jun: case Date::Sep: case Date::Nov:
        max = 30;
        break;
      case Date::Jan: case Date::Mar: case Date::May: case Date::Jul: 
      case Date::Aug: case Date::Oct: case Date::Dec:
        max = 31;
        break;
      default: throw Date::bad_date("invalid month number.");
    }
    return max;
}



/*  ****************************  std::tm  ************************************
int tm_sec
	seconds after the minute – [0, 61](until C++11) / [0, 60] (since C++11)[note 1]
(public member object)
int tm_min
	minutes after the hour – [0, 59]
(public member object)
int tm_hour
	hours since midnight – [0, 23]
(public member object)
int tm_mday
	day of the month – [1, 31]
(public member object)
int tm_mon
	months since January – [0, 11]
(public member object)
int tm_year
	years since 1900
(public member object)
int tm_wday
	days since Sunday – [0, 6]
(public member object)
int tm_yday
	days since January 1 – [0, 365]
(public member object)
int tm_isdst
	Daylight Saving Time flag. The value is positive if DST is in effect, zero if not and negative if no information is available
(public member object)

**********************************************************************************/
