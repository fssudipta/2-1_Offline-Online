#ifndef RBTREE_HPP
#define RBTREE_HPP

#include <bits/stdc++.h>
using namespace std;

enum Color
{
    RED,
    BLACK
};

struct Node
{
    string key;
    int value;
    Color color;
    Node *left;
    Node *right;
    Node *parent;

    Node(string k, int v)
        : key(k), value(v), color(RED), left(nullptr), right(nullptr), parent(nullptr) {}
};

class RBTree
{
private:
    Node *root;
    Node *NIL; // Sentinel node

    void leftRotate(Node *x)
    {
        Node *y = x->right;
        x->right = y->left;
        if (y->left != NIL)
            y->left->parent = x;
        y->parent = x->parent;

        if (x->parent == nullptr)
            root = y;
        else if (x == x->parent->left)
            x->parent->left = y;
        else
            x->parent->right = y;

        y->left = x;
        x->parent = y;
    }

    void rightRotate(Node *x)
    {
        Node *y = x->left;
        x->left = y->right;
        if (y->right != NIL)
            y->right->parent = x;
        y->parent = x->parent;

        if (x->parent == nullptr)
            root = y;
        else if (x == x->parent->right)
            x->parent->right = y;
        else
            x->parent->left = y;

        y->right = x;
        x->parent = y;
    }

    void insertFixup(Node *z)
    {
        while (z->parent != nullptr && z->parent->color == RED)
        {
            if (z->parent == z->parent->parent->left)
            {
                Node *y = z->parent->parent->right;
                if (y != NIL && y->color == RED)
                {
                    z->parent->color = BLACK;
                    y->color = BLACK;
                    z->parent->parent->color = RED;
                    z = z->parent->parent;
                }
                else
                {
                    if (z == z->parent->right)
                    {
                        z = z->parent;
                        leftRotate(z);
                    }
                    z->parent->color = BLACK;
                    z->parent->parent->color = RED;
                    rightRotate(z->parent->parent);
                }
            }
            else
            {
                Node *y = z->parent->parent->left;
                if (y != NIL && y->color == RED)
                {
                    z->parent->color = BLACK;
                    y->color = BLACK;
                    z->parent->parent->color = RED;
                    z = z->parent->parent;
                }
                else
                {
                    if (z == z->parent->left)
                    {
                        z = z->parent;
                        rightRotate(z);
                    }
                    z->parent->color = BLACK;
                    z->parent->parent->color = RED;
                    leftRotate(z->parent->parent);
                }
            }
        }
        root->color = BLACK;
    }

    void transplant(Node *u, Node *v)
    {
        if (u->parent == nullptr)
            root = v;
        else if (u == u->parent->left)
            u->parent->left = v;
        else
            u->parent->right = v;
        if (v != NIL)
            v->parent = u->parent;
    }

    Node *minimum(Node *node)
    {
        while (node->left != NIL)
            node = node->left;
        return node;
    }

    void deleteFixup(Node *x)
    {
        while (x != root && x->color == BLACK)
        {
            if (x == x->parent->left)
            {
                Node *w = x->parent->right;
                if (w->color == RED)
                {
                    w->color = BLACK;
                    x->parent->color = RED;
                    leftRotate(x->parent);
                    w = x->parent->right;
                }
                if ((w->left == NIL || w->left->color == BLACK) &&
                    (w->right == NIL || w->right->color == BLACK))
                {
                    w->color = RED;
                    x = x->parent;
                }
                else
                {
                    if (w->right == NIL || w->right->color == BLACK)
                    {
                        if (w->left != NIL)
                            w->left->color = BLACK;
                        w->color = RED;
                        rightRotate(w);
                        w = x->parent->right;
                    }
                    w->color = x->parent->color;
                    x->parent->color = BLACK;
                    if (w->right != NIL)
                        w->right->color = BLACK;
                    leftRotate(x->parent);
                    x = root;
                }
            }
            else
            {
                Node *w = x->parent->left;
                if (w->color == RED)
                {
                    w->color = BLACK;
                    x->parent->color = RED;
                    rightRotate(x->parent);
                    w = x->parent->left;
                }
                if ((w->right == NIL || w->right->color == BLACK) &&
                    (w->left == NIL || w->left->color == BLACK))
                {
                    w->color = RED;
                    x = x->parent;
                }
                else
                {
                    if (w->left == NIL || w->left->color == BLACK)
                    {
                        if (w->right != NIL)
                            w->right->color = BLACK;
                        w->color = RED;
                        leftRotate(w);
                        w = x->parent->left;
                    }
                    w->color = x->parent->color;
                    x->parent->color = BLACK;
                    if (w->left != NIL)
                        w->left->color = BLACK;
                    rightRotate(x->parent);
                    x = root;
                }
            }
        }
        x->color = BLACK;
    }

