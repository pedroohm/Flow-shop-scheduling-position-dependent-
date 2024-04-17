
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
    //std::vector<std::vector<int>> processingTimes(m+1, std::vector<int>(n+1, 0)); //Considerar que a matriz processing times, será equivalente ao Completion time
    
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
//Construção da matriz tridimensional TP

    //TP[j][i][k] = quanto tempo a tarefa j demorará a ser feita pela máquina i
    //   caso ela esteja na posição k

    //  k = j  //(nº posições = nº de tarefas)
    //  TP[j][i][k] = processingTimes[i][j] * k^alpha;
    
    std::vector<std::vector<std::vector<float>>> TP(n, std::vector<std::vector<float>>(m, std::vector<float>(n)));

    int count_j = 0;
    int count_i = 0;
    int count_k = 0;
    int k = 0;
    for(int j = 0; j < n; j++) {
        count_j++;
        for(int i = 0; i < m; i++) {
            count_i++;
            for(int k = 0; k < n; k++) {
                TP[j][i][k] = processingTimes[i][j] * (std::pow((k + 1.0), alpha));
                count_k++;
            }
        }
    }

 /*
    std::cout << "i- " << count_i << std::endl;
    std::cout << "j- " << count_j << std::endl;
    std::cout << "k- " << count_k << std::endl;
    std::cout << "m- " << m << std::endl;
    std::cout<<"\n";
*/


//-------------------------------------------------------//
//Definindo uma solução inicial (pi0) para o problema
//Uma solução é uma ordem para a execução das tarefas pela máquinas 1

    //Calculando a média de todos os tempos das tarefas em todas as máquinas
    std::vector<std::pair<float, int>> temposMedios(n, std::pair<float, int>(0.0f, 0));
    for(int j=0; j<n; j++){
        float soma = 0;
        for(int i=0; i<m; i++){
            soma += processingTimes[i][j];
        }
        float p_medio = soma / m;
        //temposMedios[j] = std::make_pair((dueDates[j] / p_medio), j); calculo original
        temposMedios[j] = std::make_pair(dueDates[j], j);

    }

    std::sort(temposMedios.begin(), temposMedios.end());

    std::cout<<"Tempos Médios - Solução Inicial:\n";
    for(int j=0; j<n; j++){
        sequenciaTarefas[j] = temposMedios[j].second;
        std::cout<<"("<<temposMedios[j].second+1<<", "<<temposMedios[j].first<<")\n";
    }

    std::cout<<"\n";

    

   //Calcula completion time
   std::vector<std::vector<float>> compTime(n+1, std::vector<float>(m+1, 0));
    for(int k=1; k<=n; k++){
        for(int i=1; i<=m; i++){
            compTime[k][i] = std::max(compTime[k-1][i], compTime[k][i-1]) + TP[sequenciaTarefas[k-1]][i-1][k-1];
        }
    }

    //Impressão do completion time
    std::cout << "\nCalculo do completion time:\n";
    for(int k=1; k<=n; k++){
        for(int i=1; i<=m; i++){
            std::cout << compTime[k][i] << " ";
        }
        std::cout << "\n";
    }

    std::cout<<"Sequência de Tarefas - Solução Final\n";
    for(int j=0; j<n; j++)
        std::cout<<sequenciaTarefas[j]+1<<" ";
    std::cout<<"\n";

    std::cout << "\nMatriz TP tri-dimensional\n";
    for(int j = 0; j < n; j++) {
        for(int i = 0; i < m; i++) {
            for(int k = 0; k <n; k++) {
                std::cout << TP[j][i][k] << " ";
            }
            std::cout << "\n";
        }
        std::cout << "\n";
    }


    return 0;

//-------------------------------------------------------//

//Com a solução inicial pronta, agora é necessário melhorá-la
//Para isso será aplicado um RVND - Random Variable Neighborhood Descent
//Serão utilizadas duas estruturas de vizinhança: Swap e Insertion

