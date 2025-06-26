#include<iostream>
#include<string>
#include<unordered_map>
#include<vector>
#include<graphics.h>
using namespace std;

struct Scene{
    string description;
    vector<string> choices;
    vector<string> nextScenes;
    void screen(string id);
};


class StoryGraph{
    unordered_map<string, Scene> scenes;

    public:
    void addScene(string id, string description, vector<string> choices, vector<string> nextScenes){
        scenes[id] = {description, choices, nextScenes};
    }

    void play(string startID){
        string current = startID;
        while(true){
            Scene &scene = scenes[current];
            cout << "\n--- " << current << " ---\n";
            cout << scene.description << "\n";

            if (scene.choices.empty()) break;

            for (size_t i = 0; i < scene.choices.size(); ++i){
                cout << i + 1 << ". " << scene.choices[i] << "\n"; 
            }

            int choice;
            cout << "Choose: ";
            cin >> choice;

            if(choice < 1 || choice > scene.nextScenes.size()){
                cout << "Inavlid choice.\n";
                break;
            }

            current = scene.nextScenes[choice - 1];
        }
        cout << "\n=== The END ===\n";
    }
};

int main(){
    StoryGraph game;

    game.addScene("intro", "Ronan and his friends Camila and Cannon go hiking. A light flashes...",{},{});

    game.addScene("summoned", "They wake in a throne room. The king asks: Will you save our world?",{"Yes", "No"}, {"affinity_check", "bad_end"});
    game.play("summoned");
    getch();
}