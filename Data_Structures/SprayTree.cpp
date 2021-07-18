#include <bits/stdc++.h>
using namespace std;

// An AVL tree node
class node
{
    public:
    int cases;
    int deaths;
    string state;
    string date;
    node *left, *right;
};

vector<node> result;    //this vector will stores the result
int resultNumber;       //Number of results to be stored

//comparing two nodes if data of A > B return true else false
bool Compare(node &A, node &B){
    if(A.cases != B.cases) return A.cases > B.cases;
    if(A.deaths != B.deaths) return A.deaths > B.deaths;
    if(A.date.substr(6, 4) != B.date.substr(6, 4)) return A.date.substr(6, 4) > B.date.substr(6, 4);
    if(A.date.substr(3, 2) != B.date.substr(3, 2)) return A.date.substr(3, 2) > B.date.substr(3, 2);
    if(A.date.substr(0, 2) != B.date.substr(0, 2)) return A.date.substr(0, 2) > B.date.substr(0, 2);
    return true;
}

// checking if the two nodes are equal or not
bool areEqual(node &A, node &B){
    // in case any data is not equal return false
    if(A.state != B.state) return false;
    if(A.date != B.date) return false;
    if(A.cases != B.cases) return false;
    if(A.deaths != B.deaths) return false;
    // else return true
    return true;
}

/* Helper function that allocates
a new node with the given key and
    NULL left and right pointers. */
node* newNode(node A)
{
    node* Node = new node();
    Node->cases = A.cases;
    Node->deaths = A.deaths;
    Node->state = A.state;
    Node->date = A.date;
    Node->left = Node->right = NULL;
    return (Node);
}

// A utility function to right
// rotate subtree rooted with y
// See the diagram given above.
node *rightRotate(node *x)
{
    node *y = x->left;
    x->left = y->right;
    y->right = x;
    return y;
}

// A utility function to left
// rotate subtree rooted with x
// See the diagram given above.
node *leftRotate(node *x)
{
    node *y = x->right;
    x->right = y->left;
    y->left = x;
    return y;
}

// This function brings the key at
// root if key is present in tree.
// If key is not present, then it
// brings the last accessed item at
// root. This function modifies the
// tree and returns the new root
node *splay(node *root, node key)
{
    // Base cases: root is NULL or
    // key is present at root
    if (root == NULL || areEqual(*root, key))
        return root;

    // Key lies in left subtree
    if (Compare(*root, key))
    {
        // Key is not in tree, we are done
        if (root->left == NULL) return root;

        // Zig-Zig (Left Left)
        if (Compare(*(root->left), key))
        {
            // First recursively bring the
            // key as root of left-left
            root->left->left = splay(root->left->left, key);

            // Do first rotation for root,
            // second rotation is done after else
            root = rightRotate(root);
        }
        else if (!Compare(*(root->left), key)) // Zig-Zag (Left Right)
        {
            // First recursively bring
            // the key as root of left-right
            root->left->right = splay(root->left->right, key);

            // Do first rotation for root->left
            if (root->left->right != NULL)
                root->left = leftRotate(root->left);
        }

        // Do second rotation for root
        return (root->left == NULL)? root: rightRotate(root);
    }
    else // Key lies in right subtree
    {
        // Key is not in tree, we are done
        if (root->right == NULL) return root;

        // Zig-Zag (Right Left)
        if (Compare(*(root->right), key))
        {
            // Bring the key as root of right-left
            root->right->left = splay(root->right->left, key);

            // Do first rotation for root->right
            if (root->right->left != NULL)
                root->right = rightRotate(root->right);
        }
        else if (!Compare(*(root->right), key))// Zag-Zag (Right Right)
        {
            // Bring the key as root of
            // right-right and do first rotation
            root->right->right = splay(root->right->right, key);
            root = leftRotate(root);
        }

        // Do second rotation for root
        return (root->right == NULL)? root: leftRotate(root);
    }
}

