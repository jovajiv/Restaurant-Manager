//
// Created by yoavgi@wincs.cs.bgu.ac.il on 11/6/18.
//

#include "Restaurant.h"
#include <sstream>
using namespace std;


Restaurant::Restaurant(): open(false), tables(), menu(),actionsLog() { } // empty constructor, putted false because we'll open with "void open()" function

Restaurant::Restaurant(const std::string &configFilePath): open(), tables(), menu(),actionsLog(){

    ReadConfile(configFilePath);
}



void Restaurant::start() {
    open = true;
    string user_Input,command;
    int customerIndex=0;
    vector<int> moveVec;

    cout << "Restaurant is now open!" << endl;

    while (open){
        getline(cin, user_Input);
        size_t pos = user_Input.find(" ");
        string command = user_Input.substr (0,pos);     // get from "live" to the end
        int command_num=Convert_command_toInt(command);
        switch (command_num){
            case 1:                  //open //add user id
            {
                vector<Customer *> list = OpenTableParser(user_Input, customerIndex);
                OpenTable *table = new OpenTable(IdParser(user_Input),list);              // creates action object opentable
                table->act(*this);                                                      //executes opentable
                actionsLog.push_back(table);                                            // stores action object opentable in log Vector.
            }
                break;
            case 2: {               // order
                Order *ord =new Order(IdParser(user_Input));
                ord->act(*this);
                actionsLog.push_back(ord);
            }

                break;
            case 3:             // move
            {
                moveVec = MoveParser(user_Input);
                MoveCustomer *move = new MoveCustomer(moveVec.at(0), moveVec.at(1), moveVec.at(2));
                move->act(*this);
                actionsLog.push_back(move);
            }
                break;
            case 4:              // close
            {
                Close *cls = new Close(IdParser(user_Input));
                cls->act(*this);
                actionsLog.push_back(cls);
            }
                break;
            case 5:                                             //closeall
                CloseAll().act(*this);
                open=false;                  // need to free objects in actionslog , multiple objects there.

                break;
            case 6:            //  menu
            {
                PrintMenu *menuprint = new PrintMenu();
                menuprint->act(*this);
                actionsLog.push_back(menuprint);
            }
                break;
            case 7:      // status
            {
                PrintTableStatus *tableStat = new PrintTableStatus(IdParser(user_Input));
                tableStat->act(*this);
                actionsLog.push_back(tableStat);
            }

                break;
            case 8:       // log
            {
                PrintActionsLog *Log = new PrintActionsLog();
                Log->act(*this);
                actionsLog.push_back(Log);
            }

                break;
            case 9:      // backup
            {
                BackupRestaurant *bckup=new BackupRestaurant();
                bckup->act(*this);
                actionsLog.push_back(bckup);
            }
                break;
            case 10:    // restore
            {
                RestoreResturant *restore=new RestoreResturant();
                restore->act(*this);
                actionsLog.push_back(restore);
            }
                break;
        }

    }

}

int Restaurant::getNumOfTables() const {
    return tables.size();
}
Table* Restaurant::getTable(int ind) { //CHANGED by Shai, 10.11.18 - Action open table, Return null if table does not exist
    if (ind>=(int)tables.size() || ind<0) { // if table does not exist we'll return null pointer.
        return nullptr;
    }
    return tables.at(ind); //id's are given by insert order, so when we get id we'll return table.at(id) - the place of them in the vector
}
const std::vector<BaseAction*>& Restaurant::getActionsLog() const { // Return a reference to the history of actions
    return actionsLog;
}
std::vector<Dish>& Restaurant::getMenu() {
    return menu;
}

// parses the config file.
void Restaurant::ReadConfile(const std::string &configFilePath) { //reading the file into the Restaurant
    string line;
    int id (0);//id counter
    int index=0;// line 1,2....eof
    ifstream myfile (configFilePath);
    if (myfile.is_open())
    {
        while ( getline (myfile,line) )
        {
            if(line != "\r" && line.length()>0 && line.at(0)!='#') { //skipping white space or commends
                if(index==0) { //num of tables - line 1 only
                }                       // we discovered there is no need in reading the number of tables , since its given in the next line.
                else if(index==1) {
                    CreateTables(line);
                }
                else {
                    CreateDish(line,id);
                    id++; // advance id (i=id) by one, so we won't get same id's to different dishes.
                }
                index++;
            }

        }
        myfile.close();
    }

    else cout << "Unable to open file";

}



