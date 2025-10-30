// Simple Console Chess in C (single-file)
// Features:
//  - Legal move validation for all pieces
//  - Check detection; basic checkmate/stalemate detection
//  - Pawn moves incl. double-step and auto-promotion to queen
//  - No castling, no en passant, no draw by repetition/50-move rule
//  - Input format: e2e4, g1f3, a7a8 (auto-promotes to queen)
//  - Quit with: quit / exit
//
// Build: gcc
// Run:   ./chess
//
// Note: Board uses ranks 8..1 top->bottom. White at bottom, Black at top.
 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
 
#define BOARD_SIZE 8
 
typedef struct { int r, c; } Sq;
 
static char board[BOARD_SIZE][BOARD_SIZE];
static int side_to_move = 1; // 1 = White, -1 = Black
 
static void reset_board(void) {
    const char *start[8] = {
        "rnbqkbnr",
        "pppppppp",
        "........",
        "........",
        "........",
        "........",
        "PPPPPPPP",
        "RNBQKBNR"
    };
    for (int r = 0; r < 8; ++r)
        for (int c = 0; c < 8; ++c)
            board[r][c] = start[r][c];
    side_to_move = 1;
}
 
static int in_bounds(int r, int c) { return r >= 0 && r < 8 && c >= 0 && c < 8; }
static int is_white(char p) { return p >= 'A' && p <= 'Z'; }
static int is_black(char p) { return p >= 'a' && p <= 'z'; }
static int same_color(char a, char b){ if(a=='.'||b=='.') return 0; return (is_white(a) && is_white(b)) || (is_black(a) && is_black(b)); }
 
static void print_board(void) {
    printf("\n    a   b   c   d   e   f   g   h\n");
    printf("  +---+---+---+---+---+---+---+---+\n");
    for (int r = 0; r < 8; ++r) {
        printf("%d |", 8 - r);
        for (int c = 0; c < 8; ++c) {
            char p = board[r][c];
            if (p == '.') p = ' ';
            printf(" %c |", p);
        }
        printf(" %d\n", 8 - r);
        printf("  +---+---+---+---+---+---+---+---+\n");
    }
    printf("    a   b   c   d   e   f   g   h\n\n");
}
 
static Sq find_king(int white) {
    char k = white ? 'K' : 'k';
    for (int r = 0; r < 8; ++r)
        for (int c = 0; c < 8; ++c)
            if (board[r][c] == k) return (Sq){r,c};
    return (Sq){-1,-1};
}
 
static int square_attacked(int r, int c, int by_white) {
    // Pawns
    if (by_white) {
        int pr = r+1, pc1 = c-1, pc2 = c+1;
        if (in_bounds(pr,pc1) && board[pr][pc1]=='P') return 1;
        if (in_bounds(pr,pc2) && board[pr][pc2]=='P') return 1;
    } else {
        int pr = r-1, pc1 = c-1, pc2 = c+1;
        if (in_bounds(pr,pc1) && board[pr][pc1]=='p') return 1;
        if (in_bounds(pr,pc2) && board[pr][pc2]=='p') return 1;
    }
    // Knights
    const int kdel[8][2]={{-2,-1},{-2,1},{-1,-2},{-1,2},{1,-2},{1,2},{2,-1},{2,1}};
    for (int i=0;i<8;++i){int rr=r+kdel[i][0],cc=c+kdel[i][1]; if(!in_bounds(rr,cc))continue; char p=board[rr][cc]; if(by_white? p=='N': p=='n') return 1;}
    // King
    for(int dr=-1;dr<=1;++dr) for(int dc=-1;dc<=1;++dc){ if(!dr&&!dc)continue; int rr=r+dr,cc=c+dc; if(in_bounds(rr,cc)){char p=board[rr][cc]; if(by_white? p=='K': p=='k') return 1;}}
    // Sliding pieces: bishops/queens (diagonals)
    const int d1[4][2]={{-1,-1},{-1,1},{1,-1},{1,1}};
    for(int i=0;i<4;++i){int rr=r+d1[i][0],cc=c+d1[i][1]; while(in_bounds(rr,cc)){ char p=board[rr][cc]; if(p!='.'){ if(by_white? (p=='B'||p=='Q') : (p=='b'||p=='q')) return 1; else break;} rr+=d1[i][0]; cc+=d1[i][1]; }}
    // Sliding pieces: rooks/queens (orthogonals)
    const int d2[4][2]={{-1,0},{1,0},{0,-1},{0,1}};
    for(int i=0;i<4;++i){int rr=r+d2[i][0],cc=c+d2[i][1]; while(in_bounds(rr,cc)){ char p=board[rr][cc]; if(p!='.'){ if(by_white? (p=='R'||p=='Q') : (p=='r'||p=='q')) return 1; else break;} rr+=d2[i][0]; cc+=d2[i][1]; }}
    return 0;
}
 
static int in_check(int white){ Sq k=find_king(white); if(k.r==-1) return 0; return square_attacked(k.r,k.c,!white); }
 
static int path_clear(int r1,int c1,int r2,int c2){ int dr=(r2>r1)-(r2<r1); int dc=(c2>c1)-(c2<c1); int rr=r1+dr,cc=c1+dc; while(rr!=r2||cc!=c2){ if(board[rr][cc] != '.') return 0; rr+=dr; cc+=dc;} return 1; }
 
