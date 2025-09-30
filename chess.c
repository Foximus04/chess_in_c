/**
 * Chess!
 * @file chess.c
 * @author Mats K. - mats.herman.knutsen@gmail.com
 * @date 16/09/2025
 */
#include <stdio.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdlib.h>

// Constants:
// const int // Borders, variation 1
//     TOPL = 201, TOPM = 203, TOPR = 187, // ╔ ╦ ╗
//     MIDL = 204, MIDM = 206, MIDR = 185, // ╠ ╬ ╣
//     BOTL = 200, BOTM = 202, BOTR = 188, // ╚ ╩ ╝
//     BARH = 205, BARV = 186;             // ═ ║

const int // Borders, variation 2
    TOPL = 218, TOPM = 194, TOPR = 191, // ┌ ┬ ┐
    MIDL = 195, MIDM = 197, MIDR = 180, // ├ ┼ ┤
    BOTL = 192, BOTM = 193, BOTR = 217, // └ ┴ ┘
    BARH = 196, BARV = 179;             // ─ │

const int   // White pieces, variation 2
    WPAWN = 80,     // P,
    WROOK = 82,     // R,
    WKNIG = 74,     // J,
    WBISH = 66,     // B,
    WQUEN = 81,     // Q,
    WKING = 75,     // K;
    whitePieces[] = { WPAWN, WROOK, WKNIG, WBISH, WQUEN, WKING };

const int   // Black pieces, variation 2
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
// int chessBoard[8][8] = {
//     {BROOK, BKNIG,  BBISH,  BQUEN,  BKING,  BBISH,  BKNIG,  BROOK },
//     {0,     0,      0,      0,      0,      0,      0,      0     },
//     {0,     0,      0,      0,      0,      0,      0,      0     },
//     {0,     0,      0,      BPAWN,      0,      0,      0,      0     },
//     {0,     0,      0,      0,      0,      0,      0,      0     },
//     {0,     0,      0,      0,      0,      0,      0,      0     },
//     {0,     0,      0,      0,      0,      0,      0,      0     },
//     {WROOK, WKNIG,  WBISH,  WQUEN,  WKING,  WBISH,  WKNIG,  WROOK }
// };
bool gameRunning = true;