// Function to insert a new key k
// in splay tree with given root
node *insert(node *root, node k)
{
    // Simple Case: If tree is empty
    if (root == NULL) return newNode(k);

    // Bring the closest leaf node to root
    root = splay(root, k);

    // If key is already present, then return
    if (areEqual(*root, k)) return root;

    // Otherwise allocate memory for new node
    node *newnode = newNode(k);

    // If root's key is greater, make
    // root as right child of newnode
    // and copy the left child of root to newnode
    if (Compare(*root , k))
    {
        newnode->right = root;
        newnode->left = root->left;
        root->left = NULL;
    }

    // If root's key is smaller, make
    // root as left child of newnode
    // and copy the right child of root to newnode
    else
    {
        newnode->left = root;
        newnode->right = root->right;
        root->right = NULL;
    }

    return newnode; // newnode becomes new root
}

// A utility function to print
// preorder traversal of the tree.
// The function also prints data of every node
void Traversal(node *root, bool flag)
{
    if(flag && result.size() >= resultNumber) return;
    if (root != NULL)
    {
        Traversal(root->right, flag);
        if((flag && result.size() < resultNumber) || !flag)
        result.push_back(*root);
        Traversal(root->left, flag);
    }
}

// this function will get the data of all the states with higher number of Cases
void findTopStatesByCases(node *root)
{
    Traversal(root, true);   
    if(result.size() > resultNumber)
     result.erase(result.end() - result.size() + resultNumber, result.end());
}

//this function will get the data of all the states with higher number of deaths
void findTopSatesByDeaths(node *root)
{
    Traversal(root, false);
    sort(result.begin(), result.end(), [](node&A, node&B){
        return A.deaths > B.deaths;
    });
    
    if(result.size() > resultNumber)
     result.erase(result.end() - result.size() + resultNumber, result.end());
}

node *root = new node();    //root pointer of splay tree

void insertElements(){

    // for(each element in Csv File) insert element in Splay tree as given below
    node *Datax = new node();

    //data 1;
    Datax->state = "State1";
    Datax->date = "01-01-2020";
    Datax->cases = 20;
    Datax->deaths = 10;
    root = insert(root, *Datax);

    //data 2;
    Datax->state = "State2";
    Datax->date = "02-01-2020";
    Datax->cases = 30;
    Datax->deaths = 99;
    root = insert(root, *Datax);

    //data 3;
    Datax->state = "State3";
    Datax->date = "01-11-2020";
    Datax->cases = 40;
    Datax->deaths = 2;
    root = insert(root, *Datax);

    //data 4;
    Datax->state = "State4";
    Datax->date = "01-01-2020";
    Datax->cases = 500;
    Datax->deaths = 10;
    root = insert(root, *Datax);

    //data 5;
    Datax->state = "State5";
    Datax->date = "01-01-2021";
    Datax->cases = 60;
    Datax->deaths = 145;
    root = insert(root, *Datax);

    //data 6;
    Datax->state = "State6";
    Datax->date = "01-01-2020";
    Datax->cases = 70;
    Datax->deaths = 70;
    root = insert(root, *Datax);

    //data 7;
    Datax->state = "State7";
    Datax->date = "01-01-2020";
    Datax->cases = 80;
    Datax->deaths = 200;
    root = insert(root, *Datax);

    //data 8;
    Datax->state = "State8";
    Datax->date = "01-01-2020";
    Datax->cases = 88;
    Datax->deaths = 100;
    root = insert(root, *Datax);

    //data 9;
    Datax->state = "State9";
    Datax->date = "01-01-2020";
    Datax->cases = 200;
    Datax->deaths = 10;
    root = insert(root, *Datax);

}

/* Driver code*/
int main()
{

    root = root->right;
    insertElements();   //this function will insert elements in the splay tree

    cout<<"Traversal of the modified Splay tree is \nstate\tdate\t\tcases\tdeaths\n";
    Traversal(root, false);
    for(node x: result){
        cout<<x.state<<" : "<<x.date<<" : "<<x.cases<<" : "<<x.deaths<<endl;
    }

    result.clear(); //clearing the result vector

    resultNumber = 5;   //number of results you want to show
    findTopStatesByCases(root); //finding states having higher number of Cases
    
    //printing the result
    cout<<"\n\nTop States by number of cases\nstate\tCases"<<endl;
    for(node x: result){
        cout<<x.state<<" : "<<x.cases<<endl;
    }

    result.clear();
    findTopSatesByDeaths(root); //finding states having higher number of deaths
    
    // printing the result
    cout<<"\nTop States by number of deaths\nstate\tdeaths"<<endl;
    for(node x: result){
        cout<<x.state<<" : "<<x.deaths<<endl;
    }

    return 0;
}
//*****************************************************************//