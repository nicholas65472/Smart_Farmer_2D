#include "Game.h"

#include <stdexcept>

Game::Game()
    : window_(sf::VideoMode(SCREEN_X, SCREEN_Y), "Smart Farmer") {
    resources_.loadAll();

    if (!cursorHand_.loadFromSystem(sf::Cursor::Hand)) {
        throw std::runtime_error("Nu pot incarca cursorul hand.");
    }

    if (!cursorDefault_.loadFromSystem(sf::Cursor::Arrow)) {
        throw std::runtime_error("Nu pot incarca cursorul default.");
    }

    fenceOriginSquares_.fill(-1);
    fencePlaced_.fill(false);
    fenceUsable_.fill(true);
    fenceSelected_.fill(false);
    levelCompleted_.fill(false);

    initializeSprites();
    initializeClickableAreas();
}

void Game::run() {
    resources_.mainMusic.play();

    while (window_.isOpen() && state_ != State::Exit) {
        mousePos_ = sf::Mouse::getPosition(window_);
        leftClicked_ = false;

        handleEvents();
        update();
        render();
    }
}

void Game::handleEvents() {
    sf::Event event{};

    while (window_.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            window_.close();
        }

        if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
            leftClicked_ = true;
            mousePos_ = {event.mouseButton.x, event.mouseButton.y};

            if (state_ == State::Playing) {
                for (int i = 0; i < FENCE_COUNT; ++i) {
                    if (fenceMenuSprites_[i].getGlobalBounds().contains(
                            static_cast<float>(mousePos_.x),
                            static_cast<float>(mousePos_.y)
                        ) &&
                        !fencePlaced_[i] &&
                        fenceUsable_[i]) {
                        selectFence(i);
                        return;
                    }
                }

                if (selectedFence_ != -1) {
                    for (int i = 0; i < BOARD_FENCE_SQUARES; ++i) {
                        if (fenceSquares_[i].getGlobalBounds().contains(
                                static_cast<float>(mousePos_.x),
                                static_cast<float>(mousePos_.y)
                            )) {
                            tryPlaceSelectedFence(i);
                            return;
                        }
                    }
                } else {
                    for (int i = 0; i < FENCE_COUNT; ++i) {
                        if (fencePlaced_[i] &&
                            fenceBoardSprites_[i].getGlobalBounds().contains(
                                static_cast<float>(mousePos_.x),
                                static_cast<float>(mousePos_.y)
                            )) {
                            tryRemoveFence(i);
                            return;
                        }
                    }
                }
            }
        }
    }
}

void Game::update() {
    if ((state_ == State::MainMenu ||
         state_ == State::LevelMenu ||
         state_ == State::HowToPlay ||
         state_ == State::Playing) &&
        resources_.mainMusic.getStatus() != sf::SoundSource::Playing) {
        resources_.winMusic.stop();
        resources_.loseMusic.stop();
        resources_.mainMusic.play();
        winMusicPlayed_ = false;
        loseMusicPlayed_ = false;
    }

    if (state_ == State::Playing && clock_.getElapsedTime().asSeconds() > timeLimit_) {
        resultWin_ = 0;
        state_ = State::Result;
    }

    if (pendingWin_ && winDelayClock_.getElapsedTime().asSeconds() >= 1.f) {
        pendingWin_ = false;
        resultWin_ = 1;
        state_ = State::Result;
    }
}

void Game::render() {
    window_.clear();

    switch (state_) {
        case State::MainMenu:
            renderMainMenu();
            break;

        case State::LevelMenu:
            renderLevelMenu();
            break;

        case State::HowToPlay:
            renderHowToPlay();
            break;

        case State::Playing:
            renderPlaying();
            break;

        case State::Result:
            renderResult();
            break;

        case State::Exit:
            break;
    }

    window_.display();
}

