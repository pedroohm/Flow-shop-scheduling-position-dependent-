
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
    int tarefa, indice;
    for(int k=1; k<=n; k++){
        double newAtraso = compTime[k][m] - dueDates[sequenciaTarefas[k-1]];
        if (newAtraso > atraso)
            atraso = newAtraso;
            tarefa = sequenciaTarefas[k-1];
            indice = k-1;
    }
    //std::cout << "\nMaior atraso da tarefa " << tarefa << "pois " << compTime[indice][m] << "-" << dueDates[sequenciaTarefas[indice]] << "\n";
    return atraso;
}

void imprimeCompletionTime(int n, int m, std::vector<std::vector<double>> compTime){
    std::cout << "\n Ultima linha da completion time:\n";
    for(int k=0; k<n; k++){         
        std::cout << compTime[k+1][m] << " ";
    }
}

std::vector<std::pair<double, int>> ordemTarefas(const int n, const int m, std::vector<int> &dueDates, std::vector<std::vector<int>> &processingTimes, const int param){
    std::vector<std::pair<double, int>> temposMedios(n, std::pair<double, int>(0.0f, 0)); 
    if (param == 1){
        //crescente EDD+PT
        for(int j=0; j<n; j++){
            double soma = 0;
            for(int i=0; i<m; i++){
                soma += processingTimes[i][j];
            }
            double p_medio = soma / m;
            temposMedios[j] = std::make_pair((dueDates[j] / p_medio), j);       
        }
        std::sort(temposMedios.begin(), temposMedios.end());
        return temposMedios;
    } else if (param == 2){
        //decrescente LPT
        for(int j=0; j<n; j++){
            double soma = 0;
            for(int i=0; i<m; i++){
                soma += processingTimes[i][j];
            }
            double p_medio = soma / m;
            temposMedios[j] = std::make_pair(p_medio, j);       
        }
        std::sort(temposMedios.begin(), temposMedios.end(), [](std::pair<double, int> a, std::pair<double, int> b) { return a.first > b.first; }); // Ordena decrescente
        return temposMedios;
        
    } else{
        //EDD -> crescente
        for(int j=0; j<n; j++)
            temposMedios[j] = std::make_pair(dueDates[j], j);
        std::sort(temposMedios.begin(), temposMedios.end());
        return temposMedios;

    }
}

std::pair<std::vector<int>, double> destructConstruct(std::vector<int> &sequencia, std::vector<std::vector<double>> &completionTime, int d, int n, int m, std::vector<int> dueDates, const std::vector<std::vector<int>> &processingTimes, const std::vector<std::vector<std::vector<double>>> &TP, double alpha){
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
    double menorAtraso;
    double atrasoAux;
    for(int i=0; i<deletados.size(); i++){
        menorAtraso = INT_MAX;
        pos = 0;
        std::vector<int> seqParcial;
        for(int j=0; j<sequencia.size(); j++){
            seqParcial = sequencia;
            seqParcial.insert(seqParcial.begin() + j, deletados[i]);
            completionTime.resize(seqParcial.size()+1, std::vector<double>(m+1, 0));
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

// se MODO = 1, a busca local utilizará o primeiro melhor vizinho. Se MODO = 0, a busca local utilizará o melhor vizinho
std::pair<std::vector<int>, double> generateNeighbors(const std::vector<int> &sequencia, double melhorAtraso, int n, int m, std::vector<std::vector<double>> &completionTime, const std::vector<int> &dueDates, const std::vector<std::vector<int>> &processingTimes, const std::vector<std::vector<std::vector<double>>> &TP, int MODO){
    std::vector<int> vizinho(n);
    std::vector<int> melhorVizinho(n, 0); // Inicialize com uma sequência inválida
    double melhorAtrasoVizinho = melhorAtraso;
    int distancia;
    double atraso;

   distancia = n/4;

    //std::cout << "\nProcurando vizinhos com melhor atraso que: " << melhorAtraso << " \n";
    int cont = 0; // conta o numero de vizinhos gerados

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
                atraso = atrasoMaximo(n, m, vizinho, completionTime, dueDates);
                
                //std::cout << "\nAtraso do Vizinho ";
                //imprimeSequencia(vizinho);
                //std::cout << "Vizinho n° " <<  cont++ << " -> Atraso:" << atraso << "\n";

                if (atraso < melhorAtrasoVizinho){
                    melhorVizinho = vizinho;
                    melhorAtrasoVizinho = atraso;

                    //MODO == 1 -> retorna o primeiro vizinho que melhora a solução
                    if (MODO == 1){
                        return std::make_pair(melhorVizinho, melhorAtrasoVizinho);
                    }
                }                
            }            
        }
    }

    //std::cout << "\nSaindo do gerador de vizinhos,  " 
    return std::make_pair(melhorVizinho, melhorAtrasoVizinho);
}

