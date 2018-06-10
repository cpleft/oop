/**********************************************************************
 * ϰ�� P141 Exercise 6.6.16
 *
 * ���庯�� int atoi(const char*)�� Ҫ�������ַ�������ʽΪ��
 * 1. ʮ���ơ��˽��ơ�ʮ��������ʽ�����ִ�������һ����Ч�� C/C++ �����е�
 *        ������������
 *    ����ʲô���ƣ������ű����ȳ��ֻ򲻳��֣�Ȼ�������
 *   1.1 ʮ�������ִ���
 *       '1'~'9'�е���һ�ַ���ʼ�����0������λ 10 ���������ַ�'0'~'9'��
 *   1.2 �˽������ִ���
 *       �������ַ� '0' ��ͷ�����0������λ8���������ַ�('0'~'7')��
 *   1.3 ʮ���������ִ���
 *       �� "0x" �� "0X" ��ͷ�����0������λ16���������ַ�('0'~'7','a'~'f','A'~'F').
 *
 * 2. �����ַ����У����������⣬�κ�һ��"�ַ�"�ɲ��� C/C++ ������ַ���������ʽ��
 *   2.1 ������ʽ�����ַ� '0' ���� '0' ��ʾ���ַ� '1' ����'1' ��ʾ��...��
 *   2.2 �˽�����ʽ���Ա�ʾת��� '\' �ַ���ͷ�����1��2��3λ8����
 *        �����ַ����������ַ� '0' �� '\60' �� '\060', '1' �� '\61' �� '\061'��...��
 *   2.3 16������ʽ���� '\x' �� '\X' ��ͷ����� 1 �� 2 λ16���������ַ���
 *        ���ַ� '0' �� '\x30', '1' �� '\x31'��...��
 *
 * �� �� �ˣ������� @XiDian University
 * ����ʱ�䣺2017.5
 */

/**********************************************************************
 * ʵ��˵����
 * 0. Ϊ���������򣬱���ʵ���У����ڴ�ͷ��ѡ�� + - �š���ʾ 8 �������Ĵ�ͷ
 *    �ַ� "0"���Լ���ʾ 16 �������Ĵ�ͷ�ַ��� "0x" �� "0X"����֧������
 *    �ġ�2.1 ������ʽ����
 *
 * 1. main() �ɴӡ������в������ж�ȡ����ʾ�����ģ��ַ���������ÿ����ת��Ϊ
 *    ��Ӧ���������� 10 ���Ʒ�ʽ��ʾ����.
 *
 * 2. main() �ɴӡ���׼�����豸����ȡ ���� ��ʾ�������ַ�����ÿ��ȡһ����
 *    ��֮ת��Ϊ��Ӧ���������� 10 ���Ʒ�ʽ��ʾ����.
 *
 * 3. ������Ԥ�ȱ�д��һЩ���� atoi() �Ĳ������������� main() ���á��������
 *     test_case(), test_cases().
 */


#include <cstring>  // C++ wrapper of <string.h>, for strlen()
#include <cstdlib>  // C++ wrapper of <stdlib.h>, for exit()
#include <cctype>   // C++ wrapper of <ctype.h>,  for isspace()
#include <climits>  // C++ wrapper of <limits.h>, for INT_MAX, INT_MIN

#include <iostream> // for std::cin, std::cout
#include <iomanip>  // for std::setw() ��ʽ�����
#include <string>   // for std::string

using namespace std;

// ������� atoi() ����ĿҪ��ʵ�ֵĺ�����
// ���������������� main() ֮�����ʵ�֡�
//
// ������Ϊȫ�ֺ���ʱ�����õı�����[����]�ᱨ�� or ������ʱ��
//   ���������ú�������Ӧ���ֺ����Ĳ���������ֵ���ͷ�����ĿҪ��
// �� 
//   Ҫ�ͱ�׼�� stdlib.h �� atoi() �������ԱȵĻ���
// ��� atoi() Ҳ��Ҫ��������
//
// ������������������һ�ַ����������������ŵ��Լ������� namespace �У�
namespace OOPLAB
{
    int atoi(const char* str);
}


void  test_cases(); // �����������������塣

