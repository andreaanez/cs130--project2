#include "project2.h"
#include <iostream>
#include<fstream>
#include <vector>
#include <sstream>
#include <cstdlib>
using namespace std;

// argv[1] contains the text file 
// first read from the file 
int main (int argc, char *argv[]) {
    // First compute the number of words in the file 
    char a[80];
    long word_count = 0;
    ifstream file(argv[1]);
        if(!file){
            cout << "While opening a file an error is encountered" << endl;
        }

        while (!file.eof()){
            file >> a;
            word_count ++;
            
        }
    word_count --;
    
    Dictionary t (argv[1], word_count);
    t.writeToFile(argv[2]);
    
    // construcotrs for string object 
    // constructor that takes a charector pointer 
// t size num words in the file name call that first 
    // compute the number of files in main 
    //open file 

    
}