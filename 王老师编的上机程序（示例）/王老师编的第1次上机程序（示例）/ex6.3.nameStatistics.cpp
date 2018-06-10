/**********************************************************************
 *
 * ��ϰ�� P140 6.6.3 �Ĳο�ʵ��
 *
 * ��ĿҪ��
 * Read a sequence of possibly whitespace-separated (name,value) pairs,
 * where the name is a single whitespace-separated word and the value
 * is an integer or a floating-point value. Compute and print the sum
 * and mean for each name and the sum and mean for all names.
 */

/**********************************************************************
 * ���ڱ�ʵ�ֵ�˵����
 *
 * 1) �����ۼ���ƽ��
 *   Ϊ�ܹ�����ÿ������ֵ֮���ۼƺͺ�ƽ��ֵ����Ҫʹ���ʵ�������
 *   �ṹ��������ļ���ʹ���� C++ ��׼��(STL)�е� map ��һ���Զ�
 *   ��ṹ���� T_ValueState��Ҳʹ����STL����ص��㷨 for_each��
 *   �����������ʵ���ֶ�, ҲΪ����ṩ��һ��������
 *
 *   ��Ȼ����Ҳ�����Լ�����һ�����ƵĽṹ�������飬��
 *      struct  T_onePair {
 *         string  name;
 *         double  current_sum; // sum for name to now
 *         unsigned occurrence; // times for name to now
 *      };
 *      T_onePair  datas[1000];
 *    �ٶ���������ݵĸ��¡������������ܷ����ģ���
 *
 * 2) ���������в���
 *    ����ʵ�����������г���ʱ���������в�����ʽָ��������Դ��ĳ���ѱ༭�õ�
 *    �ı��ļ����������ļ�) ��/�� ����ļ�.
 *    ��ʹ���������ļ�ʱ��ͨ��ȫ�ֱ��� inputFile �� outputFile ���ʡ�
 *
 * *** ���ʵ�ֹ���Ҳο�ѧϰ��Ϊ��ɵڶ��Ρ��������ϻ���ҵ��׼����
 *
 * �� �� �ˣ������� @XiDian University
 * ����ʱ�䣺2017.5
 ********************************************************************
 */

#include <iostream>  // for cout, cin, endl
#include <iomanip>   // �����ʽ�� ����ͷ�ļ�

#include <fstream>   // for ifstream, ofstream: read from or write to files

#include <string>    // for string���� �Ƚϱ������ַ�������
#include <map>       // for map<> ���� ������ϵ��
#include <algorithm> // for for_each to traverse the map

#include <cstring>  // wrapper for C header, <string.h>, for strcmp

using namespace std;  // using directive for all names of std, see Chapter 8.


/**********************************************************************
                ��������˵��
                ~~~~~~~~~~~~

  Ϊ���ڴ����⣬���Ĵ��밴�����Ĳ�����֯���У�

  Part I: ������
    �ȸ��� main()�����ڴ�����˽����س�����ʲô��
    ���Ҫ���� main() ����֮ǰ���ȸ�������Ҫʹ�õ����ֵ�������
    ��Щ���ֵĶ�������� main() ֮��

  Part II:  �ļ�׼�����������
    �������в���ָ��������/����ļ�����ʹ��֮������ʹ�ñ�׼����/���
    �豸 cin/cout.

  Part III:  ��ȡ���ݡ��ۼơ��洢������ݽṹ�����
    ÿ�������ļ�����һ�� (name, value) �ԣ������Ը� name ��Ӧ����Ϣ
    �������¼��㲢�洢�����£���

  Part IV:  ͳ�ƺ���ʾ����

 */


//  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//  Part I: ������
//

// ��ǰ�����������Ϊ  �Ƕ�������
int  parseCommandLine(int argc, char* argv[]);
void readInput();
void showResult();
void closeFile();


