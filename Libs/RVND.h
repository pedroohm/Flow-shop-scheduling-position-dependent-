void RVND(int n, int m, float &atraso_maximo, std::vector<int> &sequenciaTarefas, std::vector<std::vector<std::vector<float>>> &TP, 
std::vector<float> &temposAtraso, std::vector<int> &dueDates, std::vector<std::vector<int>> &processingTimes, float alpha){

    int vizinhancas[2];
    vizinhancas[0] = rand() % 2;
    if(vizinhancas[0]==0) vizinhancas[1] = 1;
    else vizinhancas[1] = 0;
    
    int vizinhanca_atual = 0;

    while(vizinhanca_atual<=1){

        std::vector<int> solucao_vizinha(n);
        std::vector<int> melhor_vizinho(n);
        std::vector<float> temposAtrasoVizinho(n, 0);

        float atraso_maximo_vizinho = 0.0;
        float menor_atraso = atraso_maximo;
        melhor_vizinho = sequenciaTarefas;
        
        std::vector<std::vector<int>> C(n);
        for(int i=0; i<n; i++) C[i].resize(m);

        if(vizinhancas[vizinhanca_atual]==0){
            
            for(int i1=0; i1<n; i1++){
                for(int i2=i1+1; i2<n; i2++){

                    solucao_vizinha = sequenciaTarefas;
                    int tarefa = sequenciaTarefas[i1];

                    std::swap(solucao_vizinha[i1], solucao_vizinha[i2]);

                    for(int _k=0; _k<n; _k++){
                        for(int _i=0; _i<m; _i++){

                            if(_k==0 && _i==0){
                                C[solucao_vizinha[_k]][_i] = TP[solucao_vizinha[_k]][_i][_k];
                            }
                            else if(_k==0){
                                C[solucao_vizinha[_k]][_i] = TP[solucao_vizinha[_k]][_i-1][_k] + TP[solucao_vizinha[_k]][_i][_k];
                            }
                            else if(_i==0){
                                C[solucao_vizinha[_k]][_i] = C[solucao_vizinha[_k-1]][_i] + TP[solucao_vizinha[_k]][_i][_k];
                            }
                            else{
                                C[solucao_vizinha[_k]][_i] = std::max((C[solucao_vizinha[_k-1]][_i]+TP[solucao_vizinha[_k]][_i][_k]), (C[solucao_vizinha[_k]][_i-1]+TP[solucao_vizinha[_k]][_i][_k]));
                            }

                        }
                    }

                    atraso_maximo_vizinho = std::max(0, (C[solucao_vizinha[0]][m-1]-dueDates[0]));
                    for(int _k=0; _k<n; _k++){

                        if((C[solucao_vizinha[_k]][m-1]-dueDates[_k]) > 0) temposAtrasoVizinho[_k] = C[solucao_vizinha[_k]][m-1]-dueDates[_k];
                        else temposAtrasoVizinho[_k] = 0.0;

                        if(temposAtrasoVizinho[_k] > atraso_maximo_vizinho) atraso_maximo_vizinho = temposAtrasoVizinho[_k];
                    }

                    if(atraso_maximo_vizinho < menor_atraso){
                        menor_atraso = atraso_maximo_vizinho;
                        melhor_vizinho = solucao_vizinha;
                    }
                }
            }
        }
        else if(vizinhancas[vizinhanca_atual]==1){
            
            for(int i=0; i<n; i++){
                for(int k=0; k<n; k++){

                    if(i==k || k==i-1) continue;

                    solucao_vizinha = sequenciaTarefas;
                    int tarefa = sequenciaTarefas[i];

                    std::vector<int>::iterator it(solucao_vizinha.begin()+i);
                    solucao_vizinha.erase(it);

                    it = solucao_vizinha.begin() + k;
                    solucao_vizinha.insert(it, tarefa);

                    for(int _k=0; _k<n; _k++){
                        for(int _i=0; _i<m; _i++){

                            if(_k==0 && _i==0){
                                C[solucao_vizinha[_k]][_i] = TP[solucao_vizinha[_k]][_i][_k];
                            }
                            else if(_k==0){
                                C[solucao_vizinha[_k]][_i] = TP[solucao_vizinha[_k]][_i-1][_k] + TP[solucao_vizinha[_k]][_i][_k];
                            }
                            else if(_i==0){
                                C[solucao_vizinha[_k]][_i] = C[solucao_vizinha[_k-1]][_i] + TP[solucao_vizinha[_k]][_i][_k];
                            }
                            else{
                                C[solucao_vizinha[_k]][_i] = std::max((C[solucao_vizinha[_k-1]][_i]+TP[solucao_vizinha[_k]][_i][_k]), (C[solucao_vizinha[_k]][_i-1]+TP[solucao_vizinha[_k]][_i][_k]));
                            }

                        }
                    }

                    atraso_maximo_vizinho = std::max(0, (C[solucao_vizinha[0]][m-1]-dueDates[0]));
                    for(int _k=0; _k<n; _k++){

                        if((C[solucao_vizinha[_k]][m-1]-dueDates[_k]) > 0) temposAtrasoVizinho[_k] = C[solucao_vizinha[_k]][m-1]-dueDates[_k];
                        else temposAtrasoVizinho[_k] = 0.0;

                        if(temposAtrasoVizinho[_k] > atraso_maximo_vizinho) atraso_maximo_vizinho = temposAtrasoVizinho[_k];
                    }

                    if(atraso_maximo_vizinho < menor_atraso){
                        menor_atraso = atraso_maximo_vizinho;
                        melhor_vizinho = solucao_vizinha;
                    }

                }
            }
        }

        //Caso seja encontrado um vizinho melhor que a solução atual
        if(menor_atraso < atraso_maximo){
            sequenciaTarefas = melhor_vizinho;
            atraso_maximo = menor_atraso;

            //Sorteia-se novamente as vizinhanças
            vizinhancas[0] = rand() % 2;
            if(vizinhancas[0]==0) vizinhancas[1] = 1;
            else vizinhancas[1] = 0;

            vizinhanca_atual = 0;
        }
        else vizinhanca_atual++;

        // vizinhanca_atual++;
    }

}