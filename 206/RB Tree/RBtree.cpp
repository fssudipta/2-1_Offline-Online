// first things first: RBtree properties:
// 1. every node is either red or black
// 2. the root is always black
// 3. no two red nodes appear porpor(red nodes er red child thakbe na)
// 4. every path from a given node to any of its descendant leaves contains same number of black nodes
// 5. all leaves (NIL) are black --> sentinel niye kaj korbo
#include <bits/stdc++.h>
#include "color.hpp"
using namespace std;
using namespace hue;

enum Color
{
    RED,
    BLACK
};

struct Product
{
    int id;
    string name;
    int stock;
    Product(int id, string name, int stock) : id(id), name(name), stock(stock) {}
};

struct Node
{
    Product product;
    Color color;
    Node *left, *right, *parent;
    Node(Product p) : product(p), color(RED), left(nullptr), right(nullptr), parent(nullptr) {}
};

class RBtree
{
private:
    Node *root;
    Node *NIL; // sentinel na nile hocche na

    // purpose: x er right child y, x er parent hobe
    //  x er rc er subtree thik korbo, jate y er lc x hoy
    void rotateLeft(Node *x)
    {
        Node *y = x->right;
        x->right = y->left;

        if (y->left != NIL)
            y->left->parent = x;

        y->parent = x->parent;

        if (x->parent == NIL)
            root = y;
        else if (x == x->parent->left)
            x->parent->left = y;
        else
            x->parent->right = y;

        y->left = x;
        x->parent = y;
    }

    // purpose: y er lc-x, y er parent hobe
    // ar x theke je subtree, ota fix kore x er rc hobe y
    void rotateRight(Node *y)
    {
        Node *x = y->left;
        y->left = x->right;

        if (x->right != NIL)
            x->right->parent = y;

        x->parent = y->parent;

        if (y->parent == NIL)
            root = x;
        else if (y == y->parent->right)
            y->parent->right = x;
        else
            y->parent->left = x;

        x->right = y;
        y->parent = x;
    }

    void insertionFix(Node *z)
    {
        // property 3 dekhtesi, red-red hoy kina
        while (z->parent != NIL && z->parent->color == RED)
        {
            // case A: parent hoilo grandparent er lc
            if (z->parent == z->parent->parent->left)
            {
                Node *y = z->parent->parent->right; // uncle
                // case 1: uncle-->red
                if (y != NIL && y->color == RED)
                {
                    z->parent->color = BLACK;
                    y->color = BLACK;
                    // parent ar uncle ke black
                    // gp ke red
                    z->parent->parent->color = RED;
                    z = z->parent->parent; // z ke gp te uthalam
                }
                // case 2,3: uncle-->black
                else
                {
                    // subcase 2: z hoilo rc-->convert to ll case
                    if (z == z->parent->right)
                    {
                        z = z->parent;
                        rotateLeft(z);
                    }
                    // subcase 3: parent kalo, gp tai red
                    z->parent->color = BLACK;
                    z->parent->parent->color = RED;
                    rotateRight(z->parent->parent); // gp ke color fix korte ekta rotation
                }
            }
            // case B: parent hoilo gp er rc
            else
            {
                Node *y = z->parent->parent->left; // uncle
                // case 1: uncle-->red
                if (y != NIL && y->color == RED)
                {
                    z->parent->color = BLACK;
                    y->color = BLACK;
                    z->parent->parent->color = RED;
                    z = z->parent->parent;
                }
                // case 2,3: uncle-->black
                else
                {
                    // subcase 2: z-lc --> convert to rr case
                    if (z == z->parent->left)
                    {
                        z = z->parent;
                        rotateRight(z);
                    }
                    // fix the rr case
                    z->parent->color = BLACK;
                    z->parent->parent->color = RED;
                    rotateLeft(z->parent->parent);
                }
            }
        }
        // aage jai kori na keno, root black
        root->color = BLACK;
    }