/** �������з�ʽ�����������ɵĿ�ִ���ļ���Ϊ ex6.3.exe��
 *  1)  ex6.3.exe
 *    ��ʱ main()�п����� argv[0]����"ex6.3.exe", argc=1
 *
 *  2)  ex6.3.exe  -I  "�����ļ�·��"
 *    ��ʱ main()�п����� argv[0]����"ex6.3.exe",  argv[1]="-I"
 *    argv[2]="�����ļ�·��", argc=3
 *
 *  3)  ex6.3.exe  -O "����ļ�·��"
 *    ��ʱ main()�п����� argv[0]����"ex6.3.exe",  argv[1]="-O"
 *    argv[2]="����ļ�·��", argc=3
 *
 *  4)  ex6.3.exe  -I "�����ļ�·��"  -O  "����ļ�·��"
 *      argv[0]  argv[1] argv[2]   argv[3] argv[4], argc=5
 *    ��ʱ main()�п����� argv[0]����"ex6.3.exe",
 *    argv[1]="-I" ,   argv[2]="�����ļ�·��",
 *    argv[3]="-O" ,   argv[4]="����ļ�·��",
 *    argc=5
 *
 * �����"-I" ָ���������ļ�����Ӹ��ļ���ȡԤ����ʽ�����ݣ�����ӱ�׼
 * �����豸(cin)��ȡ��
 * �����"-O" ָ��������ļ����򽫽���洢��ָ���ļ����������ʾ�ڱ�׼
 * ����豸(cout).
 */
int main(int argc, char* argv[])
{
    // Step 1: �ȷ��������в�����׼��������/����ļ�
    --argc; ++argv;   // jump the argv[0], e.g. ex6.3.exe
    if( parseCommandLine(argc, argv) < 0)
    {
        return 1;
    }

    readInput();  // Step 2: ��ȡ���벢�ۼӡ���¼

    showResult(); // Step 3: ��ʾͳ�ƽ��

    closeFile();  // Step 4: �������ֳ�

	return 0;
}


//  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// Part II:  �ļ�׼�����������
//


// ������Ϊ֧�ִ�ָ���ļ��� cin ��ȡ���ݶ������ȫ�ֱ���
// a pointer to input stream(input file) object,
// it is cin or a prepared file specified by command-line argument
istream *inputFile = &cin;   // ȱʡΪ�ӱ�׼�����豸�����̣���ȡ����
string  inFileName = "std::cin";  // name of input stream

// ������Ϊ֧�ֽ��������д��ָ���ļ��� cout �������ȫ�ֱ���
// a pointer to output stream(output file) object,
// it is cout or a file specified by command-line argument
ostream *outputFile = &cout; // ȱʡ����Ϣ�������׼����豸����Ļ��
string  outFileName = "std::cout"; // name of output stream

/**
 * �������ܣ�
 *   �������ʱ���ر��ļ����ͷ���Դ��
 *
 * ������/����ļ�����ȱʡ�� cin/cout����ɾ����ʾ�ļ��Ķ�������.
 * ע�⣬����ɾ���ļ�����
 */
void closeFile()
{
    if( inputFile  != &cin  ) delete inputFile;
    if( outputFile != &cout ) delete outputFile;
}


/**
 * �������ܣ�
 *   ���������в��������ݴ����� ����/����ļ�.
 * ������
 *    argc  ���� argv[] ��Ԫ�ظ���
 *    argv  �����в����и�������ɵ�ָ������
 * ����ֵ��
 *   ������/����ļ����򿪳ɹ��򷵻�0�����򷵻ظ�������
 */
