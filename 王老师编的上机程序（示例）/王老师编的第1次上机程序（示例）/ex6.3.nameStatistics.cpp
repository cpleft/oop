/**********************************************************************
 *
 * 练习题 P140 6.6.3 的参考实现
 *
 * 题目要求：
 * Read a sequence of possibly whitespace-separated (name,value) pairs,
 * where the name is a single whitespace-separated word and the value
 * is an integer or a floating-point value. Compute and print the sum
 * and mean for each name and the sum and mean for all names.
 */

/**********************************************************************
 * 关于本实现的说明：
 *
 * 1) 数据累计与平均
 *   为能够计算每个名字之值的累计和和平均值，需要使用适当的数据
 *   结构。如这个文件中使用了 C++ 标准库(STL)中的 map 和一个自定
 *   义结构类型 T_ValueState，也使用了STL中相关的算法 for_each。
 *   这是最便利的实现手段, 也为大家提供了一个范例。
 *
 *   当然，你也可以自己定义一个类似的结构及其数组，如
 *      struct  T_onePair {
 *         string  name;
 *         double  current_sum; // sum for name to now
 *         unsigned occurrence; // times for name to now
 *      };
 *      T_onePair  datas[1000];
 *    再定义此类数据的更新、遍历操作（很繁琐的）。
 *
 * 2) 关于命令行参数
 *    本文实现允许你运行程序时，以命令行参数方式指定数据来源（某个已编辑好的
 *    文本文件，即输入文件) 或/和 输出文件.
 *    在使用这两个文件时，通过全局变量 inputFile 和 outputFile 访问。
 *
 * *** 这个实现供大家参考学习，为完成第二次、第三次上机作业做准备。
 *
 * 编 制 人：王献青 @XiDian University
 * 编制时间：2017.5
 ********************************************************************
 */

#include <iostream>  // for cout, cin, endl
#include <iomanip>   // 输出格式化 所需头文件

#include <fstream>   // for ifstream, ofstream: read from or write to files

#include <string>    // for string—— 比较便利的字符串类型
#include <map>       // for map<> —— 关联关系表
#include <algorithm> // for for_each to traverse the map

#include <cstring>  // wrapper for C header, <string.h>, for strcmp

using namespace std;  // using directive for all names of std, see Chapter 8.


/**********************************************************************
                程序排列说明
                ~~~~~~~~~~~~

  为便于大家理解，本文代码按以下四部分组织排列：

  Part I: 主程序
    先给出 main()，便于大家先了解主控程序做什么。
    这就要求在 main() 定义之前，先给出其中要使用的名字的声明，
    这些名字的定义出现在 main() 之后。

  Part II:  文件准备、清理操作
    若命令行参数指定了输入/输出文件，则使用之，否则使用标准输入/输出
    设备 cin/cout.

  Part III:  读取数据、累计、存储相关数据结构与操作
    每从输入文件读入一个 (name, value) 对，立即对该 name 对应的信息
    进行重新计算并存储（更新）。

  Part IV:  统计和显示操作

 */


//  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//  Part I: 主程序
//

// 提前声明，这里均为  非定义声明
int  parseCommandLine(int argc, char* argv[]);
void readInput();
void showResult();
void closeFile();


/** 程序运行方式：（假设生成的可执行文件名为 ex6.3.exe）
 *  1)  ex6.3.exe
 *    此时 main()中看到的 argv[0]就是"ex6.3.exe", argc=1
 *
 *  2)  ex6.3.exe  -I  "输入文件路径"
 *    此时 main()中看到的 argv[0]就是"ex6.3.exe",  argv[1]="-I"
 *    argv[2]="输入文件路径", argc=3
 *
 *  3)  ex6.3.exe  -O "输出文件路径"
 *    此时 main()中看到的 argv[0]就是"ex6.3.exe",  argv[1]="-O"
 *    argv[2]="输出文件路径", argc=3
 *
 *  4)  ex6.3.exe  -I "输入文件路径"  -O  "输出文件路径"
 *      argv[0]  argv[1] argv[2]   argv[3] argv[4], argc=5
 *    此时 main()中看到的 argv[0]就是"ex6.3.exe",
 *    argv[1]="-I" ,   argv[2]="输入文件路径",
 *    argv[3]="-O" ,   argv[4]="输出文件路径",
 *    argc=5
 *
 * 若结合"-I" 指定了输入文件，则从该文件读取预定格式的数据，否则从标准
 * 输入设备(cin)读取；
 * 若结合"-O" 指定了输出文件，则将结果存储到指定文件，否则仅显示在标准
 * 输出设备(cout).
 */
