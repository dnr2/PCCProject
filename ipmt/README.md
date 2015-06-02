Indexed Pattern Matching Tool (IPMT) � uma ferramenta de cria��o de �ndices com compress�o e busca de padr�es  que implementa conhecidos algoritmos da �rea. O IPMT foi constru�do para a disciplina de Processamento de Cadeias de Caracteres, ministrada no Centro de Inform�tica da UFPE no semestre 2014.2.

O IPMT foi constru�do por Danilo Neves (dnr2) e V�tor Antero (vham).

##Compila��o

Para facilitar o processo de compila��o, foram criados scripts para Windows e sistemas Unix.
Para execut�-los, o usu�rio precisa ter instalado o compilador `g++` com a vers�o que suporte c++11.

###Windows

O script de compila��o para Windows foi criado e testado no Windows 8.1.
Para compilar o programa, basta rodar o arquivo compile.bat a partir da pasta `src/`.
O execut�vel `ipmt.exe` estar� na pasta `bin/`.

###Sistemas Unix

O script de compila��o para sistemas Unix foi criado e testado no Linux Ubuntu 12.04.
A pasta `src/` cont�m um arquivo `Makefile`, bastando executar o comando `make` a partir dela para compilar o programa.
O execut�vel `ipmt` estar� na pasta `bin/`.
