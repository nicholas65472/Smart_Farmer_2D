#include "Resources.h"
#include "Board.h"

#include <array>
#include <stdexcept>

void Resources::loadTexture(sf::Texture& texture, const std::string& path) {
    if (!texture.loadFromFile(path)) {
        throw std::runtime_error("Nu pot incarca textura: " + path);
    }
}

void Resources::loadMusic(sf::Music& music, const std::string& path, float volume, bool loop) {
    if (!music.openFromFile(path)) {
        throw std::runtime_error("Nu pot incarca sunetul: " + path);
    }

    music.setVolume(volume);
    music.setLoop(loop);
}

void Resources::loadAll() {
    if (!font.loadFromFile(Path::FONT_CONVERGENCE)) {
        throw std::runtime_error("Nu pot incarca fontul.");
    }

    loadMusic(mainMusic, Path::MAIN_MENU_SONG, 40.f, true);
    loadMusic(winMusic, Path::VICTORY_SOUND, 50.f, false);
    loadMusic(loseMusic, Path::LOSE_SOUND, 50.f, false);
    loadMusic(fencePlaceMusic, Path::FENCE_PLACE_SOUND, 70.f, false);
    loadMusic(fenceUnplaceMusic, Path::FENCE_UNPLACE_SOUND, 50.f, false);
    loadMusic(buttonMusic, Path::BUTTON_SOUND, 70.f, false);

    loadTexture(mainBackground, Path::MAIN_BACKGROUND);
    loadTexture(levelsBackground, Path::LEVELS_BACKGROUND);
    loadTexture(helpBackground, Path::HELP_BACKGROUND);
    loadTexture(winLoseBackground, Path::WIN_LOSE_BACKGROUND);
    loadTexture(boardTexture, Path::BOARD);

    loadTexture(levelIcon, Path::LEVEL_ICON);
    loadTexture(levelIconHover, Path::LEVEL_ICON_HOVER);
    loadTexture(backButton, Path::BACK_BUTTON);
    loadTexture(checkmark, Path::CHECKMARK);
    loadTexture(credits, Path::CREDITS);
    loadTexture(soundOn, Path::SOUND_ON);
    loadTexture(soundOff, Path::SOUND_OFF);

    const std::array<const char*, FENCE_COUNT> fencePaths = {
        Path::FENCE_0,
        Path::FENCE_1,
        Path::FENCE_2,
        Path::FENCE_3,
        Path::FENCE_4,
        Path::FENCE_5,
        Path::FENCE_6,
        Path::FENCE_7
    };

    for (int i = 0; i < FENCE_COUNT; ++i) {
        loadTexture(fenceTextures[i], fencePaths[i]);
    }

    const std::array<int, 7> animalIds = {4, -4, 5, -5, 6, 7, 8};

    for (int id : animalIds) {
        loadTexture(animalTextures[id], animalTexturePath(id));
    }
}