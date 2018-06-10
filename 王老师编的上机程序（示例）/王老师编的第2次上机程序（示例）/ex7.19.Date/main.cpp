/* 对日期类型及操作的测试程序。

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


上面这个宏在编译程序时指定。
方式1. 在编译器的命令行指定，如用 g++ 时:
    若你使用儒略日类型的日期，则执行
        g++ -D USE_DATE_JDN -o jdn date_jdn.cpp main.cpp
    可得到可执行文件 jdn.exe.
    
    若你使用年份、月份、日表示的日期类型，则执行
        g++  -o ymd date_ymd.cpp main.cpp
    可得到可执行文件 ymd.exe.
方式2：若你使用的是某个集成开发环境，则在项目的配置中，找到预处理器的配置项，
    若你在其中的宏定义中添加 USE_DATE_JDN，即表示你要采用 儒略日的实现，此时
    你应当在项目源文件中包括 date_JDN.cpp。
    否则表示你要采用格里历的实现，此时你应当在项目源文件中包括 date_YMD.cpp。

 */

#include <cstdlib> // for exit()
#include <iostream>

#include "date.hpp"

using namespace std;

//
// Part I   单元测试代码
//

#define assert(e) ((e) ? (void)0 : my_assert(#e, __FILE__, __LINE__))

void my_assert(const char* expression, const char* srcFile, int srcLineNo)
{
    cerr << "测试表达式 [" << expression << "] 失败，位置:\n  "
         << srcFile << ":" << srcLineNo << endl;
    exit(1);
}

#if defined(USE_DATE_JDN)
bool testLeapYear(int year)
{
    Date d1, d2;
    dateInit(d1, year, 3, 1);
    dateInit(d2, year, 2, 29);

    int r = (d1.julianDayNumber - d2.julianDayNumber);
    if( r == isLeapYear(year) )
        return true;
    else
        return false;
}
#else
bool testLeapYear(int year)
{
    // TO BE DONE............
    return true; 
}
#endif

int unit_test()
{
    int julianDayNumber = 2415021; // 1900-01-01
    int weekDay = 1; // Monday

    for (int year = 1900; year < 2500; ++year)
    {
        assert( testLeapYear( year ) );
        for (int month = 1; month <= 12; ++month)
        {
            int lastDay = lastDayOfMonth(year, month);
            for (int day = 1; day <= lastDay; ++day)
            {
              Date d;
              dateInit(d, year, month, day);

              assert(year    == yearOfDate(d));
              assert(month   == monthOfDate(d));
              assert(day     == dayOfDate(d));
              assert(weekDay == dayOfWeek(d));
              assert(julianDayNumber == date2JulianDay(d));

              Date d2;
              dateInit(d2, julianDayNumber);
              assert(year    == yearOfDate(d2));
              assert(month   == monthOfDate(d2));
              assert(day     == dayOfDate(d2));
              assert(weekDay == dayOfWeek(d2));
              assert(julianDayNumber == date2JulianDay(d2));

              ++julianDayNumber;
              weekDay = (weekDay+1) % 7;
            }
        }
    }

    cout << "All passed.\n";
    return 0;
}

//
// Part  II.1  主程序辅助代码
//
int selectMainMenu()
{
    const char* prompt[] =
          { "\n主程序提供下列功能： \n",
            "1 执行单元测试           \t", "2 输入儒略日验证 Date\n",
            "3 输入年/月/日验证 Date  \t", "4 Date的算术运算\n",
            "0 结束程序执行\n"
          };
    int n_prompt = sizeof(prompt)/sizeof(prompt[0]);
    int select =0;
    while(1)
    {
        for(int i=0; i<n_prompt; ++i) cout << prompt[i];
        cout << "请输入功能编码[0-" << n_prompt << "]: ";

        cin >> select;
        if(cin.eof())
            return 0;
        if( !cin )
        {
            cin.clear(); // 清除错误状态
            cin.sync(); // 清空已输入内容
            continue;
        }
        if( 0 <= select && select <= n_prompt-2)
            return select;
    }
}

int inputJulianDayNumber(int & julianDayNumber)
{
    julianDayNumber =0;
    while(1)
    {
        cout << "\n输入儒略日(>0): ";
        cin >> julianDayNumber;
        if(cin.eof())
        {
            julianDayNumber =0;
            return 0;
        }

        if( !cin )
        {
            cin.clear(); // 清除错误状态
            cin.sync(); // 清空已输入内容
            continue;
        }
        if( 0 < julianDayNumber )
            return julianDayNumber;
    }
}

int inputYMD(int & year, int & month, int & day)
{
    while(1)
    {
        cout << "输入三个整数，依次表示日期中的年、月、日: ";
        cin >> year >> month >> day;
        if(cin.eof())
        {
            return 0;
        }

        if( !cin )
        {
            cin.clear(); // 清除错误状态
            cin.sync(); // 清空已输入内容
            continue;
        }

        if(year == 0) return 0;
        return 1;
    }
}

void arithmetic () // 日期的算术运算
{
    Date d19000101;
    dateInit(d19000101, 1900, 1,1);

    int year, month, day;
    Date d2, d3;
    while(1)
    {
        if( inputYMD(year, month, day) == 0 )
            return;
        dateInit(d2, year, month, day);
        d3 = d2;
        cout << "days after "
             << date2string(d19000101) << ":"
             << daysBetweenDate(d2, d19000101) << endl;
/*
        for(int i=-4; i<=4; ++i)
        {
            d2 = dateAddDays(d3, 10 * i);
            cout << " after " << 10 * i << " days: " << date2string(d2) << endl;
        }
*/

        for(int i=-12; i<=12; ++i)
        {
            d2 = dateAddMonths(d3, i);
            cout << "after " << i << " months: " << date2string(d2) << endl;
        }
/*
        for(int i=-12; i<=12; ++i)
        {
            d2 = dateAddYears(d3, i);
            cout << "after " << i << " years: " << date2string(d2) << endl;
        }
*/
    }
}

//
// Part II.2   主程序
//

int main()
{

    Date d;
    while(int func=selectMainMenu())
    {
        switch(func)
        {
            case 1:
                unit_test();
                continue;
            case 2:
            {
                int n;
                if( inputJulianDayNumber(n) == 0 )
                    continue;
                dateInit(d, n);
            }
                break;
            case 3:
            {
                int year, month, day;
                if( inputYMD(year, month, day) == 0 )
                    continue;
                dateInit(d, year, month, day);
            }
                break;
            case 4:
                arithmetic();
                continue;
            default:
                continue;
        }
        string txt=date2string(d);
        cout << "Date " << txt
             << ": Leap=" << (isLeapYear(d) ? "YES" : "NO")
             << " JDN=" << date2JulianDay(d)
             << " Week=" << dayOfWeek(d)
             << " day#year=" << dayOfYear(d)
             << " nextMonday=" << date2string(nextMonday(d))
             << endl;
    }
    return 0;
}


