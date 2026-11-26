#include <iostream>
#include <cstdio>
using namespace std;

#define N 14
#define INF 999999

const char* nome[N] = {"Sao Paulo", "Lisboa", "Porto", "Bruxelas", "Bruges", "Gante", "Antuerpia",
                       "Roma", "Milao", "Veneza", "Liubliana", "Bratislava", "Podgorica", "Kotor"};

double custo[N][N];
double distancia[N][N];
double tempo[N][N];

int SP = 0, LIS = 1, POR = 2, BRU = 3, BRG = 4, GAN = 5, ANT = 6;
int ROM = 7, MIL = 8, VEN = 9, LJU = 10, BTS = 11, POD = 12, KOT = 13;

void ligar(int a, int b, double c, double d, double t) {
    custo[a][b] = custo[b][a] = c;
    distancia[a][b] = distancia[b][a] = d;
    tempo[a][b] = tempo[b][a] = t;
}

void montarGrafo() {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            if (i == j) {
                custo[i][j] = distancia[i][j] = tempo[i][j] = 0;
            } else {
                custo[i][j] = distancia[i][j] = tempo[i][j] = INF;
            }
        }
    }

    ligar(SP, LIS, 550, 7900, 10.5);
    ligar(SP, POR, 600, 8000, 10.5);
    ligar(SP, BRU, 620, 9200, 11.5);
    ligar(SP, ROM, 650, 9300, 11.5);
    ligar(SP, MIL, 680, 9400, 12);

    ligar(LIS, POR, 30, 313, 3);
    ligar(LIS, BRU, 110, 1720, 2.7);
    ligar(LIS, ROM, 90, 1870, 3);
    ligar(LIS, MIL, 100, 1700, 3);
    ligar(POR, BRU, 100, 1500, 2.5);
    ligar(POR, MIL, 95, 1550, 2.8);

    ligar(BRU, BRG, 17, 100, 1);
    ligar(BRU, GAN, 10, 55, 0.6);
    ligar(BRU, ANT, 9, 45, 0.5);
    ligar(BRG, GAN, 9, 45, 0.5);
    ligar(BRG, ANT, 18, 95, 1.3);
    ligar(GAN, ANT, 15, 60, 0.8);

    ligar(BRU, MIL, 80, 700, 1.6);
    ligar(BRU, ROM, 95, 1180, 2.2);
    ligar(BRU, BTS, 85, 980, 1.9);

    ligar(ROM, MIL, 60, 480, 1.2);
    ligar(ROM, VEN, 55, 530, 3.8);
    ligar(MIL, VEN, 30, 270, 2.5);

    ligar(VEN, LJU, 25, 245, 4);
    ligar(MIL, LJU, 35, 480, 7);
    ligar(LJU, BTS, 40, 430, 5.5);

    ligar(ROM, BTS, 60, 780, 1.8);
    ligar(MIL, BTS, 50, 670, 1.6);

    ligar(POD, KOT, 8, 90, 1.5);
    ligar(POD, ROM, 60, 560, 1.5);
    ligar(POD, MIL, 55, 880, 1.9);
    ligar(KOT, ROM, 80, 600, 1.6);
    ligar(KOT, MIL, 85, 880, 2);
}

int visitado[N];

void dfs(int v) {
    visitado[v] = 1;
    for (int i = 0; i < N; i++) {
        if (custo[v][i] < INF && visitado[i] == 0) {
            dfs(i);
        }
    }
}

void mostrarMatriz(const char* titulo, double m[N][N], int tipo) {
    printf("\n=== MATRIZ DE %s ===\n", titulo);
    printf("            ");
    for (int j = 0; j < N; j++) {
        printf("%6d", j);
    }
    printf("\n");
    for (int i = 0; i < N; i++) {
        printf("%2d %-9.9s ", i, nome[i]);
        for (int j = 0; j < N; j++) {
            if (m[i][j] == INF) {
                printf("%6s", "-");
            } else if (tipo == 1) {
                printf("%6.1f", m[i][j]);
            } else {
                printf("%6.0f", m[i][j]);
            }
        }
        printf("\n");
    }
}

