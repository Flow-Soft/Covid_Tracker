#include<bits/stdc++.h>
using namespace std;

// some macros/micros to make the code a bit smaller
#define pb(a) push_back(a)  // define for push_back in the vector
#define all(a) a.begin(),a.end()   
#define trav(a,b) for(auto &a: b)   // iterating the list
#define ff first
#define ss second

// strcuture of node which will store the data for each state on a particular date

struct node{
    string state;   //state name
    string date;    //date
    int cases;      //number of cases
    int deaths;     //deaths count
};

//hashTable whch will store the data of each state with key value of the name of the state
unordered_map<string , vector<node>> HashTable; // Hashtable whih use predefine mapping for string
vector<node> result;    //this vector will store the final result of the queries
int result_number;      //this variable will store the number of results we want to receive

// comparator function which will compare the data on the bases of number of cases
bool cmp(node A , node B){
    if(A.cases > B.cases) return true;
    return false;
}

// comparator function which will compare the data on the bases of number of deaths
bool cmp2(node A , node B){
    if(A.deaths > B.deaths) return true;
    return false;
}


// IN this function we will store the elements in the HashTable
void insert_elements(){

    for(int i=0;i<4;i++){
        node Datax = node();
        Datax.state  = "State1";
        Datax.date = "01-01-2020";
        Datax.cases = rand()%100;
        Datax.deaths = rand()%100;
        HashTable[Datax.state].push_back(Datax);
    }

    for(int i=0;i<4;i++){
        node Datax = node();
        Datax.state  = "State2";
        Datax.date = "01-01-2020";
        Datax.cases = rand()%100;
        Datax.deaths = rand()%100;
        HashTable[Datax.state].push_back(Datax);
    }

    for(int i=0;i<4;i++){
        node Datax = node();
        Datax.state  = "State3";
        Datax.date = "01-01-2020";
        Datax.cases = rand()%100;
        Datax.deaths = rand()%100;
        HashTable[Datax.state].push_back(Datax);
    }

    //sorting the table on the cases of number of cases
    trav(state, HashTable) sort(all(state.ss), cmp);

}

// this function traverse the whole table and print each data
void print_all_data(){
    cout<<"State\tDate\tCases\tDeaths\n";
    trav(x, HashTable){
        trav(data, x.ss){
            cout<<data.state<<" "<<data.date<<" "<<data.cases<<"\t"<<data.deaths<<endl;
        }
        cout<<endl;
    }
}


// this function will gave the result for highest number of cases
vector<node> Cases_in_each_state(int num){

    result.clear(); //clearing the list

    // travering each state and push the data on 0th index in the result
    trav(data, HashTable){
        result.pb(data.ss[0]);
    }

    //sort the result on the basis of number of cases
    sort(all(result), cmp);
    //while result size is more then required number pop_back from the result
    while(result.size() > num) result.pop_back();

    //return the result
    return result;
}

// this function will resturn the data of states woth highest number of deaths
vector<node> Deaths_in_each_state(int num){

    //clearing the result
    result.clear();

    //traversing the whole table and put the data in result with the most number of deaths
    trav(state, HashTable){
        node temp = state.ss[0];
        trav(data, state.ss){
            // comparing the number of deaths on each day for the perticular state
            if(data.deaths > temp.deaths) temp = data;
        }
        result.pb(temp);
    }

    //sort the result on the basis of deaths
    sort(all(result), cmp2);
    //while result size is more then required number pop_back from the result
    while(result.size() > num) result.pop_back();

    return result;

}

// this funtion will return the data of each state on the perticular date
vector<node> Data_of_each_state_on_date(int num, string date){

    //clearing the result
     result.clear();

    //traverse the whole data for the perticular state for the perticular date
    trav(data, HashTable){
        auto it = find_if(all(data.ss), [&date = date](node&A){
            return (A.date == date); 
        });
        // if the date is found put thatg data into the result
        if(it != data.ss.end())
        result.pb(*it);
    }

    //return result
    return result;

}

int main() {
   
    insert_elements();  //inserting all the data
    print_all_data();   //traversing whole data

    result_number = 5;  //number of results we want

    result = Cases_in_each_state(result_number);    // find number of states with higher number of cases

    //printing result
    cout<<"State\tDate\tCases\tDeaths\n";
    trav(data, result){
        cout<<data.state<<" "<<data.date<<" "<<data.cases<<"\t"<<data.deaths<<endl;
    }
    cout<<endl;

    result = Deaths_in_each_state(result_number);// find number of states with higher number of deaths

    //printing result

    cout<<"State\tDate\tCases\tDeaths\n";
    trav(data, result){
        cout<<data.state<<" "<<data.date<<" "<<data.cases<<"\t"<<data.deaths<<endl;
    }
    cout<<endl;

    result = Data_of_each_state_on_date(result_number, "01-01-2020"); //get the data on the particular date

    //sort the data
    sort(all(result), cmp);

    //pop_back until result size is equals to the required result size
    while(result.size() > result_number) result.pop_back();

    //printing result
    cout<<"State\tDate\tCases\tDeaths\n";
    trav(data, result){
        cout<<data.state<<" "<<data.date<<" "<<data.cases<<"\t"<<data.deaths<<endl;
    }
    cout<<endl;

    return 0;
}