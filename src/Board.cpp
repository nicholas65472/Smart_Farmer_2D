#include "Board.h"

#include <fstream>
#include <stdexcept>

void Board::loadLevel(int level, float& timeLimit) {
    clear();

    std::ifstream finBoard("../nivele/planse/plansa_" + std::to_string(level) + ".txt");
    std::ifstream finSolution("../nivele/rezolvari/rezolvare_" + std::to_string(level) + ".txt");

    if (!finBoard.is_open()) {
        throw std::runtime_error("Nu pot deschide plansa nivelului " + std::to_string(level));
    }

    if (!finSolution.is_open()) {
        throw std::runtime_error("Nu pot deschide rezolvarea nivelului " + std::to_string(level));
    }

    for (int i = 0; i < BOARD_ROWS; ++i) {
        for (int j = 0; j < BOARD_COLS; ++j) {
            finBoard >> board_[i][j];
            initial_[i][j] = board_[i][j];
            fences_[i][j] = 0;

            finSolution >> solution_[i][j];
        }
    }

    if (level >= 1 && level <= 12) {
        timeLimit = 30.f;
    } else if (level >= 13 && level <= 24) {
        timeLimit = 45.f;
    } else if (level >= 25 && level <= 34) {
        timeLimit = 60.f;
    } else {
        timeLimit = 90.f;
    }
}

void Board::clear() {
    for (auto& row : board_) {
        row.fill(0);
    }

    for (auto& row : solution_) {
        row.fill(0);
    }

    for (auto& row : initial_) {
        row.fill(0);
    }

    for (auto& row : fences_) {
        row.fill(0);
    }
}

bool Board::isCompleted() const {
    return board_ == solution_;
}

const Matrix& Board::current() const {
    return board_;
}

const Matrix& Board::solution() const {
    return solution_;
}

const Matrix& Board::initial() const {
    return initial_;
}

Matrix& Board::current() {
    return board_;
}

Matrix& Board::fences() {
    return fences_;
}

std::string animalTexturePath(int animalType) {
    switch (animalType) {
        case 4:
            return Path::HORSE2;
        case -4:
            return Path::HORSE1;
        case 5:
            return Path::COW2;
        case -5:
            return Path::COW1;
        case 6:
            return Path::PIG;
        case 7:
            return Path::SHEEP;
        case 8:
            return Path::WATERPOT;
        default:
            return "";
    }
}

int cellValueFromFenceCount(int fenceCount) {
    return fenceCount > 0 ? 2 : 0;
}