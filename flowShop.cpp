
#include <iostream>
#include <vector>
#include <cmath>
#include <bits/stdc++.h>
#include <time.h>
#include <random>
#include <climits>
#include <chrono>

#include "Libs/RVND.h"
#include "Libs/destructConstruct.h"

//Função que calcula o completion time


int main(){

    auto start = std::chrono::high_resolution_clock::now();

    int m; //Número de workers ou máquinas
    int n; //Número de jobs
    float T; //Dita se as datas de entrega serão maiores ou menores
    float R; //Índice de depreciação
    float alpha=0.8; //Penalidade por atrasos

    std::cin>> n >> m;

    std::vector<int> sequenciaTarefas(n); //Onde será armazenada a solução do problema

    std::vector<int> dueDates(n, 0); //Datas de entregas dos jobs

    std::vector<std::vector<int>> processingTimes(m, std::vector<int>(n));

    //Lendo os tempos de processamento
    for(int i=0; i<m; i++){
        for(int j=0; j<n; j++)
            std::cin>>processingTimes[i][j];
    }

    // ---------------------------------------------//
    // Impressão das leituras para verificação
    std::cout<<"Máquinas: "<<m<<"\n";
    std::cout<<"Tarefas: "<<n<<"\n\n";
    std::cout<<"Datas de Entrega:\n";
    for(int j=0; j<n; j++)
        std::cout<<dueDates[j]<<"\n";

    std::cout<<"\nTempos de Processamento:\n";
    for(int i=0; i<m; i++){
        std::cout << "Máquina " << i+1 << ": ";
        for(int j=0; j<n; j++)
            std::cout << processingTimes[i][j] <<" ";
        std::cout<<"\n";
    }

    std::cout << "\nLeitura da Sequencia de tarefas!\n";
    for(int i=0; i<n; i++){
        std::cin >> sequenciaTarefas[i];
    }

    std::vector<std::vector<float>> compTime(n+1, std::vector<float>(m+1, 0));

    for(int i=1; i<=n; i++){
        for(int j=1; j<=m; j++){
            compTime[i][j] = std::max(compTime[i-1][j], compTime[i][j-1]) + processingTimes[sequenciaTarefas[k]][i][j];
        }
    }

    std::cout << "\nCalculo do completion time:\n";
    for(int i=1; i<=n; i++){
        for(int j=1; j<=m; j++){
            std::cout << compTime[i][j] << " ";
        }
        std::cout << "\n";
    }

    //Não estou considerando a sequencia pra calcular o completion time


    return 0;
}