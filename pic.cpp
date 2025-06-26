#include<iostream>
#include<graphics.h>

void typeText(int x, int y, const char* text, int charDelay = 5) {
    int count = 0;
    char temp[2] = "";  // to hold single character as a string
    for (int i = 0; i < strlen(text); i++) {
        temp[0] = text[i];
        outtextxy(x, y, temp);
        count++;
        x += textwidth(temp);  // move to next character position
        delay(charDelay);      // delay in milliseconds
        if(count > 56 || (temp[0] == ' ' && count > 50)){
            x = 20;
            y += 20;
            count = 0;
        }
    }

}
int main() {
    initwindow(1024, 682, "pic");
    setcolor(BLACK);
    cleardevice();

    readimagefile("intro.jpeg", 0, 0, 1024, 682); // Background image

    // Dialog box
    setfillstyle(SOLID_FILL, BLACK);
    bar(10, 472, 1014, 672);
    setlinestyle(0, 1, 6);
    setcolor(WHITE);
    rectangle(10, 472, 1014, 672);

    // Display text
    setcolor(WHITE);
    settextstyle(1, HORIZ_DIR, 2);
    typeText(20, 482, "123456789012345678901234567890123456789012345678901234567 A light flashes...",10);

    setfillstyle(SOLID_FILL, RED);
    int triangle[] = {960, 640, 970, 660, 980, 640, 960, 640};
    fillpoly(3, triangle);

    char x;
    while(x != '\r'){
        x = getch();
    }


    getch();
    closegraph();
}
