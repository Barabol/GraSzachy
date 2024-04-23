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
void chessBoard::findMoves(short x, short y) {
  pieces const *piece = layout[y][x];
  if (!piece || piece->Color != this->playing)
    return;
  selected[0] = x;
  selected[1] = y;
  for (int X = 0; X < 8; X++)
    this->moves[X] = 0;
  switch (piece->Type) {
  case Pawn:
    if (!this->layout[y][x]
             ->Color) { // zminimalizować ilość if-ów // pierwsze 2 spr. 1
                        // pozycji na starcie reszta to już spr. na oś Y
      if (this->layout[y][x]->notMoved && !this->layout[y - 1][x] &&
          !this->layout[y - 2][x])
        this->moves[y - 2] |= 1 << (x);
      if (y - 1 > -1 && !this->layout[y - 1][x])
        this->moves[y - 1] |= 1 << (x);
      if (y - 1 > -1 && x + 1 < 8 && this->layout[y - 1][x + 1])
        this->moves[y - 1] |= 1 << (x + 1);
      if (y - 1 > -1 && x - 1 > -1 && this->layout[y - 1][x - 1])
        this->moves[y - 1] |= 1 << (x - 1);
    } else {
      if (this->layout[y][x]->notMoved && !this->layout[y + 1][x] &&
          !this->layout[y + 2][x])
        this->moves[y + 2] |= 1 << (x);
      if (y + 1 < 8 && !this->layout[y + 1][x])
        this->moves[y + 1] |= 1 << (x);
      if (y + 1 < 8 && x + 1 < 8 && this->layout[y + 1][x + 1])
        this->moves[y + 1] |= 1 << (x + 1);
      if (y + 1 < 8 && x - 1 > -1 && this->layout[y + 1][x - 1])
        this->moves[y + 1] |= 1 << (x - 1);
    }
    break;
  case Knight:
    if (y + 1 < 8) {
      if (x + 2 < 8)
        this->moves[y + 1] |= 1 << (x + 2);
      if (x - 2 > -1)
        this->moves[y + 1] |= 1 << (x - 2);
    }
    if (y - 1 > -1) {
      if (x + 2 < 8)
        this->moves[y - 1] |= 1 << (x + 2);
      if (x - 2 > -1)
        this->moves[y - 1] |= 1 << (x - 2);
    }
    if (y + 2 < 8) {
      if (x + 1 < 8)
        this->moves[y + 2] |= 1 << (x + 1);
      if (x - 1 > -1)
        this->moves[y + 2] |= 1 << (x - 1);
    }
    if (y - 2 > -1) {
      if (x + 1 < 8)
        this->moves[y - 2] |= 1 << (x + 1);
      if (x - 1 > -1)
        this->moves[y - 2] |= 1 << (x - 1);
    }
    break;
  case Queen:
  case Bishop:
    for (int m = x + 1, n = y + 1; m < 8 && n < 8; m++, n++) {
      this->moves[n] |= 1 << (m);
      if (this->layout[n][m])
        break;
    }
    for (int m = x - 1, n = y - 1; m > -1 && n > -1; m--, n--) {
      this->moves[n] |= 1 << (m);
      if (this->layout[n][m])
        break;
    }
    for (int m = x + 1, n = y - 1; m < 8 && n > -1; m++, n--) {
      this->moves[n] |= 1 << (m);
      if (this->layout[n][m])
        break;
    }
    for (int m = x - 1, n = y + 1; m > -1 && n < 8; m--, n++) {
      this->moves[n] |= 1 << (m);
      if (this->layout[n][m])
        break;
    }
    if (piece->Type == Bishop)
      break;
  case Rook:
    for (int m = x + 1; m < 8; m++) { // prawo
      this->moves[y] |= 1 << (m);
      if (this->layout[y][m])
        break;
    }
    for (int m = x - 1; m > -1; m--) { // lewo
      this->moves[y] |= 1 << (m);
      if (this->layout[y][m])
        break;
    }
    for (int m = y + 1; m < 8; m++) { // góra
      this->moves[m] |= 1 << (x);
      if (this->layout[m][x])
        break;
    }
    for (int m = (y - 1); m > -1; m--) { // dół
      this->moves[m] |= 1 << (x);
      if (this->layout[m][x])
        break;
    }
    break;
  case King: // KRÓL MUSI SPRAWDZAĆ CZY TO NIE JEGO KOLOR TU BO TAK
             // sprawdzanie szacha będzie banalne sprawdzić czy moves króla
             // równe 0 i czy jest jego pozycja na flag od przeciwnika i
             // szukanko pojedynczych figur przeciwnika ktore go szachoja i czy
             // sa sflagowane przez gracza
    this->flag_all();
    if (x + 1 < 8) {
      if (y + 1 < 8 && !(this->flag[~(piece->Color) & 1][y + 1] & 1 << (x + 1)))
        if ((this->layout[y + 1][x + 1] &&
             this->layout[y + 1][x + 1]->Color != this->layout[y][x]->Color) ||
            !this->layout[y + 1][x + 1])
          this->moves[y + 1] |= 1 << (x + 1);
      if (y - 1 > -1 &&
          !(this->flag[~(piece->Color) & 1][y - 1] & 1 << (x + 1)))
        if ((this->layout[y - 1][x + 1] &&
             this->layout[y - 1][x + 1]->Color != this->layout[y][x]->Color) ||
            !this->layout[y - 1][x + 1])
          this->moves[y - 1] |= 1 << (x + 1);
      if (!(this->flag[~(piece->Color) & 1][y] & 1 << (x + 1)))
        if ((this->layout[y][x + 1] &&
             this->layout[y][x + 1]->Color != this->layout[y][x]->Color) ||
            !this->layout[y][x + 1])
          this->moves[y] |= 1 << (x + 1);
    }
    if (x - 1 > -1) {
      if (y + 1 < 8 && !(this->flag[~(piece->Color) & 1][y + 1] & 1 << (x - 1)))
        if ((this->layout[y + 1][x - 1] &&
             this->layout[y + 1][x - 1]->Color != this->layout[y][x]->Color) ||
            !this->layout[y + 1][x - 1])
          this->moves[y + 1] |= 1 << (x - 1);
      if (y - 1 > -1 &&
          !(this->flag[~(piece->Color) & 1][y - 1] & 1 << (x - 1)))
        if ((this->layout[y - 1][x - 1] &&
             this->layout[y - 1][x - 1]->Color != this->layout[y][x]->Color) ||
            !this->layout[y - 1][x - 1])
          this->moves[y - 1] |= 1 << (x - 1);
      if (!(this->flag[~(piece->Color) & 1][y] & 1 << (x - 1)))
        if ((this->layout[y][x - 1] &&
             this->layout[y][x - 1]->Color != this->layout[y][x]->Color) ||
            !this->layout[y][x - 1])
          this->moves[y] |= 1 << (x - 1);
    }
    if (y + 1 < 8 && !(this->flag[~(piece->Color) & 1][y + 1] & 1 << (x)))
      if ((this->layout[y + 1][x] &&
           this->layout[y + 1][x]->Color != this->layout[y][x]->Color) ||
          !this->layout[y + 1][x])
        this->moves[y + 1] |= 1 << (x);
    if (y - 1 > -1 && !(this->flag[~(piece->Color) & 1][y - 1] & 1 << (x)))
      if ((this->layout[y - 1][x] &&
           this->layout[y - 1][x]->Color != this->layout[y][x]->Color) ||
          !this->layout[y - 1][x])
        this->moves[y - 1] |= 1 << (x);
    break;
  }
}
void chessBoard::_flag4szach(
    const short x, const short y) { // tu jest coś nie tak chyba nie reternuje
                                    // //tia coś się nie imie
  const pieces *piece = this->layout[y][x];
  puts("???");
  this->cmdBoard(true, White);
  for (int z = 0; z < 8; z++)
    this->flagprototype[z] = 0;
  switch (piece->Type) {
  case Queen:
  case Bishop:
    for (int m = x + 1, n = y + 1; m < 8 && n < 8; m++, n++) {
      this->flagprototype[n] |= 1 << (m);
      if (this->layout[n][m]) {
        if (this->layout[n][m]->Type == King &&
            this->layout[n][m]->Color != this->layout[y][x]->Color) {
          for (int z = 0; z < 8; z++)
            this->flag[(~this->playing) & 1][z] = this->flagprototype[z];
          return;
        } else
          for (int z = 0; z < 8; z++)
            this->flagprototype[z] = 0;
      }
    }
    for (int m = x - 1, n = y - 1; m > -1 && n > -1; m--, n--) {
      this->flagprototype[n] |= 1 << (m);
      if (this->layout[n][m]) {
        if (this->layout[n][m]->Type == King &&
            this->layout[n][m]->Color != this->layout[y][x]->Color) {
          for (int z = 0; z < 8; z++)
            this->flag[(~this->playing) & 1][z] = this->flagprototype[z];
          return;
        } else
          for (int z = 0; z < 8; z++)
            this->flagprototype[z] = 0;
      }
    }
    for (int m = x + 1, n = y - 1; m < 8 && n > -1; m++, n--) {
      this->flagprototype[n] |= 1 << (m);
      if (this->layout[n][m]) {
        if (this->layout[n][m]->Type == King &&
            this->layout[n][m]->Color != this->layout[y][x]->Color) {
          for (int z = 0; z < 8; z++)
            this->flag[(~this->playing) & 1][z] = this->flagprototype[z];
          return;
        } else
          for (int z = 0; z < 8; z++)
            this->flagprototype[z] = 0;
      }
    }
    for (int m = x - 1, n = y + 1; m > -1 && n < 8; m--, n++) {
      this->flagprototype[n] |= 1 << (m);
      if (this->layout[n][m]) {
        if (this->layout[n][m]->Type == King &&
            this->layout[n][m]->Color != this->layout[y][x]->Color) {
          for (int z = 0; z < 8; z++)
            this->flag[(~this->playing) & 1][z] = this->flagprototype[z];
          return;
        } else
          for (int z = 0; z < 8; z++)
            this->flagprototype[z] = 0;
      }
    }
    if (piece->Type == Bishop)
      break;
  case Rook:
    for (int m = x + 1; m < 8; m++) { // prawo
      this->flagprototype[y] |= 1 << (m);
      if (this->layout[y][m]) {
        if (this->layout[y][m]->Type == King &&
            this->layout[y][m]->Color != this->layout[y][x]->Color) {
          for (int z = 0; z < 8; z++)
            this->flag[(~this->playing) & 1][z] = this->flagprototype[z];
          return;
        } else
          for (int z = 0; z < 8; z++)
            this->flagprototype[z] = 0;
      }
    }
    for (int m = x - 1; m > -1; m--) { // lewo
      this->flagprototype[y] |= 1 << (m);
      if (this->layout[y][m]) {
        if (this->layout[y][m]->Type == King &&
            this->layout[y][m]->Color != this->layout[y][x]->Color) {
          for (int z = 0; z < 8; z++)
            this->flag[(~this->playing) & 1][z] = this->flagprototype[z];
          return;
        } else
          for (int z = 0; z < 8; z++)
            this->flagprototype[z] = 0;
      }
    }
    for (int m = y + 1; m < 8; m++) { // góra
      this->flagprototype[m] |= 1 << (x);
      if (this->layout[m][x]) {
        if (this->layout[m][x]->Type == King &&
            this->layout[m][x]->Color != this->layout[y][x]->Color) {
          for (int z = 0; z < 8; z++)
            this->flag[(~this->playing) & 1][z] = this->flagprototype[z];
          return;
        } else
          for (int z = 0; z < 8; z++)
            this->flagprototype[z] = 0;
      }
    }
    for (int m = (y - 1); m > -1; m--) { // dół
      this->flagprototype[m] |= 1 << (x);
      if (this->layout[m][x]) {

        if (this->layout[m][x]->Type == King &&
            this->layout[m][x]->Color != this->layout[y][x]->Color) {
          for (int z = 0; z < 8; z++)
            this->flag[(~this->playing) & 1][z] = this->flagprototype[z];
          return;
        } else
          for (int z = 0; z < 8; z++)
            this->flagprototype[z] = 0;
      }
    }
  default:
    break;
  }
  this->cmdBoard(true, (Colors)(this->playing));
}
char chessBoard::isunszachable() {
  char spr = 0;
  int heup = 0;
  for (int x = 0; x < 8; x++) {
    spr += this->flag[(~playing) & 1][x] & this->flag[playing][x];
    heup = x;
  }
  printf("GG %d,%d\n", heup, spr);
  this->cmdBoard(true, Black);
  this->cmdBoard(true, White);
  if (spr)
    return 0;
  return 1;
}
char chessBoard::move(short x, short y) {
  static const unsigned char PValues[6] = {1, 3, 3, 5, 9, 10};
  if (this->szach[playing] &&
      this->layout[this->selected[1]][this->selected[0]]->Type != King) {
    for (int y = 0; y < 8; y++)
      this->flag[(~this->playing) & 1][y] = 0;
    _flag4szach(this->selectedBefore[0], this->selectedBefore[1]);
    printf("<%d|%d>\n", this->selectedBefore[0], this->selectedBefore[1]);
    puts("---------------------");
    this->cmdBoard(true, (Colors)((~this->playing) & 1));
    puts("---------------------");
    if ((this->selectedBefore[0] == x && this->selectedBefore[1] == y) ||
        this->flag[(~this->playing) & 1][y] & 1 << x)
      flag_all();
    else {
      flag_all();
      return 0;
    }
  }

  if (!this->layout[y][x]) {
    this->layout[y][x] = this->layout[this->selected[1]][this->selected[0]];
    this->layout[this->selected[1]][this->selected[0]] = nullptr;
  } else if (this->layout[y][x]->Color ==
             this->layout[this->selected[1]][this->selected[0]]->Color)
    return 0;
  else {
    this->Points[this->playing] += PValues[this->layout[y][x]->Type];
    this->layout[y][x]->~pieces();
    this->layout[y][x] = this->layout[this->selected[1]][this->selected[0]];
    this->layout[this->selected[1]][this->selected[0]]->~pieces();
    this->layout[this->selected[1]][this->selected[0]] = nullptr;
  }
  if (this->layout[y][x]->notMoved)
    this->layout[y][x]->notMoved = false;
  if (this->layout[y][x]->Type == King) {
    this->Kings[this->layout[y][x]->Color][0] = x;
    this->Kings[this->layout[y][x]->Color][1] = y;
  }
  this->szach[playing] = false;
  this->playing = (~this->playing) & 1;
  flag_all();
  if (this->flag[(~this->playing) & 1][this->Kings[this->playing][1]] &
      1 << this->Kings[this->playing][0]) {
    this->szach[this->playing] = true;
    this->selectedBefore[0] = x;
    this->selectedBefore[1] = y;
    for (int y = 0; y < 8; y++)
      this->flag[(~this->playing) & 1][y] = 0;
    _flag4szach(this->selectedBefore[0], this->selectedBefore[1]);
    if (!(this->flag[this->playing][y] & 1 << x) && this->isunszachable())
      return 1;
    this->flag_all();
  }
  for (int z = 0; z < 8; z++)
    this->moves[z] = 0;
  return 0;
}
void chessBoard::cmdBoard(bool useColors, Colors color) {

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
        if (flag[color][x] & 1 << y) // do zmiany ale to potem// zmienione nwm
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
  for (int x = 0; x < 8; x++)
    this->moves[x] = 0;
  this->Time[0] = timer;
  this->Time[1] = timer;
  this->Kings[White][0] = 4;
  this->Kings[White][1] = 7;
  this->Kings[Black][0] = 4;
  this->Kings[Black][1] = 0;
  this->playing = White;
  this->Points[0] = 0;
  this->Points[1] = 0;
  this->selected[0] = 0;
  this->selected[1] = 0;
  this->szach[0] = 0;
  this->szach[1] = 0;
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
    for (int y = 0; y < 8; y++) {
      this->flag[White][y] = 0;
      this->flag[Black][y] = 0;
    }
  for (int x = 0; x < 8; x++)
    for (int y = 0; y < 8; y++)
      if (this->layout[y][x])
        this->_flag(x, y);
}

