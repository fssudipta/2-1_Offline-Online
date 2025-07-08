// C++ program to find all Duplicate Subtrees
/*
Step by step approach:
Traverse the binary tree recursively, starting from the root.
For each node,
If it is NULL, return "N" to represent a null node.
Recursively serialize the left and right subtrees of the current node.
Combine the serialized left subtree, the current node's value, and the serialized right subtree into a single string in the format "(left) value (right)".
Use a hash map to count how many times each serialized subtree string appears.
If a serialized subtree string appears exactly twice ( to avoid adding same duplicate subtree into result), add the current node (root of the subtree) to the result list.
Return the serialized string for further processing in the recursion.
After traversing the entire tree, the result list will contain the roots of all duplicate subtrees.
*/
#include <bits/stdc++.h>
using namespace std;

class Node
{
public:
    int data;
    Node *left, *right;
    Node(int x)
    {
        data = x;
        left = nullptr;
        right = nullptr;
    }
};

// Function to serialize all subtrees
// of a binary tree.
string serializeTree(Node *root, unordered_map<string, int> &map,
                     vector<Node *> &ans)
{
    if (!root)
        return "N";

    string left = serializeTree(root->left, map, ans);
    string right = serializeTree(root->right, map, ans);

    string val = to_string(root->data);

    // Subtree serialization
    // left - root - right
    string curr = "(" + left + ")" + val + "(" + right + ")";

    map[curr]++;

    // If such subtree already exists
    // add root to answer
    if (map[curr] == 2)
    {
        ans.push_back(root);
    }

    return curr;
}

// Function to print all duplicate
// subtrees of a binary tree.
vector<Node *> printAllDups(Node *root)
{

    // Hash map to store count of
    // subtrees.
    unordered_map<string, int> map;
    vector<Node *> ans;

    // Function which will serialize all subtrees
    // and store duplicate subtrees into answer.
    serializeTree(root, map, ans);

    return ans;
}

void preOrder(Node *root)
{
    if (root == nullptr)
        return;

    cout << root->data << " ";
    preOrder(root->left);
    preOrder(root->right);
}

int main()
{
    Node *root = new Node(1);
    root->left = new Node(2);
    root->right = new Node(3);
    root->left->left = new Node(4);
    root->right->left = new Node(2);
    root->right->right = new Node(4);
    root->right->left->left = new Node(4);

    vector<Node *> ans = printAllDups(root);

    for (Node *node : ans)
    {
        preOrder(node);
        cout << endl;
    }

    return 0;
}