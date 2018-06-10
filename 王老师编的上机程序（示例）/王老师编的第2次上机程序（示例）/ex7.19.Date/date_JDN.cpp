/*
  本文基于 儒略日 实现了 date.hpp 中声明的日期相关操作。
*/

#include <cstdio> // for snprintf
#include "date.hpp"

#if !defined(USE_DATE_JDN)

#error "只有定义宏 USE_DATE_JDN 时本文件才起作用!"

#else

//
// 在下文中， Date 实质上是 struct Date_JDN 的别名
//


using namespace std;


/* 
提示：儒略日 和 格里历日期之间的转换算法有多种，至少有：
1. 本文 未 采用的算法
   http://www.tondering.dk/claus/cal/julperiod.php
   这个算法仅支持公历 4801BC/3/1 及之后的日期。
2. 本文采用的算法
   "Astronomy Answers：Julian Day Number"
   http://aa.quae.nl/en/reken/juliaansedag.html#3
   该网页说这个算法可支持任意日期。
   但要注意：由于实现时，用 int 类型来存储日月年、和儒略日，所以该算法
   可支持 [ 1469901BC/3/1, 1470000AD/2/29 ] 之间的日期，
   其他日期则造成会 int 溢出!
*/


// 计算 number/divisor 的商和余数。
// 商必须向下取整，余数必须为非负数.
// 这里要求 divisor 必须为 正整数!
static inline void div_int(int number,     // 被除数
                           int divisor,    // 除数
                           int & quotient, // 商
                           int & remainder)// 余数
{
    remainder = number % divisor;

    // 能整除，或 被除数为 非负数 时，直接进行整数除法即可
    if(remainder == 0 || number >= 0)
        quotient =  number / divisor;
    else // 不能整除，且被除数为负数时，需向左取整
        quotient = number / divisor - 1;

    if( remainder < 0 )
        remainder += divisor;

    return;
}

/*
JDN = -1721120, -9426/8/19
JDN = -1721119, -9426/8/20
JDN = 0:        本来应表示儒略历的 4713 BC/1/1，即公历 4714 BC/11/24
JDN = 2415021;  1900-01-01
*/
// calculate and return the CJDN J from a Gregorian date
static int YMD2JDN(int y, int m, int d)
{
    // 因为公元前1年结束后，紧接着是公元后1年，所以没有公元0年。
    // 从而这里需要调整一下： 如 1BC->0, 10BC -> -9
    if(y<0) y += 1;

    // 这里将每年的3月记作 0月 来计算. 1，2月就是前一年的10,11月
    int c0 = (m<3) ? -1 : 0;
    int x4 = y + c0;           // 矫正后的年份
    int x1 = m - 12 * c0 - 3;  // 矫正后的月份
    
    // x3 为 世纪，采用向下取整策略
    // x2 为 当前世纪中的年份
    int x3, x2;
    div_int(x4, 100, x3, x2);
    

    // Concept: Calculation years run from 1 March to 1 March.
    // J1 is the number of days from the beginning of calculation year 0
    // until the beginning of the current 100-calculation-year-period.
    int J1 , r;
    div_int(146097*x3, 4, J1, r);
    
    // J2 is the number of days since the beginning of the current
    // 100-calculation-year-period.
    int J2;
    div_int(36525*x2, 100, J2, r);
    
    
    // J3 is the number of days minus one from the beginning of the
    // current calculation year until the beginning of the current month.
    int J3;
    div_int(153*x1+2, 5, J3, r);
    
    return  J1 + J2 + J3 
           + d        // days in the month
           + 1721119; // an offset to make day 0 is Nov 24, 4714 BC
}

// calculate and return the Gregorian date  from a CJDN julianDayNumber
static Date_YMD JDN2YMD(int julianDayNumber)
{
    int q, r;

    int x3, r3;
    div_int(4*julianDayNumber - 6884477, 146097, x3, r3);
    // 等价于:
    // div_int(4*(julianDayNumber - 1721120) +3 , 146097, x3, r3);

    int x2, r2;
    div_int(r3, 4, q, r);
    div_int(100*q+99, 36525, x2, r2);

    int x1, r1;
    div_int(r2, 100, q, r);
    div_int(5*q+2, 153, x1, r1);

    Date_YMD ymd;
    div_int(r1, 5, q, r);
    ymd.day =  q + 1;            // day of the month
    div_int(x1+2, 12, q, r);     // q is c0 in algorithm
    ymd.month = x1 - 12 * q + 3; // normal month
    ymd.year = 100 * x3 + x2 + q;// normal year

    if(ymd.year < 1) // 调整公元前的年份
        ymd.year -= 1;

    return ymd;
}

