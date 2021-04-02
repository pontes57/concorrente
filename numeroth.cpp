#include<mutex>
#include<condition_variable>
#include<iostream>
#include<thread>
#include<vector>
using namespace std;
int variavel=0;
int N=30000;
const int quantidadeth=30000;
bool vec[quantidadeth];
class numeroth
{
private:
    int numax;
    int nuatual=0;
    mutex mux;
    condition_variable_any C;
public:
    numeroth(int n){
        numax=n;
    }
    void requisita(int id){
        mux.lock();
        while ((nuatual+id)>numax) C.wait(mux);
        nuatual+=id;
        mux.unlock();
    }
    void libera(int id){
        mux.lock();
        nuatual=nuatual-id;
        mux.unlock();
        C.notify_all();
        vec[id]=true;
    }
    void testa(){
        if(nuatual>numax)
            cout <<"erro\n";
}
};

numeroth objeto(N);

void pega(int id){
    objeto.requisita(id);
    objeto.testa();
    variavel++;//acessa a variavel
    objeto.libera(id);
}


int main(){
      for (int i = 1; i <quantidadeth ; i++){
        vec[i]=false;
    }
    vector<thread> threads;
    for(int i=1;i<quantidadeth;i++){
        thread t(pega,i);
        threads.push_back(move(t));
    }

    for ( thread &t : threads){
        t.join();
    }
    for (int i = 1; i <quantidadeth ; i++){
        if(vec[i]==false)cout << "erro\n";
    }
    return 0;
}