#include <bits/stdc++.h>
using namespace std;

/*
binomial(min) heap data structure properties:
1. a binomial min heap is a collection of binomial tree that satisfy min-heap property
   each individual tree in the collection is a binomial tree, and each tree obeys the min-heap
   property: the key of a node is less than or equal to the keys of its children
2. for every node in a binoial tree: node.key<=all of it's children's keys
3. there can be at most one binomial tree of each degree in the heap
4. B0: a single node
   Bk: one binomial tree of degree k consists of two B(k-1) trees linked together,
   where one becomes the leftmost child of the other.
   Properties of Bk tree:
   1. number of nodes: 2^k
   2. height: k
   3. root degree: k
   4. children of root: B0, B1, B2,..., B(k-1) in order
5. operations and time complexity:
   make-heap: O(1)
   insert: O(logN)
   Find-Min: O(logN)
   Extract-min: O(logN)
   Decrease/Update-key: O(logN)
   Delete/Remove-key: O(logN)
   Union: O(logN)
*/

class Node
{
public:
    int key, degree;
    Node *parent, *child, *sibling;
    // edike child holo leftmost child
    Node(int k) : key(k), degree(0), parent(nullptr), child(nullptr), sibling(nullptr) {}
};

class BinomialHeap
{
private:
    Node *head;
    Node *merge(Node *h1, Node *h2)
    {
        /*
        mainly h1 ar h2 duita tree, eder degree compare korbo,
        smaller degree wala ta ke result list e appent korbo
        erpor merge korte thakbo
        */
        if (!h1)
            return h2;
        if (!h2)
            return h1;
        Node *res = nullptr, *tail = nullptr;
        if (h1->degree <= h2->degree)
        {
            res = tail = h1;
            h1 = h1->sibling;
        }
        else
        {
            res = tail = h2;
            h2 = h2->sibling;
        }
        while (h1 && h2)
        {
            if (h1->degree <= h2->degree)
            {
                tail->sibling = h1;
                tail = h1;
                h1 = h1->sibling;
            }
            else
            {
                tail->sibling = h2;
                tail = h2;
                h2 = h2->sibling;
            }
        }
        tail->sibling = h1 ? h1 : h2;
        return res;
    }

    void TreeLinker(Node *h1, Node *h2)
    {
        /*
        h1 er parent h2, taile h2 er baccha h1, ar h2 er prev baccha h1 er sibling
        plus jehetu h1 ekhn h1 er baccha, degree++
        */
        h1->parent = h2;
        h1->sibling = h2->child;
        h2->child = h1;
        h2->degree++;
    }

    Node *UnionHeap(Node *h1, Node *h2)
    {
        /*
        first e merge korbo duita heap ke. tarpor merged list ta ke traverse korbo.
        jodi duita binomial tree er same degree hoy, taile combine korar try korbo.
        1. ekhon jodi emn hoy, tree gular degree same, abar oder sibling gular degree o same
           kisui korbo na for now
        2. kintu jodi ta na hoy, taile picchi key wala tree ta ke borotar child banabo.
        return kore dibo, shesh.
        */
        Node *h = merge(h1, h2);
        if (!h)
            return nullptr;

        Node *prev = nullptr;
        Node *x = h;
        Node *next = x->sibling;

        while (next)
        {
            if (x->degree != next->degree || (next->sibling && next->sibling->degree == x->degree))
            {
                prev = x;
                x = next;
            }
            else if (x->key <= next->key)
            {
                x->sibling = next->sibling;
                TreeLinker(next, x);
            }
            else
            {
                if (!prev)
                    h = next;
                else
                    prev->sibling = next;
                TreeLinker(x, next);
                x = next;
            }
            next = x->sibling;
        }
        return h;
    }

    Node *Finder(Node *root, int key)
    {
        if (!root)
            return nullptr;
        if (root->key == key)
            return root;
        Node *found = Finder(root->child, key);
        if (found)
            return found;
        return Finder(root->sibling, key);
    }

    void decreaseKey(Node *node, int newKey)
    {
        /*
        aage dekhbo newkey picchi kina. hoile, node er key update kore nibo.
        erpr just heap property maintain kore swap korte thakbo
        */
        if (newKey > node->key)
            return;
        node->key = newKey;
        Node *y = node;
        Node *z = y->parent;

        while (z && y->key < z->key)
        {
            swap(y->key, z->key);
            y = z;
            z = y->parent;
        }
    }

    void DeleteNode(Node *node)
    {
        decreaseKey(node, INT_MIN);
        extractMin();
    }

