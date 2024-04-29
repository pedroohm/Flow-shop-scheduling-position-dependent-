
# Diretório contendo os arquivos de entrada
input_dir="Entradas"

g++ -o programa main.cpp -g

if [ $? -ne 0 ]; then
    echo "Erro na compilação"
    exit 1
fi

for input_file in $input_dir/*.txt; do
    
    # Ignora os arquivos que correspondem ao padrão "instancia-50*.txt"
    if [[ $input_file =~ "instancia-50-"* ]] || [[ $input_file =~ "instancia-30-"* ]]; then
        echo "Pulando instancia: $input_file"
        continue
    fi
    echo "Testando arquivo: $input_file"
    ./programa < "$input_file" > ./Saidas/$(basename "$input_file")
done
