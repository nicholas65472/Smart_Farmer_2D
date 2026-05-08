#include "Game.h"
#include "FenceLogic.h"

#include <array>
#include <functional>
#include <vector>

static bool isForbiddenFenceSquare(int fenceId, int squareIndex) {
    if (fenceId == 0 && (squareIndex < 3 || squareIndex == 5 || squareIndex > 10)) {
        return true;
    }

    if (fenceId == 1 && (squareIndex < 3 || squareIndex == 3 || squareIndex == 7)) {
        return true;
    }

    if (fenceId == 2 && ((squareIndex >= 8 && squareIndex <= 11) || squareIndex == 3)) {
        return true;
    }

    if ((fenceId == 4 || fenceId == 5) && (squareIndex > 12 || squareIndex < 3)) {
        return true;
    }

    if (fenceId == 7 && (squareIndex == 3 || squareIndex == 7 || squareIndex >= 8)) {
        return true;
    }

    if (fenceId == 3 && squareIndex >= 3) {
        return true;
    }

    return false;
}

static sf::Vector2f getExactFencePosition(int fenceId, int squareIndex) {
    float x = 0.f;
    float y = 0.f;

    switch (fenceId) {
        case 0:
            x = (squareIndex == 3 ? 145.f : (squareIndex == 4 ? 270.f : (squareIndex == 8 ? 145.f : 270.f)));
            y = (squareIndex == 3 ? 250.f : (squareIndex == 4 ? 250.f : (squareIndex == 8 ? 373.f : 374.f)));
            break;

        case 1:
            x = (squareIndex == 4 ? 277.f : (squareIndex == 5 ? 403.f : (squareIndex == 6 ? 528.f : 0.f)));
            y = (squareIndex == 4 ? 245.f : (squareIndex == 5 ? 245.f : (squareIndex == 6 ? 245.f : 0.f)));
            break;

        case 2:
            x = (squareIndex == 0 ? 278.f : (squareIndex == 1 ? 402.f : (squareIndex == 4 ? 277.f : (squareIndex == 5 ? 402.f : 0.f))));
            y = (squareIndex == 0 ? 132.f : (squareIndex == 1 ? 132.f : (squareIndex == 4 ? 255.f : (squareIndex == 5 ? 255.f : 0.f))));
            break;

        case 3:
            x = (squareIndex == 0 ? 174.f : (squareIndex == 1 ? 298.f : (squareIndex == 2 ? 423.f : 0.f)));
            y = (squareIndex == 0 ? 155.f : (squareIndex == 1 ? 155.f : (squareIndex == 2 ? 155.f : 0.f)));
            break;

        case 4:
            x = (squareIndex == 3 || squareIndex == 8 ? 198.f : (squareIndex == 4 || squareIndex == 9 ? 323.f : (squareIndex == 5 || squareIndex == 10 ? 447.f : 0.f)));
            y = (squareIndex >= 3 && squareIndex <= 5 ? 260.f : 383.f);
            break;

        case 5:
            x = (squareIndex == 3 || squareIndex == 8 ? 172.f : 293.f);
            y = (squareIndex == 3 || squareIndex == 4 ? 238.f : 360.f);
            break;

        case 6:
            x = (squareIndex == 0 ? 235.f : (squareIndex == 1 ? 360.f : (squareIndex == 2 ? 485.f : 0.f)));
            y = (squareIndex == 0 ? 145.f : (squareIndex == 1 ? 145.f : (squareIndex == 2 ? 145.f : 0.f)));
            break;

        case 7:
            x = (squareIndex == 0 ? 252.f : (squareIndex == 1 ? 375.f : (squareIndex == 2 ? 500.f : (squareIndex == 4 ? 250.f : (squareIndex == 5 ? 377.f : (squareIndex == 6 ? 502.f : 0.f))))));
            y = (squareIndex == 0 ? 137.f : (squareIndex == 1 ? 137.f : (squareIndex == 2 ? 137.f : (squareIndex == 4 ? 262.f : (squareIndex == 5 ? 262.f : (squareIndex == 6 ? 262.f : 0.f))))));
            break;

        default:
            break;
    }

    return {x, y};
}

static void setFenceGroupAvailability(std::array<bool, FENCE_COUNT>& usable, int fenceId, bool value) {
    if (fenceId >= 0 && fenceId <= 3) {
        usable[0] = value;
        usable[1] = value;
        usable[2] = value;
        usable[3] = value;
        return;
    }

    if (fenceId == 5 || fenceId == 6) {
        usable[5] = value;
        usable[6] = value;
        return;
    }

    if (fenceId == 4 || fenceId == 7) {
        usable[4] = value;
        usable[7] = value;
        return;
    }
}

