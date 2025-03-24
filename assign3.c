
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

typedef struct {
    int x, h, type;
} Event;
typedef struct {
    int *heap;
    int size;
} MaxHeap;
typedef struct {
    int *neighbors;
    int count;
    int capacity;
} AdjList;
typedef struct {
    int parent;
    int p1;
    int k1;
    int p2;
    int k2;
    int p3;
} Node;

#define max(a,b) ((a) > (b) ? (a) : (b))

typedef struct node {
    int data;
    struct node *left;
    struct node *right;
} bst;

bst* createNode(int a) {
    bst *temp = (bst*)malloc(sizeof(bst));
    if(temp == NULL) {
        printf("Memory allocation error\n");
        exit(1);
    }
    temp->data = a;
    temp->left = NULL;
    temp->right = NULL;
    return temp;
}

bst* insert(bst* root, int a) {
    if(root == NULL)
        return createNode(a);
    if(a < root->data) {
        root->left = insert(root->left, a);
    } else {
        root->right = insert(root->right, a);
    }
    return root;
}

int height(bst* root) {
    if(root == NULL) return 0;
    int left_ht = height(root->left);
    int right_ht = height(root->right);
    return max(left_ht, right_ht) + 1;
}
void function1(){
    int n;
    scanf("%d", &n);
    bst* root = NULL;
    for(int i = 0; i < n; i++){
        int a;
        scanf("%d", &a);
        root = insert(root, a);
    }
    int ans = height(root);
    printf("%d", ans);
    return ;
}


void addKey(int **sortedKeys, int *sortedSize, int *capacity, int key) {
    if (*capacity == 0) {
        *capacity = 128;
        *sortedKeys = (int *)malloc(*capacity * sizeof(int));
    }
    if (*sortedSize >= *capacity) {
        *capacity *= 2;
        *sortedKeys = (int *)realloc(*sortedKeys, *capacity * sizeof(int));
    }
    (*sortedKeys)[(*sortedSize)++] = key;
}

void inorder(Node *nodes, int idx, int **sortedKeys, int *sortedSize, int *capacity) {
    if (idx == -1)
        return;

    Node curr = nodes[idx];

    if (curr.k2 == -1) { 
        inorder(nodes, curr.p1, sortedKeys, sortedSize, capacity);
        addKey(sortedKeys, sortedSize, capacity, curr.k1);
        inorder(nodes, curr.p2, sortedKeys, sortedSize, capacity);
    } else { 
        inorder(nodes, curr.p1, sortedKeys, sortedSize, capacity);
        addKey(sortedKeys, sortedSize, capacity, curr.k1);
        inorder(nodes, curr.p2, sortedKeys, sortedSize, capacity);
        addKey(sortedKeys, sortedSize, capacity, curr.k2);
        inorder(nodes, curr.p3, sortedKeys, sortedSize, capacity);
    }
}

int binarySearch(int *sortedKeys, int sortedSize, int x) {
    int lo = 0, hi = sortedSize - 1;
    while (lo <= hi) {
        int mid = (lo + hi) / 2;
        if (sortedKeys[mid] == x)
            return mid;
        else if (sortedKeys[mid] < x)
            lo = mid + 1;
        else
            hi = mid - 1;
    }
    return -1;
}
void function2(){
    int n, q;
    if (scanf("%d %d", &n, &q) != 2) return;

    Node *nodes = (Node *)malloc(n * sizeof(Node));

    for (int i = 0; i < n; i++) {
        scanf("%d %d %d %d %d %d", &nodes[i].parent, &nodes[i].p1, &nodes[i].k1, &nodes[i].p2, &nodes[i].k2, &nodes[i].p3);
    }

    int *sortedKeys = NULL;
    int sortedSize = 0, capacity = 0;
    inorder(nodes, 0, &sortedKeys, &sortedSize, &capacity);

    for (int i = 0; i < q; i++) {
        int type, x;
        scanf("%d %d", &type, &x);

        if (type == 1) {
            int index = binarySearch(sortedKeys, sortedSize, x);
            printf("%d\n", (index != -1) ? 1 : 0);
        } else if (type == 2) {
            int index = binarySearch(sortedKeys, sortedSize, x);
            if (index == -1 || index == sortedSize - 1) {
                printf("0\n");
            } else {
                printf("%d\n", sortedKeys[index + 1]);
            }
        }
    }
    return ;
}


