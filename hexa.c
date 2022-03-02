/*
 *  作者 芳賀七海
 *  copyright (c) 2020 芳賀七海 All rights reserved.
 */

#include <GL/glut.h>
#include <GL/glpng.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "render.h"
#include "core.h"
#include "colorcode.h"
#include "vector.h"

#define BLOCK(LABEL)

/*  ===== CONSTANT ===== */
#define FILENAME ("source.png")

#define UNIT    (32)
#define VERTEX  (6)
#define PIECE_W (64)
#define PIECE_H (56)

#define GRID_X (9)
#define GRID_Y (8)
#define PICKER_PIECE_CELL (7)

#define LAYOUT_WINDOW_PADDING (UNIT)

#define LAYOUT_IMAGE_X (LAYOUT_WINDOW_PADDING)
#define LAYOUT_IMAGE_Y (LAYOUT_WINDOW_PADDING)
#define LAYOUT_IMAGE_W (512)
#define LAYOUT_IMAGE_H (512)
#define LAYOUT_IMAGE_PADDING_HO (UNIT)
#define LAYOUT_IMAGE_PADDING_VA ((LAYOUT_IMAGE_H - PIECE_H * GRID_Y - PIECE_H / 2) / 2)

#define LAYOUT_MESSAGE_X (LAYOUT_IMAGE_X + LAYOUT_IMAGE_W)
#define LAYOUT_MESSAGE_Y (LAYOUT_IMAGE_Y)

#define LAYOUT_GRID_X (LAYOUT_IMAGE_X + LAYOUT_IMAGE_PADDING_HO)
#define LAYOUT_GRID_Y (LAYOUT_IMAGE_Y + LAYOUT_IMAGE_PADDING_VA)
#define LAYOUT_GRID_W (LAYOUT_IMAGE_W - LAYOUT_IMAGE_PADDING_HO * 2)
#define LAYOUT_GRID_H (LAYOUT_IMAGE_H - LAYOUT_IMAGE_PADDING_VA * 2)
#define LAYOUT_GRID_PADDING_HO (UNIT)
#define LAYOUT_GRID_PADDING_VA (PIECE_H / 2)

#define LAYOUT_PICKER_MARGIN_L (UNIT)
#define LAYOUT_PICKER_X (LAYOUT_IMAGE_X + LAYOUT_IMAGE_W + LAYOUT_PICKER_MARGIN_L)
#define LAYOUT_PICKER_Y (LAYOUT_WINDOW_PADDING)
#define LAYOUT_PICKER_W (PIECE_W / 4 + PIECE_W + PIECE_W / 4)
#define LAYOUT_PICKER_H (LAYOUT_IMAGE_H)

#define LAYOUT_PICKER_CURSOR_H (UNIT)
#define LAYOUT_PICKER_CURSOR_UP_Y (LAYOUT_PICKER_Y)
#define LAYOUT_PICKER_CURSOR_DOWN_Y (LAYOUT_PICKER_Y + LAYOUT_PICKER_H - LAYOUT_PICKER_CURSOR_H)

#define LAYOUT_PICKER_PIECE_CELL_H (UNIT * 2)
#define LAYOUT_PICKER_PIECE_H (LAYOUT_PICKER_PIECE_CELL_H * PICKER_PIECE_CELL)
#define LAYOUT_PICKER_PIECE_Y (LAYOUT_PICKER_CURSOR_UP_Y + LAYOUT_PICKER_CURSOR_H)

#define LAYOUT_WINDOW_W (LAYOUT_WINDOW_PADDING + LAYOUT_IMAGE_W + LAYOUT_PICKER_MARGIN_L + LAYOUT_PICKER_W + LAYOUT_WINDOW_PADDING)
#define LAYOUT_WINDOW_H (LAYOUT_WINDOW_PADDING + LAYOUT_IMAGE_H + LAYOUT_WINDOW_PADDING)

#define PICK_NONE    (-1)
#define PICK_REMOVED (-2)

/* ===== TYPE ===== */
typedef struct {
  GLuint  id;
  pngInfo info;
} Image;
void Image_read(Image *, char *);

/* ===== GLOBAL VARIABLE =====*/
enum {
  SCENE_START,
  SCENE_PLAY,
  SCENE_ASSEMBLED
} g_scene;
Puzzle    g_puzzle;
PieceList g_piece;
Image     g_image;
Image     g_message_start;
Image     g_message_assembled;
Vector    g_mouse;
Vector    g_grid;
int       g_offset;
Piece    *g_picked;
int       g_picked_idx;
enum {
  MOUSE_ON_NONE,
  MOUSE_ON_PICKER_CURSOR_UP,
  MOUSE_ON_PICKER_PIECE,
  MOUSE_ON_PICKER_CURSOR_DOWN
} g_mouse_on;