void Game::selectFence(int fenceId) {
    fenceSelected_[fenceId] = !fenceSelected_[fenceId];

    if (fenceSelected_[fenceId]) {
        selectedFence_ = fenceId;
    } else {
        selectedFence_ = -1;
    }

    for (int i = 0; i < FENCE_COUNT; ++i) {
        if (i != fenceId) {
            fenceSelected_[i] = false;
        }
    }
}

bool Game::isFencePlacementAllowed(int fenceId, int squareIndex) {
    if (fenceId < 0 || fenceId >= FENCE_COUNT) {
        return false;
    }

    if (squareIndex < 0 || squareIndex >= BOARD_FENCE_SQUARES) {
        return false;
    }

    if (!fenceUsable_[fenceId] || fencePlaced_[fenceId]) {
        return false;
    }

    if (isForbiddenFenceSquare(fenceId, squareIndex)) {
        return false;
    }

    const GridPosition p = squareToGridPosition(squareIndex);
    const int r = p.row;
    const int c = p.col;

    Matrix fencesMock = board_.fences();
    const Matrix& currentBoard = board_.current();

    auto blocked = [&](int row, int col, int orientation, int prevRow, int prevCol, int prevPrevRow, int prevPrevCol) {
        if (row < 0 || row >= BOARD_ROWS || col < 0 || col >= BOARD_COLS) {
            return true;
        }

        return isFenceSegmentBlocked(
            currentBoard,
            fencesMock,
            row,
            col,
            orientation,
            prevRow,
            prevCol,
            prevPrevRow,
            prevPrevCol
        );
    };

    switch (fenceId) {
        case 0:
            return !(blocked(r, c, 0, 0, 0, 0, 0) ||
                     blocked(r, c + 2, 0, r, c, 0, 0) ||
                     blocked(r, c + 4, 0, r, c + 2, r, c) ||
                     blocked(r + 2, c + 4, 1, r, c + 4, r, c + 2));

        case 1:
            return !(blocked(r, c, 0, 0, 0, 0, 0) ||
                     blocked(r, c + 2, 0, r, c, 0, 0) ||
                     blocked(r + 2, c, 1, r, c + 2, r, c) ||
                     blocked(r + 4, c, 1, r + 2, c, r, c + 2));

        case 2:
            return !(blocked(r, c, 0, 0, 0, 0, 0) ||
                     blocked(r + 2, c, 1, r, c, 0, 0) ||
                     blocked(r + 2, c + 2, 0, r + 2, c, r, c) ||
                     blocked(r + 2, c + 4, 0, r + 2, c + 2, r + 2, c));

        case 3:
            return !(blocked(r, c, 0, 0, 0, 0, 0) ||
                     blocked(r + 2, c, 1, r, c, 0, 0) ||
                     blocked(r + 4, c, 1, r + 2, c, r, c) ||
                     blocked(r + 4, c - 2, 0, r + 4, c, r + 2, c));

        case 4:
            return !(blocked(r, c, 0, 0, 0, 0, 0) ||
                     blocked(r, c + 2, 0, r, c, 0, 0) ||
                     blocked(r, c + 4, 0, r, c + 2, r, c));

        case 5:
            return !(blocked(r, c, 0, 0, 0, 0, 0) ||
                     blocked(r, c + 2, 0, r, c, 0, 0) ||
                     blocked(r, c + 4, 0, r, c + 2, r, c) ||
                     blocked(r, c + 6, 0, r, c + 4, r, c + 2));

        case 6:
            return !(blocked(r, c, 0, 0, 0, 0, 0) ||
                     blocked(r + 2, c, 1, r, c, 0, 0) ||
                     blocked(r + 4, c, 1, r + 2, c, r, c) ||
                     blocked(r + 6, c, 1, r + 4, c, r + 2, c));

        case 7:
            return !(blocked(r, c, 0, 0, 0, 0, 0) ||
                     blocked(r + 2, c, 1, r, c, 0, 0) ||
                     blocked(r + 4, c, 1, r + 2, c, r, c));

        default:
            return false;
    }
}

void Game::tryPlaceSelectedFence(int squareIndex) {
    if (!isFencePlacementAllowed(selectedFence_, squareIndex)) {
        return;
    }

    const sf::Vector2f exactPosition = getExactFencePosition(selectedFence_, squareIndex);

    fenceBoardSprites_[selectedFence_].setTexture(resources_.fenceTextures[selectedFence_]);
    fenceBoardSprites_[selectedFence_].setPosition(exactPosition);
    fenceBoardSprites_[selectedFence_].setScale(1.f, 1.f);

    applyFenceToBoard(selectedFence_, squareIndex, board_.current(), board_.fences(), 2);

    fenceOriginSquares_[selectedFence_] = squareIndex;
    fencePlaced_[selectedFence_] = true;
    fenceSelected_[selectedFence_] = false;

    updateFenceAvailabilityAfterPlace(selectedFence_);

    resources_.fencePlaceMusic.play();

    if (board_.isCompleted()) {
        if (selectedLevel_ >= 1 && selectedLevel_ <= LEVEL_COUNT) {
            levelCompleted_[selectedLevel_ - 1] = true;
        }

        pendingWin_ = true;
        winDelayClock_.restart();
    }

    selectedFence_ = -1;
}

