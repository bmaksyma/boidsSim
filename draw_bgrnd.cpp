#define WIDTH 480
#define HEIGHT 320

void draw_background(unsigned short* fb, unsigned short background_color) {
    for (int i = 0; i < WIDTH * HEIGHT; i++) {
        fb[i] = background_color;
    }
}
