#pragma once

constexpr int SCREEN_X = 900;
constexpr int SCREEN_Y = 700;
constexpr int MIDDLE_X = SCREEN_X / 2;
constexpr int MIDDLE_Y = SCREEN_Y / 2;

constexpr int BOARD_ROWS = 7;
constexpr int BOARD_COLS = 9;
constexpr int LEVEL_COUNT = 60;
constexpr int FENCE_COUNT = 8;
constexpr int BOARD_FENCE_SQUARES = 16;
constexpr int ANIMAL_SQUARES = 12;

namespace Path {
    constexpr const char* COW1 = "../images/Board/vaca1 (1).png";
    constexpr const char* COW2 = "../images/Board/vaca2 (1).png";
    constexpr const char* PIG = "../images/Board/porc1 (1).png";
    constexpr const char* SHEEP = "../images/Board/oaie (1).png";
    constexpr const char* HORSE1 = "../images/Board/cal1 (1).png";
    constexpr const char* HORSE2 = "../images/Board/cal2 (1).png";
    constexpr const char* WATERPOT = "../images/Board/waterPot.png";

    constexpr const char* FONT_CONVERGENCE = "../fonts/Convergence.ttf";

    constexpr const char* MAIN_MENU_SONG = "../music/MainMenuSong.mp3";
    constexpr const char* VICTORY_SOUND = "../music/victory (1).wav";
    constexpr const char* LOSE_SOUND = "../music/lose.wav";
    constexpr const char* FENCE_PLACE_SOUND = "../music/buttonSound.wav";
    constexpr const char* FENCE_UNPLACE_SOUND = "../music/wood_unplace.wav";
    constexpr const char* BUTTON_SOUND = "../music/press_button.wav";

    constexpr const char* MAIN_BACKGROUND = "../images/main_screen/SF_background.jpg";

    constexpr const char* LEVELS_BACKGROUND = "../images/Board/levelBackground.jpg";
    constexpr const char* HELP_BACKGROUND = "../images/Board/Help.jpg";
    constexpr const char* WIN_LOSE_BACKGROUND = "../images/Board/WinLose.jpg";
    constexpr const char* BOARD = "../images/Board/board (1).png";

    constexpr const char* LEVEL_ICON = "../images/main_screen/level_icon.png";
    constexpr const char* LEVEL_ICON_HOVER = "../images/main_screen/hov_level_icon.png";
    constexpr const char* BACK_BUTTON = "../images/main_screen/go_back_arrow.png";
    constexpr const char* CHECKMARK = "../images/main_screen/checkmark (2).png";
    constexpr const char* CREDITS = "../images/main_screen/credits.png";
    constexpr const char* SOUND_ON = "../images/main_screen/sound_on.png";
    constexpr const char* SOUND_OFF = "../images/main_screen/sound_off.png";

    constexpr const char* FENCE_0 = "../images/Board/GardColt1.png";
    constexpr const char* FENCE_1 = "../images/Board/GardColt2.png";
    constexpr const char* FENCE_2 = "../images/Board/GardColt3 (1).png";
    constexpr const char* FENCE_3 = "../images/Board/GardColt4 (1).png";
    constexpr const char* FENCE_4 = "../images/Board/GardOrizontalL2 (1).png";
    constexpr const char* FENCE_5 = "../images/Board/GardOrizontalL3 (1).png";
    constexpr const char* FENCE_6 = "../images/Board/GardVerticalL3 (1).png";
    constexpr const char* FENCE_7 = "../images/Board/GardVerticalL2 (1).png";
}