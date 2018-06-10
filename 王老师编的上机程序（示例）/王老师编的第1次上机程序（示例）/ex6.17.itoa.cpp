/**********************************************************************
 * 习题 P141 Exercise 6.6.17
 *
 * 定义函数 itoa(int i,char b[])， 它将整数i转换为字符串，存储到 b[] 中，
 *   并返回 b.
 *
 * 编 制 人：王献青 @XiDian University
 * 编制时间： 2017.5
 *
 */

/**********************************************************************
 * 实现说明：
 * 0. 假设 itoa() 的数组参数 b[] 空间足够;
 * 1. 本文件实现了题目要求的函数 itoa()；
 * 2. 同时还允许按指定的(2或3或4或...或16)进制产生字符串。但
 *    当采用非十进制产生字符串时，统一将整数看作是无符号整数。
 */

#include <cstdio>   // C++ wrapper of <stdio.h> for sprintf();
#include <cstring>  // C++ wrapper of <string.h> for strlen()
#include <cstdlib>  // C++ wrapper of <stdlib.h> for exit()
#include <climits>  // C++ wrapper of <limits.h> for INT_MAX, INT_MIN

#include <iostream>  // for cout, cin, endl
#include <iomanip>   // for setw()，格式化输出

using namespace std;


// 下面这个 itoa() 是题目要求实现的函数，
// 这里先声明，再在 main() 之后给出实现。
// 当定义为全局函数，你用的编译器可能会报警 or 报错。此时就
//   重新命名该函数，但应保持函数的参数表、返回值类型符合题目要求。
// 或要和系统中 stdlib.h 的 itoa() 计算结果对比的话，
// 你的 itoa() 也需要重命名，或者放到自己命名的 namespace 中，如这里：
namespace OOPLAB
{
    char* itoa(int  n, char str[], int base =10);
}


// 执行一个测试用例，与C语言标准库函数 sprintf() 的结果对比
//  注意：标准库函数 *printf() 仅支持 8,10,16这三种进制
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
        sprintf(std_str, "0X%X", n);  // 使用大写 A~F
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


// 执行预先编制的测试用例
void  test_cases()
{
    cout << "Start test cases..." << endl;

    cout << "BASE" << setw(15) << "decimal-num" << setw(15)
         << "Result" << setw(15)  << "Expected" << "    Passed?" << endl;
    cout << "+===========================================================+\n";

    int testValues[] =  // 定义被测整数
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
    // 执行 预先编制的测试用例, 若有失败，则结束程序。
    test_cases();

    int  number;        // 下文被转换的整数
    char result[100];   // 将 number 转换得到的字符串
    long long big_number;  // 为防止输入超大的整数，用该变量去读 cin

    for( ; ; ) // loop for ever
    {
    	cout << "Press ^d(on linux) or ^z(on Windows) to exit." << endl;
    	cout << "Give me a decimal integer between [" << INT_MIN << ", " << INT_MAX << "]: ";
//    	cout.flush();

        cin >> big_number;   // 为防止输入超 int 范围的整数而先读入到 long long
        if( cin.eof() )
        {
            cout << "\n*** end of input." << endl;
            break;
        }
        else if( !cin ) //just means: cin.fail() == true
        {
            // 忽略无效输入，以便于继续读入下一个整数
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

        number = int(big_number);  // 将 long long 转换为 int

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
//   下面是 WXQ_itoa() 的实现，及所需的辅助函数定义
//
//


namespace OOPLAB
{

// 辅助函数 (Helper Function)，
// 全部定义在嵌套的 匿名 namespace 中，这仅仅是为了强调 局部可用性。
namespace
{
    
// 将整数 n(0 <= n < max_base ) 转换为对应的字符
inline char  __digit2Char(unsigned int n)
{
    const unsigned max_base = 36; //支持的最大进制
    const char  symbol[max_base +1] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    if( n <  max_base )
        return symbol[n];
    else
        return '?';
}

// 辅助函数 (Helper Function)
// 产生十进制字符串
void __itoa10(int n,  char str[])
{
//Step 1: 判断 正负
    // 为防止 最小负数的绝对值超出 int 范围，下面处理中均使用 long long
    long long nLL    = n;
    bool isNegative = false; // n 是否为负数

    if( n < 0 )
    {
        isNegative = true;
        nLL *= -1;
    }

//Step 2: 先填写可能的'+',’-'，再从末位开始，向左逐位得到对应字符
    int tail =-1 ;  // tail index of string
    if( isNegative ) str[++tail] = '-';  //产生负号

    // 利用辗转相除法获得余数；再将所得余数按相反顺序组织即可结果.
    for(   ;   ;  )
    {
        str[++tail] = __digit2Char(nLL % 10);
        if( 0 == (nLL = nLL / 10) )
            break;
    }
    str[tail+1] = '\0'; // !!! Very very important

//Step 3: 除了可能存在的符号外，将后面的余数按“倒序”组织.
    int start = 0 ;
    if( isNegative ) start = 1;  // 跳过负号

    for(char c=0 ; start < tail; )
    {
        c = str[start];
        str[start] = str[tail];
        str[tail] = c;

        ++start; --tail;
    }
    return;
}

} // 匿名 namespace 结束


// 这个函数是题目要求的，但参数 base 是扩展的，允许按指定进制base转换.
char* itoa(int  n, char str[], int base)
{
    if( base == 10 )
    {
        __itoa10(n, str);
        return str;
    }

    // 当按照非10进制产生字符串时，则统一将 n 当作 unsigned 处理.
    // 这点与标准库中的 *printf() 相同.
    // 但对于 8  进制串，增加前缀为  "0" ;
    //   对于 16 进制串，增加前缀为 "0X".

//Step 1: 转换为 无符号 int，注意这里不能转换为 unsigned long long 的！！
    // 你可以试试 :-)
    unsigned int uValue = (unsigned int)(n);

//Step 2: 先产生必要的前缀字符
    int tail =-1 ;  // tail index of string 
    if( base == 8 )           // 产生8/16进制字面量的打头标志字符
        str[++tail] = '0';
    else if( base == 16 )
    {
        str[++tail] = '0';   str[++tail] = 'X';
    }

    // 利用辗转相除法获得余数；在将所得余数按相反顺序组织即可结果.
    for(   ;   ;  )
    {
        str[++tail] = __digit2Char(uValue % base);
        if( 0 == (uValue = uValue / base) )
            break;
    }
    str[tail+1] = '\0'; // !!! Very very important

//Step 3: 非10 进制都看作unsigned
    int start = 0 ;
    if( base == 8 )           // 跳过8/16进制字面量的打头标志字符
        start += 1;
    else if( base == 16 )
        start += 2;

    // 除了打头的字符序列外，将后面的余数按“倒序”组织.
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
