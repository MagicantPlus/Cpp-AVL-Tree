#include <iostream>
#include <fstream>
#include <climits>
#include <vector>
#include <list>
#include <forward_list>
#include <cmath>
#include <algorithm>
#include <stack>
#include <queue>
#include <utility>

// #define cin fin
// #define cout fout

using namespace std;

ifstream fin("data.in");
ofstream fout("data.out");

class AVLTree
{
private:
    struct node;
    struct pairNodepRow;

    node makeNode(int val, int height, node* left, node* right);
    pairNodepRow makePairNodepRow(node* nodep, int row);

    node* createNode(int val);
    node* insert(node* nodep, int val, bool& inseration);
    void erase(node* nodep, int val, int& phase, bool& deletion, bool& deleteDirection, node*& deleteNode);
    void print(node& node_, int row, int& lastRow);
    void rotateRight(node& x, node& father);
    void rotateLeft(node& x, node& father);
    void balanceNode(node& node_, node& father);
    int balance(node& node_);
    void updateNode(node& node_);
public:

    node* root;

    AVLTree();
    node* insert(int val);
    void erase(int val);
    node* find(int val);
    node* find(node& node_, int val);
    node* find(node& node_, int val, node*& father);
    void print();
};

int main()
{
    AVLTree tree;

    int type;

    while(cin >> type)
    {
        int val;

        if(type == -1)
        {
            return 0;
        }
        else if(type == 0)
        {
            tree.print();
        }
        else if(type == 1)
        {
            cin >> val;

            tree.insert(val);
        }
        else if(type == 2)
        {
            cin >> val;

            tree.erase(val);
        }
    }

    return 0;
}
struct AVLTree::node
{
    int val, height;
    node *left, *right;

