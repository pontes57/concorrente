#include<iostream>
#include <omp.h>
#include<vector>
using namespace std;
const int tamanho=1000000000;
int *A=new int [tamanho];
int *B=new int [tamanho];
long long int total=0;

void preenche(){
    int eu=omp_get_thread_num();
    if(eu==0){
        for (int i = 1; i < tamanho; i++){
            A[i]=(A[i-1]+i)%100;
        }
    }
    else{
        for (int i = 1; i < tamanho; i++){
            B[i]=(B[i-1]+2*i)%100;
        }
    }
}

int main(){
    A[0]=0;
    B[0]=0;

    #pragma omp parallel num_threads(2)
    preenche();

    #pragma omp parallel for reduction (+:total)
    for (int i = 0; i < tamanho; i++){
       total+=(A[i]*B[i]);
    }

    cout << total << endl;
}