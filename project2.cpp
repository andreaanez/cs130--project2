#include "hash24.h"
#include "project2.h"
#include <cmath>
#include <iostream>
#include <fstream>
using namespace std; 

// How do I get the hash function to hash to the right index ????
// WTF !!:method will also recursively follow all references in an object and write the objects that arereferenced as well
//will write the entire hash table to a binary file in one step.
// How do I start reading it??
// How should I print these to the file ??
//Hw number 5 and the ternary tree one !!!


//BIN Functions 
Bin::Bin(){
 count = 0;
 single_word = "";

}

int Bin::get_count(){
    return count;
}

void Bin::increase_count(){
    count ++;
}

void Bin::resize_table(int size ){
    second_table.resize(size);
}

int Bin::calc_hash(string str){
    long result = 0 ;
    for (int i = 0 ; i < str.length() ; i++) {
        result = ( random_c_s * result + str.at(i) ) % prime1 ;
    }
    return hash(result) ;  // call long to int hash() 
}

int Bin::hash(long x) {
    if (x >= prime2) throw std::runtime_error( "Input is greater than prime number!!" ); ;
    return (int) abs(( ( random_a_s * x + random_b_s ) % prime2 )%tsize_s) ;
}

Dictionary::Dictionary(){}

Dictionary::Dictionary(long a1, long b1, long c1, long tsize1, vector<Bin> bins1) {
    random_a = a1;
    random_b = b1;
    random_c = c1;
    tsize = tsize1;
    table = bins1;
}


Dictionary::Dictionary(string fname, int tsize){
    this -> tsize = tsize;
    table.resize(tsize);  
   
    //******why ?
    hash24 = Hash24();
    random_a = hash24.get_random_a();
    random_b = hash24.get_random_b();
    random_c = hash24.get_random_c();
    hash24.dump();
    

//PASS 1
//read file contents 
    string word;

    ifstream file(fname);
    // REMEMBER: when we call get line in the while loop it gets a line, 
    //don't call twice unless you want to skip a line
        while (getline(file, word)){
            int table_index = 0;
            table_index = (hash24.hash(word)%tsize);
            table[table_index].add_word(word);
            table[table_index].increase_count();
        }
   
cout <<"Number of words = " << tsize<<endl;
cout << "Table size = " << tsize <<endl;
// PASS 2
num_collisions.resize(20);
num_hash.resize(21);
for (int i =0; i < tsize; i++){

    if (table [i].get_count() == 0){
        int num = num_collisions[0];
        num ++;
        num_collisions[0] = num; 
    }

    if (table [i].get_count() == 1){
        int num = num_collisions[1];
        num ++;
        num_collisions[1] = num; 
        table [i].single_word = table[i].get_word();
    }

    if( table[i].get_count() > 1){
      
        //updating num collisions
        int num = num_collisions[table[i].get_count()];
        num++;
        num_collisions[table[i].get_count()] = num;
        int num_col = table[i].get_count();
        int size_table = (num_col * num_col);
        table[i].tsize_s = size_table;
        table[i].resize_table(size_table);

        // updating max collisions 
        if (table[i].get_count()>max_collisions ){
            max_collisions = table[i].get_count();
            max_collisions_index = i;
        }
       
        int flag = 0; 
        int hash_col_index = 0; 
        while ( flag == 0 ){
        
            hash_col_index++;
            table[i].hash24_s = Hash24();
            for(int j=0; j < table[i].get_table_size(); j++){
                string word = table[i].get_index(j);
                int new_index_s = (table[i].hash24_s.hash(word)%size_table);
                    if (table[i].second_table[new_index_s] == ""){
                        table[i].second_table[new_index_s] = word;
                    }
                    else {
                        table[i].second_table.clear();
                        table[i].resize_table(size_table);
                        break; 
                    }
                    if (j == (table[i].get_table_size()-1)){
                        flag = 1;
                        int temp = num_hash[hash_col_index];
                        temp ++;
                        num_hash[hash_col_index] = temp;
                        table[i].random_a_s = table[i].hash24_s.get_random_a(); 
                        table[i].random_b_s = table[i].hash24_s.get_random_b();
                        table[i].random_c_s =table[i].hash24_s.get_random_c();
                        break;
                    }

            }
        
        }

    }
}
    cout << "Max collisions = " << max_collisions <<endl;

    int col_num = num_collisions.size();
    for (int i =0; i < col_num ; i++){
        cout << "# of primary slots with " << i << " words = " << num_collisions[i] << endl;
    }
    
    cout << "** Words in the slot with most collisions ***"<<endl;

    int num_words_col = table[max_collisions_index].get_second_table_size();
    for (int i =0; i < num_words_col; i++){
        if (table[max_collisions_index].second_table[i] != ""){
            cout << table[max_collisions_index].second_table[i]<< endl;
        }

    }

    int hash_num = num_hash.size();
    for (int i =0; i < hash_num ; i++){
        if (i!= 0){
            cout << "# of secondary hash tables trying " << i << " hash functions = " << num_hash[i] << endl;
        }
    }
    calc_averageHash();

}


