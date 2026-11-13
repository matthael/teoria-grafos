
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "hamiltoniano.h"
#include "tps.h"

/* =====================================================
   GERACAO DE GRAFOS PARA OS EXPERIMENTOS
   ===================================================== */

/*
    Gera um grafo nao direcionado.

    Primeiro cria um ciclo garantindo conexidade.
    Depois adiciona algumas arestas extras.
*/
static Grafo *gerar_grafo_experimento(int n) {

    Grafo *g = grafo_criar(n);

    if (g == NULL) {
        return NULL;
    }

    /* Cria um ciclo envolvendo todos os vertices */
    for (int i = 0; i < n; i++) {
        grafo_adicionar_aresta(g, i, (i + 1) % n);
    }

    /* Adiciona arestas extras de forma deterministica */
    for (int i = 0; i < n; i++) {

        for (int j = i + 1; j < n; j++) {

            if ((i * 3 + j * 7 + n) % 4 != 0) {
                grafo_adicionar_aresta(g, i, j);
            }
        }
    }

    return g;
}

/* =====================================================
   TESTES DE CAMINHO E CICLO HAMILTONIANO
   ===================================================== */

static void testar_hamiltoniano(void) {

    printf("\n");
    printf("========================================\n");
    printf(" TESTE CAMINHO E CICLO HAMILTONIANO\n");
    printf("========================================\n");

    Grafo *g = gerar_grafo_experimento(5);

    if (g == NULL) {
        printf("Erro ao criar o grafo.\n");
        return;
    }

    int *caminho = malloc(g->n * sizeof(int));
    int *ciclo = malloc(g->n * sizeof(int));

    if (caminho == NULL || ciclo == NULL) {
        printf("Erro ao alocar memoria.\n");

        free(caminho);
        free(ciclo);
        grafo_destruir(g);

        return;
    }

    /* Teste do caminho Hamiltoniano */
    printf("\nCaminho Hamiltoniano:\n");

    if (encontrar_caminho_hamiltoniano_bt(g, caminho)) {
        grafo_exibir_caminho(caminho, g->n);
    } else {
        printf("Nenhum caminho Hamiltoniano encontrado.\n");
    }

    /* Teste do ciclo Hamiltoniano */
    printf("\nCiclo Hamiltoniano:\n");

    if (encontrar_ciclo_hamiltoniano_bt(g, ciclo)) {

        grafo_exibir_caminho(ciclo, g->n);

        printf("Retorno a origem: %d\n", ciclo[0]);

    } else {
        printf("Nenhum ciclo Hamiltoniano encontrado.\n");
    }

    free(caminho);
    free(ciclo);

    grafo_destruir(g);
}

/* =====================================================
   EXPERIMENTO TSP
   ===================================================== */

/* Calcula o tempo em milissegundos */
static double calcular_tempo(clock_t inicio, clock_t fim) {

    return ((double)(fim - inicio) / CLOCKS_PER_SEC) * 1000.0;
}

