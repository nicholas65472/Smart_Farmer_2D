#include "Game.h"

static bool containsMouse(const sf::FloatRect& bounds, const sf::Vector2i& mouse) {
    return bounds.contains(static_cast<float>(mouse.x), static_cast<float>(mouse.y));
}

void Game::renderPlaying() {
    if (selectedLevel_ == -1) {
        state_ = State::LevelMenu;
        return;
    }

    sf::RectangleShape rectBG;
    rectBG.setFillColor(levelColor_);
    rectBG.setSize({static_cast<float>(SCREEN_X), static_cast<float>(SCREEN_Y)});
    window_.draw(rectBG);

    window_.draw(boardSprite_);

    for (int i = 0; i < FENCE_COUNT; ++i) {
        window_.draw(fenceMenuSprites_[i]);
    }

    for (int i = 0; i < BOARD_FENCE_SQUARES; ++i) {
        window_.draw(fenceSquares_[i]);
    }

    for (int i = 0; i < ANIMAL_SQUARES; ++i) {
        window_.draw(animalSquares_[i]);
    }

    drawAnimals();

    for (int i = 0; i < FENCE_COUNT; ++i) {
        if (fencePlaced_[i]) {
            window_.draw(fenceBoardSprites_[i]);
        }
    }

    drawFenceOverlays();
    drawLevelAndTimer();
    drawBackButtonForPlaying();
    drawSolveButtonForPlaying();
}

void Game::drawAnimals() {
    const Matrix& initial = board_.initial();

    for (int i = 0; i < BOARD_ROWS; ++i) {
        for (int j = 0; j < BOARD_COLS; ++j) {
            int value = initial[i][j];

            if (value == 0) {
                continue;
            }

            auto it = resources_.animalTextures.find(value);
            if (it == resources_.animalTextures.end()) {
                continue;
            }

            int animalIndex = (i / 3 + (i > 4 ? 1 : 0)) * 4 + j / 2;

            if (animalIndex < 0 || animalIndex >= ANIMAL_SQUARES) {
                continue;
            }

            sf::Sprite animalSprite;
            animalSprite.setTexture(it->second);
            animalSprite.setScale(0.8f, 0.8f);
            animalSprite.setOrigin(
                animalSprite.getLocalBounds().width / 2.f,
                animalSprite.getLocalBounds().height / 2.f
            );
            animalSprite.setPosition(
                animalSquares_[animalIndex].getGlobalBounds().left,
                animalSquares_[animalIndex].getGlobalBounds().top
            );

            window_.draw(animalSprite);
        }
    }
}

void Game::drawFenceOverlays() {
    for (int i = 0; i < FENCE_COUNT; ++i) {
        sf::RectangleShape overlay;
        overlay.setPosition(fenceMenuSprites_[i].getPosition());
        overlay.setSize({
            fenceMenuSprites_[i].getGlobalBounds().width,
            fenceMenuSprites_[i].getGlobalBounds().height
        });

        if (fenceSelected_[i]) {
            overlay.setFillColor(selectedFenceColor_);
            window_.draw(overlay);
        }

        if (fencePlaced_[i]) {
            overlay.setFillColor(usedFenceColor_);
            window_.draw(overlay);
        } else if (!fenceUsable_[i]) {
            overlay.setFillColor(unusableFenceColor_);
            window_.draw(overlay);
        }
    }
}

void Game::drawLevelAndTimer() {
    sf::Text preLevelNumberText;
    preLevelNumberText.setString("Nivel:");
    preLevelNumberText.setCharacterSize(40);
    preLevelNumberText.setPosition({SCREEN_X - 180.f, 15.f});
    preLevelNumberText.setFillColor(sf::Color::White);
    preLevelNumberText.setOutlineThickness(2.f);
    preLevelNumberText.setOutlineColor(sf::Color::Black);
    preLevelNumberText.setStyle(sf::Text::Bold);
    preLevelNumberText.setFont(resources_.font);

    sf::Text levelNumberText;
    levelNumberText.setString(std::to_string(selectedLevel_));
    levelNumberText.setCharacterSize(40);
    levelNumberText.setPosition({SCREEN_X - 60.f, 15.f});
    levelNumberText.setFillColor(sf::Color::White);
    levelNumberText.setOutlineThickness(2.f);
    levelNumberText.setOutlineColor(sf::Color::Black);
    levelNumberText.setStyle(sf::Text::Bold);
    levelNumberText.setFont(resources_.font);

    window_.draw(levelNumberText);
    window_.draw(preLevelNumberText);

    sf::Time elapsedTime = clock_.getElapsedTime();

    sf::Text timerText;
    timerText.setString(std::to_string(static_cast<int>(elapsedTime.asSeconds())));
    timerText.setFont(resources_.font);
    timerText.setPosition(
        elapsedTime.asSeconds() >= 10 ? SCREEN_X - 155.f : SCREEN_X - 130.f,
        SCREEN_Y - 100.f
    );
    timerText.setCharacterSize(50);
    timerText.setFillColor(sf::Color::Black);
    timerText.setStyle(sf::Text::Bold);

    sf::Text timerTextLimit;
    timerTextLimit.setString("/" + std::to_string(static_cast<int>(timeLimit_)));
    timerTextLimit.setFont(resources_.font);
    timerTextLimit.setPosition(SCREEN_X - 100.f, SCREEN_Y - 100.f);
    timerTextLimit.setCharacterSize(50);
    timerTextLimit.setFillColor(sf::Color::Black);
    timerTextLimit.setStyle(sf::Text::Bold);

    window_.draw(timerTextLimit);
    window_.draw(timerText);
}

void Game::drawBackButtonForPlaying() {
    sf::Sprite backButton(resources_.backButton);
    backButton.setScale(0.5f, 0.5f);
    backButton.setPosition(20.f, 20.f);
    window_.draw(backButton);

    if (containsMouse(backButton.getGlobalBounds(), mousePos_)) {
        window_.setMouseCursor(cursorHand_);

        if (leftClicked_) {
            resources_.buttonMusic.play();
            goToLevelMenu();
            leftClicked_ = false;
        }
    } else {
        window_.setMouseCursor(cursorDefault_);
    }
}

void Game::drawSolveButtonForPlaying() {
    sf::RectangleShape solveButton({120.f, 50.f});
    solveButton.setFillColor(buttonColor_);
    solveButton.setPosition(SCREEN_X - 170.f, SCREEN_Y - 170.f);

    sf::Text solveText;
    solveText.setFont(resources_.font);
    solveText.setString("Solve");
    solveText.setCharacterSize(25);
    solveText.setFillColor(sf::Color::White);
    solveText.setStyle(sf::Text::Bold);

    sf::FloatRect buttonBounds = solveButton.getGlobalBounds();
    sf::FloatRect textBounds = solveText.getLocalBounds();

    solveText.setPosition(
        buttonBounds.left + (buttonBounds.width - textBounds.width) / 2.f - textBounds.left,
        buttonBounds.top + (buttonBounds.height - textBounds.height) / 2.f - textBounds.top - 3.f
    );

    window_.draw(solveButton);
    window_.draw(solveText);

    if (solveButton.getGlobalBounds().contains(
        static_cast<float>(mousePos_.x),
        static_cast<float>(mousePos_.y)
    )) {
        window_.setMouseCursor(cursorHand_);

        if (leftClicked_) {
            resources_.buttonMusic.play();
            solveCurrentLevel();
            leftClicked_ = false;
        }
    }
}