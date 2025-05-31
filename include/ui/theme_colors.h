#ifndef THEME_COLORS_H
#define THEME_COLORS_H

#include <cstdint>
#include <vector>
#include <string>

/* White Theme Colors */
extern const uint16_t WHITE_THEME_COLOR1;
extern const uint16_t WHITE_THEME_COLOR2;
extern const uint16_t WHITE_THEME_COLOR3;
extern const uint16_t WHITE_THEME_COLOR4;

/* Dark Theme Colors */
extern const uint16_t DARK_THEME_COLOR1;
extern const uint16_t DARK_THEME_COLOR2;
extern const uint16_t DARK_THEME_COLOR3;
extern const uint16_t DARK_THEME_COLOR4;

extern std::vector<uint16_t> whiteThemeColors;
extern std::vector<uint16_t> darkThemeColors;
extern std::vector<std::string> themeNames;
extern int currentThemeIndex;
extern uint16_t currentBackgroundColor;

void switchToTheme(int themeIndex);
std::vector<uint16_t>& getCurrentThemeColors();
uint16_t getCurrentBackgroundColor();

#endif