/*
  本文基于年份、月份、日三个分离的变量实现了 date.hpp 中声明的日期相关操作。
*/

#include <cstdio> // for snprintf
#include "date.hpp"

#if defined(USE_DATE_JDN)

#error "只有当宏 USE_DATE_JDN 未被定义时本文件才起作用!"

#else

//
// 在下文中， Date 实质上是 struct Date_YMD 的别名
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

bool dateInit(Date& theDate, int y, int m, int d)
{
    // 这里只是进行简单的、不完整的检查，不完整的!!!
    if( m < 1 || m > 12 ) return false;
    if( d < 1 || d > 31 ) return false;
    if( y == 0 ) return false;

    theDate.year = y;
    theDate.month = m;
    
    // 需要检查 d 是否超过当月最后1天，否则计算就是错误的
    int maxDay = lastDayOfMonth(y, m);
    if( d > maxDay )
    {
        theDate.day = maxDay;
        return false;
    }

    theDate.day = d;
    return true;
}

bool dateInit(Date& theDate, Date_YMD ymd)
{
    return dateInit(theDate, ymd.year, ymd.month, ymd.day);
}

bool dateInit(Date& theDate, int julianDayNum)
{
    if( julianDayNum <= 0 ) return false;
    theDate = JDN2YMD(julianDayNum);
    return true;
}

int date2JulianDay(const Date& theDate)
{
    return YMD2JDN(theDate.year, theDate.month, theDate.day);
}

Date_YMD date2YMD(const Date& theDate)
{
    return theDate;
}

bool isValidDate(const Date& theDate)
{
    if( theDate.month < 1 || theDate.month > 12 ) return false;
    if( theDate.day < 1 || theDate.day > 31 ) return false;
    if( theDate.year == 0 ) return false;

    //MUST be a day after Nov 24, 4714 BC
    int jdn = YMD2JDN(theDate.year, theDate.month, theDate.day);
    if( jdn <= 0 )
        return false;
    else
        return true;
}


int yearOfDate(const Date& theDate)
{
    return theDate.year;
}

int monthOfDate(const Date& theDate)
{
    return theDate.month;
}

int dayOfDate(const Date& theDate)
{
    return theDate.day;
}


// [0, 1, ..., 6] => [Sunday, Monday, ..., Saturday ]
int dayOfWeek(const Date& theDate)
{
    return dayOfWeek(theDate.year, theDate.month, theDate.day);
}

/* 关于如何计算指定日期对应的星期
 我们在网上可以找到多个所需的计算公式。
1. 函数 weekday_Tondering 所用公式 来自
    http://www.tondering.dk/claus/cal/intro.php
    http://www.tondering.dk/main/calendar-information/14-calendar-algorithms

2. 蔡勒（Zeller）公式
3. 基姆拉尔森计算公式(下面的函数 weekday_KimLarsson)
    Week=(Day + 2*Month + 3*(Month+1）/5 + Year + Year/4 - Year/100 + Year/400) % 7  

  经测试发现，这三个算法对于公元1/1/1 及之后的星期计算是正确的。
*/

//tondering.dk
int weekday_Tondering(int year, int month, int day)
{
    int a = (14 - month) / 12;
    int y = year - a;
    int m = month + 12*a - 2;
    
    int w = (day + y + y/4 - y/100 + y/400 + (31*m)/12) % 7;
    if( w < 0 )
        w += 7;

    return w;
}

// 基姆拉尔森(Kim Larsson)计算公式
int weekday_KimLarsson(int y, int m, int d) 
{ 
    if( m < 3 ) // 1月，2月要算作上年的第13、14月
    {
        m += 12; --y;
    }
    
    int w = ((d+2*m+3*(m+1)/5+y+y/4-y/100+y/400)+1) % 7; 
    if( w < 0 )
        return w + 7;
    else
        return w;
}


//Zeller's Rule
int weekday_Zeller(int year, int month, int day)
{
    int y = year;
    int m = month;
    if( m < 3 ) // 1月，2月要算作上年的第13、14月
    {
        m += 12; --y;
    }
    int c = y / 100; // 世纪-1
    y = y % 100;

    int w = (day-1 + y + y/4 + c/4 - 2*c + (26*(m+1)/10)) % 7;
    if( w < 0 )
        w += 7;

    return w;
}

int dayOfWeek(int year, int month, int day)
{
    // 为了利用上面的任何一个星期算法，这里将公元前的年份统一调整一下。
    // 即将 4800BC 当作 1AD，1BC 当作 4800AD.
    // 之所以选择 4800，  4801BC/1/1 恰好是星期一，而且4800恰好是400的整数倍!!
    if( year < 0 )
    {
        year = year + 1 + 4800*500; // *500 主要为了支持 1469901BC/3/1
    }
    return weekday_Zeller(year, month, day);
}

string date2string(const Date& theDate)
{
    char buf[32];
    snprintf(buf, sizeof buf, "%+05d-%02d-%02d", 
             theDate.year, theDate.month, theDate.day);
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
}

int isLeapYear(const Date& theDate)
{
    return isLeapYear(theDate.year);
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
    return dayOfYear(theDate.year, theDate.month, theDate.day);
}

int dayOfMonth(int year, int month, int day)
{
    return day;
}

int dayOfMonth(const Date& theDate)
{
    return theDate.day;
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
    return lastDayOfMonth(theDate.year, theDate.month);
}

int daysBetweenDate(const Date& d1, const Date& d2)
{
    return date2JulianDay(d1) - date2JulianDay(d2);
}

// d + n天
Date dateAddDays(const Date& d, int n)
{
    int jdn = date2JulianDay(d) + n;
    return JDN2YMD(jdn);
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

    Date_YMD ymd = d;
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

    return ymd;
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
