#include "constants.h"

#ifndef BOARDCLASSES
#define BOARDCLASSES
class pieces {
public:
  Names Type;
  Colors Color;
  char notMoved = 1;
  void promote(Names type);
  char symbol();
  pieces(Names type, Colors color);
  ~pieces();
};
class chessBoard {
public:
  chessBoard();
  ~chessBoard();
  void tag(short x, short y);
  void cmdBoard(bool useColors);
  void clear(unsigned int timer);
  void flag_all();
  void flagforme(short x, short y);
  void name(short x, short y);
  pawn_struct *value(short x, short y);
  unsigned int Time[2];
  unsigned char Points[2];
  char playing;

private:
  void _flag(const short x, const short y);
  pieces *layout[8][8];
  char flag[2][8];
  char tag_[2];
};
#endif
