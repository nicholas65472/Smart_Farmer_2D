#pragma once

#include "Constants.h"
#include <array>
#include <string>

using Matrix = std::array<std::array<int, BOARD_COLS>, BOARD_ROWS>;

class Board {
public:
    void loadLevel(int level, float& timeLimit);
    void clear();

    bool isCompleted() const;
    const Matrix& current() const;
    const Matrix& solution() const;
    const Matrix& initial() const;
    Matrix& current();
    Matrix& fences();

private:
    Matrix board_{};
    Matrix solution_{};
    Matrix initial_{};
    Matrix fences_{};
};

std::string animalTexturePath(int animalType);
int cellValueFromFenceCount(int fenceCount);