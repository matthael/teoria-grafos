#include <stdio.h>
#include "dijkstra.h"

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
    dijkstra(grafo, 0, dist, pred);

    printf("Dijkstra a partir de A\n");
    for (int v = 0; v < grafo->num_vertices; v++)
    {
        printf("A ate %c: distancia %i, caminho ", 'A' + v, dist[v]);
        reconstruir_caminho(0, v, pred);
        printf("\n");
    }

    return 0;
}
