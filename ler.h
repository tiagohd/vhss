#include <cstdio>
#include <cstdlib>
#include <vector>
#include <string>
#include <iostream>

#ifndef ler_H
#define ler_H


#define MAX 400
#define MAXSTR 450

//typedef enum { false, true } bool;

typedef struct no {
    std::string nome;
    int linha;
    int tam;
    int tipo;
    //struct no *prox;
} No;

typedef struct linha {
    int n;
    int tipo;
    std::vector <std::string> token;
} Linha;


extern void budasynToVhdl(char *nome);
extern void exception(int lin, char* aux);

#endif