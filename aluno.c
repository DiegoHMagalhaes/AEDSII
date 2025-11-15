#include "aluno.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
//Diego Henrique Marques Magalhães 22.2.8082

TAluno* aluno(int matricula, const char* nome, const char* cpf, const char* curso,
              const char* mae, const char* pai, int ano, float ca) {
    TAluno* a = (TAluno*) malloc(sizeof(TAluno));
    if (a) {
        a->matricula = matricula;
        strcpy(a->nome, nome);
        strcpy(a->cpf, cpf);
        strcpy(a->curso, curso);
        strcpy(a->mae, mae);
        strcpy(a->pai, pai);
        a->ano_ingresso = ano;
        a->ca = ca;
    }
    return a;
}

void imprime_aluno(TAluno* a) {
    if (!a) return;
    printf("\n**********************************************");
    printf("\nMatricula: %d", a->matricula);
    printf("\nNome: %s", a->nome);
    printf("\nCPF: %s", a->cpf);
    printf("\nCurso: %s", a->curso);
    printf("\nMae: %s", a->mae);
    printf("\nPai: %s", a->pai);
    printf("\nAno Ingresso: %d", a->ano_ingresso);
    printf("\nCA: %.2f", a->ca);
    printf("\n**********************************************\n");
}

int tamanho_maximo_registro_aluno() {
    return sizeof(TAluno);
}

int tamanho_real_registro_aluno(TAluno* a) {
    int tam = 0;
    tam += sizeof(a->matricula);
    tam += strlen(a->nome) + 1;
    tam += strlen(a->cpf) + 1;
    tam += strlen(a->curso) + 1;
    tam += strlen(a->mae) + 1;
    tam += strlen(a->pai) + 1;
    tam += sizeof(a->ano_ingresso);
    tam += sizeof(a->ca);
    return tam;
}

TAluno** criar_base_alunos_ficticia(int tam) {
    int i; 
    
    printf("Gerando %d registros de alunos ficticios...\n", tam);
    TAluno** base = (TAluno**) malloc(sizeof(TAluno*) * tam);
    
    for (i = 0; i < tam; i++) { 
        char nome[51], curso[31], mae[31], pai[31], cpf[12];
        sprintf(nome, "Aluno Ficticio %d", i + 1);
        sprintf(curso, "Curso %d", (i % 5) + 1);
        sprintf(mae, "Mae do Aluno %d", i + 1);
        sprintf(pai, "Pai do Aluno %d", i + 1);
        sprintf(cpf, "111222333%02d", i % 99);

        base[i] = aluno(i + 1, nome, cpf, curso, mae, pai, 2020 + (i % 5), (float)(i % 100) / 10.0f);
    }
    printf("Base ficticia gerada.\n");
    return base;
}

void liberar_base_alunos(TAluno** base, int tam) {
    int i; 
    
    for (i = 0; i < tam; i++) {
        free(base[i]);
    }
    free(base);
}