int main(int argc, char* argv[])
{
    // ��ִ�� Ԥ�ȱ��ƵĲ�������, ����ʧ�ܣ����������
    test_cases();

	int  value;

    // �������в���ָ���� 1 �������������ַ������������֮
    if( argc > 1 )
    {
        --argc; ++argv; // jump argv[0]-- this executable file Path(Name)

        for(int i=0; argc > 0; ++i, --argc)
        {
            value = OOPLAB::atoi( argv[i] ); // call my atoi()
            //��ʮ������ʾ����
            cout << "argument \"" << argv[i] << "\" is " << value << endl;
        }

        return 0;
    }


    // �ӱ�׼�����豸�����������ִ������ܲ�����Ч�����ִ���
    string  numberString;
    while( cin )
    {
    	cout << "Press ^d on Linux, or ^z on Microsoft Windows to exit." << endl;
    	cout << "Input a number: " ;  // no end-of-line
//    	cout.flush(); // flush buffer of std::cout to display above text

        cin >> numberString;
        if( !cin ) break; // for end-of-input on ^Z

        value = OOPLAB::atoi( numberString.c_str() ); // call my atoi()
        //��ʮ������ʾ����
        cout << "string \"" << numberString << "\" = " << value << endl;
    }

    return 0;
}


//
//
//   ������ atoi() ��ʵ�֣������������������������
//
//
namespace OOPLAB
{

// ������������ǰ�����������ٸ�������.
int parseOneChar(const char* & pc);

// *****************   ��ĿҪ��ʵ�ֵĺ���  ******************
/*
 * ���ַ�����ʾ������������ת��Ϊ������ڲ���ʾ��������
 * ����:
 *    str  ��ʾһ���������ַ���������ÿ���ַ������Ա����ʼ������
 *        ��ʽ���֡�
 * �� str ������������ɨ�裬ֱ������������ ���� ������Ч�ַ�Ϊֹ��
 */
int atoi(const char* str)
{
    bool isNegative = false; // �Ƿ�Ϊ����������
    int  base = 10;          // str �Ľ��Ʊ�ʶ��ȱʡΪ 10 ����������

    if(NULL == str) return 0;     // NULL address

    const char* p = str;  // ������ָ�� p �� str ������������ɨ��
    while( isspace(*p) )  // ������ʼ�����ڵ������հ��ַ����ո��Ʊ���ȣ�
        ++p;

//// Step 1: �ж�������ʶ���ѡ�������ţ�
     if( *p == '-' || *p == '+')
     {
    	 if(*p == '-')
    		 isNegative = true;

    	 ++p;  // jump the leading '-' or '+'
     }

//// Step 2: �жϽ���
     if( *p == '0' )
     {
         base = 8;     // str ������ 8 �� 16 ������
         ++p;  // ������ͷ�� '0'
         if( (*p == 'x' || *p == 'X') )
         {
             base = 16;
             ++p;  // �������� 'x' �� 'X'
         }
     }
     if( *p == '\0') return 0;  // �ַ��������ˣ��ͷ��� 0

//// Step 3: ��ɨ�裬�߰���Ӧ���ƽ���ת��

     int  value   =  0;   // ��� ����ֵ
     bool inRange = true; // ��ֵ�Ƿ��� int ���͵Ŀɱ�ʾ��Χ
     int  char_value = '\0'; // ĳһ���ַ���Ӧ������ֵ
     for(
            int lastValue=0   //����ֲ������洢���һ��ת����������
          ; *p != '\0'        // ���Ƿ����
          ;   // p �ı仯 ��ѭ���帺��
        )
     {
    	 // �� p ָ��ġ���ʾ1λ���ֵ��ַ�(����)ת��Ϊ��Ӧ��������
    	 // ��: '0' --> 0, "\060" --> 0, "\x30" --> 0��
    	 // �����÷��ص�����ʱ��
    	 // p ��ָ���� ��ʾ��һ�����ֵ��ַ��������еĿ�ʼ������
         char_value = parseOneChar( p );

         // ������һ����Ч�ַ��򷵻ص�ǰ�Ѿ��õ�������.
         // �������ͱ�׼ atoi() ������һ��.
         if(char_value < 0 || char_value >= base)
         {
        	 break;
         }
         value = value * base + char_value; // ��ָ������ת��Ϊ����

         // �������� int �ı�ʾ��Χ����ת����
         if(value < lastValue)
         {
        	 //cerr << "[overflow detected]:" << lastValue << "->" << value << endl;
        	 inRange = false;
        	 break;
         }
         lastValue = value;
     }

// Last step: ���������ԡ��Ƿ񳬳� int �ķ�Χ.

     if( inRange ) // δ���� int �Ŀɱ�ʾ��Χ�Ĵ���
     {
        if( isNegative )
            return -value;
        else
            return value;
     }
     else  // ���� int �Ŀɱ�ʾ��Χʱ��
     {
        // C/C++ ��׼��� atoi ����Ϊ�ǡ�δ����ġ�.
        // ���������ʹ�õ� ��������������
        return value; // ֱ�ӷ��ظ�ֵ.
     }
}


// �������� (Helper Function)

// ����8/10/16���Ƶģ������ַ� c ת��Ϊ���ʾ������(����Ӧ������
int  numberChar2Int(const char c)
{
    switch(c)
    {
        case '0': //�� ASCII �ַ����ж�Ӧ���� 48����������
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
        default: return -1; // ��Ч�ַ�
    }
}

/* Helper Function: parseOneChar
 *
 * ����ʾ�������ַ�����ת���ַ����У�ת��Ϊ���ʾ������.
 *   �罫 '0' �� '\060' �� '\X30' ת��Ϊ 0.
 *   �����ַ��������Ķ�����ʽ����μ������ʼ��ע�͡�
 * ������
 *   pc [in,out] ָ���������ͣ�
 *         ����ǰ��ָ��һ���ַ�, ��ת���ַ�����֮���ַ���
 *         ��������ʱ��ָ����һ���ַ�, ����һ��ת���ַ�����֮���ַ���
 * ����ֵ��
 * (1)���ַ���ת���ַ�������Ч������ʾһ����Ч�������ַ������򷵻� pc ָ���
 *    �ַ���ת�����ж�Ӧ������, ���޸� pc������ָ����һ���ַ���ת���ַ����еĿ�ʼ��
 * (2) ���򷵻�-1������ pc ָ���һ����Ч�ַ�.
 */
int parseOneChar(const char* & pc)
{
    int value = 0; // �ַ�����ת���ַ����У���Ӧ������
    int cBase = 0; // �ַ�����ת���ַ����У�֮��ʽ���֣�
                   // 0��ʾ������ʽ, 8��ʾ8���Ʊ�ʾ���ַ���16��ʾ16���Ʊ�ʾ���ַ�

// Step 1: ���ж��ǳ����ַ������� 8 ��16 ���Ʊ�ʾ��ת���ַ�����
    switch( *pc )
    {
    case '\0':  // �ַ���ĩβ����Ӧ�ô����������������������ߵ��߼�������.
        return 0;

    case '\\':  // �� 8 �� 16 ���Ʊ�ʾ��ת���ַ����еĿ�ʼ��־
        ++ pc;  // jump the char '\'
        cBase = 8; // ������ 8 ���� �� 16 ����
        if( *pc == 'x' || *pc == 'X' )  // 16 ����
        {
            ++ pc;  // jump the char 'x' or 'X'
            cBase = 16;
        }
        break;

    default:  // ������ʽ���ַ�
        value = numberChar2Int( *pc );
        if( value >= 0 ) ++pc;  // �� pc ָ����һ���ַ�
        return value;
    }


// Step 2.1: ���� 8 ���Ʊ�ʾ��ת���ַ���������ʾ����ֵ
    if(cBase == 8)
    {
        value = 0;
        // �ȵõ� 8 ���Ʊ�ʾ���ַ�֮���ڱ��룬��
        // "\060" ��ʾ���ַ����� '0'��������Ϊ48��
        int n = 0; // �Ѿ������� 8 �������ĸ��������3��
        for(int vc=0 ; *pc != '\0' && n < 3; ++n, ++pc)
        {
            vc = numberChar2Int( *pc );
            if(0 <= vc && vc <= 7)  // 8������
                value = value * 8 + vc;
            else
            	break;  // 8���Ʊ�ʾ ������
        }
        //��ѭ������ʱ��
        // 1. ��ת��������ȷ���� pc Ӧָ����һ�����������ַ�֮��ʼ��
        //    ��ת�����в���ȷ���� pc Ӧָ���1��������ַ���
        // 2. ǡ�ý� 8 ������ʽ��ת���ַ�����ת������,
        //    �� value �д洢�ľ��� ת�����ַ��Ķ�Ӧ���ַ�����.
        // 
        //�� ���������ַ������� ��\060�� ��������뽫����ת��Ϊ�ַ�'0'��
        //   �������� 48������ASCII���ַ�'0' �ı��롣

        // ���ţ����轫���ַ�ת��Ϊ��Ӧ���������罫�ַ�'0'ת��Ϊ����0
        value = numberChar2Int( value );

        return value;    // Done
    }

// Step 2.2: ���� 16 ���Ʊ�ʾ��ת���ַ���������ʾ����ֵ

    value = 0;
    // �ȵõ� 16 ���Ʊ�ʾ���ַ�֮���ڱ��룬��
    // \x30 ��ʾ���ַ����� '0'��������Ϊ48��
    int n=0; //�Ѿ������� 16 �����ַ����������2��
    for( int vc=0; *pc != '\0' && n<2; ++n, ++pc)
    {
        vc = numberChar2Int( *pc );
        if(0 <= vc && vc <= 15)
            value = value * 16 + vc;
        else
            break; // 16 ���Ʊ�ʾ ������
    }
    //��ѭ������ʱ��
    // 1. ��ת��������ȷ���� pc Ӧָ����һ�����������ַ�֮��ʼ��
    //    ��ת�����в���ȷ���� pc Ӧָ���1��������ַ���
    // 2. ǡ�ý� 16 ������ʽ��ת���ַ�����ת������,
    //    �� value �д洢�ľ��� ת�����ַ��Ķ�Ӧ���ַ�����.

    // ���Ż��轫���ַ�ת��Ϊ����.
    //  �� ���������ַ������� \x30 ��������ת��Ϊ���� 48��
    // ����ASCII���ַ�'0' �ı��룬�ٽ���ת��Ϊ����0
    value = numberChar2Int( value );
    return value;
}

} // end of namespace OOPLAB



