// Header files for standard input/output, strings, data structures, graphics and sound
#include<iostream>
#include<string>
#include<unordered_map>
#include<vector>
#include<graphics.h>
#include<mmsystem.h>
#include<windows.h>
#include<fstream>
#pragma comment(lib,"winmm.lib") // Link Multimedia sound library
#include<sstream>
using namespace std;

// Scene structure to hold all data relevant to a scene in the story
struct Scene{
    string description;             // Text description (filename) for the scene
    string choicequestion;         // Question to present to player
    vector<string> choices;        // List of available choices
    vector<string> nextScenes;     // Corresponding scene IDs for each choice
    string img;                    // Background image (filename)
    string audio;                  // Audio file (filename)
};

// Function to display text character-by-character with a typewriter effect
void typeText(int x, int y, const char* text, int charDelay = 5) {
    int leftMargin = 10;
    int rightMargin = 1005;
    int maxWidth = rightMargin - leftMargin;
    int lineWidth = 0;
    int lineHeight = 20;

    char temp[2] = "";  // buffer to print a single character
    bool skip = false;  // if true, skip typewriter effect
    int startX = x;

    stringstream ss(text); // Split text into words
    string word;

    while (ss >> word) {
        string displayWord = (lineWidth > 0) ? " " + word : word;
        int wordWidth = textwidth((char*)displayWord.c_str());

        // Wrap line if it exceeds max width
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
            outtextxy(x, y, temp);  // Print character
            int charWidth = textwidth(temp);
            x += charWidth;
            lineWidth += charWidth;

            if (!skip) {
                delay(charDelay);  // Delay per character
            }
        }
    }
}

// StoryGraph manages scenes and transitions
class StoryGraph {
    unordered_map<string, Scene> scenes; // Map of scene ID to scene data

public:
    // Adds a scene to the story graph
    void addScene(string id, string choicequestion, vector<string> choices, vector<string> nextScenes){
        scenes[id] = {"", choicequestion, choices, nextScenes, "", ""};
    }

    // Main game loop to play through the story from a given start ID
    void play(string startID){
        string current = startID;
        while(true){
            cleardevice();
            Scene &scene = scenes[current];

            // Load asset filenames
            scene.img = current + ".jpeg";
            scene.audio = current + ".wav";
            scene.description = current + ".txt";

            backprep(scene); // Draw background and dialog box

            // Read scene description from file
            ifstream file(scene.description);
            string line;
            if(file.is_open()){
                getline(file, line);
            }
            file.close();

            settextstyle(1, HORIZ_DIR, 2);
            PlaySoundA(scene.audio.c_str(), NULL, SND_FILENAME | SND_ASYNC);
            typeText(20, 482, line.c_str(), 50); // Display description text

            // Triangle to indicate "press enter"
            setfillstyle(SOLID_FILL, RED);
            int triangle[] = {960, 640, 970, 660, 980, 640, 960, 640};
            fillpoly(3, triangle);

            // Wait for enter key
            char x = 'A';
            while(x != '\r'){
                x = getch();
            }

            // Display and process choices
            int choice = choicebox(scene);
            current = scene.nextScenes[choice - 1]; // Move to next scene
        }
    }

    // Prepares the scene visuals (image and dialog box)
    void backprep(Scene &scene){
        setbkcolor(BLACK);
        cleardevice();

        // Load background image
        readimagefile(scene.img.c_str(), 0, 0, 1024, 682); 

        // Dialog box area
        setfillstyle(SOLID_FILL, BLACK);
        bar(10, 472, 1014, 672);
        setlinestyle(0, 1, 6);
        setcolor(WHITE);
        rectangle(10, 472, 1014, 672);
    }

