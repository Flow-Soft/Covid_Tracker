// C++ program for B-Tree insertion
#include<iostream>
#include<bits/stdc++.h>
using namespace std;

// This data structure will stores the data of a state at a perticular date
struct data{
    string state;   //name of the state
    string date;    //date
    int cases;      // number of cases on that date
    int deaths;     // number of deaths on that date
};

vector<data> result;    //this vector will stores the result
int resultNumber;       //Number of results to be stored

// Compare two datas of the state in order of number of cases -> number of deaths -> date
bool Compare(data &A, data &B){
    if(A.cases != B.cases) return A.cases > B.cases;
    if(A.deaths != B.deaths) return A.deaths > B.deaths;
    if(A.date.substr(6, 4) != B.date.substr(6, 4)) return A.date.substr(6, 4) > B.date.substr(6, 4);
    if(A.date.substr(3, 2) != B.date.substr(3, 2)) return A.date.substr(3, 2) > B.date.substr(3, 2);
    if(A.date.substr(0, 2) != B.date.substr(0, 2)) return A.date.substr(0, 2) > B.date.substr(0, 2);
    return true;
}

bool CompareDate(data &A, string&B){
    if(A.date.substr(6, 4) != B.substr(6, 4)) return A.date.substr(6, 4) > B.substr(6, 4);
    if(A.date.substr(3, 2) != B.substr(3, 2)) return A.date.substr(3, 2) > B.substr(3, 2);
    if(A.date.substr(0, 2) != B.substr(0, 2)) return A.date.substr(0, 2) > B.substr(0, 2);
    return true;
}

//this function will check if the date matches or not
bool areEqual(data &A, string&B){
    if(A.date != B) return false;
    return true;
}

// A BTree node
class BTreeNode
{
public:
    data *keys; // An array of keys
    int t;   // Minimum degree (defines the range for number of keys)
    BTreeNode **C; // An array of child pointers
    int n;   // Current number of keys
    bool leaf; // Is true when node is leaf. Otherwise false
public:
    BTreeNode(int _t, bool _leaf); // Constructor

    // A utility function to insert a new key in the subtree rooted with
    // this node. The assumption is, the node must be non-full when this
    // function is called
    void insertNonFull(data k);

    // A utility function to split the child y of this node. i is index of y in
    // child array C[]. The Child y must be full when this function is called
    void splitChild(int i, BTreeNode *y);

    // A function to traverse all nodes in a subtree rooted with this node
    void traverse();

    // A function to traverse all nodes in a subtree rooted with this node and stores it
    void findTopSatesByDeaths();

    // A function to store Top Cases in all the states
    void findTopStatesByCases();

    // A function to search a key in the subtree rooted with this node.
    void search(string k); // returns NULL if k is not present.

// Make BTree friend of this so that we can access private members of this
// class in BTree functions
friend class BTree;
};

// A BTree
class BTree
{
    BTreeNode *root; // Pointer to root node
    int t; // Minimum degree
public:
    // Constructor (Initializes tree as empty)
    BTree(int _t)
    { root = NULL; t = _t; }

    // function to traverse the tree
    void traverse()
    { if (root != NULL) root->traverse(); }

    void findTopSatesByDeaths()
    { if (root != NULL) root->findTopSatesByDeaths(); }


    void findTopStatesByCases()
    { if (root != NULL) root->findTopStatesByCases(); }

    // function to search a key in this tree
    void search(string k)
    { if (root != NULL) root->search(k); }

    // The main function that inserts a new key in this B-Tree
    void insert(data k);
};

// Constructor for BTreeNode class
BTreeNode::BTreeNode(int t1, bool leaf1)
{
    // Copy the given minimum degree and leaf property
    t = t1;
    leaf = leaf1;

    // Allocate memory for maximum number of possible keys
    // and child pointers
    keys = new data[2*t-1];
    C = new BTreeNode *[2*t];

    // Initialize the number of keys as 0
    n = 0;
}

// Function to traverse all nodes in a subtree rooted with this node
void BTreeNode::traverse()
{
    // There are n keys and n+1 children, traverse through n keys
    // and first n children
    int i;
    for (i = 0; i < n; i++)
    {
        // If this is not leaf, then before printing key[i],
        // traverse the subtree rooted with child C[i].
        if (leaf == false)
            C[i]->traverse();
        if(keys != NULL)
        cout << " " << keys[i].state<<" "<<keys[i].date<<" "<<keys[i].cases<<" "<<keys[i].deaths<<endl;
    }

    // Print the subtree rooted with last child
    if (leaf == false)
        C[i]->traverse();
}

