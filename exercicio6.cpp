#include <iostream>
#include <thread>
#include <future>
#include <vector>
#include <semaphore.h>
#include <unistd.h>
using namespace std;
int copo=20;
int cont=0;
sem_t ga,b;
void serve(int n){
    for(int i=0;i<n;i++){
        sem_wait(&ga);
        if(cont!=0) cout << "erro" << endl;
        int c=rand()%3+1;
        copo=c;
        cont++;
        sem_post(&b);
    }
    sem_wait(&ga);
    copo=10;
    sem_post(&b);
}
void bebe(int id){
    while (true){
        sem_wait(&b);
        if(copo<=id){
            if(cont!=1) cout << "erro" << endl;
            cont--;
            copo=20;
            sem_post(&ga);
        }
        else sem_post(&b);
        if (copo==10){
            sem_post(&b);
            break;
        }

    }
    
}

int main(){
    vector<thread> threads;
    sem_init(&ga,0,1);
    sem_init(&b,0,0);
    thread g(serve,500000);
    threads.push_back(move(g));
    for(int i=0;i<3;i++){
        thread t(bebe,i+1);
        threads.push_back(move(t));
    }

    for ( thread &t : threads){
        t.join();
    }
    return 0;
}