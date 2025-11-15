#include "simulacao.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h> // Adicionado para printf
//Diego Henrique Marques Magalhães 22.2.8082

// Função auxiliar para preencher um bloco com um caractere (ex: '#')
void preencher_bloco(FILE* out, int bytes, char fill_char) {
    int i; 
    for (i = 0; i < bytes; i++) {
        fwrite(&fill_char, sizeof(char), 1, out);
    }
}

// Implementação da Estratégia 1: Tamanho Fixo
void simular_escrita_fixa(TAluno** base, int tam_base, int TAM_BLOCO, FILE* out, TStats* stats) {
    int i; 
    int espaco_livre_bloco = TAM_BLOCO;
    int tam_reg = tamanho_maximo_registro_aluno();

    printf("\nIniciando simulacao: REGISTROS DE TAMANHO FIXO\n");
    stats->tam_bloco = TAM_BLOCO;
    stats->num_blocos_total = 1;

    for (i = 0; i < tam_base; i++) {
        TAluno* a = base[i];

        if (tam_reg > TAM_BLOCO) {
            printf("ERRO: Registro %d (tam=%d) maior que o bloco (tam=%d). Abortando.\n", a->matricula, tam_reg, TAM_BLOCO);
            return;
        }

        if (espaco_livre_bloco < tam_reg) {
            preencher_bloco(out, espaco_livre_bloco, '#'); 
            stats->bytes_usados_total += espaco_livre_bloco;
            if (espaco_livre_bloco > 0) stats->num_blocos_parciais++;

            stats->num_blocos_total++;
            espaco_livre_bloco = TAM_BLOCO;
        }

        fwrite(a, tam_reg, 1, out);
        espaco_livre_bloco -= tam_reg;
        stats->bytes_dados_uteis += tam_reg;
        stats->num_registros_gravados++;
    }

    if (espaco_livre_bloco > 0 && espaco_livre_bloco != TAM_BLOCO) {
        preencher_bloco(out, espaco_livre_bloco, '#');
        stats->bytes_usados_total += espaco_livre_bloco;
        stats->num_blocos_parciais++;
    }
}

// Implementação da Estratégia 2a: Variável Contíguo (Sem Espalhamento)
void simular_escrita_variavel_contigua(TAluno** base, int tam_base, int TAM_BLOCO, FILE* out, TStats* stats) {
    int i;
    int espaco_livre_bloco = TAM_BLOCO;

    printf("\nIniciando simulacao: REGISTROS VARIAVEIS (CONTIGUOS)\n");
    stats->tam_bloco = TAM_BLOCO;
    stats->num_blocos_total = 1;

    for (i = 0; i < tam_base; i++) {
        TAluno* a = base[i];
        int tam_reg = tamanho_real_registro_aluno(a); 

        if (tam_reg > TAM_BLOCO) {
            printf("ERRO: Registro %d (tam=%d) maior que o bloco (tam=%d). Abortando.\n", a->matricula, tam_reg, TAM_BLOCO);
            return;
        }

        if (espaco_livre_bloco < tam_reg) {
            preencher_bloco(out, espaco_livre_bloco, '#');
            stats->bytes_usados_total += espaco_livre_bloco;
            if (espaco_livre_bloco > 0) stats->num_blocos_parciais++;

            stats->num_blocos_total++;
            espaco_livre_bloco = TAM_BLOCO;
        }
        
        fwrite(a, tam_reg, 1, out); 
        
        espaco_livre_bloco -= tam_reg;
        stats->bytes_dados_uteis += tam_reg;
        stats->num_registros_gravados++;
    }

    if (espaco_livre_bloco > 0 && espaco_livre_bloco != TAM_BLOCO) {
        preencher_bloco(out, espaco_livre_bloco, '#');
        stats->bytes_usados_total += espaco_livre_bloco;
        stats->num_blocos_parciais++;
    }
}

