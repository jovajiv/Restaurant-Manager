#include <iostream>
#include "Action.h"
#include "Table.h"
#include "Restaurant.h"
#include "Customer.h"
#include <sstream>
using namespace std;

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

BaseAction::BaseAction(): errorMsg(""),status(),ActionType("") {} //Should we make the status pending each time we open a new Action or leave it empty?

ActionStatus BaseAction::getStatus() const {
    return  status;
}
void BaseAction::complete() {
    status = COMPLETED;
}
string BaseAction::getErrorMsg() const {
    return errorMsg;
}
void BaseAction::error(std::string errorMsg) {
    status = ERROR;
    this->errorMsg = errorMsg;


}
string BaseAction::StatusAndErrorMsg() const{
    if(status == PENDING) return "Pending";
    else if(status == COMPLETED) return "Completed";
    else if(status == ERROR) return string("Error: ")+ errorMsg;
    else return "";
}
void BaseAction::setType(std::string type) {
    ActionType = type;
}
std::string BaseAction::getType() const {
    return ActionType;
}
void BaseAction::setStatus(const ActionStatus & Other_status) {
    this->status = Other_status;
}

BaseAction::~BaseAction(){}


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

OpenTable::OpenTable(int id, vector<Customer*> &customersList): tableId(id), customers(customersList) {
    this->setType("open");
}


OpenTable::OpenTable(const OpenTable &other): tableId(other.getId()) ,customers() { //copy constructor
    Customer* cust;
    for (unsigned int i=0; i<other.customers.size(); i++) { //adding the customers to the table, by checking the type
        cust=other.customers.at(i);
        if(cust->getType()=="VEG")
            customers.push_back(new VegetarianCustomer(*cust));
        else if (cust->getType()=="CHP")
            customers.push_back(new CheapCustomer(*cust));
        else if (cust->getType()=="SPC")
            customers.push_back(new SpicyCustomer(*cust));
        else if (cust->getType()=="ALC")
            customers.push_back(new AlchoholicCustomer(*cust));
    }
    this->setType("open");
    this->setStatus(other.getStatus());
    if (other.getStatus()==ERROR)
    this->error(other.getErrorMsg());

}
int OpenTable::getId() const { //added by us 14.11
    return this->tableId;
}
void OpenTable::act(Restaurant &restaurant) {
    this->setStatus(PENDING);
    Table * table = restaurant.getTable(tableId);
    Customer *cust;
    if (table == nullptr || table->isOpen() ) {              //check if table does not exist or already open FLAG - WHY TABLE ALWAYS NOT NULLPOINTER? DID NULLPTR IN rest.getTable()
        error("Table does not exist or is already open");
        cout << StatusAndErrorMsg() << endl;                      //printing the error as needed
    }
    else {
        table->openTable();
        for (unsigned int i=0; i<customers.size(); i++) {           //adding the customers to the table. FLAG - WHAT IF CANT ADD MORE CUSTOMER TO THE TABLE????
            cust=customers.at(i);
            if(cust->getType()=="VEG")
                table->addCustomer(new VegetarianCustomer(*cust));
            else if (cust->getType()=="CHP")
                table->addCustomer(new CheapCustomer(*cust));
            else if (cust->getType()=="SPC")
                table->addCustomer(new SpicyCustomer(*cust));
            else if (cust->getType()=="ALC")
                table->addCustomer(new AlchoholicCustomer(*cust));
        }
        complete();
    }
}


std::string OpenTable::toString() const {

    string msg= "open " + to_string(tableId) + " ";
    for (unsigned int i=0;i<customers.size();i++) {
        msg+= customers.at(i)->toString() + " ";
    }
    msg+=this->StatusAndErrorMsg();
    return msg;

}

OpenTable::~OpenTable(){ //destructor
    for (unsigned int i=0;i<customers.size();i++)
        delete(customers.at(i));
}

/* OpenTable& OpenTable::operator=(const OpenTable& other) { not needed - FAQ
    if(this!=&other)
        return *Clone();
    return *this;

   if (other.getStatus()==ERROR)
        this->error(other.getErrorMsg());
    this->setStatus(other.getStatus());
    this->customers = other.customers; //warning!! same pointers?
    this->setType(other.getTy   pe());


}
*/