/* ===== RENDER DECLARE ===== */
RENDER_DECLARE_WINDOW(hexa, LAYOUT_WINDOW_W, LAYOUT_WINDOW_H, 3, g_scene)

RENDER_DECLARE_OBJECT(image,                   LAYOUT_IMAGE_W,  LAYOUT_IMAGE_H)

RENDER_DECLARE_OBJECT(start_message,           LAYOUT_PICKER_W, LAYOUT_IMAGE_H)
RENDER_DECLARE_OBJECT(start_window_front,      LAYOUT_WINDOW_W, LAYOUT_WINDOW_H)

RENDER_DECLARE_OBJECT(play_window_back,        LAYOUT_WINDOW_W, LAYOUT_WINDOW_H)
RENDER_DECLARE_OBJECT(play_grid,               LAYOUT_GRID_W,   LAYOUT_GRID_H)
RENDER_DECLARE_OBJECT(play_picker_cursor_up,   LAYOUT_PICKER_W, LAYOUT_PICKER_CURSOR_H)
RENDER_DECLARE_OBJECT(play_picker_piece,       LAYOUT_PICKER_W, LAYOUT_PICKER_PIECE_H)
RENDER_DECLARE_OBJECT(play_picker_cursor_down, LAYOUT_PICKER_W, LAYOUT_PICKER_CURSOR_H)
RENDER_DECLARE_OBJECT(play_picked,             LAYOUT_WINDOW_W, LAYOUT_WINDOW_H)
RENDER_DECLARE_OBJECT(play_window_front,       LAYOUT_WINDOW_W, LAYOUT_WINDOW_H)

RENDER_DECLARE_OBJECT(assembled_message,      LAYOUT_PICKER_W, LAYOUT_IMAGE_H)
RENDER_DECLARE_OBJECT(assembled_window_front, LAYOUT_PICKER_W, LAYOUT_IMAGE_H)

/* ===== PROTOTYPE ===== */
void cb_reshape(int, int);
void cb_exit(void);

void draw_hexagon(int, int);
void draw_rectangle(int, int, int);
void draw_triangle(int, int);
void draw_image(Image *);
void draw_piece(Piece *, int, Image *);

void grid(Vector *, Vector *);
int  offset_canInc(void);
int  offset_canDec(void);
void offset_inc(void);
void offset_dec(void);

void RENDER_image_display(void);
RENDER_DECLARE_OBJECT_FUNC_NULL(image, mouse);
RENDER_DECLARE_OBJECT_FUNC_NULL(image, motion);
RENDER_DECLARE_OBJECT_FUNC_NULL(image, passiveMotion);
RENDER_DECLARE_OBJECT_FUNC_NULL(image, keyboard);
RENDER_DECLARE_OBJECT_FUNC_NULL(image, special);

void RENDER_start_message_display(void);
RENDER_DECLARE_OBJECT_FUNC_NULL(start_message, mouse);
RENDER_DECLARE_OBJECT_FUNC_NULL(start_message, motion);
RENDER_DECLARE_OBJECT_FUNC_NULL(start_message, passiveMotion);
RENDER_DECLARE_OBJECT_FUNC_NULL(start_message, keyboard);
RENDER_DECLARE_OBJECT_FUNC_NULL(start_message, special);

RENDER_DECLARE_OBJECT_FUNC_NULL(start_window_front, display);
RENDER_DECLARE_OBJECT_FUNC_NULL(start_window_front, mouse);
RENDER_DECLARE_OBJECT_FUNC_NULL(start_window_front, motion);
RENDER_DECLARE_OBJECT_FUNC_NULL(start_window_front, passiveMotion);
void RENDER_start_window_front_keyboard(unsigned char, int, int);
RENDER_DECLARE_OBJECT_FUNC_NULL(start_window_front, special);

RENDER_DECLARE_OBJECT_FUNC_NULL(play_window_back, display);
void RENDER_play_window_back_mouse(int, int, int, int);
RENDER_DECLARE_OBJECT_FUNC_NULL(play_window_back, motion);
RENDER_DECLARE_OBJECT_FUNC_NULL(play_window_back, passiveMotion);
RENDER_DECLARE_OBJECT_FUNC_NULL(play_window_back, keyboard);
RENDER_DECLARE_OBJECT_FUNC_NULL(play_window_back, special);

void RENDER_play_grid_display(void);
void RENDER_play_grid_mouse(int, int, int, int);
void RENDER_play_grid_motion(int, int);
void RENDER_play_grid_passiveMotion(int, int);
void RENDER_play_grid_keyboard(unsigned char, int, int);
void RENDER_play_grid_special(int, int, int);

void RENDER_play_picker_cursor_up_display(void);
void RENDER_play_picker_cursor_up_mouse(int, int, int, int);
void RENDER_play_picker_cursor_up_motion(int, int);
void RENDER_play_picker_cursor_up_passiveMotion(int, int);
RENDER_DECLARE_OBJECT_FUNC_NULL(play_picker_cursor_up, keyboard);
RENDER_DECLARE_OBJECT_FUNC_NULL(play_picker_cursor_up, special);