    /*
    jokhon min elem extract kori, mane root ta, tokhon children ta ke reinsert kora laage heap e.
    kintu children gula emne decreasing order e thake: B2→B1→B0... kintu amader lagbe ulta ta
    reverse korle correct order ta pabo, union operation e naile jhamela hoy
    */
    Node *reverseList(Node *root)
    {
        if (!root || !root->sibling)
            return root;
        Node *prev = nullptr;
        Node *cur = root;
        Node *next;
        while (cur)
        {
            next = cur->sibling;
            cur->sibling = prev;
            cur->parent = nullptr;
            prev = cur;
            cur = next;
        }
        return prev;
    }

    void printTree(Node *root, int treeIndex, ostream &out)
    {
        if (!root)
            return;

        out << "Binomial Tree, B" << treeIndex << endl;

        queue<pair<Node *, int>> q;
        q.push({root, 0});

        int currentLevel = 0;
        vector<int> levelNodes;

        while (!q.empty())
        {
            auto front = q.front();
            q.pop();

            Node *node = front.first;
            int level = front.second;

            if (level != currentLevel)
            {
                if (!levelNodes.empty())
                {
                    out << "Level " << currentLevel << ": ";
                    for (int i = 0; i < levelNodes.size(); i++)
                    {
                        if (i > 0)
                            out << " ";
                        out << levelNodes[i];
                    }
                    out << endl;
                }
                levelNodes.clear();
                currentLevel = level;
            }

            levelNodes.push_back(node->key);

            // queue e children add korlam
            Node *child = node->child;
            while (child)
            {
                q.push({child, level + 1});
                child = child->sibling;
            }
        }

        // Print the last level
        if (!levelNodes.empty())
        {
            out << "Level " << currentLevel << ": ";
            for (int i = 0; i < levelNodes.size(); i++)
            {
                if (i > 0)
                    out << " ";
                out << levelNodes[i];
            }
            out << endl;
        }
    }

public:
    BinomialHeap() : head(nullptr) {}

    void insert(int key)
    {
        Node *newnode = new Node(key);
        head = UnionHeap(head, newnode);
    }

    int findMin()
    {
        if (!head)
            return -1;
        int min = INT_MAX;
        Node *cur = head;

        while (cur)
        {
            if (cur->key < min)
                min = cur->key;
            cur = cur->sibling;
        }
        return min;
    }

    int extractMin()
    {
        if (!head)
            return -1;
        Node *minNode = head;
        Node *prevMin = nullptr;
        Node *prev = nullptr;
        Node *cur = head;

        int min = minNode->key;
        while (cur)
        {
            if (cur->key < min)
            {
                min = cur->key;
                minNode = cur;
                prevMin = prev;
            }
            prev = cur;
            cur = cur->sibling;
        }
        if (prevMin)
            prevMin->sibling = minNode->sibling;
        else
            head = minNode->sibling;
        Node *newHead = reverseList(minNode->child);
        head = UnionHeap(head, newHead);

        int res = minNode->key;
        delete minNode;
        return res;
    }

    void updateKey(int oldKey, int newKey)
    {
        Node *eh = Finder(head, oldKey);
        if (eh)
            decreaseKey(eh, newKey);
    }

    void removeKey(int key)
    {
        Node *node = Finder(head, key);
        if (node)
            DeleteNode(node);
    }

    void print(ostream &out = cout)
    {
        out << "Printing Binomial Heap..." << endl;

        if (!head)
        {
            return;
        }

        Node *current = head;
        while (current)
        {
            printTree(current, current->degree, out);
            current = current->sibling;
        }
    }
};

int main()
{
    BinomialHeap heap;

    ifstream inFile("input.txt");
    ofstream outFile("output.txt");
    string line;
    char operation;
    istream &input = inFile.is_open() ? inFile : cin;
    ostream &output = outFile.is_open() ? outFile : cout;
    while (input >> operation)
    {
        switch (operation)
        {
        case 'I':
        {
            int key;
            input >> key;
            heap.insert(key);
            break;
        }
        case 'F':
        {
            int min = heap.findMin();
            output << "Find Min returned: " << min << endl;
            break;
        }
        case 'E':
        {
            int min = heap.extractMin();
            output << "Extract Min returned: " << min << endl;
            break;
        }
        case 'D':
        {
            int oldKey, newKey;
            input >> oldKey >> newKey;
            heap.updateKey(oldKey, newKey);
            break;
        }
        case 'R':
        {
            int key;
            input >> key;
            heap.removeKey(key);
            break;
        }
        case 'P':
        {
            heap.print(output);
            break;
        }
        default:
        {
            output << "Jhamela" << endl;
            break;
        }
        }
    }
    if (inFile.is_open())
        inFile.close();
    if (outFile.is_open())
        outFile.close();
    return 0;
}