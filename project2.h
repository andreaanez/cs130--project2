//project2.h


#ifndef PROJECT2_H
#define PROJECT2_H
#include "hash24.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
using namespace std;

class Bin{

    private: 
        int count;
        vector<string> col_words;
    public:
        Bin();
        int get_count();
        void set_count(int c){count = c;}
        void increase_count();
        void resize_table(int size );
        //~Bin();
        void insert(string word);
        void add_word(string word){col_words.push_back(word);}
        string get_word(){return col_words.back();}
        int get_table_size(){ return col_words.size();}
        int get_second_table_size(){ return second_table.size();}
        string get_index(int index){return col_words[index];}
        int calc_hash(std::string str);
        int hash(long x);
        Hash24 hash24_s;
        vector<string> second_table;
        string single_word;
        int  tsize_s;
        long random_a_s; 
        long random_b_s; 
        long random_c_s;
        static long const prime1 = 16890581L;
        static long const prime2 = 17027399L ;
};



class Dictionary {
    
    private:
    long tsize; 
    int max_collisions;
    int max_collisions_index;
    // Hash fuction for the table 
    Hash24 hash24;

    // Hash Functions for collisions 
   
    vector<Bin> table;
    vector<int > num_collisions;
    vector<int > num_hash;

    long random_a; 
    long random_b; 
    long random_c;

    static long const prime1 = 16890581L;
    static long const prime2 = 17027399L ;

    public:

    //Constructor that prints stuff with cout 

    int calc_hash(std::string str);
    int hash(long x);
    void calc_averageHash();
    void compute_division(int a, int b, int n);
    Dictionary();
    Dictionary(std::string fname, int tsize);
    Dictionary(long a1, long b1, long c1, long tsize1, vector<Bin> bins1);
    void writeToFile(std::string fName);
    bool find(std::string word);

    static Dictionary readFromFile(std::string fName){
   
        ifstream file_obj_;
        file_obj_.open(fName, ios::in | ios::binary);
       
        //table size 
        int tsize_;
        file_obj_.read((char*)&tsize_, sizeof(tsize_));
        cout << "table size "<< tsize_ <<endl;



        // Hash table information a, b, c

        long a_;
        file_obj_.read((char*)&a_, sizeof(a_));
        long b_;
        file_obj_.read((char*)&b_, sizeof(b_));
        long c_;
        file_obj_.read((char*)&c_, sizeof(c_));

        cout << " HASH  : " << a_ << " b : " << b_ <<" c : " << c_ << endl;
       
        
        // for loop through entire primary hash table
        vector<Bin> bins_(tsize_); 
        for (int i = 0; i < tsize_; i++){
            
            // initalize a bib 
            

            //read the count 
            int count_;
            file_obj_.read((char*)&count_, sizeof(count_));
            

            // conditions:

            // if count is zero 
            if (count_ ==0 ){
                Bin bin1 ;
                bins_[i] = bin1;
            }

            // if count is 1 
            if (count_ ==1 ){
                Bin bin1 ;
                // lets read that word

                // get string length 
                int str_len;
                file_obj_.read((char*)&str_len, sizeof(str_len));
                // Read string
                char* strData = new char[str_len];
                file_obj_.read(strData, str_len);
                string name(strData);
                if (name == "resi"){
                    cout<< "resi found count ==1 at index " << i<<endl;
                }
                
                // put in bin
                bin1.increase_count();
                bin1.single_word = name;
                if (name == "resi"){
                cout << "resi in bin by name " << name <<endl;
                }
                bins_[i] = bin1;

                if (name == "resi"){
                    cout << "here is resi in the bin " << bins_[i].single_word <<endl;
                }
                

            }

            if (count_ > 1){
                // creat the bin initalize the count 
                Bin bin1 ;
                bin1.set_count(count_);
                
                // retrive table size 
                int tsize_s;
                file_obj_.read((char*)&tsize_s, sizeof(tsize_s));
                bin1.tsize_s = tsize_s;
                // retrieve the hash numbers 
                long a_s;
                file_obj_.read((char*)&a_s, sizeof(a_s));
                long b_s;
                file_obj_.read((char*)&b_s, sizeof(b_s));
                long c_s;
                file_obj_.read((char*)&c_s, sizeof(c_s));
                
                // assign them into the object 
                
                bin1.random_a_s = a_s;
                bin1.random_a_s = a_s;
                bin1.random_a_s = a_s;

                // now we are going to read the words into bin1's secondary hash table
                // lets initalize our secondary hash table
                bin1.resize_table(tsize_s);
                
                //iterate through all words we printed
                for(int j = 0; j < count_; j++){
                    int str_len;
                    file_obj_.read((char*)&str_len, sizeof(str_len));
                    // Read string
                    char* strData = new char[str_len];
                    file_obj_.read(strData, str_len);
                    string name(strData);
                    if (name == "resi"){
                    cout<< "resi found at count > 1 index " << i<<endl;
                }
                    
                    // hash to the right index
                    int index =0;
                    index = bin1.calc_hash(name);
                    bin1.second_table[index] = name;
                    
                }
                // enter the bin into the hash table
                bins_[i] = bin1;
                
                
            }
        }
        
        file_obj_.close();
        Dictionary* temp = new Dictionary(a_, b_, c_, tsize_, bins_);
        Dictionary d = *temp;
        cout <<"resi in the hash" << d.table[330800].single_word<<endl;
        return *temp;
        }
    };



#endif