void RENDER_play_picker_piece_display(void);
void RENDER_play_picker_piece_mouse(int, int, int, int);
void RENDER_play_picker_piece_motion(int, int);
void RENDER_play_picker_piece_passiveMotion(int, int);
void RENDER_play_picker_piece_keyboard(unsigned char, int, int);
void RENDER_play_picker_piece_special(int, int, int);

void RENDER_play_picker_cursor_down_display(void);
void RENDER_play_picker_cursor_down_mouse(int, int, int, int);
void RENDER_play_picker_cursor_down_motion(int, int);
void RENDER_play_picker_cursor_down_passiveMotion(int, int);
RENDER_DECLARE_OBJECT_FUNC_NULL(play_picker_cursor_down, keyboard);
RENDER_DECLARE_OBJECT_FUNC_NULL(play_picker_cursor_down, special);

void RENDER_play_picked_display(void);
RENDER_DECLARE_OBJECT_FUNC_NULL(play_picked, mouse);
RENDER_DECLARE_OBJECT_FUNC_NULL(play_picked, motion);
RENDER_DECLARE_OBJECT_FUNC_NULL(play_picked, passiveMotion);
RENDER_DECLARE_OBJECT_FUNC_NULL(play_picked, keyboard);
RENDER_DECLARE_OBJECT_FUNC_NULL(play_picked, special);

RENDER_DECLARE_OBJECT_FUNC_NULL(play_window_front, display);
void RENDER_play_window_front_mouse(int, int, int, int);
void RENDER_play_window_front_motion(int, int);
void RENDER_play_window_front_motion(int, int);
void RENDER_play_window_front_passiveMotion(int, int);
void RENDER_play_window_front_keyboard(unsigned char, int, int);
void RENDER_play_window_front_special(int, int, int);

void RENDER_assembled_message_display(void);
RENDER_DECLARE_OBJECT_FUNC_NULL(assembled_message, mouse);
RENDER_DECLARE_OBJECT_FUNC_NULL(assembled_message, motion);
RENDER_DECLARE_OBJECT_FUNC_NULL(assembled_message, passiveMotion);
RENDER_DECLARE_OBJECT_FUNC_NULL(assembled_message, keyboard);
RENDER_DECLARE_OBJECT_FUNC_NULL(assembled_message, special);

RENDER_DECLARE_OBJECT_FUNC_NULL(assembled_window_front, display);
RENDER_DECLARE_OBJECT_FUNC_NULL(assembled_window_front, mouse);
RENDER_DECLARE_OBJECT_FUNC_NULL(assembled_window_front, motion);
RENDER_DECLARE_OBJECT_FUNC_NULL(assembled_window_front, passiveMotion);
void RENDER_assembled_window_front_keyboard(unsigned char, int, int);
RENDER_DECLARE_OBJECT_FUNC_NULL(assembled_window_front, special);

int main(int argc, char **argv) {
  glutInit(&argc, argv);

  RENDER_WINDOW(hexa, "HeXa", 0, 0); {
    RENDER_OBJECT(hexa, SCENE_START, image,              LAYOUT_IMAGE_X,   LAYOUT_IMAGE_Y);
    RENDER_OBJECT(hexa, SCENE_START, start_message,      LAYOUT_MESSAGE_X, LAYOUT_MESSAGE_Y);
    RENDER_OBJECT(hexa, SCENE_START, start_window_front, 0,                0);

    RENDER_OBJECT(hexa, SCENE_PLAY, play_window_back,        0,               0);
    RENDER_OBJECT(hexa, SCENE_PLAY, image,                   LAYOUT_IMAGE_X,  LAYOUT_IMAGE_Y);
    RENDER_OBJECT(hexa, SCENE_PLAY, play_grid,               LAYOUT_GRID_X,   LAYOUT_GRID_Y);
    RENDER_OBJECT(hexa, SCENE_PLAY, play_picker_cursor_up,   LAYOUT_PICKER_X, LAYOUT_PICKER_CURSOR_UP_Y);
    RENDER_OBJECT(hexa, SCENE_PLAY, play_picker_piece,       LAYOUT_PICKER_X, LAYOUT_PICKER_PIECE_Y);
    RENDER_OBJECT(hexa, SCENE_PLAY, play_picker_cursor_down, LAYOUT_PICKER_X, LAYOUT_PICKER_CURSOR_DOWN_Y);
    RENDER_OBJECT(hexa, SCENE_PLAY, play_picked,             0,               0);
    RENDER_OBJECT(hexa, SCENE_PLAY, play_window_front,       0,               0);

    RENDER_OBJECT(hexa, SCENE_ASSEMBLED, image,                  LAYOUT_IMAGE_X,   LAYOUT_IMAGE_Y);
    RENDER_OBJECT(hexa, SCENE_ASSEMBLED, assembled_message,      LAYOUT_MESSAGE_X, LAYOUT_MESSAGE_Y);
    RENDER_OBJECT(hexa, SCENE_ASSEMBLED, assembled_window_front, 0,                0);

    glutReshapeFunc(cb_reshape);
    glutInitDisplayMode(GLUT_RGBA | GLUT_ALPHA);
    glClearColor(0, 0, 0, 0);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
  }

  atexit(cb_exit);

  BLOCK("GLOBAL VARIABLE INIT") {
    Puzzle_init(&g_puzzle, GRID_X, GRID_Y, VERTEX);

    PieceList_init(&g_piece);

    Image_read(&g_image, FILENAME);
    if (g_image.info.Width != 512 || g_image.info.Height != 512) {
      printf("%s must be 512 * 512.\n", FILENAME);
      exit(1);
    }

    Image_read(&g_message_start, "resource/message-start.png");

    Image_read(&g_message_assembled, "resource/message-assembled.png");

    g_mouse.x = -1;
    g_mouse.y = -1;

    g_grid.x = -1;
    g_grid.y = -1;

    g_offset = 0;

    g_picked = NULL;

    g_picked_idx = PICK_NONE;

    g_mouse_on = MOUSE_ON_NONE;
  }

  glutMainLoop();

  return 0;
}

