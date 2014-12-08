/*
 *  Chess
 *  Copyright (C) 2014  A. Cortes
 *  This program is under the terms of the GNU GPL v3
 *  See LICENSE file in the root of this project
 */

#ifndef GUI_H
#define GUI_H

void InitNCurses();
void EndNCurses();
void UpdateFEN(const std::string fen);
void UpdateBoard(acortes::chess::Board & board, const bool print_as_white);
void UpdateMoves(const acortes::chess::Board *const current,
                 const acortes::chess::Board *const analysis);

#endif
