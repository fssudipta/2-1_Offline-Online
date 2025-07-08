#include <bits/stdc++.h>
#include "HashTable.hpp"
#include "WordGenerator.hpp"

using namespace std;
using namespace std::chrono;

// Helper function to calculate uniqueness ratio of two separate hash functions, just to check
double calculateUniquenessRatio(HashTable &ht, const vector<string> &words, int hashFunc)
{
    set<unsigned long long> uniqueHashes;
    for (const string &word : words)
    {
        unsigned long long hash;
        if (hashFunc == 1)
        {
            hash = ht.hash1(word);
        }
        else
        {
            hash = ht.hash2(word);
        }
        uniqueHashes.insert(hash);
    }
    return (double)uniqueHashes.size() / words.size();
}

struct TestResult
{
    int collisions;
    double avgSearchTime;
    double avgProbes;
    double avgSearchTimeAfterDeletion;
    double avgProbesAfterDeletion;
    double uniquenessRatio;
};

TestResult testMethod(HashTable &ht, const vector<string> &words, const string &methodName, int hashFunc)
{
    TestResult result = {0, 0.0, 0.0, 0.0, 0.0, 0.0};
    ht.resetTable();
    ht.resetMetrics();

    // insert all words
    for (const string &word : words)
    {
        if (methodName == "Separate Chaining")
        {
            ht.insertChaining(word, hashFunc);
        }
        else if (methodName == "Linear Probing")
        {
            ht.insertLinearProbing(word, hashFunc);
        }
        else if (methodName == "Double Hashing")
        {
            ht.insertDoubleHashing(word);
        }
    }
    result.collisions = ht.getCollisions();
    result.uniquenessRatio = calculateUniquenessRatio(ht, words, hashFunc);

    // 1. before Deletion Search Test (10% random elements)
    vector<string> searchSetBefore = words;
    // fisher-yates shuffle: https://www.geeksforgeeks.org/dsa/shuffle-a-given-array-using-fisher-yates-shuffle-algorithm/
    // to get accurate result
    for (int i = searchSetBefore.size() - 1; i > 0; i--)
    {
        int j = rand() % (i + 1);
        swap(searchSetBefore[i], searchSetBefore[j]);
    }

    int searchCount = max(1, (int)(words.size() * 0.1));
    searchSetBefore.resize(searchCount);

    ht.resetMetrics();

    auto startBefore = high_resolution_clock::now();

    for (const string &word : searchSetBefore)
    {
        if (methodName == "Separate Chaining")
        {
            ht.searchChaining(word, hashFunc);
        }
        else if (methodName == "Linear Probing")
        {
            ht.searchLinearProbing(word, hashFunc);
        }
        else if (methodName == "Double Hashing")
        {
            ht.searchDoubleHashing(word);
        }
    }

    auto endBefore = high_resolution_clock::now();

    result.avgSearchTime = duration_cast<nanoseconds>(endBefore - startBefore).count() / (double)searchCount;
    result.avgProbes = ht.getAverageProbes();

    // 2. delete 10% random elements (same set i nilam jate shubidha hoy)
    vector<string> deleteSet = searchSetBefore;
    for (const string &word : deleteSet)
    {
        if (methodName == "Linear Probing")
        {
            ht.deleteLinearProbing(word, hashFunc);
        }
        else if (methodName == "Double Hashing")
        {
            ht.deleteDoubleHashing(word);
        }
        else
        {
            ht.deleteSeparateChain(word, hashFunc);
        }
    }

    // 3. after Deletion Search Test (50% deleted, 50% existing)
    vector<string> searchSetAfter;

    // half from deleted set (should not be found)
    for (size_t i = 0; i < deleteSet.size() / 2; i++)
    {
        searchSetAfter.push_back(deleteSet[i]);
    }

    // half from remaining words (should be found)
    vector<string> remainingWords;
    for (const string &word : words)
    {
        if (find(deleteSet.begin(), deleteSet.end(), word) == deleteSet.end())
        {
            remainingWords.push_back(word);
        }
    }

    // shuffling
    for (int i = remainingWords.size() - 1; i > 0; i--)
    {
        int j = rand() % (i + 1);
        swap(remainingWords[i], remainingWords[j]);
    }
    for (size_t i = 0; i < deleteSet.size() / 2 && i < remainingWords.size(); i++)
    {
        searchSetAfter.push_back(remainingWords[i]);
    }

    ht.resetMetrics();
    auto startAfter = high_resolution_clock::now();
    for (const string &word : searchSetAfter)
    {
        if (methodName == "Separate Chaining")
        {
            ht.searchChaining(word, hashFunc);
        }
        else if (methodName == "Linear Probing")
        {
            ht.searchLinearProbing(word, hashFunc);
        }
        else if (methodName == "Double Hashing")
        {
            ht.searchDoubleHashing(word);
        }
    }
    auto endAfter = high_resolution_clock::now();
    result.avgSearchTimeAfterDeletion = duration_cast<nanoseconds>(endAfter - startAfter).count() / (double)searchSetAfter.size();
    result.avgProbesAfterDeletion = ht.getAverageProbes();

    return result;
}

