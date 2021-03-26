#include<mutex>
#include<condition_variable>
#include<iostream>
#include<thread>
#include <unistd.h>
#include <vector>
using namespace std;

vector <thread> threads;

class MonitorCaixaPostal {
private:
    int *carro;
    condition_variable cheio, vazio,pentrar;
    mutex mux;
    int n,c;
    int dentro=0;
    int saiu=0;
public:

MonitorCaixaPostal(int a,int b){
    n=a;
    c=b;
    carro=new int[c];
    for (int i = 0; i < c; i++){
        carro[i]=-1;
    }
    
}
~MonitorCaixaPostal(){
    delete [] carro;
}
void espera_encher(){
    unique_lock<mutex> lck(mux);
    while (dentro!=c){
        vazio.wait(lck);
    }
}
void espera_esvaziar(){
    unique_lock<mutex> lck(mux);
    while (saiu!=0){
        cheio.wait(lck);
    }
    pentrar.notify_all();
}
void entra_no_carro(int id){
    unique_lock<mutex> lck(mux);
    while (dentro==c){
        pentrar.wait(lck);
    }
    for (int i = 0; i < c; i++){
        if (carro[i]==-1){
            carro[i]=id;
            break;
        }
    }
    dentro++;
    saiu--;
    if (dentro==c)
        vazio.notify_one();
}
void sai_do_carro(int id){
    
}
void teste(){
    for (int i = 0; i < c; i++){
        cout<<carro[i]<<" ";
    }
    cout <<endl;
}

};

void montanha(int n,MonitorCaixaPostal &russa){
    for(int i=0; i<n; ++i) {
        russa.espera_encher();
        cout << "saiu: ";
        russa.teste();
        russa.espera_esvaziar();
    }
}

void cliente(int n,int id,MonitorCaixaPostal &russa) {
    for(int i=0;i<n;i++)
        russa.entra_no_carro(id);
}
int main(){
    int c;
    cin >>c;
    int n=10*c;
    int v;
    cin>>v;
    MonitorCaixaPostal russa(n,c);
    thread t1(montanha,10*v,ref(russa));
    for (int j = 0; j < n; j++){
        thread t(cliente,v,j,ref(russa));
        threads.push_back(move(t));
    }
    for(auto &t:threads)
        t.join();
    t1.join();
    return 0;
}