// se MODO = 1, a busca local utilizará o primeiro melhor vizinho
std::pair<std::vector<int>, double> buscaLocal(std::vector<int> &sequencia, double melhorAtraso, int n, int m, std::vector<std::vector<double>> &completionTime, std::vector<int> dueDates, const std::vector<std::vector<int>> &processingTimes, const std::vector<std::vector<std::vector<double>>> &TP, int MODO = 0){
    if (MODO == 0) std::cout << "Busca Local com o melhor vizinho.";    else std::cout << "Iniciando Busca Local com o primeiro melhor vizinho.";
    //std::cout << "\nIniciando Busca Local. Atraso: " << melhorAtraso;

    bool melhora = true;
    //std::cout << "\nEntrando na Busca Local com atraso = " << melhorAtraso;
    int i=1;
    while(melhora && i<4){
        melhora = false;
        std::pair<std::vector<int>, double> sequenciaTmax;
        
        sequenciaTmax = generateNeighbors(sequencia, melhorAtraso, n, m, completionTime, dueDates, processingTimes, TP, MODO);
        //std::cout << i++ << ", ";

        if(sequenciaTmax.second < melhorAtraso){
            melhorAtraso = sequenciaTmax.second;
            sequencia = sequenciaTmax.first;
            melhora = true;
            //std::cout << "\nMelhor vizinho: " << melhorAtraso << "\n";
        }
    }
    std::cout << "\nEncerrando a Busca Local. Atraso: " << melhorAtraso;
    return std::make_pair(sequencia, melhorAtraso);
}

std::pair<std::vector<int>, double> iteratedGreedy(std::vector<int> initialSolution, double atrasoInicialSolution, int n, int m, int d, double alpha, std::vector<std::vector<double>> &completionTime, std::vector<int> dueDates, const std::vector<std::vector<int>> &processingTimes, const std::vector<std::vector<std::vector<double>>> &TP, int modoLS){
    std::vector<int> pi0(initialSolution);
    double atrasoPi0 = atrasoInicialSolution;

    //std::pair<std::vector<int>, double> sequenceTmaxPi = buscaLocal(pi0, atrasoInicialSolution, n, m, completionTime, dueDates, processingTimes, TP);
    //std::vector<int> pi = sequenceTmaxPi.first;
    //double atrasoPi = sequenceTmaxPi.second;

    std::vector<int> pi = pi0;
    double atrasoPi = atrasoPi0;

    std::cout << "\nITERATED GREEDY (com busca local).\nIteração - Atraso\n";
    int i = 0;
    int j=0;
    int qtdBuscas=0;
    while (i<50){

        i++;
        j++;

        std::cout << i << " - Atraso: " << atrasoPi << "\n";

        //chamada de constructDestruct removendo d das tarefas
        std::pair<std::vector<int>, double> sequenceTmaxPi1 = destructConstruct(pi, completionTime, d, n, m, dueDates, processingTimes, TP, alpha);
        std::vector<int> pi1(sequenceTmaxPi1.first);
        double atrasoPi1 = sequenceTmaxPi1.second;
    
        if (atrasoPi1 < atrasoPi){
            pi = pi1;
            atrasoPi = atrasoPi1;


            if (j>=20 && j%2){
                std::pair<std::vector<int>, double> sequenceTmaxPi2 = buscaLocal(pi, atrasoPi, n, m, completionTime, dueDates, processingTimes, TP, modoLS);
                std::vector<int> pi2(sequenceTmaxPi2.first);
                double atrasoPi2 = sequenceTmaxPi2.second;
                
                qtdBuscas++;

                if (atrasoPi2 < atrasoPi){
                    pi = pi2;
                    atrasoPi = atrasoPi2;
                }
            }

            i = 0;                     
        }
    }  
    std::cout << "Foram feitas " << qtdBuscas << "\n";
    return std::make_pair(pi, atrasoPi);
}


