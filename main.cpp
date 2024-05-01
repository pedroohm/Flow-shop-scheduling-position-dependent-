
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
    std::cout<<"\n";
}

void imprimeCompTime(const std::vector<std::vector<double>> &compTime){
    std::cout << "\nCompletion Time\n";
    for(int k=1; k<=compTime.size(); k++){
        for(int i=1; i<=compTime[0].size(); i++){
            std::cout << compTime[k][i] << " ";
        }
        std::cout << "\n";
    }
    std::cout << "\n";
}

void calculaTP(const int n, const int m, std::vector<std::vector<int>> &processingTimes, std::vector<std::vector<std::vector<double>>> &TP, double alpha){
    for(int j=0; j<n; j++){
        for(int i=0; i<m; i++){
            for(int k=0; k<n; k++){
                TP[j][i][k] = processingTimes[i][j] * (std::pow((k + 1.0), alpha));
            }
        }
    }
}

void calculaCompletionTime(const int n, const int m, const std::vector<int> &sequenciaTarefas, const std::vector<std::vector<std::vector<double>>> &TP, std::vector<std::vector<double>> &compTime){
    compTime.resize(n + 1, std::vector<double>(m + 1));
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

double atrasoMaximo(const int n, const int m, const std::vector<int> &sequenciaTarefas, const std::vector<std::vector<double>> &compTime, const std::vector<int> &dueDates){
    double atraso = 0;
    for(int k=0; k<n; k++){
        double newAtraso = compTime[k+1][m] - dueDates[sequenciaTarefas[k]];
        if (newAtraso > atraso)
            atraso = newAtraso;
    }
    return atraso;
}

std::vector<std::pair<double, int>> ordemTarefas(const int n, const int m, std::vector<int> &dueDates, std::vector<std::vector<int>> &processingTimes, const int param){
    std::vector<std::pair<double, int>> temposMedios(n, std::pair<double, int>(0.0f, 0)); 
    if (param ==1){
        for(int j=0; j<n; j++){
            double soma = 0;
            for(int i=0; i<m; i++){
                soma += processingTimes[i][j];
            }
            double p_medio = soma / m;
            temposMedios[j] = std::make_pair((dueDates[j] / p_medio), j);       
        }
    } else {
        for(int j=0; j<n; j++)
            temposMedios[j] = std::make_pair(dueDates[j], j);
    }

    std::sort(temposMedios.begin(), temposMedios.end());
    return temposMedios;
}

std::pair<std::vector<int>, double> destructConstruct(std::vector<int> &sequencia, std::vector<std::vector<double>> &completionTime, int d, int n, int m, std::vector<int> dueDates, const std::vector<std::vector<int>> &processingTimes, const std::vector<std::vector<std::vector<double>>> &TP, double alpha){
    std::vector<int> deletados;

    //utilizar 
    //int n= sequencia.size();

    // Processo de destruição
    std::random_device rd; //verificar a complexidade desaa operacao
    std::uniform_int_distribution<int> dist(0, sequencia.size()-1);
    int pos = dist(rd); // define qual posicao será excluida
    while (d>0){        
        deletados.push_back(sequencia[pos]); // armazena o elemento que será excluído
        sequencia.erase(sequencia.begin() + pos);
        d--;
        //n--;
        std::uniform_int_distribution<int> dist(0, sequencia.size()-1);
        pos = dist(rd); // qual a proxima posicao a excluir
    }

    // Processo de construção
    double menorAtraso;
    for(int i=0; i<deletados.size(); i++){
        menorAtraso = INT_MAX;
        int pos = 0;
        std::vector<int> seqParcial;
        for(int j=0; j<sequencia.size(); j++){
            seqParcial = sequencia;
            seqParcial.insert(seqParcial.begin() + j, deletados[i]);
            completionTime.resize(seqParcial.size()+1, std::vector<double>(m+1, 0));
            calculaCompletionTime(seqParcial.size(), m, seqParcial, TP, completionTime);
                    
            double atrasoAux = atrasoMaximo(seqParcial.size(), m, seqParcial, completionTime, dueDates);
            if(atrasoAux < menorAtraso){
                menorAtraso = atrasoAux;
                pos = j;
            }
        }
        
        sequencia.insert(sequencia.begin() + pos, deletados[i]);
    }
    
    //retornar um par sendo a sequencia e o menorAtraso
    return std::make_pair(sequencia, menorAtraso);
}

std::pair<std::vector<int>, double> generateNeighbors(std::vector<int> &sequencia, double melhorAtraso, int n, int m, std::vector<std::vector<double>> &completionTime, std::vector<int> dueDates, const std::vector<std::vector<int>> &processingTimes, const std::vector<std::vector<std::vector<double>>> &TP){
    std::vector<int> vizinho(n);
    std::vector<int> melhorVizinho(n);
    
    int distancia = n/2;

    for(int job = 0; job < n; job++){
        for(int pos = 0; pos < n; pos++){
            // && std::abs(job-pos)<distancia 
            if (job != pos && pos != (job-1) && std::abs(job-pos)<distancia){
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
                } //fim while

                calculaCompletionTime(n, m, vizinho, TP, completionTime);
                double atraso = atrasoMaximo(n, m, vizinho, completionTime, dueDates);

                if (atraso < melhorAtraso){
                    melhorVizinho = vizinho;
                    melhorAtraso = atraso;
                }                
            }
            
        }
    }
    //return bestNeighbor and its Tmax
    return std::make_pair(melhorVizinho, melhorAtraso);
}

