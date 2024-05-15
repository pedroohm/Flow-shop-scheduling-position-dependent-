input_dir="Entradas"

g++ -o programa main.cpp -g

if [ $? -ne 0 ]; then
    echo "Erro na compilação"
    exit 1
fi

for input_file in $input_dir/*.txt; do
    
    # Ignora os arquivos que correspondem ao padrão "instancia-50*.txt" ou "instancia-30*.txt" ou "entradaSlide.txt"
    if [[ $input_file =~ "instancia-50-"* ]] || [[ $input_file =~ "instancia-30-"* ]] || [[ $input_file =~ "entradaSlide.txt"* ]]; then
        echo "Pulando instancia: $input_file"
        continue
    fi
    echo "Testando arquivo: $input_file"
    #./programa 0.1 < "$input_file" > ./Saidas/d01$(basename "$input_file")
    #./programa 0.2 < "$input_file" > ./Saidas/d02$(basename "$input_file")
    ./programa 0.3 < "$input_file" > ./Saidas/d03$(basename "$input_file")
    #./programa 0.4 < "$input_file" > ./Saidas/d04$(basename "$input_file")
    #./programa 0.5 < "$input_file" > ./Saidas/d05$(basename "$input_file")
done
