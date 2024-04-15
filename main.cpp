#include "./src/constants.h"
#include <stdio.h>
class pieces {
public:
  Names Type;
  Colors Color;
  char notMoved = 1;
  void promote(Names type) {
    if (this->Type == Pawn && type != Pawn)
      this->Type = type;
  }
  char value() {
    if (this->Type == Bishop)
      return 3;
    return this->Type;
  }
  pieces(Names type, Colors color) {
    this->Type = type;
    this->Color = color;
  }
  ~pieces() {}

private:
};
class chessBoard {
public:
  chessBoard() {
    for (int x = 0; x < 8; x++)
      for (int y = 0; y < 8; y++)
        this->layout[x][y] = nullptr;
  }
  ~chessBoard() {
    for (int x = 0; x < 8; x++)
      for (int y = 0; y < 8; y++)
        this->layout[x][y]->~pieces();
  }
  void cmdBoard(bool useColors) {

    const char BG[2][7] = {{"\x1b[46m"}, {"\x1b[42m"}};
    const char PC[2][6] = {{"\x1b[37m"}, {"\x1b[30m"}};
    char Symbol = ' ';
    char colorek = 0;
    if (useColors) {
      for (int x = 0; x < 8; x++) {
        for (int y = 0; y < 8; y++) {
          if (this->layout[x][y]) {
            switch (this->layout[x][y]->Type) {
            case Pawn:
              Symbol = 'P';
              break;
            case Knight:
              Symbol = 'N';
              break;
            case Bishop:
              Symbol = 'B';
              break;
            case Rook:
              Symbol = 'R';
              break;
            case Queen:
              Symbol = 'Q';
              break;
            case King:
              Symbol = 'K';
              break;
            }
            colorek = this->layout[x][y]->Color;
            if (colorek)
              Symbol += 32;
          } else
            Symbol = ' ';
          printf("%s%s%c \x1b[0m", BG[(x + y) & 1], PC[colorek], Symbol);
        }
        printf("\n");
      }
    } else {
      for (int x = 0; x < 8; x++) {
        for (int y = 0; y < 8; y++) {
          if (this->layout[x][y]) {
            switch (this->layout[x][y]->Type) {
            case Pawn:
              Symbol = 'P';
              break;
            case Knight:
              Symbol = 'N';
              break;
            case Bishop:
              Symbol = 'B';
              break;
            case Rook:
              Symbol = 'R';
              break;
            case Queen:
              Symbol = 'Q';
              break;
            case King:
              Symbol = 'K';
              break;
            }
            colorek = this->layout[x][y]->Color;
            if (colorek)
              Symbol += 32;
          } else
            Symbol = ' ';
          printf("%c ", Symbol);
        }
        printf("\n");
      }
    }
  }
  void clear() {
    const Names Based[8] = {Rook, Knight, Bishop, Queen,
                            King, Bishop, Knight, Rook};
    for (int x = 0; x < 8; x++)
      for (int y = 0; y < 8; y++) {
        if (this->layout[x][y]) {
          this->layout[x][y]->~pieces();
          this->layout[x][y] = nullptr;
        }
      }
    for (int x = 0; x < 8; x++) {
      for (int y = 0; y < 2; y++)
        flag[y][x] = 0;
      this->layout[0][x] = new pieces(Based[x], Black);
      this->layout[1][x] = new pieces(Pawn, Black);
      this->layout[6][x] = new pieces(Pawn, White);
      this->layout[7][x] = new pieces(Based[x], White);
    }
  }

private:
  pieces *layout[8][8];
  char flag[2][8];
};
int main() {
  chessBoard board;
  board.clear();
  board.cmdBoard(true);
  board.cmdBoard(false);
}
