
#include "project2.h"
#include <iostream>
#include <vector>
#include <sstream>
using namespace std;



int main(int argc, char** argv) {
    
    Dictionary d = Dictionary::readFromFile(argv[1]);

    string funcs = argv[2];
    vector <string> v;
    int size = funcs.size();
    string words; 
    for (int i=0; i < size; i++){
        
        if (funcs[i] == 32){
           v.push_back(words);
           words="";
           i++;
        }
        if (funcs[i]== 44){
            v.push_back(words);
            words="";
            i++;
            i++;
        }
        words += funcs[i];
        if ( i == size -1){
            v.push_back(words);
            words="";
        }
              
    }
    

    int j = v.size();
    for (int i =0; i < j; i++){
        d.find(v[i]);
           
    }
}






