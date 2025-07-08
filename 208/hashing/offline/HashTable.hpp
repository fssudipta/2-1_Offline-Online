#ifndef HASHTABLE_HPP
#define HASHTABLE_HPP

#include <bits/stdc++.h>
#include "RBTree.hpp"
using namespace std;

struct HashEntry
{
    string key;
    int value;
    bool isEmpty;
    bool isDeleted;

    HashEntry() : key(""), value(-1), isEmpty(true), isDeleted(false) {}
    HashEntry(string k, int v) : key(k), value(v), isEmpty(false), isDeleted(false) {}
};

class HashTable
{
private:
    int tableSize;
    int nextValue;

    vector<RBTree *> chainTable; //  separate chaining
    vector<HashEntry> openTable; //  open addressing

    int collisionCount;
    long long totalProbes;
    int successfulSearchCount;
    int unsuccessfulSearchCount;

    bool isPrime(int n)
    {
        if (n <= 1)
            return false;
        if (n <= 3)
            return true;
        if (n % 2 == 0 || n % 3 == 0)
            return false;

        for (int i = 5; i * i <= n; i += 6)
        {
            if (n % i == 0 || n % (i + 2) == 0)
                return false;
        }
        return true;
    }

    int nextPrime(int n)
    {
        while (!isPrime(n))
            n++;
        return n;
    }

public:
    HashTable(int size) : tableSize(nextPrime(size)),
                          nextValue(1),
                          collisionCount(0),
                          totalProbes(0),
                          successfulSearchCount(0),
                          unsuccessfulSearchCount(0)
    {
        // init chaining table
        chainTable.resize(tableSize);
        for (int i = 0; i < tableSize; i++)
        {
            chainTable[i] = new RBTree();
        }
        // init open addressing table
        openTable.resize(tableSize);
    }

    ~HashTable()
    {
        for (auto *tree : chainTable)
        {
            delete tree;
        }
    }

    // Polynomial rolling Hash
    int hash1(const string &key)
    {
        const int PRIME_BASE = 31;
        long long hash = 0;
        long long power = 1;

        for (char c : key)
        {
            hash = (hash + (c - 'a' + 1) * power) % tableSize;
            power = (power * PRIME_BASE) % tableSize;
        }

        return (int)hash;
    }

    // DJB2 Hash
    int hash2(const string &key)
    {
        unsigned long hash = 5381;
        for (char c : key)
        {
            hash = ((hash << 5) + hash) + c;
        }
        return (int)(hash % tableSize);
    }

    // separate chaining operations
    bool insertChaining(const string &key, int hashFunction)
    {
        int index = (hashFunction == 1) ? hash1(key) : hash2(key);
        if (chainTable[index]->search(key).first)
        {
            return false; // Key already exists
        }
        if (!chainTable[index]->isEmpty())
        {
            collisionCount++;
        }
        chainTable[index]->insert(key, nextValue++);
        return true;
    }

    pair<bool, int> searchChaining(const string &key, int hashFunction)
    {
        int index = (hashFunction == 1) ? hash1(key) : hash2(key);
        auto result = chainTable[index]->search(key);
        if (result.first)
        {
            successfulSearchCount++;
        }
        else
        {
            unsuccessfulSearchCount++;
        }
        return result;
    }

    // Linear probing operations
    bool insertLinearProbing(const string &key, int hashFunction)
    {
        int index = (hashFunction == 1) ? hash1(key) : hash2(key);
        int originalIndex = index;
        int probes = 0;

        while (probes < tableSize)
        {
            if (openTable[index].isEmpty || openTable[index].isDeleted)
            {
                break;
            }
            // key already exists
            if (openTable[index].key == key)
            {
                return false;
            }
            probes++;
            index = (originalIndex + probes) % tableSize;
        }

        if (probes >= tableSize)
        {
            return false; // Table full hoye gese
        }

        if (probes > 0)
        {
            collisionCount += probes;
        }

        openTable[index] = HashEntry(key, nextValue++);
        return true;
    }

    pair<bool, int> searchLinearProbing(const string &key, int hashFunction)
    {
        int index = (hashFunction == 1) ? hash1(key) : hash2(key);
        int originalIndex = index;
        int probes = 0;

        while (probes < tableSize)
        {
            // found it
            if (!openTable[index].isEmpty && !openTable[index].isDeleted &&
                openTable[index].key == key)
            {
                totalProbes += probes + 1;
                successfulSearchCount++;
                return {true, openTable[index].value};
            }

            // found truly empty slot (never occupied): cormen ref: mark it
            if (openTable[index].isEmpty && !openTable[index].isDeleted)
            {
                totalProbes += probes + 1;
                unsuccessfulSearchCount++;
                return {false, -1};
            }

            probes++;
            index = (originalIndex + probes) % tableSize;
        }

        // searched entire table
        totalProbes += probes;
        unsuccessfulSearchCount++;
        return {false, -1};
    }

