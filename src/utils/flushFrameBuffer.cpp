#include <vector>
#include "Button.h"
#include "Window.h"
#include "draw_utils.h"

#include "mzapo_parlcd.h"

#define HEIGHT 320
#define WIDTH 480


void flushFramebuffer(unsigned char* parlcd_mem_base, unsigned short* fb) {
    parlcd_write_cmd(parlcd_mem_base, 0x2C);
    for (int i = 0; i < WIDTH * HEIGHT; ++i) {
        parlcd_write_data(parlcd_mem_base, fb[i]);
    }
}