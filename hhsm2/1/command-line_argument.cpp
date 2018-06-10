/* Write a program like ‘‘Hello, world!’’ that takes a name as a 
 * command-line argument and writes ‘‘Hello, name!’’. Modify this 
 * program to take any number of names as arguments and to say 
 * hello to each.
 */
#include <iostream>
using namespace std;

int parseCommandline(int argc, char* argv[]) {
  // judge effect ..

  int i = 0;
  while (argc--) {
    cout << "hello, " << argv[i] << endl;
    ++i;
  }
  return 0;
}

int main(int argc, char* argv[]) {
  --argc; ++argv;
  if (parseCommandline(argc, argv) < 0) return 1;
  return 0;
}








