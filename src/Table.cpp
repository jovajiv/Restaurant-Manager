//
// Created by yoavgi@wincs.cs.bgu.ac.il on 11/5/18.
//
// need to complete "order" function.
#include <string>
#include <algorithm>    // std::find
#include "Table.h"
#include <iostream>
using namespace std;


Table::Table(int t_capacity): capacity(t_capacity), open(false),billAmount(0),customersList(),orderList() {}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Rule of five

Table::~Table() {
    for (unsigned int i=0;i<customersList.size();i++)
        delete customersList.at(i);
    orderList.clear();
    customersList.clear();
}

Table::Table(const Table &other): capacity(other.capacity),open(other.open),billAmount(other.billAmount),customersList(),orderList() {
    Customer* cust;
    for (unsigned int i=0; i<other.customersList.size(); i++) {           //adding the customers to the table.
        cust = other.customersList.at(i);
        if (cust->getType() == "VEG")
            this->addCustomer(new VegetarianCustomer(*cust));
        else if (cust->getType() == "CHP")
            this->addCustomer(new CheapCustomer(*cust));
        else if (cust->getType() == "SPC")
            this->addCustomer(new SpicyCustomer(*cust));
        else if (cust->getType() == "ALC")
            this->addCustomer(new AlchoholicCustomer(*cust));
    }
    orderList.clear(); //A list of pairs for each order in a table - (customer_id, Dish)
    for (unsigned int i=0;i<other.orderList.size();i++){
        orderList.push_back(other.orderList.at(i));
    }

}

Table& Table::operator=(const Table& other){
    unsigned int i=0;
    capacity=other.capacity;
    capacity=other.capacity;
    open=other.open;
    billAmount=other.billAmount;
    for (i=0;i<customersList.size();i++)                //delete customers on "current" restaurant from heap
        delete customersList.at(i);
    for (i=0;i<other.customersList.size();i++){             // create a copy of the customers from the "other" table and assign them to the "current" table.
        Customer* cust=other.customersList.at(i);
        if(cust->getType()=="VEG")
            this->addCustomer(new VegetarianCustomer(*cust));
        else if (cust->getType()=="CHP")
            this->addCustomer(new CheapCustomer(*cust));
        else if (cust->getType()=="SPC")
            this->addCustomer(new SpicyCustomer(*cust));
        else if (cust->getType()=="ALC")
            this->addCustomer(new AlchoholicCustomer(*cust));
    }

    orderList.clear(); //A list of pairs for each order in a table - (customer_id, Dish)
    for (unsigned i=0;i<other.orderList.size();i++){
        orderList.push_back(other.orderList.at(i));
    }
    return *this;
}

Table::Table(Table&& other): capacity(other.getCapacity()), open(other.isOpen()), billAmount(other.getBill()),customersList(),orderList() {
    for (unsigned int i=0; i<other.customersList.size(); i++) { // other pointers are now yours
        customersList.push_back(other.customersList.at(i));
    }
    for (unsigned int i=0; i<other.customersList.size(); i++) { // other pointers are now nullptr.
        other.customersList.at(i) = nullptr;
    }
    for (unsigned int i=0; i<other.orderList.size(); i++) { // other orders are now yours
        orderList.push_back(other.orderList.at(i));
    }
    other.customersList.clear();
    other.orderList.clear();

}

Table& Table::operator=(Table&& other) {//move assignment
    capacity = other.getCapacity();
    open = other.isOpen();
    billAmount = other.getBill();
    for (unsigned int i=0; i<customersList.size(); i++) {
        delete(customersList.at(i));
    }
    orderList.clear();
    for (unsigned int i=0; i<other.orderList.size(); i++) {
        OrderPair to_push(other.orderList.at(i).first,other.orderList.at(i).second);
        orderList.push_back(to_push);
    }
    for (unsigned int i=0; i<other.customersList.size(); i++) {
        this->addCustomer(other.customersList[i]);
        other.customersList[i] = nullptr;
    }
    other.customersList.clear();
    other.orderList.clear();
    return *this;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

unsigned int Table::getCapacity() const{
    return capacity;
}
void Table::addCustomer(Customer* customer){            // add customer if there is enough room at table , do not look if table is open or closed.
    if (capacity>customersList.size())
        customersList.push_back(customer);

}
void Table::removeCustomer(int id){
    int index=getCustomerLocation(id);
    if(index!=-1){
        customersList.erase(customersList.begin() + index );
    }
}
int Table::getCustomerLocation(int id){                 // return Customer location in vector, if customer not present , return -1.
    int i=0;
    vector<Customer*> ::iterator it;  // declare an iterator to a vector of strings
    for(it = customersList.begin(); it != customersList.end(); it++,i++ )    {
        if((*it)->getId() == id) {
            return i;

        }
    }
    return -1;

}
Customer* Table::getCustomer(int id){

    int index=0;
    index=getCustomerLocation(id);
    if(index !=-1){
        return customersList.at(index);
    }
    return nullptr;

}
vector<Customer*>& Table::getCustomers(){
    return customersList;

}
std::vector<OrderPair>& Table::getOrders(){
    return orderList;

}
void Table::order(const std::vector<Dish> &menu){           // needs to print ordered dishes.
    vector<int> customer_orders;
    Customer* customer;
    int customerid;
    if (menu.empty())
        return ;
    for(unsigned int i=0;i<customersList.size();i++) {
        customer=customersList.at(i);
        customerid=customer->getId();
        customer_orders = customer->order(menu);
        for(unsigned int j=0;j<customer_orders.size(); j++) {
            Dish  dish=menu.at(customer_orders.at(j));
            orderList.push_back(make_pair(customerid, dish));
            cout << customer->getName() << " ordered " << dish.getName() << endl;
            billAmount+=dish.getPrice();

        }

    }

}

void Table::openTable(){
    open=true;
    billAmount = 0; //when new table is open the bill amount will set to 0.

}
void Table::closeTable(){
    open = false;
    billAmount = 0; //just for safety, when closed = table bill will set to 0.
for (int i=customersList.size()-1;i>=0;i--){
        delete(customersList.at(i));
        customersList.pop_back();           // pop back removes the poped customers from memory. calls their destructor.

    }
    orderList.clear();

}

int Table::getBill(){
    return billAmount;
}

bool Table::isOpen(){
    return open;

}

void Table::change_bill_if_move(int new_bill) {
    billAmount = new_bill;
}

void Table::change_orderList(vector <OrderPair> & other) {
    this->orderList.clear();
    for (unsigned int i=0; i<other.size(); i++) {
        orderList.push_back(other.at(i));
    }
}