string formatProbes(double probes, const string &method)
{
    if (method == "Separate Chaining")
    {
        return "  N/A";
    }
    ostringstream oss;
    oss << fixed << setprecision(3) << probes;
    return oss.str();
}

void printTableForLoadFactor(double loadFactor, int tableNumber)
{
    const int TABLE_SIZE = (int)(10000.0 / loadFactor);
    const int NUM_WORDS = 10000;

    WordGenerator wg;
    vector<string> words = wg.generateUniqueWords(NUM_WORDS);

    HashTable ht(TABLE_SIZE);

    cout << "\nFor load factor " << fixed << setprecision(1) << loadFactor << "\n\n";

    cout << "+---------------------+----------------------------------------------+----------------------------------------------+\n";
    cout << "|                     |               Hash1 Function                 |               Hash2 Function                 |\n";
    cout << "+---------------------+----------------------------------------------+----------------------------------------------+\n";
    cout << "|                     |        Before Deletion     | After Deletion  |        Before Deletion     | After Deletion  |\n";
    cout << "+---------------------+----------+----------+------+----------+------+----------+----------+------+----------+------+\n";
    cout << "|       Method        | # of     |   Avg    | Avg  |   Avg    | Avg  | # of     |   Avg    | Avg  |   Avg    | Avg  |\n";
    cout << "|                     |Collisions| Search   |Probes| Search   |Probes|Collisions| Search   |Probes| Search   |Probes|\n";
    cout << "|                     | during   |  Time    |      |  Time    |      | during   |  Time    |      |  Time    |      |\n";
    cout << "|                     |insertion |          |      |          |      |insertion |          |      |          |      |\n";
    cout << "+---------------------+----------+----------+------+----------+------+----------+----------+------+----------+------+\n";

    vector<string> methods = {"Separate Chaining", "Linear Probing", "Double Hashing"};

    for (const string &method : methods)
    {
        TestResult result1 = testMethod(ht, words, method, 1);
        TestResult result2 = testMethod(ht, words, method, 2);
        if (method == "Double Hashing")
            result2 = result1;
        string probesStr1 = formatProbes(result1.avgProbes, method);
        string probesStr2 = formatProbes(result2.avgProbes, method);

        // same for after deletion
        string probesAfterStr1 = formatProbes(result1.avgProbesAfterDeletion, method);
        string probesAfterStr2 = formatProbes(result2.avgProbesAfterDeletion, method);

        cout << "|" << setw(21) << left << method;
        cout << "|" << setw(10) << right << result1.collisions;
        cout << "|" << setw(10) << right << fixed << setprecision(2) << result1.avgSearchTime;
        cout << "|" << setw(6) << right << fixed << setprecision(3) << probesStr1;
        cout << "|" << setw(10) << right << fixed << setprecision(2) << result1.avgSearchTimeAfterDeletion;
        cout << "|" << setw(6) << right << fixed << setprecision(3) << probesAfterStr1;
        cout << "|" << setw(10) << right << result2.collisions;
        cout << "|" << setw(10) << right << fixed << setprecision(2) << result2.avgSearchTime;
        cout << "|" << setw(6) << right << fixed << setprecision(3) << probesStr2;
        cout << "|" << setw(10) << right << fixed << setprecision(2) << result2.avgSearchTimeAfterDeletion;
        cout << "|" << setw(6) << right << fixed << setprecision(3) << probesAfterStr2 << "|\n";

        if (method == "Separate Chaining")
        {
            cout << "|with balanced BST    |          |          |      |          |      |          |          |      |          |      |\n";
        }
        else if (method == "Linear Probing")
        {
            cout << "|with Step Adjustment |          |          |      |          |      |          |          |      |          |      |\n";
        }
    }

    cout << "+---------------------+----------+----------+------+----------+------+----------+----------+------+----------+------+\n";

    // print uniqueness ratios
    cout << "\nUniqueness Ratios:\n";
    TestResult tempResult1 = testMethod(ht, words, "Separate Chaining", 1);
    TestResult tempResult2 = testMethod(ht, words, "Separate Chaining", 2);
    cout << "Hash1 (Polynomial): " << fixed << setprecision(4) << tempResult1.uniquenessRatio << "\n";
    cout << "Hash2 (DJB2): " << fixed << setprecision(4) << tempResult2.uniquenessRatio << "\n";
    cout << "\nTable " << tableNumber << "\n";
    cout << "successful search count: " << ht.getSuccessfulSearchCount() << endl;
    cout << "unsuccessful search count: " << ht.getUnsuccessfulSearchCount() << endl;
}

int main()
{
    cout << "Hash Table Performance Analysis" << endl;
    cout << "===============================" << endl;
    cout << "Hash Functions: Polynomial Rolling Hash (Hash1) and DJB2 (Hash2)" << endl;
    cout << "Word Number: 10000" << endl;
    cout << "Implementation: Separate Chaining with BST, Linear Probing, Double Hashing" << endl;

    vector<double> loadFactors = {0.4, 0.5, 0.6, 0.7, 0.8, 0.9};

    for (int i = 0; i < loadFactors.size(); i++)
    {
        printTableForLoadFactor(loadFactors[i], i + 1);
    }

    return 0;
}