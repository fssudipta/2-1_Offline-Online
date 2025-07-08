#include <iostream>
#include <unordered_map>
using namespace std;

int main()
{
    int l, k;
    cin >> l >> k;
    string dna;
    cin >> dna;

    unordered_map<string, int> kmerCount;

    for (int i = 0; i <= l - k; ++i)
    {
        string kmer = dna.substr(i, k);
        kmerCount[kmer]++;
    }

    for (auto &it : kmerCount)
    {
        cout << it.first << " " << it.second << endl;
    }

    return 0;
}
