#ifndef CONSTANTS
#define CONSTANTS
#define W_CHUJ 9999
typedef enum {
  Pawn = 1,
  Bishop = 3,
  Knight = 4, // potem będzie podmieniane
  Rook = 5,
  Queen = 9,
  King = W_CHUJ
} Names;
typedef enum { White = 0, Black = 1 } Colors;
#endif // !