void Restaurant::CreateTables(string &capacity){    // get tables size and put them - line 2 only
    stringstream streamLine;
    streamLine.str(capacity); // changed from << to .str(...)
    while(getline(streamLine,capacity,',')) {
        Table *T = new Table(stoi(capacity));
        tables.push_back(T);
    }
}
// parses a string line representing a dish
void Restaurant::CreateDish(string &line,int id){    //menu, line 3 to the end , called multiple times (called in while loop)
    string dishName,dishType,price;
    u_long subPlaceNow (0);//to substring
    dishName = line.substr(subPlaceNow,line.find_first_of(',',subPlaceNow)); //from 0 to the first ','
    subPlaceNow = line.find_first_of(',',subPlaceNow+1);
    dishType = line.substr(subPlaceNow+1,3); //from the first ',' 3 Letter (TYPE is always 3 letters)
    subPlaceNow = line.find_first_of(',',subPlaceNow+1);
    price = line.substr(subPlaceNow+1); //from the second ',' to the end (price + '\r')
    if (price.back() == '\r') {
        price.pop_back(); // '\r' in the end that we need to pop
    }
    subPlaceNow=0; //reset sub Place!!!!!!!!!
    Dish dish(id, dishName, stoi(price), Convert(dishType)); //changed from new to not lose memory 13.11.18

    menu.push_back(dish);

}

DishType Restaurant::Convert(const string &str){
    if(str == "VEG") return VEG;
    else if(str == "SPC") return SPC;
    else if(str == "BVG") return BVG;
    else  return ALC;

}

// used in rest for switchcase
int Restaurant::Convert_command_toInt(const string &str){
    if      (str == "open") return 1;           //
    else if (str == "order") return 2;
    else if (str == "move") return 3;
    else if (str == "close") return 4;          //
    else if (str == "closeall") return 5;
    else if (str == "menu") return 6;           //
    else if (str == "status") return 7;         //
    else if (str == "log") return 8;
    else if (str == "backup") return 9;
    else if (str == "restore") return 10;
    else return 0;
}


// parses open table input from user.
vector<Customer*> Restaurant::OpenTableParser(string line,int &customerIndex){
    string customerName,customerType;
    vector<Customer*> customerList;

    int Spos=5;         //Starting position is 5 to receive the ID of table , no need to read the word open.
    int Epos=line.find_first_of(' ',Spos);        //Ending position is init to this to find the end of the ID ,

//    int id= stoi(line.substr(Spos,Epos-Spos));          // reads the table id

    while (Epos>0){                                     // recoursivly read Customer names and types
        Spos=Epos+1;
        Epos=line.find_first_of(',',Spos);
        customerName=line.substr(Spos,Epos-Spos);

        Spos=Epos+1;
        Epos=line.find_first_of(' ',Spos);
        if (Epos>0){                                    // problem when reaching end of String , Epos becomes negetive and reiterate over the word, if statement put in place to prevent.
            customerType=line.substr(Spos,Epos-Spos);
        }
        else{
            customerType=line.substr(Spos,line.size());
        }


        if(customerType=="veg")
            customerList.push_back(new VegetarianCustomer(customerName,customerIndex));
        else if(customerType=="chp")
            customerList.push_back(new CheapCustomer(customerName,customerIndex));
        else if(customerType=="alc")
            customerList.push_back(new AlchoholicCustomer(customerName,customerIndex));
        else if(customerType=="spc")
            customerList.push_back(new SpicyCustomer(customerName,customerIndex));

        customerIndex++;
    }

//    OpenTable(id,customerList).act(*this);
    return customerList;

}

int Restaurant::IdParser(string line){
    int Spos=0;         //Starting position is 5 to receive the ID of table , no need to read the word open.
    int Epos=line.find_first_of(' ',Spos);        //Ending position is init to this to find the end of the ID ,
    Spos=Epos+1;
    Epos=line.size();

    int id= stoi(line.substr(Spos,Epos-Spos));          // reads the table id

    return id;
}

