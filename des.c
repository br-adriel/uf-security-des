#include <stdio.h>
#include "des.h"

#define BLOCK_SIZE 8 // 64 bits = 8 bytes

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
 * saída com tamanhoTabela bits
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
 * Utiliza o sbox especificado para alterar a entrada de 6 para 4 bits
 */
unsigned long long usarSbox(unsigned long long entrada, int indiceSbox)
{
  unsigned int primeiroBit = (entrada >> 5) & 0b1;
  unsigned int ultimoBit = entrada & 0b1;
  unsigned int linhaSbox = (primeiroBit << 1) | ultimoBit;

  unsigned int coluna = (entrada >> 1) & 0b1111;

  return sboxes[indiceSbox][linhaSbox * 16 + coluna];
}

/**
 * Processa o bloco de 48 bits usando as caixas de substituição
 */
unsigned long long substituir(unsigned long long entrada)
{
  // Divide o número em 8 partes de 6 bits cada
  unsigned long long mascara = 0x3F; // Máscara de 6 bits (0b00111111)
  unsigned long long novoValor = 0ULL;
  unsigned long long resultado = 0ULL;

  for (int i = 0; i < 8; i++)
  {
    novoValor = usarSbox((entrada >> ((7 - i) * 6)) & mascara, i);
    resultado = (resultado << 4) | novoValor & 0xF;
  }

  return resultado;
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

/**
 * Criptografa um bloco de 64 bits
 *
 * Use decript como 0 para encriptar e como 1 para decriptar
 */
unsigned long long executarDes(unsigned long long bloco64, int decript)
{
  unsigned long long blocoPermutado = permutacaoInicial(bloco64);
  // printf("Após permutação inicial: %016llX\n", blocoPermutado);

  unsigned long long metadeDireita = blocoPermutado & 0xFFFFFFFFULL;
  unsigned long long metadeDireitaOriginal = blocoPermutado & 0xFFFFFFFFULL;
  unsigned long long metadeEsquerda = (blocoPermutado >> 32) & 0xFFFFFFFFULL;
  // printf("Após divisão: ESQ=%016llX DIR=%016llX\n\n", metadeEsquerda, metadeDireita);

  for (int round = 0; round < 16; round++)
  {
    // expande a metade direita para 48 bits
    int tabelaExpansao[48] = {
        32, 1, 2, 3, 4, 5,
        4, 5, 6, 7, 8, 9,
        8, 9, 10, 11, 12, 13,
        12, 13, 14, 15, 16, 17,
        16, 17, 18, 19, 20, 21,
        20, 21, 22, 23, 24, 25,
        24, 25, 26, 27, 28, 29,
        28, 29, 30, 31, 32, 1};
    metadeDireita = permutar(metadeDireita, tabelaExpansao, 48, 32);

    // faz ou exclusivo com a subchave do round
    if (decript == 1)
    {
      metadeDireita = metadeDireita ^ subchaves[15 - round];
    }
    else
    {
      metadeDireita = metadeDireita ^ subchaves[round];
    }

    // sbox
    metadeDireita = substituir(metadeDireita);

    // permutacao da saida do sbox
    int tabelaPermutacaoSbox[32] = {
        16, 7, 20, 21, 29, 12, 28, 17,
        1, 15, 23, 26, 5, 18, 31, 10,
        2, 8, 24, 14, 32, 27, 3, 9,
        19, 13, 30, 6, 22, 11, 4, 25};
    metadeDireita = permutar(metadeDireita, tabelaPermutacaoSbox, 32, 32);

    // ou exclusivo com metade esquerda => nova metade direita
    metadeDireita = metadeEsquerda ^ metadeDireita;

    // troca metades e repete
    metadeEsquerda = metadeDireitaOriginal | 0ULL;
    metadeDireitaOriginal = metadeDireita | 0ULL;

    // printf("Rodada %d: ESQ=%016llX DIR=%016llX\n", round + 1, metadeEsquerda, metadeDireita);
  }

  unsigned long long resultaoPrePF = metadeDireita << 32 | metadeEsquerda;
  return permutacaoInversa(resultaoPrePF);
}

/**
 * Chama a função executarDes sobre um arquivo
 *
 * Utilize decript como 0 para encriptar e como 1 decriptar
 */
int executarDesArquivo(unsigned long long chave, char *caminhoArquivoEntrada, char *caminhoArquivoSaida, int decrypt)
{
  gerarSubchaves(chave);

  FILE *arquivoEntrada;
  FILE *arquivoSaida;
  unsigned long long buffer;
  size_t elements_read;

  // Abre o arquivo de entrada no modo de leitura binária
  arquivoEntrada = fopen(caminhoArquivoEntrada, "rb");
  if (arquivoEntrada == NULL)
  {
    perror("Erro ao abrir o arquivo");
    return 1;
  }

  // Abre arquivo de saida no modo de escrita binaria
  arquivoSaida = fopen(caminhoArquivoSaida, "wb");
  if (arquivoSaida == NULL)
  {
    perror("Erro ao abrir o arquivo de saída");
    fclose(arquivoSaida);
    return 1;
  }

  // Lê o arquivo em blocos de 64 bits até o final do arquivo
  int blocos = 1;
  while ((elements_read = fread(&buffer, 1, BLOCK_SIZE, arquivoEntrada)) > 0)
  {
    // prenche blocos com menos de 64 bits na criptografia com zeros
    if (elements_read < 8 && decrypt == 0)
    {
      buffer = buffer << (8 - elements_read) * 8;
    }

    unsigned long long blocoResultante = executarDes(buffer, decrypt);

    // remove zeros adicionados na criptografia
    int bytesZero = 0;
    if (decrypt == 1)
    {
      // se ainda há o que ler, nao faz nada
      if (fread(&buffer, 1, BLOCK_SIZE, arquivoEntrada) > 0)
      {
        fseek(arquivoEntrada, -8, SEEK_CUR);
      }
      else
      {
        while ((blocoResultante & 0xFF) == 0)
        {
          blocoResultante = blocoResultante >> 8;
          bytesZero++;
        }
      }
    }
    fwrite(&blocoResultante, BLOCK_SIZE - bytesZero, 1, arquivoSaida);
  }

  // Fecha os arquivo de entrada e saida
  fclose(arquivoEntrada);
  fclose(arquivoSaida);

  return 0;
}
