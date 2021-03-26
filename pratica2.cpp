#include <thread>
#include <mutex>
#include <iostream>
#include <vector>
#include <unistd.h>
#include <utility>
using namespace std;

vector <thread> threads;
mutex m[5];
int recurso[5]={0,0,0,0,0};

void libera(int r)
{
    m[r].unlock();
}

void requisita(int r){
    m[r].lock();    
}

void soma (int r){
    requisita(r);
    for (int i = 0; i < 30; i++){
        recurso[r]++;
        sleep(1);
        cout << recurso[r] << " " << r <<"\n";
    }
    libera(r);
}

int main()
{
    for (int i = 0; i < 5; i++){
        for (int j = 0; j < 3; j++){
            thread t(soma,i);
            threads.push_back(move(t));
        }
    }

    for(auto &t:threads)
        t.join();


}