void Image_read(Image *self, char *filename) {
  self->id = pngBind(
    filename,
    PNG_NOMIPMAP,
    PNG_ALPHA,
    &self->info,
    GL_CLAMP,
    GL_NEAREST,
    GL_NEAREST
  );
}

void cb_reshape(int w, int h) {
  w = LAYOUT_WINDOW_W;
  h = LAYOUT_WINDOW_H;

  glutReshapeWindow(w, h);

  glViewport(0, 0, w, h);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  gluOrtho2D(0, w, 0, h);
  glScaled(1, -1, 1);
  glTranslated(0, -h, 0);
}

void cb_exit(void) {
  Puzzle_destroy(&g_puzzle);
  PieceList_destroy(&g_piece);
}

void draw_hexagon(int size, int fill) {
  glBegin(fill ? GL_POLYGON : GL_LINE_LOOP);
    for (int i = 0; i < 6; i++) {
      glVertex2d(
        size * cos(M_PI / 3.0 * i),
        size * sin(M_PI / 3.0 * i)
      );
    }
  glEnd();
}

void draw_rectangle(int x, int y, int fill) {
  glBegin(fill ? GL_POLYGON : GL_LINE_LOOP);
    glVertex2d(0, 0);
    glVertex2d(x, 0);
    glVertex2d(x, y);
    glVertex2d(0, y);
  glEnd();
}

void draw_triangle(int size, int fill) {
  glBegin(fill ? GL_POLYGON : GL_LINE_LOOP);
    for (int i = 0; i < 3; i++) {
      glVertex2d(
        size * cos(2 * M_PI / 3.0 * i),
        size * sin(2 * M_PI / 3.0 * i)
      );
    }
  glEnd();
}

void draw_image(Image *img) {
  glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, img->id);
    glColor4ub(255, 255, 255, 255);

    glBegin(GL_QUADS);
      glTexCoord2i(0, 0);
      glVertex2d(0, 0);

      glTexCoord2i(0, 1);
      glVertex2d(0, img->info.Height);

      glTexCoord2i(1, 1);
      glVertex2d(img->info.Width, img->info.Height);

      glTexCoord2i(1, 0);
      glVertex2d(img->info.Width, 0);
    glEnd();
  glDisable(GL_TEXTURE_2D);
}

void draw_piece(Piece *p, int size, Image *img) {
  const Vector center = {
    LAYOUT_IMAGE_PADDING_HO + LAYOUT_GRID_PADDING_HO + PIECE_W / 4 * 3 * p->position.x,
    LAYOUT_IMAGE_PADDING_VA + LAYOUT_GRID_PADDING_VA + ((p->position.x % 2) ? PIECE_H / 2 : 0) + PIECE_H * p->position.y
  };

  glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, img->id);
    glColor4ub(255, 255, 255, 255);

    glPushMatrix();
      glRotated(p->rotate * 360 / VERTEX, 0, 0, 1);
      glBegin(GL_POLYGON);
        for (int i = 0; i < VERTEX; i++) {
          glTexCoord2d(
            (center.x + size * cos(2 * M_PI / VERTEX * i)) / img->info.Width,
            (center.y + size * sin(2 * M_PI / VERTEX * i)) / img->info.Height
          );
          glVertex2d(
            size * cos(M_PI / 3.0 * i),
            size * sin(M_PI / 3.0 * i)
          );
        }
      glEnd();
    glPopMatrix();
  glDisable(GL_TEXTURE_2D);
}

