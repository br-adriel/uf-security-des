#include <stdio.h>
#include <stdlib.h>
#include "des.h"

int main(int argc, char *argv[])
{
  if (argc < 5)
  {
    printf("Uso: des-alg chave arquivo_entrada arquivo_saida decriptografar\n");
    return 1;
  }

  unsigned long long chave = strtoull(argv[1], NULL, 0);
  char *caminhoArquivoEntrada = argv[2];
  char *caminhoArquivoSaida = argv[3];
  int long long decriptografar = atoi(argv[4]);

  executarDesArquivo(chave, caminhoArquivoEntrada, caminhoArquivoSaida, decriptografar);

  return 0;
}
