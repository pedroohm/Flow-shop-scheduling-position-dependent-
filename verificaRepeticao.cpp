#include <iostream>
using namespace std;

int main(){
    //programa que le n numeros e verifica se ha repeticao
    int n, i, j, repetido = 0;
    cout << "Digite a quantidade de numeros: ";
    cin >> n;
    int vet[n];
    for(i = 0; i < n; i++){
        cin >> vet[i];
    }
    for(i = 0; i < n; i++){
        for(j = i+1; j < n; j++){
            if(vet[i] == vet[j]){
                repetido = 1;
                break;
            }
        }
        if(repetido == 1){
            break;
        }
    }
    if(repetido == 1){
        cout << "Ha repeticao de numeros" << endl;
    }
    else{
        cout << "Nao ha repeticao de numeros" << endl;
    }
    return 0;
}