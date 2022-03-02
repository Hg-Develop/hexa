/*
 *  作者 芳賀七海
 *  copyright (c) 2020 芳賀七海 All rights reserved.
 */

#ifndef CORE_H
#define CORE_H

#include "vector.h"

typedef struct {
  Vector position;
  int rotate;
} Piece;

typedef struct {
  Vector size;
  int vertex;
  Piece *piece;
  Piece **grid;
} Puzzle;

void    Puzzle_init(Puzzle *, int, int, int);
Piece  *Puzzle_createPiece(Puzzle *);
Piece **Puzzle_createGrid(Puzzle *);
void    Puzzle_destroy(Puzzle *);

Piece *Puzzle_get(Puzzle *, int, int);
void   Puzzle_put(Puzzle *, Piece *, int, int);
Piece *Puzzle_remove(Puzzle *, int, int);
int    Puzzle_isComplete(Puzzle *);

typedef struct pieceListE {
  Piece *piece;
  struct pieceListE *next;
  struct pieceListE *prev;
} PieceListE;

typedef struct {
  PieceListE *head;
  PieceListE *tail;
  int length;
} PieceList;

void   PieceList_init(PieceList *);
void   PieceList_load(PieceList *, Puzzle *);
void   PieceList_destroy(PieceList *);
void   PieceList_add(PieceList *, Piece *);
Piece *PieceList_get(PieceList *, int);
void   PieceList_remove(PieceList *, int);
int    PieceList_isEmpty(PieceList *);

void Piece_rotate(Piece *, Puzzle *, int);

#endif