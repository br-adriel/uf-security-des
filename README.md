# Algoritmo DES

Projeto para a terceira unidade da disciplina Tópicos Especiais em Segurança da
Informação I, do curso de Sistemas de Informação da UFRN, que implementa na
linguagem de programação C o algoritmo DES.

![C](https://img.shields.io/badge/c-%2300599C.svg?style=for-the-badge&logo=c&logoColor=white)

## Materiais utilizados para a implementação desse projeto

- [:page_facing_up: Criptografia - UFRGS](http://penta.ufrgs.br/gere96/segur2/des.htm)
- [:page_facing_up: Data Encryption Standard - Wikipedia (português)](https://pt.wikipedia.org/wiki/Data_Encryption_Standard)
- [:page_facing_up: Data Encryption Standard - Wikipedia (inglês)](https://en.wikipedia.org/wiki/Data_Encryption_Standard)
- [:page_facing_up: DES supplementary material - Wikipedia](https://en.wikipedia.org/wiki/DES_supplementary_material)
- [:film_strip: Data Encryption Standard (DES) - Explained with an Example - Cryptography - CyberSecurity - CSE4003](https://youtu.be/-j80aA8q_IQ)
- [:film_strip: Working of DES with Example](https://youtu.be/Q5p0WIa8S0o)

## Executando o projeto

Os passos a seguir utilizam o [gcc](https://pt.wikipedia.org/wiki/GNU_Compiler_Collection)
e considera que você está utilizando um ambiente linux, pesquise os comandos
equivalentes no Windows para compilar para a sua máquina, caso esse seja o seu
caso.

### Compilação

Para compilar os arquivos basta executar o comando a seguir.

Você pode utilizar outro nome no lugar de `des_alg`.

```bash
gcc main.c des.c -o des_alg
```

### Execução

```bash
./des_alg [chave] [arquivo_entrada] [arquivo_saida] [descriptografar]
```

- `[chave]`: Chave de criptografia de 64 bits, no formato hexadecimal, que você
deseja utilizar com o algoritmo
- `[arquivo_entrada]`: Caminho para o arquivo sobre o qual o algoritmo deve ser
executado
- `[arquivo_saida]`: Caminho para o arquivo de saída
- `[descriptografar]`: use `0` para criptografar e `1` para descriptografar

## Medindo o desempenho

Através da execução do script `medir_desempenho.py` é possível medir o tempo de
execução do algoritmo na criptografia e descriprografia.

O script funciona da seguinte forma:

1. Um arquivo binário chamado `arquivo_original` é preenchido com 10KB usando
interiros aleatórios, em seguida é feita a criptografia e descriptografia desse
arquivo. O tempo é dessas operações é cronometrado e adicionado ao arquivo
`tempo_criptografia.txt` e `tempo_descriptografia.txt` junto com o tamanho do
arquivo em KB.

2. Esse processo é repetido adicionando mais conteúdo ao `arquivo_original` até
que ele atinja o tamanho de 2MB.

### Alterando os parâmetros

Para personalizar o tamanho do acréscimo de bytes, ou o limite máximo de tamanho
do arquivo original, será necessário modificar o arquivo do script.

A variável `tamanho_adicao`, na linha 23, controla o tamanho do ácrescimo no
tamanho do arquivo a cada rodada.

A variável `limite_tamanho`, na linha 24, controla o tamanho máximo do arquivo.