static void executar_experimento_tsp(int n) {

    printf("\n");
    printf("----------------------------------------\n");
    printf(" EXPERIMENTO COM %d VERTICES\n", n);
    printf("----------------------------------------\n");

    Grafo *g = gerar_grafo_experimento(n);

    if (g == NULL) {
        printf("Erro ao criar o grafo.\n");
        return;
    }

    int **matriz = grafo_para_tsp(g);

    if (matriz == NULL) {
        printf("Erro ao gerar matriz de custos.\n");
        grafo_destruir(g);
        return;
    }

    clock_t inicio;
    clock_t fim;

    TSPResultado resultado;

    /* =========================================
       TSP FORCA BRUTA
       ========================================= */

    inicio = clock();

    resultado = tsp_forca_bruta(matriz, n);

    fim = clock();

    double tempo_bruta = calcular_tempo(inicio, fim);

    printf("\n[TSP - FORCA BRUTA]\n");

    if (resultado.custo >= 0) {

        printf("Custo: %d\n", resultado.custo);
        printf("Tempo: %.4f ms\n", tempo_bruta);

        tsp_exibir_resultado("Melhor rota encontrada", resultado);

    } else {
        printf("Falha ao executar o algoritmo.\n");
    }

    tsp_destruir_resultado(&resultado);

    /* =========================================
       TSP VIZINHO MAIS PROXIMO
       ========================================= */

    inicio = clock();

    resultado = tsp_vizinho_mais_proximo(matriz, n);

    fim = clock();

    double tempo_vizinho = calcular_tempo(inicio, fim);

    printf("\n[TSP - VIZINHO MAIS PROXIMO]\n");

    if (resultado.custo >= 0) {

        printf("Custo: %d\n", resultado.custo);
        printf("Tempo: %.4f ms\n", tempo_vizinho);

        tsp_exibir_resultado("Rota encontrada", resultado);

    } else {
        printf("Falha ao executar o algoritmo.\n");
    }

    tsp_destruir_resultado(&resultado);

    /* =========================================
       TSP INSERCAO DO MAIS PROXIMO
       ========================================= */

    inicio = clock();

    resultado = tsp_insercao(matriz, n);

    fim = clock();

    double tempo_insercao = calcular_tempo(inicio, fim);

    printf("\n[TSP - INSERCAO DO MAIS PROXIMO]\n");

    if (resultado.custo >= 0) {

        printf("Custo: %d\n", resultado.custo);
        printf("Tempo: %.4f ms\n", tempo_insercao);

        tsp_exibir_resultado("Rota encontrada", resultado);

    } else {
        printf("Falha ao executar o algoritmo.\n");
    }

    tsp_destruir_resultado(&resultado);

    /* =========================================
       RESUMO COMPARATIVO
       ========================================= */

    printf("\nRESUMO COMPARATIVO - %d VERTICES\n", n);

    printf("-----------------------------------------------\n");
    printf("%-25s %-12s %-12s\n",
           "Algoritmo", "Custo", "Tempo (ms)");
    printf("-----------------------------------------------\n");

    /* Executa novamente para apresentar os custos */
    TSPResultado exato = tsp_forca_bruta(matriz, n);
    TSPResultado vizinho = tsp_vizinho_mais_proximo(matriz, n);
    TSPResultado insercao = tsp_insercao(matriz, n);

    printf("%-25s %-12d %-12.4f\n",
           "Forca Bruta",
           exato.custo,
           tempo_bruta);

    printf("%-25s %-12d %-12.4f\n",
           "Vizinho Mais Proximo",
           vizinho.custo,
           tempo_vizinho);

    printf("%-25s %-12d %-12.4f\n",
           "Insercao",
           insercao.custo,
           tempo_insercao);

    printf("-----------------------------------------------\n");

    tsp_destruir_resultado(&exato);
    tsp_destruir_resultado(&vizinho);
    tsp_destruir_resultado(&insercao);

    tsp_destruir_matriz(matriz, n);
    grafo_destruir(g);
}

/* =====================================================
   FUNCAO PRINCIPAL
   ===================================================== */

int main(void) {

    printf("\n");
    printf("========================================\n");
    printf(" PRATICA 10 - CAMINHO HAMILTONIANO E TSP\n");
    printf("========================================\n");

    /* Testa caminho e ciclo Hamiltoniano */
    testar_hamiltoniano();

    /* Tamanhos exigidos pela atividade */
    int tamanhos[] = {5, 8, 10, 12};

    int quantidade = sizeof(tamanhos) / sizeof(tamanhos[0]);

    printf("\n");
    printf("========================================\n");
    printf(" EXPERIMENTOS DE TSP\n");
    printf("========================================\n");

    for (int i = 0; i < quantidade; i++) {
        executar_experimento_tsp(tamanhos[i]);
    }

    printf("\n");
    printf("========================================\n");
    printf(" FIM DOS EXPERIMENTOS\n");
    printf("========================================\n");

    return 0;
}
