#include "board.hpp"
#include "constants.h"
#include <stdio.h>
void pieces::promote(Names type) {
  if (this->Type == Pawn && type != Pawn)
    this->Type = type;
}
char pieces::symbol() {
  switch (this->Type) {
  case Pawn:
    return 'P';
  case Knight:
    return 'N';
  case Bishop:
    return 'B';
  case Rook:
    return 'R';
  case Queen:
    return 'Q';
  case King:
    return 'K';
  }
  return '?';
}
pieces::pieces(Names type, Colors color) {
  this->Type = type;
  this->Color = color;
}
pieces::~pieces(){};
//---------------------------------------------//
pawn_struct *chessBoard::value(short x, short y) {
  static pawn_struct *ret = new pawn_struct;
  if (this->layout[y][x]) {
    ret->name = this->layout[y][x]->Type;
    ret->color = this->layout[y][x]->Color;
    return ret;
  }
  return nullptr;
}
chessBoard::chessBoard() {
  for (int x = 0; x < 8; x++)
    for (int y = 0; y < 8; y++)
      this->layout[x][y] = nullptr;
}
chessBoard::~chessBoard() {
  for (int x = 0; x < 8; x++)
    for (int y = 0; y < 8; y++)
      this->layout[x][y]->~pieces();
}
void chessBoard::tag(short x, short y) { this->tag_[1] = x, this->tag_[0] = y; }
void chessBoard::cmdBoard(bool useColors) {

  const char BG[4][7] = {
      {"\x1b[46m"}, {"\x1b[42m"}, {"\x1b[41m"}, {"\x1b[105m"}};
  const char PC[2][8] = {{"\x1b[1;37m"}, {"\x1b[1;30m"}};
  char Symbol = ' ';
  char bgstyle;
  // flag[0][0] = -1;
  char colorek = 0;
  if (useColors) {
    printf("\x1b[44;1;31m   ");
    for (int x = 0; x < 8; x++)
      printf("\x1b[44;1;31m%c ", 'A' + x);
    puts("\x1b[0m");
    for (int x = 0; x < 8; x++) {
      printf("\x1b[44;1;31m %c \x1b[0m", '8' - x);
      for (int y = 0; y < 8; y++) {
        if (this->layout[x][y]) {
          Symbol = this->layout[x][y]->symbol();
          colorek = this->layout[x][y]->Color;
          if (colorek)
            Symbol += 32;
        } else
          Symbol = ' ';
        if (flag[0][x] & 1 << y) // do zmiany ale to potem
          bgstyle = 2;
        else
          bgstyle = (x + y) & 1;
        if (this->tag_[0] == x && this->tag_[1] == y) {
          bgstyle = 3;
          this->tag_[0] = -1;
        }
        printf("%s%s%c \x1b[0m", BG[bgstyle], PC[colorek], Symbol);
      }
      printf("\n");
    }
  } else {
    for (int x = 0; x < 8; x++) {
      for (int y = 0; y < 8; y++) {
        if (this->layout[x][y]) {
          colorek = this->layout[x][y]->Color;
          Symbol = this->layout[x][y]->symbol();
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
void chessBoard::clear(unsigned int timer) {
  const Names Based[8] = {Rook, Knight, Bishop, Queen,
                          King, Bishop, Knight, Rook};
  this->Time[0] = timer;
  this->Time[1] = timer;
  this->playing = White;
  this->Points[0] = 0;
  this->Points[1] = 0;
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
  this->tag(-1, -1);
}
void chessBoard::flag_all() {
  for (int x = 0; x < 8; x++)
    for (int y = 0; y < 8; y++)
      if (this->layout[y][x])
        this->_flag(x, y);
}

void chessBoard::flagforme(short x, short y) {
  _flag(x, y);
  cmdBoard(true);
}
void chessBoard::name(short x, short y) {
  if (this->layout[y][x])
    printf(">>> %c \n", this->layout[y][x]->symbol());
  else
    printf(">>> none");
}
void chessBoard::_flag(const short x, const short y) {
  pieces const *piece = layout[y][x];
  switch (piece->Type) {
  case Pawn:
    if (y + 1 < 8 && piece->Color) {
      if (x + 1 < 8)
        this->flag[piece->Color][y + 1] |= 1 << (x + 1);
      if (x - 1 > -1)
        this->flag[piece->Color][y + 1] |= 1 << (x - 1);
    } else if (y - 1 > -1 && !piece->Color) {
      if (x + 1 < 8)
        this->flag[piece->Color][y - 1] |= 1 << (x + 1);
      if (x - 1 > -1)
        this->flag[piece->Color][y - 1] |= 1 << (x - 1);
    }
    break;
  case Knight:
    if (y + 1 < 8) {
      if (x + 2 < 8)
        this->flag[piece->Color][y + 1] |= 1 << (x + 2);
      if (x - 2 > -1)
        this->flag[piece->Color][y + 1] |= 1 << (x - 2);
    }
    if (y - 1 > -1) {
      if (x + 2 < 8)
        this->flag[piece->Color][y - 1] |= 1 << (x + 2);
      if (x - 2 > -1)
        this->flag[piece->Color][y - 1] |= 1 << (x - 2);
    }
    if (y + 2 < 8) {
      if (x + 1 < 8)
        this->flag[piece->Color][y + 2] |= 1 << (x + 1);
      if (x - 1 > -1)
        this->flag[piece->Color][y + 2] |= 1 << (x - 1);
    }
    if (y - 2 > -1) {
      if (x + 1 < 8)
        this->flag[piece->Color][y - 2] |= 1 << (x + 1);
      if (x - 1 > -1)
        this->flag[piece->Color][y - 2] |= 1 << (x - 1);
    }
    break;
  case Queen:
  case Bishop:
    for (int m = x + 1, n = y + 1; m < 8 && n < 8; m++, n++) {
      this->flag[piece->Color][n] |= 1 << (m);
      if (this->layout[n][m])
        break;
    }
    for (int m = x - 1, n = y - 1; m > -1 && n > -1; m--, n--) {
      this->flag[piece->Color][n] |= 1 << (m);
      if (this->layout[n][m])
        break;
    }
    for (int m = x + 1, n = y - 1; m < 8 && n > -1; m++, n--) {
      this->flag[piece->Color][n] |= 1 << (m);
      if (this->layout[n][m])
        break;
    }
    for (int m = x - 1, n = y + 1; m > -1 && n < 8; m--, n++) {
      this->flag[piece->Color][n] |= 1 << (m);
      if (this->layout[n][m])
        break;
    }
    if (piece->Type == Bishop)
      break;
  case Rook:
    for (int m = x + 1; m < 8; m++) { // prawo
      this->flag[piece->Color][y] |= 1 << (m);
      if (this->layout[y][m])
        break;
    }
    for (int m = x - 1; m > -1; m--) { // lewo
      this->flag[piece->Color][y] |= 1 << (m);
      if (this->layout[y][m])
        break;
    }
    for (int m = y + 1; m < 8; m++) { // góra
      this->flag[piece->Color][m] |= 1 << (x);
      if (this->layout[m][x])
        break;
    }
    for (int m = (y - 1); m > -1; m--) { // dół
      this->flag[piece->Color][m] |= 1 << (x);
      if (this->layout[m][x])
        break;
    }
    break;
  case King:
    break;
  }
}
