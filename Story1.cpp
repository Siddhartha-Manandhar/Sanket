#include<iostream>
#include<string>
#include<unordered_map>
#include<vector>
#include<graphics.h>
#include<mmsystem.h>
#include<windows.h>
#include<fstream>
#pragma comment(lib,"winmm.lib")
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
    void addScene(string id,string choicequestion, vector<string> choices, vector<string> nextScenes){
        scenes[id] = {"", choicequestion, choices, nextScenes, "", ""};
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
            typeText(20, 482, line.c_str(),50);
            setfillstyle(SOLID_FILL, RED);
            int triangle[] = {960, 640, 970, 660, 980, 640, 960, 640};
            fillpoly(3, triangle);

            char x;
            x = 'A';
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
    game.addScene("intro","Press Enter",{"Enter"},{"summoned"});
    game.addScene("summoned","Will you save our world?",{"Yes","No"},{"affinity_check", "bad_end"});
    game.addScene("bad_end","They lived as nobles until Kizaru destroyed the kingdom. The End",{"Game Over"},{"Game Over"});
    game.addScene("affinity_check","What path will you choose?",{"Sword","Wand"},{"sword_path", "magic_path"});
    // game.addScene("sword_path","Choose your weapon",{"Katana", "Longsword", "Khukuri"},{"katana_path", "longsword_path", "khukuri_path"});
    // game.addScene("magic_path","Choose your element",{"Water", "Fire", "Lightning"},{"water_path", "fire_path", "lightning_path"});

    // game.addScene("katana_path","The academy is under attack! What will you do?",{"Fight", "Flight"},{"katana_fight", "katana_flight"});
    // game.addScene("longsword_path","The academy is under attack! What will you do?",{"Fight", "Flight"},{"longsword_fight", "longsword_flight"});
    // game.addScene("khukuri_path","The academy is under attack! What will you do?",{"Fight", "Flight"},{"khukuri_fight", "khukuri_flight"});
    // game.addScene("water_path","The academy is under attack! What will you do?",{"Fight", "Flight"},{"water_fight", "water_flight"});
    // game.addScene("fire_path","The academy is under attack! What will you do?",{"Fight", "Flight"},{"fire_fight", "fire_flight"});
    // game.addScene("lightning_path","The academy is under attack! What will you do?",{"Fight", "Flight"},{"lightning_fight", "lightning_flight"});

    // game.addScene("khukuri_fight","You fought bravely, but Cannon fell in battle.",{"Continue"},{"resolve"});
    // game.addScene("water_fight","Camila was struck down during the battle.",{"Continue"},{"resolve"});
    // game.addScene("fire_fight","You defeated the enemy with Camila's fire magic!",{"Continue"},{"resolve"});
    // game.addScene("lightning_fight","You held your ground with lightning speed!",{"Continue"},{"resolve"});
    // game.addScene("katana_fight","Balanced effort. You barely survived!",{"Continue"},{"resolve"});
    // game.addScene("longsword_fight","Victory with combined sword power!",{"Continue"},{"resolve"});

    // game.addScene("resolve","Ronan trained for months in the jungle and stepped into the demonic portal.",{"Proceed"},{"final_battle"});
    // game.addScene("final_battle","The battle against Kizaru lasted three days. At last, he fell.",{"The End"},{"final_choice"});
    // game.addScene("final_choice","Will you stay in this world?",{"Yes", "No"},{"stay_hero", "return_home"});
    // game.addScene("stay_hero","You remain as a legendary hero in this world.",{"Game Over"},{"Game Over"});
    // game.addScene("return_home","You return to Nepal, forever changed.",{"Game Over"},{"Game Over"});
  

    game.play("intro");

    getch();
    closegraph();
}
