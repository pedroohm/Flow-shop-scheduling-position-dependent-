input_dir="Entradas"
g++ -o programa main.cpp

if [ $? -ne 0 ]; then
    echo "Erro na compilação"
    exit 1
fi

# Inicializa o array para instâncias grandes
large_instances=()

echo "time=$(date +"%T")"
# Percorre os arquivos de entrada
for input_file in $(ls $input_dir/*.txt | sort); do    
    # Ignora os arquivos que correspondem ao padrão "instancia-50*.txt" ou "instancia-30*.txt" ou "entradaSlide.txt"
    if [[ $input_file =~ "instancia-50-"* ]] || [[ $input_file =~ "instancia-30-"* ]] || [[ $input_file =~ "entradaSlide.txt"* ]]; then
        # Adiciona o arquivo ao array de instâncias grandes
        large_instances+=("$input_file")
        continue
    fi    
    # Executa o programa para cada instância pequena
    echo "Testando arquivo: $input_file - time=$(date +"%T")"
    ./programa 0.0 0.2 < "$input_file" > "./Saidas/alpha0d10$(basename "$input_file")"
    echo "Executou alpha 0.0 - time=$(date +"%T")"
    ./programa 0.2 0.2 < "$input_file" > "./Saidas/alpha02d10$(basename "$input_file")"
    echo "Executou alpha 0.2 - time=$(date +"%T")"
    ./programa 0.8 0.2 < "$input_file" > "./Saidas/alpha08d10$(basename "$input_file")"
    echo "Executou alpha 0.8 - time=$(date +"%T")"
done

# Ordena o array de instâncias grandes
sorted_large_instances=($(printf "%s\n" "${large_instances[@]}" | sort))

# Executa o programa para cada arquivo no array
for input_file in "${sorted_large_instances[@]}"; do
    echo "Testando arquivo: $input_file - time=$(date +"%T")"
    echo "Iniciando testes com d=5%"
    ./programa 0.0 0.05 < "$input_file" > "./Saidas/alpha0d5$(basename "$input_file")"
    echo "Executou alpha=0.0 d=5% - time=$(date +"%T")"
    ./programa 0.2 0.05 < "$input_file" > "./Saidas/alpha02d5$(basename "$input_file")"
    echo "Executou alpha=0.2 d=5% - time=$(date +"%T")"
    ./programa 0.8 0.05 < "$input_file" > "./Saidas/alpha08d5$(basename "$input_file")"
    echo "Executou alpha=0.8 d=5%- time=$(date +"%T")"

    echo "Iniciando testes com d=10%"
    ./programa 0.0 0.1 < "$input_file" > "./Saidas/alpha0d10$(basename "$input_file")"
    echo "Executou alpha=0.0 d=10% - time=$(date +"%T")"
    ./programa 0.2 0.1 < "$input_file" > "./Saidas/alpha02d10$(basename "$input_file")"
    echo "Executou alpha=0.2 d=10% - time=$(date +"%T")"
    ./programa 0.8 0.1 < "$input_file" > "./Saidas/alpha08d10$(basename "$input_file")"
    echo "Executou alpha=0.8 d=10%- time=$(date +"%T")"

    echo "Iniciando testes com d=15%"
    ./programa 0.0 0.15 < "$input_file" > "./Saidas/alpha0d15$(basename "$input_file")"
    echo "Executou alpha=0.0 d=15% - time=$(date +"%T")"
    ./programa 0.2 0.15 < "$input_file" > "./Saidas/alpha02d15$(basename "$input_file")"
    echo "Executou alpha=0.2 d=15% - time=$(date +"%T")"
    ./programa 0.8 0.15 < "$input_file" > "./Saidas/alpha08d15$(basename "$input_file")"
    echo "Executou alpha=0.8 d=15%- time=$(date +"%T")"
done