void dijkstra(const char* titulo, double m[N][N], int origem, int destino) {
    double dist[N];
    int ant[N];
    int fechado[N];

    for (int i = 0; i < N; i++) {
        dist[i] = INF;
        ant[i] = -1;
        fechado[i] = 0;
    }
    dist[origem] = 0;

    for (int k = 0; k < N; k++) {
        int u = -1;
        for (int i = 0; i < N; i++) {
            if (fechado[i] == 0 && (u == -1 || dist[i] < dist[u])) {
                u = i;
            }
        }
        fechado[u] = 1;

        for (int v = 0; v < N; v++) {
            if (m[u][v] < INF && dist[u] + m[u][v] < dist[v]) {
                dist[v] = dist[u] + m[u][v];
                ant[v] = u;
            }
        }
    }

    double totalCusto = 0, totalDist = 0, totalTempo = 0;
    int v = destino;
    while (ant[v] != -1) {
        totalCusto += custo[ant[v]][v];
        totalDist += distancia[ant[v]][v];
        totalTempo += tempo[ant[v]][v];
        v = ant[v];
    }

    int rota[N];
    int qtd = 0;
    v = destino;
    while (v != -1) {
        rota[qtd] = v;
        qtd++;
        v = ant[v];
    }

    printf("\nMenor caminho por %s:\n", titulo);
    for (int i = qtd - 1; i >= 0; i--) {
        printf("%s", nome[rota[i]]);
        if (i > 0) {
            printf(" -> ");
        }
    }
    printf("\nCusto: %.1f EUR | Distancia: %.1f km | Tempo: %.1f h\n", totalCusto, totalDist, totalTempo);
}

void prim() {
    double menor[N];
    int pai[N];
    int naArvore[N];

    for (int i = 0; i < N; i++) {
        menor[i] = INF;
        pai[i] = -1;
        naArvore[i] = 0;
    }
    menor[0] = 0;

    double totalCusto = 0, totalDist = 0, totalTempo = 0;

    printf("\n=== ARVORE GERADORA MINIMA (PRIM - CUSTO) ===\n");
    for (int k = 0; k < N; k++) {
        int u = -1;
        for (int i = 0; i < N; i++) {
            if (naArvore[i] == 0 && (u == -1 || menor[i] < menor[u])) {
                u = i;
            }
        }
        naArvore[u] = 1;

        if (pai[u] != -1) {
            int p = pai[u];
            printf("  %-11s -- %-11s | %4.0f EUR | %5.0f km | %4.1f h\n",
                   nome[p], nome[u], custo[p][u], distancia[p][u], tempo[p][u]);
            totalCusto += custo[p][u];
            totalDist += distancia[p][u];
            totalTempo += tempo[p][u];
        }

        for (int v = 0; v < N; v++) {
            if (naArvore[v] == 0 && custo[u][v] < menor[v]) {
                menor[v] = custo[u][v];
                pai[v] = u;
            }
        }
    }

    printf("Total AGM -> Custo: %.1f EUR | Distancia: %.1f km | Tempo: %.1f h\n", totalCusto, totalDist, totalTempo);
}

int main() {
    montarGrafo();

    dfs(0);
    int conexo = 1;
    for (int i = 0; i < N; i++) {
        if (visitado[i] == 0) {
            conexo = 0;
        }
    }

    int grauMinimo = N;
    for (int i = 0; i < N; i++) {
        int grau = 0;
        for (int j = 0; j < N; j++) {
            if (i != j && custo[i][j] < INF) {
                grau++;
            }
        }
        if (grau < grauMinimo) {
            grauMinimo = grau;
        }
    }

    printf("Vertices: %d | Conexo: %s | Grau minimo: %d\n", N, conexo ? "sim" : "nao", grauMinimo);

    mostrarMatriz("CUSTO (EUR)", custo, 0);
    mostrarMatriz("DISTANCIA (km)", distancia, 0);
    mostrarMatriz("TEMPO (h)", tempo, 1);

    printf("\n=== CAMINHOS MINIMOS (%s -> %s) ===\n", nome[SP], nome[KOT]);
    dijkstra("CUSTO (EUR)", custo, SP, KOT);
    dijkstra("DISTANCIA (km)", distancia, SP, KOT);
    dijkstra("TEMPO (h)", tempo, SP, KOT);

    prim();

    return 0;
}
