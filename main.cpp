#include <stdio.h>
class pieces {
public:
  pieces() {}
  ~pieces() {}
};
class chessBoard {
public:
  chessBoard() {
    for (int x = 0; x < 8; x++)
      for (int y = 0; y < 8; y++)
        this->layout[x][y] = nullptr;
  }
  void clear() {
    for (int x = 0; x < 8; x++)
      for (int y = 0; y < 8; y++) {
        if (this->layout[x][y]) {
          this->layout[x][y]->~pieces();
        }
        this->layout[x][y] = new pieces();
      }
  }

private:
  pieces *layout[8][8];
};
int main() { printf("a"); }
