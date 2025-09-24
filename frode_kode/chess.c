/**
 * Chess!
 * @file chess.c
 * @author Mats K. - mats.herman.knutsen@gmail.com  og FrodeH
 * @date 20/09/2025
 */


 //  NB:  Y-retning/1.aksen  er nedover  (8 --> 1)  og
 //       X-retning/2.aksen  er bortover (a --> h)


#include <stdio.h>
#include <ctype.h>
#include <math.h>
#include <stdbool.h>


enum Brikker  {  HBonde, HTaarn, HHest, HLoper, HDronning, HKonge, Ledig,
                 SBonde, STaarn, SHest, SLoper, SDronning, SKonge, };
enum Spiller  {  Hvit, Svart };

const char tegn[] = "BTHLDK bthldk";

const int    TOPL = 218, TOPM = 194, TOPR = 191, // ┌ ┬ ┐
             MIDL = 195, MIDM = 197, MIDR = 180, // ├ ┼ ┤
             BOTL = 192, BOTM = 193, BOTR = 217, // └ ┴ ┘
             BARH = 196, BARV = 179;             // ─ │

// Variables:
enum Brikker chessBoard[8][8] = {
    {STaarn, SHest,  SLoper, SDronning, SKonge,  SLoper,  SHest,   STaarn  },
    {SBonde, SBonde, SBonde, SBonde,    SBonde,  SBonde,  SBonde,  SBonde },
    {Ledig,  Ledig,  Ledig,  Ledig,     Ledig,   Ledig,   Ledig,   Ledig  },
    {Ledig,  Ledig,  Ledig,  Ledig,     Ledig,   Ledig,   Ledig,   Ledig  },
    {Ledig,  Ledig,  Ledig,  Ledig,     Ledig,   Ledig,   Ledig,   Ledig  },
    {Ledig,  Ledig,  Ledig,  Ledig,     Ledig,   Ledig,   Ledig,   Ledig  },
    {HBonde, HBonde, HBonde, HBonde,    HBonde,  HBonde,  HBonde,  HBonde },
    {HTaarn, HHest,  HLoper, HDronning, HKonge,  HLoper,  HHest,   HTaarn } };


enum Spiller spiller;
bool gameRunning = true;


int printBoard() {
    printf("\n\n\n     a   b   c   d   e   f   g   h  \n");
    printf("   %c", TOPL);                          // Top border line
    for (int i = 0; i < 7; i++)
        printf("%c%c%c%c", BARH, BARH, BARH, TOPM);
    printf("%c%c%c%c", BARH, BARH, BARH, TOPR);

    for (int i = 0; i < 8; i++)  {
        printf("\n %i ", 8-i);                      // Numbers
        for (int j = 0; j < 8; j++) {               // Line that contains
            printf("%c  %c",BARV, tegn[(int) chessBoard[i][j]]);
        }                                           //
        printf("%c %i\n   %c", BARV,8-i, MIDL);     // Separation line
        for (int j = 0; j < 8; j++)                 //
            printf("%c%c%c%c", BARH, BARH, BARH, MIDM);
        printf("\b%c", MIDR);                       //
    }
    for (int j = 0; j < 64; j++) printf("\b");
    printf("   %c", BOTL);                          // Botton border line
    for (int j = 0; j < 8; j++) printf("%c%c%c%c", BARH, BARH, BARH, BOTM);
    printf("\b%c", BOTR);
    printf("\n     a   b   c   d   e   f   g   h  \n");
    return 0;
}