    void fixDelete(Node *x)
    {
        // double-black situation: property 4 violate kore
        while (x != root && x->color == BLACK)
        {

            if (x == x->parent->left)
            {
                Node *w = x->parent->right; // w-->sibling
                // case 1: sibling->red
                if (w->color == RED)
                {
                    w->color = BLACK;
                    x->parent->color = RED;
                    rotateLeft(x->parent);
                    w = x->parent->right;
                }
                // case 2: sibling ar or dui bacchai black
                if ((w->left == NIL || w->left->color == BLACK) &&
                    (w->right == NIL || w->right->color == BLACK))
                {
                    w->color = RED;
                    x = x->parent;
                }
                // case 3: sibling's rc is black
                else
                {
                    // subcase 1: ekbar right rotation subcase 2 te nite
                    if (w->right == NIL || w->right->color == BLACK)
                    {
                        if (w->left != NIL)
                            w->left->color = BLACK;
                        w->color = RED;
                        rotateRight(w);
                        w = x->parent->right;
                    }
                    // subcase 2: sibling parent same color+parent black+sibling er rc black+parent ke left rotation
                    w->color = x->parent->color;
                    x->parent->color = BLACK;
                    if (w->right != NIL)
                        w->right->color = BLACK;
                    rotateLeft(x->parent);
                    x = root;
                }
            }
            // same jinish, just image kora
            else
            {
                Node *w = x->parent->left;

                if (w->color == RED)
                {
                    w->color = BLACK;
                    x->parent->color = RED;
                    rotateRight(x->parent);
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
                        rotateLeft(w);
                        w = x->parent->left;
                    }

                    w->color = x->parent->color;
                    x->parent->color = BLACK;
                    if (w->left != NIL)
                        w->left->color = BLACK;
                    rotateRight(x->parent);
                    x = root;
                }
            }
        }
        // root black
        x->color = BLACK;
    }

    void inorder(Node *node, ostream &out) const
    {
        if (node == NIL) // base case
            return;

        inorder(node->left, out);

        if (&out == &cout)
        {
            if (node->color == RED)
                out << red;
            else
                out << black;
        }

        out << node->product.id << " => " << node->product.name << " (" << node->product.stock << ")" << endl;

        if (&out == &cout)
            out << reset;

        inorder(node->right, out);
    }

    void printTree(Node *node, ostream &out) const
    {
        if (node == NIL)
            return;

        if (node->color == RED)
            out << red;
        else
            out << black;

        out << node->product.id << "_" << node->product.name << reset;

        if (node->left != NIL || node->right != NIL)
        {
            out << "(";

            if (node->left != NIL)
                printTree(node->left, out);
            else
                out << "";

            out << ",";

            if (node->right != NIL)
                printTree(node->right, out);
            else
                out << "";

            out << ")";
        }
    }

    void clear(Node *node)
    {
        if (node == NIL)
            return;

        clear(node->left);
        clear(node->right);
        delete node;
    }

    // color property dekhtesi na. just dekhbo u er subtree v te replace korar way ta
    void transplant(Node *u, Node *v)
    {
        if (u->parent == NIL)
            root = v;
        else if (u == u->parent->left)
            u->parent->left = v;
        else
            u->parent->right = v;

        v->parent = u->parent;
    }

    Node *minimum(Node *node)
    {
        while (node->left != NIL)
        {
            node = node->left;
        }
        return node;
    }

    Node *maximum(Node *node)
    {
        while (node->right != NIL)
        {
            node = node->right;
        }
        return node;
    }

    void deleteNode(Node *z, ostream &out)
    {
        Node *y = z; // z ke muchbo, y hoilo temp
        Color y_original_color = y->color;
        Node *x;
        // case 1: node er left child i nai
        if (z->left == NIL)
        {
            x = z->right;
            transplant(z, z->right); // replace z with its right child
        }
        // case 2: node er right child i nai
        else if (z->right == NIL)
        {
            x = z->left;
            transplant(z, z->left); // replace z with its left child
        }
        // case 3: node er duita baccha ase
        else
        {
            // y = minimum(z->right); // min in right subtree
            y = maximum(z->left);
            y_original_color = y->color; // porer fixup er jonno lagbe
            x = y->right;

            if (y->parent == z)
            {
                x->parent = y;
            }
            // jodi y, z er direct child na hoy
            else
            {
                // transplant(y, y->right); // y ke current position theke shorabo
                transplant(y, y->left);
                // y->right = z->right;
                y->left = z->left;
                // y->right->parent = y; // make z's right child y's right child
                if (y->left != NIL)
                    y->left->parent = y;
            }

            transplant(z, y); // replace z with y
            // y->left = z->left; // attach z's left child to y
            // y->left->parent = y;
            y->right = z->right;
            if (y->right != NIL)
                y->right->parent = y;
            y->color = z->color; // inherit z's color
        }

        delete z; // jeta niye kaj ota dilam muchhe
        // tree balance korar dorkar hoile fix
        if (y_original_color == BLACK)
            fixDelete(x);
        if (root != NIL)
        {
            printTree(root, cout);
            cout << endl;
            printTree(root, out);
            out << endl;
        }
        // tree empty hoile?
        else
        {
            cout << "" << endl;
            out << "" << endl;
        }
    }

