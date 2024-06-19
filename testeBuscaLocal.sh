

input_dir="Entradas"
output_dir="Saidas"
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

    for alpha in "${alphas[@]}"; do     
        output_file="${output_dir}/alpha${alpha}d10$(basename "$input_file")"
        echo "Inicio alpha=$alpha d=10 - time=$(date +"%T")"
        ./programa "$alpha" 10 buscalocal.csv < "$input_file" > "$output_file"
        echo "Fim alpha=$alpha d=10 - time=$(date +"%T")"
    done
}

echo "time=$(date +"%T")"

# Percorre os arquivos de entrada e os ordena
for input_file in $(ls $input_dir/*.txt | sort); do
    # Extrai o número de máquinas, número de tarefas, alpha e d do nome do arquivo
    if [[ $input_file =~ instancia-([0-9]+)-([0-9]+)-([0-9]+)-([0-9]+)\.txt ]]; then
        num_maquinas=${BASH_REMATCH[1]}
        num_tarefas=${BASH_REMATCH[2]}
        alpha=${BASH_REMATCH[3]}
        d=${BASH_REMATCH[4]}
        echo "num_maquinas=$num_maquinas num_tarefas=$num_tarefas alpha=$alpha d=$d"

        if (( num_tarefas > 10 )) && (( num_maquinas < 50 )); then
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