//Para implementar o RVND será necessário saber o atraso máximo das soluções encontradas
    // -> Seja temposAtraso[i] o atraso da tarefa i em relação a sua data de entrega
    // -> Seja C[j][i] o tempo em que a tarefa j terminou de ser processada na máquina i
    std::vector<float> temposAtraso(n, 0.0f); // Inicializa um vetor de floats com tamanho n e todos os elementos como 0.0f

    //std::vector<std::vector<int>> C(n, std::vector<int>(m, 0));  
    std::vector<std::vector<int>> C(n+1, std::vector<int>(m+1, 0)); //matriz 2d para o Completion time
    
  //  for(int i=0; i<n; i++) C[i].resize(m);

    for(int k=0; k<n; k++){
   //   std::cout << "k- " << k << std::endl;
        for(int i=0; i<m; i++){
   //       std::cout << "i- " << i << std::endl;
   //       std::cout << "C[k-1] - " << C[sequenciaTarefas[k-1]][i] << std::endl;
   //       std::cout << "TP - " << TP[sequenciaTarefas[k]][i][k] << std::endl;
   //       std::cout << "C[k] - " << C[sequenciaTarefas[k]][i+1] << std::endl;
          if(k==0 && i==0){
                C[sequenciaTarefas[k]][i] = TP[sequenciaTarefas[k]][i][k];
                //std::cout<<TP[sequenciaTarefas[k]][i][k]<<"-"<<k<<"-"<<i<<"\n";
          }
         else if (i == 0){
            C[sequenciaTarefas[k]][i+1] = C[sequenciaTarefas[k-1]][i] + TP[sequenciaTarefas[k]][i][k];
          } 
           else if (k==0){
             C[sequenciaTarefas[k]][i] = TP[i-1][sequenciaTarefas[k]][k] + TP[i][sequenciaTarefas[k]][k];
          }   
            else{
                C[sequenciaTarefas[k]][i] = std::max((C[sequenciaTarefas[k-1]][i]+TP[sequenciaTarefas[k]][i][k]), (C[sequenciaTarefas[k]][i-1]+TP[sequenciaTarefas[k]][i][k]));
//                // std::cout<<TP[sequenciaTarefas[k]][i][k]<<"-"<<k<<"-"<<i<<"\n";
            }

        
        }
  }
  float atraso_maximo = std::max(0, (C[sequenciaTarefas[0]][m-1]-dueDates[0]));
    for(int k=0; k<n; k++){
        if((C[sequenciaTarefas[k]][m-1]-dueDates[k]) > 0) temposAtraso[k] = (C[sequenciaTarefas[k]][m-1]-dueDates[k]);
        else temposAtraso[k] = 0.0;

        if(temposAtraso[k] > atraso_maximo) atraso_maximo = temposAtraso[k];
    }

/*
    //-------------------------------------------------------//
//Implementação do RVND - a solução que sai do RVND é a pi

//Sorteando a ordem de aplicação das vizinhanças 

    unsigned seed = time(0);
    srand(seed);

    RVND(n, m, atraso_maximo, sequenciaTarefas, TP, temposAtraso, dueDates, processingTimes, alpha);
//-------------------------------------------------------//
//A solução encontrada até agora será usada como entrada para uma Metaheurísticas
//Será usada a Iterated Greedy para melhorar a solução, que irá iterar até que a 
//solução não seja melhorada por 50 iterações seguidas 

    int iteracoes_sem_melhora = 0;
    //Fator de depreciação
    int d;
    if(n<20) d = 2;
    else if(n<60) d = 4;
    else if(n<100) d = 6;
    else d = 8;

    while(iteracoes_sem_melhora < 50){

        std::vector<int> solucao_pi1(n);
        float atraso_maximo_pi1 = 0;

        solucao_pi1 = sequenciaTarefas;
        atraso_maximo_pi1 = atraso_maximo;

    //     //Destruindo a solução para explorar novas vizinhanças
    //     destructConstruct(n, m, atraso_maximo, sequenciaTarefas, solucao_pi1, atraso_maximo_pi1, d, TP, dueDates);

        RVND(n, m, atraso_maximo_pi1, solucao_pi1, TP, temposAtraso, dueDates, processingTimes, alpha);

        if(atraso_maximo_pi1 < atraso_maximo){

            sequenciaTarefas = solucao_pi1;
            atraso_maximo = atraso_maximo_pi1;

            iteracoes_sem_melhora = 0;
        }
        else iteracoes_sem_melhora++;
    }

*/

//-------------------------------------------------------//

    std::cout<<"Sequência de Tarefas - Solução Final\n";
    for(int j=0; j<n; j++)
        std::cout<<sequenciaTarefas[j]+1<<" ";
    std::cout<<"\n";

    std::cout<<"Atraso Máximo: "<<atraso_maximo;

    auto stop = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
 
    std::cout << "\nTempo: " << duration.count()/1000000.0 << " segundos\n";

//-------------------------------------------------------//

  return 0;
}
