#ifndef COBERTURA_H
#define COBERTURA_H

typedef struct {
    int u;
    int v;
} Aresta;

int eh_cobertura(Aresta *a, int *cobertura, int m);
int cobertura_minima_bt(Aresta *a, int m, int n, int *melhor);
int cobertura_gulosa(Aresta *a, int m, int n, int *cobertura);

#endif
