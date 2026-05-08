#include "FenceLogic.h"

GridPosition squareToGridPosition(int squareIndex) {
    GridPosition pos;

    if (squareIndex <= 2) {
        pos.row = 0;
    } else if (squareIndex <= 7) {
        pos.row = 2;
    } else if (squareIndex <= 12) {
        pos.row = 4;
    } else {
        pos.row = 6;
    }

    if (squareIndex == 3 || squareIndex == 8) {
        pos.col = 0;
    } else if (squareIndex == 0 || squareIndex == 4 || squareIndex == 9 || squareIndex == 13) {
        pos.col = 2;
    } else if (squareIndex == 1 || squareIndex == 5 || squareIndex == 10 || squareIndex == 14) {
        pos.col = 4;
    } else if (squareIndex == 2 || squareIndex == 6 || squareIndex == 11 || squareIndex == 15) {
        pos.col = 6;
    } else {
        pos.col = 8;
    }

    return pos;
}

bool isValidPreviousPosition(int x, int y) {
    return (x != 0 || y != 0);
}

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
) {
    const bool vertical = orientation == 1;

    const bool firstCondValid =
        row - 2 >= 0 && col - 2 >= 0 && col + 2 <= 8 && vertical &&
        isValidPreviousPosition(prevPrevRow, prevPrevCol);

    const bool secondCondValid =
        row - 2 >= 0 && col - 2 >= 0 && row + 2 <= 6 && !vertical &&
        isValidPreviousPosition(prevPrevRow, prevPrevCol);

    const bool verticalBlocked =
        vertical &&
        prevRow == row - 2 && prevCol == col &&
        isValidPreviousPosition(prevRow, prevCol) &&
        fencesMock[row - 2][col] == 4 &&
        !fencesMock[row - 2][col - 2] && !fencesMock[row - 2][col + 2] &&
        fencesMock[row][col] == 2 &&
        !fencesMock[row][col - 2] && !fencesMock[row][col + 2] &&
        row - 2 >= 0 && col - 2 >= 0;

    const bool verticalContinuationBlocked =
        firstCondValid &&
        row - 4 == prevPrevRow &&
        fencesMock[row - 4][col] &&
        board[row - 2][col - 2] &&
        board[row - 2][col + 2];

    const bool horizontalContinuationBlocked =
        secondCondValid &&
        col - 4 == prevPrevCol &&
        fencesMock[row][col - 4] &&
        board[row - 2][col - 2] &&
        board[row + 2][col - 2];

    const bool horizontalBlocked =
        !vertical &&
        prevRow == row && prevCol == col - 2 &&
        isValidPreviousPosition(prevRow, prevCol) &&
        fencesMock[row][col - 2] == 4 &&
        !fencesMock[row - 2][col - 2] && !fencesMock[row + 2][col - 2] &&
        fencesMock[row][col] == 2 &&
        !fencesMock[row - 2][col] && !fencesMock[row + 2][col] &&
        col - 2 >= 0 && row - 2 >= 0;

    if (verticalBlocked || verticalContinuationBlocked || horizontalContinuationBlocked || horizontalBlocked) {
        return true;
    }

    fencesMock[row][col] += 2;
    return false;
}

static void addSegment(Matrix& board, Matrix& fences, int row, int col, int delta) {
    fences[row][col] += delta;
    board[row][col] = cellValueFromFenceCount(fences[row][col]);
}

void applyFenceToBoard(int fenceId, int squareIndex, Matrix& board, Matrix& fences, int delta) {
    const GridPosition p = squareToGridPosition(squareIndex);
    const int r = p.row;
    const int c = p.col;

    switch (fenceId) {
        case 0:
            addSegment(board, fences, r, c, delta);
            addSegment(board, fences, r, c + 2, delta);
            addSegment(board, fences, r, c + 4, delta);
            addSegment(board, fences, r + 2, c + 4, delta);
            break;
        case 1:
            addSegment(board, fences, r, c, delta);
            addSegment(board, fences, r, c + 2, delta);
            addSegment(board, fences, r + 2, c, delta);
            addSegment(board, fences, r + 4, c, delta);
            break;
        case 2:
            addSegment(board, fences, r, c, delta);
            addSegment(board, fences, r + 2, c, delta);
            addSegment(board, fences, r + 2, c + 2, delta);
            addSegment(board, fences, r + 2, c + 4, delta);
            break;
        case 3:
            addSegment(board, fences, r, c, delta);
            addSegment(board, fences, r + 2, c, delta);
            addSegment(board, fences, r + 4, c, delta);
            addSegment(board, fences, r + 4, c - 2, delta);
            break;
        case 4:
            addSegment(board, fences, r, c, delta);
            addSegment(board, fences, r, c + 2, delta);
            addSegment(board, fences, r, c + 4, delta);
            break;
        case 5:
            addSegment(board, fences, r, c, delta);
            addSegment(board, fences, r, c + 2, delta);
            addSegment(board, fences, r, c + 4, delta);
            addSegment(board, fences, r, c + 6, delta);
            break;
        case 6:
            addSegment(board, fences, r, c, delta);
            addSegment(board, fences, r + 2, c, delta);
            addSegment(board, fences, r + 4, c, delta);
            addSegment(board, fences, r + 6, c, delta);
            break;
        case 7:
            addSegment(board, fences, r, c, delta);
            addSegment(board, fences, r + 2, c, delta);
            addSegment(board, fences, r + 4, c, delta);
            break;
        default:
            break;
    }
}