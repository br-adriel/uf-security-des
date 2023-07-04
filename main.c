#include <stdio.h>

/**
 * Tabela utilizada na rotação de bits para cálculo de sub-chaves
 */
int tabelaRotacao[16] = {1, 1, 2, 2, 2, 2, 2, 2, 1, 2, 2, 2, 2, 2, 2, 1};

/**
 * Sboxes utilizados na criptografia do bloco de 64 bits
 */
unsigned int sboxes[8][64] = {
    {14, 4, 13, 1, 2, 15, 11, 8, 3, 10, 6, 12, 5, 9, 0, 7,
     0, 15, 7, 4, 14, 2, 13, 1, 10, 6, 12, 11, 9, 5, 3, 8,
     4, 1, 14, 8, 13, 6, 2, 11, 15, 12, 9, 7, 3, 10, 5, 0,
     15, 12, 8, 2, 4, 9, 1, 7, 5, 11, 3, 14, 10, 0, 6, 13},

    {15, 1, 8, 14, 6, 11, 3, 4, 9, 7, 2, 13, 12, 0, 5, 10,
     3, 13, 4, 7, 15, 2, 8, 14, 12, 0, 1, 10, 6, 9, 11, 5,
     0, 14, 7, 11, 10, 4, 13, 1, 5, 8, 12, 6, 9, 3, 2, 15,
     13, 8, 10, 1, 3, 15, 4, 2, 11, 6, 7, 12, 0, 5, 14, 9},

    {10, 0, 9, 14, 6, 3, 15, 5, 1, 13, 12, 7, 11, 4, 2, 8,
     13, 7, 0, 9, 3, 4, 6, 10, 2, 8, 5, 14, 12, 11, 15, 1,
     13, 6, 4, 9, 8, 15, 3, 0, 11, 1, 2, 12, 5, 10, 14, 7,
     1, 10, 13, 0, 6, 9, 8, 7, 4, 15, 14, 3, 11, 5, 2, 12},

    {7, 13, 14, 3, 0, 6, 9, 10, 1, 2, 8, 5, 11, 12, 4, 15,
     14, 13, 8, 11, 5, 6, 15, 0, 3, 4, 7, 2, 12, 1, 10, 9,
     10, 6, 9, 0, 12, 11, 7, 13, 15, 1, 3, 14, 5, 2, 8, 4,
     3, 15, 0, 6, 10, 1, 13, 8, 9, 4, 5, 11, 12, 7, 2, 14},

    {2, 12, 4, 1, 7, 10, 11, 6, 8, 5, 3, 15, 13, 0, 14, 9,
     14, 11, 2, 12, 4, 7, 13, 1, 5, 0, 15, 10, 3, 9, 8, 6,
     4, 2, 1, 11, 10, 13, 7, 8, 15, 9, 12, 5, 6, 3, 0, 14,
     11, 8, 12, 7, 1, 14, 2, 13, 6, 15, 0, 9, 10, 4, 5, 3},

    {12, 1, 10, 15, 9, 2, 6, 8, 0, 13, 3, 4, 14, 7, 5, 11,
     10, 15, 4, 2, 7, 12, 9, 5, 6, 1, 13, 14, 0, 11, 3, 8,
     9, 14, 15, 5, 2, 8, 12, 3, 7, 0, 4, 10, 1, 13, 11, 6,
     4, 3, 2, 12, 9, 5, 15, 10, 11, 14, 1, 7, 6, 0, 8, 13},

    {4, 11, 2, 14, 15, 0, 8, 13, 3, 12, 9, 7, 5, 10, 6, 1,
     13, 0, 11, 7, 4, 9, 1, 10, 14, 3, 5, 12, 2, 15, 8, 6,
     1, 4, 11, 13, 12, 3, 7, 14, 10, 15, 6, 8, 0, 5, 9, 2,
     6, 11, 13, 8, 1, 4, 10, 7, 9, 5, 0, 15, 14, 2, 3, 12},

    {13, 2, 8, 4, 6, 15, 11, 1, 10, 9, 3, 14, 5, 0, 12, 7,
     1, 15, 13, 8, 10, 3, 7, 4, 12, 5, 6, 11, 0, 14, 9, 2,
     7, 11, 4, 1, 9, 12, 14, 2, 0, 6, 10, 13, 15, 3, 5, 8,
     2, 1, 14, 7, 4, 10, 8, 13, 15, 12, 9, 0, 3, 5, 6, 11},
};

