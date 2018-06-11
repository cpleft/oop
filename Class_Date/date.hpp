/* class Date */
class Date {
public:
  enum Month {Jan=1, Feb, Mar, Apr, May, Jun, Jul, Aug, Sep, Oct, Nov, Dec};

  // constructor
  Date(Month mm=Month(0), int dd=0, int yy=0); // 0 means pick a default

  // functions for examing the date
  int day() const;
  Month month() const;
  int year() const;

  // functions for changing the date
  Date& add_day(int n);
  Date& add_month(int n);
  Date& add_year(int n);

  // functions for set default date
  static void set_default(Month , int , int );

  // handle error
  struct bad_date {
    const char* p;
    bad_date(const char* q) {p = q;}
  };

private:
  int N;          // days from Jan 1 1970. N=0 means Jan 1 1970.
  static Date default_date;

  /* functions */
  int addOneMonth();  // return days betweem months. 
  int addOneYear(); // return days betweem years.
};


/* friend functions */
// if it is a leapyear, return 1; else return 0.
int leapyear(int y);
// the maxium days of the given month m.
int maxDaysofMonth(Date::Month mm, int y);


