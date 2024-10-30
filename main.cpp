#include "WordTree.hpp"
#include "rlutil.h"
#include <iostream>
#include <memory>
#include <string>
#include <vector>

extern std::shared_ptr<WordTree> readDictionary(const std::string& filename);

void runPredictiveTextUI(std::shared_ptr<WordTree> wordTree)
{
    std::string userInput;
    rlutil::cls();

    while (true)
    {
        rlutil::locate(1, 1);
        std::cout << "Type a sentence (ESC to exit): " << userInput
                  << std::string(30, ' '); // Clear line

        rlutil::locate(1, 3);
        std::cout << "--- Predictions ---";

        // Extract the last word
        std::string lastWord =
            userInput.substr(userInput.find_last_of(" \t\n\r") + 1);
        auto predictions = wordTree->predict(lastWord, rlutil::trows() - 5);

        for (size_t i = 0; i < predictions.size(); ++i)
        {
            rlutil::locate(1, 5 + i);
            std::cout << predictions[i] << std::string(20, ' '); // Clear line
        }

        int key = rlutil::getkey();
        if (key == rlutil::KEY_BACKSPACE && !userInput.empty())
        {
            userInput.pop_back();
        }
        else if (key == 27)
        { // ESC to exit
            break;
        }
        else if (key >= ' ' && key <= '~')
        { // Printable ASCII range
            userInput.push_back(static_cast<char>(key));
        }
    }
}

int main()
{
    auto wordTree = readDictionary("dictionary.txt");
    if (!wordTree)
    {
        std::cerr << "Failed to initialize word tree." << std::endl;
        return 1; // Return error code
    }
    runPredictiveTextUI(wordTree);
    return 0;
}
