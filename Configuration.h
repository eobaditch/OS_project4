#ifndef CONFIGURATION_H
#define CONFIGURATION_H

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>

using namespace std; 

class Configuration{

    public: 
        Configuration(){
            PERIOD_FETCH = 180; 
            NUM_FETCH = 1; 
            NUM_PARSE = 1; 
            SEARCH_FILE = "search.txt";
            SITE_FILE = "sites.txt";            
        }
        Configuration(char * file){
            ifstream infile(file); 
            string line; 
            while(getline(infile, line)){
                string param, value; 
                stringstream s(line); 
                getline(s, param, '='); 
                getline(s, value, '='); 
                if (param.compare("PERIOD_FETCH") == 0){
                    PERIOD_FETCH = stoi(value); 
                } else if (param.compare("NUM_FETCH") == 0){
                    NUM_FETCH = stoi(value); 
                } else if (param.compare("NUM_PARSE") == 0){
                    NUM_PARSE = stoi(value); 
                } else if (param.compare("SEARCH_FILE") == 0){
                    SEARCH_FILE = value; 
                } else if (param.compare("SITE_FILE") == 0){
                    SITE_FILE = value; 
                }

            }
        }
        int period_num(void){
            return PERIOD_FETCH; 
        }
        int fetch_num(void){
            return NUM_FETCH; 
        }
        int parse_num(void){
            return NUM_PARSE; 
        }
        string search_file(void){
            return SEARCH_FILE; 
        }
        string site_file(void){
            return SITE_FILE; 
        }
        void print_params(void){
            cout<<"PERIOD_FETCH"<<PERIOD_FETCH<<endl; 
            cout<<"NUM_FETCH"<<NUM_FETCH<<endl;
            cout<<"NUM_PARSE"<<NUM_PARSE<<endl; 
            cout<<"SEARC_FILE"<<SEARCH_FILE<<endl;
            cout<<"SITE_FILE"<<SITE_FILE<<endl;

        }
    private:
        int PERIOD_FETCH;
        int NUM_FETCH;
        int NUM_PARSE;
        string SEARCH_FILE; 
        string SITE_FILE; 


}; 

#endif
