#ifndef CONSTANTS
#define CONSTANTS

typedef enum {
  Pawn = 0,
  Bishop = 1,
  Knight = 2, // potem będzie podmieniane
  Rook = 3,
  Queen = 4,
  King = 5
} Names;
typedef enum { White = 0, Black = 1 } Colors;
#define BUTTON_SIZE 90
#define WINDOW_W 1280
#define WINDOW_H 720
#define FRAMES 30
typedef struct {
  Colors color;
  Names name;
} pawn_struct;
extern pawn_struct PAWNER;
#endif // !
