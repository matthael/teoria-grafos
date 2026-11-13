
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include "tps.h"

/* =====================================================
   REDUCAO DE GRAFO PARA TSP
   ===================================================== */

int **grafo_para_tsp(const Grafo *g) {
    if (g == NULL || g->n <= 0) {
        return NULL;
    }

    int n = g->n;

    int **matriz = malloc(n * sizeof(int *));

    if (matriz == NULL) {
        return NULL;
    }

    for (int i = 0; i < n; i++) {

        matriz[i] = malloc(n * sizeof(int));

        if (matriz[i] == NULL) {
            for (int j = 0; j < i; j++) {
                free(matriz[j]);
            }

            free(matriz);
            return NULL;
        }

        for (int j = 0; j < n; j++) {

            if (i == j) {
                matriz[i][j] = 0;
            }
            else if (grafo_tem_aresta(g, i, j)) {
                matriz[i][j] = 1;
            }
            else {
                matriz[i][j] = 2;
            }
        }
    }

    return matriz;
}

/* Libera matriz de custos */
void tsp_destruir_matriz(int **matriz, int n) {
    if (matriz == NULL) {
        return;
    }

    for (int i = 0; i < n; i++) {
        free(matriz[i]);
    }

    free(matriz);
}

/* =====================================================
   FUNCOES AUXILIARES
   ===================================================== */

/* Inicializa uma solucao */
static TSPResultado tsp_resultado_criar(int n) {
    TSPResultado resultado;

    resultado.custo = -1;
    resultado.tamanho = 0;
    resultado.ciclo = NULL;

    if (n < 2) {
        return resultado;
    }

    resultado.ciclo = malloc((n + 1) * sizeof(int));

    if (resultado.ciclo == NULL) {
        return resultado;
    }

    resultado.tamanho = n + 1;

    return resultado;
}

/* Libera uma solucao */
void tsp_destruir_resultado(TSPResultado *resultado) {
    if (resultado == NULL) {
        return;
    }

    free(resultado->ciclo);

    resultado->ciclo = NULL;
    resultado->custo = -1;
    resultado->tamanho = 0;
}

/* Calcula o custo de uma rota */
static int calcular_custo(int **matriz, int *rota, int n) {
    int custo = 0;

    for (int i = 0; i < n; i++) {
        custo += matriz[rota[i]][rota[i + 1]];
    }

    return custo;
}

/* Exibe uma solucao */
void tsp_exibir_resultado(const char *nome, TSPResultado resultado) {
    printf("\n%s\n", nome);

    if (resultado.ciclo == NULL || resultado.custo < 0) {
        printf("Nao foi possivel encontrar uma solucao.\n");
        return;
    }

    printf("Rota: ");

    for (int i = 0; i < resultado.tamanho; i++) {
        printf("%d", resultado.ciclo[i]);

        if (i < resultado.tamanho - 1) {
            printf(" -> ");
        }
    }

    printf("\nCusto total: %d\n", resultado.custo);
}

/* =====================================================
   TSP POR FORCA BRUTA
   ===================================================== */

/*
    Fixa o vertice 0 como origem.

    Gera todas as permutacoes dos demais vertices,
    calculando o custo de cada ciclo Hamiltoniano.
*/

static void tsp_permutar(
    int **matriz,
    int n,
    int *rota,
    int *visitado,
    int posicao,
    int custo_atual,
    int *melhor_custo,
    int *melhor_rota
) {
    /* Todos os vertices foram visitados */
    if (posicao == n) {

        /* Fecha o ciclo retornando a origem */
        int custo_total = custo_atual + matriz[rota[n - 1]][0];

        if (custo_total < *melhor_custo) {

            *melhor_custo = custo_total;

            for (int i = 0; i < n; i++) {
                melhor_rota[i] = rota[i];
            }

            melhor_rota[n] = 0;
        }

        return;
    }

    /* Testa os vertices ainda nao visitados */
    for (int v = 1; v < n; v++) {

        if (!visitado[v]) {

            int novo_custo = custo_atual +
                matriz[rota[posicao - 1]][v];

            /*
                Poda: se o custo parcial ja e maior
                ou igual ao melhor custo, descarta.
            */
            if (novo_custo >= *melhor_custo) {
                continue;
            }

            rota[posicao] = v;
            visitado[v] = 1;

            tsp_permutar(
                matriz,
                n,
                rota,
                visitado,
                posicao + 1,
                novo_custo,
                melhor_custo,
                melhor_rota
            );

            /* Backtracking */
            visitado[v] = 0;
        }
    }
}

/* Funcao publica do TSP por forca bruta */
TSPResultado tsp_forca_bruta(int **matriz, int n) {

    TSPResultado resultado = tsp_resultado_criar(n);

    if (matriz == NULL || n < 2 ||
        resultado.ciclo == NULL) {
        return resultado;
    }

    int *rota = malloc(n * sizeof(int));
    int *visitado = calloc(n, sizeof(int));

    if (rota == NULL || visitado == NULL) {
        free(rota);
        free(visitado);
        tsp_destruir_resultado(&resultado);
        return resultado;
    }

    int melhor_custo = INT_MAX;

    int *melhor_rota = resultado.ciclo;

    /* Origem fixa */
    rota[0] = 0;
    visitado[0] = 1;

    tsp_permutar(
        matriz,
        n,
        rota,
        visitado,
        1,
        0,
        &melhor_custo,
        melhor_rota
    );

    if (melhor_custo != INT_MAX) {
        resultado.custo = melhor_custo;
    }

    free(rota);
    free(visitado);

    return resultado;
}

