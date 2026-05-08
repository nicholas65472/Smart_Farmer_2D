#pragma once

#include "Board.h"
#include "Resources.h"

#include <SFML/Graphics.hpp>
#include <array>

class Game {
public:
    Game();
    void run();

private:
    enum class State {
        MainMenu,
        LevelMenu,
        HowToPlay,
        Exit,
        Playing,
        Result
    };

    void handleEvents();
    void update();
    void render();

    void renderMainMenu();
    void renderLevelMenu();
    void renderHowToPlay();
    void renderPlaying();
    void renderResult();

    void resetCurrentLevelState();
    void goToLevel(int level);
    void goToLevelMenu();

    void initializeSprites();
    void initializeClickableAreas();

    void selectFence(int fenceId);
    void tryPlaceSelectedFence(int squareIndex);
    void tryRemoveFence(int fenceId);

    bool isFencePlacementAllowed(int fenceId, int squareIndex);
    void updateFenceAvailabilityAfterPlace(int fenceId);
    void updateFenceAvailabilityAfterRemove(int fenceId);

    void drawFenceOverlays();
    void drawAnimals();
    void drawBackButtonForPlaying();
    void drawSolveButtonForPlaying();
    void drawLevelAndTimer();

    void solveCurrentLevel();

private:
    sf::RenderWindow window_;
    Resources resources_;
    Board board_;

    State state_ = State::MainMenu;
    sf::Clock clock_;
    sf::Clock winDelayClock_;

    float timeLimit_ = 30.f;
    int selectedLevel_ = -1;
    int selectedFence_ = -1;
    int resultWin_ = 0;

    bool soundOn_ = true;
    bool winMusicPlayed_ = false;
    bool loseMusicPlayed_ = false;
    bool pendingWin_ = false;
    bool leftClicked_ = false;

    sf::Vector2i mousePos_{};

    sf::Color buttonColor_{178, 70, 76};
    sf::Color levelColor_{141, 182, 60};
    sf::Color selectedFenceColor_{172, 195, 52, 128};
    sf::Color usedFenceColor_{104, 104, 104, 128};
    sf::Color unusableFenceColor_{255, 0, 0, 128};

    sf::Cursor cursorHand_;
    sf::Cursor cursorDefault_;

    sf::Sprite mainBackground_;
    sf::Sprite levelsBackground_;
    sf::Sprite helpBackground_;
    sf::Sprite boardSprite_;
    sf::Sprite soundSprite_;

    std::array<sf::Sprite, FENCE_COUNT> fenceMenuSprites_;
    std::array<sf::Sprite, FENCE_COUNT> fenceBoardSprites_;

    std::array<sf::RectangleShape, BOARD_FENCE_SQUARES> fenceSquares_;
    std::array<sf::RectangleShape, ANIMAL_SQUARES> animalSquares_;

    std::array<int, FENCE_COUNT> fenceOriginSquares_{};
    std::array<bool, FENCE_COUNT> fencePlaced_{};
    std::array<bool, FENCE_COUNT> fenceUsable_{};
    std::array<bool, FENCE_COUNT> fenceSelected_{};
    std::array<bool, LEVEL_COUNT> levelCompleted_{};
};