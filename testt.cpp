#include <graphics.h>
#include <dos.h>

// Function to blend two colors with opacity (opacity in 0.0 to 1.0)
int blendColor(int fgColor, int bgColor, float opacity) {
    int r1 = fgColor;
    int g1 = fgColor;
    int b1 = fgColor;

    int r2 = bgColor;
    int g2 = bgColor;
    int b2 = bgColor;

    int r = (int)(opacity * r1 + (1 - opacity) * r2);
    int g = (int)(opacity * g1 + (1 - opacity) * g2);
    int b = (int)(opacity * b1 + (1 - opacity) * b2);

    return COLOR(r, g, b);
}

int main() {
    int gd = DETECT, gm;
    initgraph(&gd, &gm, "");

    // Define box coordinates
    int left = 100, top = 100, right = 300, bottom = 200;

    // Background color (assume black)
    setbkcolor(BLACK);
    cleardevice();

    // Target box color (e.g., red)
    int boxColor = RED;
    float opacity = 0.6;

    // Simulate opacity by blending
    for (int y = top; y <= bottom; y++) {
        for (int x = left; x <= right; x++) {
            int bgColor = getpixel(x, y);
            int blended = blendColor(boxColor, bgColor, opacity);
            putpixel(x, y, blended);
        }
    }

    getch();
    closegraph();
    return 0;
}
