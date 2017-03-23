#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <signal.h>
#include <unistd.h>
#include <sstream>
#include <stdio.h>
#include <time.h>
#include <curl/curl.h>
#include <stdlib.h>
#include <vector>
#include "FetchQueue.h"
#include "ParseQueue.h"
#include "Node.h"
#include "Configuration.h"
#include "File.h"
#include "Curl.h"
#include "Parser.h"


using namespace std; 

FetchQueue fetch; 
ParseQueue parse; 
vector<string> site_content; 
vector<string> search_content; 
ofstream fs; 
int PERIOD_FETCH, NUM_FETCH, NUM_PARSE; 
int TIMER =1; 
int run=1; 
int ROUND=1; 
int curr_fetch_threads = 0; 
int curr_parse_threads = 0; 
pthread_t * p_fetch= (pthread_t *)malloc(sizeof(pthread_t)*NUM_FETCH); 
pthread_t * p_parse = (pthread_t *)malloc(sizeof(pthread_t)*NUM_PARSE); 
int i=0; 
int j=0; 
void *fetch_handler(void *); 
void *parse_handler(void *); 


void sig_handler(int s){
    fs.close(); 
    string filename; 
    filename = to_string(ROUND) + ".csv";
    cout<<"FILE: "<<filename<<endl; 
    fs.open(filename, ofstream::out | ofstream::app); 
    
    
    while(curr_fetch_threads < NUM_FETCH){
        pthread_create(&p_fetch[i], NULL, fetch_handler, NULL);
        curr_fetch_threads++; 
        i++; 
    }
    while(curr_parse_threads < NUM_PARSE){
        pthread_create(&p_parse[i], NULL, parse_handler, NULL);
        curr_parse_threads++; 
        j++; 
    } 

    cout<<"Begin alarm"<<endl; 
        for (unsigned int i=0; i<site_content.size(); i++){
        Node curr = {site_content[i], ""}; 
        fetch.push(curr); 
    }
    TIMER = 1; 
    ROUND++;
    alarm(PERIOD_FETCH); 
    signal(SIGALRM, sig_handler); 
}



int main(int argc, char * argv[]){
    
    Configuration config(argv[1]); 
   
    config.print_params(); 
    PERIOD_FETCH =  config.period_num();
    NUM_FETCH = config.fetch_num(); 
    NUM_PARSE = config.parse_num();
    string SEARCH_FILE = config.search_file(); 
    string SITE_FILE =  config.site_file(); 

    signal(SIGALRM, sig_handler); 
    alarm(1); 
    
    File search(SEARCH_FILE); 
    search_content = search.get_content(); 
    File sites(SITE_FILE); 
    site_content = sites.get_content(); 
    search.print_content(); 
    sites.print_content();
    
    //Parser web_content(site_content[0]);  
    //string content = web_content.download(); 
    for (int i=0; i<NUM_FETCH; i++){
        //create threads
        //pthread_create(&p_fetch[i], NULL, fetch_handler, NULL); 
    }
    for(int i=0; i<NUM_PARSE; i++){
        pthread_create(&p_parse[i], NULL, parse_handler, NULL); 
    }

    while(run){
    }
}


void * fetch_handler(void *unused){
    cout<<"in fetch handler"<<endl; 
    while (run){
        Node current = fetch.pop(); 
        Curl curr_curl(current.sitename); 
        current.content = curr_curl.download(); 
        parse.push(current); 
    }
    pthread_exit(NULL); 
    curr_fetch_threads--; 
    return 0; 
}

void * parse_handler(void * unused){
    cout<<"in parse handler"<<endl; 
    int freq; 
    time_t now = time(0); 
    struct tm tstruct; 
    char buf[80]; 
    //char * output = new char[2048]; 
    string output; 
    tstruct = *localtime(&now);
    strftime(buf, sizeof(buf), "%Y-%m-%d.%X", &tstruct); 
    //ile(run){
        Node current = parse.pop(); 
        Parser curr_parse(current.content); 
        for(unsigned int i=0; i<search_content.size(); i++){
            freq = curr_parse.parse(search_content[i]); 
            //sprintf(output, "%s,%s,%s,%d\n", buf, search_content[i], current.sitename, freq); 
            string buf_s(buf); 
            output = buf_s + "," + search_content[i] + "," + current.sitename + "," + to_string(freq);
            fs<<output<<endl; 
            cout<<output; 
            //fs.write(output.c_str(), 2048); 
        }
   // 
    pthread_exit(NULL); 
    curr_parse_threads--; 
    return 0; 
    
}
