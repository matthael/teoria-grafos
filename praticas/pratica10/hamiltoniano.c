
#include <stdio.h>
#include <stdlib.h>
#include "hamiltoniano.h"

/* Cria um grafo com n vertices */
Grafo *grafo_criar(int n) {
    if (n <= 0) {
        return NULL;
    }

    Grafo *g = malloc(sizeof(Grafo));

    if (g == NULL) {
        return NULL;
    }

    g->n = n;
    g->matriz = malloc(n * sizeof(int *));

    if (g->matriz == NULL) {
        free(g);
        return NULL;
    }

    for (int i = 0; i < n; i++) {
        g->matriz[i] = calloc(n, sizeof(int));

        if (g->matriz[i] == NULL) {
            for (int j = 0; j < i; j++) {
                free(g->matriz[j]);
            }

            free(g->matriz);
            free(g);
            return NULL;
        }
    }

    return g;
}

/* Libera a memoria do grafo */
void grafo_destruir(Grafo *g) {
    if (g == NULL) {
        return;
    }

    for (int i = 0; i < g->n; i++) {
        free(g->matriz[i]);
    }

    free(g->matriz);
    free(g);
}

/* Adiciona uma aresta nao direcionada */
void grafo_adicionar_aresta(Grafo *g, int origem, int destino) {
    if (g == NULL) {
        return;
    }

    if (origem < 0 || origem >= g->n ||
        destino < 0 || destino >= g->n) {
        return;
    }

    g->matriz[origem][destino] = 1;
    g->matriz[destino][origem] = 1;
}

/* Verifica se existe uma aresta */
int grafo_tem_aresta(const Grafo *g, int origem, int destino) {
    if (g == NULL) {
        return 0;
    }

    if (origem < 0 || origem >= g->n ||
        destino < 0 || destino >= g->n) {
        return 0;
    }

    return g->matriz[origem][destino] == 1;
}

/* Verifica se um vertice pode ser adicionado ao caminho */
static int vertice_valido(
    const Grafo *g,
    int vertice,
    int *visitado,
    int anterior
) {
    return !visitado[vertice] &&
           grafo_tem_aresta(g, anterior, vertice);
}

/* Backtracking para encontrar caminho Hamiltoniano */
static int caminho_bt(
    const Grafo *g,
    int *caminho,
    int *visitado,
    int posicao
) {
    /* Todos os vertices foram visitados */
    if (posicao == g->n) {
        return 1;
    }

    int anterior = caminho[posicao - 1];

    for (int v = 0; v < g->n; v++) {

        if (vertice_valido(g, v, visitado, anterior)) {

            caminho[posicao] = v;
            visitado[v] = 1;

            if (caminho_bt(g, caminho, visitado, posicao + 1)) {
                return 1;
            }

            /* Backtracking */
            visitado[v] = 0;
            caminho[posicao] = -1;
        }
    }

    return 0;
}

/* Funcao publica para encontrar caminho Hamiltoniano */
int encontrar_caminho_hamiltoniano_bt(
    const Grafo *g,
    int *caminho
) {
    if (g == NULL || caminho == NULL || g->n <= 0) {
        return 0;
    }

    int *visitado = calloc(g->n, sizeof(int));

    if (visitado == NULL) {
        return 0;
    }

    /* Testa cada vertice como ponto inicial */
    for (int inicio = 0; inicio < g->n; inicio++) {

        for (int i = 0; i < g->n; i++) {
            caminho[i] = -1;
            visitado[i] = 0;
        }

        caminho[0] = inicio;
        visitado[inicio] = 1;

        if (caminho_bt(g, caminho, visitado, 1)) {
            free(visitado);
            return 1;
        }
    }

    free(visitado);
    return 0;
}

/* Backtracking para encontrar ciclo Hamiltoniano */
static int ciclo_bt(
    const Grafo *g,
    int *ciclo,
    int *visitado,
    int posicao
) {
    /* Todos os vertices foram visitados */
    if (posicao == g->n) {

        /* Verifica se existe aresta de retorno a origem */
        return grafo_tem_aresta(
            g,
            ciclo[g->n - 1],
            ciclo[0]
        );
    }

    int anterior = ciclo[posicao - 1];

    for (int v = 1; v < g->n; v++) {

        if (vertice_valido(g, v, visitado, anterior)) {

            ciclo[posicao] = v;
            visitado[v] = 1;

            if (ciclo_bt(g, ciclo, visitado, posicao + 1)) {
                return 1;
            }

            /* Backtracking */
            visitado[v] = 0;
            ciclo[posicao] = -1;
        }
    }

    return 0;
}

/* Funcao publica para encontrar ciclo Hamiltoniano */
int encontrar_ciclo_hamiltoniano_bt(
    const Grafo *g,
    int *ciclo
) {
    if (g == NULL || ciclo == NULL || g->n < 3) {
        return 0;
    }

    int *visitado = calloc(g->n, sizeof(int));

    if (visitado == NULL) {
        return 0;
    }

    /* Mantem o vertice 0 como origem */
    for (int i = 0; i < g->n; i++) {
        ciclo[i] = -1;
    }

    ciclo[0] = 0;
    visitado[0] = 1;

    int resultado = ciclo_bt(g, ciclo, visitado, 1);

    free(visitado);

    return resultado;
}

/* Exibe um caminho ou ciclo */
void grafo_exibir_caminho(const int *caminho, int tamanho) {
    if (caminho == NULL || tamanho <= 0) {
        return;
    }

    for (int i = 0; i < tamanho; i++) {
        printf("%d", caminho[i]);

        if (i < tamanho - 1) {
            printf(" -> ");
        }
    }

    printf("\n");
}