int offset_canInc(void) {
  return g_offset < g_piece.length - PICKER_PIECE_CELL;
}

int offset_canDec(void) {
  return g_offset > 0;
}

void offset_inc(void) {
  if (offset_canInc()) {
    g_offset++;
  }
}

void offset_dec(void) {
  if (offset_canDec()) {
    g_offset--;
  }
}

void RENDER_image_display(void) {
  draw_image(&g_image);
}

void RENDER_start_message_display(void) {
  draw_image(&g_message_start);
}

void RENDER_play_grid_display(void) {
  glTranslated(LAYOUT_GRID_PADDING_HO, LAYOUT_GRID_PADDING_VA, 0);

  glLineWidth(2);
  for (int i = 0; i < GRID_X; i++) {
    for (int j = 0; j < GRID_Y; j++) {
      glPushMatrix();
        glTranslated(
          i * PIECE_W / 4 * 3,
          j * PIECE_H + ((i % 2) ? PIECE_H / 2 : 0),
          0
        );

        glColor3ub(CC_BLACK);
        draw_hexagon(UNIT, 1);
        if (Puzzle_get(&g_puzzle, i, j) != NULL) {
          draw_piece(Puzzle_get(&g_puzzle, i, j), UNIT, &g_image);
        }
        glColor3ub(CC_WHITE);
        draw_hexagon(UNIT, 0);
      glPopMatrix();
    }
  }

  if (g_grid.x != -1) {
    const int vertical   = g_grid.x * PIECE_W / 4 * 3;
    const int horizontal = g_grid.y * PIECE_H + (g_grid.x % 2 ? PIECE_H / 2 : 0);

    glColor3ub(CC_GREEN);
    glLineWidth(2);
    glBegin(GL_LINES);
      glVertex2i(vertical, - LAYOUT_GRID_PADDING_VA - LAYOUT_IMAGE_PADDING_VA);
      glVertex2i(vertical, - LAYOUT_GRID_PADDING_VA - LAYOUT_IMAGE_PADDING_VA + LAYOUT_IMAGE_H);
      glVertex2i(- LAYOUT_GRID_PADDING_HO - LAYOUT_IMAGE_PADDING_HO, horizontal);
      glVertex2i(- LAYOUT_GRID_PADDING_HO - LAYOUT_IMAGE_PADDING_HO+ LAYOUT_IMAGE_W, horizontal);
    glEnd();
    glPushMatrix();
      glTranslated(
        PIECE_W / 4 * 3 * g_grid.x,
        PIECE_H * g_grid.y + ((g_grid.x % 2) ? PIECE_H / 2 : 0),
        0
      );
      glColor3ub(CC_GREEN);
      glLineWidth(3);
      draw_hexagon(UNIT, 0);
    glPopMatrix();
  }
}

void RENDER_play_picker_cursor_up_display(void) {
  glColor3ub(CC_WHITE);
  draw_rectangle(LAYOUT_PICKER_W, LAYOUT_PICKER_CURSOR_H, 1);
  if (g_mouse_on == MOUSE_ON_PICKER_CURSOR_UP && offset_canDec()) {
    glColor4ub(CC_BLACK, 255 / 2);
    draw_rectangle(LAYOUT_PICKER_W, LAYOUT_PICKER_CURSOR_H, 1);
  }
  glColor3ub(CC_GRAY);
  glLineWidth(2);
  draw_rectangle(LAYOUT_PICKER_W, LAYOUT_PICKER_CURSOR_H, 0);

  if (offset_canDec()) {
    glTranslated(
      LAYOUT_PICKER_W / 2,
      20,
      0
    );
    glRotated(-90, 0, 0, 1);
    glColor3ub(CC_BLACK);
    draw_triangle(LAYOUT_PICKER_CURSOR_H / 2, 1);
  }
}

void RENDER_play_picker_piece_display(void) {
  glPushMatrix();
    glLineWidth(3);
    for (int i = 0; i < PICKER_PIECE_CELL; i++) {
      glColor3ub(CC_WHITE);
      draw_rectangle(LAYOUT_PICKER_W, LAYOUT_PICKER_PIECE_CELL_H, 1);
      if (
        g_mouse_on == MOUSE_ON_PICKER_PIECE
        && i == (g_mouse.y - LAYOUT_PICKER_PIECE_Y) / LAYOUT_PICKER_PIECE_CELL_H
        && PieceList_get(&g_piece, i) != NULL
      ) {
        glColor4ub(CC_BLACK, 255 / 2);
        draw_rectangle(LAYOUT_PICKER_W, LAYOUT_PICKER_PIECE_CELL_H, 1);
      }
      glColor3ub(CC_GRAY);
      draw_rectangle(LAYOUT_PICKER_W, LAYOUT_PICKER_PIECE_CELL_H, 0);
      glTranslated(0, LAYOUT_PICKER_PIECE_CELL_H, 0);
    }
  glPopMatrix();
  glPushMatrix();
    glTranslated(LAYOUT_PICKER_W / 2, LAYOUT_PICKER_PIECE_CELL_H / 2, 0);
    for (int i = 0; i < PICKER_PIECE_CELL; i++) {
      Piece * const target = PieceList_get(&g_piece, g_offset + i);
      if (target != NULL) {
        draw_piece(target, UNIT, &g_image);
        glColor3ub(CC_BLACK);
        draw_hexagon(UNIT, 0);
      }
      glTranslated(0, LAYOUT_PICKER_PIECE_CELL_H, 0);
    }
  glPopMatrix();
}

