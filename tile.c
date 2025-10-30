
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
 
#define N 4
 
static int A[N][N];
static long long score = 0;
 
static int rng_percent() { return rand() % 100; }
 
static void add_random_tile(void) {
    int empties[N*N][2], ec = 0;
    for (int i = 0; i < N; ++i)
        for (int j = 0; j < N; ++j)
            if (A[i][j] == 0) { empties[ec][0] = i; empties[ec][1] = j; ++ec; }
 
    if (ec == 0) return;
    int k = rand() % ec;
    int val = (rng_percent() < 90) ? 2 : 4;
    A[empties[k][0]][empties[k][1]] = val;
}
 
static void init_board(void) {
    memset(A, 0, sizeof(A));
    add_random_tile();
    add_random_tile();
    score = 0;
}
 
static void print_board(void) {
    printf("\nScore: %lld\n", score);
    printf("+------+------+------+------+ \n");
    for (int i = 0; i < N; ++i) {
        printf("|");
        for (int j = 0; j < N; ++j) {
            if (A[i][j] == 0) printf("      |");
            else              printf(" %4d |", A[i][j]);
        }
        printf("\n+------+------+------+------+ \n");
    }
    printf("Move with W/A/S/D (or UP/LEFT/DOWN/RIGHT). Q to quit.\n");
}
 
static void process_line_left(int v[], int n, long long *sc) {
    int tmp[N], t = 0;
    for (int i = 0; i < n; ++i) if (v[i] != 0) tmp[t++] = v[i];
    for (int i = t; i < n; ++i) tmp[i] = 0;
 
    int w[N], wlen = 0;
    for (int i = 0; i < n; ) {
        if (i + 1 < n && tmp[i] != 0 && tmp[i] == tmp[i+1]) {
            int m = tmp[i] * 2;
            w[wlen++] = m;
            *sc += m;
            i += 2;
        } else {
            w[wlen++] = tmp[i];
            i++;
        }
    }
    for (int i = wlen; i < n; ++i) w[i] = 0;
    for (int i = 0; i < n; ++i) v[i] = w[i];
}
 
static int boards_differ(int B[N][N]) {
    for (int i = 0; i < N; ++i)
        for (int j = 0; j < N; ++j)
            if (A[i][j] != B[i][j]) return 1;
    return 0;
}
 
static void copy_board(int B[N][N], int src[N][N]) {
    for (int i = 0; i < N; ++i)
        for (int j = 0; j < N; ++j)
            B[i][j] = src[i][j];
}
 
static void move_left(void) {
    for (int i = 0; i < N; ++i) process_line_left(A[i], N, &score);
}
 
static void move_right(void) {
    for (int i = 0; i < N; ++i) {
        int rev[N];
        for (int j = 0; j < N; ++j) rev[j] = A[i][N-1-j];
        process_line_left(rev, N, &score);
        for (int j = 0; j < N; ++j) A[i][N-1-j] = rev[j];
    }
}
 
static void move_up(void) {
    for (int j = 0; j < N; ++j) {
        int col[N];
        for (int i = 0; i < N; ++i) col[i] = A[i][j];
        process_line_left(col, N, &score);
        for (int i = 0; i < N; ++i) A[i][j] = col[i];
    }
}
 
static void move_down(void) {
    for (int j = 0; j < N; ++j) {
        int col[N];
        for (int i = 0; i < N; ++i) col[i] = A[i][j];
        for (int i = 0; i < N/2; ++i) {
            int t = col[i]; col[i] = col[N-1-i]; col[N-1-i] = t;
        }
        process_line_left(col, N, &score);
        for (int i = 0; i < N/2; ++i) {
            int t = col[i]; col[i] = col[N-1-i]; col[N-1-i] = t;
        }
        for (int i = 0; i < N; ++i) A[i][j] = col[i];
    }
}
 
static int can_move(void) {
    for (int i = 0; i < N; ++i)
        for (int j = 0; j < N; ++j)
            if (A[i][j] == 0) return 1;
    for (int i = 0; i < N; ++i)
        for (int j = 0; j < N; ++j) {
            if (i+1 < N && A[i][j] == A[i+1][j]) return 1;
            if (j+1 < N && A[i][j] == A[i][j+1]) return 1;
        }
    return 0;
}
 
static int normalize_move(const char *s) {
    if (!s || !*s) return 0;
    if (strcasecmp(s, "a")==0 || strcasecmp(s, "left")==0)  return 1;
    if (strcasecmp(s, "d")==0 || strcasecmp(s, "right")==0) return 2;
    if (strcasecmp(s, "w")==0 || strcasecmp(s, "up")==0)    return 3;
    if (strcasecmp(s, "s")==0 || strcasecmp(s, "down")==0)  return 4;
    if (strcasecmp(s, "q")==0 || strcasecmp(s, "quit")==0)  return 9;
    return 0;
}
 
int main(void) {
    srand((unsigned)time(NULL));
    init_board();
    print_board();
 
    char line[64];
    while (1) {
        printf("Your move: ");
        if (!fgets(line, sizeof(line), stdin)) break;
        int len = (int)strlen(line);
        if (len && line[len-1]=='\n') line[len-1]=0;
        int mv = normalize_move(line);
 
        if (mv == 9) { printf("Bye!\n"); break; }
        if (mv == 0) { printf("Invalid move.\n"); continue; }
 
        int before[N][N]; copy_board(before, A);
 
        switch (mv) {
            case 1: move_left();  break;
            case 2: move_right(); break;
            case 3: move_up();    break;
            case 4: move_down();  break;
        }
 
        if (boards_differ(before)) {
            add_random_tile();
            print_board();
            if (!can_move()) {
                printf("\nNo moves left. Game Over!\n");
                break;
            }
        } else {
            printf("No tiles moved. Try another direction.\n");
        }
    }
    return 0;
}