public:
    RBtree()
    {
        NIL = new Node(Product(-1, "", 0));
        NIL->color = BLACK;
        NIL->left = NIL->right = NIL->parent = NIL;
        root = NIL;
    }

    ~RBtree()
    {
        if (root != NIL)
        {
            clear(root);
        }
        delete NIL;
    }

    // Inventory system

    void addItem(int id, const string &name, int stock, ostream &out)
    {
        Node *node = root;
        Node *parent = NIL;
        // kothay add korbo khujtesi
        while (node != NIL)
        {
            if (id == node->product.id)
            {
                // product id millei naam update
                node->product.name = name;
                node->product.stock = stock;

                printTree(root, cout);
                cout << endl;
                printTree(root, out);
                out << endl;
                return; // dorkar nai new node er
            }
            // move down the tree
            parent = node;
            node = (id < node->product.id) ? node->left : node->right;
        }
        // accha new node lagbe ebar
        Node *newNode = new Node(Product(id, name, stock));
        newNode->parent = parent; // parent set korlam
        // children ke sentinel hishebe initialize kore fellam
        newNode->left = newNode->right = NIL;
        // case: root nai so far
        if (parent == NIL)
            root = newNode;
        else if (id < parent->product.id)
            parent->left = newNode; // insert as left child
        else                        // insert as right child
            parent->right = newNode;
        // fix any violations
        insertionFix(newNode);

        printTree(root, cout);
        cout << endl;
        printTree(root, out);
        out << endl;
    }

    bool buyItem(int id, int qty, ostream &out)
    {
        Node *node = root;

        while (node != NIL && node->product.id != id)
        {
            node = (id < node->product.id) ? node->left : node->right;
        }

        if (node == NIL)
            return false;
        // qt thik korlam
        node->product.stock -= qty;

        if (node->product.stock <= 0)
        {
            deleteNode(node, out);
            return true;
        }

        printTree(root, cout);
        cout << endl;
        printTree(root, out);
        out << endl;

        return true;
    }

    Node *find(int id) const
    {
        Node *node = root;
        // normal searching
        while (node != NIL)
        {
            if (id == node->product.id)
                return node;

            node = (id < node->product.id) ? node->left : node->right;
        }

        return nullptr;
    }

    void clearFinal(ostream &out)
    {
        if (root == NIL)
        {
            cout << "unsuccessful" << endl;
            out << "unsuccessful" << endl;
            return;
        }
        clear(root);
        root = NIL;

        cout << "successful" << endl;
        out << "successful" << endl;
    }

    void listInventory(ostream &out) const
    {
        inorder(root, out);
    }

    bool isEmpty() const
    {
        return root == NIL;
    }

    int count(Node *node) const
    {
        if (node == NIL)
            return 0;
        return 1 + count(node->left) + count(node->right);
    }

    int size()
    {
        return count(root);
    }
};

int main()
{
    ifstream fin("input.txt");
    ofstream fout("output.txt");
    RBtree inventory;
    string line;

    while (getline(fin, line))
    {
        if (line.empty())
            continue;

        stringstream ss(line);
        string cmd;
        ss >> cmd;

        if (cmd == "Clr")
        {
            inventory.clearFinal(fout);
        }
        else if (cmd == "Em")
        {
            fout << (inventory.isEmpty() ? "yes" : "no") << endl;
            cout << (inventory.isEmpty() ? "yes" : "no") << endl;
        }
        else if (cmd == "S")
        {
            fout << inventory.size() << endl;
            cout << inventory.size() << endl;
        }
        else if (cmd == "Itr")
        {
            inventory.listInventory(fout);
            inventory.listInventory(cout);
        }
        else if (cmd == "AI")
        {
            int id, stock;
            string name;
            ss >> id;
            ss >> ws;

            if (ss.peek() == '"')
            {
                ss.get();
                getline(ss, name, '"');
            }
            else
            {
                ss >> name;
            }

            ss >> stock;
            inventory.addItem(id, name, stock, fout);
        }
        else if (cmd == "BI")
        {
            int id, qt;
            ss >> id >> qt;

            if (!inventory.buyItem(id, qt, fout))
            {
                fout << "Not Available" << endl;
                cout << "Not Available" << endl;
            }
        }
        else if (cmd == "CI")
        {
            int id;
            ss >> id;
            Node *n = inventory.find(id);

            if (n == nullptr)
            {
                fout << "Not Available" << endl;
                cout << "Not Available" << endl;
            }
            else
            {
                fout << "Stock left: " << n->product.stock << endl;
                cout << "Stock left: " << n->product.stock << endl;
            }
        }
    }

    return 0;
}