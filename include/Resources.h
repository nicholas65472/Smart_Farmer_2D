#pragma once

#include "Constants.h"
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <array>
#include <map>
#include <string>

class Resources {
public:
    void loadAll();

    sf::Font font;

    sf::Music mainMusic;
    sf::Music winMusic;
    sf::Music loseMusic;
    sf::Music fencePlaceMusic;
    sf::Music fenceUnplaceMusic;
    sf::Music buttonMusic;

    sf::Texture mainBackground;
    sf::Texture levelsBackground;
    sf::Texture helpBackground;
    sf::Texture winLoseBackground;
    sf::Texture boardTexture;

    sf::Texture levelIcon;
    sf::Texture levelIconHover;
    sf::Texture backButton;
    sf::Texture checkmark;
    sf::Texture credits;
    sf::Texture soundOn;
    sf::Texture soundOff;

    std::array<sf::Texture, FENCE_COUNT> fenceTextures;
    std::map<int, sf::Texture> animalTextures;

private:
    static void loadTexture(sf::Texture& texture, const std::string& path);
    static void loadMusic(sf::Music& music, const std::string& path, float volume, bool loop);
};