void Dictionary::writeToFile(string fName) {
    
    ofstream file_obj;
    file_obj.open(fName, ios::out | ios::trunc);

    // print out the size of the hash table 
    file_obj.write((char*)&tsize, sizeof(tsize));

     cout << " HASH a : " << random_a << " b : " << random_b <<" c : " << random_c << endl;
    long a_p = random_a;
    long b_p = random_b;
    long c_p = random_c;

    file_obj.write((char*)&a_p, sizeof(a_p));
    file_obj.write((char*)&b_p, sizeof(b_p));
    file_obj.write((char*)&c_p, sizeof(c_p));

    // print out the hash table numbers 
    
    
    // for loop iterating through entire primary hash table 
    for (int i = 0; i < tsize; i++){
        
        // print just count if it equals zero 
        if (table[i].get_count() == 0){
            int count_temp = table[i].get_count();
            file_obj.write((char*)&count_temp, sizeof(count_temp));
        }
        // if count equals one we will print the count and the one word and move on 
        if (table[i].get_count() == 1){
            // first, write count 
            int count_temp = table[i].get_count();
            file_obj.write((char*)&count_temp, sizeof(count_temp));
            //second, write the one word 
            //lets get that word 
            string word = table[i].single_word;
            // lets get its length 
            int str_len = word.length();
            // first we need to tell the read function the size and print a number 
            file_obj.write((char*)&str_len, sizeof(str_len));
            //now lets print the actual word 
            file_obj.write(word.c_str(), str_len);
            
        }
        // the case where we need a secondary hash function 
        if (table[i].get_count() > 1){
            // first, write the count 
            int count_temp = table[i].get_count();
            file_obj.write((char*)&count_temp, sizeof(count_temp));
            
            // write the size of the table 
            int tsize_s = table[i].tsize_s;
            file_obj.write((char*)&tsize_s, sizeof(tsize_s));
            // write out hash numbers 
            long a = table[i].random_a_s;
            long b = table[i].random_b_s;
            long c = table[i].random_c_s;
            file_obj.write((char*)&a, sizeof(a));
            file_obj.write((char*)&b, sizeof(b));
            file_obj.write((char*)&c, sizeof(c));
            //print all words in secondary hash table
            //it can hash them into the table with the above information
            for (int j = 0; j < tsize_s; j++){
            //get the word
            // we only want the words not the empty spots 
                if (table[i].second_table[j] != ""){
                    string word_s = table[i].second_table[j];
                    // lets get its length 
                    int str_len_s = word_s.length();
                    // first we need to tell the read function the size and print a number 
                    file_obj.write((char*)&str_len_s, sizeof(str_len_s));
                    //now lets print the actual word 
                    file_obj.write(word_s.c_str(), str_len_s); 

                }    
                             
            }
            
        }


    }


}

bool Dictionary::find(string word){
    cout << "got here" <<endl;
    cout <<"word"<<word <<"word"<< endl;
    int indexP = calc_hash(word);

    if (word == "resi"){
        cout<<"hash index " << indexP << endl;
    }

    if (table[indexP].get_count() == 0){
        cout << word<<" not found"<<endl;
    }
    if (table[indexP].get_count() == 1){
        if (word != table[indexP].single_word){
            cout << word<<" not found"<<endl;
        }
        else{
            cout << word<<" found"<<endl;
            return true;
        }
        

    }

    if (table[indexP].get_count() > 1){
       int indexS = table[indexP].calc_hash(word);

       if (word  == table[indexP].second_table[indexS]){
            cout << word<<" found"<<endl;
            return true;
        }
        else{
            cout << word<<" not found"<<endl;
            return true;
        }

    }
    return false;

}
void Dictionary::calc_averageHash(){
    int sum_hash = 0;
    int count_hash = 0;
    int k = num_hash.size();
    for (int i =1; i < k; i++){
     sum_hash += num_hash[i] * i;
     count_hash += num_hash[i];

    }
    cout << "Average # of hash functions tried = ";
    compute_division(sum_hash , count_hash, 6);
    cout << endl;
}



int Dictionary::calc_hash(string str){
    long result = 0 ;
    for (int i = 0 ; i < str.length() ; i++) {
        result = ( random_c * result + str.at(i) ) % prime1 ;
    }
    return hash(result) ;  // call long to int hash() 
}

int Dictionary::hash(long x) {
    if (x >= prime2) throw std::runtime_error( "Input is greater than prime number!!" ); ;
    return (int) (( ( random_a * x + random_b ) % prime2 )%tsize);
}


void Dictionary::compute_division(int a, int b, int n) {
    
    if (n <= 0) {
        cout << a / b << endl;
        return;
    }
 
    int dec = a / b;
    for (int i = 0; i <= n; i++) {
        cout <<dec;
        a = a - (b * dec);
        if (a == 0)
            break;
        a = a * 10;
        dec = a / b;
        if (i == 0)
            cout << ".";
    }
}




