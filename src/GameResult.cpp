#include "Game.h"

static bool containsMouse(const sf::FloatRect& bounds, const sf::Vector2i& mouse) {
    return bounds.contains(static_cast<float>(mouse.x), static_cast<float>(mouse.y));
}

void Game::renderResult() {
    resources_.mainMusic.stop();

    sf::Sprite backgroundSprite(resources_.winLoseBackground);
    backgroundSprite.setScale(
        SCREEN_X / backgroundSprite.getGlobalBounds().width,
        SCREEN_Y / backgroundSprite.getGlobalBounds().height
    );
    window_.draw(backgroundSprite);

    sf::RectangleShape menuButton({200.f, 60.f});
    menuButton.setFillColor(buttonColor_);
    menuButton.setPosition(SCREEN_X / 2.f - menuButton.getSize().x / 2.f, 350.f);

    sf::Text menuButtonText;
    menuButtonText.setFont(resources_.font);
    menuButtonText.setString("Levels Menu");
    menuButtonText.setCharacterSize(30);
    menuButtonText.setFillColor(sf::Color::White);
    menuButtonText.setPosition(
        SCREEN_X / 2.f - menuButtonText.getGlobalBounds().width / 2.f,
        350.f + menuButton.getSize().y / 2.f - 20.f
    );

    bool hover = false;

    if (resultWin_ == 1) {
        if (!winMusicPlayed_) {
            winMusicPlayed_ = true;
            resources_.winMusic.play();
        }

        sf::Text winText;
        winText.setFont(resources_.font);
        winText.setString("You Won!");
        winText.setCharacterSize(80);
        winText.setFillColor(sf::Color::White);
        winText.setOutlineColor(sf::Color::Black);
        winText.setOutlineThickness(2.f);
        winText.setStyle(sf::Text::Bold);
        winText.setPosition(SCREEN_X / 2.f - winText.getGlobalBounds().width / 2.f, 100.f);
        window_.draw(winText);

        sf::RectangleShape nextButton({200.f, 60.f});
        nextButton.setFillColor(buttonColor_);
        nextButton.setPosition(SCREEN_X / 2.f - nextButton.getSize().x / 2.f, 250.f);

        sf::Text nextButtonText;
        nextButtonText.setFont(resources_.font);
        nextButtonText.setString("Next Level");
        nextButtonText.setCharacterSize(30);
        nextButtonText.setFillColor(sf::Color::White);
        nextButtonText.setPosition(
            SCREEN_X / 2.f - nextButtonText.getGlobalBounds().width / 2.f,
            250.f + nextButton.getSize().y / 2.f - 20.f
        );

        if (selectedLevel_ < LEVEL_COUNT) {
            window_.draw(nextButton);
            window_.draw(nextButtonText);
        }

        window_.draw(menuButton);
        window_.draw(menuButtonText);

        if (selectedLevel_ < LEVEL_COUNT && containsMouse(nextButton.getGlobalBounds(), mousePos_)) {
            hover = true;

            if (leftClicked_) {
                resources_.buttonMusic.play();
                winMusicPlayed_ = false;
                goToLevel(selectedLevel_ + 1);
                leftClicked_ = false;
            }
        } else if (containsMouse(menuButton.getGlobalBounds(), mousePos_)) {
            hover = true;

            if (leftClicked_) {
                resources_.buttonMusic.play();
                goToLevelMenu();
                leftClicked_ = false;
            }
        }
    } else {
        if (!loseMusicPlayed_) {
            loseMusicPlayed_ = true;
            resources_.loseMusic.play();
        }

        sf::Text loseText;
        loseText.setFont(resources_.font);
        loseText.setString("You Lose!");
        loseText.setCharacterSize(60);
        loseText.setFillColor(sf::Color::White);
        loseText.setOutlineColor(sf::Color::Black);
        loseText.setOutlineThickness(2.f);
        loseText.setStyle(sf::Text::Bold);
        loseText.setPosition(SCREEN_X / 2.f - loseText.getGlobalBounds().width / 2.f, 100.f);
        window_.draw(loseText);

        sf::RectangleShape retryButton({200.f, 60.f});
        retryButton.setFillColor(buttonColor_);
        retryButton.setPosition(SCREEN_X / 2.f - retryButton.getSize().x / 2.f, 250.f);
        window_.draw(retryButton);

        sf::Text retryButtonText;
        retryButtonText.setFont(resources_.font);
        retryButtonText.setString("Retry");
        retryButtonText.setCharacterSize(30);
        retryButtonText.setFillColor(sf::Color::White);
        retryButtonText.setPosition(
            SCREEN_X / 2.f - retryButtonText.getGlobalBounds().width / 2.f,
            250.f + retryButton.getSize().y / 2.f - 20.f
        );
        window_.draw(retryButtonText);

        window_.draw(menuButton);
        window_.draw(menuButtonText);

        if (containsMouse(retryButton.getGlobalBounds(), mousePos_)) {
            hover = true;

            if (leftClicked_) {
                resources_.buttonMusic.play();
                loseMusicPlayed_ = false;
                goToLevel(selectedLevel_);
                leftClicked_ = false;
            }
        } else if (containsMouse(menuButton.getGlobalBounds(), mousePos_)) {
            hover = true;

            if (leftClicked_) {
                resources_.buttonMusic.play();
                goToLevelMenu();
                leftClicked_ = false;
            }
        }
    }

    window_.setMouseCursor(hover ? cursorHand_ : cursorDefault_);
}