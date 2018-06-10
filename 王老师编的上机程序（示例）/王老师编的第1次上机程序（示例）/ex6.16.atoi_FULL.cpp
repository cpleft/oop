/**********************************************************************
 * 习题 P141 Exercise 6.6.16
 *
 * 定义函数 int atoi(const char*)， 要求输入字符串的形式为：
 * 1. 十进制、八进制、十六进制形式的数字串，即任一个有效的 C/C++ 语言中的
 *        整数字面量。
 *    无论什么进制，正负号必须先出现或不出现，然后紧跟：
 *   1.1 十进制数字串：
 *       '1'~'9'中的任一字符开始，后跟0或若干位 10 进制数字字符'0'~'9'；
 *   1.2 八进制数字串：
 *       以数字字符 '0' 打头，后跟0或若干位8进制数字字符('0'~'7')；
 *   1.3 十六进制数字串：
 *       以 "0x" 或 "0X" 打头，后跟0或若干位16进制数字字符('0'~'7','a'~'f','A'~'F').
 *
 * 2. 上述字符串中，除正负号外，任何一个"字符"可采用 C/C++ 允许的字符字面量形式：
 *   2.1 常规形式，如字符 '0' 就用 '0' 表示，字符 '1' 就用'1' 表示，...；
 *   2.2 八进制形式，以表示转义的 '\' 字符打头，后跟1、2、3位8进制
 *        数字字符，如数字字符 '0' 用 '\60' 或 '\060', '1' 用 '\61' 或 '\061'，...；
 *   2.3 16进制形式，以 '\x' 或 '\X' 打头，后跟 1 或 2 位16进制数字字符，
 *        如字符 '0' 用 '\x30', '1' 用 '\x31'，...。
 *
 * 编 制 人：王献青 @XiDian University
 * 编制时间：2017.5
 */

/**********************************************************************
 * 实现说明：
 * 0. 为便于理解程序，本文实现中，对于打头可选的 + - 号、表示 8 进制数的打头
 *    字符 "0"、以及表示 16 进制数的打头字符串 "0x" 和 "0X"，仅支持上述
 *    的“2.1 常规形式”。
 *
 * 1. main() 可从【命令行参数】中读取（表示整数的）字符串，并将每个串转换为
 *    相应整数、并以 10 进制方式显示出来.
 *
 * 2. main() 可从【标准输入设备】读取 若干 表示整数的字符串，每读取一个，
 *    则将之转换为相应整数，并以 10 进制方式显示出来.
 *
 * 3. 本文中预先编写了一些调用 atoi() 的测试用例，并由 main() 调用。详见函数
 *     test_case(), test_cases().
 */


#include <cstring>  // C++ wrapper of <string.h>, for strlen()
#include <cstdlib>  // C++ wrapper of <stdlib.h>, for exit()
#include <cctype>   // C++ wrapper of <ctype.h>,  for isspace()
#include <climits>  // C++ wrapper of <limits.h>, for INT_MAX, INT_MIN

#include <iostream> // for std::cin, std::cout
#include <iomanip>  // for std::setw() 格式化输出
#include <string>   // for std::string

using namespace std;

// 下面这个 atoi() 是题目要求实现的函数，
// 这里先声明，再在 main() 之后给出实现。
//
// 当定义为全局函数时，你用的编译器[可能]会报警 or 报错。此时就
//   重新命名该函数，但应保持函数的参数表、返回值类型符合题目要求。
// 或 
//   要和标准库 stdlib.h 的 atoi() 计算结果对比的话，
// 你的 atoi() 也需要重命名。
//
// 解决上述两个问题的另一种方法如下面这样，放到自己命名的 namespace 中：
namespace OOPLAB
{
    int atoi(const char* str);
}


void  test_cases(); // 先声明，最后给出定义。

