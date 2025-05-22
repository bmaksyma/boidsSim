#include "SceneManager.h"
#include "Window.h"
#include "Button.h"

void UIManager::render(unsigned char* fb) {
    windows[active_index].drawWindow(fb);
}

void UIManager::switchTo(int index) {
    if (index >= 0 && index < windows.size()) {
        active_index = index;
    }
}

void UIManager::handleInput(uint8_t knob_val) {
    static uint8_t last_val = 0;
    int delta = (int)(knob_val - last_val);
    last_val = knob_val;

    if (delta > 0) {
        windows[active_index].nextBtn();
    } else if (delta < 0) {
        windows[active_index].previousBtn();
    }
}

void UIManager::activateSelected() {
    windows[active_index].activateSelected();
}
