#include <stdio.h>
#include "bellman_ford.h"

int main()
{
    // grafo de grafo.dot (A=0, B=1, C=2, D=3, E=4, F=5)
    GrafoListaPonderado *grafo = criar_grafo(6);

    adicionar_aresta(grafo, 0, 1, 4);  // A - B
    adicionar_aresta(grafo, 0, 2, 2);  // A - C
    adicionar_aresta(grafo, 1, 2, 1);  // B - C
    adicionar_aresta(grafo, 1, 3, 5);  // B - D
    adicionar_aresta(grafo, 1, 4, 8);  // B - E
    adicionar_aresta(grafo, 2, 3, 4);  // C - D
    adicionar_aresta(grafo, 2, 4, 10); // C - E
    adicionar_aresta(grafo, 3, 4, 2);  // D - E
    adicionar_aresta(grafo, 3, 5, 1);  // D - F
    adicionar_aresta(grafo, 4, 5, 3);  // E - F

    printf("Grafo ponderado nao orientado\n");
    imprimir_grafo(grafo);

    int dist[6];
    int pred[6];
    if (bellman_ford(grafo, 0, dist, pred))
    {
        printf("Bellman-Ford a partir de A\n");
        for (int v = 0; v < grafo->num_vertices; v++)
        {
            printf("A ate %c: distancia %i (predecessor ", 'A' + v, dist[v]);
            if (pred[v] == -1)
                printf("-)\n");
            else
                printf("%c)\n", 'A' + pred[v]);
        }
    }
    else
    {
        reconstruir_ciclo_negativo(grafo, dist, pred);
    }
    printf("\n");

    // mesmo grafo, mas com D - E = -2. Como a aresta vale nos dois sentidos,
    // ir e voltar por ela (D -> E -> D) já é um ciclo de custo -4
    GrafoListaPonderado *negativo = criar_grafo(6);

    adicionar_aresta(negativo, 0, 1, 4);  // A - B
    adicionar_aresta(negativo, 0, 2, 2);  // A - C
    adicionar_aresta(negativo, 1, 2, 1);  // B - C
    adicionar_aresta(negativo, 1, 3, 5);  // B - D
    adicionar_aresta(negativo, 1, 4, 8);  // B - E
    adicionar_aresta(negativo, 2, 3, 4);  // C - D
    adicionar_aresta(negativo, 2, 4, 10); // C - E
    adicionar_aresta(negativo, 3, 4, -2); // D - E (negativa)
    adicionar_aresta(negativo, 3, 5, 1);  // D - F
    adicionar_aresta(negativo, 4, 5, 3);  // E - F

    printf("Grafo com a aresta D - E negativa\n");
    imprimir_grafo(negativo);

    int dist2[6];
    int pred2[6];
    if (bellman_ford(negativo, 0, dist2, pred2))
    {
        printf("Nenhum ciclo negativo\n");
    }
    else
    {
        reconstruir_ciclo_negativo(negativo, dist2, pred2);
    }

    return 0;
}