void chessBoard::flagforme(short x, short y) {
  this->_flag(x, y);
  this->cmdBoard(true, White);
  this->cmdBoard(true, Black);
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
  case King: // co ja najlepszego zrobiłem?!
    if (x + 1 < 8) {
      if (y + 1 < 8 && !(this->flag[~(piece->Color) & 1][y + 1] & 1 << (x + 1)))
        if ((this->layout[y + 1][x + 1] &&
             this->layout[y + 1][x + 1]->Color != this->layout[y][x]->Color) ||
            !this->layout[y + 1][x + 1])
          this->flag[piece->Color][y + 1] |= 1 << (x + 1);
      if (y - 1 > -1 &&
          !(this->flag[~(piece->Color) & 1][y - 1] & 1 << (x + 1)))
        if ((this->layout[y - 1][x + 1] &&
             this->layout[y - 1][x + 1]->Color != this->layout[y][x]->Color) ||
            !this->layout[y - 1][x + 1])
          this->flag[piece->Color][y - 1] |= 1 << (x + 1);
      if (!(this->flag[~(piece->Color) & 1][y] & 1 << (x + 1)))
        if ((this->layout[y][x + 1] &&
             this->layout[y][x + 1]->Color != this->layout[y][x]->Color) ||
            !this->layout[y][x + 1])
          this->flag[piece->Color][y] |= 1 << (x + 1);
    }
    if (x - 1 > -1) {
      if (y + 1 < 8 && !(this->flag[~(piece->Color) & 1][y + 1] & 1 << (x - 1)))
        if ((this->layout[y + 1][x - 1] &&
             this->layout[y + 1][x - 1]->Color != this->layout[y][x]->Color) ||
            !this->layout[y + 1][x - 1])
          this->flag[piece->Color][y + 1] |= 1 << (x - 1);
      if (y - 1 > -1 &&
          !(this->flag[~(piece->Color) & 1][y - 1] & 1 << (x - 1)))
        if ((this->layout[y - 1][x - 1] &&
             this->layout[y - 1][x - 1]->Color != this->layout[y][x]->Color) ||
            !this->layout[y - 1][x - 1])
          this->flag[piece->Color][y - 1] |= 1 << (x - 1);
      if (!(this->flag[~(piece->Color) & 1][y] & 1 << (x - 1)))
        if ((this->layout[y][x - 1] &&
             this->layout[y][x - 1]->Color != this->layout[y][x]->Color) ||
            !this->layout[y][x - 1])
          this->flag[piece->Color][y] |= 1 << (x - 1);
    }
    if (y + 1 < 8 && !(this->flag[~(piece->Color) & 1][y + 1] & 1 << (x)))
      if ((this->layout[y + 1][x] &&
           this->layout[y + 1][x]->Color != this->layout[y][x]->Color) ||
          !this->layout[y + 1][x])
        this->flag[piece->Color][y + 1] |= 1 << (x);
    if (y - 1 > -1 && !(this->flag[~(piece->Color) & 1][y - 1] & 1 << (x)))
      if ((this->layout[y - 1][x] &&
           this->layout[y - 1][x]->Color != this->layout[y][x]->Color) ||
          !this->layout[y - 1][x])
        this->flag[piece->Color][y - 1] |= 1 << (x);
    break;
  }
}
