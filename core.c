/*
 *  作者 芳賀七海
 *  copyright (c) 2020 芳賀七海 All rights reserved.
 */

#include "core.h"
#include <stdlib.h>
#include <time.h>

#include <stdio.h>

void Puzzle_init(Puzzle *self, int x, int y, int vertex) {
  self->vertex = vertex;
  self->size.x = x;
  self->size.y = y;

  self->piece  = NULL;
  self->grid = NULL;
}

Piece *Puzzle_createPiece(Puzzle *self) {
  srand(time(NULL));
  self->piece  = (Piece *)malloc(self->size.x * self->size.y * sizeof(Piece));
  for (int i = 0; i < self->size.x; i++) {
    for (int j = 0; j < self->size.y; j++) {
      self->piece[i * self->size.y + j].position.x = i;
      self->piece[i * self->size.y + j].position.y = j;
      self->piece[i * self->size.y + j].rotate = rand() % self->vertex;
    }
  }
  return self->piece;
}

Piece **Puzzle_createGrid(Puzzle *self) {
  self->grid = (Piece **)malloc(self->size.x * self->size.y * sizeof(Piece *));
  for (int i = 0; i < self->size.x; i++) {
    for (int j = 0; j < self->size.y; j++) {
      Puzzle_put(self, NULL, i, j);
    }
  }
  return self->grid;
}

void Puzzle_destroy(Puzzle *self) {
  free(self->grid);
  free(self->piece);
  Puzzle_init(self, 0, 0, 0);
}

Piece *Puzzle_get(Puzzle *self, int x, int y) {
  return self->grid[x * self->size.y + y];
}

void Puzzle_put(Puzzle *self, Piece *p, int x, int y) {
  self->grid[x * self->size.y + y] = p;
}

Piece *Puzzle_remove(Puzzle *self, int x, int y) {
  Piece * const tmp = Puzzle_get(self, x, y);
  Puzzle_put(self, NULL, x, y);
  return tmp;
}

int Puzzle_isComplete(Puzzle *self) {
  for (int i = 0; i < self->size.x; i++) {
    for (int j = 0; j < self->size.y; j++) {
      Piece * const target = Puzzle_get(self, i, j);
      if (target == NULL) {
        return 0;
      }

      if (target->position.x != i || target->position.y != j) {
        return 0;
      }

      if (target->rotate) {
        return 0;
      }
    }
  }

  return 1;
}

void PieceList_init(PieceList *self) {
  self->head = NULL;
  self->tail = NULL;
  self->length = 0;
}

void PieceList_load(PieceList *self, Puzzle *p) {
  const int piece = p->size.x * p->size.y;

  int * const is_already = (int *)malloc(piece * sizeof(int));
  for (int i = 0; i < piece; i++) {
    is_already[i] = 0;
  }

  int already = 0;
  while (already < piece) {
    const int target = rand() % piece;
    if (is_already[target]) {
      continue;
    }

    PieceList_add(self, &p->piece[target]);
    is_already[target] = 1;
    already++;
  }

  free(is_already);
}

void PieceList_destroy(PieceList *self) {
  PieceListE *e = self->head;
  while (e != NULL) {
    PieceListE * const next = e->next;
    free(e);
    e = next;
  }

  PieceList_init(self);
}

void PieceList_add(PieceList *self, Piece *p) {
  PieceListE * const new = (PieceListE *)malloc(sizeof(PieceListE));
  new->piece = p;
  new->prev  = self->tail;
  new->next  = NULL;

  if (self->head == NULL) {
    self->head = new;
  }
  else {
    new->prev->next = new;
  }

  self->tail = new;
  self->length++;
}

Piece *PieceList_get(PieceList *self, int idx) {
  if (idx < 0 || idx > self->length - 1) {
    return NULL;
  }

  const int direction = idx < self->length / 2 ? 1 : -1;
  int i = direction == 1 ? 0 : self->length - 1;

  PieceListE *e = direction == 1 ? self->head : self->tail;
  while (e != NULL) {
    if (i != idx) {
      i += direction;
      e = (direction == 1 ? e->next : e->prev);
      continue;
    }

    return e->piece;
  }

  return NULL;
}

void PieceList_remove(PieceList *self, int idx) {
  if (idx < 0 || idx > self->length - 1) {
    return;
  }

  const int direction = idx < self->length / 2 ? 1 : -1;
  int i = direction == 1 ? 0 : self->length - 1;
  PieceListE *e = direction == 1 ? self->head : self->tail;
  while (e != NULL) {
    if (i != idx) {
      i += direction;
      e = direction == 1 ? e->next : e->prev;
      continue;
    }

    if (e->prev == NULL) {
      self->head = e->next;
    }
    else {
      e->prev->next = e->next;
    }

    if (e->next == NULL) {
      self->tail = e->prev;
    }
    else {
      e->next->prev = e->prev;
    }

    self->length--;
    free(e);
    return;
  }
}

int PieceList_isEmpty(PieceList *self) {
  return !self->length;
}

void Piece_rotate(Piece *self, Puzzle *p, int d) {
  if (d) {
    self->rotate++;
    if (self->rotate > p->vertex - 1) {
      self->rotate = 0;
    }
  }
  else {
    self->rotate--;
    if (self->rotate < 0) {
      self->rotate = p->vertex - 1;
    }
  }
}