/* =====================================================
   TSP - VIZINHO MAIS PROXIMO
   ===================================================== */

/*
    Comeca no vertice 0.

    A cada etapa, escolhe o vertice nao visitado
    que possui o menor custo a partir do atual.
*/

TSPResultado tsp_vizinho_mais_proximo(int **matriz, int n) {

    TSPResultado resultado = tsp_resultado_criar(n);

    if (matriz == NULL || n < 2 ||
        resultado.ciclo == NULL) {
        return resultado;
    }

    int *visitado = calloc(n, sizeof(int));

    if (visitado == NULL) {
        tsp_destruir_resultado(&resultado);
        return resultado;
    }

    int atual = 0;
    int custo_total = 0;

    resultado.ciclo[0] = 0;
    visitado[0] = 1;

    /* Escolhe os demais vertices */
    for (int posicao = 1; posicao < n; posicao++) {

        int proximo = -1;
        int menor_custo = INT_MAX;

        for (int v = 0; v < n; v++) {

            if (!visitado[v] &&
                matriz[atual][v] < menor_custo) {

                menor_custo = matriz[atual][v];
                proximo = v;
            }
        }

        if (proximo == -1) {
            free(visitado);
            tsp_destruir_resultado(&resultado);
            return resultado;
        }

        resultado.ciclo[posicao] = proximo;

        visitado[proximo] = 1;

        custo_total += matriz[atual][proximo];

        atual = proximo;
    }

    /* Retorna ao vertice inicial */
    custo_total += matriz[atual][0];

    resultado.ciclo[n] = 0;
    resultado.custo = custo_total;

    free(visitado);

    return resultado;
}

/* =====================================================
   TSP - INSERCAO DO MAIS PROXIMO
   ===================================================== */

/*
    1. Comeca com o vertice 0 e seu vizinho mais proximo.
    2. Escolhe o vertice externo mais proximo da rota.
    3. Insere esse vertice na posicao que gera
       o menor aumento de custo.
    4. Repete ate incluir todos os vertices.
*/

TSPResultado tsp_insercao(int **matriz, int n) {

    TSPResultado resultado = tsp_resultado_criar(n);

    if (matriz == NULL || n < 2 ||
        resultado.ciclo == NULL) {
        return resultado;
    }

    int *visitado = calloc(n, sizeof(int));

    if (visitado == NULL) {
        tsp_destruir_resultado(&resultado);
        return resultado;
    }

    int *rota = resultado.ciclo;

    /* Inicializa a rota com o vertice 0 */
    rota[0] = 0;
    rota[1] = 0;

    visitado[0] = 1;

    int tamanho_rota = 1;

    /* Encontra o vizinho mais proximo da origem */
    int primeiro = -1;
    int menor_custo = INT_MAX;

    for (int v = 1; v < n; v++) {

        if (matriz[0][v] < menor_custo) {
            menor_custo = matriz[0][v];
            primeiro = v;
        }
    }

    if (primeiro == -1) {
        free(visitado);
        tsp_destruir_resultado(&resultado);
        return resultado;
    }

    rota[1] = primeiro;
    rota[2] = 0;

    visitado[primeiro] = 1;
    tamanho_rota = 2;

    /* Insere os vertices restantes */
    while (tamanho_rota < n) {

        int escolhido = -1;
        int menor_distancia = INT_MAX;

        /*
            Escolhe o vertice externo mais proximo
            de algum vertice que ja esta na rota.
        */
        for (int v = 0; v < n; v++) {

            if (visitado[v]) {
                continue;
            }

            for (int i = 0; i < tamanho_rota; i++) {

                int custo = matriz[rota[i]][v];

                if (custo < menor_distancia) {
                    menor_distancia = custo;
                    escolhido = v;
                }
            }
        }

        if (escolhido == -1) {
            free(visitado);
            tsp_destruir_resultado(&resultado);
            return resultado;
        }

        /* Procura a melhor posicao para insercao */
        int melhor_posicao = -1;
        int menor_aumento = INT_MAX;

        for (int i = 0; i < tamanho_rota; i++) {

            int origem = rota[i];
            int destino = rota[i + 1];

            int aumento =
                matriz[origem][escolhido] +
                matriz[escolhido][destino] -
                matriz[origem][destino];

            if (aumento < menor_aumento) {
                menor_aumento = aumento;
                melhor_posicao = i + 1;
            }
        }

        /* Abre espaco na rota */
        for (int i = tamanho_rota + 1;
             i > melhor_posicao;
             i--) {

            rota[i] = rota[i - 1];
        }

        rota[melhor_posicao] = escolhido;

        visitado[escolhido] = 1;

        tamanho_rota++;
    }

    /* Fecha o ciclo */
    rota[n] = 0;

    resultado.custo = calcular_custo(matriz, rota, n);

    free(visitado);

    return resultado;
}
