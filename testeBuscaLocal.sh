input_dir="Entradas"
output_dir="Saidas"
planilhas_dir="experiemntos"

g++ main.cpp -o programa

if [ $? -ne 0 ]; then
    echo "Erro na compilação"
    exit 1
fi

# Inicializa o array para instâncias grandes
large_instances=()

# Função para executar o programa com diferentes valores de alpha e d
run_tests() {
    local input_file=$1
    local alphas=(0.0 0.2 0.8)
    local destructs=(6 8)

    for alpha in "${alphas[@]}"; do
        for d in "${destructs[@]}"; do
            local output_file="${output_dir}/alpha${alpha}d${d}$(basename "$input_file").txt"
            echo "Inicio alpha=$alpha d=$d - time=$(date +"%T")"
            ./programa "$alpha" "$d" teste.csv < "$input_file" > "$output_file"
            echo "Fim alpha=$alpha d=$d - time=$(date +"%T")"
        done
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

# Executa o programa para cada arquivo no array de instâncias grandes
for input_file in "${sorted_large_instances[@]}"; do
    echo "Testando arquivo grande: $input_file - time=$(date +"%T")"
    run_tests "$input_file"
done
