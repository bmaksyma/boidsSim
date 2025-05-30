#include "globals.h"
#include "fonts/font_manager.h"

const int WIDTH = 480;
const int HEIGHT = 320;
const int PREYS_COUNT = 100;
const int PREDATORS_COUNT = 3;
const float PREY_SIZE = 10.0f;
const float PREDATOR_SIZE = 20.0f;
const float KILL_DISTANCE = 5.0f;

float MAX_SPEED = 10.0f;
float MAX_FORCE = 1.3f;
float PERCEPTION_RADIUS = 25.0f;
float SEPARATION_RADIUS = 10.0f;
float FOOD_ATTRACTION = 1.0f;
float ALIGNMENT_WEIGHT = 1.0f;
float COHESION_WEIGHT = 1.0f;
float SEPARATION_WEIGHT = 1.5f;
float MARGIN_SIZE = 40.0f;
float TURN_FORCE = 5.0f;

uint16_t PREY_COLOR = 0x001F;
uint16_t PREDATOR_COLOR = 0xF800;
font_descriptor_t* CUR_FONT = &font_rom8x16;

const uint16_t WHITE_THEME_COLOR1 = 0xC618;// light gray
const uint16_t WHITE_THEME_COLOR2 = 0x4FE0;// light blue
const uint16_t WHITE_THEME_COLOR3 = 0x001F;// dark blue
const uint16_t WHITE_THEME_COLOR4 = 0xFFE0;// light yellow

const uint16_t DARK_THEME_COLOR1 = 0x4208;// dark gray
const uint16_t DARK_THEME_COLOR2 = 0x001F;// dark blue
const uint16_t DARK_THEME_COLOR3 = 0x07E0;// light green
const uint16_t DARK_THEME_COLOR4 = 0x8010;// dark red

std::vector<uint16_t> whiteThemeColors = {
    WHITE_THEME_COLOR1, WHITE_THEME_COLOR2, WHITE_THEME_COLOR3, WHITE_THEME_COLOR4
};

std::vector<uint16_t> darkThemeColors = {
    DARK_THEME_COLOR1, DARK_THEME_COLOR2, DARK_THEME_COLOR3, DARK_THEME_COLOR4
};

std::vector<uint16_t> buttonColors = whiteThemeColors;
std::vector<std::string> themeNames = {"White", "Dark"};
std::vector<font_descriptor_t*> availableFonts = {
    &font_rom8x16, &font_winFreeSystem14x16
};

int currentThemeIndex = 0;
uint16_t currentBackgroundColor = 0xFFFF;


unsigned short* fb = nullptr;
