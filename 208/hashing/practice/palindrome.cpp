#include <bits/stdc++.h>
using namespace std;
#define mod 1000000007
#define int long long int

// function to calculate the prefix hash
vector<int> prefixHash(string &str, vector<int> &power)
{
    int n = str.size();
    vector<int> prefix(n + 1);
    prefix[0] = 0;
    prefix[1] = str[0] - 'a' + 1;

    for (int i = 2; i <= n; i++)
    {
        prefix[i] = (prefix[i - 1] +
                     (str[i - 1] - 'a' + 1) * power[i - 1]) %
                    mod;
    }
    return prefix;
}

// function to calculate the suffix hash
vector<int> suffixHash(string &str, vector<int> &power)
{
    int n = str.size();
    vector<int> suffix(n + 1);
    suffix[0] = 0;
    suffix[1] = str[n - 1] - 'a' + 1;

    for (int i = n - 2, j = 2; i >= 0 && j <= n; i--, j++)
    {
        suffix[j] = (suffix[j - 1] +
                     (str[i] - 'a' + 1) * power[j - 1]) %
                    mod;
    }
    return suffix;
}

// A Function to find pow (base, exponent) % MOD
// in log (exponent) time
int modPow(int base, int exp)
{
    if (exp == 0)
        return 1;
    if (exp == 1)
        return base % mod;

    int temp = modPow(base, exp / 2);
    if (exp % 2 == 0)
    {
        return (temp * temp) % mod;
    }
    else
    {
        return (((temp * temp) % mod) * base) % mod;
    }
}

// function to calculate modulo multiplicative inverse
int findMMI(int n)
{
    return modPow(n, mod - 2);
}

// function to solve the queries
int solveQuery(string &str, vector<int> &query,
               vector<int> &prefix, vector<int> &suffix, vector<int> &power)
{
    int l = query[0], r = query[1];
    int n = str.size();

    // hash value of substring [l, r]
    int hash = (((prefix[r + 1] - prefix[l] + mod) %
                 mod) *
                findMMI(power[l])) %
               mod;

    // reverse hash value of substring [l, r]
    int reverseHash = (((suffix[n - l] - suffix[n - r - 1] +
                         mod) %
                        mod) *
                       (findMMI(power[n - r - 1]) % mod)) %
                      mod;

    // check if both hashes are equal
    return (hash == reverseHash) ? 1 : 0;
}

// to compute the powers of 101
vector<int> computePowers(int n)
{
    vector<int> power(n + 1);
    power[0] = 1;
    for (int i = 1; i <= n; i++)
    {
        power[i] = (power[i - 1] * 101) % mod;
    }
    return power;
}

// function to check if the substring
// is a palindrome for each query
vector<int> palindromeQueries(string str,
                              vector<vector<int>> &queries)
{
    int n = str.size();
    int q = queries.size();

    // to store the powers of 101
    vector<int> power = computePowers(n);

    // compute prefix and suffix hash arrays
    vector<int> prefix = prefixHash(str, power);
    vector<int> suffix = suffixHash(str, power);

    // compute the results
    vector<int> res(q);
    for (int i = 0; i < q; i++)
    {
        res[i] = solveQuery(str, queries[i], prefix, suffix, power);
    }
    return res;
}

signed main()
{
    string str = "abaaabaaaba";
    vector<vector<int>> queries =
        {{0, 10}, {5, 8}, {2, 5}, {5, 9}};
    vector<int> res =
        palindromeQueries(str, queries);
    for (auto i : res)
    {
        cout << i << " ";
    }
    return 0;
}