bool realMove(const char* t, const enum Brikker brikke,
              const int xFra, const int yFra, const int xTil, const int yTil)  {
  int x1, x2, y1, y2, dx, dy;

  if (yFra == yTil)  {                     //  Vannrett på samme rad:
     x1 = (xFra < xTil) ? xFra : xTil;     //  'x1' er den lengst til venstre.
     x2 = (xFra < xTil) ? xTil : xFra;     //  'x2' er den lengst til høyre.
     for (int i = x1+1;  i < x2;  i++)
         if (chessBoard[yFra][i] != Ledig)
            {  printf("\t\t\t\t\tFeil %s-flytt!\n", t);  return false;  }
  } else if (xFra == xTil)  {             //  Loddrett på samme kolonne:
     y1 = (yFra < yTil) ? yFra : yTil;    //  y1' er den øverste.
     y2 = (yFra < yTil) ? yTil : yFra;    //  y1' er den nederste.
     for (int i = y1+1;  i < y2;  i++)
         if (chessBoard[i][xFra] != Ledig)
            {  printf("\t\t\t\t\tFeil %s-flytt!\n", t);  return false;  }
  } else  {                                //  Diagonalt en eller annen vei:
     if (xFra < xTil  &&  yFra < yTil)          //  Hoveddiagonal ned:
        {  x1 = xFra;  x2 = xTil;  y1 = yFra;  y2 = yTil;  dx = 1;  dy = 1; }
     else if (xFra > xTil  &&  yFra > yTil)     //  Hoveddiagonal opp:
        {  x1 = xTil;  x2 = xFra;  y1 = yTil;  y2 = yFra;  dx = 1;  dy = 1; }
     else if (xFra > xTil  &&  yFra < yTil)     //  Bidiagonal ned:
        {  x1 = xFra;  x2 = xTil;  y1 = yFra;  y2 = yTil;  dx = -1;  dy = 1; }
     else if (xFra < xTil  &&  yFra < yTil)     //  Bidiagonal opp:
        {  x1 = xTil;  x2 = xFra;  y1 = yTil;  y2 = yFra;  dx = -1;  dy = 1; }
                                    //  Sjekker at ingen brikker på veien fra
    for (int i = x1+dx, j = y1+dy;  i != x2  &&  j != y2;  i += dx, j += dy)
        if (chessBoard[j][i] != Ledig)   //    start- til sluttpunkt.
           {  printf("\t\t\t\t\tFeil %s-flytt!\n", t);  return false;  }
  }

  chessBoard[yFra][xFra] = Ledig;                  //  Flytter brikken:
  chessBoard[yTil][xTil] = brikke;
  return true;
}


bool flyttBonde(const int xFra, const int yFra, const int xTil, const int yTil)  {
  bool ok = true;;
  if (xFra == xTil)  {                              //  Samme kolonne:
                      //  Bonde flyttes max.2 ruter i rett retning ift. farge:
     if (spiller == Hvit) {                         //  Hvit flytter:
        if (yTil == yFra-1)  {                      //  Skal EN rute frem:
           if (chessBoard[yTil][xTil] != Ledig)    ok = false;  //  Målrute opptatt.
        } else if (yTil == yFra-2)  {               //  Skal TO ruter frem:
            if (chessBoard[yTil][xTil]   != Ledig  ||           //  Minst en av de to
                chessBoard[yTil+1][xTil] != Ledig) ok = false;  //    rutene foran er opptatt.
        } else  ok = false;                         //  Ulovlig antall steg frem.
     } else  {                                      //  Svart flytter:
        if (yTil == yFra+1)  {                      //  Skal EN rute frem:
           if (chessBoard[yTil][xTil] != Ledig)    ok = false;  //  Målrute opptatt.
        } else if (yTil == yFra+2)  {               //  Skal TO ruter frem:
            if (chessBoard[yTil][xTil]   != Ledig  ||           //  Minst en av de to
                chessBoard[yTil-1][xTil] != Ledig) ok = false;  //    rutene foran er opptatt.
        } else  ok = false;                         //  Ulovlig antall steg frem.
     }
  } else  ok = false;   //  Flytt foregår IKKE innen samme kolonne.

  if (!ok) {  printf("\t\t\t\t\tFeil Bonde-flytt!\n");  return false;  }

  chessBoard[yFra][xFra] = Ledig;                   //  Flytter brikken:
  chessBoard[yTil][xTil] = (spiller == Hvit) ? HBonde : SBonde;
  return true;

//  LATER:  1) Slå på skrå til sidene foran seg
//          2) En passant
//          3) Blir til dronning (evt. annen brikke)
}


bool flyttTaarn(const int xFra, const int yFra, const int xTil, const int yTil)  {
  enum Brikker brikke = (spiller == Hvit) ? HTaarn : STaarn;
  if (xFra == xTil  ||  yFra == yTil)          //  På samme kolonne eller rad:
     return (realMove("Taarn", brikke, xFra, yFra, xTil, yTil));
  else
    printf("\t\t\t\t\tFeil Taarn-flytt!\n");
  return false;
}


bool flyttHest(const int xFra, const int yFra, const int xTil, const int yTil)  {
                                             //  Lovlig hesteflytt (2+1, 1+2):
 if (abs(xFra-xTil) + abs(yFra-yTil) == 3  &&  xFra != xTil  &&  yFra != yTil)  {
    chessBoard[yFra][xFra] = Ledig;
    chessBoard[yTil][xTil] = (spiller == Hvit) ? HHest : SHest;
    return true;
 } else
    printf("\t\t\t\t\tFeil Hest-flytt!\n");
 return false;
}


