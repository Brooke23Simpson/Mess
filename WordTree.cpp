#include "WordTree.hpp"
#include <algorithm>
#include <cctype>
#include <fstream>
#include <iostream>
#include <memory>
#include <queue>
#include <string>
#include <vector>

// Definition of readDictionary function
std::shared_ptr<WordTree> readDictionary(const std::string& filename)
{
    auto wordTree = std::make_shared<WordTree>();
    std::ifstream file(filename);
    std::string word;

    if (!file.is_open())
    {
        std::cerr << "Failed to open dictionary file: " << filename
                  << std::endl;
        return nullptr; // Return nullptr or handle the error as needed
    }

    while (file >> word)
    {
        // Optionally convert to lowercase before adding
        std::transform(word.begin(), word.end(), word.begin(), ::tolower);
        wordTree->add(word);
    }

    return wordTree;
}

WordTree::WordTree() : root(std::make_shared<TreeNode>()) {}

void WordTree::add(const std::string& word)
{
    if (word.empty() || !std::all_of(word.begin(), word.end(), ::isalpha))
        return;

    auto node = root;
    for (char c : word)
    {
        int index = std::tolower(c) - 'a';
        if (!node->children[index])
        {
            node->children[index] = std::make_shared<TreeNode>();
        }
        node = node->children[index];
    }
    node->endOfWord = true;
}

bool WordTree::find(const std::string& word) const
{
    if (word.empty() || !std::all_of(word.begin(), word.end(), ::isalpha))
        return false;

    auto node = root;
    for (char c : word)
    {
        int index = std::tolower(c) - 'a';
        if (!node->children[index])
            return false;
        node = node->children[index];
    }
    return node->endOfWord;
}

std::vector<std::string> WordTree::predict(const std::string& partial,
                                           std::uint8_t howMany) const
{
    std::vector<std::string> predictions;
    if (partial.empty() ||
        !std::all_of(partial.begin(), partial.end(), ::isalpha))
        return predictions;

    auto node = root;
    for (char c : partial)
    {
        int index = std::tolower(c) - 'a';
        if (!node->children[index])
            return predictions;
        node = node->children[index];
    }

    std::queue<std::pair<std::shared_ptr<TreeNode>, std::string>> q;
    q.push({node, partial});

    while (!q.empty() && predictions.size() < howMany)
    {
        auto [currentNode, currentWord] = q.front();
        q.pop();

        if (currentNode->endOfWord)
        {
            predictions.push_back(currentWord);
        }

        for (int i = 0; i < 26; ++i)
        {
            if (currentNode->children[i])
            {
                char nextChar = 'a' + i;
                q.push({currentNode->children[i], currentWord + nextChar});
            }
        }
    }
    return predictions;
}

std::size_t WordTree::size() const { return countWords(root); }

std::size_t WordTree::countWords(const std::shared_ptr<TreeNode>& node) const
{
    if (!node)
        return 0;

    std::size_t count = node->endOfWord ? 1 : 0;
    for (const auto& child : node->children)
    {
        count += countWords(child);
    }
    return count;
}
