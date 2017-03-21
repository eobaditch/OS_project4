#ifndef PARSER_H
#define PARSER_H
#include <string>
#include <stdlib.h>
#include <cstring>
#include <curl/curl.h>


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
     
            mem->memory = (char *)realloc(mem->memory, mem->size + realsize + 1);
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
        string download(){
            CURL *curl_handle;
            CURLcode res;
             
            struct MemoryStruct chunk;
               
            chunk.memory = new char;  /* will be grown as needed by the realloc above */ 
            chunk.size = 0;    /* no data at this point */ 
                   
            curl_global_init(CURL_GLOBAL_ALL);
                     
                      /* init the curl session */ 
            curl_handle = curl_easy_init();
                       
                        /* specify URL to get */ 
            curl_easy_setopt(curl_handle, CURLOPT_URL, url.c_str());
                     
                      /* send all data to this function  */ 
            curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
                       
                        /* we pass our 'chunk' struct to the callback function */ 
            curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, (void *)&chunk);
                         
                          /* some servers don't like requests that are made without a user-agent
                           *      field, so we provide one */ 
            curl_easy_setopt(curl_handle, CURLOPT_USERAGENT, "libcurl-agent/1.0");
                           
                            /* get it! */ 
            res = curl_easy_perform(curl_handle);
                         
                          /* check for errors */ 
            if(res != CURLE_OK) {
                fprintf(stderr, "curl_easy_perform() failed: %s\n",
                curl_easy_strerror(res));
            }
            else {
                char * char_content= chunk.memory;
                content = char_content;
                //cout<<content<<endl; 
                printf("%lu bytes retrieved\n", (long)chunk.size);
            }
                             
                              /* cleanup curl stuff */ 
            curl_easy_cleanup(curl_handle);
                               
            delete [] chunk.memory;
                                 
                                  /* we're done with libcurl, so clean it up */ 
            curl_global_cleanup();
            return content; 

        }
    private:
        string url; 
        string content; 

};

#endif
