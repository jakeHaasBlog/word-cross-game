#pragma once

#include <iostream>
#include <array>
#include <vector>
#include <fstream>


class LetterNode {
public:
    LetterNode(char letter) :
        letter(letter)
    {}

    const char letter;
    bool isWord = false;

    std::vector<LetterNode> connections;

};

class ExistentialDictionary {
public:
    ExistentialDictionary();
    ExistentialDictionary(const std::string& filepath);

    void addWord(const std::string& word);
    bool wordExists(const std::string& word);

    std::string getRandomWord(int length = -1);
    int getWordCount();

private:
    LetterNode initialNode;
    int wordCount = 0;

};




