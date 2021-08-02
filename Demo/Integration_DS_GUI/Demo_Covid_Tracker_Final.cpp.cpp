#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <map>
#include <vector>
#include <string>
#include <sstream>
#include <fstream>
#include <iostream>
#include <iomanip>
#include <stdlib.h>
#include <string.h>
#include <algorithm>
#include <unordered_map>
#include <list>
#include <chrono>
using namespace std;
using namespace std::chrono;

/***********************Hash Table Data Structure by Yuko Matsumoto******/
// some macros/micros to make the code a bit smaller
#define pb(a) push_back(a) // define for push_back in the vector
#define all(a) a.begin(), a.end()
#define trav(a, b) for (auto &a : b) // iterating the list
#define ff first
#define ss second

// strcuture of node which will store the data for each state on a particular date

struct node
{
    string state; //state name
    string date;  //date
    int cases;    //number of cases
    int deaths;   //deaths count
};

//hashTable whch will store the data of each state with key value of the name of the state
vector<node> result2; //this vector will store the final result of the queries
int result_number;    //this variable will store the number of results we want to receive

// comparator function which will compare the data on the bases of number of cases
bool cmp(node A, node B)
{
    if (A.cases > B.cases)
        return true;
    return false;
}

// comparator function which will compare the data on the bases of number of deaths
bool cmp2(node A, node B)
{
    if (A.deaths > B.deaths)
        return true;
    return false;
}

class HashTable
{
private:
    using Item = pair<string, vector<node>>;

    int itemsCount;
    int bucketsCount;
    list<Item> **buckets;

    int hash(const string &key)
    {
        size_t h = std::hash<string>{}(key) % static_cast<size_t>(bucketsCount);
        return static_cast<int>(h);
    }

public:
    HashTable()
        : itemsCount{},
          bucketsCount{},
          buckets{}
    {
    }

    ~HashTable()
    {
        for (int i = 0; i < bucketsCount; i++)
        {
            delete buckets[i];
        }
        if (buckets != nullptr)
        {
            delete[] buckets;
        }
    }

    float loadFactor()
    {
        if (bucketsCount == 0)
        {
            return -1.0f;
        }
        return static_cast<float>(itemsCount) / bucketsCount;
    }

    void resize(int newBucketsCount)
    {
        list<Item> **newBuckets = new list<Item> *[newBucketsCount] {};
        for (int i = 0; i < bucketsCount; i++)
        {
            if (buckets[i] != nullptr)
            {
                for (auto &item : *(buckets[i]))
                {
                    int index = hash(item.first);
                    if (newBuckets[index] == nullptr)
                    {
                        newBuckets[index] = new list<Item>;
                    }
                    newBuckets[index]->push_back(move(item));
                }
            }
        }

        for (int i = 0; i < bucketsCount; i++)
        {
            delete buckets[i];
        }
        if (buckets != nullptr)
        {
            delete[] buckets;
        }

        bucketsCount = newBucketsCount;
        buckets = newBuckets;
    }

    bool insert(const Item &item)
    {
        if (bucketsCount == 0)
        {
            resize(2);
        }
        else if (loadFactor() > 0.5f)
        {
            resize(bucketsCount * 2);
        }
        int index = hash(item.first);
        if (buckets[index] == nullptr)
        {
            buckets[index] = new list<Item>;
        }
        for (const auto &i : *(buckets[index]))
        {
            if (i.first == item.first)
            {
                return false;
            }
        }
        buckets[index]->push_back(item);
        itemsCount++;
        return true;
    }

    vector<node> &operator[](const string &key)
    {
        insert(Item{key, vector<node>{}});
        int index = hash(key);
        for (auto &item : *(buckets[index]))
        {
            if (item.first == key)
            {
                return item.second;
            }
        }
        throw "something is wrong";
    }

    vector<node> topStatesByCases(string date, int count)
    {
        vector<node> res;
        for (int i = 0; i < bucketsCount; i++)
        {
            if (buckets[i] != nullptr)
            {
                for (auto &item : *(buckets[i]))
                {
                    for (auto &node : item.second)
                    {
                        if (node.date == date)
                        {
                            res.push_back(node);
                        }
                    }
                }
            }
        }
        sort(res.begin(), res.end(), cmp);
        while (res.size() > count)
            res.pop_back();
        return res;
    }

    vector<node> topStatesByDeaths(string date, int count)
    {
        vector<node> res;
        for (int i = 0; i < bucketsCount; i++)
        {
            if (buckets[i] != nullptr)
            {
                for (auto &item : *(buckets[i]))
                {
                    for (auto &node : item.second)
                    {
                        if (node.date == date)
                        {
                            res.push_back(node);
                        }
                    }
                }
            }
        }
        sort(res.begin(), res.end(), cmp2);
        while (res.size() > count)
            res.pop_back();
        return res;
    }
};

HashTable hashTable; // Hashtable whih use predefine mapping for string

/***********************BTree Data Structure by Yuko Matsumoto***********/
// This data structure will stores the data of a state at a perticular date
struct data1
{
    string state; //name of the state
    string date;  //date
    int cases;    // number of cases on that date
    int deaths;   // number of deaths on that date
};

vector<data1> result; //this vector will stores the result
int resultNumber;     //Number of results to be stored

// Compare two datas of the state in order of number of cases -> number of deaths -> date
bool Compare(data1 &A, data1 &B)
{
    if (A.cases > B.cases)
        return true;
    return false;
}

bool Compare2(data1 &A, data1 &B)
{
    if (A.deaths > B.deaths)
        return true;
    return false;
}

bool CompareDate(data1 &A, string &B)
{
    if (A.date.substr(6, 4) != B.substr(6, 4))
        return A.date.substr(6, 4) > B.substr(6, 4);
    if (A.date.substr(3, 2) != B.substr(3, 2))
        return A.date.substr(3, 2) > B.substr(3, 2);
    if (A.date.substr(0, 2) != B.substr(0, 2))
        return A.date.substr(0, 2) > B.substr(0, 2);
    return true;
}

//this function will check if the date matches or not
bool areEqual(data1 &A, string &B)
{
    if (A.date != B)
        return false;
    return true;
}

