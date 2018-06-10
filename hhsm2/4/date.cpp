#include "date.hpp"

date date::default_date(1, date::jan, 1970); 



date::date(int dd, Month mm, int yy) 
{
  if (dd==0) dd = default_date.day();
  if (mm==0) mm = (Month)default_date.month();
  if (yy==0) yy = default_date.year();

  int max;
  switch (mm) {
    case feb:
      max = 28+leapyear(yy);
      break;
    case apr: case jun: case sep: case nov:
      max = 30;
      break;
    case jan: case mar: case may: case jul: case aug: case oct: case dec:
      max = 31;
      break;
    default:
      throw bad_date(); //someone cheated
  }

  if (dd<1 || max<dd) throw bad_date();
  y = yy;
  m = mm;
  d = dd;
}


inline int date::day() const {
  return d;
}
inline int date::month() const {
  return m;
}
inline int date::year() const {
  return y;
}

void date::set_default(int dd, Month mm, int yy) {
  date::default_date = date(dd, mm, yy);
}


date& date::add_one_day() {
  int max;
  switch (m) {
    case feb:
      max = 28+leapyear(y);
      break;
    case apr: case jun: case sep: case nov:
      max = 30;
      break;
    case jan: case mar: case may: case jul: case aug: case oct: case dec:
      max = 31;
      break;
    default: ;  // impossible
  }
  
  // d is at the end of month?
  if (d == max) {
    if (m == 12) {
      y += 1;
      m = 1;
      d = 1;
    }
    else {
      m += 1;
      d = 1;
    }
  }
  else d += 1;

  return *this;
}
date& date::add_day(int n) {
  while (n--) {
    add_one_day();
  }
  return *this;
}
date& date::add_month(int n) {
  if (n == 0) return *this;
  else if (n > 0) {
    int delta_y = n/12;
    int mm = m+n%12;
    if (12 < mm) {
      delta_y++;
      mm -= 12;
    }
    int dd = d;

    //handle the case where Month(mm) does not have day d:
    int max;
    switch (mm) {
      case feb:
        max = 28+leapyear(y+delta_y);
        break;
      case apr: case jun: case sep: case nov:
        max = 30;
        break;
      case jan: case mar: case may: case jul: case aug: case oct: case dec:
        max = 31;
        break;
      default: ;  // impossible
    }
    if (dd > max) dd = max;


    y += delta_y;
    m = Month(mm);
    d = dd;
    return *this;
  }
  else {
  }
}
date& date::add_year(int n) {
  if (n == 0) return *this;
  else if (n > 0) {
    y += n;
    if (m == 2 && d == 29) {
      d = d-1+leapyear(y);
    }
    return *this;
  }
  else {
  }
}

// overloading operators
inline bool operator==(date a, date b) {
  return a.day()==b.day() && a.month()==b.month() && a.year()==b.year();
}
inline bool operator!=(date a, date b) {
  return a.day()!=b.day() || a.month()!=b.month() || a.year()!=b.year();
}


// friends functions implementation
int leapyear(int y) {
  if (y%100 == 0) {
    if (y/100 % 4 == 0) return 1;
    else return 0;
  }
  else if (y%4 == 0) return 1;
  else return 0;
}

int week(date a) {
  // jan 1 1970 is a Thursday
  date x = date();  // jan 1 1970
  int n = 0;
  while (x != a) {
    x.add_day(1);
    n++;
  }
  n = n%7;
  if (4+n > 7) return 4+n-7;
  else return 4+n;
}

date first_monday(date a) {
  if (week(a) == 1) return a;
  while (week(a) != 1) {
    a.add_day(1);
  }
  return a;
}

int main() {
  date a = date(28, date::jan, 2018);
  cout << a.day() <<" "<< a.month() <<" "<< a.year() << endl;
  a.add_day(1);
  cout << a.day() <<" "<< a.month() <<" "<< a.year() << endl;
  a.add_month(1);
  cout << a.day() <<" "<< a.month() <<" "<< a.year() << endl;
   a.add_year(1);
  cout << a.day() <<" "<< a.month() <<" "<< a.year() << endl;

  cout << week(a) << endl;
  date b = first_monday(a);
  cout << week(b) << endl;

  
  return 0;
}
