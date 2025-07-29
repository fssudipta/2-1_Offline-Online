#include <bits/stdc++.h>
using namespace std;

enum SlotState
{
    EMPTY,
    OCCUPIED,
    DELETED
};

struct Slot
{
    int process_id;
    SlotState state;

    Slot() : process_id(-1), state(EMPTY) {}
};

bool isPrime(int n)
{
    if (n < 2)
        return false;
    if (n == 2 || n == 3)
        return true;
    if (n % 2 == 0 || n % 3 == 0)
        return false;
    for (int i = 5; i * i <= n; i += 6)
        if (n % i == 0 || n % (i + 2) == 0)
            return false;
    return true;
}

int nextPrime(int n)
{
    while (!isPrime(n))
        n++;
    return n;
}

int prevPrime(int n)
{
    if (n <= 2)
        return 2;
    while (!isPrime(n))
        n--;
    return n;
}

class MemoryManager
{
    vector<Slot> table;
    int N, P_MAX, deletions = 0, occupied = 0;
    int maxProbe = 0;
    int initialSize;

public:
    MemoryManager(int n, int p) : N(n), P_MAX(p), initialSize(n)
    {
        table.resize(N);
    }

    void printRehashInfo(string when, int oldSize, int newSize, int newMaxProbe)
    {
        cout << "--- Rehash Triggered: " << when << " ---\n";
        cout << "Table Size: " << oldSize << '\n';
        cout << "Active Processes: " << occupied << '\n';
        cout << "Load Factor: " << fixed << setprecision(2) << (double)occupied / oldSize << '\n';
        cout << "Max Probe Sequence: " << maxProbe << '\n';
    }

    void rehash(int newSize)
    {
        int oldSize = N;
        vector<Slot> oldTable = table;

        N = newSize;
        table.clear();
        table.resize(N);
        occupied = 0;
        maxProbe = 0;

        for (Slot &slot : oldTable)
        {
            if (slot.state == OCCUPIED)
            {
                insert(slot.process_id, false); // rehash insert
            }
        }
    }

    void insert(int pid, bool allowRehash = true)
    {
        int index, probes = 0;
        bool inserted = false;
        for (int j = 0; j < N; j++)
        {
            index = (pid + j * j) % N;
            probes++;
            if (table[index].state == EMPTY || table[index].state == DELETED)
            {
                table[index].process_id = pid;
                table[index].state = OCCUPIED;
                occupied++;
                cout << "ALLOCATE " << pid << ": Inserted at index " << index
                     << " (probes: " << probes << ")\n";
                inserted = true;
                break;
            }
        }

        maxProbe = max(maxProbe, probes);

        if (!inserted && allowRehash)
        {
            printRehashInfo("BEFORE", N, 0, maxProbe);
            int newSize = nextPrime(2 * N);
            rehash(newSize);
            printRehashInfo("AFTER", N, newSize, maxProbe);
            insert(pid); // try again
        }
        else if (probes > P_MAX && allowRehash)
        {
            printRehashInfo("BEFORE", N, 0, maxProbe);
            int newSize = nextPrime(2 * N);
            rehash(newSize);
            printRehashInfo("AFTER", N, newSize, maxProbe);
            insert(pid); // try again
        }
    }

    void freeProcess(int pid)
    {
        int index, probes = 0;
        for (int j = 0; j < N; j++)
        {
            index = (pid + j * j) % N;
            if (table[index].state == EMPTY)
                break; // not found
            if (table[index].state == OCCUPIED && table[index].process_id == pid)
            {
                table[index].state = DELETED;
                deletions++;
                occupied--;
                cout << "FREE " << pid << ": Freed from index " << index << '\n';

                if (deletions % 5 == 0 && (double)occupied / N < 0.2 && N > initialSize)
                {
                    printRehashInfo("BEFORE", N, 0, maxProbe);
                    int newSize = prevPrime(N / 2);
                    if (newSize < initialSize)
                        return;
                    rehash(newSize);
                    printRehashInfo("AFTER", N, newSize, maxProbe);
                }
                return;
            }
        }
    }

    void processCommand(string command, int pid)
    {
        if (command == "ALLOCATE")
        {
            insert(pid);
        }
        else if (command == "FREE")
        {
            freeProcess(pid);
        }
    }
};

int main()
{
    int N, P_MAX, Q;
    cin >> N >> P_MAX >> Q;
    MemoryManager mm(N, P_MAX);

    for (int i = 0; i < Q; i++)
    {
        string cmd;
        int pid;
        cin >> cmd >> pid;
        mm.processCommand(cmd, pid);
    }
    return 0;
}
