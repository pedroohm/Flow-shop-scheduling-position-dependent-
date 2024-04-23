
#include <iostream>
#include <vector>
#include <cmath>
#include <bits/stdc++.h>
#include <time.h>
#include <random>
#include <climits>
#include <chrono>
#include <algorithm>

#include "Libs/RVND.h"
#include "Libs/destructConstruct.h"

void calculaTP(int n, int m, std::vector<std::vector<int>> processingTimes, std::vector<std::vector<std::vector<float>>> &TP, float alpha){
    for(int j=0; j<n; j++){
        for(int i=0; i<m; i++){
            for(int k=0; k<n; k++){
                TP[j][i][k] = processingTimes[i][j] * (std::pow((k + 1.0), alpha));
            }
        }
    }
}

void calculaCompletionTime(int n, int m, std::vector<int> sequenciaTarefas, std::vector<std::vector<std::vector<float>>> &TP, std::vector<std::vector<float>> &compTime){
    for(int k=1; k<=n; k++){
        for(int i=1; i<=m; i++){
            compTime[k][i] = std::max(compTime[k-1][i], compTime[k][i-1]) + TP[sequenciaTarefas[k-1]][i-1][k-1];
        }
    }
}

float atrasoMaximo(int n, int m, std::vector<int> sequenciaTarefas, std::vector<std::vector<float>> compTime, std::vector<int> dueDates){
    float atraso = 0;
    for(int k=0; k<n; k++){
        atraso = compTime[k+1][m] - dueDates[sequenciaTarefas[k]];
    }
    return atraso;
}

std::vector<std::pair<float, int>> ordemTarefas(int n, int m, std::vector<int> dueDates, std::vector<std::vector<int>> processingTimes, int param){
    std::vector<std::pair<float, int>> temposMedios(n, std::pair<float, int>(0.0f, 0));
    
    if (param ==1){
        for(int j=0; j<n; j++){
            float soma = 0;
            for(int i=0; i<m; i++){
                soma += processingTimes[i][j];
            }
            float p_medio = soma / m;
            temposMedios[j] = std::make_pair((dueDates[j] / p_medio), j);
            
        }
    } else {
        for(int j=0; j<n; j++)
            temposMedios[j] = std::make_pair(dueDates[j], j);
    }

    std::sort(temposMedios.begin(), temposMedios.end());
    return temposMedios;
}

int main(){

    auto start = std::chrono::high_resolution_clock::now();

    int m; //Número de workers ou máquinas
    int n; //Número de jobs
    float T; //Dita se as datas de entrega serão maiores ou menores
    float R; //Índice de depreciação
    float alpha=0.8; //Penalidade por atrasos

    std::cin>>m>>n>>T>>R;

    std::vector<int> sequenciaTarefas(n); //Onde será armazenada a solução do problema

    std::vector<int> dueDates(n, 0); //Datas de entregas dos jobs

    std::vector<std::vector<int>> processingTimes(m, std::vector<int>(n));
    
    //Lendo as datas de entrega
    for(int j=0; j<n; j++)
        std::cin>>dueDates[j];

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
        for(int j=0; j<n; j++)
            std::cout<<processingTimes[i][j]<<" ";
        std::cout<<"\n";
    }

    //-------------------------------------------------------//   
    //          Construção da matriz tridimensional TP
    //TP[j][i][k] = quanto tempo a tarefa j demorará a ser feita pela máquina i
    //   caso ela esteja na posição k
    //  TP[j][i][k] = processingTimes[i][j] * k^alpha;
    std::vector<std::vector<std::vector<float>>> TP(n, std::vector<std::vector<float>>(m, std::vector<float>(n)));
    calculaTP(n, m, processingTimes, TP, alpha);

    //-------------------------------------------------------//
    //  Definindo uma solução inicial (pi0) para o problema
    //Calculando a média de todos os tempos das tarefas em todas as máquinas
    std::vector<std::pair<float, int>> temposMedios(n, std::pair<float, int>(0.0f, 0));
    
    //T == 0 -> Ordem crescente de dueDates, T == 1 -> Ordenado pela média de dueDates
    temposMedios = ordemTarefas(n, m, dueDates, processingTimes, 1);
    
    // Armazenando a sequência de tarefas
    for(int j=0; j<n; j++) sequenciaTarefas[j] = temposMedios[j].second;

    
    std::cout<<"\nOrdenação por dueDates - Solução Inicial:\n";
    for(int j=0; j<n; j++){
        std::cout<<"("<<temposMedios[j].second+1<<", "<<temposMedios[j].first<<")\n";
    }
     
   //Calcula completion time
   std::vector<std::vector<float>> compTime(n+1, std::vector<float>(m+1, 0));
   calculaCompletionTime(n, m, sequenciaTarefas, TP, compTime);

    /*
    //Impressão do completion time
    std::cout << "\nCalculo do completion time:\n";
    for(int k=1; k<=n; k++){
        for(int i=1; i<=m; i++){
            std::cout << compTime[k][i] << " ";
        }
        std::cout << "\n";
    }
    */

    std::cout<<"Sequência de Tarefas - Solução Final\n";
    for(int j=0; j<n; j++)
        std::cout<<sequenciaTarefas[j]+1<<" ";
    std::cout<<"\n";
    
    float atraso_maximo = atrasoMaximo(n, m, sequenciaTarefas, compTime, dueDates);

    std::cout << "\nAtraso Máximo da solução: " << atraso_maximo << "\n";

    std::vector<int> solucao(sequenciaTarefas);

    // Procura melhores soluções aleatórias que diminuam o atraso máximo
    for(int i=0; i<100; i++){

        std::random_device rd;
        std::default_random_engine rng(rd());

        // Nova sequencia de tarefas aleatória
        std::shuffle(sequenciaTarefas.begin(), sequenciaTarefas.end(), rng);

        //Calcula completion time
        calculaCompletionTime(n, m, sequenciaTarefas, TP, compTime);

        float atraso = atrasoMaximo(n, m, sequenciaTarefas, compTime, dueDates);

        if(atraso < atraso_maximo){
            atraso_maximo = atraso;
            solucao = sequenciaTarefas;
            std::cout << "\nSquencia que diminui o atraso Maximo:\nNovo Atraso = " << atraso << "\n";
            for(int j=0; j<n; j++)
                std::cout<<sequenciaTarefas[j]+1<<" ";
            std::cout << "\n";
        }
    }

    //-------------------------------------------------------//
    std::cout<<"Atraso Máximo: "<<atraso_maximo;

    auto stop = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
 
    std::cout << "\nTempo: " << duration.count()/1000000.0 << " segundos\n";
    //-------------------------------------------------------//


    return 0;

}