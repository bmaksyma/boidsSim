
#include "Window.h"
#include <vector>

class UIManager {
public:
    std::vector<Window> windows;
    int active_index = 0;

    void render(unsigned char* fb);
    void switchTo(int index);
    void handleInput(uint8_t knob_val);
    void activateSelected();
};
