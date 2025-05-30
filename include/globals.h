#ifndef GLOBALS_H
#define GLOBALS_H

#include <cstdint>
#include <vector>
#include <string>
#include "fonts/font_types.h"

extern const int WIDTH;
extern const int HEIGHT;
extern const int PREYS_COUNT;
extern const int PREDATORS_COUNT;
extern const float PREY_SIZE;
extern const float PREDATOR_SIZE;
extern const float KILL_DISTANCE;

extern float MAX_SPEED;
extern float MAX_FORCE;
extern float PERCEPTION_RADIUS;
extern float SEPARATION_RADIUS;
extern float FOOD_ATTRACTION;
extern float ALIGNMENT_WEIGHT;
extern float COHESION_WEIGHT;
extern float SEPARATION_WEIGHT;
extern float MARGIN_SIZE;
extern float TURN_FORCE;

extern uint16_t PREY_COLOR;
extern uint16_t PREDATOR_COLOR;
extern font_descriptor_t* CUR_FONT;

extern const uint16_t WHITE_THEME_COLOR1;
extern const uint16_t WHITE_THEME_COLOR2;
extern const uint16_t WHITE_THEME_COLOR3;
extern const uint16_t WHITE_THEME_COLOR4;

extern const uint16_t DARK_THEME_COLOR1;
extern const uint16_t DARK_THEME_COLOR2;
extern const uint16_t DARK_THEME_COLOR3;
extern const uint16_t DARK_THEME_COLOR4;

extern std::vector<uint16_t> whiteThemeColors;
extern std::vector<uint16_t> darkThemeColors;
extern std::vector<uint16_t> buttonColors;
extern std::vector<std::string> themeNames;
extern std::vector<font_descriptor_t*> availableFonts;
extern int currentThemeIndex;
extern uint16_t currentBackgroundColor;

extern unsigned short* fb;

#endif 