OpenTable::OpenTable(OpenTable&& other):tableId(other.getId()) ,customers() { // SHAI 18.11
    this->setType("open");
    this->setStatus(other.getStatus());
    if (other.getStatus()==ERROR)
        this->error(other.getErrorMsg());
    for (unsigned int i=0; i<other.customers.size(); i++) { // other pointers are now yours
        customers.push_back(other.customers.at(i));
    }
    for (unsigned int i=0; i<other.customers.size(); i++) { // other pointers are now nullptr.
        other.customers.at(i) = nullptr;
    }

}

//OpenTable& OpenTable::operator=(OpenTable&& other) { //SHAI 18.11 - not needed, FAQ
//    if(this!=&other)
//        return *Clone();
//    return *this;
//}

OpenTable* OpenTable::Clone() {
    vector<Customer*> cloneList;
    Customer* cust;
    for (unsigned int i=0; i<customers.size(); i++) {           //adding the customers to the table. FLAG - WHAT IF CANT ADD MORE CUSTOMER TO THE TABLE????
        cust=customers.at(i);
        if(cust->getType()=="VEG")
            cloneList.push_back(new VegetarianCustomer(*cust));
        else if (cust->getType()=="CHP")
            cloneList.push_back(new CheapCustomer(*cust));
        else if (cust->getType()=="SPC")
            cloneList.push_back(new SpicyCustomer(*cust));
        else if (cust->getType()=="ALC")
            cloneList.push_back(new AlchoholicCustomer(*cust));
    }
    OpenTable* clo=new OpenTable(tableId,cloneList); //here we need to sent the "this" status.
    clo->setType("open"); //send to the clone the type.
    clo->setStatus(this->getStatus()); //sending to the clone the status
    if (clo->getStatus()==ERROR)
        clo->error(this->getErrorMsg()); //if it's an ERROR, send the error msg.
    cloneList.clear();
    return clo;

}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Order::Order(int id):tableId(id){
    this->setType("order");
}
Order::Order(const Order &other): tableId((other).tableId) {                                            /////// warning , it let us get private arguments for free !!!!!, recheck later.
    this->setType("order");
    this->setStatus(other.getStatus());
    if (other.getStatus()==ERROR)
    this->error(other.getErrorMsg());
}
void Order::act(Restaurant &restaurant){
    this->setStatus(PENDING);
    Table * table = restaurant.getTable(tableId);

    if (table == nullptr|| !(table->isOpen()) ) {              //check if table does not exist or already open FLAG - WHY TABLE ALWAYS NOT NULLPOINTER? DID NULLPTR IN rest.getTable()
        error("Table does not exist or is not open");
        cout << StatusAndErrorMsg() << endl;                      //printing the error as needed
    }
    else {
        table->order(restaurant.getMenu());
        complete();
    }


}
Order* Order::Clone() {
    return new Order(*this);
}
std::string Order::toString() const{
    string msg= "order " + to_string(tableId) + " " + this->StatusAndErrorMsg();
    return msg;

}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

MoveCustomer::MoveCustomer(int src, int dst, int customerId): srcTable(src), dstTable(dst), id(customerId) {
    this->setType("move");
} // constructor
MoveCustomer::MoveCustomer(const MoveCustomer &other): srcTable((other).srcTable),dstTable((other).dstTable),id(other.id) {                                            /////// warning , it let us get private arguments for free !!!!!, recheck later.
    this->setType("move");
    this->setStatus(other.getStatus());
    if (other.getStatus()==ERROR)
    this->error(other.getErrorMsg());
}
void MoveCustomer::act(Restaurant &restaurant){
    this->setStatus(PENDING);
    Table * src_table = restaurant.getTable(srcTable);
    Table * dst_table = restaurant.getTable(dstTable);
    vector <OrderPair> & src_orders = src_table->getOrders();
    vector <OrderPair>  new_src_orders;     // will be deleted in the end of the scope.
    vector <OrderPair> & dst_orders = dst_table->getOrders();
    Customer * cust = src_table->getCustomer(id);

    if (srcTable >= restaurant.getNumOfTables()|| dstTable >= restaurant.getNumOfTables() || !(src_table->isOpen())
        || !(dst_table->isOpen()) || dst_table->getCapacity() <= dst_table->getCustomers().size() || cust == nullptr ) {
        error("Cannot move customer"); //src or dst table doesnt exist (id>= num of table, or one of the is closed), closed or dst if full
        cout << StatusAndErrorMsg() <<endl;
    }

    else {
        src_table->removeCustomer(id); //remove cust from src table
        dst_table->addCustomer(cust);  //add him to dst table
        for (unsigned int i=0; i<src_orders.size(); i++) {
            if (src_orders.at(i).first == id) { //the dish was ordered by our moving customer
                OrderPair dish(src_orders.at(i)); //the dish pair we ant to move
                src_table->change_bill_if_move(src_table->getBill() - dish.second.getPrice());  // remove from src table bill the cost of the dish that is moving
                dst_table->change_bill_if_move(dst_table->getBill() + dish.second.getPrice());  // add to dst table bill the cost of the dish that is moving
                dst_orders.push_back(dish); //move the dish itself
            }
        }
        for (unsigned int i=0; i<src_orders.size(); i++) {
            if (src_orders.at(i).first != id) { //the dish WASN'T ordered by our moving customer
                new_src_orders.push_back(src_orders.at(i)); //push into the new src orders vector
            }
        }
        src_table->change_orderList(new_src_orders); //change the original src orders to the new one, the one without the moving customer orders.
        if (src_table->getCustomers().size()==0) {
            src_table->closeTable();
        }
        complete();
    }
}
MoveCustomer* MoveCustomer::Clone() {
    return new MoveCustomer(*this);
}