int main(int argc, char* argv[])
{
    // 先执行 预先编制的测试用例, 若有失败，则结束程序。
    test_cases();

	int  value;

    // 若命令行参数指明了 1 个及以上整数字符串，则仅处理之
    if( argc > 1 )
    {
        --argc; ++argv; // jump argv[0]-- this executable file Path(Name)

        for(int i=0; argc > 0; ++i, --argc)
        {
            value = OOPLAB::atoi( argv[i] ); // call my atoi()
            //以十进制显示整数
            cout << "argument \"" << argv[i] << "\" is " << value << endl;
        }

        return 0;
    }


    // 从标准输入设备读入若干数字串（可能不是有效的数字串）
    string  numberString;
    while( cin )
    {
    	cout << "Press ^d on Linux, or ^z on Microsoft Windows to exit." << endl;
    	cout << "Input a number: " ;  // no end-of-line
//    	cout.flush(); // flush buffer of std::cout to display above text

        cin >> numberString;
        if( !cin ) break; // for end-of-input on ^Z

        value = OOPLAB::atoi( numberString.c_str() ); // call my atoi()
        //以十进制显示整数
        cout << "string \"" << numberString << "\" = " << value << endl;
    }

    return 0;
}


//
//
//   下面是 atoi() 的实现，及所需的两个辅助函数定义
//
//
namespace OOPLAB
{

// 辅助函数需提前声明，后面再给出定义.
int parseOneChar(const char* & pc);

// *****************   题目要求实现的函数  ******************
/*
 * 将字符串表示的整数字面量转换为计算机内部表示的整数。
 * 参数:
 *    str  表示一个整数的字符串，其中每个字符都可以本文最开始处所列
 *        形式出现。
 * 对 str 进行自左向右扫描，直到遇到串结束 或者 遇到无效字符为止。
 */
int atoi(const char* str)
{
    bool isNegative = false; // 是否为“负”整数
    int  base = 10;          // str 的进制标识，缺省为 10 进制整数串

    if(NULL == str) return 0;     // NULL address

    const char* p = str;  // 下面用指针 p 对 str 进行自左向右扫描
    while( isspace(*p) )  // 跳过开始处存在的连续空白字符（空格、制表符等）
        ++p;

//// Step 1: 判断正负（识别可选的正负号）
     if( *p == '-' || *p == '+')
     {
    	 if(*p == '-')
    		 isNegative = true;

    	 ++p;  // jump the leading '-' or '+'
     }

//// Step 2: 判断进制
     if( *p == '0' )
     {
         base = 8;     // str 可能是 8 或 16 进制数
         ++p;  // 跳过打头的 '0'
         if( (*p == 'x' || *p == 'X') )
         {
             base = 16;
             ++p;  // 继续跳过 'x' 或 'X'
         }
     }
     if( *p == '\0') return 0;  // 字符串结束了，就返回 0

//// Step 3: 边扫描，边按相应进制进行转换

     int  value   =  0;   // 存放 返回值
     bool inRange = true; // 数值是否在 int 类型的可表示范围
     int  char_value = '\0'; // 某一个字符对应的整数值
     for(
            int lastValue=0   //定义局部量，存储最近一次转换所得整数
          ; *p != '\0'        // 串是否结束
          ;   // p 的变化 由循环体负责
        )
     {
    	 // 将 p 指向的、表示1位数字的字符(序列)转换为对应的整数。
    	 // 如: '0' --> 0, "\060" --> 0, "\x30" --> 0。
    	 // 当调用返回到这里时，
    	 // p 就指向了 表示下一个数字的字符（或序列的开始处）。
         char_value = parseOneChar( p );

         // 当遇见一个无效字符则返回当前已经得到的整数.
         // 这个处理和标准 atoi() 的做法一致.
         if(char_value < 0 || char_value >= base)
         {
        	 break;
         }
         value = value * base + char_value; // 按指定进制转换为整数

         // 若超出了 int 的表示范围则不再转换。
         if(value < lastValue)
         {
        	 //cerr << "[overflow detected]:" << lastValue << "->" << value << endl;
        	 inRange = false;
        	 break;
         }
         lastValue = value;
     }

// Last step: 处理正负性、是否超出 int 的范围.

     if( inRange ) // 未超出 int 的可表示范围的处理
     {
        if( isNegative )
            return -value;
        else
            return value;
     }
     else  // 超出 int 的可表示范围时，
     {
        // C/C++ 标准库的 atoi 的行为是‘未定义的’.
        // 这里采用我使用的 编译器的做法。
        return value; // 直接返回该值.
     }
}


// 辅助函数 (Helper Function)

// 将（8/10/16进制的）数字字符 c 转换为其表示的数字(即对应整数）
int  numberChar2Int(const char c)
{
    switch(c)
    {
        case '0': //在 ASCII 字符集中对应整数 48，其他类似
                   return 0;
        case '1':  return 1;
        case '2':  return 2;
        case '3':  return 3;
        case '4':  return 4;
        case '5':  return 5;
        case '6':  return 6;
        case '7':  return 7;
        case '8':  return 8;
        case '9':  return 9;
        case 'a': case 'A':  return 10;
        case 'b': case 'B':  return 11;
        case 'c': case 'C':  return 12;
        case 'd': case 'D':  return 13;
        case 'e': case 'E':  return 14;
        case 'f': case 'F':  return 15;
        default: return -1; // 无效字符
    }
}

/* Helper Function: parseOneChar
 *
 * 将表示整数的字符（或转义字符序列）转换为其表示的整数.
 *   如将 '0' ， '\060' ， '\X30' 转换为 0.
 *   关于字符字面量的多种形式，请参见本文最开始的注释。
 * 参数：
 *   pc [in,out] 指针引用类型，
 *         调用前，指向一个字符, 或转义字符序列之首字符；
 *         函数返回时，指向下一个字符, 或下一个转义字符序列之首字符。
 * 返回值：
 * (1)若字符或转义字符序列有效（即表示一个有效的数字字符），则返回 pc 指向的
 *    字符或转义序列对应的整数, 并修改 pc，令其指向下一个字符或转义字符序列的开始；
 * (2) 否则返回-1，并令 pc 指向第一个无效字符.
 */
int parseOneChar(const char* & pc)
{
    int value = 0; // 字符（或转义字符序列）对应的整数
    int cBase = 0; // 字符（或转义字符序列）之形式区分，
                   // 0表示常规形式, 8表示8进制表示的字符，16表示16进制表示的字符

// Step 1: 先判断是常规字符、还是 8 或16 进制表示的转义字符序列
    switch( *pc )
    {
    case '\0':  // 字符串末尾，不应该存在这种情况。若有则调用者的逻辑有问题.
        return 0;

    case '\\':  // 用 8 或 16 进制表示的转义字符序列的开始标志
        ++ pc;  // jump the char '\'
        cBase = 8; // 可能是 8 进制 或 16 进制
        if( *pc == 'x' || *pc == 'X' )  // 16 进制
        {
            ++ pc;  // jump the char 'x' or 'X'
            cBase = 16;
        }
        break;

    default:  // 常规形式的字符
        value = numberChar2Int( *pc );
        if( value >= 0 ) ++pc;  // 令 pc 指向下一个字符
        return value;
    }


// Step 2.1: 计算 8 进制表示的转义字符序列所表示的数值
    if(cBase == 8)
    {
        value = 0;
        // 先得到 8 进制表示的字符之机内编码，如
        // "\060" 表示的字符就是 '0'（机内码为48）
        int n = 0; // 已经看到的 8 进制数的个数，最多3个
        for(int vc=0 ; *pc != '\0' && n < 3; ++n, ++pc)
        {
            vc = numberChar2Int( *pc );
            if(0 <= vc && vc <= 7)  // 8进制数
                value = value * 8 + vc;
            else
            	break;  // 8进制表示 结束了
        }
        //此循环结束时，
        // 1. 若转义序列正确，则 pc 应指向下一个待分析的字符之开始；
        //    若转义序列不正确，则 pc 应指向第1个错误的字符；
        // 2. 恰好将 8 进制形式的转义字符序列转换结束,
        //    且 value 中存储的就是 转换后字符的对应的字符编码.
        // 
        //如 对于输入字符字面量 “\060” ，上面代码将其先转换为字符'0'的
        //   编码整数 48。这是ASCII中字符'0' 的编码。

        // 接着，还需将该字符转换为对应的整数。如将字符'0'转换为整数0
        value = numberChar2Int( value );

        return value;    // Done
    }

// Step 2.2: 计算 16 进制表示的转义字符序列所表示的数值

    value = 0;
    // 先得到 16 进制表示的字符之机内编码，如
    // \x30 表示的字符就是 '0'（机内码为48）
    int n=0; //已经看到的 16 进制字符个数，最多2个
    for( int vc=0; *pc != '\0' && n<2; ++n, ++pc)
    {
        vc = numberChar2Int( *pc );
        if(0 <= vc && vc <= 15)
            value = value * 16 + vc;
        else
            break; // 16 进制表示 结束了
    }
    //此循环结束时，
    // 1. 若转义序列正确，则 pc 应指向下一个待分析的字符之开始；
    //    若转义序列不正确，则 pc 应指向第1个错误的字符；
    // 2. 恰好将 16 进制形式的转义字符序列转换结束,
    //    且 value 中存储的就是 转换后字符的对应的字符编码.

    // 接着还需将该字符转换为整数.
    //  如 对于输入字符字面量 \x30 ，将其先转换为整数 48，
    // 这是ASCII中字符'0' 的编码，再将其转换为整数0
    value = numberChar2Int( value );
    return value;
}

} // end of namespace OOPLAB