std::pair<std::vector<int>, double> iteratedGreedyWhitoutLocalSearch(std::vector<int> initialSolution, double atrasoInicialSolution, int n, int m, int d, double alpha, std::vector<std::vector<double>> &completionTime, std::vector<int> dueDates, const std::vector<std::vector<int>> &processingTimes, const std::vector<std::vector<std::vector<double>>> &TP){
    std::vector<int> pi0(initialSolution);
    double atrasoPi0 = atrasoInicialSolution;

    std::vector<int> pi = pi0;
    double atrasoPi = atrasoPi0;

    std::cout << "\nITERATED GREEDY (sem busca local).\nIteração - Atraso\n";
    int i = 0;
    // IGA sem busca local parando após 50 iterações sem melhoria
    while (i<50){
        i++;
        std::cout << i << " - Atraso: " << atrasoPi << "\n";

        //chamada de constructDestruct removendo d das tarefas
        std::pair<std::vector<int>, double> sequenceTmaxPi1 = destructConstruct(pi, completionTime, d, n, m, dueDates, processingTimes, TP, alpha);
        std::vector<int> pi1(sequenceTmaxPi1.first);
        double atrasoPi1 = sequenceTmaxPi1.second;
        
        if (atrasoPi1 < atrasoPi){
            pi = pi1;
            atrasoPi = atrasoPi1;
            i = 0;         
        }
    }
    return std::make_pair(pi, atrasoPi);
}

