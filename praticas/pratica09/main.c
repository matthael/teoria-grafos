#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "cobertura.h"

void imprimir_cobertura(int *cobertura, int n, int tamanho)
{
    printf("{");
    for (int i = 0; i < n; i++)
    {
        if (cobertura[i])
            printf(" %c", 'A' + i);
    }
    printf(" } tamanho %i\n", tamanho);
}

int gerar_arestas(Aresta *a, int n)
{
    int m = 0;
    for (int u = 0; u < n; u++)
    {
        for (int v = u + 1; v < n; v++)
        {
            if (rand() % 2)
            {
                a[m].u = u;
                a[m].v = v;
                m++;
            }
        }
    }
    return m;
}

void experimento(int n)
{
    Aresta arestas[n * (n - 1) / 2];
    int m = gerar_arestas(arestas, n);
    int cobertura[n];

    clock_t inicio = clock();
    int tamanho_bt = cobertura_minima_bt(arestas, m, n, cobertura);
    double tempo_bt = (double)(clock() - inicio) * 1000 / CLOCKS_PER_SEC;

    inicio = clock();
    int tamanho_gulosa = cobertura_gulosa(arestas, m, n, cobertura);
    double tempo_gulosa = (double)(clock() - inicio) * 1000 / CLOCKS_PER_SEC;

    printf("%2i | %2i | %2i | %10.3f | %2i | %10.3f\n", n, m, tamanho_bt, tempo_bt, tamanho_gulosa, tempo_gulosa);
}

int main()
{
    int n = 6;
    Aresta arestas[] = {
        {0, 1}, // A - B
        {1, 3}, // B - D
        {1, 4}, // B - E
        {2, 3}, // C - D
        {2, 4}, // C - E
        {3, 4}, // D - E
        {3, 5}, // D - F
        {1, 2}, // B - C
        {0, 2}, // A - C
        {4, 5}, // E - F
    };
    int m = sizeof(arestas) / sizeof(arestas[0]);

    int c1[] = {0, 1, 1, 1, 1, 0};
    printf("C1 = {B, C, D, E} eh cobertura? %i\n", eh_cobertura(arestas, c1, m));

    int c2[] = {0, 1, 1, 1, 0, 0};
    printf("C2 = {B, C, D} eh cobertura? %i\n\n", eh_cobertura(arestas, c2, m));

    int cobertura[n];
    int tamanho = cobertura_minima_bt(arestas, m, n, cobertura);
    printf("Backtracking: ");
    imprimir_cobertura(cobertura, n, tamanho);

    tamanho = cobertura_gulosa(arestas, m, n, cobertura);
    printf("Gulosa:       ");
    imprimir_cobertura(cobertura, n, tamanho);

    srand(42);

    printf("\nExperimento com grafos aleatorios (tempo em ms)\n");
    printf(" n |  m | BT |   tempo BT | GL | tempo gulosa\n");
    int tamanhos[] = {5, 8, 10, 12, 15};
    for (int i = 0; i < 5; i++)
    {
        experimento(tamanhos[i]);
    }

    return 0;
}
