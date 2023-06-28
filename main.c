#include <stdio.h>

unsigned long long permutar(unsigned long long entrada, int tabelaPermutacao[64])
{
  unsigned long long saida = 0;

  // percorre os 64 bits fazendo a permutação
  for (int i = 0; i < 64; i++)
  {
    // move o bit da posição tabelaPermutacao[i] para o inicio do numero e o
    // "seleciona" através de um OU com um 1 apenas na última posição
    int bitsDeslocamento = 64 - tabelaPermutacao[i];
    unsigned long long bit = (entrada >> bitsDeslocamento) & 1ULL;

    // insere o bit na saida na posicao indicada pelo indice da tabelaPermutacao
    saida |= (bit << (63 - i));
  }

  return saida;
}

unsigned long long permutacaoInicial(unsigned long long entrada)
{
  int tabelaPermutacao[64] = {
      58, 50, 42, 34, 26, 18, 10, 2,
      60, 52, 44, 36, 28, 20, 12, 4,
      62, 54, 46, 38, 30, 22, 14, 6,
      64, 56, 48, 40, 32, 24, 16, 8,
      57, 49, 41, 33, 25, 17, 9, 1,
      59, 51, 43, 35, 27, 19, 11, 3,
      61, 53, 45, 37, 29, 21, 13, 5,
      63, 55, 47, 39, 31, 23, 15, 7};
  return permutar(entrada, tabelaPermutacao);
}

unsigned long long permutacaoInversa(unsigned long long entrada)
{
  int tabelaPermutacao[64] = {
      40, 8, 48, 16, 56, 24, 64, 32,
      39, 7, 47, 15, 55, 23, 63, 31,
      38, 6, 46, 14, 54, 22, 62, 30,
      37, 5, 45, 13, 53, 21, 61, 29,
      36, 4, 44, 12, 52, 20, 60, 28,
      35, 3, 43, 11, 51, 19, 59, 27,
      34, 2, 42, 10, 50, 18, 58, 26,
      33, 1, 41, 9, 49, 17, 57, 25};
  return permutar(entrada, tabelaPermutacao);
}

int main()
{
  // testando funcao
  unsigned long long input = 0x8000000000000000; // Exemplo de entrada de 64 bits
  unsigned long long output = permutacaoInicial(input);
  unsigned long long inverso = permutacaoInversa(output);

  printf("Input:  %016llX\n", input);
  printf("Output: %016llX\n", output);
  printf("inverso: %016llX\n", inverso);

  return 0;
}