string MoveCustomer::toString() const {
    string msg= "move " + to_string(srcTable) + " " + to_string(dstTable) + " " + to_string(id) + " " + this->StatusAndErrorMsg();

    return msg;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Close::Close(int id): tableId(id) {
    this->setType("close");
} //constructor
Close::Close(const Close &other): tableId((other).tableId) {                                            /////// warning , it let us get private arguments for free !!!!!, recheck later.
    this->setType("close");
    this->setStatus(other.getStatus());
    if (other.getStatus()==ERROR)
    this->error(other.getErrorMsg());
}
void Close::act(Restaurant &restaurant) {
    this->setStatus(PENDING);
    Table * table = restaurant.getTable(tableId);
    if (table == nullptr|| !(table->isOpen()) ) { //table is closed or does not exist
        error("Table does not exist or is not open");
        cout << StatusAndErrorMsg() << endl;                      //printing the error as needed
    }
    else {
        cout << "Table " << tableId << " was closed. Bill " << table->getBill() << "NIS" << endl;
        table->closeTable();
        complete();
    }
}
Close* Close::Clone() {
    return new Close(*this);
}
string Close::toString() const {
    string msg= "close " + to_string(tableId) + " " + this->StatusAndErrorMsg();
    return msg;

}


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

PrintMenu::PrintMenu() {
    this->setType("menu");
} //empty constructor, no need for something else.
PrintMenu::PrintMenu(const PrintMenu &other) {
    this->setType("menu");
    this->setStatus(other.getStatus());
}
void PrintMenu::act(Restaurant &restaurant) {
    this->setStatus(PENDING);
    vector <Dish> &print_menu = restaurant.getMenu();
    for (unsigned int i=0; i<restaurant.getMenu().size(); i++) {
        cout << print_menu.at(i).getName() << " " << Convert_DishType_to_string (print_menu.at(i).getType()) << " " << print_menu.at(i).getPrice() <<"NIS" << endl;
    }
    complete();
}
string PrintMenu::Convert_DishType_to_string(const DishType &type){ //to convert in menu printing type to string.
    if(type == VEG) return "VEG";
    else if(type == SPC) return "SPC";
    else if(type == BVG) return "BVG";
    else if(type == ALC) return "ALC";
    else return "";
}
PrintMenu* PrintMenu::Clone() {
    return new PrintMenu(*this);
}
string PrintMenu::toString() const {
    string msg= "menu " + this->StatusAndErrorMsg();
    return msg;

}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

CloseAll::CloseAll() {
    this->setType("closeall");
}
CloseAll::CloseAll(const CloseAll &other) {
    this->setType("closeall");
    this->setStatus(other.getStatus());
}
void CloseAll::act(Restaurant &restaurant) { //same as close, but in "for" loop for all tables.
    this->setStatus(PENDING);
    Table * table;
    for (int i=0; i<restaurant.getNumOfTables(); i++) {
        table = restaurant.getTable(i);
        if (table->isOpen()) {
            cout << "Table " << i << " was closed. Bill " << table->getBill() << "NIS" << endl;
            table->closeTable();
        }
    }
    complete();

}
CloseAll* CloseAll::Clone() {
    return new CloseAll(*this);
}
string CloseAll::toString() const {
 return "";
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

PrintTableStatus::PrintTableStatus(int id): tableId(id) {
    this->setType("status");
}
PrintTableStatus::PrintTableStatus(const PrintTableStatus &other): tableId(other.tableId){
    this->setType("status");
    this->setStatus(other.getStatus());
}
void PrintTableStatus::act(Restaurant &restaurant) {
    this->setStatus(PENDING);
    Table * table = restaurant.getTable(tableId);
    if (table == nullptr) {
        complete();
    }
    else if (!table->isOpen()) {
        cout << "Table " << tableId << " status: closed" << endl;
    }
    else {
        cout << "Table " << tableId << " status: open" << endl;
        cout << "Customers:" << endl;
        vector <Customer*> & cust = table->getCustomers();
        for (unsigned int i=0; i<cust.size(); i++) {             //print all customers in the table, id followed by name
            cout << cust.at(i)->getId() << " " << cust.at(i)->getName() << endl;
        }
        cout << "Orders:" << endl;
        vector <OrderPair> & ord = table->getOrders();
        for (unsigned int i=0; i<ord.size(); i++) {
            cout << ord.at(i).second.getName() << " " << ord.at(i).second.getPrice() << "NIS " << ord.at(i).first << endl; // print all dish name, than dish price, than id of person to order.
        }
        cout << "Current Bill: " << table->getBill() << "NIS" << endl; //prints bill.
    }
    complete();
}
PrintTableStatus* PrintTableStatus::Clone() {
    return new PrintTableStatus(*this);
}
string PrintTableStatus::toString() const {
    string msg= "status " + to_string(tableId) + " " + this->StatusAndErrorMsg();
    return msg;

}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

PrintActionsLog::PrintActionsLog(){
    this->setType("log");
}
PrintActionsLog::PrintActionsLog(const PrintActionsLog &other){
    this->setType("log");
    this->setStatus(other.getStatus());
}
void PrintActionsLog::act(Restaurant &restaurant){
    this->setStatus(PENDING);
    for (unsigned int i=0; i<restaurant.getActionsLog().size(); i++){ //call the "toString" of each action to print it in log
        cout << restaurant.getActionsLog().at(i)->toString() << endl;
    }
    complete();

}
PrintActionsLog* PrintActionsLog::Clone() {
    return new PrintActionsLog(*this);
}
std::string PrintActionsLog::toString() const{
    string msg= "log " + this->StatusAndErrorMsg();
    return msg;

}


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

BackupRestaurant::BackupRestaurant() {
    this->setType("backup");
} //empty constructor
BackupRestaurant::BackupRestaurant(const BackupRestaurant &other){
    this->setType("backup");
    this->setStatus(other.getStatus());
}
void BackupRestaurant::act(Restaurant &restaurant) {
    this->setStatus(PENDING);
    if(backup!= nullptr) //check if first back up. if not - delete the old one
        delete(backup);
    backup = new Restaurant(restaurant); //creat the backup - copy constructor of restaurant
    complete();
}

string BackupRestaurant::toString() const {
    string msg= "backup "  + this->StatusAndErrorMsg();
    return msg;

}
BackupRestaurant* BackupRestaurant::Clone() {
    return new BackupRestaurant(*this);
}
BackupRestaurant::~BackupRestaurant(){
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

RestoreResturant::RestoreResturant() {
    this->setType("restore");
} //empty constructor
RestoreResturant::RestoreResturant(const RestoreResturant &other) {
    this->setType("restore");
    this->setStatus(other.getStatus());
    if (other.getStatus()==ERROR)
        this->error(other.getErrorMsg());
}
RestoreResturant* RestoreResturant::Clone() {
     return  new RestoreResturant(*this);
}
void RestoreResturant::act(Restaurant &restaurant) {
   this->setStatus(PENDING);
   if (backup == nullptr) {
       error("No backup available");
       cout << StatusAndErrorMsg() << endl;
   }
   else {
       restaurant = *backup; //the restaurant is now "the backup" one.
       complete();
   }
}

string RestoreResturant::toString() const {
    string msg= "restore "  + this->StatusAndErrorMsg();
    return msg;

}