// A BTree node
class BTreeNode
{
public:
    data1 *keys;   // An array of keys
    int t;         // Minimum degree (defines the range for number of keys)
    BTreeNode **C; // An array of child pointers
    int n;         // Current number of keys
    bool leaf;     // Is true when node is leaf. Otherwise false
public:
    BTreeNode(int _t, bool _leaf); // Constructor

    // A utility function to insert a new key in the subtree rooted with
    // this node. The assumption is, the node must be non-full when this
    // function is called
    void insertNonFull(data1 k);

    // A utility function to split the child y of this node. i is index of y in
    // child array C[]. The Child y must be full when this function is called
    void splitChild(int i, BTreeNode *y);

    // A function to traverse all nodes in a subtree rooted with this node
    void traverse();

    // A function to traverse all nodes in a subtree rooted with this node and stores it
    void findTopSatesByDeaths(string date, int count);

    // A function to store Top Cases in all the states
    void findTopStatesByCases(string date, int count);

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
    int t;           // Minimum degree
public:
    // Constructor (Initializes tree as empty)
    BTree(int _t)
    {
        root = NULL;
        t = _t;
    }

    // function to traverse the tree
    void traverse()
    {
        if (root != NULL)
            root->traverse();
    }

    void findTopSatesByDeaths(string date, int count)
    {
        result.clear();
        if (root != NULL)
            root->search(date);
        sort(result.begin(), result.end(), Compare2);
        while (result.size() > count)
            result.pop_back();
    }

    void findTopStatesByCases(string date, int count)
    {
        result.clear();
        if (root != NULL)
            root->search(date);
        sort(result.begin(), result.end(), Compare);
        while (result.size() > count)
            result.pop_back();
    }

    // function to search a key in this tree
    void search(string k)
    {
        if (root != NULL)
            root->search(k);
    }

    // The main function that inserts a new key in this B-Tree
    void insert(data1 k);
};

// Constructor for BTreeNode class
BTreeNode::BTreeNode(int t1, bool leaf1)
{
    // Copy the given minimum degree and leaf property
    t = t1;
    leaf = leaf1;

    // Allocate memory for maximum number of possible keys
    // and child pointers
    keys = new data1[2 * t - 1];
    C = new BTreeNode *[2 * t];

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
        if (keys != NULL)
            cout << " " << keys[i].state << " " << keys[i].date << " " << keys[i].cases << " " << keys[i].deaths << endl;
    }

    // Print the subtree rooted with last child
    if (leaf == false)
        C[i]->traverse();
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
        if (keys != NULL)
            if (areEqual(keys[i], Date))
            {
                result.push_back(keys[i]);
            }
    }

    // search in the subtree rooted with last child
    if (leaf == false)
        C[i]->search(Date);
}

