#ifndef WORDGENERATOR_H
#define WORDGENERATOR_H

#include <bits/stdc++.h>
using namespace std;

class WordGenerator
{
private:
    vector<char> alpha;

public:
    WordGenerator()
    {
        for (char c = 'a'; c <= 'z'; c++)
            alpha.push_back(c);
        srand(time(0));
    }

    string generateIt()
    {
        int len = 5 + rand() % 6;
        string word;
        word.reserve(len);
        for (int i = 0; i < len; i++)
        {
            char randoChar = alpha[rand() % alpha.size()];
            word += randoChar;
        }
        return word;
    }

    vector<string> generateWords(int count)
    {
        vector<string> words;
        words.reserve(count);
        for (int i = 0; i < count; i++)
            words.push_back(generateIt());
        return words;
    }

    vector<string> generateUniqueWords(int count)
    {
        unordered_set<string> seen;
        vector<string> unique;
        unique.reserve(count);
        int attempt = 0;
        while (unique.size() < count && attempt < count * 3)
        {
            string word = generateIt();
            if (seen.find(word) == seen.end())
            {
                seen.insert(word);
                unique.push_back(word);
            }
            attempt++;
        }
        return unique;
    }

    // Count duplicates in a vector of words
    int countDuplicates(const vector<string> &words)
    {
        unordered_set<string> seen;
        int duplicates = 0;
        for (const string &word : words)
        {
            if (seen.find(word) != seen.end())
            {
                duplicates++;
            }
            else
            {
                seen.insert(word);
            }
        }
        return duplicates;
    }

    // pair<int, vector<string>> countDuplicates(const vector<string> &words)
    // {
    //     unordered_set<string> seen;
    //     unordered_set<string> duplicatesSet;
    //     for (const string &word : words)
    //     {
    //         if (seen.find(word) != seen.end())
    //         {
    //             duplicatesSet.insert(word);
    //         }
    //         else
    //         {
    //             seen.insert(word);
    //         }
    //     }
    //     return {(int)duplicatesSet.size(), vector<string>(duplicatesSet.begin(), duplicatesSet.end())};
    // }
};

#endif