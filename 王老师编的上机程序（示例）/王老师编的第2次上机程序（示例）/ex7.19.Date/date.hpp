/*
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

--------------------------------------------------------------------------
���ڸ���������������֪ʶ���Ķ�
1. ���������㷨-�й������������������ϣ���
    http://blog.csdn.net/orbit/article/details/7749723
2. ���������㷨-�й������������������£���
    http://blog.csdn.net/orbit/article/details/7825004
--------------------------------------------------------------------------
������(Julian Day Number, Julian period)��ָ����������
    ��Ԫǰ4713��1��1��UTC 12:00
��ʼ���������������� 0 ��ʾ��������
    ��Ԫǰ4713��1��1�� 12:00 �� �������Ĺ�Ԫǰ4713�� 1��2��12:00֮���24Сʱ��
����˳�ƣ�ÿһ�춼������һ��Ψһ�����֡�
*** ע���������Ĺ�Ԫǰ4713��1��1�ռ� �������������� 4714 BC/11/24��

ʹ�������յ��ŵ㣺
��(1) ���԰Ѳ�ͬ���������ͳһ�������ܷ�����ڸ���������׷�����ڣ�
  (2) ������������֮��������ǳ�����. ����ʹ�õ��Ǹ���������һ������ȼ����
      �������ڵ������գ�Ȼ��ֱ������Ϳ��Եõ��������������������
  (3) ����ָ�����������ڼ�Ҳ�ǳ����㣺 ����(JDN+1)����7���������ɡ�
��ȻҲ���Լ���ȱ��...

ͬ����ʹ�ø�����Ҳ���Լ�����ȱ��...

============================================================
���ʵ���ļ������ѧϰ�ο�������Ϊ��10����ϰ��ʵ�ֻ�����
*/

/* !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
ע�����
1. ����ʵ���У������� 1582��10��5�յ�14��֮���10�� ���������Ǵ��ڵġ�
   ��Ȼ��ʵ�ϲ����ڡ�
2. ����ʵ��֧�ֵ���ʼ����Ϊ -4800BC/1/1��
3. ����ʵ���У��в��ִ�������ͬ�ġ�
4. ����ʵ���У����ڸ������еĹ�Ԫǰ N �꣬ͳһ�� -N ��ʾ����ݡ�������
   ϰ��һ�£��������ڹ�Ԫ0��!!!

��չ֪ʶ��
1. �� Microsoft Excel ����У������ڵ��ڲ���ʾ���õ��Ǵ� 1899/12/31 ������������
   �� 1 ��ʾ 1900/1/1��
2. ���ִ�����ϵͳ�� Unix, Linux, Windows �ȣ������ô� 1970/1/1 00:00:00AM ����
   ����������ʾ��ǰϵͳʱ�䣬��ʱ�̱���Ϊ����������е� Epoch��
   ���� C ���ԵĿ⺯������ localtime, gmtime, mktime
   time, asctime, difftime �ȶ������������������ʱ�䡣

* !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */


#ifndef DATE_HPP_INCLUDED
#define DATE_HPP_INCLUDED

#include <string>

//
// Part I   ���Ͷ������ʼ������
//


/**  Date in Julian Day Number(Julian period). */
struct Date_JDN
{
    int julianDayNumber; // �������ձ�ʾ������, 0 ��Ӧ ������ 4714 BC/11/24
};

/** Date in Gregorian calendar. */
struct Date_YMD
{
    int year;   // ����Ϊ0�������Ĺ�Ԫǰ N ��ͳһ�� -N ��ʾ
    int month;  // [1..12]
    int day;    // [1..31]
};

#ifdef USE_DATE_JDN      // ����ṹ���û��ṩ��һ�µ�����������
typedef Date_JDN Date;
#else
typedef Date_YMD Date;
#endif

//���ڳ�ʼ��: ��ִ����������֮ǰ�����ȳ�ʼ��!
//  ��������Ч���򷵻� false, ���򷵻� true.
bool dateInit(Date& theDate, int y, int m, int d);
bool dateInit(Date& theDate, Date_YMD ymd);
bool dateInit(Date& theDate, int julianDayNum);


//
//  Part II    ��ȡ�������ԵĲ���
//

// ���ָ�������Ƿ���Ч��
// ����Ч���򷵻� false, ���򷵻� true.
bool isValidDate(const Date& theDate);

int yearOfDate(const Date& theDate);  // ��ȡ�����е����
int monthOfDate(const Date& theDate); // ��ȡ�����е��·�
int dayOfDate(const Date& theDate);   // ��ȡ�����е�����

// �������ڶ�Ӧ�����ڡ�����ֵ�Ķ�Ӧ��ϵΪ��
// [0, 1, ..., 6] => [Sunday, Monday, ..., Saturday ]
int dayOfWeek(const Date& theDate);
int dayOfWeek(int year, int month, int day);

// ����ָ�������ڵ��굱���е���ţ��ڼ��죩.
// 1��Ϊ��1�죬��˷���1.
int dayOfMonth(const Date& theDate);
int dayOfMonth(int year, int month, int day);

// ����ָ�������ڵ����е���ţ��ڼ��죩.
// 1��1��Ϊ��1�죬��˷���1.
int dayOfYear(const Date& theDate);
int dayOfYear(int year, int month, int day);

//�ж�ָ����ݻ�ָ������������Ƿ�Ϊ����.
//  �����򷵻� 1, ���򷵻� 0.
int isLeapYear(int year);
int isLeapYear(const Date& theDate);

// ��ȡָ�����»�ָ���������·ݵ�����������ָ���µ����һ��ı�ţ���
int lastDayOfMonth(int year, int month);
int lastDayOfMonth(const Date& theDate);



//
//  Part III   �������ڵ���������
//


// ����ת��Ϊ�ַ������ô����� ISO 8601:2004 ���ڸ�ʽ�� "YYYY-MM-DD"
std::string date2string(const Date& theDate);
// ����ת��Ϊ �ա��¡�����������
Date_YMD date2YMD(const Date& theDate);
// ����ת��Ϊ������
int date2JulianDay(const Date& theDate);


// ���� d1-d2 ֮�������.
// �����ظ�ֵ����ʾ d1 < d2.
int daysBetweenDate(const Date& d1, const Date& d2);

// add or substract |n| days, and return the result date.
Date dateAddDays(const Date& d,  int n);  // return d + n��

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
