#include <iostream>
#include <string>

using namespace std;

// Tree Node Structure
struct TreeNode {
    string question;
    TreeNode* left;
    TreeNode* right;

    TreeNode(string q) : question(q), left(nullptr), right(nullptr) {}
};

// Function to play the game recursively
void playGame(TreeNode* node) {
    if (node->left == nullptr && node->right == nullptr) {
        cout << node->question << endl; // Ending statement
        return;
    }

    cout << node->question << " (left/right): ";
    string choice;
    cin >> choice;

    if (choice == "left" && node->left != nullptr) {
        playGame(node->left);
    } else if (choice == "right" && node->right != nullptr) {
        playGame(node->right);
    } else {
        cout << "Invalid choice! Try again." << endl;
        playGame(node);
    }
}

// Main function
int main() {
    // Creating the game decision tree
    TreeNode* gameTree = new TreeNode("You are in a dark forest. Do you go left or right?");
    
    gameTree->left = new TreeNode("You found a river. Do you swim across or walk along the river?");
    gameTree->right = new TreeNode("You found a cave. Do you enter or turn back?");
    
    gameTree->left->left = new TreeNode("You swam but got tired. You lost!");
    gameTree->left->right = new TreeNode("You walked along and found a village. You win!");
    
    gameTree->right->left = new TreeNode("Inside, you found treasure. You win!");
    gameTree->right->right = new TreeNode("You got lost in the forest. You lost!");

    // Start the game
    playGame(gameTree);

    // Free allocated memory
    delete gameTree->left->left;
    delete gameTree->left->right;
    delete gameTree->right->left;
    delete gameTree->right->right;
    delete gameTree->left;
    delete gameTree->right;
    delete gameTree;

    return 0;
}