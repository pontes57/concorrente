#include<mutex>
#include<condition_variable>
#include<iostream>
#include<thread>
#include<vector>
using namespace std;

int cont=0; //contador usado fora do monitor apenas para teste

class MonitorMontanhaRussa {
    private:
        int maxPessoas;         //capacidade máxima do carro
        int numPessoas=0;       //pessoas dentro do carro
        bool andando=false;     //carro dando volta
        bool esvaziando=false;  //passageiros estão saindo do carro
        condition_variable podeentrar,podesair,podeandar, vazio;
        mutex mux;
    public:
        MonitorMontanhaRussa(int capacidade) {
            maxPessoas=capacidade;
        }

        ~MonitorMontanhaRussa() {}

        void espera_encher() {
            unique_lock<mutex> lck(mux);
            podeentrar.notify_all();
            //cout<<"espera encher"<<endl;
            podeandar.wait(lck, [this]{return numPessoas==maxPessoas;});
            andando=true;
        }

        void espera_esvaziar() {
            unique_lock<mutex> lck(mux);
            andando=false;
            esvaziando=true;    //evitar que alguém entre antes que todos saiam
            podesair.notify_all();
            //cout<<"espera esvaziar"<<endl;
            vazio.wait(lck,[this]{ return numPessoas==0;} );
            esvaziando=false;
        }

        void entra_no_carro(int id, int i) {
            unique_lock<mutex> lck(mux);
            podeentrar.wait(lck, [this]{ return !andando && !esvaziando && numPessoas<maxPessoas;});
            numPessoas++;
            cont++;
            //cout<<"cliente "<<id<<" entra para volta "<<i<<endl;
            if(numPessoas==maxPessoas)
                podeandar.notify_one();
        }

        void sai_do_carro(int id, int i){
            unique_lock<mutex> lck(mux);
            podesair.wait(lck, [this]{return !andando && esvaziando;});
            numPessoas--;
            cont--;
            //cout<<"cliente "<<id<<" sai"<<endl;
            if(numPessoas==0)
                vazio.notify_one();
        }
};

int capacidade=20;

/* Testa se a quantidade de pessoas é diferente do esperado */
void dandoVolta() {
    if(cont!=capacidade)
        cout<<"cont = "<<cont<<endl;
}

pthread_barrier_t *bar;
MonitorMontanhaRussa montanharussa(capacidade);

void carro(int n) {
    for(int i=0; i<n; ++i) {
        if (i%5000==0) cout<<"preparando volta "<<i<<endl;
        montanharussa.espera_encher();
        dandoVolta();   //verifica se cont tem valor inválido
        /*Dar chance para que outras threads executem. Se tudo estiver 
        funcionando isso não fará diferença porque o carro está andando e
        ninguém pode sair e nem entrar */
        this_thread::yield(); 
        dandoVolta();   //verifica novamente se cont foi alterado
        montanharussa.espera_esvaziar();
    }
}

void cliente(int id, int n) {
    for(int i=0; i<n; ++i) {
        montanharussa.entra_no_carro(id,i);
        montanharussa.sai_do_carro(id,i);

        /* barreira utilizada para fazer com que todos os clientes do parque 
        deem a volta i antes de começar a volta i+1 */
        pthread_barrier_wait(&bar[i]);
    }
}

int main() {
    int ncli=capacidade*2;
    int voltasCarro = 100000;
    int voltasCli = voltasCarro/2;

    bar=new pthread_barrier_t[voltasCli];
    for (int i=0; i<voltasCli; ++i)
        pthread_barrier_init(&bar[i],NULL,ncli);

    vector<thread> threads;
    threads.push_back(thread(carro,voltasCarro));
    for(int i=1; i<=ncli; ++i)
        threads.push_back(thread(cliente,i,voltasCli));
    
    for(thread &t : threads)
        t.join();

    delete [] bar;
}