#pragma once

#include "Board.h"
#include <array>

struct GridPosition {
    int row = 0;
    int col = 0;
};

GridPosition squareToGridPosition(int squareIndex);
bool isValidPreviousPosition(int x, int y);
bool isFenceSegmentBlocked(
    const Matrix& board,
    Matrix& fencesMock,
    int row,
    int col,
    int orientation,
    int prevRow,
    int prevCol,
    int prevPrevRow,
    int prevPrevCol
);

void applyFenceToBoard(int fenceId, int squareIndex, Matrix& board, Matrix& fences, int delta);