int main(int argc, char* argv[])
{
    // Step 1: 先分析命令行参数，准备好输入/输出文件
    --argc; ++argv;   // jump the argv[0], e.g. ex6.3.exe
    if( parseCommandLine(argc, argv) < 0)
    {
        return 1;
    }

    readInput();  // Step 2: 读取输入并累加、记录

    showResult(); // Step 3: 显示统计结果

    closeFile();  // Step 4: 清理工作现场

	return 0;
}


//  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// Part II:  文件准备、清理操作
//


// 下面是为支持从指定文件或 cin 读取数据而定义的全局变量
// a pointer to input stream(input file) object,
// it is cin or a prepared file specified by command-line argument
istream *inputFile = &cin;   // 缺省为从标准输入设备（键盘）读取数据
string  inFileName = "std::cin";  // name of input stream

// 下面是为支持将输出数据写入指定文件或 cout 而定义的全局变量
// a pointer to output stream(output file) object,
// it is cout or a file specified by command-line argument
ostream *outputFile = &cout; // 缺省将信息输出到标准输出设备（屏幕）
string  outFileName = "std::cout"; // name of output stream

/**
 * 函数功能：
 *   工作完毕时，关闭文件，释放资源。
 *
 * 若输入/输出文件不是缺省的 cin/cout，则删除表示文件的对象数据.
 * 注意，不是删除文件本身。
 */
void closeFile()
{
    if( inputFile  != &cin  ) delete inputFile;
    if( outputFile != &cout ) delete outputFile;
}


/**
 * 函数功能：
 *   分析命令行参数，并据此设置 输入/输出文件.
 * 参数：
 *    argc  数组 argv[] 中元素个数
 *    argv  命令行参数中各参数组成的指针数组
 * 返回值：
 *   若输入/输出文件均打开成功则返回0，否则返回负整数。
 */
int parseCommandLine(int argc, char* argv[])
{
    if( (argc % 2) != 0 )
    {
        cerr << "Invalidate arguments." << endl;
        return -1;
    }

// Step 1: 先根据命令行参数，确定输入文件、输出文件
    int i=0;
    string argInputFileName;
    string argOutputFileName;
    while(argc > 0)
    {
        if(strcmp(argv[i], "-I") == 0) // 指定了输入文件
        {
            argInputFileName = argv[i+1];
            i += 2; argc -= 2; // jump "-I" "输入文件"
        }
        else if(strcmp(argv[i], "-O") == 0)
        {
            argOutputFileName = argv[i+1];
            i += 2; argc -= 2; // jump "-O" "输出文件"
        }
        else
        {
            cerr << "Unknown argument: \"" << argv[i] << "\"" << endl;
            return -1;
        }
    }

// Step 2: 若命令行参数指定了输入文件，则使用之，否则使用 cin
    if( !(argInputFileName.empty()) )
    {
        inFileName = argInputFileName;
        //动态创建表示输入文件的对象
        inputFile  = new ifstream(inFileName.c_str());
    }
    else // 可省略的代码块
    {
        // 缺省的输入文件
        inputFile  = &cin;   inFileName  = "std::cin";
    }

// Step 3: 若命令行参数指定了输出文件，则使用之，否则使用 cout
    if( !(argOutputFileName.empty()) )
    {
        outFileName = argOutputFileName;
        //动态创建表示输出文件的对象
        outputFile  = new ofstream(outFileName.c_str());
    }
    else // 可省略的代码块
    {
        // 缺省的输出文件
        outputFile = &cout;  outFileName = "std::cout";
    }

// Step 4: 最后 检查文件是否被成功打开
    if( inputFile == NULL || !(*inputFile) ){
        cerr << "Can't open input file '" << inFileName << "'!" << endl;
        return -2;
    }
    if( outputFile == NULL || !(*outputFile) ){
        cerr << "Can't create output file '" << outFileName << "'!" << endl;
        return -2;
    }

// 提示一下
    cout << "\nInput  file is " << inFileName
         << "\nOutput file is " << outFileName
         << endl;

    return 0; // OK
}


//  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// Part III:  读取数据、累计、存储相关数据结构与操作
//


