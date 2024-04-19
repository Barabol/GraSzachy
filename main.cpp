#include "./src/board.hpp"
int main() {
  chessBoard board;
  board.clear();
  board.tag(-1, -1);
  board.cmdBoard(true);
  board.flag_all();
  board.cmdBoard(true);
}
