#include "./src/board.hpp"
#include "src/constants.h"
#include <allegro5/allegro.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/bitmap.h>
#include <allegro5/bitmap_draw.h>
#include <allegro5/bitmap_io.h>
#include <allegro5/color.h>
#include <allegro5/display.h>
#include <allegro5/events.h>
#include <allegro5/keyboard.h>
#include <allegro5/mouse.h>
#include <allegro5/timer.h>
#include <cstdio>
void render(chessBoard *board, ALLEGRO_BITMAP *pieces[2][6],
            ALLEGRO_BITMAP *mark, ALLEGRO_FONT *font) {
  pawn_struct *pwn;
  static const ALLEGRO_COLOR colors[2] = {al_map_rgb(255, 0, 0),
                                          al_map_rgb(0, 255, 0)};
  al_clear_to_color(al_map_rgb(5, 5, 5));
  for (int x = 0; x < 8; x++)
    for (int y = 7; y > -1; y--) {
      pwn = board->value(x, y);
      al_draw_filled_rectangle(
          BUTTON_SIZE * x, BUTTON_SIZE * y, BUTTON_SIZE * x + BUTTON_SIZE,
          BUTTON_SIZE * y + BUTTON_SIZE, colors[(x + y) & 1]);
      if (pwn)
        al_draw_bitmap(pieces[pwn->color][pwn->name], BUTTON_SIZE * x,
                       BUTTON_SIZE * y, 0);
      if (board->moves[y] & 1 << (x))
        al_draw_bitmap(mark, BUTTON_SIZE * x, BUTTON_SIZE * y, 0);
    }

  al_draw_textf(font, al_map_rgb(0, 255, 0), 800, 30, 0, "Timer: %d:%d",
                (board->Time[Black] / 60), (board->Time[Black] % 60));
  al_draw_textf(font, al_map_rgb(0, 255, 0), 800, 90, 0, "Punkty: %d",
                board->Points[Black]);
  al_draw_textf(font, al_map_rgb(0, 255, 0), 800, 570, 0, "Punkty: %d",
                board->Points[White]);
  al_draw_textf(font, al_map_rgb(0, 255, 0), 800, 640, 0, "Timer: %d:%d",
                board->Time[White] / 60, board->Time[White] % 60);
  al_flip_display();
}
int main() {
  chessBoard board;
  board.clear(1200);
  board.flag_all();
  board.cmdBoard(true, White);
  board.cmdBoard(true, Black);
  al_init();
  al_install_mouse();
  al_install_keyboard();
  al_init_image_addon();
  al_init_font_addon();
  al_init_ttf_addon();
  al_install_audio();
  ALLEGRO_DISPLAY *display = al_create_display(WINDOW_W, WINDOW_H);

  ALLEGRO_BITMAP *mark = al_load_bitmap("src/resorces/graphics/mark.png");
  ALLEGRO_BITMAP *pieces[2][6] = {
      {al_load_bitmap("src/resorces/graphics/wp.png"),
       al_load_bitmap("src/resorces/graphics/wb.png"),
       al_load_bitmap("src/resorces/graphics/wn.png"),
       al_load_bitmap("src/resorces/graphics/wr.png"),
       al_load_bitmap("src/resorces/graphics/wq.png"),
       al_load_bitmap("src/resorces/graphics/wk.png")},
      {al_load_bitmap("src/resorces/graphics/bp.png"),
       al_load_bitmap("src/resorces/graphics/bb.png"),
       al_load_bitmap("src/resorces/graphics/bn.png"),
       al_load_bitmap("src/resorces/graphics/br.png"),
       al_load_bitmap("src/resorces/graphics/bq.png"),
       al_load_bitmap("src/resorces/graphics/bk.png")}};

  for (int x = 0; x < 6; x++)
    for (int y = 0; y < 2; y++)
      if (!pieces[y][x])
        printf("%d,%d <-- ni ma textury )-;\n", x, y);

  ALLEGRO_FONT *font = al_load_font("./src/resorces/fonts/Sans.ttf", 30, 0);
  ALLEGRO_EVENT_QUEUE *queue = al_create_event_queue();
  ALLEGRO_TIMER *timer = al_create_timer(1.00 / FRAMES);
  al_start_timer(timer);
  ALLEGRO_EVENT event;
  al_register_event_source(queue, al_get_timer_event_source(timer));
  al_register_event_source(queue, al_get_keyboard_event_source());
  al_register_event_source(queue, al_get_display_event_source(display));
  al_register_event_source(queue, al_get_mouse_event_source());
  char active = 1;
  char pframes = 0;
  pawn_struct *pawn;
  short x, y;
  while (active) {
    al_wait_for_event(queue, &event);
    switch (event.type) {
    case ALLEGRO_EVENT_TIMER:
      pframes++;
      if (pframes == FRAMES) {
        pframes = 0;
        board.Time[board.playing]--;
      }
      render(&board, pieces, mark, font);
      break;
    case 10:
      switch (event.keyboard.keycode) {
      case 59:
        active = 0;
        break;
      }
      break;
    case 20: // move
      break;
    case 21: // click
      x = (int)(event.mouse.x / BUTTON_SIZE);
      y = (int)(event.mouse.y / BUTTON_SIZE);
      printf("%d = %d\n", (int)(event.mouse.x / BUTTON_SIZE),
             (int)(event.mouse.y / BUTTON_SIZE));
      board.tag((int)(event.mouse.x / BUTTON_SIZE),
                (int)(event.mouse.y / BUTTON_SIZE));
      printf("<%d>\n", board.moves[y] & 1 << x);
      if (board.moves[y] & 1 << x)
        board.move(x, y);
      pawn = board.value((int)(event.mouse.x / BUTTON_SIZE),
                         (int)(event.mouse.y / BUTTON_SIZE));
      board.findMoves((int)(event.mouse.x / BUTTON_SIZE),
                      (int)(event.mouse.y / BUTTON_SIZE));
      board.flag_all();
      board.cmdBoard(true, White);
      board.cmdBoard(true, Black);
      if (pawn)
        printf("%d , %d \n", pawn->color, pawn->name);
      break;
    case 42:
      active = 0;
      break;
    default:
      // printf("> %d\n", event.type);
      break;
    }
  }
  al_destroy_font(font);
  for (int x = 0; x < 12; x++)
    al_destroy_bitmap(pieces[0][x]);
  al_destroy_bitmap(mark);
  al_uninstall_audio();
  al_destroy_display(display);
  al_uninstall_mouse();
  al_uninstall_keyboard();
  al_destroy_timer(timer);
  al_destroy_event_queue(queue);
}
