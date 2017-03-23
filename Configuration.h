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
        
        Configuration(char * file){
            ifstream infile(file); 
            string line; 
            PERIOD_FETCH = 180; 
            NUM_FETCH = 1; 
            NUM_PARSE = 1; 
            SEARCH_FILE = "search.txt";
            SITE_FILE = "sites.txt";
            while(getline(infile, line)){
                string param, value; 
                stringstream s(line); 
                getline(s, param, '='); 
                getline(s, value, '='); 
                //Set parameters
                if (param.compare("PERIOD_FETCH") == 0){
                    if(!value.compare("") == 0){
                        try{
                            //if out of range set to default
                            PERIOD_FETCH = stoi(value);
                        } catch (std::out_of_range& e){
                            cout<<"Error, PERIOD_FETCH not in sensible range.  Set back to default."<<endl;
                            PERIOD_FETCH = 1; 
                        }
                    
                    }
                    if (PERIOD_FETCH > 10000000 || PERIOD_FETCH <=0 ){
                        cout<<"Error, PERIOD_FETCH not in sensible range.  Set back to default."<<endl; 
                        PERIOD_FETCH=180; 
                    }
                } else if (param.compare("NUM_FETCH") == 0){
                    if(!value.compare("") == 0){
                        try{
                            //if out of range set to default
                            NUM_FETCH = stoi(value);
                        } catch (std::out_of_range& e){
                            cout<<"Error, NUM_FETCH not in sensible range.  Set back to default."<<endl;
                            NUM_FETCH = 1; 
                        }

                    }
                    if (NUM_FETCH >8 || NUM_FETCH <=0){
                        cout<<"Error, NUM_FETCH not in sensible range.  Set back to default."<<endl;
                        NUM_FETCH = 1;
                    }
                } else if (param.compare("NUM_PARSE") == 0){
                    if(!value.compare("") == 0){
                        try{
                            //if out of range set to default
                            NUM_PARSE = stoi(value);
                        } catch (std::out_of_range& e){
                            cout<<"Error, NUM_PARSE not in sensible range.  Set back to default."<<endl;
                            NUM_PARSE = 1; 
                        }
                    }
                    if (NUM_PARSE >8 || NUM_PARSE <=0){
                        cout<<"Error, NUM_PARSE not in sensible range.  Set back to default."<<endl;
                        NUM_PARSE = 1;
                    }
                } else if (param.compare("SEARCH_FILE") == 0){
                    if(!value.compare("") == 0){
                        SEARCH_FILE = value;
                    }
                } else if (param.compare("SITE_FILE") == 0){
                    if(!value.compare("") == 0){
                        SITE_FILE = value; 
                    }
                } else{
                    cout<<"Warning: unknown parameter"<<endl; 
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