static int is_legal_pseudo(int r1,int c1,int r2,int c2){
    char p = board[r1][c1];
    if (p=='.') return 0;
    if (!in_bounds(r2,c2)) return 0;
    if (same_color(board[r1][c1], board[r2][c2])) return 0;
    int dr = r2 - r1, dc = c2 - c1, adr = dr<0?-dr:dr, adc = dc<0?-dc:dc;
    int white = is_white(p);
    switch (tolower(p)) {
        case 'p': {
            int dir = white ? -1 : 1; // white moves up (towards r=0)
            int start_r = white ? 6 : 1;
            // single step
            if (dc==0 && dr==dir && board[r2][c2]=='.') return 1;
            // double step
            if (dc==0 && dr==2*dir && r1==start_r && board[r1+dir][c1]=='.' && board[r2][c2]=='.') return 1;
            // captures
            if (adr==1 && dr==dir && board[r2][c2]!='.' && !same_color(p,board[r2][c2])) return 1;
            return 0;
        }
        case 'n':
            return (adr==2 && adc==1) || (adr==1 && adc==2);
        case 'b':
            if (adr==adc && path_clear(r1,c1,r2,c2)) return 1; return 0;
        case 'r':
            if ((adr==0||adc==0) && path_clear(r1,c1,r2,c2)) return 1; return 0;
        case 'q':
            if ((adr==adc || adr==0 || adc==0) && path_clear(r1,c1,r2,c2)) return 1; return 0;
        case 'k':
            return adr<=1 && adc<=1;
    }
    return 0;
}
 
static void make_move(int r1,int c1,int r2,int c2,char *captured){ *captured = board[r2][c2]; board[r2][c2]=board[r1][c1]; board[r1][c1]='.'; }
static void unmake_move(int r1,int c1,int r2,int c2,char captured){ board[r1][c1]=board[r2][c2]; board[r2][c2]=captured; }
 
static int is_legal_move(int r1,int c1,int r2,int c2){
    char p = board[r1][c1]; if(p=='.') return 0;
    if (side_to_move==1 && !is_white(p)) return 0;
    if (side_to_move==-1 && !is_black(p)) return 0;
    if (!is_legal_pseudo(r1,c1,r2,c2)) return 0;
    // make move and see if own king left in check
    char captured; make_move(r1,c1,r2,c2,&captured);
    // promotion auto to queen
    if (tolower(board[r2][c2])=='p'){
        if (is_white(board[r2][c2]) && r2==0) board[r2][c2]='Q';
        if (is_black(board[r2][c2]) && r2==7) board[r2][c2]='q';
    }
    int illegal = in_check(side_to_move==1);
    unmake_move(r1,c1,r2,c2,captured);
    return !illegal;
}
 
static int has_legal_moves(void){
    for(int r1=0;r1<8;++r1)for(int c1=0;c1<8;++c1){
        char p=board[r1][c1]; if(p=='.') continue; if(side_to_move==1 && !is_white(p)) continue; if(side_to_move==-1 && !is_black(p)) continue;
        for(int r2=0;r2<8;++r2)for(int c2=0;c2<8;++c2){ if(is_legal_move(r1,c1,r2,c2)) return 1; }
    }
    return 0;
}
 
static int parse_move(const char *s,int *r1,int *c1,int *r2,int *c2){
    if (strlen(s) < 4) return 0;
    char f1=s[0], rnk1=s[1], f2=s[2], rnk2=s[3];
    if (f1<'a'||f1>'h'||f2<'a'||f2>'h'||rnk1<'1'||rnk1>'8'||rnk2<'1'||rnk2>'8') return 0;
    *c1 = f1-'a'; *c2 = f2-'a';
    *r1 = 8-(rnk1-'0'); *r2 = 8-(rnk2-'0');
    return 1;
}
 
static void apply_move(int r1,int c1,int r2,int c2){
    char cap; make_move(r1,c1,r2,c2,&cap);
    // promotion auto to queen
    if (tolower(board[r2][c2])=='p'){
        if (is_white(board[r2][c2]) && r2==0) board[r2][c2]='Q';
        if (is_black(board[r2][c2]) && r2==7) board[r2][c2]='q';
    }
    side_to_move = -side_to_move;
}
 
int main(void){
    printf("Simple Console Chess (C)\nNo castling/en passant; promotions auto to queen. Enter moves like e2e4.\nType 'quit' to exit.\n");
    reset_board();
 
    while (1) {
        print_board();
        int white_to_move = (side_to_move==1);
        if (in_check(white_to_move)) printf("%s is in check!\n", white_to_move?"White":"Black");
        if (!has_legal_moves()){
            if (in_check(white_to_move)) { printf("Checkmate! %s wins.\n", white_to_move?"Black":"White"); }
            else { printf("Stalemate. Draw.\n"); }
            break;
        }
        char inp[64];
        printf("%s> ", white_to_move?"White":"Black");
        if (!fgets(inp,sizeof(inp),stdin)) break;
        // trim
        size_t n=strlen(inp); while(n&& (inp[n-1]=='\n'||inp[n-1]=='\r'||isspace((unsigned char)inp[n-1]))) inp[--n]='\0';
        for(size_t i=0;i<n;++i) inp[i]=(char)tolower((unsigned char)inp[i]);
        if (!strcmp(inp,"quit")||!strcmp(inp,"exit")) break;
        int r1,c1,r2,c2; if(!parse_move(inp,&r1,&c1,&r2,&c2)){ printf("Invalid input. Use like e2e4.\n"); continue; }
        if (!is_legal_move(r1,c1,r2,c2)) { printf("Illegal move.\n"); continue; }
        apply_move(r1,c1,r2,c2);
    }
    return 0;
}
