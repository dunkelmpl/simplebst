#define _CRTDBG_MAP_ALLOC  
#include <stdlib.h>  
#include <crtdbg.h> 

#include <iostream>
#include <stdexcept>
#include <sstream>
#include <queue>
#include <vector>

#define PRINT_LABEL_WIDTH (30)

using namespace std;

struct BSTNode {
    BSTNode(int value) : value(value), left(nullptr), right(nullptr) {}

    int value;
    BSTNode *left;
    BSTNode *right;
};

typedef enum {PREORDER, INORDER, POSTORDER} Traversal;

class SimpleBST {
public:
    SimpleBST() : top(nullptr) {}
    SimpleBST(vector<int> input);
    virtual ~SimpleBST();

protected:
    void disposeNode(BSTNode *node);

public:
    BSTNode* searchNode(int value);
    void addNode(int value);
    void deleteNode(int value);

    void BFS(Traversal traversal, const char* label);
    void DFS(const char* label);

protected:
    BSTNode* searchChildNode(BSTNode *parent, int value);
    void addChildNode(BSTNode *parent, int value);
    void deleteChildNode(BSTNode *parent, BSTNode *current, int value);
    BSTNode* findNearestSuccessor(BSTNode* current, BSTNode** parent);

protected:
    void BFSChild(BSTNode* parent, Traversal traversal);

private:
    BSTNode *top;
};

SimpleBST::SimpleBST(vector<int> input) : SimpleBST()
{
    for (auto &item : input) {
        this->addNode(item);
    }
}

SimpleBST::~SimpleBST()
{
    disposeNode(top);
}

void SimpleBST::disposeNode(BSTNode *node)
{
    if (!node) {
        return;
    }

    disposeNode(node->left);
    disposeNode(node->right);

    delete node;
}

BSTNode* SimpleBST::searchNode(int value)
{
    return searchChildNode(top, value);
}

BSTNode* SimpleBST::searchChildNode(BSTNode* parentNode, int value)
{
    if (!parentNode) {
        return nullptr;
    }

    if (parentNode->value == value) {
        return parentNode;
    }

    return (value < parentNode->value)
        ? searchChildNode(parentNode->left, value)
        : searchChildNode(parentNode->right, value);
}

void SimpleBST::addNode(int value)
{
    if (!top) {
        top = new BSTNode(value);

        return;
    }

    addChildNode(top, value);
}

void SimpleBST::addChildNode(BSTNode *parent, int value)
{
    if (!parent) {
        throw invalid_argument("Parent node cannot be null");
    }

    if (value < parent->value) {
        if (parent->left) {
            addChildNode(parent->left, value);
        }
        else {
            parent->left = new BSTNode(value);
        }
    }
    else {
        if (parent->right) {
            addChildNode(parent->right, value);
        }
        else {
            parent->right = new BSTNode(value);
        }
    }
}

void SimpleBST::deleteNode(int value)
{
    deleteChildNode(nullptr, top, value);
}

void SimpleBST::deleteChildNode(BSTNode *parent, BSTNode *current, int value)
{
    if (!current) {
        stringstream s;
        s << "Cannot delete " << value << " node. Node not found.";
        throw out_of_range(s.str());
    }

    if (current->value == value) {
        if (current->left || current->right) { //any child
            if (!current->left || !current->right) { //one child
                BSTNode* child = current->left ? current->left : current->right;

                if (parent) {
                    if (parent->left == current) {
                        parent->left = child;
                    }
                    else {
                        parent->right = child;
                    }
                }
                else {
                    top = child;
                }

                delete current;
            }
            else { //two children
                BSTNode* nearestSuccessorParent = current;
                BSTNode* nearestSuccessor = findNearestSuccessor(current, &nearestSuccessorParent);
                current->value = nearestSuccessor->value;

                if (nearestSuccessorParent->left == nearestSuccessor) {
                    nearestSuccessorParent->left = nullptr;
                }
                else {
                    nearestSuccessorParent->right = nullptr;
                }

                delete nearestSuccessor;
            }
        }
        else { //no children, i.e. leaf
            if (parent) {
                if (parent->left == current) {
                    parent->left = nullptr;
                }
                else {
                    parent->right = nullptr;
                }
            }
            else {
                top = nullptr;
            }

            delete current;
        }
    }
    else {
        deleteChildNode(current, value < current->value ? current->left : current->right, value);
    }
}

BSTNode* SimpleBST::findNearestSuccessor(BSTNode* current, BSTNode** parent)
{
    if (!current || !current->right) {
        throw logic_error("Error: cannot find nearest greater. Invalid current node");
    }

    *parent = current;
    current = current->right;

    while (current->left) {
        *parent = current;
        current = current->left;
    }

    return current;
}

void SimpleBST::BFS(Traversal traversal, const char* label)
{
    cout.width(PRINT_LABEL_WIDTH);
    cout << left << label;

    BFSChild(top, traversal);
    cout << endl;
}

void SimpleBST::BFSChild(BSTNode* parent, Traversal traversal)
{
    if (!parent) {
        return;
    }

    if (traversal == PREORDER) {
        cout << parent->value << ' ';

        BFSChild(parent->left, traversal);
        BFSChild(parent->right, traversal);
    }
    else if (traversal == INORDER) {
        BFSChild(parent->left, traversal);

        cout << parent->value << ' ';

        BFSChild(parent->right, traversal);
    }
    else { //POSTORDER
        BFSChild(parent->left, traversal);
        BFSChild(parent->right, traversal);

        cout << parent->value << ' ';
    }
}

void SimpleBST::DFS(const char* label)
{
    cout.width(PRINT_LABEL_WIDTH);
    cout << left << label;

    if (!top) {
        return;
    }

    queue<BSTNode*> nodesQueue;

    nodesQueue.push(top);

    while (!nodesQueue.empty()) {
        BSTNode* current = nodesQueue.front();
        nodesQueue.pop();

        cout << current->value << ' ';

        if (current->left) {
            nodesQueue.push(current->left);
        }

        if (current->right) {
            nodesQueue.push(current->right);
        }
    }

    cout << endl;
}

int main()
{
    {
        vector<int> orig{ 479, 359, 465, 963, 706, 300, 320 };
        cout.width(PRINT_LABEL_WIDTH);
        cout << left << "Original input: ";
        for (auto& num : orig) {
            cout << num << ' ';
        }
        cout << "\n\n";

        SimpleBST bst(orig);

        auto& search = [&bst]() {
            vector<int> searching{ 465, 200, 320, 963, 11 };
            for (auto& item : searching) {
                stringstream s;
                s << "Searching " << item << ": ";

                cout.width(PRINT_LABEL_WIDTH);
                cout << left << s.str();

                BSTNode* node = bst.searchNode(item);
                cout << (node != nullptr ? "found" : "not found") << endl;
            }
            cout << endl;
        };

        auto& printTraversals = [&bst]() {
            bst.BFS(INORDER, "BFS Inorder Traversal: ");
            bst.BFS(PREORDER, "BFS Preorder Traversal: ");
            bst.BFS(POSTORDER, "BFS Postorder Traversal: ");
            bst.DFS("DFS Preorder Traversal: ");

            cout << endl;
        };

        printTraversals();

        search();

        auto& deleteNodes = [&bst]() {
            vector<int> deleting{ 359, 320, 479, 963 };
            for (auto& item : deleting) {

                cout << "Deleting " << item << endl;
                bst.deleteNode(item);
            }
            cout << endl;
        };

        deleteNodes();

        printTraversals();

        search();
    }

    _CrtDumpMemoryLeaks();

    return 0;
}
