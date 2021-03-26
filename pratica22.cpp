#include<pthread.h>
#include<map>
#include<iostream>
#include<queue>
#include <vector>

using namespace std;


pthread_mutex_t mux=
PTHREAD_MUTEX_INITIALIZER;
map<pthread_t,int> A;
pthread_mutex_t muxthread=PTHREAD_MUTEX_INITIALIZER;
int R[]={0,0,0,0,0};
queue<pthread_t> fila[5];

void block() {
    bool sair=false;
    long eu=pthread_self();
    do {
    pthread_mutex_lock(&mux);
    if(A[eu] > 0) {
    A[eu]--;
    sair=true;
    }
    pthread_mutex_unlock(&mux);
    } while (!sair);
}

void wakeup(pthread_t t) {
    pthread_mutex_lock(&mux);
    A[t]++;
    pthread_mutex_unlock(&mux);
}


void libera(int r) {
    pthread_mutex_lock(&muxthread);
    if(!fila[r].empty()) {
    pthread_t id=fila.front();
    fila.pop();
    wakeup(id);
    }
    pthread_mutex_unlock(&muxthread);
    R[r]--;
}

int requisita(int r) {
    bool bloquear;
    do {
        bloquear=false;
        pthread_mutex_lock(&muxthread);
        if(R[r]!=0) {
            bloquear=true;
            fila[r].push(pthread_self());
        } else {
            r=R[r]=1;
        }
        pthread_mutex_unlock(&muxthread);
            if(bloquear) block();
    } while(bloquear);
}

void func(int r){
    requisita(r);
    cout << R[r] << "\n";
    libera(r);
}

int main() {
    for (int i = 0; i < 5; i++){
        for (int j = 0; j < 3; j++){
        pthread_t t;
        pthread_create(&t,NULL,func,i);
        // t armazena o tid (thread id)
        A.insert(pair<pthread_t,int>(t,0));
        }
}
    for (auto &p:A) {
        pthread_join(p.first,NULL);
    }
}