// 该结构类型用于存储一个名字对应的累计数据.
// 该类型与名字一起构成一个值对： <name, T_NameState>
struct T_NameState
{
    double  currentSum; // 目前为止，某个名字各次出现时对应数值之累计
    unsigned int occus; // 目前为止，某个名字的出现次数
    
    T_NameState() : currentSum(0), occus(0)
    { }
};

// 全局变量：所有名字信息的记录。
// 作用：保存所有名字及其累计信息，
// 这里使用C++标准库提供的、表示映射表（即关联数组）的类模板 map<> 来存储。
map<string, T_NameState>  table;

/*
 * 函数功能：
 *   更新 table 中指定名字 name 的累计数据.
 *   该函数属于‘辅助函数’，被读数据的函数 readInput() 调用
 * 参数：
 *   name   名字的引用
 *   value  某次输入中，name 对应的数值
 * 返回值：无
 */
void updatePair(string& name, double value)
{
    // 若 name 在 table 中不存在，table[name] 这个表达式会自动增加 name
    // 对应的数据，且其 T_NameState 被自动初始化为 0。
    T_NameState& v = table[name];

    // 注意：上面这个 v 一定要用【引用】，否则下面的变化只会发生在
    //  一个副本数据上，当然就是无用功.
    v.currentSum += value;
    ++ (v.occus);
}


/**
 * 函数功能：
 *   从输入文件中读取若干对 “名字  值”，并按名字进行累计.
 * 参数：无
 * 返回值：无
 * Warning:
 *  这个实现没有检查输入的正确性，即假设输入是有效的，且
 *  每个名字 之后 紧跟一个数值(整数、实数均可)。
 * Notice:
 *  从标准输入设备(cin) 读取数据时，输入
 *         ctrl+z（in Microsft Windows）
 *  或
 *         ctrl+d （in Linux/Unix）
 * 表示输入结束。
 */
void readInput()
{
    string  name;
    double  value;

    // read all input until to end-of-file(^Z or ^D)
    //   or read error
    while ( *inputFile ){
        (*inputFile) >> name;  // 读入 名字
        if( !(*inputFile) ) // break on end-of-file or read error
            break;

        *inputFile >> value;  // 读入数值
        if( !(*inputFile) ) // break on end-of-file or read error
            break;

        updatePair(name, value);  // 更新 name 的统计信息
    }
}


//  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// Part IV:  统计和显示操作
//


// 为跨函数传递数据而定义的全局变量
double    totalSum =0; // 所有名字所有出现的累计和

/*
 * 函数功能：
 *   计算单个名字的累计和、平均值.
 *   该函数属于‘辅助函数’，当所有输入均被处理结束时，
 *   该函数被 showResult() 调用.
 * 参数：
 *   nv  映射表 map 中的一个 <name, value> 对的引用，
 *       需注意：pair的第一个类型参数必须为 const。
 *   用 nv.first 获得 pair 中的第一个要素 name，而用
 *   nv.second 获得 pair 中的第二个要素 T_NameState.
 */
void printPair(pair<const string,T_NameState>& nv)
{
    T_NameState & state = nv.second; //使用‘引用’，避免复制

    // 输出内容，并进行格式化，如 setw, left, endl。
    (*outputFile)
        << setw(15) << left << nv.first // 左对齐，输出 name，占据15列
        << setw(15) << state.occus      // 输出 name 的出现次数，占据15列
        << setw(15) << state.currentSum
        << setw(15) << (state.currentSum / state.occus) // 平均值
        << endl;
    totalSum += state.currentSum; // 计算‘总计’
}

/**
 * 函数功能：计算并显示:
 *     (1) 每个名字的累计和及其平均值、
 *     (2) 所有名字的累计和及其平均值.
 */
void showResult()
{
    size_t  totalOcc =0; // 不同名字的个数
    totalOcc = table.size(); // 获得输入中出现的不同名字的数量

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

    //利用C++标准库中的算法遍历映射表中的每个元素
    // 参数：第1个元素地址  末尾标志      每个元素上的操作
    for_each(table.begin(), table.end(),  printPair);

    *outputFile <<
        "--------------+--------------+--------------+---------------"
        << endl;

    // 计算并显示全部名字的累计和、平均值
    *outputFile << "Total " << totalOcc
        << " names, total sum=" << totalSum
        << ", total average=" << totalSum / totalOcc
        << endl;
}

/*附：示例
假设输入以下文本：
a 1 b 10
b    99
xd13 150
xd13 120
xd13 10
xd13 90

c 200  c 33 c 1

则程序产生的输出为：

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