// Implementação da Estratégia 2b: Variável com Espalhamento
void simular_escrita_variavel_espalhada(TAluno** base, int tam_base, int TAM_BLOCO, FILE* out, TStats* stats) {
    int i;
    int espaco_livre_bloco = TAM_BLOCO;

    printf("\nIniciando simulacao: REGISTROS VARIAVEIS (ESPALHADOS)\n");
    stats->tam_bloco = TAM_BLOCO;
    stats->num_blocos_total = 1;

    for (i = 0; i < tam_base; i++) {
        TAluno* a = base[i];
        int tam_reg_total = tamanho_real_registro_aluno(a);
        int bytes_gravados = 0;
        unsigned char* registro_bytes = (unsigned char*) a;

        if (tam_reg_total > TAM_BLOCO && TAM_BLOCO < (sizeof(int) + 2)) {
             printf("ERRO: Bloco muito pequeno para espalhamento. Abortando.\n");
             return;
        }

        stats->bytes_dados_uteis += tam_reg_total;
        stats->num_registros_gravados++;

        while (bytes_gravados < tam_reg_total) {
            int bytes_a_gravar = tam_reg_total - bytes_gravados;
            
            if (bytes_a_gravar > espaco_livre_bloco) {
                bytes_a_gravar = espaco_livre_bloco;
            }

            fwrite(registro_bytes + bytes_gravados, sizeof(unsigned char), bytes_a_gravar, out);
            bytes_gravados += bytes_a_gravar;
            espaco_livre_bloco -= bytes_a_gravar;

            if (espaco_livre_bloco == 0 && bytes_gravados < tam_reg_total) {
                stats->num_blocos_total++;
                espaco_livre_bloco = TAM_BLOCO;
            }
        }
    }

    if (espaco_livre_bloco > 0 && espaco_livre_bloco != TAM_BLOCO) {
        preencher_bloco(out, espaco_livre_bloco, '#');
        stats->bytes_usados_total += espaco_livre_bloco;
        stats->num_blocos_parciais++;
    }
}


// Exibição das Estatísticas Finais
void exibir_estatisticas(TStats* stats) {
    int i;
    double total_bytes_arquivo = (double)stats->num_blocos_total * stats->tam_bloco;
    double bytes_desperdicados = total_bytes_arquivo - stats->bytes_dados_uteis;
    
    
    double pct_ocupacao_media = (stats->bytes_dados_uteis / total_bytes_arquivo) * 100.0;
    
    double eficiencia = pct_ocupacao_media;

    printf("\n======= ESTATISTICAS DA SIMULACAO =======\n");
    printf("Registros gravados: %d\n", stats->num_registros_gravados);
    printf("Tamanho do Bloco: %d bytes\n", stats->tam_bloco);
    printf("\n--- Uso dos Blocos ---\n");
    printf("1. Numero total de blocos utilizados: %d\n", stats->num_blocos_total); 
    printf("2. Numero de blocos parcialmente usados: %d\n", stats->num_blocos_parciais); 
    printf("3. Percentual medio de ocupacao: %.2f%%\n", pct_ocupacao_media); 
    
    printf("\n--- Eficiencia ---\n");
    printf("Total de bytes (Arquivo): %.0f bytes\n", total_bytes_arquivo);
    printf("Total de bytes (Dados Uteis): %.0f bytes\n", stats->bytes_dados_uteis);
    printf("Total de bytes (Desperdicio/Padding): %.0f bytes\n", bytes_desperdicados);
    printf("4. Eficiencia de armazenamento: %.2f%%\n", eficiencia); 

    printf("\n--- Mapa de Ocupacao (Simplificado) ---\n"); 
    
    for (i = 1; i <= stats->num_blocos_total; i++) {
        if (i < stats->num_blocos_total) {
            printf("Bloco %d: %d bytes (100%% cheio)\n", i, stats->tam_bloco);
        } else {
            double bytes_ultimo_bloco = stats->tam_bloco * (pct_ocupacao_media/100.0);
            if (stats->num_blocos_parciais > 0) {
                 bytes_ultimo_bloco = (stats->bytes_dados_uteis - ( (stats->num_blocos_total - 1) * stats->tam_bloco) );
            } else if (stats->num_blocos_total == 1) {
                bytes_ultimo_bloco = stats->bytes_dados_uteis;
            }
            double pct_ultimo = (bytes_ultimo_bloco / stats->tam_bloco) * 100.0;
            printf("Bloco %d: %.0f bytes (%.0f%% cheio)\n", i, bytes_ultimo_bloco, pct_ultimo);
        }
    }
    printf("===========================================\n");
}
