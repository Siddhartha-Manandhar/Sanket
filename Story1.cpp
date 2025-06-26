#include<iostream>
#include<string>
#include<unordered_map>
#include<vector>
#include<graphics.h>
#include<mmsystem.h>
#include<windows.h>
#include<fstream>
#pragma comment(lib,"winmm.lib")
#include<sstream>
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
    int leftMargin = 10;
    int rightMargin = 1005;
    int maxWidth = rightMargin - leftMargin;
    int lineWidth = 0;
    int lineHeight = 20;

    char temp[2] = "";  // to hold single character as a string
    bool skip = false;
    int startX = x;

    stringstream ss(text);
    string word;

    while (ss >> word) {
        string displayWord = (lineWidth > 0) ? " " + word : word;
        int wordWidth = textwidth((char*)displayWord.c_str());

        if (lineWidth + wordWidth > maxWidth) {
            x = startX;
            y += lineHeight;
            lineWidth = 0;
        }

        for (int i = 0; i < displayWord.length(); i++) {
            if (!skip && kbhit()) {
                char key = getch();
                if (key == ' ') {
                    skip = true;
                }
            }

            temp[0] = displayWord[i];
            outtextxy(x, y, temp);
            int charWidth = textwidth(temp);
            x += charWidth;
            lineWidth += charWidth;

            if (!skip) {
                delay(charDelay);
            }
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

        int spacing = 40;
        vector<int> widths(scene.choices.size());
        int totalWidth = 0;

        backprep(scene);

        // Display question
        settextstyle(1, HORIZ_DIR, 2);

        typeText(20, 482, scene.choicequestion.c_str(),10);

        for(size_t i = 0; i < scene.choices.size(); ++i){
            widths[i] = textwidth((char*)scene.choices[i].c_str());
            totalWidth += widths[i];
            if(i < scene.choices.size() - 1) totalWidth += spacing;
        }
        
        int x = (getmaxx() - totalWidth) / 2;
        vector<int> xpositions(scene.choices.size());
        for(size_t i = 0; i < scene.choices.size(); ++i){
            xpositions[i] = x;
            typeText(x, 570, scene.choices[i].c_str());
            x += widths[i] + spacing;
        }

        char input;
        int choice = 0;
        int prevchoice = -1;
        while(input != '\r'){

            if(prevchoice != choice){
                if(prevchoice >= 0){
                    setfillstyle(SOLID_FILL, BLACK);
                    bar(xpositions[prevchoice] - 15, 574 ,xpositions[prevchoice] - 5, 584);
                }
                setfillstyle(SOLID_FILL, WHITE);
                bar(xpositions[choice] - 15, 574 ,xpositions[choice] - 5, 584);
            }

            input = getch();
            if (input == 75) choice--; //Left
            if (input == 77) choice++; //Right

            if (choice < 0) choice = scene.choices.size() - 1;
            if (choice > scene.choices.size()) choice = 0;


        }
        return choice + 1;
    }
};

