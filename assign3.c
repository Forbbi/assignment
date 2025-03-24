#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

typedef struct {
    int *neighbors;
    int count;
    int capacity;
} AdjList;
void addEdge(AdjList *graph, int u, int v) {
    if (graph[u].count == graph[u].capacity) {
        graph[u].capacity = graph[u].capacity ? graph[u].capacity * 2 : 2;
        graph[u].neighbors = realloc(graph[u].neighbors, graph[u].capacity * sizeof(int));
    }
    graph[u].neighbors[graph[u].count++] = v;
}
int* bfs(AdjList *graph, int n, int start) {
    int *dist = malloc(n * sizeof(int));
    int *queue = malloc(n * sizeof(int));
    for (int i = 0; i < n; i++)
        dist[i] = -1;
    int front = 0, rear = 0;
    dist[start] = 0;
    queue[rear++] = start;
    
    while (front < rear) {
        int cur = queue[front++];
        for (int i = 0; i < graph[cur].count; i++) {
            int nb = graph[cur].neighbors[i];
            if (dist[nb] == -1) {
                dist[nb] = dist[cur] + 1;
                queue[rear++] = nb;
            }
        }
    }
    return dist;
}

int cmpInt(const void *a, const void *b) {
    return (*(int*)a - *(int*)b);
}

void function3() {
    int n, e, m;
    scanf("%d %d %d", &n, &e, &m);
    
    AdjList *graph = malloc(n * sizeof(AdjList));
    for (int i = 0; i < n; i++) {
        graph[i].neighbors = NULL;
        graph[i].count = 0;
        graph[i].capacity = 0;
    }
    
    for (int i = 0; i < e; i++) {
        int u, v;
        scanf("%d %d", &u, &v);
        addEdge(graph, u, v);
        addEdge(graph, v, u);
    }
    int office, home;
    scanf("%d %d", &office, &home);
    int *markets = malloc(m * sizeof(int));
    for (int i = 0; i < m; i++) {
        scanf("%d", &markets[i]);
    }
    int *distFromOffice = bfs(graph, n, office);
    int *distFromHome = bfs(graph, n, home);
    
    int minDist = INT_MAX, count = 0;
    int *bestMarkets = malloc(m * sizeof(int));
    
    for (int i = 0; i < m; i++) {
        int market = markets[i];
        if (distFromOffice[market] == -1 || distFromHome[market] == -1)
            continue;
        int totalDist = distFromOffice[market] + distFromHome[market];
        if (totalDist < minDist) {
            minDist = totalDist;
            count = 0;
            bestMarkets[count++] = market;
        } else if (totalDist == minDist) {
            bestMarkets[count++] = market;
        }
    }
    qsort(bestMarkets, count, sizeof(int), cmpInt);
    printf("%d %d\n", minDist, count);
    for (int i = 0; i < count; i++) {
        printf("%d%s", bestMarkets[i], (i == count - 1) ? "\n" : " ");
    }
    return;
}
