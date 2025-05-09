#include <string>
#include <functional>
#include <cstdint>
#include "Button.h"

void Button::activate() {
    if (action) {
        action();
    }
}
