#include<iostream>
#include<string>
#include<unordered_map>
#include<vector>
#include<graphics.h>
#include<mmsystem.h>
#include<windows.h>
#include<fstream>
#pragma comment(lib, "winmm.lib")
using namespace std;

struct Scene{
    string description;
    string choicequestion;
    vector<string> choices;
    vector<string> nextScenes;
    string img;
    string audio;

};

void typeText(int x, int y, const char* text, int charDelay = 5) {
    int count = 0;
    char temp[2] = "";  // to hold single character as a string
    for (int i = 0; i < strlen(text); i++) {
        temp[0] = text[i];
        outtextxy(x, y, temp);
        count++;
        x += textwidth(temp);  // move to next character position
        delay(charDelay);      // delay in milliseconds
        if(count > 56 || (temp[0] == ' ' && count > 54)){
            x = 20;
            y += 20;
            count = 0;
        }
    }

}
class StoryGraph{
    unordered_map<string, Scene> scenes;

    public:
    void addScene(string id,string description, string choicequestion, vector<string> choices, vector<string> nextScenes){
        scenes[id] = {description,choicequestion, choices, nextScenes};
    }

    void play(string startID){
        string current = startID;
        while(true){
            cleardevice();
            Scene &scene = scenes[current];
            scene.img = current + ".jpeg";
            scene.audio = current + ".wav";
            scene.description = current + ".txt";

            backprep(scene);

            // Display text
             ifstream file(scene.description);
            string line;
            if(file.is_open()){
                getline(file, line);
            }
            file.close();

            settextstyle(1, HORIZ_DIR, 2);
            PlaySoundA(scene.audio.c_str(),NULL,SND_FILENAME | SND_ASYNC);
            typeText(20, 482, line.c_str(),20);
            setfillstyle(SOLID_FILL, RED);
            int triangle[] = {960, 640, 970, 660, 980, 640, 960, 640};
            fillpoly(3, triangle);

            char x;
            while(x != '\r'){
                x = getch();
            }

            int choice;
            choice = choicebox(scene);
            current = scene.nextScenes[choice - 1];
        }
    
    }

    void backprep(Scene &scene){
            //Display Image
            readimagefile(scene.img.c_str(), 0, 0, 1024, 682); 

            // Dialog box
            setfillstyle(SOLID_FILL, BLACK);
            bar(10, 472, 1014, 672);
            setlinestyle(0, 1, 6);
            setcolor(WHITE);
            rectangle(10, 472, 1014, 672);
    }
    int choicebox(Scene &scene){
        cleardevice();

        backprep(scene);

        // Display question
        settextstyle(1, HORIZ_DIR, 2);
        typeText(20, 482, scene.choicequestion.c_str(),10);

        for(size_t i = 0; i < scene.choices.size(); ++i){
            typeText(100 * (i + 1), 570, scene.choices[i].c_str(), 10);
        }

        char x;
        int choice = 1;
        int prevchoice;
        while(x != '\r'){
            prevchoice = choice;
            setfillstyle(SOLID_FILL, WHITE);
            bar((choice * 100) - 15, 574,(choice * 100) - 5, 584);
            x = getch();
            if (x == 75) choice--; //Left
            if (x == 77) choice++; //Right
            if (choice < 1) choice = scene.choices.size();
            if (choice > scene.choices.size()) choice = 1;
            setfillstyle(SOLID_FILL, BLACK);
            bar((prevchoice * 100) - 15, 574,(prevchoice * 100) - 5, 584);

        }
        return choice;
    }
};

int main(){
    initwindow(1024,684,"Story");
    StoryGraph game;
    game.addScene("intro", "Ronan and his friends Camila and Cannon go hiking. A light flashes...","Press Enter",{"Enter"},{"summoned"});
    game.play("intro");

    getch();
    closegraph();
}