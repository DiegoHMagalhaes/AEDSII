#include <stdio.h>
#include <stdlib.h>
#include "aluno.h"
#include "simulacao.h"
//Diego Henrique Marques Magalhães 22.2.8082

void menu() {
    printf("\n======= SIMULADOR DE ARMAZENAMENTO (TP1 AEDS-II) =======\n");
    printf("1. Executar Simulacao\n");
    printf("2. Imprimir Base de Alunos Ficticia (Amostra)\n");
    printf("0. Sair\n");
    printf("========================================================\n");
    printf("Escolha uma opcao: ");
}

int main() {
    int opcao;
    TAluno** base_ficticia = NULL;
    int num_registros = 0;

    do {
        menu();
        if (scanf(" %d", &opcao) != 1) {
            // Limpa o buffer de entrada se o usuário digitar algo não numérico
            while (getchar() != '\n');
            opcao = -1; // Força a opção inválida
        }

        switch (opcao) {
            case 1: {
                int tam_bloco, modo_armazenamento, modo_variavel;

                // 1. Geração dos dados
                if (!base_ficticia) {
                    printf("Primeiro, vamos gerar a base de dados ficticia.\n");
                    printf("Digite o numero total de registros a serem gerados: "); //
                    scanf(" %d", &num_registros);
                    if (num_registros <= 0) {
                        printf("Numero invalido.\n");
                        break;
                    }
                    base_ficticia = criar_base_alunos_ficticia(num_registros);
                }

                // 2. Definição dos parâmetros
                printf("Digite o tamanho maximo do bloco (em bytes): "); //
                scanf(" %d", &tam_bloco);
                if (tam_bloco <= 0) {
                    printf("Tamanho de bloco invalido.\n");
                    break;
                }

                printf("Escolha o modo de armazenamento:\n"); //
                printf("  1. Registros de tamanho fixo\n"); //
                printf("  2. Registros de tamanho variavel\n"); //
                printf("Escolha: ");
                scanf(" %d", &modo_armazenamento);

                // 3. Simulação de escrita
                FILE* arq_saida = fopen("alunos.dat", "w+b"); //
                if (!arq_saida) {
                    printf("ERRO: Nao foi possivel criar alunos.dat.\n");
                    break;
                }

                TStats stats = {0}; 

                if (modo_armazenamento == 1) {
                    simular_escrita_fixa(base_ficticia, num_registros, tam_bloco, arq_saida, &stats);
                } else if (modo_armazenamento == 2) {
                    printf("  Modo Variavel selecionado. Escolha a sub-estrategia:\n"); //
                    printf("    1. Registros contiguos (sem espalhamento)\n"); //
                    printf("    2. Registros espalhados (fragmentados)\n"); //
                    printf("  Escolha: ");
                    scanf(" %d", &modo_variavel);

                    if (modo_variavel == 1) {
                        simular_escrita_variavel_contigua(base_ficticia, num_registros, tam_bloco, arq_saida, &stats);
                    } else if (modo_variavel == 2) {
                        simular_escrita_variavel_espalhada(base_ficticia, num_registros, tam_bloco, arq_saida, &stats);
                    } else {
                        printf("Sub-estrategia invalida.\n");
                    }
                } else {
                    printf("Modo de armazenamento invalido.\n");
                }

                fclose(arq_saida);

                // 4. Exibição dos resultados
                if (stats.num_registros_gravados > 0) {
                    exibir_estatisticas(&stats); //
                }
                
                break;
            }
            case 2: {
                if (!base_ficticia) {
                    printf("A base ficticia ainda nao foi gerada. (Opcao 1)\n");
                } else {
                    printf("\n--- AMOSTRA DA BASE DE ALUNOS FICTICIA ---\n");
                    int amostra = (num_registros < 5) ? num_registros : 5;
                    
                    int i; 
                    for (i = 0; i < amostra; i++) { 
                        imprime_aluno(base_ficticia[i]);
                    }
                }
                break;
            }
            case 0:
                printf("Saindo do sistema...\n");
                break;
            default:
                printf("Opcao invalida. Tente novamente.\n");
        }
    } while (opcao != 0);

    if (base_ficticia) {
        liberar_base_alunos(base_ficticia, num_registros);
    }

    return 0;
}
