#include <iostream>
#include <queue>
#include <unordered_map>
using namespace std;

struct TrieNode {
    unordered_map<char, TrieNode*> children;
    TrieNode* fail;
    bool isEndOfWord;
    int wordIndex;
    
    TrieNode() : fail(nullptr), isEndOfWord(false), wordIndex(-1) {}
};

class AhoCorasick {
public:
    AhoCorasick(const string keywords[], int n) {
        root = new TrieNode();
        buildTrie(keywords, n);
        buildFailureLinks();
    }

    void search(const string& text) {
    TrieNode* node = root;  // Start from the root of the Trie

    for (int i = 0; i < text.size(); ++i) {  // Iterate over each character in the text
        while (node && node->children.find(text[i]) == node->children.end())
            node = node->fail;  // Follow the failure links until a match is found or the root is reached
        
        if (!node)
            node = root;  // If no match is found, reset to root
        else
            node = node->children[text[i]];  // Move to the next node in the Trie
        
        TrieNode* temp = node;  // Use a temporary node to check for matches
        while (temp) {
            if (temp->isEndOfWord)  // If the current node marks the end of a word
                cout << "Pattern found at position " << i << " ending with index " << temp->wordIndex << endl;
            temp = temp->fail;  // Follow the failure links to check for other potential matches
        }
    }
}


private:
    TrieNode* root;

    void buildTrie(const string keywords[], int n) {
        for (int i = 0; i < n; ++i) {
            TrieNode* node = root;
            for (char c : keywords[i]) {
                if (node->children.find(c) == node->children.end())
                    node->children[c] = new TrieNode();
                node = node->children[c];
            }
            node->isEndOfWord = true;
            node->wordIndex = i;
        }
    }

    void buildFailureLinks() {
    queue<TrieNode*> q;  // Create a queue to perform BFS

    // Initialize the failure links for the children of the root
    for (auto& pair : root->children) {
        pair.second->fail = root;  // Set the failure link of root's children to root
        q.push(pair.second);  // Add root's children to the queue
    }

    // Process each node in the queue
    while (!q.empty()) {
        TrieNode* current = q.front();  // Get the front node
        q.pop();  // Remove it from the queue

        // Iterate over all the children of the current node
        for (auto& pair : current->children) {
            char c = pair.first;  // Get the character
            TrieNode* child = pair.second;  // Get the child node
            TrieNode* failNode = current->fail;  // Get the failure link of the current node

            // Find the longest suffix that is also a prefix
            while (failNode && failNode->children.find(c) == failNode->children.end())
                failNode = failNode->fail;

            // Set the failure link of the child node
            if (failNode)
                child->fail = failNode->children[c];
            else
                child->fail = root;

            // Add the child to the queue for further processing
            q.push(child);
        }
    }
}

};

int main() {
    string keywords[] = {"he", "she", "his", "hers"};
    int n = sizeof(keywords) / sizeof(keywords[0]);
    string text = "ahishers";
    
    AhoCorasick ac(keywords, n);
    ac.search(text);
    
    return 0;
}