// parsing a move command received from user as input.
vector<int> Restaurant::MoveParser(string line){
    int Spos=5;         //Starting position is 5 to receive the ID of table , no need to read the word "move".
    int Epos=line.find_first_of(' ',Spos);        //Ending position is init to this to find the end of the ID ,
    int source= stoi(line.substr(Spos,Epos-Spos));
    Spos=Epos+1;
    Epos=line.find_first_of(' ',Spos);        //Ending position is init to this to find the end of the ID ,
    int destination= stoi(line.substr(Spos,Epos-Spos));
    Spos=Epos+1;
    Epos=line.size();       //Ending position is init to this to find the end of the ID ,
    int customerid= stoi(line.substr(Spos,Epos-Spos));


    return vector<int> {source,destination,customerid};
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//RULE OF FIVE
// each object is responsible of its own memory , therefor bot tables and actionslog must delete their memory. , no need to activly delete menu vector as it has no pointers.
Restaurant::~Restaurant(){ //destructor
    for (unsigned int i=0;i<tables.size();i++)
        delete tables.at(i);
  //      tables.at(i)->closeTable();
  //  tables.clear();
    for (unsigned int i=0;i<actionsLog.size();i++)
        delete actionsLog.at(i);
  //  actionsLog.clear();
    //menu.clear();


}

Restaurant::Restaurant(const Restaurant &other): open(other.open), tables(), menu(),actionsLog() { //copy constructor
    string command;

    for (unsigned int i = 0; i<other.tables.size(); i++) {
        tables.push_back(new Table(*(other.tables.at(i))));
    }

    for(unsigned int i = 0; i<other.actionsLog.size(); i++) {
        command = other.actionsLog.at(i)->getType();
        switch (Convert_command_toInt(command)) {
            case 1: {
                actionsLog.push_back(new OpenTable(*((OpenTable*)other.actionsLog.at(i))));

            }
                break;
            case 2: {
                actionsLog.push_back(new Order(*((Order*)other.actionsLog.at(i))));
            }
                break;
            case 3: {
                actionsLog.push_back(new MoveCustomer(*((MoveCustomer*)other.actionsLog.at(i))));
            }
                break;
            case 4: {
                actionsLog.push_back(new Close(*((Close*)other.actionsLog.at(i))));
            }
                break;
            case 5: {
                actionsLog.push_back(new CloseAll(*((CloseAll*)other.actionsLog.at(i))));
            }
                break;
            case 6: {
                actionsLog.push_back(new PrintMenu(*((PrintMenu*)other.actionsLog.at(i))));
            }
                break;
            case 7: {
                actionsLog.push_back(new PrintTableStatus(*((PrintTableStatus*)other.actionsLog.at(i))));
            }
                break;
            case 8: {
                actionsLog.push_back(new PrintActionsLog(*((PrintActionsLog*)other.actionsLog.at(i))));
            }
                break;
            case 9: {
                actionsLog.push_back(new BackupRestaurant(*((BackupRestaurant*)other.actionsLog.at(i))));
            }
                break;
            case 10: {
                actionsLog.push_back(new RestoreResturant(*((RestoreResturant*)other.actionsLog.at(i))));
            }
                break;

        }
    }

    for (unsigned int i = 0; i<other.menu.size(); i++)
        menu.push_back(other.menu.at(i));


}


Restaurant& Restaurant::operator=(const Restaurant& other){ //assignment operator
    open=other.open;
    unsigned int i = 0;

    menu.clear();
    for (i = 0; i < other.menu.size(); i++) { //??? your dish is other dish
        menu.push_back(other.menu.at(i));
    }

    for (i = 0; i<tables.size(); i++) {
        delete(tables.at(i)); // deleting ourself and free the memory
    }
    tables.clear();
    for (i = 0; i<other.tables.size(); i++) {
        tables.push_back(new Table(*other.tables.at(i))); //creating new, so they are not connected!
    }

    for (i = 0; i<actionsLog.size(); i++) {
        delete(actionsLog.at(i)); // deleting ourself and free the memory
    }
    actionsLog.clear();
    for (i = 0; i<other.actionsLog.size(); i++) {
        actionsLog.push_back(other.actionsLog.at(i)->Clone()); //clone actionlog, so they are not connected!
    }

    return *this;

}

Restaurant::Restaurant(Restaurant&& other): open(other.open), tables(), menu(),actionsLog() { //move constructor

    for (unsigned int i = 0; i < other.menu.size(); i++) { //??? your dish is other dish
        menu.push_back(other.menu.at(i));
    }
    other.menu.clear();

    for (unsigned int i = 0; i < other.tables.size(); i++) { //???your pointers are other pointers.
        tables.push_back(other.tables.at(i));
    }
    for (unsigned int i = 0; i < other.tables.size(); i++) { //??? now "other" pointers are nullptr
        other.tables.at(i) = nullptr;
    }
    other.tables.clear();
    for (unsigned int i = 0; i < other.actionsLog.size(); i++) { //??? your pointers are other pointers.
        actionsLog.push_back(other.actionsLog.at(i));
    }
    for (unsigned int i = 0; i < other.actionsLog.size(); i++) { //??? now "other" pointers are nullptr
        other.actionsLog.at(i) = nullptr;
    }
    other.actionsLog.clear();
}

Restaurant& Restaurant::operator=(Restaurant&& other) {
    open = other.open;
    for (unsigned int i = 0; i < other.menu.size(); i++) { //??? your dish is other dish
        menu.push_back(other.menu.at(i));
    }
    other.menu.clear();

    for (unsigned int i = 0; i < other.tables.size(); i++) { //???your pointers are other pointers.
        tables.push_back(new Table(*other.tables.at(i)));
    }
    for (unsigned int i = 0; i < other.tables.size(); i++) { //??? now "other" pointers are nullptr
        other.tables.at(i) = nullptr;
    }
    other.tables.clear();
    for (unsigned int i = 0; i < other.actionsLog.size(); i++) { //??? your pointers are other pointers.
        actionsLog.push_back(other.actionsLog.at(i)->Clone());
    }
    for (unsigned int i = 0; i < other.actionsLog.size(); i++) { //??? now "other" pointers are nullptr
        other.actionsLog.at(i) = nullptr;
    }
    other.actionsLog.clear();
    return *this;
}