void Game::initializeSprites() {
    mainBackground_.setTexture(resources_.mainBackground);

    levelsBackground_.setTexture(resources_.levelsBackground);
    const sf::FloatRect bgBounds = levelsBackground_.getLocalBounds();
    levelsBackground_.setScale(SCREEN_X / bgBounds.width, SCREEN_Y / bgBounds.height);
    levelsBackground_.setPosition(0.f, 0.f);

    helpBackground_.setTexture(resources_.helpBackground);
    helpBackground_.setScale(1.25f, 1.40f);

    boardSprite_.setTexture(resources_.boardTexture);
    boardSprite_.setScale(1.f, 1.f);
    boardSprite_.setPosition(80.f, 30.f);

    soundSprite_.setTexture(resources_.soundOn);
    soundSprite_.setScale(1.25f, 1.25f);
    soundSprite_.setPosition(30.f, 30.f);

    for (int i = 0; i < FENCE_COUNT; ++i) {
        fenceMenuSprites_[i].setTexture(resources_.fenceTextures[i]);
        fenceMenuSprites_[i].setScale(0.55f, 0.65f);
        fenceBoardSprites_[i].setTexture(resources_.fenceTextures[i]);
    }

    fenceMenuSprites_[0].setPosition(150.f, 10.f);
    fenceMenuSprites_[2].setPosition(SCREEN_X - 370.f, 10.f);

    fenceMenuSprites_[1].setPosition(20.f, 120.f);
    fenceMenuSprites_[3].setPosition(20.f, 370.f);

    fenceMenuSprites_[4].setPosition(200.f, SCREEN_Y - 85.f);
    fenceMenuSprites_[5].setPosition(SCREEN_X - 420.f, SCREEN_Y - 100.f);

    fenceMenuSprites_[6].setPosition(SCREEN_X - 160.f, 210.f);
    fenceMenuSprites_[7].setPosition(SCREEN_X - 90.f, 260.f);
}

void Game::initializeClickableAreas() {
    for (int i = 0; i < BOARD_FENCE_SQUARES; ++i) {
        fenceSquares_[i].setFillColor(sf::Color::Transparent);

        if ((i >= 4 && i <= 6) || (i >= 9 && i <= 11)) {
            fenceSquares_[i].setSize({70.f, 70.f});
        } else {
            fenceSquares_[i].setSize({30.f, 30.f});
        }
    }

    fenceSquares_[0].setPosition(310.f, 155.f);
    fenceSquares_[1].setPosition(435.f, 155.f);
    fenceSquares_[2].setPosition(555.f, 150.f);
    fenceSquares_[3].setPosition(210.f, 270.f);
    fenceSquares_[4].setPosition(290.f, 240.f);
    fenceSquares_[5].setPosition(415.f, 245.f);
    fenceSquares_[6].setPosition(540.f, 245.f);
    fenceSquares_[7].setPosition(665.f, 250.f);
    fenceSquares_[8].setPosition(210.f, 390.f);
    fenceSquares_[9].setPosition(290.f, 365.f);
    fenceSquares_[10].setPosition(420.f, 365.f);
    fenceSquares_[11].setPosition(545.f, 365.f);
    fenceSquares_[12].setPosition(670.f, 390.f);
    fenceSquares_[13].setPosition(310.f, SCREEN_Y - 210.f);
    fenceSquares_[14].setPosition(430.f, SCREEN_Y - 210.f);
    fenceSquares_[15].setPosition(560.f, SCREEN_Y - 210.f);

    for (int i = 0; i < ANIMAL_SQUARES; ++i) {
        animalSquares_[i].setFillColor(sf::Color::Transparent);
        animalSquares_[i].setSize({40.f, 40.f});
    }

    animalSquares_[0].setPosition(263.f, 223.f);
    animalSquares_[1].setPosition(390.f, 220.f);
    animalSquares_[2].setPosition(515.f, 220.f);
    animalSquares_[3].setPosition(635.f, 220.f);
    animalSquares_[4].setPosition(260.f, 350.f);
    animalSquares_[5].setPosition(390.f, 350.f);
    animalSquares_[6].setPosition(515.f, 350.f);
    animalSquares_[7].setPosition(645.f, 350.f);
    animalSquares_[8].setPosition(265.f, 470.f);
    animalSquares_[9].setPosition(390.f, 465.f);
    animalSquares_[10].setPosition(520.f, 465.f);
    animalSquares_[11].setPosition(645.f, 465.f);
}

void Game::goToLevel(int level) {
    selectedLevel_ = level;
    board_.loadLevel(level, timeLimit_);
    resetCurrentLevelState();
    clock_.restart();
    state_ = State::Playing;
}

void Game::goToLevelMenu() {
    selectedLevel_ = -1;
    board_.clear();
    resetCurrentLevelState();
    state_ = State::LevelMenu;
}

void Game::resetCurrentLevelState() {
    selectedFence_ = -1;
    fenceOriginSquares_.fill(-1);
    fencePlaced_.fill(false);
    fenceUsable_.fill(true);
    fenceSelected_.fill(false);
    pendingWin_ = false;
}