/* ���������ͼ������Ĳ��Գ���

��7.10ϰ��19:
Write functions to add one day, one month, and one year to a Date as defined in ��5.9[13].
Write a function that gives the day of the week for a given Date.
Write a function that gives the Date of the first Monday following a given Date.

ע��ϰ�� ��5.9[13] ����Ҫ���� struct Date �Լ��������ݵĶ�д���������Ըýṹ
�Ķ���δ��Ҫ�������������Ķ������ʵ���У������������ṹ�����ͣ�
  struct Date_JDN;
  struct Date_YMD;
����ǰ�ߵ����ݽṹ����һ�������������á������ա���ʾ�����ڣ�
    ���ߵ����ݽṹ������ա��¡�������������ʾ���������������ڣ���

�����û����ԣ�������������ز����ӿڡ�������(Date)����ͬ�ģ���
>> �������ʱ�����˺� USE_DATE_JDN����
   �� ʵ�ʲ��õ�������Ϊ Date_JDN��
   �� ���ڲ�����ʵ���ļ������� date_JDN.cpp
>> �������ʱδ����� USE_DATE_JDN����
   �� ʵ�ʲ��õ�������Ϊ Date_YMD��
   �� ���ڲ�����ʵ���ļ������� date_YMD.cpp
�����û�ʹ��ʱ��Ӧʹ��һ�µ��������� Date.


����������ڱ������ʱָ����
��ʽ1. �ڱ�������������ָ�������� g++ ʱ:
    ����ʹ�����������͵����ڣ���ִ��
        g++ -D USE_DATE_JDN -o jdn date_jdn.cpp main.cpp
    �ɵõ���ִ���ļ� jdn.exe.
    
    ����ʹ����ݡ��·ݡ��ձ�ʾ���������ͣ���ִ��
        g++  -o ymd date_ymd.cpp main.cpp
    �ɵõ���ִ���ļ� ymd.exe.
��ʽ2������ʹ�õ���ĳ�����ɿ���������������Ŀ�������У��ҵ�Ԥ�������������
    ���������еĺ궨������� USE_DATE_JDN������ʾ��Ҫ���� �����յ�ʵ�֣���ʱ
    ��Ӧ������ĿԴ�ļ��а��� date_JDN.cpp��
    �����ʾ��Ҫ���ø�������ʵ�֣���ʱ��Ӧ������ĿԴ�ļ��а��� date_YMD.cpp��

 */

#include <cstdlib> // for exit()
#include <iostream>

#include "date.hpp"

using namespace std;

//
// Part I   ��Ԫ���Դ���
//

#define assert(e) ((e) ? (void)0 : my_assert(#e, __FILE__, __LINE__))

void my_assert(const char* expression, const char* srcFile, int srcLineNo)
{
    cerr << "���Ա��ʽ [" << expression << "] ʧ�ܣ�λ��:\n  "
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
// Part  II.1  ������������
//
int selectMainMenu()
{
    const char* prompt[] =
          { "\n�������ṩ���й��ܣ� \n",
            "1 ִ�е�Ԫ����           \t", "2 ������������֤ Date\n",
            "3 ������/��/����֤ Date  \t", "4 Date����������\n",
            "0 ��������ִ��\n"
          };
    int n_prompt = sizeof(prompt)/sizeof(prompt[0]);
    int select =0;
    while(1)
    {
        for(int i=0; i<n_prompt; ++i) cout << prompt[i];
        cout << "�����빦�ܱ���[0-" << n_prompt << "]: ";

        cin >> select;
        if(cin.eof())
            return 0;
        if( !cin )
        {
            cin.clear(); // �������״̬
            cin.sync(); // �������������
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
        cout << "\n����������(>0): ";
        cin >> julianDayNumber;
        if(cin.eof())
        {
            julianDayNumber =0;
            return 0;
        }

        if( !cin )
        {
            cin.clear(); // �������״̬
            cin.sync(); // �������������
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
        cout << "�����������������α�ʾ�����е��ꡢ�¡���: ";
        cin >> year >> month >> day;
        if(cin.eof())
        {
            return 0;
        }

        if( !cin )
        {
            cin.clear(); // �������״̬
            cin.sync(); // �������������
            continue;
        }

        if(year == 0) return 0;
        return 1;
    }
}

void arithmetic () // ���ڵ���������
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
// Part II.2   ������
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


