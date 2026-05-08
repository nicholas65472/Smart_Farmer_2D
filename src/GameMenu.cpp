#include "Game.h"

#include <string>

static bool containsMouse(const sf::FloatRect& bounds, const sf::Vector2i& mouse) {
    return bounds.contains(static_cast<float>(mouse.x), static_cast<float>(mouse.y));
}

void Game::renderMainMenu() {
    window_.draw(mainBackground_);

    sf::Text gameTitle("Smart Farmer", resources_.font);
    gameTitle.setCharacterSize(70);
    gameTitle.setStyle(sf::Text::Bold);
    gameTitle.setFillColor(sf::Color::White);
    gameTitle.setPosition(MIDDLE_X - 250.f, 50.f);
    window_.draw(gameTitle);

    sf::RectangleShape buttonStart({270.f, 70.f});
    buttonStart.setPosition(MIDDLE_X - 135.f, MIDDLE_Y - 100.f);
    buttonStart.setFillColor(buttonColor_);

    sf::Text textStart("Start", resources_.font);
    textStart.setCharacterSize(30);
    textStart.setStyle(sf::Text::Bold);
    textStart.setFillColor(sf::Color::White);
    textStart.setPosition(MIDDLE_X - 40.f, MIDDLE_Y - 85.f);

    sf::RectangleShape buttonHTP({270.f, 70.f});
    buttonHTP.setPosition(MIDDLE_X - 135.f, MIDDLE_Y);
    buttonHTP.setFillColor(buttonColor_);

    sf::Text textHTP("How to play", resources_.font);
    textHTP.setCharacterSize(30);
    textHTP.setStyle(sf::Text::Bold);
    textHTP.setFillColor(sf::Color::White);
    textHTP.setPosition(MIDDLE_X - 90.f, MIDDLE_Y + 12.5f);

    sf::RectangleShape buttonExit({270.f, 70.f});
    buttonExit.setPosition(MIDDLE_X - 135.f, MIDDLE_Y + 100.f);
    buttonExit.setFillColor(buttonColor_);

    sf::Text textExit("Exit", resources_.font);
    textExit.setCharacterSize(30);
    textExit.setStyle(sf::Text::Bold);
    textExit.setFillColor(sf::Color::White);
    textExit.setPosition(MIDDLE_X - 30.f, MIDDLE_Y + 112.5f);

    soundSprite_.setTexture(soundOn_ ? resources_.soundOn : resources_.soundOff);

    window_.draw(buttonStart);
    window_.draw(textStart);
    window_.draw(buttonHTP);
    window_.draw(textHTP);
    window_.draw(buttonExit);
    window_.draw(textExit);
    window_.draw(soundSprite_);

    bool hover = false;

    if (containsMouse(soundSprite_.getGlobalBounds(), mousePos_)) {
        hover = true;

        if (leftClicked_) {
            soundOn_ = !soundOn_;

            resources_.mainMusic.setVolume(soundOn_ ? 40.f : 0.f);
            resources_.winMusic.setVolume(soundOn_ ? 50.f : 0.f);
            resources_.loseMusic.setVolume(soundOn_ ? 50.f : 0.f);
            resources_.fencePlaceMusic.setVolume(soundOn_ ? 70.f : 0.f);
            resources_.fenceUnplaceMusic.setVolume(soundOn_ ? 50.f : 0.f);
            resources_.buttonMusic.setVolume(soundOn_ ? 70.f : 0.f);

            leftClicked_ = false;
        }
    } else if (containsMouse(buttonStart.getGlobalBounds(), mousePos_)) {
        hover = true;

        if (leftClicked_) {
            resources_.buttonMusic.play();
            state_ = State::LevelMenu;
            leftClicked_ = false;
        }
    } else if (containsMouse(buttonHTP.getGlobalBounds(), mousePos_)) {
        hover = true;

        if (leftClicked_) {
            resources_.buttonMusic.play();
            state_ = State::HowToPlay;
            leftClicked_ = false;
        }
    } else if (containsMouse(buttonExit.getGlobalBounds(), mousePos_)) {
        hover = true;

        if (leftClicked_) {
            resources_.buttonMusic.play();
            state_ = State::Exit;
            leftClicked_ = false;
        }
    }

    window_.setMouseCursor(hover ? cursorHand_ : cursorDefault_);
}