void Game::tryRemoveFence(int fenceId) {
    if (fenceId < 0 || fenceId >= FENCE_COUNT || !fencePlaced_[fenceId]) {
        return;
    }

    const int squareIndex = fenceOriginSquares_[fenceId];

    if (squareIndex >= 0) {
        applyFenceToBoard(fenceId, squareIndex, board_.current(), board_.fences(), -2);
    }

    fenceOriginSquares_[fenceId] = -1;
    fencePlaced_[fenceId] = false;
    fenceSelected_[fenceId] = false;

    updateFenceAvailabilityAfterRemove(fenceId);

    resources_.fenceUnplaceMusic.play();

    if (board_.isCompleted()) {
        if (selectedLevel_ >= 1 && selectedLevel_ <= LEVEL_COUNT) {
            levelCompleted_[selectedLevel_ - 1] = true;
        }

        pendingWin_ = true;
        winDelayClock_.restart();
    }
}

void Game::updateFenceAvailabilityAfterPlace(int fenceId) {
    setFenceGroupAvailability(fenceUsable_, fenceId, false);
}

void Game::updateFenceAvailabilityAfterRemove(int fenceId) {
    setFenceGroupAvailability(fenceUsable_, fenceId, true);
}

void Game::solveCurrentLevel() {
    if (selectedLevel_ < 1 || selectedLevel_ > LEVEL_COUNT) {
        return;
    }

    resetCurrentLevelState();
    board_.current() = board_.initial();

    struct Move {
        int fenceId;
        int squareIndex;
    };

    std::vector<Move> solutionMoves;

    std::function<bool()> backtrack = [&]() -> bool {
        if (board_.isCompleted()) {
            return true;
        }

        for (int fenceId = 0; fenceId < FENCE_COUNT; ++fenceId) {
            if (!fenceUsable_[fenceId] || fencePlaced_[fenceId]) {
                continue;
            }

            for (int squareIndex = 0; squareIndex < BOARD_FENCE_SQUARES; ++squareIndex) {
                if (!isFencePlacementAllowed(fenceId, squareIndex)) {
                    continue;
                }

                Matrix beforeBoard = board_.current();
                Matrix beforeFences = board_.fences();

                auto beforePlaced = fencePlaced_;
                auto beforeUsable = fenceUsable_;
                auto beforeSelected = fenceSelected_;
                auto beforeOrigins = fenceOriginSquares_;

                applyFenceToBoard(fenceId, squareIndex, board_.current(), board_.fences(), 2);

                bool stillPossible = true;

                for (int i = 0; i < BOARD_ROWS; ++i) {
                    for (int j = 0; j < BOARD_COLS; ++j) {
                        int currentValue = board_.current()[i][j];
                        int solutionValue = board_.solution()[i][j];

                        if (currentValue == 2 && solutionValue != 2) {
                            stillPossible = false;
                        }

                        if (currentValue != 0 &&
                            currentValue != 2 &&
                            currentValue != solutionValue) {
                            stillPossible = false;
                        }
                    }
                }

                if (stillPossible) {
                    fencePlaced_[fenceId] = true;
                    fenceOriginSquares_[fenceId] = squareIndex;
                    updateFenceAvailabilityAfterPlace(fenceId);

                    solutionMoves.push_back({fenceId, squareIndex});

                    if (backtrack()) {
                        return true;
                    }

                    solutionMoves.pop_back();
                }

                board_.current() = beforeBoard;
                board_.fences() = beforeFences;

                fencePlaced_ = beforePlaced;
                fenceUsable_ = beforeUsable;
                fenceSelected_ = beforeSelected;
                fenceOriginSquares_ = beforeOrigins;
            }
        }

        return false;
    };

    if (!backtrack()) {
        return;
    }

    resetCurrentLevelState();
    board_.current() = board_.initial();

    for (const Move& move : solutionMoves) {
        const sf::Vector2f exactPosition = getExactFencePosition(move.fenceId, move.squareIndex);

        fenceBoardSprites_[move.fenceId].setTexture(resources_.fenceTextures[move.fenceId]);
        fenceBoardSprites_[move.fenceId].setPosition(exactPosition);
        fenceBoardSprites_[move.fenceId].setScale(1.f, 1.f);

        applyFenceToBoard(move.fenceId, move.squareIndex, board_.current(), board_.fences(), 2);

        fenceOriginSquares_[move.fenceId] = move.squareIndex;
        fencePlaced_[move.fenceId] = true;
        fenceSelected_[move.fenceId] = false;

        updateFenceAvailabilityAfterPlace(move.fenceId);
    }

    levelCompleted_[selectedLevel_ - 1] = true;
    pendingWin_ = true;
    winDelayClock_.restart();
}