/**
 * ���� test_case ���ܣ�
 *   ִ��һ����������. �ú����� test_cases() ���á�
 * ������
 *   str_number �����ַ���������ת��Ϊ����
 *   expected   �����ı�ת�����
 * ����ֵ��
 *   0  ���Գɹ�
 *   1  ���Բ��ɹ���˵�� atoi() ��ʵ����ȱ��
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


// ���� test_cases: ִ�� Ԥ�ȱ��Ƶ����ɲ�������
// �ú����� main() ���á�
void  test_cases()
{
    cout << setw(15) << std::left  << "string"
         << "     OOPLAB::atoi  "
         << "    compiler"  // �������ķ�����
         << endl;
    cout << "========================================================="
         << endl;

    struct {
        const char* str;
        int expected;
    } testData[] = {
      // ��1�������������ȷ���ַ���
        {"123",            123},
        {"-123",          -123},
        {"0123",          0123},  // 1*8*8+2*8+3 =83
        {"-0123",        -0123},
        {"0x123",        0x123},  // (1*16 + 2)*16 + 3 =291
        {"-0X123",      -0X123},
	    {"01\\062\\063",  0123}, // \062 is '2', \063 is '3'
	    {"1\\062\\x33",    123}, // \062 is '2', \x33 is '3'
	    {"0xF\\x66A",    0xFFA}, // \x66 is character 'F'

      // ��2�����������������Ч�ַ����������Ĵ�
        {"  +123.99",   +123},  // ��ͷ�Ŀհ��ַ�Ӧ����
        {"-0789",         -7},  // 8���Ʊ�ʾ�����ʽ���ȡ"-07"
        {"-",              0},
        {"+",              0},
        {"0",              0},
        {"0x",             0},
        {"",               0},
        
      // ��3����������� �߽� ���
        {"0xFFFFFFFF",  -1},
        {"0x80000000",  INT_MIN}, // min value of int: -2147483648
        {"0x80000001",  INT_MIN+1}, // -2147483647
        {"0x7FFFFFFF",  INT_MAX},  // max value of int: 2147483647

        // ���������л�����������澯��
	    // warning: integer overflow in expression
        {"0x7FFFFFFf",  INT_MIN-1}, // ???
        {"0x80000000",  INT_MAX+1}, // ??? �������� 2147483647+1 ��Ϊ����

        {NULL, 0}  // terminator element
    };

 	int n_fail =0; //ʧ�ܵĲ���������Ŀ
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
