#include <stdio.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdlib.h>

// Constants:
const int // Borders, variation 1
    TOPLDB = 201, TOPMDB = 203, TOPRDB = 187,   // ╔ ╦ ╗
    MIDLDB = 204, MIDMDB = 206, MIDRDB = 185,   // ╠ ╬ ╣
    BOTLDB = 200, BOTMDB = 202, BOTRDB = 188,   // ╚ ╩ ╝
    BARHDB = 205, BARVDB = 186;                 // ═ ║

const int // Borders, variation 2
    TOPL = 218, TOPM = 194, TOPR = 191, // ┌ ┬ ┐
    MIDL = 195, MIDM = 197, MIDR = 180, // ├ ┼ ┤
    BOTL = 192, BOTM = 193, BOTR = 217, // └ ┴ ┘
    BARH = 196, BARV = 179;             // ─ │

const int 
    WPAWN = 80,     // P,
    WROOK = 82,     // R,
    WKNIG = 74,     // J,
    WBISH = 66,     // B,
    WQUEN = 81,     // Q,
    WKING = 75,     // K;
    whitePieces[] = { WPAWN, WROOK, WKNIG, WBISH, WQUEN, WKING };

const int
    BPAWN = 80+32,  // p,
    BROOK = 82+32,  // r,
    BKNIG = 74+32,  // j,
    BBISH = 66+32,  // b,
    BQUEN = 81+32,  // q,
    BKING = 75+32,  // k;
    blackPieces[] = { BPAWN, BROOK, BKNIG, BBISH, BQUEN, BKING };

// Variables: 
int chessBoard[8][8] = {
    {BROOK, BKNIG,  BBISH,  BQUEN,  BKING,  BBISH,  BKNIG,  BROOK },
    {BPAWN, BPAWN,  BPAWN,  BPAWN,  BPAWN,  BPAWN,  BPAWN,  BPAWN },
    {0,     0,      0,      0,      0,      0,      0,      0     },
    {0,     0,      0,      0,      0,      0,      0,      0     },
    {0,     0,      0,      0,      0,      0,      0,      0     },
    {0,     0,      0,      0,      0,      0,      0,      0     },
    {WPAWN, WPAWN,  WPAWN,  WPAWN,  WPAWN,  WPAWN,  WPAWN,  WPAWN },
        {WROOK, WKNIG,  WBISH,  WQUEN,  WKING,  WBISH,  WKNIG,  WROOK }
};
int selSquare[2] = {3,3};

int printBoard() {
    for (int y = 0; y < 8; y++) {
        for (int x = 0; x < 8; x++)
        if (selSquare[y][])
        for (int x = 0; x < 8; x++) {
            printf("  %c", chessBoard[y][x] /*Value*/);
        }
        printf("\n");
    }

    return 0;
}

int main() {
    printBoard();
    return 0;
}