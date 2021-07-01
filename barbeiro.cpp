#include<mutex>
#include<condition_variable>
#include<iostream>
#include<thread>
#include<vector>
using namespace std;

int cont=0; //contador para as pessos que cortaram o cabelo
int caideiras=50;//maximo de pessoas que cabe na barbearia
int quantidade=10000;//nao posso criar milhares de threads entao cada thread vai executar esse tanto de chamadas
int cont1=0;//contador pra quantas pessoas o barbeiro cortou o cabelo
class barbearia {
    private:
        int maxPessoas;//maximo de pessoas que cabe na barbearia         
        int numPessoas=0;//numero de pessoas dentro da barbearia
        int cortando=-1;//guarda o id do cliente que ta cortando o cabelo
        int chamadas=0;//pra saber quando parar
        condition_variable esperando,barbeiroespera;
        mutex mux;
    public:
        barbearia(int cadeiras){
            maxPessoas=cadeiras;
        }
        void cliente(int id){
            unique_lock<mutex> lck(mux);
            if (numPessoas==0){//nao tem ninguem esperando
                numPessoas++;
                if(cortando!=-1)
                    cout << "erro cliente \n";
                cortando=id;
                cont++;
                barbeiroespera.notify_one();
            }
            else if (numPessoas<maxPessoas){//espera pra cortar
                numPessoas++;
                while(cortando!=-1)
                    esperando.wait(lck);
                if(cortando!=-1)
                    cout << "erro cliente \n";
                cortando=id;
                cont++;
                barbeiroespera.notify_one();
            }
            else{//vai embora
                chamadas++;
                return;
            }
            chamadas++;
            }
        void barbeiro(){
            while(true){
                unique_lock<mutex> lck(mux);
                while (numPessoas==0)//caso nao tenha ninguem dorme
                    barbeiroespera.wait(lck);
                if(cortando==-1)
                    cout << "erro barbeiro \n";
                //corta o cabelo
                cortando=-1;
                cont1++;
                numPessoas--;
                esperando.notify_one();
                if(chamadas==(quantidade*1000))//ve se pode terminar o programa,se mudar o numero de threads tem de mudar o valor tbm
                    return;
                barbeiroespera.wait(lck);//espera o cara sentar pra cortar o cabelo dele
            }
        }
};
barbearia objeto(caideiras);
void barbeiro(){
    objeto.barbeiro();
}
void cliente(int id){
    for(int i=0;i<quantidade;i++)
    objeto.cliente((id*quantidade+i));
}
int main(){
    vector<thread> threads;
    thread b(barbeiro);
    threads.push_back(move(b));
    for(int i=0;i<1000;i++){
        thread t(cliente,i);
        threads.push_back(move(t));
    }

    for ( thread &t : threads){
        t.join();
    }
    cout << cont << " " << cont1 << endl;//para ver se o numero de pessoas que cotaram o cabelo é igual ao numero de pessoas que o barbeiro cortou o cabelo

    return 0;
}