/**
 * Tabela para armazenar as subchaves geradas
 */
unsigned long long subchaves[16];

/**
 * Permuta a entrada de acordo com a tabelaPermutacao fornecida, retonando uma
 * saída com tamanhoPermutacao bits
 */
unsigned long long permutar(unsigned long long entrada, int tabelaPermutacao[], int tamanhoTabela, int tamanhoEntrada)
{
  unsigned long long saida = 0;

  // percorre os a tabela de permutacao fazendo a mudança
  for (int i = 0; i < tamanhoTabela; i++)
  {
    // move o bit da posição tabelaPermutacao[i] para o inicio do numero e o
    // "seleciona" através de um OU com um binario que tem 1 apenas na última
    // posição
    int bitsDeslocamento = tamanhoEntrada - tabelaPermutacao[i];
    unsigned long long bit = (entrada >> bitsDeslocamento) & 1ULL;

    // insere o bit na saida na posicao indicada pelo indice da tabelaPermutacao
    saida |= (bit << (tamanhoTabela - 1 - i));
  }
  return saida;
}

/**
 * Realiza a permutação inicial do algoritmo
 */
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
  return permutar(entrada, tabelaPermutacao, 64, 64);
}

/**
 * Realiza a permutação final do algoritmo
 */
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
  return permutar(entrada, tabelaPermutacao, 64, 64);
}

/**
 * Gera a chave de 56 bits usada para gerar as sub-chaves
 */
unsigned long long gerarChave56(unsigned long long chavePrincipal)
{
  int tabelaSubchave[56] = {
      57, 49, 41, 33, 25, 17, 9, 1,
      58, 50, 42, 34, 26, 18, 10, 2,
      59, 51, 43, 35, 27, 19, 11, 3,
      60, 52, 44, 36, 63, 55, 47, 39,
      31, 23, 15, 7, 62, 54, 46, 38,
      30, 22, 14, 6, 61, 53, 45, 37,
      29, 21, 13, 5, 28, 20, 12, 4};
  return permutar(chavePrincipal, tabelaSubchave, 56, 64);
}

/**
 * Rotaciona os bits de uma chave de 28 bits para a esquerda
 */
unsigned long rotacionarBits(unsigned long chave28, int bits)
{
  unsigned long rotacionado = (chave28 << bits) | (chave28 >> 28 - bits);
  return rotacionado & 0xFFFFFFFUL; // garante que so os 28 bits sao retornados
}

/**
 * Gera as subchaves de 48 bits a partir de uma chave de 64 bits
 */
void gerarSubchaves(unsigned long long chave64)
{
  unsigned long long chave56 = gerarChave56(chave64);

  // divide a chave em duas partes de 28 bits cada
  unsigned long metadeDireita = chave56 & 0xFFFFFFFUL;
  unsigned long metadeEsquerda = (chave56 >> 28) & 0xFFFFFFFUL;

  for (int round = 0; round < 16; round++)
  {
    int bitsDeRotacao = tabelaRotacao[round];

    // rotaciona cada metade para a esquerda pelo número de bitsDeRotacao
    metadeDireita = rotacionarBits(metadeDireita, bitsDeRotacao);
    metadeEsquerda = rotacionarBits(metadeEsquerda, bitsDeRotacao);

    // junta as duas chaves em uma só novamente
    unsigned long long subchaveTemporaria = metadeEsquerda << 28 | metadeDireita;

    // comprime a chave para 48 bits usando uma permutação
    int tabelaCompressao[48] = {
        14, 17, 11, 24, 1, 5, 3, 28,
        15, 6, 21, 10, 23, 19, 12, 4,
        26, 8, 16, 7, 27, 20, 13, 2,
        41, 52, 31, 37, 47, 55, 30, 40,
        51, 45, 33, 48, 44, 49, 39, 56,
        34, 53, 46, 42, 50, 36, 29, 32};
    unsigned long long subchaveComprimida = permutar(subchaveTemporaria, tabelaCompressao, 48, 56);
    subchaves[round] = subchaveComprimida;
  }
}

int main()
{
  unsigned long long chave = 0x133457799bbcdff1;
  gerarSubchaves(chave);
  for (int i = 0; i < 16; i++)
  {
    printf("%016llX\n", subchaves[i]);
  }
  return 0;
}