void addEdge(AdjList *graph, int u, int v) {
    if(graph[u].count == graph[u].capacity) {
        graph[u].capacity = (graph[u].capacity == 0) ? 2 : graph[u].capacity * 2;
        graph[u].neighbors = realloc(graph[u].neighbors, graph[u].capacity * sizeof(int));
    }
    graph[u].neighbors[graph[u].count++] = v;
}

int* bfs(AdjList *graph, int n, int start) {
    int *dist = (int*)malloc(n * sizeof(int));
    for (int i = 0; i < n; i++) dist[i] = -1;
    
    int *queue = (int*)malloc(n * sizeof(int));
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
void function3(){
    int n, e, m;
    scanf("%d %d %d", &n, &e, &m);
    
    AdjList *graph = (AdjList*)malloc(n * sizeof(AdjList));
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
    
    int *markets = (int*)malloc(m * sizeof(int));
    for (int i = 0; i < m; i++) {
        scanf("%d", &markets[i]);
    }
    
    int *distFromOffice = bfs(graph, n, office);
    int *distFromHome   = bfs(graph, n, home);
    
    int minDist = INT_MAX, count = 0;
    int *bestMarkets = (int*)malloc(m * sizeof(int));
    
    for (int i = 0; i < m; i++) {
        int market = markets[i];
        if (distFromOffice[market] == -1 || distFromHome[market] == -1) continue;
        int totalDist = distFromOffice[market] + distFromHome[market];
        if(totalDist < minDist) {
            minDist = totalDist;
            count = 1;
            bestMarkets[0] = market;
        } else if(totalDist == minDist) {
            bestMarkets[count++] = market;
        }
    }
    
    qsort(bestMarkets, count, sizeof(int), cmpInt);
    
    printf("%d %d\n", minDist, count);
    for (int i = 0; i < count; i++) {
        printf("%d", bestMarkets[i]);
        if(i < count - 1) printf(" ");
    }
    printf("\n");
    return ;
}
int compareEvents(const void *a, const void *b) {
    Event *e1 = (Event *)a, *e2 = (Event *)b;
    if (e1->x != e2->x) return e1->x - e2->x;
    return (e1->type == 1 ? -e1->h : e1->h) - (e2->type == 1 ? -e2->h : e2->h);
}
void insert(MaxHeap *heap, int h) {
    heap->heap[heap->size++] = h;
    for (int i = heap->size - 1; i > 0 && heap->heap[i] > heap->heap[(i - 1) / 2]; i = (i - 1) / 2) {
        int temp = heap->heap[i];
        heap->heap[i] = heap->heap[(i - 1) / 2];
        heap->heap[(i - 1) / 2] = temp;
    }
}

void removeMax(MaxHeap *heap, int h) {
    for (int i = 0; i < heap->size; i++) {
        if (heap->heap[i] == h) {
            heap->heap[i] = heap->heap[--heap->size];
            while (2 * i + 1 < heap->size) {
                int j = 2 * i + 1;
                if (j + 1 < heap->size && heap->heap[j + 1] > heap->heap[j]) j++;
                if (heap->heap[i] >= heap->heap[j]) break;
                int temp = heap->heap[i];
                heap->heap[i] = heap->heap[j];
                heap->heap[j] = temp;
                i = j;
            }
            break;
        }
    }
}

int getMax(MaxHeap *heap) {
    return heap->size ? heap->heap[0] : 0;
}
void function4(){
     int n;
    scanf("%d", &n);
    
    Event *events = (Event *)malloc(2 * n * sizeof(Event));
    MaxHeap heap = {malloc(n * sizeof(int)), 0};
    for (int i = 0; i < n; i++) {
        int l, r, h;
        scanf("%d %d %d", &l, &r, &h);
        events[2 * i] = (Event){l, h, 1};
        events[2 * i + 1] = (Event){r, h, 0};
    }
    qsort(events, 2 * n, sizeof(Event), compareEvents);
    
    int lastX = -1, lastHeight = 0;
    for (int i = 0; i < 2 * n; i++) {
        int x = events[i].x, h = events[i].h, type = events[i].type;
        if (type == 1) insert(&heap, h);
        else removeMax(&heap, h);
        
        int maxHeight = getMax(&heap);
        if (maxHeight != lastHeight) {
            if (lastX != x) printf("%d %d ", x, maxHeight);
            lastHeight = maxHeight;
        }
        lastX = x;
    }
    return;
}