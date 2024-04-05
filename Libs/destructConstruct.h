void destructConstruct(int n, int m, float &atraso_maximo, std::vector<int> &sequenciaTarefas, std::vector<int> &solucao_pi1, 
float &atraso_maximo_pi1, int d, std::vector<std::vector<std::vector<float>>> TP, std::vector<int> dueDates){

    int retirados[d];
    std::vector<int>::iterator it;

    for(int l=0; l<d; l++){

        int excluir = rand() % solucao_pi1.size();

        retirados[l] = solucao_pi1[excluir];

        it = solucao_pi1.begin()+excluir;
        solucao_pi1.erase(it);
    }

    for(int i=0; i<d; i++){

        std::vector<int> vizinho(solucao_pi1.size());
        float atraso = atraso_maximo_pi1;

        for(int k=0; k<solucao_pi1.size(); k++){

            // vizinho = solucao_pi1;

            int tarefa = retirados[i];

            it = vizinho.begin() + k;
            vizinho.insert(it, tarefa);

            std::vector<std::vector<int>> C(vizinho.size());
            for(int i=0; i<n; i++) C[i].resize(m);

            for(int _k=0; _k<vizinho.size(); _k++){
                for(int _i=0; _i<m; _i++){

                    if(_k==0 && _i==0){
                        C[vizinho[_k]][_i] = TP[vizinho[_k]][_i][_k];
                    }
                    else if(_k==0){
                        C[vizinho[_k]][_i] = TP[vizinho[_k]][_i-1][_k] + TP[vizinho[_k]][_i][_k];
                    }
                    else if(_i==0){
                        C[vizinho[_k]][_i] = C[vizinho[_k-1]][_i] + TP[vizinho[_k]][_i][_k];
                    }
                    else{
                        C[vizinho[_k]][_i] = std::max((C[vizinho[_k-1]][_i]+TP[vizinho[_k]][_i][_k]), (C[vizinho[_k]][_i-1]+TP[vizinho[_k]][_i][_k]));
                    }
                }
            }

            atraso = std::max(0, (C[vizinho[0]][m-1]-dueDates[0]));
            float aux;
            for(int _k=0; _k<vizinho.size(); _k++){
                if((C[vizinho[_k]][m-1]-dueDates[_k]) > 0) aux = (C[vizinho[_k]][m-1]-dueDates[_k]);
                else aux = 0.0;

                if(aux > atraso_maximo_pi1) atraso_maximo_pi1 = aux;
            }

            if(atraso_maximo_pi1 > atraso || k==0){
                atraso_maximo_pi1 = atraso;
                solucao_pi1 = vizinho;
            }

        }
    }

}