    void deleteNode(Node *z)
    {
        if (z == NIL || z == nullptr)
        {
            return;
        }

        Node *x;
        Node *y = z;
        Color yOriginalColor = y->color;

        if (z->left == NIL)
        {
            x = z->right;
            transplant(z, z->right);
        }
        else if (z->right == NIL)
        {
            x = z->left;
            transplant(z, z->left);
        }
        else
        {
            y = minimum(z->right);
            yOriginalColor = y->color;
            x = y->right;
            if (y->parent == z)
            {
                if (x != NIL)
                    x->parent = y;
            }
            else
            {
                transplant(y, y->right);
                y->right = z->right;
                y->right->parent = y;
            }
            transplant(z, y);
            y->left = z->left;
            y->left->parent = y;
            y->color = z->color;
        }

        delete z;

        if (yOriginalColor == BLACK)
            deleteFixup(x);
    }

    void clear(Node *node)
    {
        if (node == NIL)
            return;
        clear(node->left);
        clear(node->right);
        delete node;
    }

public:
    RBTree()
    {
        NIL = new Node("", -1);
        NIL->color = BLACK;
        NIL->left = NIL->right = NIL->parent = NIL;
        root = NIL;
    }

    ~RBTree()
    {
        clear(root);
        delete NIL;
    }

    void insert(string key, int value)
    {
        Node *z = new Node(key, value);
        z->left = z->right = NIL;
        z->parent = nullptr;

        Node *y = nullptr;
        Node *x = root;

        while (x != NIL)
        {
            y = x;
            if (z->key < x->key)
                x = x->left;
            else
                x = x->right;
        }

        z->parent = y;
        if (y == nullptr)
            root = z;
        else if (z->key < y->key)
            y->left = z;
        else
            y->right = z;

        z->color = RED;

        if (z->parent == nullptr)
        {
            z->color = BLACK;
            return;
        }

        if (z->parent->parent == nullptr)
        {
            return;
        }

        insertFixup(z);
    }

    pair<bool, int> search(string key)
    {
        Node *current = root;
        while (current != NIL)
        {
            if (key == current->key)
                return {true, current->value};
            else if (key < current->key)
                current = current->left;
            else
                current = current->right;
        }
        return {false, -1};
    }

    bool remove(string key)
    {
        Node *current = root;
        while (current != NIL && current->key != key)
        {
            if (key < current->key)
                current = current->left;
            else
                current = current->right;
        }
        if (current == NIL)
            return false;
        deleteNode(current);
        return true;
    }

    bool isEmpty()
    {
        return root == NIL;
    }

    Node *findProductNode(const string &key)
    {
        Node *current = root;
        while (current != NIL)
        {
            if (key == current->key)
                return current;
            else if (key < current->key)
                current = current->left;
            else
                current = current->right;
        }
        return NIL;
    }

    bool removeProduct(const string &name, ostream &out)
    {
        Node *node = findProductNode(name);
        if (node == NIL)
            return false;
        deleteNode(node);
        return true;
    }

    Node *getNil() const { return NIL; }
};

#endif