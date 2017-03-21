#include <iostream>
#include <fstream>
#include <string>
#include <curl/curl.h>
#include <stdlib.h>

using namespace std; 
struct MemoryStruct {
  char *memory;
  size_t size;
};

static size_t
WriteMemoryCallback(void *contents, size_t size, size_t nmemb, void 
*userp)
{
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
  mem->memory[mem->size] = 0;

  return realsize;
}

int main(int argc, char * argv[]){
    int PERIOD_FETCH = 180;
    int NUM_FETCH = 1;
    int NUM_PARSE = 1;
    string SEARCH_FILE = "search.txt"; 
    string SITE_FILE =  "sites.txt"; 

    /*
    string config_file = argv[1]; 
    ifstream file config_file; 

    if (!file.good()){
        fprintf(stderr, "Error opening configuration file\n"); 
        exit(1); 
    }
    //Read in contents
    while(getline(file)){
    }
    */
    CURL *curl_handle;
    CURLcode res;

    struct MemoryStruct chunk;

    chunk.memory = malloc(1);  /* will be grown as needed by the realloc 
above */
    chunk.size = 0;    /* no data at this point */

    curl_global_init(CURL_GLOBAL_ALL);

  /* init the curl session */
    curl_handle = curl_easy_init();

  /* specify URL to get */
    curl_easy_setopt(curl_handle, CURLOPT_URL, "http://www.example.com/");

  /* send all data to this function  */
    curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, 
WriteMemoryCallback);

  /* we pass our 'chunk' struct to the callback function */
    curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, (void *)&chunk);

  /* some servers don't like requests that are made without a user-agent
     field, so we provide one */
    curl_easy_setopt(curl_handle, CURLOPT_USERAGENT, "libcurl-agent/1.0");

  /* get it! */
    res = curl_easy_perform(curl_handle);

  /* check for errors */
    if(res != CURLE_OK) {
        fprintf(stderr, "curl_easy_perform() failed: %s\n",
            curl_easy_strerror(res));
    }
    else {
    /*
     * Now, our chunk.memory points to a memory block that is chunk.size
     * bytes big and contains the remote file.
     *
     * Do something nice with it!
     */

        printf("%lu bytes retrieved\n", (long)chunk.size);
    }

  /* cleanup curl stuff */
    curl_easy_cleanup(curl_handle);

    free(chunk.memory);

  /* we're done with libcurl, so clean it up */
    curl_global_cleanup();

}