    // Displays the choices and allows keyboard navigation
    int choicebox(Scene &scene){
        cleardevice();

        int spacing = 40;
        vector<int> widths(scene.choices.size());
        int totalWidth = 0;

        backprep(scene);

        settextstyle(1, HORIZ_DIR, 2);
        typeText(20, 482, scene.choicequestion.c_str(), 10); // Show question

        // Calculate choice button positions
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

        // Input loop to handle left/right navigation
        char input = 0;
        int choice = 0;
        int prevchoice = -1;
        while(input != '\r'){ // Enter key to confirm
            if(prevchoice != choice){
                // Erase previous selector
                if(prevchoice >= 0){
                    setfillstyle(SOLID_FILL, BLACK);
                    bar(xpositions[prevchoice] - 15, 574 ,xpositions[prevchoice] - 5, 584);
                }
                // Draw new selector
                setfillstyle(SOLID_FILL, WHITE);
                bar(xpositions[choice] - 15, 574 ,xpositions[choice] - 5, 584);
                prevchoice = choice;
            }

            input = getch();
            if (input == 75) choice--; // Left arrow
            if (input == 77) choice++; // Right arrow

            if (choice < 0) choice = scene.choices.size() - 1;
            if (choice >= scene.choices.size()) choice = 0;
        }

        return choice + 1; // Return 1-based index
    }
};

