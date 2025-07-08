#include <iostream>
#include <unordered_set>
#include <string>
#include <algorithm>
using namespace std;

int longestKSubstr(const string &s, int k)
{
    int ans = -1;

    // set to track unique characters in current substring
    unordered_set<char> st;

    for (int i = 0; i < s.size(); i++)
    {

        // reset the set for a new starting index - 'i'
        st.clear();

        // expand the substring from index i to j
        for (int j = i; j < s.size(); j++)
        {
            st.insert(s[j]);

            // number of unique characters becomes exactly k,
            if (st.size() == k)
            {
                ans = max(ans, j - i + 1);
            }

            if (st.size() > k)
                break;
        }
    }

    return ans;
}

int main()
{
    string s = "aabacbebebe";
    int k = 3;

    cout << longestKSubstr(s, k) << '\n';
    return 0;
}