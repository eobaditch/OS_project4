//Emily Obaditch and Lauren Ferrara

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
int TIMER =1; //to set alarm to refetch
int run=1; //to keep program going until signals
int ROUND=1; //to write to different files per fetch batch
pthread_t * p_fetch= (pthread_t *)malloc(sizeof(pthread_t)*NUM_FETCH); 
pthread_t * p_parse = (pthread_t *)malloc(sizeof(pthread_t)*NUM_PARSE); 
string filename; 
void *fetch_handler(void *); 
void *parse_handler(void *); 
void createFetchThreads(void); 
void createParseThreads(void ); 

void sig_handler(int s){
    fs.close(); 
    filename = to_string(ROUND) + ".csv";
    fs.open(filename, ofstream::out | ofstream::app); 
    //push all sites onto FetchQueue
    for (unsigned int i=0; i<site_content.size(); i++){
        Node curr = {site_content[i], ""}; 
        fetch.push(curr); 
    }
    TIMER = 1; 
    ROUND++;
    alarm(PERIOD_FETCH); 
    signal(SIGALRM, sig_handler); 
}

void term_handler(int s){
    
    for(int i=0; i<NUM_FETCH; i++){
        int jc = pthread_join(p_fetch[i], NULL); 
        if(jc !=0){
            cout<<"Thread join error"<<endl; 
            exit(1); 
        }
    }
    for(int i=0; i<NUM_PARSE; i++){
        int jc = pthread_join(p_parse[i], NULL); 
        if(jc !=0){
            cout<<"Thread join error"<<endl; 
            exit(1); 
        }
    }
    cout<<"Signal recieved to exit"<<endl; 
    run = 0;  
}

void int_handler(int s){
    run = 0; 
    
    cout<<"Signal recieved to exit"<<endl; 
}



int main(int argc, char * argv[]){
    
    Configuration config(argv[1]); 
   
    PERIOD_FETCH =  config.period_num();
    NUM_FETCH = config.fetch_num(); 
    NUM_PARSE = config.parse_num();
    string SEARCH_FILE = config.search_file(); 
    string SITE_FILE =  config.site_file(); 

    signal(SIGALRM, sig_handler); 
    signal(SIGINT, int_handler); 
    signal(SIGTERM, term_handler); 
    alarm(1); 
    
    File search(SEARCH_FILE); 
    search_content = search.get_content(); 
    File sites(SITE_FILE); 
    site_content = sites.get_content(); 
    
    createFetchThreads(); 
    createParseThreads(); 
   
    while(run){
        //continue to run 
    }
    free(p_fetch); 
    free(p_parse); 
}


void * fetch_handler(void *unused){
    while (run){
        Node current = fetch.pop(); 
        Curl curr_curl(current.sitename); 
        current.content = curr_curl.download(); 
        parse.push(current); 
    }
    return 0; 
}

void * parse_handler(void * unused){
    int freq; 
    while(run){
        time_t now = time(0); 
        struct tm tstruct; 
        char buf[80]; 
        string output; 
        //Get Current Time
        tstruct = *localtime(&now);
        strftime(buf, sizeof(buf), "%Y-%m-%d.%X", &tstruct); 
    
            Node current = parse.pop(); 
            Parser curr_parse(current.content); 
            for(unsigned int i=0; i<search_content.size(); i++){
                freq = curr_parse.parse(search_content[i]); 
                string buf_s(buf); 
                output = buf_s + "," + search_content[i] + "," + current.sitename + "," + to_string(freq);
                parse.write(output, filename); 
            }
        }
    return 0; 
    
}

void createFetchThreads(void){
    int rc; 
    for (int i=0; i<NUM_FETCH; i++){
        rc = pthread_create(&p_fetch[i], NULL, fetch_handler, NULL); 
        if (rc !=0){
            cout<<"Thread creation error"<<endl; 
            exit(1); 
        }
    }

}

void createParseThreads(void){
    int rc; 
    for(int i=0; i<NUM_PARSE; i++){
        rc = pthread_create(&p_parse[i], NULL, parse_handler, NULL); 
        if (rc !=0){
            cout<<"Thread creation error"<<endl; 
            exit(1); 
        }
    }


}
