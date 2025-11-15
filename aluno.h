#ifndef ALUNO_H
#define ALUNO_H

#include <stdio.h>
//Diego Henrique Marques Magalhães 22.2.8082

typedef struct Aluno {
    int matricula;
    char nome[51]; // 50 + 1 para '\0'
    char cpf[12];  // 11 + 1 para '\0'
    char curso[31]; // 30 + 1 para '\0'
    char mae[31];   // 30 + 1 para '\0'
    char pai[31];   // 30 + 1 para '\0'
    int ano_ingresso;
    float ca;
} TAluno;
TAluno* aluno(int matricula, const char* nome, const char* cpf, const char* curso,
              const char* mae, const char* pai, int ano, float ca);

/**
 * @brief Imprime os dados de um aluno no console.
 */
void imprime_aluno(TAluno* a);

/**
 * @brief Calcula o tamanho MÁXIMO que um registro de aluno pode ocupar.
 * Usado para a estratégia de "Tamanho Fixo". 
 */
int tamanho_maximo_registro_aluno();

/**
 * @brief Calcula o tamanho REAL de um registro de aluno (baseado no strlen).
 * Usado para a estratégia de "Tamanho Variável". 
 * NOTA: Para simplificar, consideramos o tamanho das strings + 1 ('\0')
 * mais o tamanho dos campos não-string.
 */
int tamanho_real_registro_aluno(TAluno* a);

/**
 * @brief Gera um vetor de 'tam' alunos com dados fictícios. 
 * @param tam Número de registros a serem gerados.
 * @return Um vetor alocado dinamicamente contendo os TAluno*.
 */
TAluno** criar_base_alunos_ficticia(int tam);

/**
 * @brief Libera a memória alocada para o vetor de alunos.
 */
void liberar_base_alunos(TAluno** base, int tam);

#endif // ALUNO_H
