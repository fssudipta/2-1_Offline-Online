// CPP program of the above approach
#include <bits/stdc++.h>
using namespace std;

struct Item
{
    double ratio;
    int index;
};

bool compare(Item a, Item b) { return a.ratio > b.ratio; }

int knapsack_greedy(int N, int W, vector<int> &values,
                    vector<int> &weights)
{
    vector<Item> ratios(N);
    //  Calculate value-to-weight ratios for all items
    for (int i = 0; i < N; i++)
    {
        ratios[i].ratio = static_cast<double>(values[i]) / weights[i];
        ratios[i].index = i;
    }

    sort(ratios.begin(), ratios.end(), compare);
    int total_value = 0, total_weight = 0;
    for (const auto &item : ratios)
    {
        int index = item.index;
        if (total_weight + weights[index] <= W)
        {
            total_value += values[index];
            total_weight += weights[index];
        }
    }
    return total_value;
}

// Driver's Code
int main()
{
    int N = 3;
    int W = 4;
    vector<int> values = {1, 2, 3};
    vector<int> weights = {4, 5, 1};

    int result = knapsack_greedy(N, W, values, weights);
    cout << result << endl;

    return 0;
}

