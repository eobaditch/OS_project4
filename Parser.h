#ifndef PARSER_H
#define PARSER_H
#include <string>
#include <curl.curl.h>


class Parser{

    struct MemoryStruct{
        char * memory; 
        size_t size; 
    }; 

    public:
        Parser(string u_in){
            url = u_in; 
        }
        
        static size_t  WriteMemoryCallback(void *contents, size_t size, size_t nmemb, void*userp){
            size_t realsize = size * nmemb;
            struct MemoryStruct *mem = (struct MemoryStruct *)userp;
     
            mem->memory = realloc(mem->memory, mem->size + realsize + 1);
            if(mem->memory == NULL) {
                /* out of memory! */ 
                printf("not enough memory (realloc returned NULL)\n");
                return 0;
            }
            memcpy(&(mem->memory[mem->size]), contents, realsize); 
            mem->size += realsize; 
            mem->memory[mem->size]=0; 
            return realsize; 
        }
        void download(){
            

        }
    private:
        string url; 

}; 