int parseCommandLine(int argc, char* argv[])
{
    if( (argc % 2) != 0 )
    {
        cerr << "Invalidate arguments." << endl;
        return -1;
    }

// Step 1: �ȸ��������в�����ȷ�������ļ�������ļ�
    int i=0;
    string argInputFileName;
    string argOutputFileName;
    while(argc > 0)
    {
        if(strcmp(argv[i], "-I") == 0) // ָ���������ļ�
        {
            argInputFileName = argv[i+1];
            i += 2; argc -= 2; // jump "-I" "�����ļ�"
        }
        else if(strcmp(argv[i], "-O") == 0)
        {
            argOutputFileName = argv[i+1];
            i += 2; argc -= 2; // jump "-O" "����ļ�"
        }
        else
        {
            cerr << "Unknown argument: \"" << argv[i] << "\"" << endl;
            return -1;
        }
    }

// Step 2: �������в���ָ���������ļ�����ʹ��֮������ʹ�� cin
    if( !(argInputFileName.empty()) )
    {
        inFileName = argInputFileName;
        //��̬������ʾ�����ļ��Ķ���
        inputFile  = new ifstream(inFileName.c_str());
    }
    else // ��ʡ�ԵĴ����
    {
        // ȱʡ�������ļ�
        inputFile  = &cin;   inFileName  = "std::cin";
    }

// Step 3: �������в���ָ��������ļ�����ʹ��֮������ʹ�� cout
    if( !(argOutputFileName.empty()) )
    {
        outFileName = argOutputFileName;
        //��̬������ʾ����ļ��Ķ���
        outputFile  = new ofstream(outFileName.c_str());
    }
    else // ��ʡ�ԵĴ����
    {
        // ȱʡ������ļ�
        outputFile = &cout;  outFileName = "std::cout";
    }

// Step 4: ��� ����ļ��Ƿ񱻳ɹ���
    if( inputFile == NULL || !(*inputFile) ){
        cerr << "Can't open input file '" << inFileName << "'!" << endl;
        return -2;
    }
    if( outputFile == NULL || !(*outputFile) ){
        cerr << "Can't create output file '" << outFileName << "'!" << endl;
        return -2;
    }

// ��ʾһ��
    cout << "\nInput  file is " << inFileName
         << "\nOutput file is " << outFileName
         << endl;

    return 0; // OK
}


//  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// Part III:  ��ȡ���ݡ��ۼơ��洢������ݽṹ�����
//


// �ýṹ�������ڴ洢һ�����ֶ�Ӧ���ۼ�����.
// ������������һ�𹹳�һ��ֵ�ԣ� <name, T_NameState>
struct T_NameState
{
    double  currentSum; // ĿǰΪֹ��ĳ�����ָ��γ���ʱ��Ӧ��ֵ֮�ۼ�
    unsigned int occus; // ĿǰΪֹ��ĳ�����ֵĳ��ִ���
    
    T_NameState() : currentSum(0), occus(0)
    { }
};

// ȫ�ֱ���������������Ϣ�ļ�¼��
// ���ã������������ּ����ۼ���Ϣ��
// ����ʹ��C++��׼���ṩ�ġ���ʾӳ������������飩����ģ�� map<> ���洢��
map<string, T_NameState>  table;

/*
 * �������ܣ�
 *   ���� table ��ָ������ name ���ۼ�����.
 *   �ú������ڡ��������������������ݵĺ��� readInput() ����
 * ������
 *   name   ���ֵ�����
 *   value  ĳ�������У�name ��Ӧ����ֵ
 * ����ֵ����
 */
void updatePair(string& name, double value)
{
    // �� name �� table �в����ڣ�table[name] ������ʽ���Զ����� name
    // ��Ӧ�����ݣ����� T_NameState ���Զ���ʼ��Ϊ 0��
    T_NameState& v = table[name];

    // ע�⣺������� v һ��Ҫ�á����á�����������ı仯ֻ�ᷢ����
    //  һ�����������ϣ���Ȼ�������ù�.
    v.currentSum += value;
    ++ (v.occus);
}


/**
 * �������ܣ�
 *   �������ļ��ж�ȡ���ɶ� ������  ֵ�����������ֽ����ۼ�.
 * ��������
 * ����ֵ����
 * Warning:
 *  ���ʵ��û�м���������ȷ�ԣ���������������Ч�ģ���
 *  ÿ������ ֮�� ����һ����ֵ(������ʵ������)��
 * Notice:
 *  �ӱ�׼�����豸(cin) ��ȡ����ʱ������
 *         ctrl+z��in Microsft Windows��
 *  ��
 *         ctrl+d ��in Linux/Unix��
 * ��ʾ���������
 */
