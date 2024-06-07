#!/bin/bash

input_dir="Entradas"
output_dir="Saidas"
g++ -o programa main.cpp

if [ $? -ne 0 ]; then
    echo "Erro na compilação"
    exit 1
fi

# Inicializa o array para instâncias grandes
large_instances=()
short_instances=()

# Função para executar o programa com diferentes valores de alpha e d
run_tests() {
    local input_file=$1
    local d_values=(6 8 10)
    local alphas=(0.0 0.2 0.8)

    for alpha in "${alphas[@]}"; do
        for d in "${d_values[@]}"; do        
            output_file="${output_dir}/alpha${alpha}d${d}$(basename "$input_file")"
            echo "Executando alpha=$alpha d=${d} - time=$(date +"%T")"
            ./programa "$alpha" "${d}" < "$input_file" > "$output_file"
        done
    done
}

echo "time=$(date +"%T")"

# Percorre os arquivos de entrada e os ordena
for input_file in $(ls $input_dir/*.txt | sort); do
    # Ignora os arquivos que correspondem ao padrão "instancia-50*.txt" ou "instancia-30*.txt" ou "entradaSlide.txt"
    if [[ $input_file =~ instancia-50-* ]] || [[ $input_file =~ instancia-30-* ]] || [[ $input_file =~ instancia-2-20-* ]] || [[ $input_file =~ instancia-3-20-* ]] || [[ $input_file =~ instancia-5-20-* ]] || [[ $input_file =~ instancia-10-* ]]; then
        # Adiciona o arquivo ao array de instâncias grandes
        large_instances+=("$input_file")
        continue
    fi

    short_instances+=("$input_file")
done

# Ordena o array de instâncias
sorted_short_instances=($(printf "%s\n" "${short_instances[@]}" | sort))
sorted_large_instances=($(printf "%s\n" "${large_instances[@]}" | sort))

# Executa o programa para cada arquivo no array de instâncias pequenas

#for input_file in "${sorted_short_instances[@]}"; do
#    echo "Testando arquivo pequeno: $input_file - time=$(date +"%T")"
#    run_tests "$input_file"
#done

# Executa o programa para cada arquivo no array de instâncias grandes
for input_file in "${sorted_large_instances[@]}"; do
    echo "Testando arquivo grande: $input_file - time=$(date +"%T")"
    run_tests "$input_file"
done
