
#include <iostream>
#include <vector>
#include <cmath>
#include <bits/stdc++.h>
#include <time.h>
#include <random>
#include <climits>
#include <chrono>
#include <algorithm>

void imprimeSequencia(const std::vector<int> &sequencia){
    std::cout<<"\n";
    for(int j=0; j<sequencia.size(); j++)
        std::cout<<sequencia[j]+1<<" ";
    //std::cout<<"\n";
}

void imprimeCompTime(const std::vector<std::vector<float>> &compTime){
    std::cout << "\nCompletion Time\n";
    for(int k=1; k<=compTime.size(); k++){
        for(int i=1; i<=compTime[0].size(); i++){
            std::cout << compTime[k][i] << " ";
        }
        std::cout << "\n";
    }
    std::cout << "\n";
}

void calculaTP(const int n, const int m, std::vector<std::vector<int>> &processingTimes, std::vector<std::vector<std::vector<float>>> &TP, float alpha){
    for(int j=0; j<n; j++){
        for(int i=0; i<m; i++){
            for(int k=0; k<n; k++){
                TP[j][i][k] = processingTimes[i][j] * (std::pow((k + 1.0), alpha));
            }
        }
    }
}

void calculaCompletionTime(const int n, const int m, const std::vector<int> &sequenciaTarefas, const std::vector<std::vector<std::vector<float>>> &TP, std::vector<std::vector<float>> &compTime){
    compTime.resize(n + 1, std::vector<float>(m + 1));
    for (int i = 0; i <= m; ++i)
        compTime[0][i] = 0;
    for (int k = 0; k <= n; ++k)
        compTime[k][0] = 0;
    
    for(int k=1; k<=n; k++){
        for(int i=1; i<=m; i++){
            compTime[k][i] = std::max(compTime[k-1][i], compTime[k][i-1]) + TP[sequenciaTarefas[k-1]][i-1][k-1];
        }
    }
}

float atrasoMaximo(const int n, const int m, const std::vector<int> &sequenciaTarefas, const std::vector<std::vector<float>> &compTime, const std::vector<int> &dueDates){
    float atraso = 0;
    int tarefa, indice;
    for(int k=1; k<=n; k++){
        float newAtraso = compTime[k][m] - dueDates[sequenciaTarefas[k-1]];
        if (newAtraso > atraso)
            atraso = newAtraso;
            tarefa = sequenciaTarefas[k-1];
            indice = k-1;
    }
    std::cout << "\nMaior atraso da tarefa " << tarefa << "pois " << compTime[indice][m] << "-" << dueDates[sequenciaTarefas[indice]] << "\n";
    return atraso;
}

void imprimeCompletionTime(int n, int m, std::vector<std::vector<float>> compTime){
    std::cout << "\n Ultima linha da completion time:\n";
    for(int k=0; k<n; k++){         
        std::cout << compTime[k+1][m] << " ";
    }
}

