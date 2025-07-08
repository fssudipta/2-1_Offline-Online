#include<bits/stdc++.h>
using namespace std;

string getReverseComplement(const string &s)
{
    unordered_map<char, char> complement = {{'A', 'T'}, {'T', 'A'}, {'C', 'G'}, {'G', 'C'}};
    string result;
    for (char c : s)
    {
        result += complement[c];
    }
    reverse(result.begin(), result.end());
    return result;
}

int main()
{
    int n, l;
    cin >> n >> l;
    vector<string> sequences(n);
    for (int i = 0; i < n; ++i)
    {
        cin >> sequences[i];
    }

    set<pair<string, string>> printed;

    for (int i = 0; i < n; ++i)
    {
        string revComp = getReverseComplement(sequences[i]);
        for (int j = 0; j < n; ++j)
        {
            if (i != j && sequences[j] == revComp)
            {
                string a = sequences[i];
                string b = sequences[j];
                if (a > b)
                    swap(a, b); // ensure consistent pair order
                if (printed.find({a, b}) == printed.end())
                {
                    printed.insert({a, b});
                    cout << a << " " << b << endl;
                }
            }
        }
    }

    return 0;
}