void RENDER_play_picker_cursor_down_display(void) {
  glColor3ub(CC_WHITE);
  draw_rectangle(LAYOUT_PICKER_W, LAYOUT_PICKER_CURSOR_H, 1);
  if (g_mouse_on == MOUSE_ON_PICKER_CURSOR_DOWN && offset_canInc()) {
    glColor4ub(CC_BLACK, 255 / 2);
    draw_rectangle(LAYOUT_PICKER_W, LAYOUT_PICKER_CURSOR_H, 1);
  }
  glColor3ub(CC_GRAY);
  glLineWidth(2);
  draw_rectangle(LAYOUT_PICKER_W, LAYOUT_PICKER_CURSOR_H, 0);

  if (offset_canInc()) {
    glTranslated(
      LAYOUT_PICKER_W / 2,
      13,
      0
    );
    glRotated(90, 0, 0, 1);
    glColor3ub(CC_BLACK);
    draw_triangle(LAYOUT_PICKER_CURSOR_H / 2, 1);
  }
}

void RENDER_play_picked_display(void) {
  if (g_picked_idx != PICK_NONE) {
    glTranslated(g_mouse.x, g_mouse.y, 0);
    draw_piece(g_picked, UNIT, &g_image);
    glColor3ub(CC_BLACK);
    glLineWidth(3);
    draw_hexagon(UNIT, 0);
  }
}

void RENDER_assembled_message_display(void) {
  draw_image(&g_message_assembled);
}

void RENDER_play_window_back_mouse(int b, int s, int x, int y) {
  if (s != GLUT_DOWN) {
    return;
  }

  if (b == GLUT_LEFT_BUTTON) {
    if (g_picked_idx != PICK_NONE) {
      if (g_picked_idx == PICK_REMOVED) {
        PieceList_add(&g_piece, g_picked);
      }

      g_picked = NULL;
      g_picked_idx = PICK_NONE;
    }
  }
}

void RENDER_play_grid_mouse(int b, int s, int x, int y) {
  if (s != GLUT_DOWN) {
    return;
  }

  if (b == GLUT_LEFT_BUTTON) {
    if (g_grid.x == -1) {
      if (g_picked_idx != PICK_NONE) {
        if (g_picked_idx == PICK_REMOVED) {
          PieceList_add(&g_piece, g_picked);
        }

        g_picked = NULL;
        g_picked_idx = PICK_NONE;
      }

      return;
    }

    Piece * const removed = Puzzle_remove(&g_puzzle, g_grid.x, g_grid.y);
    Puzzle_put(&g_puzzle, g_picked, g_grid.x, g_grid.y);

    if(g_picked_idx != PICK_NONE && g_picked_idx != PICK_REMOVED) {
      if (g_offset == g_piece.length - PICKER_PIECE_CELL) {
        offset_dec();
      }

      PieceList_remove(&g_piece, g_picked_idx);
    }

    g_picked = removed;
    g_picked_idx = removed == NULL ? PICK_NONE : PICK_REMOVED;
  }
  else if (b == GLUT_RIGHT_BUTTON) {
    if (g_grid.x != -1) {
      if (g_picked_idx == PICK_NONE) {
        Piece * const target = Puzzle_get(&g_puzzle, g_grid.x, g_grid.y);
        if (target != NULL) {
          Piece_rotate(target, &g_puzzle, ~glutGetModifiers() & GLUT_ACTIVE_SHIFT);
        }
      }
    }
  }

  if (PieceList_isEmpty(&g_piece) && Puzzle_isComplete(&g_puzzle)) {
    Puzzle_destroy(&g_puzzle);
    PieceList_destroy(&g_piece);
    g_scene = SCENE_ASSEMBLED;
  }
}

void RENDER_play_picker_cursor_up_mouse(int b, int s, int x, int y) {
  if (s != GLUT_DOWN) {
    return;
  }

  if (b == GLUT_LEFT_BUTTON) {
    offset_dec();
  }
}

