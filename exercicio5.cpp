#include<mutex>
#include<condition_variable>
#include<iostream>
#include<thread>
#include <unistd.h>
using namespace std;

class MonitorCaixaPostal {
private:
    int **caixa;
    int *temMsg;
    condition_variable *cheio, *vazio;
    mutex mux,*m;
public:


MonitorCaixaPostal(int n){
    caixa=new int*[n];
    temMsg=new int[n];
    for(int i=0;i<n;i++){
        temMsg[i]=0;
        caixa[i]=new int[5];
        for(int j=0;j<5;j++){
            caixa[i][j]=-1;
        }
    }
    cheio=new condition_variable[n];
    vazio=new condition_variable[n];
    m=new mutex[n];
}
~MonitorCaixaPostal(){
    delete [] caixa;
    delete [] cheio;
    delete [] vazio;
    delete [] temMsg;
} 

void enviar(int pos, int msg) {
    unique_lock<mutex> lck(mux);
    while (temMsg[pos]==5)
        vazio[pos].wait(lck);
    m[pos].lock();
    for(int j=0;j<5;j++)
        if (caixa[pos][j]==-1){
            caixa[pos][j]=msg;
            break;
        }
    if (temMsg[pos]==5)
        cout<<"AQUI\n";
    temMsg[pos]++;
    cout<<pos<<" "<<temMsg[pos] << endl;
    m[pos].unlock();
    cheio[pos].notify_one();
}
void receber(int pos, int &msg) {
    unique_lock<mutex> lck(mux);
    while(temMsg==0)
        cheio[pos].wait(lck);
    m[pos].lock();
    for(int j=0;j<5;j++)
        if (caixa[pos][j]!=-1){
            msg=caixa[pos][j];
            caixa[pos][j]=-1;
            break;
        }
    temMsg[pos]--;
    cout<<pos<<" "<<temMsg[pos] << endl;
    m[pos].unlock();
    vazio[pos].notify_one();
}
};

MonitorCaixaPostal caixapostal(10);

void remetente(int n){
    for(int j=0;j<1000;j++){
        for (int i = 0; i < n; i++){
            caixapostal.enviar(i%10,i);
            usleep(1000);
        }
    }
}

void receptor(int n){
    sleep(5);
    for(int j=0;j<1000;j++){
        for (int i = 0; i < n; i++){
            int msg;
            caixapostal.receber(i%10,msg);
            usleep(1000);
        }
    }
}

int main(){
    int n=20;
    thread t1(remetente,n);
    thread t2(receptor,n);
    t1.join();
    t2.join();
    return 0;
}