void readInput()
{
    string  name;
    double  value;

    // read all input until to end-of-file(^Z or ^D)
    //   or read error
    while ( *inputFile ){
        (*inputFile) >> name;  // ���� ����
        if( !(*inputFile) ) // break on end-of-file or read error
            break;

        *inputFile >> value;  // ������ֵ
        if( !(*inputFile) ) // break on end-of-file or read error
            break;

        updatePair(name, value);  // ���� name ��ͳ����Ϣ
    }
}


//  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// Part IV:  ͳ�ƺ���ʾ����
//


// Ϊ�纯���������ݶ������ȫ�ֱ���
double    totalSum =0; // �����������г��ֵ��ۼƺ�

/*
 * �������ܣ�
 *   ���㵥�����ֵ��ۼƺ͡�ƽ��ֵ.
 *   �ú������ڡ���������������������������������ʱ��
 *   �ú����� showResult() ����.
 * ������
 *   nv  ӳ��� map �е�һ�� <name, value> �Ե����ã�
 *       ��ע�⣺pair�ĵ�һ�����Ͳ�������Ϊ const��
 *   �� nv.first ��� pair �еĵ�һ��Ҫ�� name������
 *   nv.second ��� pair �еĵڶ���Ҫ�� T_NameState.
 */
void printPair(pair<const string,T_NameState>& nv)
{
    T_NameState & state = nv.second; //ʹ�á����á������⸴��

    // ������ݣ������и�ʽ������ setw, left, endl��
    (*outputFile)
        << setw(15) << left << nv.first // ����룬��� name��ռ��15��
        << setw(15) << state.occus      // ��� name �ĳ��ִ�����ռ��15��
        << setw(15) << state.currentSum
        << setw(15) << (state.currentSum / state.occus) // ƽ��ֵ
        << endl;
    totalSum += state.currentSum; // ���㡮�ܼơ�
}

/**
 * �������ܣ����㲢��ʾ:
 *     (1) ÿ�����ֵ��ۼƺͼ���ƽ��ֵ��
 *     (2) �������ֵ��ۼƺͼ���ƽ��ֵ.
 */
void showResult()
{
    size_t  totalOcc =0; // ��ͬ���ֵĸ���
    totalOcc = table.size(); // ��������г��ֵĲ�ͬ���ֵ�����

    totalSum = 0;

    *outputFile << "\n";

    if( totalOcc < 1 ){
        *outputFile << "No elements." << endl;
        return;
    }

    *outputFile << setw(15) << left  << "  name  "
        << setw(15)  << left << "  times  "
        << setw(15)  << left << "  sum    "
        << setw(15)  << left << "  average "
        << endl;
    *outputFile <<
        "--------------+--------------+--------------+---------------"
        << endl;

    //����C++��׼���е��㷨����ӳ����е�ÿ��Ԫ��
    // ��������1��Ԫ�ص�ַ  ĩβ��־      ÿ��Ԫ���ϵĲ���
    for_each(table.begin(), table.end(),  printPair);

    *outputFile <<
        "--------------+--------------+--------------+---------------"
        << endl;

    // ���㲢��ʾȫ�����ֵ��ۼƺ͡�ƽ��ֵ
    *outputFile << "Total " << totalOcc
        << " names, total sum=" << totalSum
        << ", total average=" << totalSum / totalOcc
        << endl;
}

/*����ʾ��
�������������ı���
a 1 b 10
b    99
xd13 150
xd13 120
xd13 10
xd13 90

c 200  c 33 c 1

�������������Ϊ��

Input  file is cin
Output file is cout

  name           times          sum            average
--------------+--------------+--------------+---------------
a              1              1              1
b              2              109            54.5
c              3              234            78
xd13           4              370            92.5
--------------+--------------+--------------+---------------
Total 4 names, total sum=714, total average=178.5

*/
//
//    E n d   o f   F i l e
/  ~~~~~~~~~~~~~~~~~~~~~~~~