#if 0  // disable follow codes
/* 下面是本文没有采用的算法
   http://www.tondering.dk/claus/cal/julperiod.php
   这个算法仅支持公历 4801BC/3/1 及之后的日期。
    http://blog.csdn.net/solstice/article/details/5814486
    http://www.faqs.org/faqs/calendars/faq/part2/
         OR:
        http://www.tondering.dk/claus/calendar.html
  最后这个网站介绍了世界上的若干种日历知识。
*/
static int YMD2JDN(int y, int m, int d) // local function
{
    // 因为公元前1年结束后，紧接着是公元后1年，所以没有公元0年。
    // 从而这里需要调整一下： 如 1BC->0, 10BC -> -9
    if(y<0) y += 1;

    // 这里将每年的3月记作第1个月来计算. 1，2月就是当年第11,12个月
    int a = (14 - m) / 12;
    y = y + 4800 - a;
    m = m + 12 * a - 3;
    return     d                 // days in the month
             + (153*m + 2) / 5   // days before the month
             + y*365 + y/4 - y/100 + y/400  // days before the year
             - 32045  // an offset to make day 0 is Nov 24, 4714 BC
             ;
}


static Date_YMD JDN2YMD(int julianDayNumber) // local function
{
    int a = julianDayNumber + 32044; // 0th day in Mar 1, epoch year
    int b = (4 * a + 3) / 146097;            // century
    int c = a - ((b * 146097) / 4);          // days in the century
    int d = (4 * c + 3) / 1461;              // year in the century
    int e = c - ((1461 * d) / 4);            // days in the year
    int m = (5 * e + 2) / 153;               // shifted month in the year

    Date_YMD ymd;
    ymd.day = e - ((153 * m + 2) / 5) + 1;   // day of the month
    ymd.month = m + 3 - 12 * (m / 10);       // normal month
    ymd.year = b * 100 + d - 4800 + (m / 10);// normal year

    if(ymd.year <= 0) // 调整公元前的年份
        ymd.year -= 1;

    return ymd;
}
#endif // end of disable

bool dateInit(Date& theDate, int y, int m, int d)
{
    // 这里只是进行简单的、不完整的检查，不完整的!!!
    if( m < 1 || m > 12 ) return false;
    if( d < 1 || d > 31 ) return false;
    if( y == 0 ) return false;

    theDate.julianDayNumber = YMD2JDN(y, m, d);
    return true;
}

bool dateInit(Date& theDate, Date_YMD ymd)
{
    return dateInit(theDate, ymd.year, ymd.month, ymd.day);
}

bool dateInit(Date& theDate, int julianDayNum)
{
    if( julianDayNum <= 0 ) return false;
    theDate.julianDayNumber = julianDayNum;
    return true;
}

int date2JulianDay(const Date& theDate)
{
    return theDate.julianDayNumber;
}

Date_YMD date2YMD(const Date& theDate)
{
    return JDN2YMD(theDate.julianDayNumber);
}

bool isValidDate(const Date& theDate)
{
    return theDate.julianDayNumber > 0;
}


int yearOfDate(const Date& theDate)
{
    return date2YMD(theDate).year;
}

int monthOfDate(const Date& theDate)
{
    return date2YMD(theDate).month;
}

int dayOfDate(const Date& theDate)
{
    return date2YMD(theDate).day;
}


// [0, 1, ..., 6] => [Sunday, Monday, ..., Saturday ]
int dayOfWeek(const Date& theDate)
{
    int w = (theDate.julianDayNumber+1) % 7;
    return (w>=0) ? w : (w+7);
}

int dayOfWeek(int year, int month, int day)
{
    Date d;
    dateInit(d, year, month, day);
    return dayOfWeek(d);
}

