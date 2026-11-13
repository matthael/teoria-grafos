#include "cobertura.h"

int eh_cobertura(Aresta *a, int *cobertura, int m)
{
    for (int i = 0; i < m; i++)
    {
        int u = a[i].u;
        int v = a[i].v;
        if (!cobertura[u] && !cobertura[v])
        {
            return 0;
        }
    }

    return 1;
}

void buscar(Aresta *a, int m, int n, int k, int *atual, int tamanho, int *melhor, int *tamanho_melhor)
{
    if (tamanho >= *tamanho_melhor)
        return;

    if (eh_cobertura(a, atual, m))
    {
        for (int i = 0; i < n; i++)
        {
            melhor[i] = atual[i];
        }
        *tamanho_melhor = tamanho;
        return;
    }

    if (k == n)
        return;

    atual[k] = 1;
    buscar(a, m, n, k + 1, atual, tamanho + 1, melhor, tamanho_melhor);

    atual[k] = 0;
    buscar(a, m, n, k + 1, atual, tamanho, melhor, tamanho_melhor);
}

int cobertura_minima_bt(Aresta *a, int m, int n, int *melhor)
{
    int atual[n];
    for (int i = 0; i < n; i++)
    {
        atual[i] = 0;
    }

    int tamanho_melhor = n + 1;
    buscar(a, m, n, 0, atual, 0, melhor, &tamanho_melhor);
    return tamanho_melhor;
}

int cobertura_gulosa(Aresta *a, int m, int n, int *cobertura)
{
    for (int i = 0; i < n; i++)
    {
        cobertura[i] = 0;
    }

    int tamanho = 0;
    while (!eh_cobertura(a, cobertura, m))
    {
        int grau[n];
        for (int i = 0; i < n; i++)
        {
            grau[i] = 0;
        }
        for (int i = 0; i < m; i++)
        {
            if (!cobertura[a[i].u] && !cobertura[a[i].v])
            {
                grau[a[i].u]++;
                grau[a[i].v]++;
            }
        }

        int escolhido = 0;
        for (int i = 1; i < n; i++)
        {
            if (grau[i] > grau[escolhido])
                escolhido = i;
        }

        cobertura[escolhido] = 1;
        tamanho++;
    }

    return tamanho;
}
