#include <iostream>
#include <thread>
#include <future>
#include <vector>

using namespace std;

void func(int comeco,int fim,int **matriz1,int **matriz2,int **r,int n1, int n2){
    int soma;
    for (int i = comeco; i <fim ; i++){
        for (int j = 0; j < n2; j++){
            soma=0;
            for (int k = 0; k < n1; k++){
               soma+=matriz1[i][k]*matriz2[k][j];
            }
            r[i][j]=soma;
        }
    }
}

int main (){
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
    int t;
    cin>>t;
    int tamanho=m1/t;
    vector<thread> threads;
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
    func(0,m2,matriz1,matriz2,r,n1,n2);
    for (int i = 0; i < t-1; i++){
        thread t(func,tamanho*i,(tamanho*i)+tamanho,matriz1,matriz2,r,n1,n2);
        threads.push_back(move(t));
    }
    thread t1(func,tamanho*(t-1),m1,matriz1,matriz2,r,n1,n2);
    threads.push_back(move(t1));
    for ( thread &t : threads){
        t.join();
    }
    for (int i = 0; i < m1; i++){
        for (int j = 0; j < n2; j++){
            cout << r[i][j] << "\n";
        }
    }
    return 0;
}