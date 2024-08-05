#!/bin/bash

# Diretórios de entrada, saída e planilhas
input_dir="Entradas"
output_dir="igaPuro/Saidas"
planilhas_dir="igaPuro/Experimentos"

# Compila o programa
g++ main.cpp -o IGApuro
if [ $? -ne 0 ]; then
    echo "Erro na compilação"
    exit 1
fi

date

# Inicializa o array para instâncias grandes e planilhas
large_instances=()
planilhas=()

# Função para executar o programa com diferentes valores de alpha e d
run_tests() {
    local input_file=$1
    local planilha=$2
    local alphas=(0.0 0.2 0.8)
    #local destructs=(10 12 14)
    local d=10

    for alpha in "${alphas[@]}"; do
        local output_file="${output_dir}/alpha${alpha}d${d}_$(basename "$input_file")_$(basename "$planilha").txt"
        echo "Inicio alpha=$alpha d=$d - time=$(date +"%T")"
        ./IGApuro "$alpha" 10 "$planilha" < "$input_file" > "$output_file"
        echo "Fim alpha=$alpha d=$d - time=$(date +"%T")"
    done
}

# Percorre os arquivos de entrada e os ordena
for input_file in $(ls $input_dir/*.txt | sort); do
    # Extrai o número de máquinas, número de tarefas, alpha e d do nome do arquivo
    if [[ $input_file =~ instancia-([0-9]+)-([0-9]+)-([0-9]+)-([0-9]+)\.txt ]]; then
        num_maquinas=${BASH_REMATCH[1]}
        num_tarefas=${BASH_REMATCH[2]}
        alpha=${BASH_REMATCH[3]}
        d=${BASH_REMATCH[4]}

        if (( num_tarefas > 10)); then
            # Adiciona o arquivo ao array de instâncias grandes
            large_instances+=("$input_file")
        fi
    else
        echo "Nome de arquivo inesperado: $input_file"
    fi
done

# Ordena o array de instâncias
sorted_large_instances=($(printf "%s\n" "${large_instances[@]}" | sort))

# Percorre os arquivos de planilhas e os ordena
for planilha in $(ls $planilhas_dir/*.csv | sort | head -n 12); do
    planilhas+=("$planilha")
done

# Executa o programa para cada combinação de instância grande e planilha
for planilha in "${planilhas[@]}"; do
    echo "Testando planilha: $planilha"
    date
    for input_file in "${sorted_large_instances[@]}"; do
        echo "Testando arquivo grande: $input_file - time=$(date +"%T")"
        run_tests "$input_file" "$planilha"
    done
    echo "Final planilha: $planilha"
done

echo "

    TESTE FINALIZADO!!!
    
    "