std::vector<std::pair<float, int>> ordemTarefas(const int n, const int m, std::vector<int> &dueDates, std::vector<std::vector<int>> &processingTimes, const int param){
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

std::pair<std::vector<int>, float> destructConstruct(std::vector<int> &sequencia, std::vector<std::vector<float>> &completionTime, int d, int n, int m, std::vector<int> dueDates, const std::vector<std::vector<int>> &processingTimes, const std::vector<std::vector<std::vector<float>>> &TP, float alpha){
    std::vector<int> deletados;

    int size = sequencia.size();

    // Processo de destruição
    std::random_device rd; //verificar a complexidade desaa operacao
    std::uniform_int_distribution<int> dist(0, size-1);
    int pos = dist(rd); // define qual posicao será excluida
    while (d>0){        
        deletados.push_back(sequencia[pos]); // armazena o elemento que será excluído
        sequencia.erase(sequencia.begin() + pos);
        d--;
        size--;
        std::uniform_int_distribution<int> dist(0, size-1);
        pos = dist(rd); // qual a proxima posicao a excluir
    }

    // Processo de construção
    float menorAtraso;
    float atrasoAux;
    int pos;
    for(int i=0; i<deletados.size(); i++){
        menorAtraso = INT_MAX;
        pos = 0;
        std::vector<int> seqParcial;
        for(int j=0; j<sequencia.size(); j++){
            seqParcial = sequencia;
            seqParcial.insert(seqParcial.begin() + j, deletados[i]);
            completionTime.resize(seqParcial.size()+1, std::vector<float>(m+1, 0));
            calculaCompletionTime(seqParcial.size(), m, seqParcial, TP, completionTime);
                    
            atrasoAux = atrasoMaximo(seqParcial.size(), m, seqParcial, completionTime, dueDates);
            if(atrasoAux < menorAtraso){
                menorAtraso = atrasoAux;
                pos = j;
            }
        }
        
        sequencia.insert(sequencia.begin() + pos, deletados[i]);
    }
    //std::cout << "\nSaindo da destruct construct";
    //retornar um par sendo a sequencia e o menorAtraso
    return std::make_pair(sequencia, menorAtraso);
}

std::pair<std::vector<int>, float> generateNeighbors(const std::vector<int> &sequencia, float melhorAtraso, int n, int m, std::vector<std::vector<float>> &completionTime, const std::vector<int> &dueDates, const std::vector<std::vector<int>> &processingTimes, const std::vector<std::vector<std::vector<float>>> &TP){
    std::vector<int> vizinho(n);
    std::vector<int> melhorVizinho(n, 0); // Inicialize com uma sequência inválida
    float melhorAtrasoVizinho = melhorAtraso;
    int distancia;

    if (n<50) distancia = n/2;
    else distancia = n/4; 

    //std::cout << "\nVizinhos:\n";

    for(int job = 0; job < n; job++){
        for(int pos = 0; pos < n; pos++){
            if (job != pos && pos != (job-1) && std::abs(job-pos) < distancia){
                int k, j;
                k = j = 0;

                while(k < n){
                    if (j!=job){
                        if(k == pos) 
                            vizinho[k] = sequencia[job];
                        else {
                            vizinho[k] = sequencia[j];
                            j++;
                        }
                        k++;
                    }
                    else j++;
                }
                
                calculaCompletionTime(n, m, vizinho, TP, completionTime);
                float atraso = atrasoMaximo(n, m, vizinho, completionTime, dueDates);
                
                //imprimeSequencia(vizinho);
                //std::cout << " ->" << atraso;

                if (atraso < melhorAtrasoVizinho){
                    melhorVizinho = vizinho;
                    melhorAtrasoVizinho = atraso;
                }                
            }            
        }
    }

    //std::cout << "\nSaindo do gerador de vizinhos,  " 
    return std::make_pair(melhorVizinho, melhorAtrasoVizinho);
}

std::pair<std::vector<int>, float> buscaLocal(std::vector<int> &sequencia, float melhorAtraso, int n, int m, std::vector<std::vector<float>> &completionTime, std::vector<int> dueDates, const std::vector<std::vector<int>> &processingTimes, const std::vector<std::vector<std::vector<float>>> &TP){
    bool melhora = true;
    //std::cout << "\nEntrando na Busca Local com atraso = " << melhorAtraso;
    int i=1;
    while(melhora){
        melhora = false;
        std::pair<std::vector<int>, float> sequenciaTmax = generateNeighbors(sequencia, melhorAtraso, n, m, completionTime, dueDates, processingTimes, TP);
        std::cout << "\nSequencia inicial " << i++;
        //imprimeSequencia(sequencia);
        std::cout << " -> " << melhorAtraso;

        if(sequenciaTmax.second < melhorAtraso){
            melhorAtraso = sequenciaTmax.second;
            sequencia = sequenciaTmax.first;
            melhora = true;
            std::cout << "\nMelhor vizinho: " << melhorAtraso << "\n";
        }
    }
    //std::cout << "\nSaindo da Busca Local com atraso = " << melhorAtraso;
    return std::make_pair(sequencia, melhorAtraso);
}

std::pair<std::vector<int>, float> iteratedGreedy(std::vector<int> initialSolution, float atrasoInicialSolution, int n, int m, int d, float alpha, std::vector<std::vector<float>> &completionTime, std::vector<int> dueDates, const std::vector<std::vector<int>> &processingTimes, const std::vector<std::vector<std::vector<float>>> &TP){
    std::vector<int> pi0(initialSolution);
    float atrasoPi0 = atrasoInicialSolution;

    std::pair<std::vector<int>, float> sequenceTmaxPi = buscaLocal(pi0, atrasoInicialSolution, n, m, completionTime, dueDates, processingTimes, TP);
    std::vector<int> pi = sequenceTmaxPi.first;
    float atrasoPi = sequenceTmaxPi.second;
    
    /*
    // se a busca local for ativada, desativar essa parte
    std::vector<int> piBest(pi);
    float atrasoPiBest = atrasoPi;
    */

    int i = 0;
    while (i<20){
        i++;
        std::cout << "\nITERAÇÃO: " << i << " - Atraso: " << atrasoPi << "\n";

        //chamada de constructDestruct removendo d das tarefas
        std::pair<std::vector<int>, float> sequenceTmaxPi1 = destructConstruct(pi, completionTime, d, n, m, dueDates, processingTimes, TP, alpha);
        std::vector<int> pi1(sequenceTmaxPi1.first);
        float atrasoPi1 = sequenceTmaxPi1.second;

        std::pair<std::vector<int>, float> sequenceTmaxPi2 = buscaLocal(pi1, atrasoPi1, n, m, completionTime, dueDates, processingTimes, TP);
        std::vector<int> pi2(sequenceTmaxPi2.first);
        float atrasoPi2 = sequenceTmaxPi2.second;
        
        if (atrasoPi2 < atrasoPi){
            pi = pi2;
            atrasoPi = atrasoPi2;
            i = 0;         
        }
    }

    return std::make_pair(pi, atrasoPi);
}

int main(int argc, char *argv[]){
    if (argc < 3) {
        std::cerr << "Insira alpha <valor_float> e a porcentagem de tarefas a serem removidas em destructConstruct <valor_float>" << std::endl;
        return 1;
    }

    std::ofstream arquivo("teste.csv", std::ios::out | std::ios::app);
    if (!arquivo.is_open()) {
        std::cerr << "Erro ao abrir o arquivo!" << std::endl;
        return 1;
    }

    int m; //Número de workers ou máquinas
    int n; //Número de jobs
    float T; //Dita se as datas de entrega serão maiores ou menores
    float R; //Índice de depreciação
    float alpha= std::stod(argv[1]); //Penalidade por atrasos
    float porcentagem = std::stod(argv[2]); //Porcentagem de tarefas a serem removidas destructConstruct

    std::cin>>m>>n>>T>>R;

    int d = n * porcentagem; //quantidade de tarefas a serem removidas

    arquivo << alpha << "," << porcentagem << "," << m << "," << n << "," << T << "," << R << ",";
    

    std::vector<int> solucao(n); //Onde será armazenada a solução do problema aplicando heristicas

    std::vector<int> dueDates(n, 0); //Datas de entregas dos jobs
    std::vector<int> sequenciaEDD(n); //Onde será armazenada a sequência de tarefas ordenadas por EDD

    std::vector<std::vector<int>> processingTimes(m, std::vector<int>(n));
    
    //Lendo as datas de entrega
    for(int j=0; j<n; j++)
        std::cin>>dueDates[j];

    //Lendo os tempos de processamento
    for(int i=0; i<m; i++){
        for(int j=0; j<n; j++)
            std::cin>>processingTimes[i][j];
    }

    //-------------------------------------------------------//   
    //          Construção da matriz tridimensional TP
    //TP[j][i][k] = quanto tempo a tarefa j demorará a ser feita pela máquina i
    //   caso ela esteja na posição k
    //  TP[j][i][k] = processingTimes[i][j] * k^alpha;
    std::vector<std::vector<std::vector<float>>> TP(n, std::vector<std::vector<float>>(m, std::vector<float>(n)));
    
    std::cout << "\nCalculando TP";
    calculaTP(n, m, processingTimes, TP, alpha);
    std::cout << "\nTP calculada";

    auto start = std::chrono::high_resolution_clock::now();

    //-------------------------------------------------------//
    //  Definindo uma solução inicial (pi0) para o problema EDD
    //Calculando a média de todos os tempos das tarefas em todas as máquinas
    std::vector<std::pair<float, int>> temposMedios(n, std::pair<float, int>(0.0f, 0));
    
    //P == 0 -> Ordem crescente de dueDates EDD, P == 1 -> Ordenado pela média de dueDates
    // Utilizando EDD para construir a solução inicial
    temposMedios = ordemTarefas(n, m, dueDates, processingTimes, 0);
    
    // Armazenando a sequência de tarefas
    for(int j=0; j<n; j++) {
        solucao[j] = temposMedios[j].second;
        std::cout << temposMedios[j].first << " - " << temposMedios[j].second << "\n";
    }
     
    //Calculo do completion time para a solução inicial
    std::vector<std::vector<float>> compTime(n+1, std::vector<float>(m+1, 0));
    calculaCompletionTime(n, m, solucao, TP, compTime);
    std::cout<<"\nSequência de Tarefas - Solução Inicial EDD";
    imprimeSequencia(solucao);

    //Calculo do atraso máximo para a solução inicial
    float atraso_maximo = atrasoMaximo(n, m, solucao, compTime, dueDates);
    std::cout << "\nAtraso Máximo da solução inicial EDD: " << atraso_maximo << "\n";

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<float> elapsed_seconds = end-start;
    
    arquivo << atraso_maximo << "," << elapsed_seconds.count() << ",";

    //-------------------------------------------------------//
    //                  Utilizando as médias
    start = std::chrono::high_resolution_clock::now();
    //P == 0 -> Ordem crescente de dueDates EDD, P == 1 -> Ordenado pela média de dueDates
    temposMedios = ordemTarefas(n, m, dueDates, processingTimes, 1);
    
    // Armazenando a sequência de tarefas
    for(int j=0; j<n; j++) {
        solucao[j] = temposMedios[j].second;
        //std::cout << temposMedios[j].first << " - " << temposMedios[j].second << "\n";
    }

    calculaCompletionTime(n, m, solucao, TP, compTime);
    std::cout<<"\nSequência de Tarefas - Solução Inicial Tempos Medios";
    imprimeSequencia(solucao);
    //Calculo do atraso máximo para a solução inicial
    atraso_maximo = atrasoMaximo(n, m, solucao, compTime, dueDates);
    std::cout << "\nAtraso Máximo da solução inicial Tempos Medios: " << atraso_maximo << "\n";

    end = std::chrono::high_resolution_clock::now();
    elapsed_seconds = end-start;
    
    arquivo << atraso_maximo << "," << elapsed_seconds.count() << ",";



    //-------------------------------------------------------//
    //                  Iterated Greedy
    std::vector<int> piBest;
    float atrasoPiBest;
    std::pair<std::vector<int>, float> sequenceTmax = iteratedGreedy(solucao, atraso_maximo, n, m, d, alpha, compTime, dueDates, processingTimes, TP);

    piBest = sequenceTmax.first;
    atrasoPiBest = sequenceTmax.second;
    
    end = std::chrono::high_resolution_clock::now();
    elapsed_seconds = end-start;
    
    arquivo << atrasoPiBest << "," << elapsed_seconds.count() << "\n";

    std::cout << "\nAtraso Máximo Iterated Greedy (IG): "<< atrasoPiBest;
    std::cout << "\nSequência de Tarefas - Solução Final IG";
    imprimeSequencia(piBest);

    //-------------------------------------------------------//
    auto stop = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
 
    std::cout << "\nTempo: " << duration.count()/1000000.0 << " segundos\n";
    //-------------------------------------------------------//
    
    arquivo.close();
    return 0;
}