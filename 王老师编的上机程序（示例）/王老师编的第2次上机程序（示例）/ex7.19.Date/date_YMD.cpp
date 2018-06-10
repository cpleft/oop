/*
  ���Ļ�����ݡ��·ݡ�����������ı���ʵ���� date.hpp ��������������ز�����
*/

#include <cstdio> // for snprintf
#include "date.hpp"

#if defined(USE_DATE_JDN)

#error "ֻ�е��� USE_DATE_JDN δ������ʱ���ļ���������!"

#else

//
// �������У� Date ʵ������ struct Date_YMD �ı���
//

using namespace std;

/* 
��ʾ�������� �� ����������֮���ת���㷨�ж��֣������У�
1. ���� δ ���õ��㷨
   http://www.tondering.dk/claus/cal/julperiod.php
   ����㷨��֧�ֹ��� 4801BC/3/1 ��֮������ڡ�
2. ���Ĳ��õ��㷨
   "Astronomy Answers��Julian Day Number"
   http://aa.quae.nl/en/reken/juliaansedag.html#3
   ����ҳ˵����㷨��֧���������ڡ�
   ��Ҫע�⣺����ʵ��ʱ���� int �������洢�����ꡢ�������գ����Ը��㷨
   ��֧�� [ 1469901BC/3/1, 1470000AD/2/29 ] ֮������ڣ�
   ������������ɻ� int ���!
*/


// ���� number/divisor ���̺�������
// �̱�������ȡ������������Ϊ�Ǹ���.
// ����Ҫ�� divisor ����Ϊ ������!
static inline void div_int(int number,     // ������
                           int divisor,    // ����
                           int & quotient, // ��
                           int & remainder)// ����
{
    remainder = number % divisor;

    // ���������� ������Ϊ �Ǹ��� ʱ��ֱ�ӽ���������������
    if(remainder == 0 || number >= 0)
        quotient =  number / divisor;
    else // �����������ұ�����Ϊ����ʱ��������ȡ��
        quotient = number / divisor - 1;

    if( remainder < 0 )
        remainder += divisor;

    return;
}

/*
JDN = -1721120, -9426/8/19
JDN = -1721119, -9426/8/20
JDN = 0:        ����Ӧ��ʾ�������� 4713 BC/1/1�������� 4714 BC/11/24
JDN = 2415021;  1900-01-01
*/
// calculate and return the CJDN J from a Gregorian date
static int YMD2JDN(int y, int m, int d)
{
    // ��Ϊ��Ԫǰ1������󣬽������ǹ�Ԫ��1�꣬����û�й�Ԫ0�ꡣ
    // �Ӷ�������Ҫ����һ�£� �� 1BC->0, 10BC -> -9
    if(y<0) y += 1;

    // ���ｫÿ���3�¼��� 0�� ������. 1��2�¾���ǰһ���10,11��
    int c0 = (m<3) ? -1 : 0;
    int x4 = y + c0;           // ����������
    int x1 = m - 12 * c0 - 3;  // ��������·�
    
    // x3 Ϊ ���ͣ���������ȡ������
    // x2 Ϊ ��ǰ�����е����
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
    // �ȼ���:
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

    if(ymd.year < 1) // ������Ԫǰ�����
        ymd.year -= 1;

    return ymd;
}

bool dateInit(Date& theDate, int y, int m, int d)
{
    // ����ֻ�ǽ��м򵥵ġ��������ļ�飬��������!!!
    if( m < 1 || m > 12 ) return false;
    if( d < 1 || d > 31 ) return false;
    if( y == 0 ) return false;

    theDate.year = y;
    theDate.month = m;
    
    // ��Ҫ��� d �Ƿ񳬹��������1�죬���������Ǵ����
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

/* ������μ���ָ�����ڶ�Ӧ������
 ���������Ͽ����ҵ��������ļ��㹫ʽ��
1. ���� weekday_Tondering ���ù�ʽ ����
    http://www.tondering.dk/claus/cal/intro.php
    http://www.tondering.dk/main/calendar-information/14-calendar-algorithms

2. ���գ�Zeller����ʽ
3. ��ķ����ɭ���㹫ʽ(����ĺ��� weekday_KimLarsson)
    Week=(Day + 2*Month + 3*(Month+1��/5 + Year + Year/4 - Year/100 + Year/400) % 7  

  �����Է��֣��������㷨���ڹ�Ԫ1/1/1 ��֮������ڼ�������ȷ�ġ�
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

// ��ķ����ɭ(Kim Larsson)���㹫ʽ
int weekday_KimLarsson(int y, int m, int d) 
{ 
    if( m < 3 ) // 1�£�2��Ҫ��������ĵ�13��14��
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
    if( m < 3 ) // 1�£�2��Ҫ��������ĵ�13��14��
    {
        m += 12; --y;
    }
    int c = y / 100; // ����-1
    y = y % 100;

    int w = (day-1 + y + y/4 + c/4 - 2*c + (26*(m+1)/10)) % 7;
    if( w < 0 )
        w += 7;

    return w;
}

int dayOfWeek(int year, int month, int day)
{
    // Ϊ������������κ�һ�������㷨�����ｫ��Ԫǰ�����ͳһ����һ�¡�
    // ���� 4800BC ���� 1AD��1BC ���� 4800AD.
    // ֮����ѡ�� 4800��  4801BC/1/1 ǡ��������һ������4800ǡ����400��������!!
    if( year < 0 )
    {
        year = year + 1 + 4800*500; // *500 ��ҪΪ��֧�� 1469901BC/3/1
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
    // ��Ԫǰ�������Ҫ������ �繫Ԫǰ1������ֵΪ-1��������������.
    if(year < 0)
        year += 1;

    // �ж�����ı�׼����
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


// ��ȡָ�����µ�������
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

// d + n��
Date dateAddDays(const Date& d, int n)
{
    int jdn = date2JulianDay(d) + n;
    return JDN2YMD(jdn);
}

// d + n��, == + n*12 ��
Date dateAddYears(const Date& d,int n)
{
    return dateAddMonths(d, n * 12);
}

// d + n��
Date dateAddMonths(const Date& d,int n)
{
    if( n == 0 )  // �������ļ���
        return d;

    int ys = n / 12;      // ��/������������
    int ms = n - ys * 12; // ��/���Ĳ���һ���������

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

    // ��������Ϊ��Ԫ0�꣬����������� n  ����������Ϊ 1 BC , 1 AD
    if( ymd.year == 0 )
    {
        ymd.year = (n>0) ? 1 : -1;
    }

    // ���ճ������µ����һ�죬�������
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
