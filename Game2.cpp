#include <iostream>
#include <unordered_map>
#include <vector>
#include <string>
using namespace std;

struct Scene {
    string description;
    vector<string> choices;
    vector<string> nextScenes;
};

class StoryGraph {
    unordered_map<string, Scene> scenes;

public:
    void addScene(string id, string description, vector<string> choices, vector<string> nextScenes) {
        scenes[id] = {description, choices, nextScenes};
    }

    void play(string startID) {
        string current = startID;
        while (true) {
            Scene &scene = scenes[current];
            cout << "\n--- " << current << " ---\n";
            cout << scene.description << "\n";

            if (scene.choices.empty()) break;

            for (size_t i = 0; i < scene.choices.size(); ++i) {
                cout << i + 1 << ". " << scene.choices[i] << "\n";
            }

            int choice;
            cout << "Choose: ";
            cin >> choice;

            if (choice < 1 || choice > scene.nextScenes.size()) {
                cout << "Invalid choice.\n";
                break;
            }

            current = scene.nextScenes[choice - 1];
        }
        cout << "\n=== THE END ===\n";
    }
};

int main() {
    StoryGraph game;

    game.addScene("intro",
        "Ronan and his friends Camila and Cannon go hiking. A light flashes...",
        {}, {});

    game.addScene("summoned",
        "They wake in a throne room. The king asks: Will you save our world?",
        {"Yes", "No"}, {"affinity_check", "bad_end"});

    game.addScene("bad_end",
        "They lived as nobles until Kizaru destroyed the kingdom.",
        {}, {});

    game.addScene("affinity_check",
        "Camila has powerful mana. Cannon has immense aura. Ronan can use both. Choose your path.",
        {"Sword", "Wand"}, {"sword_path", "wand_path"});

    game.addScene("sword_path",
        "You train at SeaBird Academy. Choose your weapon.",
        {"Katana", "Longsword", "Khukuri"}, {"katana", "longsword", "khukuri"});

    game.addScene("wand_path",
        "You train at Khwopa Academy. Choose your element.",
        {"Water", "Fire", "Lightning"}, {"water", "fire", "lightning"});

    game.addScene("katana", "You chose Katana. The academy is attacked! Fight or run?", {"Fight", "Run"}, {"katana_fight", "evacuation"});
    game.addScene("longsword", "You chose Longsword. The academy is attacked! Fight or run?", {"Fight", "Run"}, {"longsword_fight", "evacuation"});
    game.addScene("khukuri", "You chose Khukuri. The academy is attacked! Fight or run?", {"Fight", "Run"}, {"khukuri_fight", "evacuation"});

    game.addScene("water", "You chose Water Magic. The academy is attacked! Fight or run?", {"Fight", "Run"}, {"water_fight", "evacuation"});
    game.addScene("fire", "You chose Fire Magic. The academy is attacked! Fight or run?", {"Fight", "Run"}, {"fire_fight", "evacuation"});
    game.addScene("lightning", "You chose Lightning Magic. The academy is attacked! Fight or run?", {"Fight", "Run"}, {"lightning_fight", "evacuation"});

    game.addScene("evacuation", "You help students evacuate. The battle rages on...", {}, {});

    game.addScene("katana_fight", "You fight bravely and defeat minions. You find a legendary aura potion.", {}, {});
    game.addScene("longsword_fight", "You fight bravely and defeat minions. You find a legendary aura potion.", {}, {});
    game.addScene("khukuri_fight", "You fight bravely but Cannon dies during battle.", {}, {});

    game.addScene("water_fight", "You fight bravely but Camila dies during battle.", {}, {});
    game.addScene("fire_fight", "You and Camila defeat the enemy's hand together.", {}, {});
    game.addScene("lightning_fight", "You and Camila barely win the fight.", {}, {});

    game.addScene("training", "Ronan trains alone in the jungle to defeat Kizaru.", {}, {});
    game.addScene("final_battle", "You enter the Demonic Castle. The final battle begins.", {}, {});

    game.addScene("end_choice", "The king offers you to stay. Will you?", {"Yes", "No"}, {"stay", "return_home"});
    game.addScene("stay", "You stayed and became a hero of the new world.", {}, {});
    game.addScene("return_home", "You returned to Earth, forever changed.", {}, {});

    // Start the game
    game.play("summoned");

    return 0;
}