string date2string(const Date& theDate)
{
    char buf[32];
    Date_YMD ymd = date2YMD( theDate );
    snprintf(buf, sizeof buf, "%+05d-%02d-%02d", ymd.year, ymd.month, ymd.day);
    return buf;
}

int isLeapYear(int year)
{
    // 公元前的年份需要调整。 如公元前1年的年份值为-1，但该年是闰年.
    if(year < 0)
        year += 1;

    // 判断闰年的标准方法
    if ( ( year % 400 == 0 ) ||
         ( (year % 4  == 0)  && (year % 100 != 0) )
       )
        return 1;
    else
        return 0;

/* 基于本文代码，还有另一种判断闰年的方法，但计算量更大
   dateInit(date1, year, 2,29);
   dateInit(date2, year, 3,1);
   if( date1.julianDayNumber != date2.julianDayNumber )
       return 1; // true, year is leap year
   else
       return 0; // false
 */
}

int isLeapYear(const Date& theDate)
{
    Date_YMD ymd = JDN2YMD(theDate.julianDayNumber);
    return isLeapYear(ymd.year);
}

// local data only for this file
static int days_each_month[13] =
{
    0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31
};

int dayOfYear(int year, int month, int day)
{
    if( month < 1 || month > 12 ) return 0;
    int days = 0;
    for(int i=1; i<month; ++i)
        days += days_each_month[i];

    return days + day
          + ((month > 2 && isLeapYear(year)) ? 1 : 0);
}

int dayOfYear(const Date& theDate)
{
    Date_YMD ymd = JDN2YMD(theDate.julianDayNumber);
    return dayOfYear(ymd.year, ymd.month, ymd.day);
}

int dayOfMonth(int year, int month, int day)
{
    return day;
}

int dayOfMonth(const Date& theDate)
{
    Date_YMD ymd = JDN2YMD(theDate.julianDayNumber);
    return ymd.day;
}


// 获取指定年月的天数。
int lastDayOfMonth(int year, int month)
{
  if( month < 1 || month > 12 ) return 0;

  return (month==2 && isLeapYear(year))
             ? 29
             : days_each_month[month];
}

int lastDayOfMonth(const Date& theDate)
{
    Date_YMD ymd = JDN2YMD(theDate.julianDayNumber);
    return lastDayOfMonth(ymd.year, ymd.month);
}

int daysBetweenDate(const Date& d1, const Date& d2)
{
    return d1.julianDayNumber - d2.julianDayNumber;
}

// d + n天
Date dateAddDays(const Date& d, int n)
{
    Date ret = d;
    ret.julianDayNumber += n;
    return ret;
}

// d + n年, == + n*12 月
Date dateAddYears(const Date& d,int n)
{
    return dateAddMonths(d, n * 12);
}

// d + n月
Date dateAddMonths(const Date& d,int n)
{
    if( n == 0 )  // 避免多余的计算
        return d;

    int ys = n / 12;      // 加/减的完整年数
    int ms = n - ys * 12; // 加/减的不足一年的整月数

    Date_YMD ymd = JDN2YMD(d.julianDayNumber);
    ymd.year  += ys;
    ymd.month += ms;

    if( ymd.month > 12 )
    {
        ymd.month -= 12;
        ymd.year += 1;
    }
    else if ( ymd.month < 1 )
    {
        ymd.month += 12;
        ymd.year  -= 1;
    }

    // 若计算结果为公元0年，则需根据月数 n  的正负调整为 1 BC , 1 AD
    if( ymd.year == 0 )
    {
        ymd.year = (n>0) ? 1 : -1;
    }

    // 若日超过当月的最后一天，则调整。
    int maxday = lastDayOfMonth(ymd.year, ymd.month);
    if(ymd.day > maxday)
        ymd.day = maxday;

    Date ret;
    dateInit(ret, ymd);

    return ret;
}

Date nextMonday(const Date& d)
{
    // [0, 1, ..., 6] => [Sunday, Monday, ..., Saturday ]
    int w = dayOfWeek(d);
    int n;
    if( w == 0 )
        n = 1;
    else
        n = (7-w+1);
    return dateAddDays(d, n);
}

#endif
// End of File
