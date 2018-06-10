/**********************************************************************
 * ϰ�� P141 Exercise 6.6.17
 *
 * ���庯�� itoa(int i,char b[])�� ��������iת��Ϊ�ַ������洢�� b[] �У�
 *   ������ b.
 *
 * �� �� �ˣ������� @XiDian University
 * ����ʱ�䣺 2017.5
 *
 */

/**********************************************************************
 * ʵ��˵����
 * 0. ���� itoa() ��������� b[] �ռ��㹻;
 * 1. ���ļ�ʵ������ĿҪ��ĺ��� itoa()��
 * 2. ͬʱ������ָ����(2��3��4��...��16)���Ʋ����ַ�������
 *    �����÷�ʮ���Ʋ����ַ���ʱ��ͳһ�������������޷���������
 */

#include <cstdio>   // C++ wrapper of <stdio.h> for sprintf();
#include <cstring>  // C++ wrapper of <string.h> for strlen()
#include <cstdlib>  // C++ wrapper of <stdlib.h> for exit()
#include <climits>  // C++ wrapper of <limits.h> for INT_MAX, INT_MIN

#include <iostream>  // for cout, cin, endl
#include <iomanip>   // for setw()����ʽ�����

using namespace std;


// ������� itoa() ����ĿҪ��ʵ�ֵĺ�����
// ���������������� main() ֮�����ʵ�֡�
// ������Ϊȫ�ֺ��������õı��������ܻᱨ�� or ������ʱ��
//   ���������ú�������Ӧ���ֺ����Ĳ���������ֵ���ͷ�����ĿҪ��
// ��Ҫ��ϵͳ�� stdlib.h �� itoa() �������ԱȵĻ���
// ��� itoa() Ҳ��Ҫ�����������߷ŵ��Լ������� namespace �У������
namespace OOPLAB
{
    char* itoa(int  n, char str[], int base =10);
}


// ִ��һ��������������C���Ա�׼�⺯�� sprintf() �Ľ���Ա�
//  ע�⣺��׼�⺯�� *printf() ��֧�� 8,10,16�����ֽ���
int test_case_for_std(int n, int base)
{
    char std_str[40];
    char my_str[40];

    cout << setw(3)  << base << " ";
    cout.flush();

    switch(base)
    {
    case 10:
        sprintf(std_str, "%d", n);
        break;
    case 8:
        sprintf(std_str, "0%o", n);
        break;
    case 16:
        sprintf(std_str, "0X%X", n);  // ʹ�ô�д A~F
        break;
    default:
        cout << "unsupported base " << base << endl;
        return 1;
    }

    OOPLAB::itoa(n, my_str, base);

    cout << setw(15) <<  n << setw(15) << my_str;
	if( strcmp( my_str, std_str ) == 0 )
	{
		cout << setw(15) << std_str << " -> yes" << endl;
		return 0;
	}
	else
	{
		cout << setw(15)  << std_str << " -> FAIL :-(" << endl;
		return 1;
	}
}


// ִ��Ԥ�ȱ��ƵĲ�������
void  test_cases()
{
    cout << "Start test cases..." << endl;

    cout << "BASE" << setw(15) << "decimal-num" << setw(15)
         << "Result" << setw(15)  << "Expected" << "    Passed?" << endl;
    cout << "+===========================================================+\n";

    int testValues[] =  // ���屻������
        {
            123, -123, 0123, -0123, 0x12f, -0x12F,
            INT_MAX,  // +2147483647
            -INT_MAX, // -2147483647
            INT_MIN   // -2147483648
        };

	int n_fail =0;
    unsigned n_values = sizeof(testValues) / sizeof(testValues[0]);
    for(unsigned k=0; k<n_values; ++k)
    {
        n_fail += test_case_for_std( testValues[k], 10);
        n_fail += test_case_for_std( testValues[k], 16);
        n_fail += test_case_for_std( testValues[k], 8);
    }

    cout << "+===========================================================+\n";

	if(n_fail != 0)
	{
		cout << endl << "I am ABORT for test failed." << endl << endl;
		exit(1);  // in stdlib.h, terminate the program
	}

	cout << "Test cases passed." << endl << endl;
}




