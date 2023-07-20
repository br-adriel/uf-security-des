from random import randint
from subprocess import run
import time


def adicionarConteudo(caminho, dados):
    with open(caminho, 'ab') as file:
        file.write(dados)


def executar_programa(n, arquivo_saida, comando):
    inicio = time.time()
    run(comando)
    fim = time.time()

    tempo = fim-inicio
    with open(arquivo_saida, 'a') as arquivo:
        arquivo.write(f"{n} {tempo:.5f}\n")


if __name__ == "__main__":
    tamanho_total = 0
    tamanho_adicao = 10 * 1024  # 10KB
    limite_tamanho = 2 * 1000 * 1024  # 2MB
    conteudo = bytes([randint(0, 255) for _ in range(tamanho_adicao)])

    for i in range(0, limite_tamanho, tamanho_adicao):
        adicionarConteudo('./arquivo_original', conteudo)
        tamanho_total += tamanho_adicao

        executar_programa(tamanho_total/1024,
                          './tempo_criptografia.txt',
                          [
                              './des_alg',
                              '01234568789abcdef',
                              './arquivo_original',
                              './arquivo_criptografado',
                              '0'
                          ])
        executar_programa(tamanho_total/1024,
                          './tempo_descriptografia.txt',
                          [
                              './des_alg',
                              '01234568789abcdef',
                              './arquivo_criptografado',
                              './arquivo_descriptografado',
                              '1'
                          ])
        print(tamanho_total/1024)
