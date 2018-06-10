/*
§7.10习题19:
Write functions to add one day, one month, and one year to a Date as defined in §5.9[13].
Write a function that gives the day of the week for a given Date.
Write a function that gives the Date of the first Monday following a given Date.

注：习题 §5.9[13] 仅仅要求定义 struct Date 以及日期数据的读写操作，而对该结构
的定义未做要求。所以你正在阅读的这个实现中，定义了两个结构体类型：
  struct Date_JDN;
  struct Date_YMD;
其中前者的数据结构仅有一个整数，这是用“儒略日”表示的日期；
    后者的数据结构则包括日、月、年三个整数表示公历（格里历日期）。

对于用户而言，看到的日期相关操作接口、类型名(Date)是相同的，但
>> 如果编译时定义了宏 USE_DATE_JDN，则
   · 实际采用的类型名为 Date_JDN，
   · 日期操作的实现文件必须是 date_JDN.cpp
>> 如果编译时未定义宏 USE_DATE_JDN，则
   · 实际采用的类型名为 Date_YMD，
   · 日期操作的实现文件必须是 date_YMD.cpp
所以用户使用时，应使用一致的期类型名 Date.

--------------------------------------------------------------------------
关于格里历、儒略历的知识可阅读
1. 日历生成算法-中国公历（格里历）（上）：
    http://blog.csdn.net/orbit/article/details/7749723
2. 日历生成算法-中国公历（格里历）（下）：
    http://blog.csdn.net/orbit/article/details/7825004
--------------------------------------------------------------------------
儒略日(Julian Day Number, Julian period)是指从儒略历的
    公元前4713年1月1日UTC 12:00
开始所经过的天数且用 0 表示儒略历的
    公元前4713年1月1日 12:00 到 儒略历的公元前4713年 1月2日12:00之间的24小时，
依次顺推，每一天都被赋予一个唯一的数字。
*** 注：儒略历的公元前4713年1月1日即 公历（格里历） 4714 BC/11/24。

使用儒略日的优点：
　(1) 可以把不同历法的年表统一起来，很方便地在各种历法中追溯日期；
  (2) 计算两个日期之间的天数非常方便. 如你使用的是格里历，则一般策略先计算出
      两个日期的儒略日，然后直接相减就可以得到两个日期相隔的天数。
  (3) 计算指定日期是星期几也非常方便： 计算(JDN+1)除以7的余数即可。
当然也有自己的缺点...

同样，使用格里历也有自己的优缺点...

============================================================
这个实现文件供大家学习参考，并作为第10章练习的实现基础。
*/

/* !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
注意事项：
1. 两种实现中，均假设 1582年10月5日到14日之间的10天 在历法上是存在的。
   虽然事实上不存在。
2. 两种实现支持的起始日期为 -4800BC/1/1。
3. 两种实现中，有部分代码是相同的。
4. 两种实现中，对于格里历中的公元前 N 年，统一用 -N 表示其年份。这样和
   习惯一致，即不存在公元0年!!!

扩展知识：
1. 在 Microsoft Excel 软件中，其日期的内部表示采用的是从 1899/12/31 以来的天数。
   即 1 表示 1900/1/1。
2. 在现代操作系统如 Unix, Linux, Windows 等，均采用从 1970/1/1 00:00:00AM 以来
   的秒数来表示当前系统时间，该时刻被成为计算机世界中的 Epoch。
   且在 C 语言的库函数中有 localtime, gmtime, mktime
   time, asctime, difftime 等多个函数来计算日期与时间。

* !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */


#ifndef DATE_HPP_INCLUDED
#define DATE_HPP_INCLUDED

#include <string>

//
// Part I   类型定义与初始化操作
//


/**  Date in Julian Day Number(Julian period). */
struct Date_JDN
{
    int julianDayNumber; // 用儒略日表示的日期, 0 对应 公历的 4714 BC/11/24
};

/** Date in Gregorian calendar. */
struct Date_YMD
{
    int year;   // 不能为0，公历的公元前 N 年统一用 -N 表示
    int month;  // [1..12]
    int day;    // [1..31]
};

#ifdef USE_DATE_JDN      // 这个结构给用户提供了一致的日期类型名
typedef Date_JDN Date;
#else
typedef Date_YMD Date;
#endif

//日期初始化: 在执行其他操作之前必须先初始化!
//  若参数无效，则返回 false, 否则返回 true.
bool dateInit(Date& theDate, int y, int m, int d);
bool dateInit(Date& theDate, Date_YMD ymd);
bool dateInit(Date& theDate, int julianDayNum);


//
//  Part II    获取日期属性的操作
//

// 检查指定日期是否有效。
// 若无效，则返回 false, 否则返回 true.
bool isValidDate(const Date& theDate);

int yearOfDate(const Date& theDate);  // 获取日期中的年份
int monthOfDate(const Date& theDate); // 获取日期中的月份
int dayOfDate(const Date& theDate);   // 获取日期中的日子

// 计算日期对应的星期。返回值的对应关系为：
// [0, 1, ..., 6] => [Sunday, Monday, ..., Saturday ]
int dayOfWeek(const Date& theDate);
int dayOfWeek(int year, int month, int day);

// 计算指定日期在当年当月中的序号（第几天）.
// 1日为第1天，因此返回1.
int dayOfMonth(const Date& theDate);
int dayOfMonth(int year, int month, int day);

// 计算指定日期在当年中的序号（第几天）.
// 1月1日为第1天，因此返回1.
int dayOfYear(const Date& theDate);
int dayOfYear(int year, int month, int day);

//判断指定年份或指定日所在年份是否为闰年.
//  若是则返回 1, 否则返回 0.
int isLeapYear(int year);
int isLeapYear(const Date& theDate);

// 获取指定年月或指定日所属月份的总天数（即指定月的最后一天的编号）。
int lastDayOfMonth(int year, int month);
int lastDayOfMonth(const Date& theDate);



//
//  Part III   关于日期的若干运算
//


// 日期转换为字符串，该串采用 ISO 8601:2004 日期格式： "YYYY-MM-DD"
std::string date2string(const Date& theDate);
// 日期转换为 日、月、年三个分量
Date_YMD date2YMD(const Date& theDate);
// 日期转换为儒略日
int date2JulianDay(const Date& theDate);


// 返回 d1-d2 之间的天数.
// 若返回负值，表示 d1 < d2.
int daysBetweenDate(const Date& d1, const Date& d2);

// add or substract |n| days, and return the result date.
Date dateAddDays(const Date& d,  int n);  // return d + n天

// add or substract |n| years, and return the result date.
// ==  dateAddMonths(d, n*12)
Date dateAddYears(const Date& d, int n);


// add or substract |n| months, and return the result date.
// If d is the last day of the month
//    or
// if the resulting month has fewer days than the day component of d,
// then the result is the last day of the resulting month.
// Otherwise, the result has the same day component as d.
Date dateAddMonths(const Date& d, int n);

// return the Date of the first Monday following a given Date
Date nextMonday(const Date& d);

#endif // DATE_HPP_INCLUDED
