#ifndef PARSEQUEUE_H
#define PARSEQUEUE_H

#include <string>
#include <thread>
#include <vector>
#include <queue>
#include "Node.h"

class ParseQueue{

    public:
        ParseQueue(){
            //ready = 0; 
        } 
        void push(Node curr_node){
            pthread_mutex_lock(&mutex);
            q.push(curr_node); 
            pthread_cond_broadcast(&cond); 
            pthread_mutex_unlock(&mutex); 
        }
        Node pop(){
            pthread_mutex_lock(&mutex); 
            while(empty()){
                pthread_cond_wait(&cond, &mutex); 
            }
            Node result =q.front(); 
            q.pop(); 
            pthread_mutex_unlock(&mutex); 
            return result; 
        }
        bool empty(){
            return q.empty(); 
        }

    private:
        queue<Node> q; 
        pthread_mutex_t mutex; 
        pthread_cond_t cond; 

};

#endif
