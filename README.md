Indexed Pattern Matching Tool (IPMT) é uma ferramenta de criação de índices com compressão e busca de padrões  que implementa conhecidos algoritmos da área. O IPMT foi construído para a disciplina de Processamento de Cadeias de Caracteres, ministrada no Centro de Informática da UFPE no semestre 2014.2.

O IPMT foi construído por Danilo Neves (dnr2) e Vítor Antero (vham).

##Compilação

Para facilitar o processo de compilação, foram criados scripts para Windows e sistemas Unix.
Para executá-los, o usuário precisa ter instalado o compilador `g++` com a versão que suporte c++11.

###Windows

O script de compilação para Windows foi criado e testado no Windows 8.1.
Para compilar o programa, basta rodar o arquivo compile.bat a partir da pasta `src/`.
O executável `ipmt.exe` estará na pasta `bin/`.

###Sistemas Unix

O script de compilação para sistemas Unix foi criado e testado no Linux Ubuntu 12.04.
A pasta `src/` contém um arquivo `Makefile`, bastando executar o comando `make` a partir dela para compilar o programa.
O executável `ipmt` estará na pasta `bin/`.
