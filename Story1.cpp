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

    game.addScene("katana_path","The academy is under attack by Right hand of Evil Lord! What will you do?",{"Fight", "Flight"},{"katana_fight", "katana_flight"});
    game.addScene("longsword_path","The academy is under attack by Right hand of Evil Lord! What will you do?",{"Fight", "Flight"},{"longsword_fight", "longsword_flight"});
    game.addScene("khukuri_path","The academy is under attack by Right hand of Evil Lord! What will you do?",{"Fight", "Flight"},{"khukuri_fight", "khukuri_flight"});
    game.addScene("water_path","The academy is under attack by Left hand of Evil Lord! What will you do?",{"Fight", "Flight"},{"water_fight", "water_flight"});
    game.addScene("fire_path","The academy is under attack by Left hand of Evil Lord! What will you do?",{"Fight", "Flight"},{"fire_fight", "fire_flight"});
    game.addScene("lightning_path","The academy is under attack by Left hand of Evil Lord! What will you do?",{"Fight", "Flight"},{"lightning_fight", "lightning_flight"});
    
    game.addScene("khukuri_fight","You fought bravely, but Cannon fell in battle.",{"Continue"},{"resolve"});
    game.addScene("water_fight","Camila was struck down during the battle.",{"Continue"},{"resolve"});
    game.addScene("fire_fight","You defeated the enemy with Camila's fire magic!",{"Continue"},{"resolve"});
    game.addScene("lightning_fight","You held your ground with lightning speed!",{"Continue"},{"resolve"});
    game.addScene("katana_fight","Balanced effort. You barely survived!",{"Continue"},{"resolve"});
    game.addScene("longsword_fight","Victory with combined sword power!",{"Continue"},{"resolve"});

    game.addScene("khukuri_flight", "You and Cannon escaped, but guilt haunts you as others fell.", {"Continue"}, {"resolve"});
    game.addScene("katana_flight", "You managed to escape with injuries. You saw many fall.", {"Continue"}, {"resolve"});
    game.addScene("longsword_flight", "You retreated, but the academy suffered massive losses.", {"Continue"}, {"resolve"});
    game.addScene("water_flight", "You fled using water currents, but Camila got separated.", {"Continue"}, {"resolve"});
    game.addScene("fire_flight", "Your fiery distraction saved some, but not all. A narrow escape.", {"Continue"}, {"resolve"});
    game.addScene("lightning_flight", "You bolted away with lightning speed. The guilt lingers.", {"Continue"}, {"resolve"});

    
    game.addScene("resolve","After the battle, Ronan vowed to become stronger.", {"Train in the jungle"}, {"training"});
    game.addScene("training", "Ronan wants to enter the castle!", {"Enter the portal"}, {"final_battle"});

// Chapter 7: Final Battle
// game.addScene("final_battle", 
// "A brutal three-day war begins. You and your remaining allies face Kizaru himself in his throne chamber. The fate of both worlds hangs in the balance.", 
// {"Fight Kizaru"}, 
// {"final_choice"});

// Chapter 8: Final Choice
// game.addScene("final_choice", 
// "With Kizaru defeated, peace returns. But a glowing portal back to Earth opens behind you.\n\nWill you stay in this world?", 
// {"Yes", "No"}, 
// {"stay_ending", "return_ending"});

// game.addScene("stay_ending", 
// "You choose to stay. Over the years, legends spread of a great hero who saved the world and rebuilt it from ashes. You become a guiding light for generations.", 
// {"The End"}, 
// {"Game Over"});

// game.addScene("return_ending", 
// "You return home—changed, wiser, and forever marked by the battles you fought. The real world feels different, and so do you. But you carry the strength within.", 
// {"The End"}, 
// {"Game Over"});
  

    game.play("intro");

    getch();
    closegraph();
}