    void operator=(node& x)
    {
        this->val = x.val;
        this->height = x.height;
        this->left = x.left;
        this->right = x.right;
    }
};
struct AVLTree::pairNodepRow
{
    node* nodep;
    int row;
};
AVLTree::node AVLTree::makeNode(int val, int height, node* left, node* right)
{
    node temp;
    temp.val = val;
    temp.height = height;
    temp.left = left;
    temp.right = right;

    return temp;
}
AVLTree::pairNodepRow AVLTree::makePairNodepRow(node* nodep, int row)
{
    pairNodepRow temp;
    temp.nodep = nodep;
    temp.row = row;

    return temp;
}
AVLTree::node* AVLTree::createNode(int val)
{
    node* temp = new node;
    temp->val = val;
    temp->height = 0;
    temp->left = NULL;
    temp->right = NULL;

    return temp;
}
AVLTree::node* AVLTree::insert(node* nodep, int val, bool& inseration)
{
    if(nodep == NULL)
    {    
        inseration = true;
        return createNode(val);
    }

    inseration = false;

    bool inseration_;
    node* returnVal;

    if(val == nodep->val)
    {
        return nodep;
    }
    else if(val < nodep->val)
    {
        returnVal = this->insert(nodep->left, val, inseration_);

        if(inseration_)
        {
            nodep->left = returnVal;
        }

        this->updateNode(*nodep);
        this->balanceNode(*nodep->left, *nodep);
    }
    else
    {
        returnVal = insert(nodep->right, val, inseration_);

        if(inseration_)
        {
            nodep->right = returnVal;
        }

        this->updateNode(*nodep);
        this->balanceNode(*nodep->right, *nodep);
    }
    
    return returnVal;
}
void AVLTree::erase(node* nodep, int val, int& phase, bool& deletion, bool& deleteDirection, node*& deleteNode)
{
    bool deletion_ = false;
    int nextNode = 0;

    if(nodep == NULL)
        return;

    if(nodep->val == val && phase == 0)
    {
        phase = 1;

        deleteNode = nodep;

        nextNode = 2;

        if(nodep->right == NULL)
        {
            deletion = true;
            phase = 2;

            return;
        }
    }

    if(phase == 0)
    {
        if(val < nodep->val)
        {
            if(nodep->left != NULL)
            {
                nextNode = 1;
            }
        }
        else
        {
            if(nodep->right != NULL)
            {
                nextNode = 2;
            }
        }
    }
    else if(phase == 1 && nodep != deleteNode)
    {
        if(nodep->left == NULL)
        {
            deletion = true;
            phase = 2;

            return;
        }
        else
        {
            nextNode = 1;
        }
    }

    if(nextNode == 1)
        erase(nodep->left, val, phase, deletion_, deleteDirection, deleteNode);
    else if(nextNode == 2)
        erase(nodep->right, val, phase, deletion_, deleteDirection, deleteNode);

    if(deletion_)
    {   
        node* deleteMemory;

        if(nextNode == 1)
        {   
            deleteMemory = nodep->left;

            if(nodep->left == deleteNode)
            {
                nodep->left = deleteNode->left;
            }
            else
            {
                deleteNode->val = nodep->left->val;
                nodep->left = nodep->left->right;
            }
        }
        else if(nextNode == 2)
        {
            deleteMemory = nodep->right;

            if(nodep->right == deleteNode)
            {
                nodep->right = deleteNode->left;
            }
            else if(nodep == deleteNode)
            {
                deleteNode->val = nodep->right->val;
                nodep->right = nodep->right->right;
            }
        }

        delete deleteMemory;
    }

    updateNode(*nodep);

    if(!deletion_)
    {
        if(nextNode == 1)
            balanceNode(*nodep->left, *nodep);
        else if(nextNode == 2)
            balanceNode(*nodep->right, *nodep);
    }
}
void AVLTree::print(node& node_, int row, int& lastRow)
{
    if(row != lastRow)
    {
        cout << '\n';
        lastRow = row;
    }

    cout << node_.val << ' ';

    if(node_.left != NULL)
        this->print(*node_.left, row + 1, lastRow);

    if(node_.right != NULL)
        this->print(*node_.right, row + 1, lastRow);
}
void AVLTree::rotateRight(node& x, node& father)
{
    node& y = *x.left;

    if(&x == this->root)
    {
        this->root = &y;
    }
    else
    {
        if(father.left == &x)
        {
            father.left = &y;
        }
        else
        {
            father.right = &y;
        }
    }

    x.left = y.right;
    y.right = &x;

    this->updateNode(x);
    this->updateNode(y);
    if(&x != this->root)
        this->updateNode(father);
}
void AVLTree::rotateLeft(node& x, node& father)
{
    node& y = *x.right;

    if(&x == this->root)
    {
        this->root = &y;
    }
    else
    {
        if(father.left == &x)
        {
            father.left = &y;
        }
        else
        {
            father.right = &y;
        }
    }

    x.right = y.left;
    y.left = &x;

    this->updateNode(x);
    this->updateNode(y);
    if(&x != this->root)
        this->updateNode(father);
}
void AVLTree::balanceNode(node& node_, node& father)
{
    if(balance(node_) > 1)
    {
        if(balance(*node_.left) > 0)
        {
            rotateRight(node_, father);
        }
        else
        {
            rotateLeft(*node_.left, node_);
            rotateRight(node_, father);
        }
    }
    else if(balance(node_) < -1)
    {
        if(balance(*node_.right) > 0)
        {
            rotateRight(*node_.right, node_);
            rotateLeft(node_, father);
        }
        else
        {
            rotateLeft(node_, father);
        }
    }
}
int AVLTree::balance(node& node_)
{
    int left = -1, right = -1;
    
    if(node_.left != NULL)
        left = node_.left->height;
    
    if(node_.right != NULL)
        right = node_.right->height;

    return left - right;
}
void AVLTree::updateNode(node& node_)
{
    int maxHeight = -1;

    if(node_.left != NULL)
        maxHeight = max(maxHeight, node_.left->height);

    if(node_.right != NULL)
        maxHeight = max(maxHeight, node_.right->height);

    node_.height = maxHeight + 1;
}
AVLTree::AVLTree()
{
    this->root = NULL;
}
AVLTree::node* AVLTree::insert(int val)
{
    node* returnVal;
    bool inseration;

    returnVal = this->insert(this->root, val, inseration);

    if(inseration)
    {
        this->root = returnVal;
    }

    this->balanceNode(*this->root, *this->root);

    return returnVal;
}
void AVLTree::erase(int val)
{
    int phase = 0;
    bool deletion = false, deleteDirection;
    node* deleteNode = NULL;

    erase(this->root, val, phase, deletion, deleteDirection, deleteNode);

    if(deletion)
    {
        this->root = NULL;
    }
    else
    {
        updateNode(*this->root);
        balanceNode(*this->root, *this->root);
    }
}
AVLTree::node* AVLTree::find(int val)
{
    if(this->root == NULL)
        return NULL;

    return find(*this->root, val);
}
AVLTree::node* AVLTree::find(node& node_, int val)
{
    if(node_.val == val)
    {   
        return &node_;
    }
    
    node* returnVal = NULL;
    
    if(val < node_.val && node_.left != NULL)
        returnVal = find(*node_.left, val);
    else if(val > node_.val && node_.right != NULL)
        returnVal = find(*node_.right, val);

    return returnVal;
}
AVLTree::node* AVLTree::find(node& node_, int val, node*& father)
{
    if(node_.val == val)
    {   
        return &node_;
    }
    
    node* returnVal = NULL;
    
    if(val < node_.val && node_.left != NULL)
        returnVal = find(*node_.left, val, father);
    else if(val > node_.val && node_.right != NULL)
        returnVal = find(*node_.right, val, father);

    if(returnVal != NULL && father == NULL)
    {
        father = &node_;
    }

    return returnVal;
}
void AVLTree::print()
{
    int lastRow = 0;
    queue<pairNodepRow> next;
    
    if(this->root == NULL)
        return;
    
    next.push(makePairNodepRow(this->root, 0));

    while(!next.empty())
    {   
        if(next.front().row != lastRow)
        {
            cout << '\n';
            lastRow = next.front().row;
        }

        cout << next.front().nodep->val << ' ';

        if(next.front().nodep->left != NULL)
            next.push(makePairNodepRow(next.front().nodep->left, next.front().row + 1));

        if(next.front().nodep->right != NULL)
            next.push(makePairNodepRow(next.front().nodep->right, next.front().row + 1));

        next.pop();
    }

    cout << '\n';
}