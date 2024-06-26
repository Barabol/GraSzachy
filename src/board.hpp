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
  void cmdBoard(bool useColors, Colors color);
  void clear(unsigned int timer);
  void flag_all();
  void flagforme(short x, short y);
  void findMoves(short x, short y);
  void name(short x, short y);
  pawn_struct *value(short x, short y);
  unsigned int Time[2];
  unsigned char Points[2];
  char playing;
  char moves[8];
  char move(short x, short y);
  pieces *layout[8][8];
  char selectedBefore[2];

private:
  void _flag(const short x, const short y);
  char _flag4szach(const short x, const short y);
  char simulateMove(short cX, short cY, short nX, short nY);
  char isunszachable();
  char flag[2][8];
  char tag_[2];
  char selected[2];
  char flagprototype[8];
  char szach[2];
  char Kings[2][2];
};
#endif