int main(int argc, char *argv[]){
    if (argc < 4) {
        std::cerr << "Insira alpha <double>, a quantidade de tarefas a serem removidas em destructConstruct <int> e o nome do arquivo que serão salvos os testes <string>" << std::endl;
        return 1;
    }

    std::string nomeArquivo = argv[3];

    std::ofstream arquivo(nomeArquivo, std::ios::out | std::ios::app);
    if (!arquivo.is_open()) {
        std::cerr << "Erro ao abrir o arquivo!" << std::endl;
        return 1;
    }

    int m; //Número de workers ou máquinas
    int n; //Número de jobs
    double T; //Dita se as datas de entrega serão maiores ou menores
    double R; //Índice de depreciação
    double alpha= std::stod(argv[1]); //Penalidade por atrasos
    int d = std::stoi(argv[2]); // Quantidade de tarefas a serem removidas destructConstruct

    auto init = std::chrono::high_resolution_clock::now();

    std::cin>>m>>n>>T>>R;

    // se existir mais de 10 máquinas, o tempo será em minutos, como no artigo basE
    int AJUSTE; // Flag para indicar se o tempo será em minutos ou segundos
    if (m > 10) AJUSTE = 60;
    else AJUSTE = 1;

    arquivo << "\n" << alpha << "," << d << "," << m << "," << n << "," << T << "," << R << ",";
    
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
    //Criação da matriz completion time, usada para calcular o atraso
    std::vector<std::vector<double>> compTime(n+1, std::vector<double>(m+1, 0));

    std::cout << "\nCalculando TP";
    calculaTP(n, m, processingTimes, TP, alpha);
    std::cout << "\nTP calculada";

    //------------------EDD-------------------------//
    //  Definindo uma solução inicial (pi0) para o problema EDD
    //Calculando a média de todos os tempos das tarefas em todas as máquinas
    std::vector<std::pair<double, int>> heuristicaConstrutiva(n, std::pair<double, int>(0.0f, 0));
    
    auto start = std::chrono::high_resolution_clock::now();

    //P == 0 -> Ordem crescente de dueDates (EDD), P == 1 -> Ordenado pela média de dueDates, P == 2 -> Ordenado pela média de processingTimes
    // Utilizando EDD para construir a solução inicial
    heuristicaConstrutiva = ordemTarefas(n, m, dueDates, processingTimes, 0);
    
    // Armazenando a sequência de tarefas
    for(int j=0; j<n; j++) {
        solucao[j] = heuristicaConstrutiva[j].second;
        //std::cout << heuristicaConstrutiva[j].first << " - " << heuristicaConstrutiva[j].second << "\n";
    }
     
    //Calculo do completion time para a solução inicial
    calculaCompletionTime(n, m, solucao, TP, compTime);
    std::cout<<"\nSequência de Tarefas - Solução Inicial EDD";
    //imprimeSequencia(solucao);

    //Calculo do atraso máximo para a solução inicial
    double atraso_maximo = atrasoMaximo(n, m, solucao, compTime, dueDates);

    auto end = std::chrono::high_resolution_clock::now();
    auto elapsed_seconds = std::chrono::duration_cast<std::chrono::seconds>(end - start);

    arquivo << atraso_maximo << "," << elapsed_seconds.count() << ",";

    std::cout << "\nAtraso Máximo da solução inicial EDD: " << atraso_maximo;
    std::cout << "\nCPU Time duration: " << elapsed_seconds.count();

    std::vector<int> solucaoEDD(n);
    solucaoEDD = solucao;

    //-----------------MÉDIAS LPT-------------------//  
    start = std::chrono::high_resolution_clock::now();
    //P == 0 -> Ordem crescente de dueDates EDD, P == 1 -> Ordenado pela média de dueDates
    heuristicaConstrutiva = ordemTarefas(n, m, dueDates, processingTimes, 1);
    
    // Armazenando a sequência de tarefas
    for(int j=0; j<n; j++) {
        solucao[j] = heuristicaConstrutiva[j].second;
        //std::cout << heuristicaConstrutiva[j].first << " - " << heuristicaConstrutiva[j].second << "\n";
    }

    calculaCompletionTime(n, m, solucao, TP, compTime);
    //Calculo do atraso máximo para a solução inicial
    atraso_maximo = atrasoMaximo(n, m, solucao, compTime, dueDates);

    end = std::chrono::high_resolution_clock::now();
    elapsed_seconds = std::chrono::duration_cast<std::chrono::seconds>(end - start);
    
    arquivo << atraso_maximo << "," << elapsed_seconds.count() << ",";

    std::cout << "\nAtraso Máximo da solução inicial Tempos Medios: " << atraso_maximo;
    std::cout << "\nCPU Time duration: " << elapsed_seconds.count();

    std::vector<int> solucaoPLT(n);
    solucaoPLT = solucao;

    arquivo << "\n";

    //-----------------EDD + PLT-------------------//  
    start = std::chrono::high_resolution_clock::now();
    //P == 0 -> Ordem crescente de dueDates EDD, P == 1 -> Ordenado pela média de dueDates, P == 2 -> EDD+PLT
    heuristicaConstrutiva = ordemTarefas(n, m, dueDates, processingTimes, 2);
    
    // Armazenando a sequência de tarefas
    for(int j=0; j<n; j++) {
        solucao[j] = heuristicaConstrutiva[j].second;
        //std::cout << heuristicaConstrutiva[j].first << " - " << heuristicaConstrutiva[j].second << "\n";
    }

    calculaCompletionTime(n, m, solucao, TP, compTime);
    //Calculo do atraso máximo para a solução inicial
    atraso_maximo = atrasoMaximo(n, m, solucao, compTime, dueDates);

    end = std::chrono::high_resolution_clock::now();
    elapsed_seconds = std::chrono::duration_cast<std::chrono::seconds>(end - start);
    
    arquivo << atraso_maximo << "," << elapsed_seconds.count() << ",";

    std::cout << "\nAtraso Máximo da solução inicial Tempos Medios: " << atraso_maximo;
    std::cout << "\nCPU Time duration: " << elapsed_seconds.count();

    std::vector<int> solucaoEDDPLT(n);
    solucaoEDDPLT = solucao;

    arquivo << "\n";

    /*
    // ---------------------------------------------------- //

    if (alpha == 0.0) solucao = solucaoEDD;
    else solucao = solucaoMedias;

    std::pair<std::vector<int>, double> sequenceTmax;
    
    
    
    //----------------Iterated Greedy Algorithm Without Local Search-------------------//            
    start = std::chrono::high_resolution_clock::now(); // reinicia a contagem para o IGA
    calculaCompletionTime(n, m, solucao, TP, compTime);
    std::vector<int> piBest;
    double atrasoPiBest;
    sequenceTmax = iteratedGreedyWhitoutLocalSearch(solucao, atraso_maximo, n, m, d, alpha, compTime, dueDates, processingTimes, TP);

    piBest = sequenceTmax.first;
    atrasoPiBest = sequenceTmax.second;
    
    end = std::chrono::high_resolution_clock::now();
    elapsed_seconds = std::chrono::duration_cast<std::chrono::seconds>(end - start);
    
    arquivo << atrasoPiBest << "," << elapsed_seconds.count() << ",";
    
    
    std::cout << "\nAtraso Máximo Iterated Greedy sem Busca Loca (IG): "<< atrasoPiBest;
    std::cout << "\nCPU Time duration: " << elapsed_seconds.count();
    std::cout << "\nSequência de Tarefas - Solução Final IGA sem Busca Local";
    imprimeSequencia(piBest);
    // -------------------------------------------------------//

    
    //------------Local Search with 1° Best on IGA----------------//            
    start = std::chrono::high_resolution_clock::now(); // reinicia a contagem para o IGA-LS1

    // MODO = 0 -> busca local com o melhor vizinho, MODO = 1 -> busca local com o primeiro melhor vizinho
    sequenceTmax = buscaLocal(piBest, atrasoPiBest, n, m, compTime, dueDates, processingTimes, TP, 1);

    //piBest = sequenceTmax.first;
    //atrasoPiBest = sequenceTmax.second;
    
    end = std::chrono::high_resolution_clock::now();
    elapsed_seconds = std::chrono::duration_cast<std::chrono::seconds>(end - start);
    
    arquivo << sequenceTmax.second << "," << elapsed_seconds.count(); << ",";
 

    std::cout << "\nAtraso Máximo Iterated Greedy with Local Search 1 (IGA - LS1): "<< sequenceTmax.second;
    std::cout << "\nSequência de Tarefas - Solução Final IGA Local Search 1";
    imprimeSequencia(sequenceTmax.first);
    // -------------------------------------------------------//
    
    //------------IGA+ Busca Local utilizando todas as vizinhancas (pegando o melhor vizinho)----------------//            
    start = std::chrono::high_resolution_clock::now(); // reinicia a contagem para o IGA
    calculaCompletionTime(n, m, piBest, TP, compTime);
    sequenceTmax = buscaLocal(piBest, atrasoPiBest, n, m, compTime, dueDates, processingTimes, TP, 1);

    //piBest = sequenceTmax.first;
    //atrasoPiBest = sequenceTmax.second;
    
    end = std::chrono::high_resolution_clock::now();
    elapsed_seconds = std::chrono::duration_cast<std::chrono::seconds>(end - start);

    
    arquivo << sequenceTmax.second << "," << elapsed_seconds.count() << ",";


    std::cout << "\nAtraso Máximo  Busca Local Melhor Vizinho: "<< sequenceTmax.second;
    std::cout << "\nSequência de Tarefas - Solução Final IGA+LS complete:";
    imprimeSequencia(sequenceTmax.first);
    // -------------------------------------------------------//

    
    

    //------------IGA completo com melhor vizinho ----------------// 
    std::cout << "\n\n\nIterated Greedy Algorithm:\n\n";           
    start = std::chrono::high_resolution_clock::now(); // reinicia a contagem para o IGA
    calculaCompletionTime(n, m, solucao, TP, compTime);
    sequenceTmax = iteratedGreedy(solucao, atraso_maximo, n, m, d, alpha, compTime, dueDates, processingTimes, TP, 0); // MODO = 1 -> busca local com o primeiro melhor vizinho, MODO = 0 -> busca local completa limitada a 4 iterações
    //piBest = sequenceTmax.first;
    //atrasoPiBest = sequenceTmax.second;
    
    end = std::chrono::high_resolution_clock::now(); //finaliza a contagem
    elapsed_seconds = std::chrono::duration_cast<std::chrono::seconds>(end - start);
    
    arquivo << sequenceTmax.second << "," << elapsed_seconds.count() << ",";

    std::cout << "\nAtraso Máximo IGA completo: "<< sequenceTmax.second;
    std::cout << "\nCPU Time duration: " << elapsed_seconds.count();
    std::cout << "\nSequência de Tarefas - Solução Final IGA completo:";
    imprimeSequencia(sequenceTmax.first);
    // -------------------------------------------------------//
    
    */

    arquivo.close();
    return 0;
}