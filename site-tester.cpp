#include <iostream>
#include <fstream>
#include <string>

using namespace std; 

void get_params(string); 

int main(int argc, char * argv[]){
    int PERIOD_FETCH = 180;
    int NUM_FETCH = 1;
    int NUM_PARSE = 1;
    string SEARCH_FILE = "search.txt"; 
    string SITE_FILE =  "sites.txt"; 


    string config_file = argv[1]; 
    get_params(config_file);  

}

void get_params(string filename){

    

}
