# Project - LP2 / EP2 - UFPB

## Description

This project aims to create a unisex bathroom using the stick technique in c ++.

The criteria are:

- The bathroom has more than one space;
- People of the same gender can not coexist in the bathroom;
- There is a limit of consecutive uses for the same gender;
- There is a limit of uses of the bathroom.

## Compilation/Linker

To compile/linker execute `make` in the clone directory.

Example:

```bash
user@host:~/lp2_ep2$ make
[ -d OBJ ] || mkdir OBJ
g++ -c BanheiroUnissex.cpp -std=c++11 -lpthread -o OBJ/BanheiroUnissex.o
g++ OBJ/BanheiroUnissex.o -o AbreBanheiroUnissex -std=c++11 -lpthread
rm -rf OBJ
user@host:~/lp2_ep2$
```

## Execution

Then, execute the generated binary:

```bash
user@host:~/lp2_ep2$ ./AbreBanheiroUnissex --help

Uso: ./AbreBanheiroUnissex [OPÇÕES]

OPÇÕES:
-p | --pessoas        Número de pessoas
-m | --maxutilizacao  Número máximo de utilizações do Banheiro
-c | --capacidade     Capacidade máxima de pessoas no Banheiro
-u | --consecutivos    Número máximo de utilizações consecutivas por gênero

OBS.: Todos os parâmetros acima são obrigatórios e podem ser
      chamados em qualquer ordem.

-v | --version  Versão do programa
-h | --help     Ajuda (esta tela)

Exemplo de uso:

  user@host:~dir_projeto$ ./AbreBanheiroUnissex -p 10 -m 20 -c 2 -u 3
```

