#include <iostream>
#include <thread>
#include <future>
#include <vector>
#include <semaphore.h>
#include <unistd.h>
using namespace std;
sem_t ga,b;
int pedaco=0;
//int canibal [1000];   as partes comentadas servem para testa o codigo mas deixam ele mais lento,o tamanho desse arreio deve corresponder ao numero de canibais
//int cont=0;
void cozinha(int n){
    for(int i=0;i<n;i++){
        sem_wait(&ga);
        if(pedaco!=0 )cout << "erro "<< i << endl;
        pedaco=20;
        sem_post(&b);
    }
    sem_wait(&ga);
    pedaco=-1;
    sem_post(&b);
}
void come(int id){
    while (true){
        sem_wait(&b);
        if(pedaco!=0 && pedaco>0){
            if(pedaco<=0)cout << "erro" << id << endl;
            pedaco--;
            sem_post(&b);
            //canibal[id]++;
        }
        else if (pedaco==0) sem_post(&ga);
        if (pedaco==-1){
            sem_post(&b);
            break;
        }

    }
    
}
int main(){
    vector<thread> threads;
    sem_init(&ga,0,1);
    sem_init(&b,0,0);
    thread g(cozinha,1000000);
    threads.push_back(move(g));
    for(int i=0;i<1000;i++){
        //canibal[i]=0;
        thread t(come,i);
        threads.push_back(move(t));
    }

    for ( thread &t : threads){
        t.join();
    }
    /*for(int i=0;i<1000;i++){
        cont+=canibal[i];
    }
    cout << cont << endl;*/
    return 0;
}