bool flyttLoper(const int xFra, const int yFra, const int xTil, const int yTil)  {
  enum Brikker brikke = (spiller == Hvit) ? HLoper : SLoper;
  if (abs(xFra-xTil) ==  abs(yFra-yTil))           //  På en korrekt diagonal:
     return (realMove("Loper", brikke, xFra, yFra, xTil, yTil));
  else
    printf("\t\t\t\t\tFeil Loper-flytt!\n");
  return false;
}


bool flyttDronning(const int xFra, const int yFra, const int xTil, const int yTil)  {
  enum Brikker brikke = (spiller == Hvit) ? HDronning : SDronning;
                                      //  Vannrett, loddrett eller diagonalt:
  if (xFra == xTil  ||  yFra == yTil  || abs(xFra-xTil) ==  abs(yFra-yTil))
     return (realMove("Dronning", brikke, xFra, yFra, xTil, yTil));
  else
    printf("\t\t\t\t\tFeil Dronning-flytt!\n");
  return false;
}


bool flyttKonge(const int xFra, const int yFra, const int xTil, const int yTil)  {
  if (abs(xFra-xTil) > 1  ||  abs(yFra-yTil) > 1)
     {  printf("\t\t\t\t\tFeil Konge-flytt!\n");  return false;  }

  chessBoard[yFra][xFra] = Ledig;
  chessBoard[yTil][xTil] = (spiller == Hvit) ? HKonge : SKonge;
  return true;

//  LATER:  1) Rokering
//          2) Ikke sette seg selv i "sjakk"
//          3) Står i matt, så remis om ikke andre alternative flytt finnes
}


void movePiece() {
  char fra, til;
  int  xFra, xTil, yFra, yTil;
  bool feil, ok;

  do {
    do  {
      do  {                          //  Leser inntil "lovlig" trekk A-H, 1-8:
        if (spiller == Hvit) printf("\nHVIT "); else printf("\nSVART ");
        printf("i trekket (fra-til f.eks a1 a2):  ");
        scanf(" %c %i %c %i", &fra, &yFra, &til, &yTil);   getchar();
        fra = toupper(fra);   til = toupper(til);
      } while (fra < 'A'  ||  fra > 'H'  ||  til < 'A'  ||  til > 'H'  ||
               yFra < 1   ||  yFra > 8   ||  yTil < 1   ||  yTil > 8);

      yFra = 8-yFra;   yTil = 8-yTil;   xFra = fra-'A';   xTil = til-'A';

      feil = false;
      if (chessBoard[yFra][xFra] == Ledig)  {    //  Tom fra-rute:
         printf("\t\t\t\t\tUmulig aa flytte en tom rute!\n");
         feil = true;
      } else                         //  Motstanders brikke forsøkes flyttes:
         if ((spiller == Hvit   &&  chessBoard[yFra][xFra] > Ledig)  ||
             (spiller == Svart  &&  chessBoard[yFra][xFra] < Ledig))  {
             printf("\t\t\t\t\tUmulig aa flytte motstanders brikke!\n");
             feil = true;
      } else                         //  Kan ikke slå egen brikke:
         if  ((spiller == Hvit   &&  chessBoard[yTil][xTil] < Ledig)  ||
              (spiller == Svart  &&  chessBoard[yTil][xTil] > Ledig))  {
             printf("\t\t\t\t\tUmulig aa slaa egen brikke!\n");
             feil = true;
      }
    } while (feil);

    switch(chessBoard[yFra][xFra])  {              //  Prøver å flytte brikke:
      case HBonde:    case SBonde:    ok = flyttBonde(xFra, yFra, xTil, yTil);    break;
      case HTaarn:    case STaarn:    ok = flyttTaarn(xFra, yFra, xTil, yTil);    break;
      case HHest:     case SHest:     ok = flyttHest(xFra, yFra, xTil, yTil);     break;
      case HLoper:    case SLoper:    ok = flyttLoper(xFra, yFra, xTil, yTil);    break;
      case HDronning: case SDronning: ok = flyttDronning(xFra, yFra, xTil, yTil); break;
      case HKonge:    case SKonge:    ok = flyttKonge(xFra, yFra, xTil, yTil);    break;
    }
  } while (!ok);
}


int main(void) {

    printBoard();
    spiller = Hvit;

    while (gameRunning == true) {
        movePiece();
        printBoard();
        spiller = (spiller == Hvit) ? Svart : Hvit;
    }

    return 0;
}
