#include "ExistentialDictionary.h"

ExistentialDictionary::ExistentialDictionary() :
    initialNode(LetterNode(' '))
{}


ExistentialDictionary::ExistentialDictionary(const std::string& filepath) :
    initialNode(LetterNode(' '))
{
    std::ifstream stream(filepath);
    if (stream.good()) {

        std::string word;
        while (stream >> word) {
            addWord(word);
        }

    }
    else {
        std::cout << "Filepath given to instance of ExistentialDictionary was invalid\n";
        //__debugbreak();
    }
    stream.close();
}


void ExistentialDictionary::addWord(const std::string& word) {

    if (wordExists(word)) {
        return;
    }

    LetterNode* currentNode = &initialNode;
    for (char c : word) {

        bool letterInList = false;
        int nextLetterIndex = 0;
        for (LetterNode& n : currentNode->connections) {
            if (n.letter == c) {
                letterInList = true;
                break;
            }

            nextLetterIndex++;
        }

        if (!letterInList) {
            currentNode->connections.push_back(LetterNode(c));
        }

        currentNode = &currentNode->connections[nextLetterIndex];

    }

    currentNode->isWord = true;
    wordCount++;
}


bool ExistentialDictionary::wordExists(const std::string& word) {

    LetterNode* currentNode = &initialNode;

    for (char c : word) {

        bool letterInList = false;
        int nextLetterIndex = 0;
        for (LetterNode& n : currentNode->connections) {
            if (n.letter == c) {
                letterInList = true;
                break;
            }

            nextLetterIndex++;
        }

        if (!letterInList) {
            return false;
        }

        currentNode = &currentNode->connections[nextLetterIndex];

    }

    if (currentNode->isWord == true) {
        return true;
    }
    return false;

}

std::string ExistentialDictionary::getRandomWord(int length)
{

    LetterNode* currentNode = &initialNode;
    std::string currentWord;
    
    bool wordFound = false;
    while (!wordFound) {
        currentWord = "";
        currentNode = &initialNode;
        wordFound = true;

        for (int i = 0; i < length; i++) {
            if (currentNode->connections.size() == 0) {
                wordFound = false;
                break;
            }

            currentNode = &currentNode->connections[rand() % currentNode->connections.size()];
            currentWord += currentNode->letter;
        }

        if (!currentNode->isWord) {
            wordFound = false;
        }
    }

    return currentWord;
}

int ExistentialDictionary::getWordCount()
{
    return wordCount;
}