void Game::renderLevelMenu() {
    window_.draw(levelsBackground_);

    constexpr int numColumns = 9;
    constexpr float offsetX = 85.f;
    constexpr float offsetY = 70.f;
    constexpr float startX = 60.f;
    constexpr float startY = 60.f;

    bool hover = false;

    for (int i = 0; i < LEVEL_COUNT; ++i) {
        int col = i % numColumns;
        int row = i / numColumns;

        sf::Sprite levelSprite(resources_.levelIcon);
        levelSprite.setPosition(startX + col * offsetX, startY + row * offsetY);

        if (containsMouse(levelSprite.getGlobalBounds(), mousePos_)) {
            levelSprite.setTexture(resources_.levelIconHover);
            hover = true;

            if (leftClicked_) {
                resources_.buttonMusic.play();
                goToLevel(i + 1);
                leftClicked_ = false;
            }
        }

        window_.draw(levelSprite);

        sf::Text levelNumber(std::to_string(i + 1), resources_.font, 18);
        levelNumber.setFillColor(sf::Color::White);
        levelNumber.setStyle(sf::Text::Bold);

        sf::FloatRect levelBounds = levelSprite.getGlobalBounds();
        sf::FloatRect textBounds = levelNumber.getLocalBounds();

        float textX = levelBounds.left + (levelBounds.width - textBounds.width) / 2.f - textBounds.left;
        float textY = levelBounds.top + (levelBounds.height - textBounds.height) / 2.f - textBounds.top;
        levelNumber.setPosition(textX, textY);

        window_.draw(levelNumber);

        if (levelCompleted_[i]) {
            sf::Sprite checkmark(resources_.checkmark);
            checkmark.setPosition(levelBounds.left + levelBounds.width - 50.f, levelBounds.top + 5.f);
            checkmark.setScale(0.1f, 0.1f);
            window_.draw(checkmark);
        }
    }

    sf::Sprite backButton(resources_.backButton);
    backButton.setScale(0.5f, 0.5f);
    backButton.setPosition(20.f, 20.f);
    window_.draw(backButton);

    if (containsMouse(backButton.getGlobalBounds(), mousePos_)) {
        hover = true;

        if (leftClicked_) {
            resources_.buttonMusic.play();
            state_ = State::MainMenu;
            leftClicked_ = false;
        }
    }

    window_.setMouseCursor(hover ? cursorHand_ : cursorDefault_);
}

void Game::renderHowToPlay() {
    window_.draw(helpBackground_);

    sf::Sprite backButton(resources_.backButton);
    backButton.setScale(0.5f, 0.5f);
    backButton.setPosition(20.f, 20.f);
    window_.draw(backButton);

    const float offsetYText = 20.f;

    auto makeHelpText = [&](const std::string& value, sf::Vector2f position, unsigned int size) {
        sf::Text text(value, resources_.font, size);
        text.setPosition(position);
        text.setFillColor(sf::Color::White);
        text.setOutlineColor(sf::Color::Black);
        text.setOutlineThickness(2.f);
        return text;
    };

    sf::Text textH1Obj = makeHelpText("Obiectiv: ", {50.f, offsetYText + 120.f}, 40);
    sf::Text textP1Obj1 = makeHelpText("asaza gardurile pe tabla astfel incat animalele sa fie", {230.f, offsetYText + 133.f}, 25);
    sf::Text textP1Obj2 = makeHelpText("separate corect in tarcuri. Fiecare tip de animal trebuie", {50.f, offsetYText + 173.f}, 25);
    sf::Text textP1Obj3 = makeHelpText("sa ajunga in propriul tarc, fara sa fie amestecat cu alt tip.", {50.f, offsetYText + 213.f}, 25);

    sf::Text textH1Reg = makeHelpText("Reguli: ", {50.f, offsetYText + 263.f}, 40);
    sf::Text textP1Reg1 = makeHelpText("Animalele de acelasi fel trebuie sa ramana impreuna.", {190.f, offsetYText + 275.f}, 25);
    sf::Text textP1Reg2 = makeHelpText("Animalele diferite nu au voie sa imparta acelasi tarc.", {50.f, offsetYText + 315.f}, 25);
    sf::Text textP1Reg3 = makeHelpText("Fiecare tarc trebuie sa contina cel putin o sursa de apa.", {50.f, offsetYText + 350.f}, 25);

    sf::Text textH1Rez = makeHelpText("Cum joci: ", {50.f, offsetYText + 390.f}, 40);
    sf::Text textP1Rez1 = makeHelpText("alege un gard din marginea ecranului si apasa pe tabla", {260.f, offsetYText + 405.f}, 25);
    sf::Text textP1Rez2 = makeHelpText("pentru a-l aseza. Daca vrei sa modifici solutia, apasa pe", {50.f, offsetYText + 445.f}, 25);
    sf::Text textP1Rez3 = makeHelpText("un gard deja pus pentru a-l elimina si incearca alta varianta.", {50.f, offsetYText + 485.f}, 25);

    textH1Obj.setStyle(sf::Text::Bold);
    textH1Reg.setStyle(sf::Text::Bold);
    textH1Rez.setStyle(sf::Text::Bold);

    window_.draw(textH1Obj);
    window_.draw(textP1Obj1);
    window_.draw(textP1Obj2);
    window_.draw(textP1Obj3);
    window_.draw(textH1Reg);
    window_.draw(textP1Reg1);
    window_.draw(textP1Reg2);
    window_.draw(textP1Reg3);
    window_.draw(textH1Rez);
    window_.draw(textP1Rez1);
    window_.draw(textP1Rez2);
    window_.draw(textP1Rez3);

    if (containsMouse(backButton.getGlobalBounds(), mousePos_)) {
        window_.setMouseCursor(cursorHand_);

        if (leftClicked_) {
            resources_.buttonMusic.play();
            state_ = State::MainMenu;
            leftClicked_ = false;
        }
    } else {
        window_.setMouseCursor(cursorDefault_);
    }
}