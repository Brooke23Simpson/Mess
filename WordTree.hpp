#ifndef WORDTREE_HPP
#define WORDTREE_HPP

#include <array>
#include <memory>
#include <string>
#include <vector>

class WordTree {
public:
    // Constructor
    WordTree();

    // Adds a new word to the tree if valid (no duplicates, only alphabetic, lowercase)
    void add(const std::string& word);

    // Searches for a word in the tree; returns true if found, false otherwise
    bool find(const std::string& word) const;

    // Predicts up to 'howMany' words based on a partial word using BFS traversal
    std::vector<std::string> predict(const std::string& partial, std::uint8_t howMany) const;

    // Returns the total number of words stored in the tree
    std::size_t size() const;

private:
    // Private struct representing each node in the 26-ary tree
    struct TreeNode {
        bool endOfWord = false;  // Indicates if this node marks the end of a valid word
        std::array<std::shared_ptr<TreeNode>, 26> children;  // 26 possible children for 'a' to 'z'
    };

    // Root node of the tree
    std::shared_ptr<TreeNode> root;

    // Helper method to recursively count words in the tree
    std::size_t countWords(const std::shared_ptr<TreeNode>& node) const;
};

#endif // WORDTREE_HPP