/**
 * 函数 test_case 功能：
 *   执行一个测试用例. 该函数被 test_cases() 调用。
 * 参数：
 *   str_number 任意字符串，将被转换为整数
 *   expected   期望的被转换结果
 * 返回值：
 *   0  测试成功
 *   1  测试不成功，说明 atoi() 的实现有缺陷
*/
int test_case(const char* str_number,  int  expected)
{
    int  value = OOPLAB::atoi(str_number);

    cout << setw(15) << std::left  << str_number
         << setw(15) << std::right << value
         << setw(15) << std::right << expected;

	if( value == expected )
	{
		cout <<  "    passed." << endl;
		return 0;
	}
	else
	{
		cout <<  "    FAIL!" << endl;
		return 1;
	}
}


// 函数 test_cases: 执行 预先编制的若干测试用例
// 该函数被 main() 调用。
void  test_cases()
{
    cout << setw(15) << std::left  << "string"
         << "     OOPLAB::atoi  "
         << "    compiler"  // 编译器的翻译结果
         << endl;
    cout << "========================================================="
         << endl;

    struct {
        const char* str;
        int expected;
    } testData[] = {
      // 第1类测试用例：正确的字符串
        {"123",            123},
        {"-123",          -123},
        {"0123",          0123},  // 1*8*8+2*8+3 =83
        {"-0123",        -0123},
        {"0x123",        0x123},  // (1*16 + 2)*16 + 3 =291
        {"-0X123",      -0X123},
	    {"01\\062\\063",  0123}, // \062 is '2', \063 is '3'
	    {"1\\062\\x33",    123}, // \062 is '2', \x33 is '3'
	    {"0xF\\x66A",    0xFFA}, // \x66 is character 'F'

      // 第2类测试用例：存在无效字符、不正常的串
        {"  +123.99",   +123},  // 打头的空白字符应忽略
        {"-0789",         -7},  // 8进制表示法，故仅读取"-07"
        {"-",              0},
        {"+",              0},
        {"0",              0},
        {"0x",             0},
        {"",               0},
        
      // 第3类测试用例： 边界 情况
        {"0xFFFFFFFF",  -1},
        {"0x80000000",  INT_MIN}, // min value of int: -2147483648
        {"0x80000001",  INT_MIN+1}, // -2147483647
        {"0x7FFFFFFF",  INT_MAX},  // max value of int: 2147483647

        // 下面这两行会引起编译器告警：
	    // warning: integer overflow in expression
        {"0x7FFFFFFf",  INT_MIN-1}, // ???
        {"0x80000000",  INT_MAX+1}, // ??? 编译器将 2147483647+1 变为多少

        {NULL, 0}  // terminator element
    };

 	int n_fail =0; //失败的测试用例数目
    for(size_t i = 0; testData[i].str != NULL; ++i)
    {
        n_fail += test_case(testData[i].str, testData[i].expected);
    }

    cout << "========================================================="
         << endl;

	if(n_fail != 0)
	{
		cout << endl << "I am ABORT for test-case failed." << endl << endl;
		exit(1);  // in stdlib.h, terminates the program
	}

	cout << endl << "Test cases passed." << endl << endl;
}



// ============   T h e   E n d ============
