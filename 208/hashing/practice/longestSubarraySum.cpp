// C++ Code to find longest Subarray With Sum Divisible
// By K using Prefix Sum and Hash map

#include <iostream>
#include <unordered_map>
#include <vector>
using namespace std;

int longestSubarrayDivK(vector<int> &arr, int k)
{
    int n = arr.size(), res = 0;
    unordered_map<int, int> prefIdx;
    int sum = 0;

    // Iterate over all ending points
    for (int i = 0; i < n; i++)
    {

        // prefix sum mod k (handling negative prefix sum)
        sum = ((sum + arr[i]) % k + k) % k;

        // If sum == 0, then update result with the
        // length of subarray arr[0...i]
        if (sum == 0)
            res = i + 1;

        // Update max length for repeating sum
        else if (prefIdx.find(sum) != prefIdx.end())
        {
            res = max(res, i - prefIdx[sum]);
        }

        // Store the first occurrence of sum
        else
        {
            prefIdx[sum] = i;
        }
    }
    return res;
}

int main()
{
    vector<int> arr = {2, 7, 6, 1, 4, 5};
    int k = 3;

    cout << longestSubarrayDivK(arr, k);
}