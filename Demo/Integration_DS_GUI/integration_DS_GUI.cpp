/*
     THIS IS THE PART WHERE THE DATA IMPLEMENTATION SHOULD BE PLACED INSIDE OF GUI
*/
//begin data structure integration
//covidObj.driver(number, deathBox, numDay, numMonth, numYear);

//integration class
class userMenu
{
private:
    //variables
    ifstream file;

    //file processing methods
    void openFile();								//open covid data file
    void validate();								//confirm data file open properly
    void populate();								//read covid data file & populate data structures with data
    void closeFile();								//close covid data file

public:
    void driver(int num, bool death, string numDay, string numMonth, string numYear);				//execute methods in proper order
    void displayTopStates(int n, bool death, string numDay, string numMonth, string numYear);       //display top states by death
    vector<pair<string, int>> stateRanks;
    // Make BTree friend of this so that we can access private members of this
    // class in BTree functions
    //friend class BTree;
    //friend class BTreeNode;
};

//global objects
BTree tree(3);                                      //create BTree object to access methods

/****************************userMenu Method Defs******************************/
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
        /*state*/
        getline(row, state, ',');
        Datax.state = state;
        /*discard fips - not used*/
        getline(row, tempfips, ',');
        fips = stoi(tempfips);
        /*number of cases*/
        getline(row, tempcases, ',');
        cases = stoi(tempcases);
        Datax.cases = cases;
        /*number of deaths*/
        getline(row, tempdeaths, ',');
        deaths = stoi(tempdeaths);
        Datax.deaths = deaths;
        //add current object to Btree data structure
        tree.insert(Datax);
    }
}

//close employee data file
void userMenu::closeFile()
{
    file.close();
}

// Comparator function to sort pairs according to second value
bool compare(const pair<string, int>& a, const pair<string, int>& b)
{
    return (a.second > b.second);
}

//display top n states by death
void userMenu::displayTopStates(int num, bool death, string numDay, string numMonth, string numYear)
{
    //populate results
    //populate vector with top states
    //call tree function based on death or cases toggle bool
    if (death)
        tree.findTopSatesByDeaths();
    else
        tree.findTopStatesByCases();

    //utilize stl data structures for counting and sorting primary data structures
    map<string, int> mapper;
    map<string, int>::iterator iter;

    //ensure date adjustment for current or futute dates
    int day = stoi(numDay);
    int month = stoi(numMonth);
    int year = stoi(numYear);
    if (month > 7 && day > 1 && year > 20)
    {
        //correction dates are based on file dates
        numDay = 1;
        numMonth = 7;
        numYear = 21;
    }

    //iterate throught results vector from BTree
    for (int i = 0; i < result.size(); i++)
    {
        //date check to meet date parameters selected by user
        if (result[i].date >= (numDay + "-" + numMonth + "-" + "20" + numYear))
            //tally state results to a map
            mapper[result[i].state]++;
    }
    //move map to vector so data can be sorted
    for (auto& it : mapper)
    {
        stateRanks.push_back(it);
    }
    //sort vevctor of states by cases
    sort(stateRanks.begin(), stateRanks.end(), compare);
    //test display sorted values
    /*for (int i = 0; i < num; i++)
    {
        cout << stateRanks[i].first << endl;
    }*/
}

//execute methods in proper order
void userMenu::driver(int num, bool death, string numDay, string numMonth, string numYear)
{
    openFile();
    populate();
    displayTopStates(num, death, numDay, numMonth, numYear);
    closeFile();
}