std::pair<std::vector<int>, double> buscaLocal(std::vector<int> &sequencia, double melhorAtraso, int n, int m, std::vector<std::vector<double>> &completionTime, std::vector<int> dueDates, const std::vector<std::vector<int>> &processingTimes, const std::vector<std::vector<std::vector<double>>> &TP){
    bool melhora = true;

    while(melhora){
        melhora = false;
        std::pair<std::vector<int>, double> sequenciaTmax = generateNeighbors(sequencia, melhorAtraso, n, m, completionTime, dueDates, processingTimes, TP);
        if(sequenciaTmax.second < melhorAtraso){
            melhorAtraso = sequenciaTmax.second;
            sequencia = sequenciaTmax.first;
            melhora = true;
        }
    }

    return std::make_pair(sequencia, melhorAtraso);
}

int main(int argc, char *argv[]){
    if (argc < 2) {
        std::cerr << "Insira a porcentagem de tarefas a serem removidas em destructConstruct <valor_double>" << std::endl;
        return 1;
    }
    double porcentagem = std::stod(argv[1]);

    std::ofstream arquivo("resultados.csv", std::ios::out | std::ios::app);
    if (!arquivo.is_open()) {
        std::cerr << "Erro ao abrir o arquivo!" << std::endl;
        return 1;
    }


    int m; //Número de workers ou máquinas
    int n; //Número de jobs
    double T; //Dita se as datas de entrega serão maiores ou menores
    double R; //Índice de depreciação
    double alpha=0.8; //Penalidade por atrasos


    std::cin>>m>>n>>T>>R;

    int d = n * porcentagem; //Porcentagem de tarefas a serem removidas

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
    std::vector<std::vector<std::vector<double>>> TP(n, std::vector<std::vector<double>>(m, std::vector<double>(n)));
    calculaTP(n, m, processingTimes, TP, alpha);

    auto start = std::chrono::high_resolution_clock::now();

    //-------------------------------------------------------//
    //  Definindo uma solução inicial (pi0) para o problema
    //Calculando a média de todos os tempos das tarefas em todas as máquinas
    std::vector<std::pair<double, int>> temposMedios(n, std::pair<double, int>(0.0f, 0));
    
    //P == 0 -> Ordem crescente de dueDates, P == 1 -> Ordenado pela média de dueDates
    // Utilizando EDD para construir a solução inicial
    temposMedios = ordemTarefas(n, m, dueDates, processingTimes, 0);
    
    // Armazenando a sequência de tarefas
    for(int j=0; j<n; j++) solucao[j] = temposMedios[j].second;    
     
    //Calculo do completion time para a solução inicial
    std::vector<std::vector<double>> compTime(n+1, std::vector<double>(m+1, 0));
    calculaCompletionTime(n, m, solucao, TP, compTime);
    std::cout<<"\nSequência de Tarefas - Solução Inicial EDD";
    imprimeSequencia(solucao);
    //Calculo do atraso máximo para a solução inicial
    double atraso_maximo = atrasoMaximo(n, m, solucao, compTime, dueDates);
    std::cout << "\nAtraso Máximo da solução inicial EDD: " << atraso_maximo << "\n";

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed_seconds = end-start;
    
    arquivo << atraso_maximo << "," << elapsed_seconds.count() << ",";
    start = std::chrono::high_resolution_clock::now();

    // Utilizando heuristica para construir a solucao inicial
    temposMedios = ordemTarefas(n, m, dueDates, processingTimes, 1);
    for(int j=0; j<n; j++) solucao[j] = temposMedios[j].second;
    calculaCompletionTime(n, m, solucao, TP, compTime);
    std::cout<<"\nSequência de Tarefas - Solução Inicial Heuristica";
    imprimeSequencia(solucao);
    atraso_maximo = atrasoMaximo(n, m, solucao, compTime, dueDates);
    std::cout << "\nAtraso Máximo da solução inicial heurística: " << atraso_maximo << "\n";

    end = std::chrono::high_resolution_clock::now();
    elapsed_seconds = end-start;
    arquivo << atraso_maximo << "," << elapsed_seconds.count() << ",";
    start = std::chrono::high_resolution_clock::now();
    //-------------------------------------------------------//
    //                  Iterated Greedy
    std::vector<int> pi0(solucao);
    std::pair<std::vector<int>, double> sequenceTmaxPi = buscaLocal(pi0, atraso_maximo, n, m, compTime, dueDates, processingTimes, TP);
    std::vector<int> pi = sequenceTmaxPi.first;
    double atrasoPi = sequenceTmaxPi.second;
    
    std::vector<int> piBest(pi);
    double atrasoPiBest = atrasoPi;

    int i = 0;
    while (i<50){
        i++;
        
        //chamada de constructDestruct removendo d% das tarefas
        std::pair<std::vector<int>, double> sequenceTmaxPi1 = destructConstruct(pi, compTime, d, n, m, dueDates, processingTimes, TP, alpha);
        std::vector<int> pi1(sequenceTmaxPi1.first);
        double atrasoPi1 = sequenceTmaxPi1.second;

        std::pair<std::vector<int>, double> sequenceTmaxPi2 = buscaLocal(pi1, atrasoPi1, n, m, compTime, dueDates, processingTimes, TP);
        std::vector<int> pi2(sequenceTmaxPi2.first);
        double atrasoPi2 = sequenceTmaxPi2.second;
        atrasoPi = atrasoMaximo(n, m, pi, compTime, dueDates);
        if (atrasoPi2 < atrasoPi){
            pi = pi2;
            if (atrasoPi < atrasoPiBest){
                piBest = pi;
                atrasoPiBest = atrasoPi;
                i = 0;
            }            
        }
    }
    
    end = std::chrono::high_resolution_clock::now();
    elapsed_seconds = end-start;
    arquivo << atrasoPiBest << "," << elapsed_seconds.count() << "\n";

    std::cout<< "\nAtraso Máximo Iterated Greedy (IG): "<< atrasoPiBest;
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