void BTreeNode::findTopStatesByCases(){

    if(result.size() >= resultNumber) return;
    int i;
    for (i = 0; i < n; i++)
    {
        // If this is not leaf, then before printing key[i],
        // traverse the subtree rooted with child C[i].
        if (leaf == false)
            C[i]->findTopStatesByCases();
        if(keys != NULL){
            if(result.size() >= resultNumber) return;   // in case of overflow return
            result.push_back(keys[i]);
        }
    }

    // store the subtree rooted with last child
    if (leaf == false)
        C[i]->findTopStatesByCases();
    }

    void BTreeNode::findTopSatesByDeaths(){
        int i;
    for (i = 0; i < n; i++)
    {
        // If this is not leaf, then before printing key[i],
        // traverse the subtree rooted with child C[i].
            if (leaf == false)
                C[i]->findTopSatesByDeaths();
            if(keys != NULL)
            result.push_back(keys[i]);
        }

        // store the subtree rooted with last child
        if (leaf == false)
            C[i]->findTopSatesByDeaths();
    }

// Function to search key k in subtree rooted with this node
void BTreeNode::search(string Date)
{

    int i;
    for (i = 0; i < n; i++)
    {
        // If this is not leaf, then compare key[i].date and Date,
        // traverse the subtree rooted with child C[i].
        if (leaf == false)
            C[i]->search(Date);
        if(keys != NULL)
        if (areEqual(keys[i] , Date)){
                result.push_back(keys[i]);
        }
    }

    // search in the subtree rooted with last child
    if (leaf == false)
        C[i]->search(Date);
}

// The main function that inserts a new key in this B-Tree
void BTree::insert(data Data)
{
    // If tree is empty
    if (root == NULL)
    {
        // Allocate memory for root
        root = new BTreeNode(t, true);
        root->keys[0] = Data; // Insert key
        root->n = 1; // Update number of keys in root
    }
    else // If tree is not empty
    {
        // If root is full, then tree grows in height
        if (root->n == 2*t-1)
        {
            // Allocate memory for new root
            BTreeNode *s = new BTreeNode(t, false);

            // Make old root as child of new root
            s->C[0] = root;

            // Split the old root and move 1 key to the new root
            s->splitChild(0, root);

            // New root has two children now. Decide which of the
            // two children is going to have new key
            int i = 0;
            if(Compare(s->keys[0], Data))
                i++;
            s->C[i]->insertNonFull(Data);

            // Change root
            root = s;
        }
        else // If root is not full, call insertNonFull for root
            root->insertNonFull(Data);
    }
}

// A utility function to insert a new key in this node
// The assumption is, the node must be non-full when this
// function is called
void BTreeNode::insertNonFull(data Data)
{
    // Initialize index as index of rightmost element
    int i = n-1;

    // If this is a leaf node
    if (leaf == true)
    {
        // The following loop does two things
        // a) Finds the location of new key to be inserted
        // b) Moves all greater keys to one place ahead
        while (i >= 0 && Compare(Data, keys[i]))
        {
            keys[i+1] = keys[i];
            i--;
        }

        // Insert the new key at found location
        keys[i+1] = Data;
        n = n+1;
    }
    else // If this node is not leaf
    {
        // Find the child which is going to have the new key
        while (i >= 0 && Compare(Data, keys[i]))
            i--;

        // See if the found child is full
        if (C[i+1]->n == 2*t-1)
        {
            // If the child is full, then split it
            splitChild(i+1, C[i+1]);

            // After split, the middle key of C[i] goes up and
            // C[i] is splitted into two. See which of the two
            // is going to have the new key
            if (Compare(keys[i+1] , Data))
                i++;
        }
        C[i+1]->insertNonFull(Data);
    }
}