void RENDER_play_picker_piece_mouse(int b, int s, int x, int y) {
  if (s != GLUT_DOWN) {
    return;
  }

  const int     picked_idx   = g_offset + y / LAYOUT_PICKER_PIECE_CELL_H;
  Piece * const picked_piece = PieceList_get(&g_piece, picked_idx);

  if (b == GLUT_LEFT_BUTTON) {
    if (g_picked_idx != PICK_NONE) {
      if (g_picked == picked_piece) {
        g_picked_idx = PICK_NONE;
        g_picked = NULL;

        return;
      }

      if (g_picked_idx == PICK_REMOVED) {
        PieceList_add(&g_piece, g_picked);
      }

      g_picked_idx = PICK_NONE;
      g_picked = NULL;
    }

    if (picked_piece == NULL) {
      return;
    }
    g_picked_idx = picked_idx;
    g_picked = picked_piece;
  }
  else if (b == GLUT_RIGHT_BUTTON) {
    if (g_picked_idx == PICK_NONE) {
      Piece_rotate(picked_piece, &g_puzzle, ~glutGetModifiers() & GLUT_ACTIVE_SHIFT);
    }
  }
}

void RENDER_play_picker_cursor_down_mouse(int b, int s, int x, int y) {
  if (s != GLUT_DOWN) {
    return;
  }

  if (b == GLUT_LEFT_BUTTON) {
    offset_inc();
  }
}

void RENDER_play_window_front_mouse(int b, int s, int x, int y) {
  if (s != GLUT_DOWN) {
    return;
  }

  if (b == GLUT_RIGHT_BUTTON) {
    if (g_picked_idx != PICK_NONE) {
      Piece_rotate(g_picked, &g_puzzle, ~glutGetModifiers() & GLUT_ACTIVE_SHIFT);
    }
  }
}

void RENDER_play_grid_motion(int x, int y) {
  RENDER_play_grid_passiveMotion(x, y);
}

void RENDER_play_picker_cursor_up_motion(int x, int y) {
  RENDER_play_picker_cursor_up_passiveMotion(x, y);
}

void RENDER_play_picker_piece_motion(int x, int y) {
  RENDER_play_picker_piece_passiveMotion(x, y);
}

void RENDER_play_picker_cursor_down_motion(int x, int y) {
  RENDER_play_picker_cursor_down_passiveMotion(x, y);
}

void RENDER_play_window_front_motion(int x, int y) {
  RENDER_play_window_front_passiveMotion(x, y);
}

void RENDER_play_grid_passiveMotion(int x, int y) {
  g_grid.x = x / (PIECE_W / 2 + PIECE_W / 4);
  g_grid.y = (y - (g_grid.x % 2 ? PIECE_H / 2 : 0)) / PIECE_H;

  const int left = x % (PIECE_W / 2 + PIECE_W / 4);
  if (left > PIECE_W / 4) {
    if (
      y < 0 + (g_grid.x % 2 ? PIECE_H / 2 : 0)
      || y > PIECE_H * GRID_Y + (g_grid.x % 2 ? PIECE_H / 2 : 0) - 1
    ) {
      g_grid.x = -1;
      g_grid.y = -1;
    }

    return;
  }

  const int horizontal = PIECE_H / 2 + g_grid.y * PIECE_H + (g_grid.x % 2 ? PIECE_H / 2 : 0);
  if (y < horizontal - sqrt(3) * left) {
    if (g_grid.x % 2) {
      if (y < horizontal - PIECE_H + sqrt(3) * left) {
        g_grid.x = -1;
        g_grid.y = -1;
        return;
      }
    }
    else {
      if (g_grid.y - 1 < 0) {
        g_grid.x = -1;
        g_grid.y = -1;
        return;
      }
      else {
        g_grid.y--;
      }
    }

    g_grid.x--;
    return;
  }

  if (y > horizontal + sqrt(3) * left) {
    if (g_grid.x % 2) {
      if (g_grid.y + 1 > GRID_Y - 1) {
        g_grid.x = -1;
        g_grid.y = -1;
        return;
      }

      g_grid.y++;
    }

    g_grid.x--;
    return;
  }

  if (g_grid.x > GRID_X - 1 || g_grid.y > GRID_Y - 1) {
    g_grid.x = -1;
    g_grid.y = -1;
  }
}

void RENDER_play_picker_cursor_up_passiveMotion(int x, int y) {
  g_mouse_on = MOUSE_ON_PICKER_CURSOR_UP;
}

void RENDER_play_picker_piece_passiveMotion(int x, int y) {
  g_mouse_on = MOUSE_ON_PICKER_PIECE;
}

void RENDER_play_picker_cursor_down_passiveMotion(int x, int y) {
  g_mouse_on = MOUSE_ON_PICKER_CURSOR_DOWN;
}

