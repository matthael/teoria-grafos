
#ifndef TPS_H
#define TPS_H

#include "hamiltoniano.h"

/* Estrutura que representa uma solucao do TSP */
typedef struct {
    int custo;
    int *ciclo;
    int tamanho;
} TSPResultado;

/* Converte um grafo para uma matriz de custos do TSP */
int **grafo_para_tsp(const Grafo *g);

/* Libera uma matriz de custos */
void tsp_destruir_matriz(int **matriz, int n);

/* Algoritmo TSP por forca bruta */
TSPResultado tsp_forca_bruta(int **matriz, int n);

/* Heuristica do vizinho mais proximo */
TSPResultado tsp_vizinho_mais_proximo(int **matriz, int n);

/* Heuristica de insercao do mais proximo */
TSPResultado tsp_insercao(int **matriz, int n);

/* Libera os recursos de uma solucao */
void tsp_destruir_resultado(TSPResultado *resultado);

/* Exibe uma solucao */
void tsp_exibir_resultado(const char *nome, TSPResultado resultado);

#endif
