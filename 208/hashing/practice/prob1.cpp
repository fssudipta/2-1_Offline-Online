// employee count
// C++ program to find number of
// employees under every manager
#include <bits/stdc++.h>
using namespace std;

// Helper function to recursively count
// the number of employees under a manager
int findEmployees(unordered_map<string, vector<string>> &dataset,
                  unordered_map<string, int> &result, string mngr)
{

    // if the result is already calculated
    if (result[mngr] != -1)
    {
        return result[mngr];
    }

    // if the manager is not present in the dataset
    if (dataset.find(mngr) == dataset.end())
    {
        result[mngr] = 0;
        return 0;
    }

    // count the number of employees under the manager
    int count = 0;
    for (auto i : dataset[mngr])
    {

        if (result[i] != -1)
            count += result[i];

        else
        {
            result[i] = findEmployees(dataset, result, i);
            count += result[i];
        }
    }
    result[mngr] = count + dataset[mngr].size();
    return result[mngr];
}

// Function to find the count of
// employees under each manager
vector<pair<string, int>> findCount(vector<vector<string>> arr)
{

    // to store the employees under each manager
    unordered_map<string, vector<string>> dataset;

    // to store the results
    unordered_map<string, int> result;
    for (auto i : arr)
    {
        if (i[1] == i[0])
            continue;
        dataset[i[1]].push_back(i[0]);

        // initialize the result with -1
        result[i[0]] = -1;
        result[i[1]] = -1;
    }

    for (auto i : result)
    {

        // if the manager is not visited yet
        if (i.second == -1)
        {
            findEmployees(dataset, result, i.first);
        }
    }

    vector<pair<string, int>> res;
    for (auto i : result)
    {
        res.push_back({i.first, i.second});
    }

    return res;
}

int main()
{
    vector<vector<string>> arr = {
        {"A", "C"},
        {"B", "C"},
        {"C", "F"},
        {"D", "E"},
        {"E", "F"},
        {"F", "F"}};

    vector<pair<string, int>> result = findCount(arr);

    // sort the result
    sort(result.begin(), result.end());
    for (auto i : result)
    {
        cout << i.first << ": " << i.second << endl;
    }
    return 0;
}