int main()
{
    // ִ�� Ԥ�ȱ��ƵĲ�������, ����ʧ�ܣ����������
    test_cases();

    int  number;        // ���ı�ת��������
    char result[100];   // �� number ת���õ����ַ���
    long long big_number;  // Ϊ��ֹ���볬����������øñ���ȥ�� cin

    for( ; ; ) // loop for ever
    {
    	cout << "Press ^d(on linux) or ^z(on Windows) to exit." << endl;
    	cout << "Give me a decimal integer between [" << INT_MIN << ", " << INT_MAX << "]: ";
//    	cout.flush();

        cin >> big_number;   // Ϊ��ֹ���볬 int ��Χ���������ȶ��뵽 long long
        if( cin.eof() )
        {
            cout << "\n*** end of input." << endl;
            break;
        }
        else if( !cin ) //just means: cin.fail() == true
        {
            // ������Ч���룬�Ա��ڼ���������һ������
            char discard[100];
            cin.clear();  // reset state to good
            cin.getline(discard, sizeof(discard));
            cout << "\nYour input is invalidate: \"" << discard << "\"" << endl << endl;
            continue;
        }

        if( big_number < INT_MIN || big_number > INT_MAX )
        {
            cout << "\n\aNumber " << big_number << " is out of range." << endl << endl;
            continue;
        }

        number = int(big_number);  // �� long long ת��Ϊ int

        OOPLAB::itoa( number, result, 10 );
        cout << " base-10:\"" << result << "\"" << endl;
        OOPLAB::itoa( number, result, 16 );
        cout << " base-16:\"" << result << "\"" << endl;
        OOPLAB::itoa( number, result, 8 );
        cout << " base-8 :\"" << result << "\"" << endl;
        OOPLAB::itoa( number, result, 4 );
        cout << " base-4 :\"" << result << "\"" << endl;
        OOPLAB::itoa( number, result, 2 );
        cout << " base-2 :\"" << result << "\"" << endl;
        OOPLAB::itoa( number, result, 36 );
        cout << " base-36:\"" << result << "\"" << endl;

        cout << endl;
    } // end of loop

    return 0;
}


//
//
//   ������ WXQ_itoa() ��ʵ�֣�������ĸ�����������
//
//


namespace OOPLAB
{

// �������� (Helper Function)��
// ȫ��������Ƕ�׵� ���� namespace �У��������Ϊ��ǿ�� �ֲ������ԡ�
namespace
{
    
// ������ n(0 <= n < max_base ) ת��Ϊ��Ӧ���ַ�
inline char  __digit2Char(unsigned int n)
{
    const unsigned max_base = 36; //֧�ֵ�������
    const char  symbol[max_base +1] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    if( n <  max_base )
        return symbol[n];
    else
        return '?';
}

// �������� (Helper Function)
// ����ʮ�����ַ���
void __itoa10(int n,  char str[])
{
//Step 1: �ж� ����
    // Ϊ��ֹ ��С�����ľ���ֵ���� int ��Χ�����洦���о�ʹ�� long long
    long long nLL    = n;
    bool isNegative = false; // n �Ƿ�Ϊ����

    if( n < 0 )
    {
        isNegative = true;
        nLL *= -1;
    }

//Step 2: ����д���ܵ�'+',��-'���ٴ�ĩλ��ʼ��������λ�õ���Ӧ�ַ�
    int tail =-1 ;  // tail index of string
    if( isNegative ) str[++tail] = '-';  //��������

    // ����շת���������������ٽ������������෴˳����֯���ɽ��.
    for(   ;   ;  )
    {
        str[++tail] = __digit2Char(nLL % 10);
        if( 0 == (nLL = nLL / 10) )
            break;
    }
    str[tail+1] = '\0'; // !!! Very very important

//Step 3: ���˿��ܴ��ڵķ����⣬���������������������֯.
    int start = 0 ;
    if( isNegative ) start = 1;  // ��������

    for(char c=0 ; start < tail; )
    {
        c = str[start];
        str[start] = str[tail];
        str[tail] = c;

        ++start; --tail;
    }
    return;
}

} // ���� namespace ����


// �����������ĿҪ��ģ������� base ����չ�ģ�����ָ������baseת��.
char* itoa(int  n, char str[], int base)
{
    if( base == 10 )
    {
        __itoa10(n, str);
        return str;
    }

    // �����շ�10���Ʋ����ַ���ʱ����ͳһ�� n ���� unsigned ����.
    // ������׼���е� *printf() ��ͬ.
    // ������ 8  ���ƴ�������ǰ׺Ϊ  "0" ;
    //   ���� 16 ���ƴ�������ǰ׺Ϊ "0X".

//Step 1: ת��Ϊ �޷��� int��ע�����ﲻ��ת��Ϊ unsigned long long �ģ���
    // ��������� :-)
    unsigned int uValue = (unsigned int)(n);

//Step 2: �Ȳ�����Ҫ��ǰ׺�ַ�
    int tail =-1 ;  // tail index of string 
    if( base == 8 )           // ����8/16�����������Ĵ�ͷ��־�ַ�
        str[++tail] = '0';
    else if( base == 16 )
    {
        str[++tail] = '0';   str[++tail] = 'X';
    }

    // ����շת���������������ڽ������������෴˳����֯���ɽ��.
    for(   ;   ;  )
    {
        str[++tail] = __digit2Char(uValue % base);
        if( 0 == (uValue = uValue / base) )
            break;
    }
    str[tail+1] = '\0'; // !!! Very very important

//Step 3: ��10 ���ƶ�����unsigned
    int start = 0 ;
    if( base == 8 )           // ����8/16�����������Ĵ�ͷ��־�ַ�
        start += 1;
    else if( base == 16 )
        start += 2;

    // ���˴�ͷ���ַ������⣬���������������������֯.
    for(char c=0 ; start < tail; ) 
    {
        c = str[start];
        str[start] = str[tail];
        str[tail] = c;

        ++start; --tail;
    }
    return str;
}

} // end of namespace OOPLAB

// ============   T h e   E n d ============
