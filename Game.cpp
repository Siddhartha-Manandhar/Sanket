#include <iostream>
#include <string>
#include <limits>
using namespace std;

// -------- Binary Tree Node --------
struct Node {
    string text;
    string choice1Text;
    string choice2Text;
    Node* left;  // Choice 1
    Node* right; // Choice 2

    Node(string t, string c1 = "", string c2 = "") {
        text = t;
        choice1Text = c1;
        choice2Text = c2;
        left = nullptr;
        right = nullptr;
    }
};

// -------- Get Valid Input --------
int getChoice(int maxChoice) {
    int choice;
    while (!(cin >> choice) || choice < 1 || choice > maxChoice) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Invalid input. Enter 1 or " << maxChoice << ": ";
    }
    return choice;
}

// -------- Game Engine --------
void play(Node* current) {
    while (current != nullptr) {
        cout << "\n" << current->text << "\n";

        // No more choices: leaf node
        if (current->left == nullptr && current->right == nullptr) {
            cout << "\n=== The End ===\n";
            break;
        }

        // Display choices
        if (!current->choice1Text.empty()) cout << "1. " << current->choice1Text << "\n";
        if (!current->choice2Text.empty()) cout << "2. " << current->choice2Text << "\n";

        int input = getChoice(current->choice2Text.empty() ? 1 : 2);
        current = (input == 1) ? current->left : current->right;
    }
}

// -------- Build Story Tree --------
Node* buildStory() {
    // === Endings ===
    Node* end_destroyed = new Node("You lived as nobles, but the world was destroyed by Kizaru.");
    Node* end_escape = new Node("You return to Earth with fading memories of this world.");
    Node* end_rebuild = new Node("You stay in the world and help rebuild it with your surviving friend.");

    // === Final Boss Path ===
    Node* finalChoice = new Node("You defeat Kizaru and hold the Essence of Darkness. Return or stay?", "Return", "Stay");
    finalChoice->left = end_escape;
    finalChoice->right = end_rebuild;

    // === Ronan's Journey ===
    Node* journey_start = new Node("Ronan, a 17-year-old with a strong sense of justice, is on a hiking trip with his best friends, Camila and Cannon. They suddenly find themselves in a bricked room filled with robed mages and knights. The king asks them to save the world from Kizaru, the Lord of Darkness. Will they accept the quest?", "No", "Yes");
    journey_start->left = end_destroyed; // If they say No
    journey_start->right = new Node("The king is pleased and takes them to an alchemist lab to check their affinities. Camila has high mana, Cannon has powerful aura. Ronan has both but must choose one. What will he choose?", "Sword", "Wand");

    // === Ronan's Choice ===
    Node* choose_sword = new Node("Ronan chooses the Sword. He and Cannon attend the Sword Academy SeaBird to master their techniques. They choose their weapons. Ronan can choose between Katana, Longsword, or Khukuri.", "Katana", "Khukuri");
    Node* choose_wand = new Node("Ronan chooses the Wand. He and Camila attend the Magic Academy Khwopa to master their magic. Ronan can choose between Water, Fire, or Lightning.", "Water", "Fire");

    // === Weapon Choices ===
    Node* sword_katana = new Node("Ronan chose the Katana. He trains hard and learns powerful techniques.", "Continue");
    Node* sword_khukuri = new Node("Ronan chose the Khukuri. He trains hard and learns powerful techniques.", "Continue");
    Node* wand_water = new Node("Ronan chose Water. He trains hard and learns powerful techniques.", "Continue");
    Node* wand_fire = new Node("Ronan chose Fire. He trains hard and learns powerful techniques.", "Continue");

    // Connecting choices
    choose_sword->left = sword_katana;
    choose_sword->right = sword_khukuri;
    choose_wand->left = wand_water;
    choose_wand->right = wand_fire;

    // === Attack on Academy ===
    Node* attack_academy = new Node("The academy is attacked by the Hand of Darkness. Students are in danger! Will Ronan fight the minions or flee to help students?", "Fight", "Flee");
    Node* fight_result = new Node("Ronan fights bravely and defeats the minions, earning a Mana/Aura Amplification Potion. He escapes with the survivors.", "Continue");
    Node* flee_result = new Node("Ronan flees and helps many students escape the academy.", "Continue");

    attack_academy->left = fight_result;
    attack_academy->right = flee_result;

    // === Aftermath ===
    Node* aftermath = new Node("The academy lies in ruins. Ronan reunites with either Camila or Cannon, who survived the attack. The Grandmaster gives them a final piece of information about the Sanctuary of Eternia, but warns them of the dangers ahead.", "Continue");

    // === New Journey Arc after Aftermath ===
    Node* journey_arc_intro = new Node(
        "Their journey begins. They gather allies — rebels, exiled royals, and a former monster tamer. Along the way, they face ambushes and moral dilemmas.\n"
        "In one village they liberate, an old seer warns Ronan: \"The path to saving the world comes at a price — choose between your friends, or lose your soul.\"",
        "Accept the burden", "Reject the burden"
    );

    Node* accept_burden = new Node("Ronan accepts the burden, steels his resolve, and the party grows stronger with new allies.", "Continue");
    Node* reject_burden = new Node("Ronan doubts his path, the party faces tougher challenges ahead, but determination keeps them moving.", "Continue");

    journey_arc_intro->left = accept_burden;
    journey_arc_intro->right = reject_burden;

    // Connect aftermath to journey arc
    aftermath->left = journey_arc_intro;

    // === Final battle connected to journey outcomes
    Node* final_battle = new Node("Ronan confronts Kizaru in a cataclysmic duel. After an intense battle, he strikes the final blow. Kizaru's essence collapses into a dark crystal. Will Ronan return home or stay to rebuild?", "Return Home", "Stay Behind");
    final_battle->left = end_escape; // If he returns home
    final_battle->right = end_rebuild; // If he stays behind

    // Connect journey arcs to final battle
    accept_burden->left = final_battle;
    reject_burden->left = final_battle;

    // Connect attack outcomes to aftermath
    fight_result->left = aftermath;
    flee_result->left = aftermath;

    // Connect initial choices
    journey_start->right->left = choose_sword; // Sword path
    journey_start->right->right = choose_wand; // Wand path
    sword_katana->left = attack_academy;
    sword_khukuri->left = attack_academy;
    wand_water->left = attack_academy;
    wand_fire->left = attack_academy;

    return journey_start; // Starting point of the story
}

// -------- Main --------
int main() {
    Node* storyRoot = buildStory();
    play(storyRoot);
    return 0;
}