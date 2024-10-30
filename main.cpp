#include "WordTree.hpp"
#include "rlutil.h"
#include <iostream>
#include <string>
#include <sstream>

// Assuming readDictionary function is provided to load words
extern std::shared_ptr<WordTree> readDictionary(const std::string& filename);

void runPredictiveTextUI(std::shared_ptr<WordTree> wordTree) {
    std::string userInput;
    const std::string PROMPT = "Type a sentence (ESC to exit): ";
    const std::string PREDICTIONS = "--- Predictions ---";
    
    // Clear screen initially
    rlutil::cls();
    
    while (true) {
        // Draw the input prompt and current user input
        rlutil::locate(1, 1);
        std::cout << PROMPT << userInput;
        
        // Clear any remaining characters from previous longer inputs
        std::cout << std::string(10, ' ');
        
        // Draw the predictions header
        rlutil::locate(1, 3);
        std::cout << PREDICTIONS;
        
        // Get the last word being typed
        std::string lastWord;
        std::size_t lastSpace = userInput.find_last_of(" \t\n\r");
        if (lastSpace == std::string::npos) {
            lastWord = userInput;
        } else {
            lastWord = userInput.substr(lastSpace + 1);
        }
        
        // Get predictions for the last word
        // Leave 5 lines for UI elements (prompt, header, etc.)
        auto predictions = wordTree->predict(lastWord, rlutil::trows() - 5);
        
        // Clear previous predictions
        for (int i = 0; i < rlutil::trows() - 5; ++i) {
            rlutil::locate(1, 5 + i);
            std::cout << std::string(50, ' ');
        }
        
        // Display new predictions
        for (int i = 0; i < predictions.size(); ++i) {
            rlutil::locate(1, 5 + i);
            std::cout << predictions[i];
        }
        
        // Handle user input
        int key = rlutil::getkey();
        
        if (key == rlutil::KEY_ESCAPE) {
            break;
        }
        else if (key == rlutil::KEY_BACKSPACE && !userInput.empty()) {
            userInput.pop_back();
        }
        else if (key == rlutil::KEY_SPACE) {
            userInput += ' ';
        }
        else if (std::isprint(key)) {  // Only add printable characters
            userInput += static_cast<char>(key);
        }
    }
    
    // Clear screen when exiting
    rlutil::cls();
}

int main() {
    auto wordTree = readDictionary("dictionary.txt");
    runPredictiveTextUI(wordTree);
    return 0;
}