// A utility function to split the child y of this node
// Note that y must be full when this function is called
void BTreeNode::splitChild(int i, BTreeNode *y)
{
    // Create a new node which is going to store (t-1) keys
    // of y
    BTreeNode *z = new BTreeNode(y->t, y->leaf);
    z->n = t - 1;

    // Copy the last (t-1) keys of y to z
    for (int j = 0; j < t-1; j++)
        z->keys[j] = y->keys[j+t];

    // Copy the last t children of y to z
    if (y->leaf == false)
    {
        for (int j = 0; j < t; j++)
            z->C[j] = y->C[j+t];
    }

    // Reduce the number of keys in y
    y->n = t - 1;

    // Since this node is going to have a new child,
    // create space of new child
    for (int j = n; j >= i+1; j--)
        C[j+1] = C[j];

    // Link the new child to this node
    C[i+1] = z;

    // A key of y will move to this node. Find the location of
    // new key and move all greater keys one space ahead
    for (int j = n-1; j >= i; j--)
        keys[j+1] = keys[j];

    // Copy the middle key of y to this node
    keys[i] = y->keys[t-1];

    // Increment count of keys in this node
    n = n + 1;
}

BTree t(3); // A B-Tree with minium degree 3
data Datax = data();

void InsertBTreeElements(){
    // for(each element in Csv File) insert element in Btree
    
    //data 1;
    Datax.state = "State1";
    Datax.date = "01-01-2020";
    Datax.cases = 20;
    Datax.deaths = 10;
    t.insert(Datax);

    //data 2;
    Datax.state = "State2";
    Datax.date = "02-01-2020";
    Datax.cases = 30;
    Datax.deaths = 99;
    t.insert(Datax);

    //data 3;
    Datax.state = "State3";
    Datax.date = "01-11-2020";
    Datax.cases = 40;
    Datax.deaths = 2;
    t.insert(Datax);

    //data 4;
    Datax.state = "State4";
    Datax.date = "01-01-2020";
    Datax.cases = 500;
    Datax.deaths = 10;
    t.insert(Datax);

    //data 5;
    Datax.state = "State5";
    Datax.date = "01-01-2021";
    Datax.cases = 60;
    Datax.deaths = 145;
    t.insert(Datax);

    //data 6;
    Datax.state = "State6";
    Datax.date = "01-01-2020";
    Datax.cases = 70;
    Datax.deaths = 70;
    t.insert(Datax);

    //data 7;
    Datax.state = "State7";
    Datax.date = "01-01-2020";
    Datax.cases = 80;
    Datax.deaths = 200;
    t.insert(Datax);

    //data 8;
    Datax.state = "State8";
    Datax.date = "01-01-2020";
    Datax.cases = 88;
    Datax.deaths = 100;
    t.insert(Datax);

    //data 9;
    Datax.state = "State9";
    Datax.date = "01-01-2020";
    Datax.cases = 200;
    Datax.deaths = 10;
    t.insert(Datax);
}

// Driver program to test above functions
int main()
{
    
    InsertBTreeElements();  //this function will input the Data into BTree

    //Traversing the Tree 
    cout << "Traversal of the constucted tree is \n";
    t.traverse();

    resultNumber = 5;   //this variable will stores the number of results you want
    //put this to INT_MAX for getting all results
    result.clear();

    //this function will find the top cases in the states
    t.findTopStatesByCases();
    cout<<"\nTop States with higher number of cases\n";
    for(data x: result) cout << x.state<<" "<<x.date<<" "<<x.cases<<" "<<x.deaths<<endl;
    
    result.clear();

    //this function will find top deaths in the states
    t.findTopSatesByDeaths();
    sort(result.begin(), result.end(), [](data&A, data&B){
        return A.deaths > B.deaths;
    });

    cout<<"\nTop States with higher number of deaths\n";
    for(int i=0;i<min(resultNumber, (int)result.size());i++) cout << result[i].state<<" "<<result[i].date<<" "<<result[i].cases<<" "<<result[i].deaths<<endl;

    result.clear();
    t.search("01-01-2020");

    sort(result.begin(), result.end(), [](data&A, data&B){
        return A.deaths > B.deaths; // in case you want result on the bases of number of deaths
        // return A.cases > B.cases;    // in case you want result on the bases of number of cases
    });
    
    cout<<"\nTop States with higher number of deaths on date 01-01-2020\n";
    for(int i=0;i<min(resultNumber, (int)result.size());i++) cout << result[i].state<<" "<<result[i].date<<" "<<result[i].cases<<" "<<result[i].deaths<<endl;


    return 0;
}