int main(){
    initwindow(1024, 684, "Story"); // Initialize graphics window
    StoryGraph game;

    // Add all scenes to the story
    game.addScene("intro", "Press Enter", {"Enter"}, {"summoned"});
    game.addScene("summoned", "Will you save our world?", {"Yes", "No"}, {"discovery", "bad_end"});
    game.addScene("discovery", "Continue", {"Enter"}, {"affinity_check"});
    game.addScene("bad_end", "They lived as nobles until Kizaru destroyed the kingdom. The End", {"Game Over"}, {"Game_Over"});
    game.addScene("affinity_check", "What path will you choose?", {"Sword", "Wand"}, {"sword_path", "magic_path"});
    game.addScene("sword_path", "Choose your weapon", {"Katana", "Longsword", "Khukuri"}, {"katana_path", "longsword_path", "khukuri_path"});
    game.addScene("magic_path", "Choose your element", {"Water", "Fire", "Lightning"}, {"water_path", "fire_path", "lightning_path"});

    // Academy Attack Paths
    game.addScene("katana_path", "The academy is under attack! What will you do?", {"Fight", "Flight"}, {"katana_fight", "katana_flight"});
    game.addScene("longsword_path", "The academy is under attack! What will you do??", {"Fight", "Flight"}, {"longsword_fight", "longsword_flight"});
    game.addScene("khukuri_path", "The academy is under attack! What will you do?", {"Fight", "Flight"}, {"khukuri_fight", "khukuri_flight"});
    game.addScene("water_path", "The academy is under attack! What will you do?", {"Fight", "Flight"}, {"water_fight", "water_flight"});
    game.addScene("fire_path", "The academy is under attack! What will you do?", {"Fight", "Flight"}, {"fire_fight", "fire_flight"});
    game.addScene("lightning_path", "The academy is under attack! What will you do?", {"Fight", "Flight"}, {"lightning_fight", "lightning_flight"});

    // Fight/Flight Outcomes
    game.addScene("khukuri_flight", "You and Cannon escaped, but guilt haunts you as others fell.", {"Continue"}, {"resolve"});
    game.addScene("katana_flight", "You managed to escape with injuries. You saw many fall.", {"Continue"}, {"resolve"});
    game.addScene("longsword_flight", "You retreated, but the academy suffered massive losses.", {"Continue"}, {"resolve"});
    game.addScene("water_flight", "You fled using water currents, but Camila got separated.", {"Continue"}, {"resolve"});
    game.addScene("fire_flight", "Your fiery distraction saved some, but not all. A narrow escape.", {"Continue"}, {"resolve"});
    game.addScene("lightning_flight", "You bolted away with lightning speed. The guilt lingers.", {"Continue"}, {"resolve"});

    // Battles for each path
    game.addScene("khukuri_battle", "The Hand attacks! Strike or defend?", {"Strike", "Defend"}, {"Strike_Won", "game_over"});
    game.addScene("water_battle", "The Hand attacks! Strike or defend?", {"Strike", "Defend"}, {"Strike_Won", "game_over"});
    game.addScene("fire_battle", "The Hand attacks! Strike or defend?", {"Strike", "Defend"}, {"Strike_Won", "game_over"});
    game.addScene("lightning_battle", "The Hand attacks! Strike or defend?", {"Strike", "Defend"}, {"Strike_Won", "game_over"});
    game.addScene("katana_battle", "The Hand attacks! Strike or defend?", {"Strike", "Defend"}, {"Strike_Won", "game_over"});
    game.addScene("longsword_battle", "The Hand attacks! Strike or defend?", {"Strike", "Defend"}, {"Strike_Won", "game_over"});

    // Run paths
    game.addScene("khukuri_run", "You escaped, but darkness spreads across the land.", {"Continue"}, {"resolve"});
    game.addScene("water_run", "You escaped, but darkness spreads across the land.", {"Continue"}, {"resolve"});
    game.addScene("fire_run", "You escaped, but darkness spreads across the land.", {"Continue"}, {"resolve"});
    game.addScene("lightning_run", "You escaped, but darkness spreads across the land.", {"Continue"}, {"resolve"});
    game.addScene("katana_run", "You escaped, but darkness spreads across the land.", {"Continue"}, {"resolve"});
    game.addScene("longsword_run", "You escaped, but darkness spreads across the land.", {"Continue"}, {"resolve"});

    // After victory or escape
    game.addScene("Strike_Won", "The Hand is defeated.", {"Continue"}, {"continue"});
    game.addScene("continue", "Continue", {"Enter"}, {"resolve"});
    game.addScene("resolve", "Ronan vows to grow stronger.", {"Train in jungle"}, {"training"});
    game.addScene("training", "Ronan aims for the castle.", {"Enter portal"}, {"final_battle_1"});

    // Final Battle
    game.addScene("final_battle_1", "3-day war. You face Kizaru. World's fate at stake.", {"Fight Kizaru"}, {"final_battle_2"});
    game.addScene("final_battle_2", "Continue", {"Continue"}, {"Final_battle_3"});
    game.addScene("final_battle_3", "Continue", {"Continue"}, {"final_choice"});

    // Endings
    game.addScene("final_choice", "Will you Stay in this world?", {"Yes", "No"}, {"stay_ending", "return_ending"});
    game.addScene("stay_ending", "You stayed and rebuilt the world. Hero forever.", {"The End"}, {"Game_Over"});
    game.addScene("return_ending", "You returned changed, wiser. The journey shaped you.", {"The End"}, {"Game_Over"});
    game.addScene("Game_Over", "", {"Exit"}, {});

    // Main Menu
    int choice = 0;
    int choicepos = 0;
    int midX = getmaxx() / 2;

    setbkcolor(RED);
    cleardevice();
    readimagefile("Menu.jpeg", 0, 0, 1024, 682);
    PlaySoundA("Menu.wav", NULL, SND_FILENAME | SND_ASYNC);

    while(choice != 2){
        setfillstyle(SOLID_FILL, RED);

        // Draw buttons
        for(int i = 0; i < 2; i++){
            bar(midX - (200 + 5), 295 + 150 * i, midX + (200 + 5), 405 + 150 * i);
        }

        settextstyle(1, HORIZ_DIR, 2);
        outtextxy(midX - 100, 300 + 30, " Start Game");
        outtextxy(midX - 100, 450 + 30, "      Exit");

        setcolor(WHITE);
        setlinestyle(0, 1, 6);
        rectangle(midX - 200, 300 + 150 * choicepos, midX + 200, 400 + 150 * choicepos);

        int input = getch();
        if(input == 72 && choicepos > 0) choicepos--; // Up arrow
        if(input == 80 && choicepos < 1) choicepos++; // Down arrow
        if(input == 13) choice = choicepos + 1;       // Enter key

        switch(choice){
            case 1: game.play("intro"); break;
            case 2: exit(0); // Exit game
        }
    }

    getch();
    closegraph(); // Close graphics window
}