// The main function that inserts a new key in this B-Tree
void BTree::insert(data1 Data)
{
    // If tree is empty
    if (root == NULL)
    {
        // Allocate memory for root
        root = new BTreeNode(t, true);
        root->keys[0] = Data; // Insert key
        root->n = 1;          // Update number of keys in root
    }
    else // If tree is not empty
    {
        // If root is full, then tree grows in height
        if (root->n == 2 * t - 1)
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
            if (Compare(s->keys[0], Data))
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
void BTreeNode::insertNonFull(data1 Data)
{
    // Initialize index as index of rightmost element
    int i = n - 1;

    // If this is a leaf node
    if (leaf == true)
    {
        // The following loop does two things
        // a) Finds the location of new key to be inserted
        // b) Moves all greater keys to one place ahead
        while (i >= 0 && Compare(Data, keys[i]))
        {
            keys[i + 1] = keys[i];
            i--;
        }

        // Insert the new key at found location
        keys[i + 1] = Data;
        n = n + 1;
    }
    else // If this node is not leaf
    {
        // Find the child which is going to have the new key
        while (i >= 0 && Compare(Data, keys[i]))
            i--;

        // See if the found child is full
        if (C[i + 1]->n == 2 * t - 1)
        {
            // If the child is full, then split it
            splitChild(i + 1, C[i + 1]);

            // After split, the middle key of C[i] goes up and
            // C[i] is splitted into two. See which of the two
            // is going to have the new key
            if (Compare(keys[i + 1], Data))
                i++;
        }
        C[i + 1]->insertNonFull(Data);
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
    for (int j = 0; j < t - 1; j++)
        z->keys[j] = y->keys[j + t];

    // Copy the last t children of y to z
    if (y->leaf == false)
    {
        for (int j = 0; j < t; j++)
            z->C[j] = y->C[j + t];
    }

    // Reduce the number of keys in y
    y->n = t - 1;

    // Since this node is going to have a new child,
    // create space of new child
    for (int j = n; j >= i + 1; j--)
        C[j + 1] = C[j];

    // Link the new child to this node
    C[i + 1] = z;

    // A key of y will move to this node. Find the location of
    // new key and move all greater keys one space ahead
    for (int j = n - 1; j >= i; j--)
        keys[j + 1] = keys[j];

    // Copy the middle key of y to this node
    keys[i] = y->keys[t - 1];

    // Increment count of keys in this node
    n = n + 1;
}
/*******************End of BTree Data Structure*************/

/*******************Begin Integration by Dave Rowe/*********/
class userMenu
{
private:
    //variables
    ifstream file;

    //file processing methods
    void openFile();  //open covid data file
    void validate();  //confirm data file open properly
    void populate();  //read covid data file & populate data structures with data
    void closeFile(); //close covid data file

public:
    userMenu();

    void driver(int num, bool death, string startDate);         //execute methods in proper order
    void displayTopStates(int n, bool death, string startDate); //display top states by death

    //utilize stl data structures for counting and sorting primary data structures
    map<string, int> mapper;
    map<string, int> mapper2;
    map<string, int>::iterator iter;
    map<string, int>::iterator iter2;
    vector<pair<string, int>> stateRanks;
    vector<pair<string, int>> stateRanks2;
    vector<node> hTableRanks;
    std::chrono::duration<double> total1;
    std::chrono::duration<double> total2;
    std::chrono::duration<double> qtotal1;
    std::chrono::duration<double> qtotal2;

    // Make BTree friend of this so that we can access private members of this
    // class in BTree functions
    //friend class BTree;
    //friend class BTreeNode;
};

//global objects
BTree tree(3); //create BTree object to access methods
               //no HashTable class

/****************************userMenu Method Defs******************************/

userMenu::userMenu()
{
    openFile();
    populate();
    closeFile();
}

//open covid data file
void userMenu::openFile()
{
    file.open("us-states.csv");

    validate();
}

//confirm covid data file open properly
void userMenu::validate()
{
    if (!file)
    {
        std::cerr << "Covid data file did not open properly, please ensure file is it local directory.";
        exit(123);
    }
}

//read covid data file & populate data structures with data
void userMenu::populate()
{
    //read and parse csv file data into data structures
    //local variables for processing file
    string recordLine;
    string date;
    string state;
    int fips;
    string tempfips;
    int cases;
    string tempcases;
    int deaths;
    string tempdeaths;
    //BTree tree(3);
    data1 Datax = data1();
    //Hash table
    node Datay = node();
    //reset timers
    total1 = high_resolution_clock::now() - high_resolution_clock::now();
    total2 = high_resolution_clock::now() - high_resolution_clock::now();
    //read header to discard
    getline(file, recordLine);
    //process census data
    while (getline(file, recordLine))
    {
        //convert line into string stream
        stringstream row(recordLine);
        //assigned data from stream to variables
        /*date*/
        getline(row, date, ',');
        Datax.date = date;
        Datay.date = date;
        /*state*/
        getline(row, state, ',');
        Datax.state = state;
        Datay.state = state;
        /*discard fips - not used*/
        getline(row, tempfips, ',');
        fips = stoi(tempfips);
        /*number of cases*/
        getline(row, tempcases, ',');
        cases = stoi(tempcases);
        Datax.cases = cases;
        Datay.cases = cases;
        /*number of deaths*/
        getline(row, tempdeaths, ',');
        deaths = stoi(tempdeaths);
        Datax.deaths = deaths;
        Datay.deaths = deaths;
        //add current object to Btree data structure
        //Btree timer
        auto start1 = high_resolution_clock::now();
        tree.insert(Datax);
        auto stop1 = high_resolution_clock::now();
        //add current object to Hash Table
        //HTable timer
        auto start2 = high_resolution_clock::now();
        hashTable[Datay.state].push_back(Datay);
        auto stop2 = high_resolution_clock::now();
        //add up times for data structures
        auto duration1 = duration_cast<duration<double>>(stop1 - start1);
        auto duration2 = duration_cast<duration<double>>(stop2 - start2);
        total1 = total1 + duration1;
        total2 = total2 + duration2;
    }
    //sorting the table on the cases of number of cases
    //trav(state, HashTable) sort(all(state.ss), cmp);
}

//close employee data file
void userMenu::closeFile()
{
    file.close();
}

// Comparator function to sort pairs according to second value
bool compare(const pair<string, int> &a, const pair<string, int> &b)
{
    return (a.second > b.second);
}

//display top n states by death
void userMenu::displayTopStates(int num, bool death, string date)
{
    //populate results
    //populate vector with top states
    //call tree function based on death or cases toggle bool
    result.clear();
    result2.clear();
    hTableRanks.clear();

    qtotal1 = high_resolution_clock::now() - high_resolution_clock::now();
    qtotal2 = high_resolution_clock::now() - high_resolution_clock::now();

    std::cout << date << std::endl;
    if (death)
    {
        auto start1 = high_resolution_clock::now();
        tree.findTopSatesByDeaths(date, num);
        auto stop1 = high_resolution_clock::now();
        auto start2 = high_resolution_clock::now();
        hTableRanks = hashTable.topStatesByDeaths(date, num);
        auto stop2 = high_resolution_clock::now();
        auto duration1 = duration_cast<duration<double>>(stop1 - start1);
        auto duration2 = duration_cast<duration<double>>(stop2 - start2);
        qtotal1 += duration1;
        qtotal2 += duration2;
    }
    else
    {
        auto start1 = high_resolution_clock::now();
        tree.findTopStatesByCases(date, num);
        auto stop1 = high_resolution_clock::now();
        auto start2 = high_resolution_clock::now();
        hTableRanks = hashTable.topStatesByCases(date, num);
        auto stop2 = high_resolution_clock::now();
        auto duration1 = duration_cast<duration<double>>(stop1 - start1);
        auto duration2 = duration_cast<duration<double>>(stop2 - start2);
        qtotal1 += duration1;
        qtotal2 += duration2;
    }
    std::cout << result.size() << " " << hTableRanks.size() << std::endl;

    //ensure date adjustment for current or futute dates
    // int day = stoi(numDay);
    // int month = stoi(numMonth);
    // int year = stoi(numYear);
    // if (month > 7 && day > 1 && year > 20)
    // {
    //     //correction dates are based on file dates
    //     numDay = 1;
    //     numMonth = 7;
    //     numYear = 21;
    // }

    stateRanks.clear();
    stateRanks2.clear();

    for (auto &it : result)
    {
        if (death)
            stateRanks.push_back(make_pair(it.state, it.deaths));
        else
            stateRanks.push_back(make_pair(it.state, it.cases));
    }

    for (auto &it : hTableRanks)
    {
        if (death)
            stateRanks2.push_back(make_pair(it.state, it.deaths));
        else
            stateRanks2.push_back(make_pair(it.state, it.cases));
    }

    //test display sorted values

    for (int i = 0; i < num; i++)
    {
        cout << stateRanks[i].first << "    " << stateRanks[i].second << endl;
    }
}

//execute methods in proper order
void userMenu::driver(int num, bool death, string startDate)
{
    displayTopStates(num, death, startDate);
}
/**************End Integration********************/

/****************GUI Interface by Andrew Yu****************************/
//void updatePageTwo(int number, sf::Font, vector<sf::Text> &vec);
bool checkNum(int start, int end, string number);
void StoreImage(string name, sf::Texture &text, map<string, sf::Texture> &imageStorage);
int main()
{
    //create objects for integration
    userMenu covidObj;
    //flags
    int page = 0;
    int visible1 = 0;
    int visibleMonth = 0;
    int visibleDay = 0;
    int visibleYear = 0;
    bool contained = false;
    bool containedMonth = false;
    bool containedDay = false;
    bool containedYear = false;
    bool deathBox = false;
    bool caseBox = false;

    string numState = "";
    string numMonth = "";
    string numYear = "";
    string numDay = "";

    vector<sf::Text> textVec;
    vector<sf::Text> textVec2;
    vector<sf::Text> textVec3;
    vector<sf::Text> textVec4;

    //start up window
    int length = 1920;
    int width = 1000;
    sf::RenderWindow window(sf::VideoMode(length, width), "Covid Tracker", sf::Style::Titlebar | sf::Style::Close);
    window.setVerticalSyncEnabled(true);
    // tgui::Gui gui(window);
    //load main text font
    sf::Font font;
    font.loadFromFile("Arvo-Regular.ttf");
    sf::Font bold;
    bold.loadFromFile("Arvo-Bold.ttf");

    map<string, sf::Texture> imageStorage;
    sf::Vector2u windowSize = window.getSize();

    //set up all the textures
    sf::Texture backText;
    backText.loadFromFile("Background.jpg");
    StoreImage("Background", backText, imageStorage);

    //sf::Texture white;
    //white.loadFromFile("images/white.jpg");
    //StoreImage("White", white, imageStorage);

    sf::RectangleShape top(sf::Vector2f(1920, 100));
    top.setPosition(0, 0);
    top.setFillColor(sf::Color(144, 238, 144, 180));

    sf::Sprite background(imageStorage["Background"]);

    //fill the entire window will a background image
    sf::Vector2u textureBackSize = backText.getSize();
    //found scaling in a sfml tutorial that scales images to the size of the screen on stack overflow
    float scaleBackX = (float)windowSize.x / textureBackSize.x;
    float scaleBackY = (float)windowSize.y / textureBackSize.y;
    background.setScale(scaleBackX, scaleBackY);
    //set the top rectange of the window
    //  sf::Sprite top(imageStorage["White"]);
    //  sf::Vector2u textureWhiteSize = white.getSize();
    //  float scaleWhiteX = (float)windowSize.x / textureWhiteSize.x;
    //make it transparent
    //top.setColor(sf::Color(255, 255, 255, 200));
    // top.setScale(scaleWhiteX, .15);
    //add title
    sf::Text title;
    title.setString("Covid Tracker");
    title.setFont(bold);
    title.setCharacterSize(35);
    title.setFillColor(sf::Color::Blue);
    title.setPosition(750, 30);

    //make the first answering space
    sf::RectangleShape answerNumStates(sf::Vector2f(170, 40));
    answerNumStates.setPosition(1100, 260);
    answerNumStates.setOutlineColor(sf::Color::Black);
    answerNumStates.setOutlineThickness(0);

    sf::Text questionOne;
    questionOne.setString("Please enter number of states to display:");
    questionOne.setFont(font);
    questionOne.setCharacterSize(40);
    questionOne.setFillColor(sf::Color::Black);
    questionOne.setPosition(280, 250);

    sf::Text constraint1;
    constraint1.setString("Enter 1-50 only");
    constraint1.setFont(font);
    constraint1.setCharacterSize(20);
    constraint1.setFillColor(sf::Color(169, 169, 169, 150));
    constraint1.setPosition(1110, 265);

    sf::Text answerTextOne;
    answerTextOne.setString("");
    answerTextOne.setFont(font);
    answerTextOne.setCharacterSize(27);
    answerTextOne.setFillColor(sf::Color::Black);
    answerTextOne.setPosition(1230, 265);

    sf::Text error1;
    int visibleError = false;
    error1.setString("Invalid Input");
    error1.setFont(font);
    error1.setCharacterSize(27);
    error1.setFillColor(sf::Color::Red);
    error1.setPosition(1100, 300);

    //second answering space//
    //month
    sf::RectangleShape answerDate1(sf::Vector2f(70, 40));
    answerDate1.setPosition(1250, 500);
    answerDate1.setOutlineColor(sf::Color::Black);
    answerDate1.setOutlineThickness(0);

    sf::Text monthText;
    monthText.setString("Mth");
    monthText.setFont(font);
    monthText.setCharacterSize(20);
    monthText.setFillColor(sf::Color(169, 169, 169, 150));
    monthText.setPosition(1260, 510);

    sf::Text answerTextMonth;
    answerTextMonth.setString("");
    answerTextMonth.setFont(font);
    answerTextMonth.setCharacterSize(27);
    answerTextMonth.setFillColor(sf::Color::Black);
    answerTextMonth.setPosition(1260, 510);

    sf::Text errorMonth;
    int visibleErrorMonth = false;
    errorMonth.setString("Invalid Month");
    errorMonth.setFont(font);
    errorMonth.setCharacterSize(25);
    errorMonth.setFillColor(sf::Color::Red);
    errorMonth.setPosition(1240, 550);

    //day
    sf::RectangleShape answerDate2(sf::Vector2f(70, 40));
    answerDate2.setPosition(1350, 500);
    answerDate2.setOutlineColor(sf::Color::Black);
    answerDate2.setOutlineThickness(0);

    sf::Text dayText;
    dayText.setString("Day");
    dayText.setFont(font);
    dayText.setCharacterSize(20);
    dayText.setFillColor(sf::Color(169, 169, 169, 150));
    dayText.setPosition(1360, 510);

    sf::Text answerTextDay;
    answerTextDay.setString("");
    answerTextDay.setFont(font);
    answerTextDay.setCharacterSize(27);
    answerTextDay.setFillColor(sf::Color::Black);
    answerTextDay.setPosition(1360, 510);

    sf::Text errorDay;
    int visibleErrorDay = false;
    errorDay.setString("Invalid Day");
    errorDay.setFont(font);
    errorDay.setCharacterSize(25);
    errorDay.setFillColor(sf::Color::Red);
    errorDay.setPosition(1240, 570);
    //year
    sf::RectangleShape answerDate3(sf::Vector2f(70, 40));
    answerDate3.setPosition(1450, 500);
    answerDate3.setOutlineColor(sf::Color::Black);
    answerDate3.setOutlineThickness(0);

    sf::Text yearText;
    yearText.setString("Year");
    yearText.setFont(font);
    yearText.setCharacterSize(20);
    yearText.setFillColor(sf::Color(169, 169, 169, 150));
    yearText.setPosition(1455, 510);

    sf::Text answerTextYear;
    answerTextYear.setString("");
    answerTextYear.setFont(font);
    answerTextYear.setCharacterSize(27);
    answerTextYear.setFillColor(sf::Color::Black);
    answerTextYear.setPosition(1455, 510);

    sf::Text errorYear;
    int visibleErrorYear = false;
    errorYear.setString("Invalid Year");
    errorYear.setFont(font);
    errorYear.setCharacterSize(25);
    errorYear.setFillColor(sf::Color::Red);
    errorYear.setPosition(1240, 590);

    sf::Text questionTwo;
    questionTwo.setString("Please enter effective date desired for cases:");
    questionTwo.setFont(font);
    questionTwo.setCharacterSize(40);
    questionTwo.setFillColor(sf::Color::Black);
    questionTwo.setPosition(280, 500);

    //make the bottom team promo
    sf::RectangleShape bottonPromo(sf::Vector2f(240, 90));
    bottonPromo.setPosition(1690, 950);
    bottonPromo.setOutlineColor(sf::Color::Black);
    bottonPromo.setFillColor(sf::Color(50, 50, 50, 240));

    sf::Text promoText;

    promoText.setString("By team FlowSoft");
    promoText.setFont(font);
    promoText.setCharacterSize(24);
    promoText.setFillColor(sf::Color::White);
    promoText.setPosition(1700, 960);

    //make buttons

    sf::RectangleShape searchButton(sf::Vector2f(190, 70));
    searchButton.setPosition(330, 790);
    searchButton.setOutlineColor(sf::Color::Black);
    searchButton.setOutlineThickness(1);
    searchButton.setFillColor(sf::Color::Yellow);

    sf::Text searchText;
    searchText.setString("Search");
    searchText.setFont(font);
    searchText.setCharacterSize(34);
    searchText.setFillColor(sf::Color::Black);
    searchText.setPosition(370, 800);

    sf::RectangleShape resetButton(sf::Vector2f(160, 70));
    resetButton.setPosition(1700, 720);
    resetButton.setOutlineColor(sf::Color::Black);
    resetButton.setOutlineThickness(1);
    resetButton.setFillColor(sf::Color::Yellow);

    sf::Text resetText;
    resetText.setString("Reset");
    resetText.setFont(font);
    resetText.setCharacterSize(34);
    resetText.setFillColor(sf::Color::Black);
    resetText.setPosition(1740, 730);

    sf::RectangleShape quitButton(sf::Vector2f(160, 70));
    quitButton.setPosition(1700, 790);
    quitButton.setOutlineColor(sf::Color::Black);
    quitButton.setOutlineThickness(1);
    quitButton.setFillColor(sf::Color::Yellow);

    sf::Text quitText;
    quitText.setString("Quit");
    quitText.setFont(font);
    quitText.setCharacterSize(34);
    quitText.setFillColor(sf::Color::Black);
    quitText.setPosition(1740, 800);

    //choose cases or deaths option
    sf::RectangleShape caseButton(sf::Vector2f(160, 70));
    caseButton.setPosition(750, 630);
    caseButton.setOutlineColor(sf::Color::Black);
    //caseButton.setOutlineThickness(1);
    caseButton.setFillColor(sf::Color(135, 206, 250));

    sf::Text caseText;
    caseText.setString("Cases");
    caseText.setFont(font);
    caseText.setCharacterSize(34);
    caseText.setFillColor(sf::Color::Black);
    caseText.setPosition(775, 640);

    sf::RectangleShape deathButton(sf::Vector2f(160, 70));
    deathButton.setPosition(950, 630);
    deathButton.setOutlineColor(sf::Color::Black);
    //deathButton.setOutlineThickness(1);
    deathButton.setFillColor(sf::Color(135, 206, 250));

    sf::Text deathText;
    deathText.setString("Deaths");
    deathText.setFont(font);
    deathText.setCharacterSize(34);
    deathText.setFillColor(sf::Color::Black);
    deathText.setPosition(970, 640);

    sf::Text searchByTitle;
    searchByTitle.setString("Search By: ");
    searchByTitle.setFont(font);
    searchByTitle.setCharacterSize(34);
    searchByTitle.setFillColor(sf::Color::Black);
    searchByTitle.setPosition(860, 580);

    sf::Text searchByError;
    bool visibleSearch = false;
    searchByError.setString("Choose One");
    searchByError.setFont(font);
    searchByError.setCharacterSize(25);
    searchByError.setFillColor(sf::Color::Red);
    searchByError.setPosition(860, 710);

    //make the second page background rectangles
    sf::RectangleShape whiteSpace1(sf::Vector2f(740, 800));
    whiteSpace1.setPosition(50, 170);
    whiteSpace1.setFillColor(sf::Color(255, 255, 255, 170));

    sf::Text bTreeTitle;
    bTreeTitle.setString("BTree");
    bTreeTitle.setPosition(350, 170);
    bTreeTitle.setFont(bold);
    bTreeTitle.setCharacterSize(30);
    bTreeTitle.setFillColor(sf::Color::Black);

    sf::RectangleShape whiteSpace2(sf::Vector2f(740, 800));
    whiteSpace2.setPosition(900, 170);
    whiteSpace2.setFillColor(sf::Color(255, 255, 255, 170));

    sf::Text hashTableTitle;
    hashTableTitle.setString("HashTable");
    hashTableTitle.setPosition(1150, 170);
    hashTableTitle.setFont(bold);
    hashTableTitle.setCharacterSize(30);
    hashTableTitle.setFillColor(sf::Color::Black);

    sf::Text description;
    description.setString("");
    description.setPosition(50, 110);
    description.setFont(font);
    description.setCharacterSize(20);
    description.setFillColor(sf::Color::Black);

    sf::Text time1;
    time1.setString("BTree Insert Time: " + to_string(covidObj.total1.count()));
    time1.setPosition(50, 140);
    time1.setFont(font);
    time1.setCharacterSize(20);
    time1.setFillColor(sf::Color::Red);

    sf::Text qtime1;
    qtime1.setString("BTree Query Time: " + to_string(covidObj.qtotal1.count()));
    qtime1.setPosition(510, 140);
    qtime1.setFont(font);
    qtime1.setCharacterSize(20);
    qtime1.setFillColor(sf::Color::Red);

    sf::Text time2;
    time2.setString("HashTable Insert: " + to_string(covidObj.total2.count()));
    time2.setPosition(900, 140);
    time2.setFont(font);
    time2.setCharacterSize(20);
    time2.setFillColor(sf::Color::Red);

    sf::Text qtime2;
    qtime2.setString("HashTable Query: " + to_string(covidObj.qtotal2.count()));
    qtime2.setPosition(1370, 140);
    qtime2.setFont(font);
    qtime2.setCharacterSize(20);
    qtime2.setFillColor(sf::Color::Red);

    for (int x = 1; x <= 25; x++)
    {
        sf::Text tempText;
        ///create the text for the 2nd page
        tempText.setString("");
        tempText.setPosition(60, 180 + 30 * x);
        tempText.setFont(font);
        tempText.setCharacterSize(16);
        tempText.setFillColor(sf::Color::Black);
        textVec.push_back(tempText);

        sf::Text tempText1;
        tempText1.setString("");
        tempText1.setPosition(400, 180 + 30 * x);
        tempText1.setFont(font);
        tempText1.setCharacterSize(16);
        tempText1.setFillColor(sf::Color::Black);
        textVec2.push_back(tempText1);

        sf::Text tempText2;
        tempText2.setString("");
        tempText2.setPosition(930, 180 + 30 * x);
        tempText2.setFont(font);
        tempText2.setCharacterSize(16);
        tempText2.setFillColor(sf::Color::Black);
        textVec3.push_back(tempText2);

        sf::Text tempText3;
        tempText3.setString("");
        tempText3.setPosition(1280, 180 + 30 * x);
        tempText3.setFont(font);
        tempText3.setCharacterSize(16);
        tempText3.setFillColor(sf::Color::Black);
        textVec4.push_back(tempText3);
    }
    while (window.isOpen())
    {
        sf::Event event;

        while (window.pollEvent(event))
        {

            if (event.type == sf::Event::Closed)
                window.close();
            if (event.type == sf::Event::MouseButtonPressed)
            {

                if (event.mouseButton.button == sf::Mouse::Left)
                {

                    auto mousePosition = sf::Mouse::getPosition();
                    mousePosition = sf::Mouse::getPosition(window);

                    sf::Vector2f mousePosFloat(mousePosition.x, mousePosition.y);
                    sf::FloatRect quitRectangle = quitButton.getGlobalBounds();
                    if (quitRectangle.contains(mousePosFloat))
                    {

                        window.close();
                    }
                    sf::FloatRect resetRectangle = resetButton.getGlobalBounds();
                    if (resetRectangle.contains(mousePosFloat))
                    {
                        visibleError = false;
                        visibleErrorDay = false;
                        visibleErrorMonth = false;
                        visibleErrorYear = false;
                        visibleSearch = false;
                        deathBox = false;
                        caseBox = false;

                        numState = "";
                        numYear = "";
                        numDay = "";
                        numMonth = "";

                        visible1 = 0;
                        visibleDay = 0;
                        visibleMonth = 0;
                        visibleYear = 0;

                        contained = false;
                        containedDay = false;
                        containedMonth = false;
                        containedYear = false;
                        deathButton.setOutlineThickness(0);
                        caseButton.setOutlineThickness(0);

                        answerTextOne.setString("");
                        answerTextDay.setString("");
                        answerTextMonth.setString("");
                        answerTextYear.setString("");
                        page = 0;
                        for (int x = 0; x < textVec.size(); x++)
                        {
                            textVec[x].setString("");
                        }
                        for (int x = 0; x < textVec2.size(); x++)
                        {
                            textVec2[x].setString("");
                        }
                        for (int x = 0; x < textVec3.size(); x++)
                        {
                            textVec3[x].setString("");
                        }
                        for (int x = 0; x < textVec4.size(); x++)
                        {
                            textVec4[x].setString("");
                        }
                    }
                    if (page == 0)
                    {
                        sf::FloatRect caseRectangle = caseButton.getGlobalBounds();
                        if (caseRectangle.contains(mousePosFloat))
                        {
                            caseButton.setOutlineThickness(1);
                            deathButton.setOutlineThickness(0);
                            deathBox = false;
                            caseBox = true;
                            visibleSearch = false;
                        }
                        sf::FloatRect deathRectangle = deathButton.getGlobalBounds();
                        if (deathRectangle.contains(mousePosFloat))
                        {
                            deathBox = true;
                            caseBox = false;
                            caseButton.setOutlineThickness(0);
                            deathButton.setOutlineThickness(1);
                            visibleSearch = false;
                        }

                        //if click on search
                        sf::FloatRect searchRectangle = searchButton.getGlobalBounds();
                        if (searchRectangle.contains(mousePosFloat))
                        {

                            //test if the number is a number and is less than 50 and over 0
                            bool flag = checkNum(1, 50, numState);
                            bool flagMonth = checkNum(1, 12, numMonth);
                            bool flagDay = checkNum(1, 31, numDay);
                            bool flagYear = checkNum(19, 21, numYear);

                            // cout << numState << endl;

                            if (flag && flagMonth && flagDay && flagYear && (deathBox == true || caseBox == true))
                            {
                                int number = stoi(numState);
                                string toggle;
                                if (deathBox == true)
                                {
                                    toggle = "Deaths";
                                }
                                else
                                {
                                    toggle = "Cases";
                                }
                                description.setString("Date " + numMonth + "/" + numDay + "/" + numYear + "                                               Search by " + toggle);
                                /*
                                    THIS IS THE PART WHERE THE DATA IMPLEMENTATION SHOULD BE PLACED
                                */
                                //begin data structure integration
                                if (stoi(numDay) < 10)
                                    numDay.insert(0, "0");
                                if (stoi(numMonth) < 10)
                                    numMonth.insert(0, "0");
                                string startDate = "20" + numYear + "-" + numMonth + "-" + numDay;
                                covidObj.driver(number, deathBox, startDate);

                                //was going to put this into a function but it appears that sf might not have supported this
                                page = 1;
                                int value;
                                if (number <= 25)
                                {
                                    value = number;
                                }
                                else
                                {
                                    value = 25;
                                }
                                //counter control variable for results vector, needs to start with 0
                                int index = 0;
                                int numberList = 0;
                                for (int x = 1; x <= value; x++)
                                {
                                    numberList = covidObj.stateRanks[index].second;
                                    ///create the text for the 2nd page
                                    textVec[x - 1].setString(to_string(x) + ". " + covidObj.stateRanks[index].first + "      " + to_string(numberList));
                                    index++;
                                }
                                if (number > 25)
                                {
                                    int value2 = number - 25;
                                    for (int x = 1; x <= value2; x++)
                                    {
                                        numberList = covidObj.stateRanks[index].second;
                                        textVec2[x - 1].setString((to_string(x + 25)) + ". " + covidObj.stateRanks[index].first + "     " + to_string(numberList));
                                        index++;
                                    }
                                }
                                //reset index for hash table vector iteration
                                index = 0;
                                numberList = 0;
                                for (int x = 1; x <= value; x++)
                                {
                                    numberList = covidObj.stateRanks2[index].second;
                                    textVec3[x - 1].setString(to_string(x) + ". " + covidObj.stateRanks2[index].first + "     " + to_string(numberList));
                                    index++;
                                }
                                if (number > 25)
                                {
                                    int value2 = number - 25;
                                    for (int x = 1; x <= value2; x++)
                                    {
                                        numberList = covidObj.stateRanks2[index].second;
                                        textVec4[x - 1].setString((to_string(x + 25)) + ". " + covidObj.stateRanks2[index].first + "     " + to_string(numberList));
                                        index++;
                                    }
                                }
                                cout << "Btree Time: " << covidObj.total1.count() << endl; //may need to use to_string(covidObj.total1.count()) in setString.
                                cout << "HashTable Time: " << covidObj.total2.count() << endl;
                                time1.setString("BTree Insert Time: " + to_string(covidObj.total1.count()));
                                qtime1.setString("BTree Query Time: " + to_string(covidObj.qtotal1.count()));
                                time2.setString("HashTable Insert: " + to_string(covidObj.total2.count()));
                                qtime2.setString("HashTable Query: " + to_string(covidObj.qtotal2.count()));
                            }
                            else
                            {
                                if (flag == false)
                                    visibleError = true;
                                if (flagDay == false)
                                    visibleErrorDay = true;
                                if (flagMonth == false)
                                    visibleErrorMonth = true;
                                if (flagYear == false)
                                    visibleErrorYear = true;
                                if (caseBox == false && deathBox == false)
                                {
                                    visibleSearch = true;
                                    //cout << "triggered" << endl;
                                }
                            }
                        }

                        sf::FloatRect answerOneRect = answerNumStates.getGlobalBounds();
                        if (answerOneRect.contains(mousePosFloat))
                        {
                            answerNumStates.setOutlineThickness(1);
                            visibleError = false;
                            contained = true;
                            visible1 = 1;
                        }
                        if (!answerOneRect.contains(mousePosFloat))
                        {
                            contained = false;
                            answerNumStates.setOutlineThickness(0);
                            if (numState == "")
                            {
                                visible1 = 0;
                            }
                        }
                        sf::FloatRect answerMonthRect = answerDate1.getGlobalBounds();
                        if (answerMonthRect.contains(mousePosFloat))
                        {
                            answerDate1.setOutlineThickness(1);
                            visibleErrorMonth = false;
                            containedMonth = true;
                            visibleMonth = 1;
                        }
                        if (!answerMonthRect.contains(mousePosFloat))
                        {
                            containedMonth = false;
                            answerDate1.setOutlineThickness(0);
                            if (numMonth == "")
                            {
                                visibleMonth = 0;
                            }
                        }
                        sf::FloatRect answerDayRect = answerDate2.getGlobalBounds();
                        if (answerDayRect.contains(mousePosFloat))
                        {
                            answerDate2.setOutlineThickness(1);
                            visibleErrorDay = false;
                            containedDay = true;
                            visibleDay = 1;
                        }
                        if (!answerDayRect.contains(mousePosFloat))
                        {
                            containedDay = false;
                            answerDate2.setOutlineThickness(0);
                            if (numDay == "")
                            {
                                visibleDay = 0;
                            }
                        }

                        sf::FloatRect answerYearRect = answerDate3.getGlobalBounds();
                        if (answerYearRect.contains(mousePosFloat))
                        {
                            answerDate3.setOutlineThickness(1);
                            visibleErrorYear = false;
                            containedYear = true;
                            visibleYear = 1;
                        }
                        if (!answerYearRect.contains(mousePosFloat))
                        {
                            containedYear = false;
                            answerDate3.setOutlineThickness(0);
                            if (numYear == "")
                            {
                                visibleYear = 0;
                            }
                        }
                    }
                }
            }

            if (contained == true)
            {
                if (event.type == sf::Event::TextEntered)
                {
                    //if delete was hit, i found the number from a youtube video on making buttons with a class
                    //cout << event.text.unicode << endl;
                    if (event.text.unicode == 8)
                    {
                        if (numState.size() > 0)
                        {
                            string x = "";
                            for (int c = 0; c < numState.size() - 1; c++)
                            {
                                x += numState[c];
                            }
                            numState = x;
                            answerTextOne.setString(numState);
                        }
                    }
                    else if (event.text.unicode == 32)
                    {
                        //to stop spaces
                    }
                    //if esc or enter is pressed
                    else if (event.text.unicode == 13 || event.text.unicode == 27)
                    {
                        contained = false;
                        answerNumStates.setOutlineThickness(0);
                        if (numState == "")
                        {
                            visible1 = 0;
                        }
                    }
                    else if (event.text.unicode < 128 && numState.size() < 2)
                    {
                        char key = (char)event.text.unicode;
                        numState = numState + key;

                        answerTextOne.setString(numState);
                    }
                }
            }
            //text change for month
            if (containedMonth == true)
            {
                if (event.type == sf::Event::TextEntered)
                {

                    if (event.text.unicode == 8)
                    {
                        if (numMonth.size() > 0)
                        {
                            string x = "";
                            for (int c = 0; c < numMonth.size() - 1; c++)
                            {
                                x += numMonth[c];
                            }
                            numMonth = x;
                            answerTextMonth.setString(numMonth);
                        }
                    }
                    else if (event.text.unicode == 32)
                    {
                        //to stop spaces
                    }
                    //if esc or enter is pressed
                    else if (event.text.unicode == 13 || event.text.unicode == 27)
                    {
                        containedMonth = false;
                        answerTextMonth.setOutlineThickness(0);
                        if (numMonth == "")
                        {
                            visibleMonth = 0;
                        }
                    }
                    else if (event.text.unicode < 128 && numMonth.size() < 2)
                    {
                        char key = (char)event.text.unicode;
                        numMonth = numMonth + key;

                        answerTextMonth.setString(numMonth);
                    }
                }
            }

            //Day change text
            if (containedDay == true)
            {
                if (event.type == sf::Event::TextEntered)
                {

                    if (event.text.unicode == 8)
                    {
                        if (numDay.size() > 0)
                        {
                            string x = "";
                            for (int c = 0; c < numDay.size() - 1; c++)
                            {
                                x += numDay[c];
                            }
                            numDay = x;
                            answerTextDay.setString(numDay);
                        }
                    }
                    else if (event.text.unicode == 32)
                    {
                        //to stop spaces
                    }
                    //if esc or enter is pressed
                    else if (event.text.unicode == 13 || event.text.unicode == 27)
                    {
                        containedDay = false;
                        answerTextDay.setOutlineThickness(0);
                        if (numDay == "")
                        {
                            visibleDay = 0;
                        }
                    }
                    else if (event.text.unicode < 128 && numDay.size() < 2)
                    {
                        char key = (char)event.text.unicode;
                        numDay = numDay + key;

                        answerTextDay.setString(numDay);
                    }
                }
            }
            //change year
            if (containedYear == true)
            {
                if (event.type == sf::Event::TextEntered)
                {

                    if (event.text.unicode == 8)
                    {
                        if (numYear.size() > 0)
                        {
                            string x = "";
                            for (int c = 0; c < numYear.size() - 1; c++)
                            {
                                x += numYear[c];
                            }
                            numYear = x;
                            answerTextYear.setString(numYear);
                        }
                    }
                    else if (event.text.unicode == 32)
                    {
                        //to stop spaces
                    }
                    //if esc or enter is pressed
                    else if (event.text.unicode == 13 || event.text.unicode == 27)
                    {
                        containedYear = false;
                        answerTextYear.setOutlineThickness(0);
                        if (numYear == "")
                        {
                            visibleYear = 0;
                        }
                    }
                    else if (event.text.unicode < 128 && numYear.size() < 2)
                    {
                        char key = (char)event.text.unicode;
                        numYear = numYear + key;

                        answerTextYear.setString(numYear);
                    }
                }
            }
        }
        window.clear();
        //all pages
        window.draw(background);
        window.draw(top);
        window.draw(bottonPromo);
        window.draw(title);
        window.draw(promoText);
        window.draw(quitButton);
        window.draw(quitText);
        window.draw(resetButton);
        window.draw(resetText);

        //first page
        if (page == 0)
        {
            window.draw(answerNumStates);
            window.draw(answerDate1);
            window.draw(answerDate2);
            window.draw(answerDate3);
            window.draw(questionOne);

            window.draw(deathButton);

            window.draw(caseButton);
            window.draw(deathText);
            window.draw(caseText);

            window.draw(searchByTitle);

            if (visible1 == 0)
            {
                window.draw(constraint1);
            }
            else
            {
                window.draw(answerTextOne);
            }
            if (visibleMonth == 0)
            {
                window.draw(monthText);
            }
            else
            {
                window.draw(answerTextMonth);
            }
            if (visibleDay == 0)
            {
                window.draw(dayText);
            }
            else
            {
                window.draw(answerTextDay);
            }
            if (visibleYear == 0)
            {
                window.draw(yearText);
            }
            else
            {
                window.draw(answerTextYear);
            }

            window.draw(questionTwo);

            window.draw(searchButton);

            if (visibleError == true)
            {
                window.draw(error1);
            }
            if (visibleErrorDay == true)
            {
                window.draw(errorDay);
            }
            if (visibleErrorMonth == true)
            {
                window.draw(errorMonth);
            }
            if (visibleErrorYear == true)
            {
                window.draw(errorYear);
            }
            if (visibleSearch == true)
            {
                window.draw(searchByError);
            }

            window.draw(searchText);
        }
        if (page == 1)
        {
            window.draw(whiteSpace1);
            window.draw(whiteSpace2);
            window.draw(time1);
            window.draw(qtime1);
            window.draw(time2);
            window.draw(qtime2);
            window.draw(description);
            window.draw(bTreeTitle);
            window.draw(hashTableTitle);
            for (int x = 0; x < textVec.size(); x++)
            {
                window.draw(textVec[x]);
            }
            for (int x = 0; x < textVec2.size(); x++)
            {
                window.draw(textVec2[x]);
            }
            for (int x = 0; x < textVec3.size(); x++)
            {
                window.draw(textVec3[x]);
            }
            for (int x = 0; x < textVec4.size(); x++)
            {
                window.draw(textVec4[x]);
            }
        }

        window.display();
    }
}

void StoreImage(string name, sf::Texture &text, map<string, sf::Texture> &imageStorage)
{
    imageStorage.emplace(name, text);
}
bool checkNum(int start, int end, string number)
{
    bool flag = true;
    if (number == "" || number == " " || number == "  ")
    {

        flag = false;
    }
    for (int c = 0; c < number.size(); c++)
    {

        if (isdigit(number[c]) == false)
        {
            flag = false;
        }
    }
    try
    {
        int value = stoi(number);
        if (value > end || value < start)
        {
            flag = false;
        }
    }
    catch (...)
    {
        flag = false;
    }

    return flag;
}