int main(){
    initwindow(1024,684,"Story");
    StoryGraph game;
    game.addScene("intro","Press Enter",{"Enter"},{"summoned"});
    game.addScene("summoned","Will you save our world?",{"Yes","No"},{"discovery", "bad_end"});
    game.addScene("discovery","Continue",{"Enter"},{"affinity_check"});
    game.addScene("bad_end","They lived as nobles until Kizaru destroyed the kingdom. The End",{"Game Over"},{"Game Over"});
    game.addScene("affinity_check","What path will you choose?",{"Sword","Wand"},{"sword_path", "magic_path"});
    game.addScene("sword_path","Choose your weapon",{"Katana", "Longsword", "Khukuri"},{"katana_path", "longsword_path", "khukuri_path"});
    game.addScene("magic_path","Choose your element",{"Water", "Fire", "Lightning"},{"water_path", "fire_path", "lightning_path"});

    game.addScene("katana_path","The academy is under attack! What will you do?",{"Fight", "Flight"},{"katana_fight", "katana_flight"});
    game.addScene("longsword_path","The academy is under attack! What will you do??",{"Fight", "Flight"},{"longsword_fight", "longsword_flight"});
    game.addScene("khukuri_path","The academy is under attack! What will you do?",{"Fight", "Flight"},{"khukuri_fight", "khukuri_flight"});
    game.addScene("water_path","The academy is under attack! What will you do?",{"Fight", "Flight"},{"water_fight", "water_flight"});
    game.addScene("fire_path","The academy is under attack! What will you do?",{"Fight", "Flight"},{"fire_fight", "fire_flight"});
    game.addScene("lightning_path","The academy is under attack! What will you do?",{"Fight", "Flight"},{"lightning_fight", "lightning_flight"});
    
    game.addScene("khukuri_fight","The Hand appears! Fight or run?",{"battle","Run"},{"khukuri_battle","khukuri_run"});
//     game.addScene("water_fight","Camila fell. What now?",{"battle","Run"},{"katana_battle","katana_run"});
//     game.addScene("fire_fight","Camila's fire saved many!",{"battle","Run"},{"katana_battle","katana_run"});
//     game.addScene("lightning_fight","You struck with lightning speed!",{"battle","Run"},{"katana_battle","katana_run"});
//     game.addScene("katana_fight","You barely made it!",{"battle","Run"},{"katana_battle","katana_run"});
//     game.addScene("longsword_fight","You led to victory!",{"battle","Run"},{"katana_battle","katana_run"});

//     game.addScene("khukuri_battle", "The Hand attacks! Strike or defend?", {"Strike", "Defend"}, {"khukuri_strike", "khukuri_defend"});
//     game.addScene("khukuri_strike", "Hit landed! It counters!", {"Endure", "Escape"}, {"final_battle", "game_over"});
//     game.addScene("khukuri_defend", "You blocked it! Act now!", {"Counter", "Retreat"}, {"final_battle", "game_over"});

//     game.addScene("game_over", "You fell to the Hand. The End.", {"Game Over"}, {"Game Over"});
//     game.addScene("khukuri_run", "You escaped, but the world suffers.", {"Continue"}, {"resolve"});

//     game.addScene("katana_run", "You escaped with scars.", {"Continue"}, {"resolve"});
//     game.addScene("longsword_flight", "You retreated. Many lost.", {"Continue"}, {"resolve"});
//     game.addScene("khukuri_flight", "You and Cannon fled, guilt remains.", {"Continue"}, {"resolve"});
//     game.addScene("water_flight", "You fled, but lost Camila.", {"Continue"}, {"resolve"});
//     game.addScene("fire_flight", "Fire saved some. Narrow escape.", {"Continue"}, {"resolve"});
//     game.addScene("lightning_flight", "You escaped fast. Regret lingers.", {"Continue"}, {"resolve"});

//     game.addScene("resolve","Ronan vows to grow stronger.", {"Train in jungle"}, {"training"});
//     game.addScene("training", "Ronan aims for the castle.", {"Enter portal"}, {"final_battle"});

//     // Final Battle
//     game.addScene("final_battle", 
//         "3-day war. You face Kizaru. World's fate at stake.", 
//         {"Fight Kizaru"}, 
//         {"final_choice"});

//     game.addScene("final_choice", 
//         "Kizaru is down. A portal opens behind you. Stay?", 
//         {"Yes", "No"}, 
//         {"stay_ending", "return_ending"});

//     game.addScene("stay_ending", 
//         "You stayed and rebuilt the world. Hero forever.", 
//         {"The End"}, 
//         {"Game Over"});

//     game.addScene("return_ending", 
//         "You returned changed, wiser. The journey shaped you.", 
//         {"The End"}, 
//         {"Game Over"});

//     game.play("intro");

//     getch();
//     closegraph();
// }
