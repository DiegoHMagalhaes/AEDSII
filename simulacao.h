#ifndef SIMULACAO_H
#define SIMULACAO_H

#include "aluno.h"
#include <stdio.h>
//Diego Henrique Marques Magalhães 22.2.8082

// Estrutura para armazenar as estatísticas de saída 
typedef struct Stats {
    int num_blocos_total;
    int num_blocos_parciais;
    double bytes_usados_total;
    double bytes_dados_uteis;
    int num_registros_gravados;
    int tam_bloco;
} TStats;

/**
 * @brief Simula a escrita usando Registros de Tamanho Fixo. 
 */
void simular_escrita_fixa(TAluno** base, int tam_base, int TAM_BLOCO, FILE* out, TStats* stats);

/**
 * @brief Simula a escrita usando Registros de Tamanho Variável (Contiguos). [cite: 37, 40]
 */
void simular_escrita_variavel_contigua(TAluno** base, int tam_base, int TAM_BLOCO, FILE* out, TStats* stats);

/**
 * @brief Simula a escrita usando Registros de Tamanho Variável (Espalhados). [cite: 37, 41]
 */
void simular_escrita_variavel_espalhada(TAluno** base, int tam_base, int TAM_BLOCO, FILE* out, TStats* stats);

/**
 * @brief Exibe o relatório final de estatísticas. [cite: 72, 73, 74]
 */
void exibir_estatisticas(TStats* stats);

#endif // SIMULACAO_H
