#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <sstream>
#include <stdio.h>
#include <curl/curl.h>
#include <stdlib.h>
#include <vector>

#include "Configuration.h"
#include "File.h"
#include "Parser.h"

using namespace std; 

int main(int argc, char * argv[]){
    Configuration config(argv[1]); 
   
    config.print_params(); 
    int PERIOD_FETCH =  config.period_num();
    int NUM_FETCH = config.fetch_num(); 
    int NUM_PARSE = config.parse_num();
    string SEARCH_FILE = config.search_file(); 
    string SITE_FILE =  config.site_file(); 

    File search(SEARCH_FILE); 
    vector<string> search_content = search.get_content(); 
    File sites(SITE_FILE); 
    vector<string> site_content = sites.get_content(); 
    search.print_content(); 
    sites.print_content();
    
    Parser web_content(site_content[0]);  
    string content = web_content.download(); 
    cout<<content<<endl; 

}



