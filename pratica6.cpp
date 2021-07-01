#include<iostream>
#include <omp.h>
#include<vector>
using namespace std;

int main(){
    int m1;
    cin>>m1;
    int n1;
    cin>>n1;
    int m2=n1;
    int n2;
    cin>>n2;
    int **matriz1=new int *[m1];
    int **matriz2=new int *[m2];
    int ** r=new int *[m1];
    for (int i = 0; i < m1; i++){
        matriz1[i]=new int[n1];
        r[i]=new int[n2];
        for (int j = 0; j < n1; j++){
            matriz1[i][j]=2;
        }
    }
    for (int i = 0; i < m2; i++){
        matriz2[i]=new int[n2];
        for (int j = 0; j < n2; j++){
            matriz2[i][j]=2;
        }
    }
    int soma;
    double start; 
    double end; 
    start = omp_get_wtime(); 
    #pragma omp parallel for default (none) shared (matriz1,matriz2,r,m1,n2,n1) private(soma)
    for (int i = 0; i <m1 ; i++){
        //#pragma omp parallel for default (none) shared (matriz1,matriz2,r,m1,n2,n1,i) private(soma)
        for (int j = 0; j < n2; j++){
            soma=0;
            //#pragma omp parallel for default (none) shared (matriz1,matriz2,r,m1,n2,n1,i,j) reduction(+:soma)
            for (int k = 0; k < n1; k++){
                soma+=matriz1[i][k]*matriz2[k][j];
            }
            r[i][j]=soma;
        }
    }
    end = omp_get_wtime(); 
    printf("Work took %f seconds\n", end - start);
    int numero=r[0][0];
    for (int i = 0; i < m1; i++){
        for (int j = 0; j < n2; j++){
            if(r[i][j]!=numero)//como todas as matrizes so tem 2 a matriz resultado tem q ter o numero em todas as posicoes
                cout << "erro" << endl;
        }
    }
}