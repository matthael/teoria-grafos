
#ifndef HAMILTONIANO_H
#define HAMILTONIANO_H

/* Estrutura do grafo utilizando matriz de adjacencia */
typedef struct {
    int n;
    int **matriz;
} Grafo;

/* Criacao e destruicao do grafo */
Grafo *grafo_criar(int n);
void grafo_destruir(Grafo *g);

/* Adiciona uma aresta nao direcionada */
void grafo_adicionar_aresta(Grafo *g, int origem, int destino);

/* Verifica se existe uma aresta */
int grafo_tem_aresta(const Grafo *g, int origem, int destino);

/* Caminho Hamiltoniano utilizando backtracking */
int encontrar_caminho_hamiltoniano_bt(
    const Grafo *g,
    int *caminho
);

/* Ciclo Hamiltoniano utilizando backtracking */
int encontrar_ciclo_hamiltoniano_bt(
    const Grafo *g,
    int *ciclo
);

/* Exibe o caminho ou ciclo encontrado */
void grafo_exibir_caminho(const int *caminho, int tamanho);

#endif