    // Double hashing operations
    bool insertDoubleHashing(const string &key)
    {
        int h1 = hash1(key);
        int h2 = hash2(key);
        if (h2 == 0)
            h2 = 1; // Ensure h2 isn't 0
        int probes = 0;

        while (probes < tableSize)
        {
            int index = (h1 + probes * h2) % tableSize;

            if (openTable[index].isEmpty || openTable[index].isDeleted)
            {
                if (probes > 0)
                {
                    collisionCount++;
                }
                openTable[index] = HashEntry(key, nextValue++);
                return true;
            }

            if (openTable[index].key == key)
            {
                return false; // key exists
            }

            probes++;
        }

        return false; // Table full hoye gese
    }

    pair<bool, int> searchDoubleHashing(const string &key)
    {
        int h1 = hash1(key);
        int h2 = hash2(key);
        if (h2 == 0)
            h2 = 1;
        int probes = 0;

        while (probes < tableSize)
        {
            int index = (h1 + probes * h2) % tableSize;

            if (!openTable[index].isEmpty && !openTable[index].isDeleted &&
                openTable[index].key == key)
            {
                totalProbes += probes + 1;
                successfulSearchCount++;
                return {true, openTable[index].value};
            }

            if (openTable[index].isEmpty && !openTable[index].isDeleted)
            {
                totalProbes += probes + 1;
                unsuccessfulSearchCount++;
                return {false, -1};
            }

            probes++;
        }

        totalProbes += probes;
        unsuccessfulSearchCount++;
        return {false, -1};
    }

    // deletion operations
    bool deleteLinearProbing(const string &key, int hashFunction)
    {
        int index = (hashFunction == 1) ? hash1(key) : hash2(key);
        int originalIndex = index;
        int probes = 0;

        while (probes < tableSize)
        {
            if (openTable[index].isEmpty && !openTable[index].isDeleted)
            {
                return false;
            }

            if (!openTable[index].isDeleted && openTable[index].key == key)
            {
                openTable[index].isDeleted = true;
                return true;
            }

            probes++;
            index = (originalIndex + probes) % tableSize;
        }

        return false;
    }

    bool deleteDoubleHashing(const string &key)
    {
        int h1 = hash1(key);
        int h2 = hash2(key);
        if (h2 == 0)
            h2 = 1;
        int probes = 0;

        while (probes < tableSize)
        {
            int index = (h1 + probes * h2) % tableSize;

            if (openTable[index].isEmpty && !openTable[index].isDeleted)
            {
                return false;
            }

            if (!openTable[index].isDeleted && openTable[index].key == key)
            {
                openTable[index].isDeleted = true;
                return true;
            }

            probes++;
        }

        return false;
    }

    bool deleteSeparateChain(const string &key, int hashFunction)
    {
        int index = (hashFunction == 1) ? hash1(key) : hash2(key);
        // access the RBTree at that index
        RBTree *tree = chainTable[index];
        // find the key first
        Node *node = tree->findProductNode(key);
        if (node == tree->getNil())
        {
            return false; // key not found
        }
        // remove the key from the tree
        return tree->removeProduct(key, cout);
    }

    void resetTable()
    {
        // reset chaining table
        for (auto *tree : chainTable)
        {
            delete tree;
        }
        chainTable.clear();
        chainTable.resize(tableSize);
        for (int i = 0; i < tableSize; i++)
        {
            chainTable[i] = new RBTree();
        }

        // reset open addressing table
        openTable.clear();
        openTable.resize(tableSize);

        // reset counters
        resetMetrics();
        nextValue = 1;
    }

    void resetMetrics()
    {
        collisionCount = 0;
        totalProbes = 0;
        successfulSearchCount = 0;
        unsuccessfulSearchCount = 0;
    }

    // Getters
    int getCollisions() const { return collisionCount; }
    double getAverageProbes() const
    {
        int totalSearches = successfulSearchCount + unsuccessfulSearchCount;
        return totalSearches > 0 ? (double)(totalProbes) / totalSearches : 0.0;
    }
    int getTableSize() const { return tableSize; }
    int getCurrentValue() const { return nextValue; }
    int getSuccessfulSearchCount() const { return successfulSearchCount; }
    int getUnsuccessfulSearchCount() const { return unsuccessfulSearchCount; }
    double getSuccessRate() const
    {
        int totalSearches = successfulSearchCount + unsuccessfulSearchCount;
        return totalSearches > 0 ? (double)(successfulSearchCount) / totalSearches : 0.0;
    }
};

#endif 