int printBoard() {  //
    printf("\n     a   b   c   d   e   f   g   h  \n");
    printf("   %c", TOPL);                          // Top border line
    for (int i = 0; i < 7; i++)     printf("%c%c%c%c", BARH, BARH, BARH, TOPM);
    printf("%c%c%c%c", BARH, BARH, BARH, TOPR);
    for (int i = 0; i < 8; i++) {
        printf("\n %i ", 8-i);                      // Numbers
        for (int j = 0; j < 8; j++) {               // Line that contains
            printf("%c ",BARV);                     // the pieces
            if (chessBoard[i][j] != 0)  printf("%c ", chessBoard[i][j]);   
            else printf("  ");                      //
        }                                           //
        printf("%c %i\n   %c", BARV,8-i, MIDL);     // Separation line
        for (int j = 0; j < 8; j++) printf("%c%c%c%c", BARH, BARH, BARH, MIDM);
        printf("\b%c", MIDR);                       //
    };
    for (int j = 0; j < 64; j++)    printf("\b");      // Remove last line
    printf("   %c", BOTL);                          // Botton border line
    for (int j = 0; j < 8; j++)     printf("%c%c%c%c", BARH, BARH, BARH, BOTM);
    printf("\b%c", BOTR);                           //
    printf("\n     a   b   c   d   e   f   g   h  \n");

    return 0;
}
bool baseLogic(int x1, int x2, int y1, int y2, int piece) {
    if (
        (x1 < 0) || (x1 > 8)
                 ||
        (x2 < 0) || (x2 > 8)
                 ||
        (y1 < 0) || (y1 > 8)
                 ||
        (y2 < 0) || (y2 > 8)
    )  {
        printf("Invalid move, out of bounds.");
        return false;
    }
    if (piece == 0) {
        printf("Invalid move, there is nothing to move.");
        return false;
    }
    if ((x2 == x1) && (y2 == y1)){
        printf("Invalid move, same position.");
        return false;
    }
    return true;
}
bool isMoveDiagonal(int x1, int x2, int y1, int y2) {
    if (abs(x2 - x1) == abs(y2 - y1)) 
    return true;
    return false;
}
bool isMoveStraight(int x1, int x2, int y1, int y2) {
    if ((((x2 - x1) == 0) && ((y2 - y1) != 0)) 
     || (((x2 - x1) != 0) && ((y2 - y1) == 0)))
    return true;
    return false;
}
bool isWhite(int piece) {
    for (int i = 0; i < 6; i++) 
    if (piece == whitePieces[i]) 
    return true;
    return false;
}
bool friendlyFire(int targetSq, int p) {
    if (
        (targetSq > 0)  // Not empty
        &&
        (
            ((isupper(targetSq) > 0) // White -> white
            && 
            (isWhite(p) == true)) 
            ||
            ((islower(targetSq) > 0) // Black -> black
            && 
            (isWhite(p) == false))
        )
    )  {
        printf("Invalid move, can't capture own piece.");
        return true;
    }
    return false;
}
bool wPawnLogic(int x1, int x2, int y1, int y2) { // WHITE
    int targetSquare = chessBoard[y2][x2];
    // Only forward moves allowed
    if (y2 >= y1) {
        printf("Invalid move, pawns can't go backwards or sideways.\n");
        return false;
    }
    int dx = x2 - x1,
        dy = y2 - y1;

    // Sideways move (not capturing)
    if (dy == 0 && dx != 0) {
        printf("Invalid move, pawns can't go sideways.\n");
        return false;
    }
    // Move forward by 1
    if (dy == -1 && dx == 0) {
        if (chessBoard[y2][x2] != 0) {
            printf("Invalid move, something in the way.\n");
            return false;
        }
        return true;
    }
    // Move forward by 2 from starting position
    if (y1 == 6 && dy == -2 && dx == 0) {
        if (chessBoard[y2][x2] != 0 || chessBoard[y1-1][x1] != 0) {
            printf("Invalid move, something in the way.\n");
            return false;
        }
        return true;
    }
    // Capturing
    if (dy == -1 && (dx == -1 || dx == 1)) {
        if (chessBoard[y2][x2] == 0) {
            printf("Invalid move, nothing to capture.\n");
            return false;
        }
        return true;
    }
    // More than 1 forward
    if (dy < -1) {
        printf("Invalid move, can't go more than 1 (except from start).\n");
        return false;
    }
    // En passant (not implemented)
    // ...

    printf("Invalid pawn move, how so? i don't know dude.\n");
    return false;
}
bool bPawnLogic(int x1, int x2, int y1, int y2) { // BLACK
    int targetSquare = chessBoard[y2][x2];
    // Only forward moves allowed
    if (y2 <= y1) {
        printf("Invalid move, pawns can't go backwards or sideways.\n");
        return false;
    }
    int dx = x2 - x1,
        dy = y2 - y1;

    // Sideways move (not capturing)
    if (dy == 0 && dx != 0) {
        printf("Invalid move, pawns can't go sideways.\n");
        return false;
    }
    // Move forward by 1
    if (dy == 1 && dx == 0) {
        if (chessBoard[y2][x2] != 0) {
            printf("Invalid move, something in the way.\n");
            return false;
        }
        return true;
    }
    // Move forward by 2 from starting position
    if (y1 == 1 && dy == 2 && dx == 0) {
        if (chessBoard[y2][x2] != 0 || chessBoard[y1+1][x1] != 0) {
            printf("Invalid move, something in the way.\n");
            return false;
        }
        return true;
    }
    // Capturing
    if (dy == 1 && (dx == -1 || dx == 1)) {
        if (chessBoard[y2][x2] == 0) {
            printf("Invalid move, nothing to capture.\n");
            return false;
        }
        return true;
    }
    // More than 1 forward
    if (dy < 1) {
        printf("Invalid move, can't go more than 1 (except from start).\n");
        return false;
    }
    // En passant (not implemented)
    // ...

    printf("Invalid pawn move, how so? i don't know dude.\n");
    return false;
}
int checkDeltaStraight(int x1, int x2, int y1, int y2) {
    int dx = x2 - x1,
        dy = y2 - y1;
    // x axis
    if (dx > 0)   // Rightwards
    for (int i = x1+1; i <= x2; i++) {
        if (chessBoard[y2][i] > 0) {
            if (i != x1)
            return chessBoard[y2][i];
        }
    }
    if (dx < 0)    // Leftwards
    for (int i = x1-1; i >= x2; i--) {
        if (chessBoard[y2][i] > 0) {
            if (i != x1)
            return chessBoard[y2][i];
        }
    }
    // y axis
    if (dy > 0)    // Backwards
    for (int i = y1+1; i <= y2; i++) {
        if (chessBoard[i][x2] > 0) {
            if (i != y1)
            return chessBoard[i][x2];
        }
    }
    if (dy < 0)    // Forwards
    for (int i = y1-1; i >= y2; i--) {
        if (chessBoard[i][x2] > 0) {
            if (i != y1)
            return chessBoard[i][x2];
        }
    }
    return 0;
}
int checkDeltaDiagonal(int x1, int x2, int y1, int y2) {
    int dx = x2 - x1,
        dy = y2 - y1;
    bool right = (x2 > x1),
         down  = (y2 > y1);
    int dirX = (right) ? 1 : -1;
    int dirY = (down)  ? 1 : -1;

    if (right) 
    for (
        int i = y1,         j = x1;
        abs(i) < abs(y2),   abs(j) < abs(x2);
        i += dirY,          j += dirX
    ) 
    if (chessBoard[i + dirY][ j + dirX] != 0)
    return chessBoard[i + dirY][j + dirX];

    else if (((i + dirY > 7) || (i + dirY < 0)) 
          || ((j + dirX > 7) || (j + dirX < 0)))
    return 0;
    
    
    if (!right) 
    for (
        int i = y1,         j = x1;
        abs(i) > abs(y2),   abs(j) > abs(x2);
        i += dirY,          j += dirX
    ) 
    if (chessBoard[i + dirY][ j + dirX] != 0) 
    return chessBoard[i + dirY][j + dirX];

    else if (((i + dirY > 7) || (i + dirY < 0)) 
          || ((j + dirX > 7) || (j + dirX < 0)))
    return 0;
    

    return 0;
}
bool rookLogic(int x1, int x2, int y1, int y2) {
    if (!isMoveStraight(x1,x2,y1,y2)) {
        printf("Invalid move, can only do straight moves.\n");
        return false;
    }
    int p = checkDeltaStraight(x1,x2,y1,y2);
    if (p > 0) {
        printf("Invalid move, %c in the way.\n", p);
        return false;
    }
    printf("\n");
    return true;
}
bool knightLogic(int x1, int x2, int y1, int y2) {
    int dx = x2 - x1,
        dy = y2 - y1;
    
    int possibleMoves[8][2] = {
        { y1 + 2, x1 + 1 }, { y1 + 2, x1 - 1 },
        { y1 + 1, x1 + 2 }, { y1 + 1, x1 - 2 },
        { y1 - 2, x1 + 1 }, { y1 - 2, x1 - 1 },
        { y1 - 1, x1 + 2 }, { y1 - 1, x1 - 2 }
    };
    for (int i = 0; i < 8; i++) 
    if ((y2 == possibleMoves[i][0]) && (x2 == possibleMoves[i][1]))
    return true;
    
    printf("Invalid knight move.");
    return false;
}
bool bishopLogic(int x1, int x2, int y1, int y2) {
    if (!isMoveDiagonal(x1,x2,y1,y2)) {
        printf("Invalid move, can only go diagonally.\n");
        return false;
    }
    int p = checkDeltaDiagonal(x1,x2,y1,y2);
    if (p > 0) {
        printf("Invalid move, %c in the way.\n", p);
        return false;
    }
    printf("\n");
    return true;
}
bool queenLogic(int x1, int x2, int y1, int y2) {
    bool straightMove = isMoveStraight(x1,x2,y1,y2),
         diagonalMove = isMoveDiagonal(x1,x2,y1,y2);

    if (!straightMove && !diagonalMove) {
        printf("Invalid move, can only go straight or diagonal.\n");
        return false;
    }
    int p1 = checkDeltaStraight(x1,x2,y1,y2);
    if (straightMove)
    if (p1 > 0) {
        printf("Invalid move, %c in the way.\n", p1);
        return false;
    }
    int p2 = checkDeltaDiagonal(x1,x2,y1,y2);
    if (diagonalMove)
    if (p2 > 0) {
        printf("Invalid move, %c in the way.\n", p2);
        return false;
    }

    return true;
}
bool kingLogic(int x1, int x2, int y1, int y2) {
    int dx = x2 - x1,
        dy = y2 - y1;
    
    if ((abs(dx) > 1) || (abs(dy) > 1)) {
        printf("Invalid move, can only move 1 square.\n");
        return false;
    }
    // int shortest = (x2 < y2) ? x2 : y2;

    int dangersStraight[4] = {
        checkDeltaStraight(x2, 0,y2,y2),
        checkDeltaStraight(x2, 7,y2,y2),
        checkDeltaStraight(x2,x2,y2-dy, 0),
        checkDeltaStraight(x2,x2,y2+dy, 7)
    };
    int dangersDiagonal[4] = {
        checkDeltaDiagonal(x2, 0,y2, 0),
        checkDeltaDiagonal(x2, 7,y2, 0),
        checkDeltaDiagonal(x2, 0,y2, 7),
        checkDeltaDiagonal(x2, 7,y2, 7)
    };
    
    int enemyRook, enemyQuen, enemyBish, enemyKnig, enemyPawn, enemyKing;

    switch (isWhite(chessBoard[y1][x1])) {
    case true:
        enemyRook = BROOK;
        enemyQuen = BQUEN;
        enemyBish = BBISH;
        enemyKnig = BKNIG;
        enemyPawn = BPAWN;
        enemyKing = BKING;
        break;
    case false:
        enemyRook = WROOK;
        enemyQuen = WQUEN;
        enemyBish = WBISH;
        enemyKnig = WKNIG;
        enemyPawn = WPAWN;
        enemyKing = WKING;
        break;
    default:
        break;
    }
    for (int i = 0; i < 4; i++) {
        // printf("Straight: '%c'\t nr '%i'\n",dangersStraight[i], i);
        if ((dangersStraight[i] == enemyRook) || (dangersStraight[i] == enemyQuen)) {
            printf("Invalid move, %c threatens that square.\n", dangersStraight[i]);
            return false;
        }
        // if (dangersStraight[i] == WKING) printf("i hate myself");
        // printf("Diagonal: '%c'\n",dangersDiagonal[i]);
        if ((dangersDiagonal[i] == enemyBish) || (dangersDiagonal[i] == enemyQuen)) {
            printf("Invalid move, %c threatens that square.\n", dangersDiagonal[i]);
            return false;
        }
    }
    int possibleKnights[8][2] = {
        { y2 + 2, x2 + 1 }, { y2 + 2, x2 - 1 },
        { y2 + 1, x2 + 2 }, { y2 + 1, x2 - 2 },
        { y2 - 2, x2 + 1 }, { y2 - 2, x2 - 1 },
        { y2 - 1, x2 + 2 }, { y2 - 1, x2 - 2 }
    };
    for (int i = 0; i < 8; i++){
        if (chessBoard[possibleKnights[i][0]][possibleKnights[i][1]] == enemyKnig) {
            printf("Invalid move, %c threatens that square.\n", enemyKnig);
            return false;
        }
    }
    int possibleWhitePawns[2][2] = {
        { y2 - 1, x2 - 1 },
        { y2 - 1, x2 + 1 }
    };
    int possibleBlackPawns[2][2] = {
        { y2 + 1, x2 - 1 },
        { y2 + 1, x2 + 1 }
    };
    int possibleEnemyPawns[2][2];
    
    if (isWhite(chessBoard[y1][x1])) {
        if (chessBoard[possibleBlackPawns[0][0]][possibleBlackPawns[0][1]] == enemyPawn) {
            printf("Invalid move, %c is threatening that square.\n", enemyPawn);
            return false;
        }
        if (chessBoard[possibleBlackPawns[1][0]][possibleBlackPawns[1][1]] == enemyPawn) {
            printf("Invalid move, %c is threatening that square.\n", enemyPawn);
            return false;
        }
    }
    else {
        if (chessBoard[possibleWhitePawns[0][0]][possibleWhitePawns[0][1]] == enemyPawn) {
            printf("Invalid move, %c is threatening that square.\n", enemyPawn);
            return false;
        }
        if (chessBoard[possibleWhitePawns[1][0]][possibleWhitePawns[1][1]] == enemyPawn) {
            printf("Invalid move, %c is threatening that square.\n", enemyPawn);
            return false;
        }
    }

    
    printf("\n");
    return true;
}
bool movePiece() {
    char fromAbc, toAbc, piece;
    int  from123, to123,
         fromAbcInt, toAbcInt,
         xPos1, yPos1,
         xPos2, yPos2;

    printf("Move your piece\n");    // Scan input
    printf("from-to\n");            //
    printf(" (a1 a2):\n  ");        // 
    scanf(                          // 
        "%c%i %c%i",                // 
        &fromAbc,                   // starting point x 
        &from123,                   // starting point y 
        &toAbc,                     // end point x 
        &to123                      // end point y 
    );                              // 
    getchar();

    fromAbcInt   = tolower(fromAbc);// Convert char to int, and make sure 
    toAbcInt     = tolower(toAbc);  // it's lowercase
    
    fromAbcInt -= 97;               // Convert from char ascii value 
    toAbcInt   -= 97;               // to 1 - 8

    yPos1 = 8-from123;
    xPos1 = fromAbcInt;
    yPos2 = 8-to123;
    xPos2 = toAbcInt;
    
    piece = chessBoard[yPos1][xPos1];

    system("cls");
    printf("\nYou moved:\n%c from %c%i to %c%i\n",piece,fromAbc,from123,toAbc,to123);
    
    // Logic ################################################################### 
        if (baseLogic(xPos1, xPos2, yPos1, yPos2, piece) == false)
        return false;

        if (friendlyFire(chessBoard[yPos2][xPos2], piece) == true)
        return false;

        // Pawns
        if (piece == WPAWN)
        if ((wPawnLogic(xPos1, xPos2, yPos1, yPos2) == false))
        return false;

        if (piece == BPAWN)
        if (bPawnLogic(xPos1, xPos2, yPos1, yPos2) == false)
        return false;
        
        // Rooks
        if ((piece == WROOK) || (piece == BROOK))
        if (rookLogic(xPos1, xPos2, yPos1, yPos2) == false)
        return false;

        // Knights
        if ((piece == WKNIG) || (piece == BKNIG))
        if (knightLogic(xPos1, xPos2, yPos1, yPos2) == false)
        return false;

        // Bishops
        if ((piece == WBISH) || (piece == BBISH))
        if (bishopLogic(xPos1, xPos2, yPos1, yPos2) == false)
        return false;

        // Queens 
        if ((piece == WQUEN) || (piece == BQUEN))
        if (queenLogic(xPos1, xPos2, yPos1, yPos2) == false)
        return false;
        // Kings
        if ((piece == WKING) || (piece == BKING))
        if (kingLogic(xPos1, xPos2, yPos1, yPos2) == false)
        return false;

    chessBoard[yPos2][xPos2] =      // Move piece from starting point
    chessBoard[yPos1][xPos1];       // to end point
    chessBoard[yPos1][xPos1] = 0;   // Leave no trace.

    return true;
}

int main(void) {
    system("cls");
    printf("\n ------=== Welcome to Chess! ===------ \n");
    printBoard();
    while (gameRunning == true) {
        movePiece();
        printBoard();

    }
    
    return 0;
}