void RENDER_play_window_front_passiveMotion(int x, int y) {
  g_mouse.x = x;
  g_mouse.y = y;
  g_grid.x = -1;
  g_grid.y = -1;
  g_mouse_on = MOUSE_ON_NONE;
}

void RENDER_start_window_front_keyboard(unsigned char c, int x, int y) {
  if (c == 13) {
    Puzzle_init(&g_puzzle, GRID_X, GRID_Y, VERTEX);
    Puzzle_createPiece(&g_puzzle);
    Puzzle_createGrid(&g_puzzle);

    PieceList_load(&g_piece, &g_puzzle);
    g_scene = SCENE_PLAY;
  }
}

void RENDER_play_grid_keyboard(unsigned char c, int x, int y) {
  if (g_picked_idx != PICK_NONE) {
    return;
  }

  if (g_grid.x == -1) {
    return;
  }

  Piece * const target = Puzzle_get(&g_puzzle, g_grid.x, g_grid.y);
  if (target == NULL) {
    return;
  }

  switch (c) {
    case 'd':
    case 'A':
      Piece_rotate(target, &g_puzzle, 1);
      break;
    case 'a':
    case 'D':
      Piece_rotate(target, &g_puzzle, 0);
      break;
    default:
      break;
  }
}

void RENDER_play_picker_piece_keyboard(unsigned char c, int x, int y) {
  if (g_picked_idx != PICK_NONE) {
    return;
  }

  Piece * const target = PieceList_get(&g_piece, g_offset + y / LAYOUT_PICKER_PIECE_CELL_H);
  if (target == NULL) {
    return;
  }

  switch (c) {
    case 'd':
    case 'A':
      Piece_rotate(target, &g_puzzle, 1);
      break;
    case 'a':
    case 'D':
      Piece_rotate(target, &g_puzzle, 0);
      break;
    default:
      break;
  }
}

void RENDER_play_window_front_keyboard(unsigned char c, int x, int y) {
  switch (c) {
    case 'w':
      offset_dec();
      break;
    case 's':
      offset_inc();
      break;
    case 'd':
    case 'A':
      if (g_picked_idx != PICK_NONE) {
        Piece_rotate(g_picked, &g_puzzle, 1);
      }
      break;
    case 'a':
    case 'D':
      if (g_picked_idx != PICK_NONE) {
        Piece_rotate(g_picked, &g_puzzle, 0);
      }
      break;
    default:
      break;
  }
}

void RENDER_assembled_window_front_keyboard(unsigned char c, int x, int y) {
  if (c == 13) {
    g_scene = SCENE_START;
  }
}

void RENDER_play_grid_special(int c, int x, int y) {
  if (g_picked_idx != PICK_NONE) {
    return;
  }

  if (g_grid.x == -1) {
    return;
  }

  Piece * const target = Puzzle_get(&g_puzzle, g_grid.x, g_grid.y);
  if (target == NULL) {
    return;
  }

  switch (c) {
    case GLUT_KEY_RIGHT:
      Piece_rotate(target, &g_puzzle, glutGetModifiers() & GLUT_ACTIVE_SHIFT ? 0 : 1);
      break;
    case GLUT_KEY_LEFT:
      Piece_rotate(target, &g_puzzle, glutGetModifiers() & GLUT_ACTIVE_SHIFT ? 1 : 0);
      break;
    default:
      break;
  }
}

void RENDER_play_picker_piece_special(int c, int x, int y) {
  if (g_picked_idx != PICK_NONE) {
    return;
  }

  Piece * const target = PieceList_get(&g_piece, g_offset + y / LAYOUT_PICKER_PIECE_CELL_H);
  if (target == NULL) {
    return;
  }

  switch (c) {
    case GLUT_KEY_RIGHT:
      Piece_rotate(target, &g_puzzle, glutGetModifiers() & GLUT_ACTIVE_SHIFT ? 0 : 1);
      break;
    case GLUT_KEY_LEFT:
      Piece_rotate(target, &g_puzzle, glutGetModifiers() & GLUT_ACTIVE_SHIFT ? 1 : 0);
      break;
    default:
      break;
  }
}

void RENDER_play_window_front_special(int c, int x, int y) {
  switch (c) {
    case GLUT_KEY_UP:
      offset_dec();
      break;
    case GLUT_KEY_DOWN:
      offset_inc();
      break;
    case GLUT_KEY_RIGHT:
      if (g_picked_idx != PICK_NONE) {
        Piece_rotate(g_picked, &g_puzzle, glutGetModifiers() & GLUT_ACTIVE_SHIFT ? 0 : 1);
      }
      break;
    case GLUT_KEY_LEFT:
      if (g_picked_idx != PICK_NONE) {
        Piece_rotate(g_picked, &g_puzzle, glutGetModifiers() & GLUT_ACTIVE_SHIFT ? 1 : 0);
      }
      break;
    default:
      break;
  }
}