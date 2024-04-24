#!/bin/bash

# Diretório contendo os arquivos de entrada
input_dir="Entradas"

# Compilando o programa
g++ -o programa main.cpp -g

# Verificando se a compilação teve sucesso
if [ $? -ne 0 ]; then
    echo "Erro na compilação"
    exit 1
fi

# Loop pelos arquivos de entrada
for input_file in $input_dir/*.txt; do
    echo "Testando arquivo: $input_file"
    valgrind --leak-check=full ./programa < "$input_file" >./teste/$(basename "$input_file")
done