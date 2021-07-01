#include <iostream>
#include <cmath>
#include<list>
#include<omp.h>
using namespace std;
/* nos meus testes notei que paralelisar os dois for aumenta muito o tempo de execucao visto que ambos vao disputar as threads entre si
decidi paralelizar o for de fora pois ele ira emglobar a funcao sqrt que pode levar muito tempo para numeros grandes
nos testes o melhor escalonamento foi o dynamic provavelmente pelo fato dele nao deixar thredas paradas se ainda ha numeros a ser processado
e seu funcionamento se deu melhor para numeros bem pequenos por isso optei por pegar o numero de threds da maquina*/

int main()
{
    int aux,Max=40000000, div;
    list <int> primos;
    #pragma omp parallel for schedule (dynamic,omp_get_num_threads()) private(div,aux)
    for(int i=2 ; i<=Max ; i++){
        div=0;
        aux=sqrt(i);
        for(int j=2 ; j<=aux ; j++)
            if(i%j==0)
                div++;

        //if(!div)
            //cout<<i<<" ";
    }

    cout<<endl;

    return 0;
}