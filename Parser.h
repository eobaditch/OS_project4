#ifndef PARSER_H
#define PARSER_H
#include <string>
#include <stdlib.h>
#include <cstring>
#include <curl/curl.h>


class Parser{

    public:
        Parser(string c){
            content = c; 
        }
       
        //Find all substrings within string
        int parse(string keyword){
            int count = 0; 
            size_t pos = content.find(keyword, 0); //first occurence
            while(pos != string::npos){
                count++; 
                pos = content.find(keyword, pos+1); 
            }
            return count; 
        }



    private: 
        string content; 

};

#endif
