//
// Created by yoavgi@wincs.cs.bgu.ac.il on 11/6/18.
//


#include "Customer.h"
#include <iostream>
using namespace std;




Customer::Customer(string c_name, int c_id): name(c_name),id(c_id),isFirstOrder(true),failedToOrder(false),orders(),CustomerType() {}
string Customer::getName() const{
    return name;
}
int Customer::getId() const{
    return id;
}
bool Customer::getIsFirstOrder() const{
    return isFirstOrder;
}
void Customer::setIsFirstOrder(bool val) {
    isFirstOrder=val;
}
bool Customer::getFailedToOrder() const{
    return failedToOrder;
}
void Customer::setFailedToOrder(bool val) {
    failedToOrder=val;
}
void Customer::setOrders(std::vector<int> customers){
    orders=customers;
}
void Customer::setType(string type){
    CustomerType=type;
}
string Customer::getType() const{
    return CustomerType;
}
std::vector<int>& Customer::getOrders(){
    return orders;
}


//--------------Rule of 5-----------------///

Customer::~Customer(){
}



VegetarianCustomer::VegetarianCustomer(string name, int id): Customer::Customer(name,id) {
    this->setType("VEG");

}               // inherits the customer constractor

// we  collect all expected future orders just onced,
// and then, each time we reorder, we use the same vector we orignally created when we traversed the menu, and depending on startegy , we pop elements".
vector<int> VegetarianCustomer::order(const std::vector<Dish> &menu){               // Vegi orders smalles ID vegi dish , and lowest cost non-alcholic bev, if no vegi dish , he orders nothing.
    vector<int> orders;



    if(!getFailedToOrder())         // if didnt fail to order ,execute
        if(getIsFirstOrder()) {        // if didnt order at all so far, execute
            orders=orderFirst(menu);
            setIsFirstOrder(false);             //update -> first order is in process, therefor next order is not first.
            if(orders.empty()) {                 //customer did not order anything on first order , therefor must be blocked from ever ordering again.
                setFailedToOrder(true);
                return orders;
            }
            else{
                setOrders(orders);          // update customer info, he will repeatadly re-order same things, so we are saving his first order.
            }

            return orders;

        }

        else{                       // this is a re-order.
            return getOrders();


        }
    else                            // if already failed to order previously , blocked from ordering.
        return orders;


}
vector<int> VegetarianCustomer::orderFirst(const std::vector<Dish> &menu){

    vector<int> orders;
    int index_non_alco_bev=-1;
    int index_vegi_dish=-1;
    int cost_of_non_alco=-1;


    for(unsigned int i=0;i<menu.size();i++){
        if(index_vegi_dish==-1 && menu.at(i).getType() == VEG) {
            index_vegi_dish=i;
        }
        else if(menu.at(i).getType()==BVG){
            if(cost_of_non_alco==-1) {
                cost_of_non_alco = menu.at(i).getPrice();
                index_non_alco_bev=i;
            }
            else if(cost_of_non_alco<menu.at(i).getPrice()){
                cost_of_non_alco = menu.at(i).getPrice();
                index_non_alco_bev=i;

            }
        }
    }

    if(index_non_alco_bev!=-1 && index_vegi_dish!=-1){
        orders.push_back(index_vegi_dish);
        orders.push_back(index_non_alco_bev);
    }
    return orders;      // orders is empty unless both dishes are accounted for.
}
VegetarianCustomer::VegetarianCustomer(const Customer &other): Customer(other){
    this->setType("VEG");
}
string VegetarianCustomer::toString() const{
    return getName() + "," +"veg";
}

VegetarianCustomer::~VegetarianCustomer(){                  // might need to delete the int vector, not sure at the moment.
}


CheapCustomer::CheapCustomer(string name, int id): Customer(name,id) {
    this->setType("CHP");
}               // inherits the customer constractor


// we  collect all expected future orders just onced,
// and then, each time we reorder, we use the same vector we orignally created when we traversed the menu, and depending on startegy , we pop elements".
std::vector<int> CheapCustomer::order(const std::vector<Dish> &menu) {
    vector<int> orders;


    if(!getFailedToOrder())         // if didnt fail to order ,execute
        if(getIsFirstOrder()) {        // if didnt order at all so far, execute
            orders=orderFirst(menu);
            setIsFirstOrder(false);             //update -> first order is in process, therefor next order is not first.
            if(orders.empty()) {                 //customer did not order anything on first order , therefor must be blocked from ever ordering again.
                setFailedToOrder(true);
                return orders;
            }
            else{
                setOrders(orders);          // update customer info, he will repeatadly re-order same things, so we are saving his first order.
            }

            return orders;

        }

        else{                       // this is a re-order.
            return orders;


        }
    else                            // if already failed to order previously , blocked from ordering.
        return orders;

}
vector<int> CheapCustomer::orderFirst(const std::vector<Dish> &menu){
    vector <int> cheapCustomer;
    int cheapest = menu.at(0).getPrice(); // IF menu is empty, this will result ERROR!!
    int cheapest_index {0}; // IF menu is empty, this will result ERROR!!
    for (unsigned int i=1; i<menu.size(); i++) {
        if (cheapest > menu.at(i).getPrice()) { //check if there is cheaper dish in the menu
            cheapest = menu.at(i).getPrice();
            cheapest_index = i;
        }
    }
    cheapCustomer.push_back(menu.at(cheapest_index).getId()); //only one dish, the cheapest
    return cheapCustomer;
}
CheapCustomer::CheapCustomer(const Customer &other): Customer(other){
    this->setType("CHP");
}
string CheapCustomer::toString() const{
    return getName() + "," +"chp";
}


CheapCustomer::~CheapCustomer(){
}


SpicyCustomer::SpicyCustomer(string name, int id): Customer(name,id) {
    this->setType("SPC");
}               // inherits the customer constractor


// we  collect all expected future orders just onced,
// and then, each time we reorder, we use the same vector we orignally created when we traversed the menu, and depending on startegy , we pop elements".
std::vector<int> SpicyCustomer::order(const std::vector<Dish> &menu) {
    vector<int> orders;

    if(!getFailedToOrder())         // if didnt fail to order ,execute
        if(getIsFirstOrder()) {        // if didnt order at all so far, execute
            orders=orderFirst(menu);
            setIsFirstOrder(false);             //update -> first order is in process, therefor next order is not first.
            if(orders.empty()) {                 //customer did not order anything on first order , therefor must be blocked from ever ordering again.
                setFailedToOrder(true);
                return orders;
            }
            else{
                int dishId=orders.back();
                orders.pop_back();
                setOrders(orders);
                return vector<int> {dishId};

            }

        }

        else{                       // this is a re-order.
            return getOrders();

        }
    else                            // if already failed to order previously , blocked from ordering.

        return orders;

}

vector<int> SpicyCustomer::orderFirst(const std::vector<Dish> &menu){
    vector<int> orders;
    int index_non_alco_bev=-1;
    int index_spicy_dish=-1;
    int cost_of_spicy=-1;
    int cost_of_non_alco=-1;


    for(unsigned int i=0;i<menu.size();i++){
        if(menu.at(i).getType() == SPC) {
            if(index_spicy_dish==-1) {
                index_spicy_dish = i;
                cost_of_spicy = menu.at(i).getPrice();
            }
            else if(cost_of_spicy<menu.at(i).getPrice()){
                index_spicy_dish = i;
                cost_of_spicy = menu.at(i).getPrice();
            }
        }
        else if(menu.at(i).getType()==BVG){
            if(cost_of_non_alco==-1) {
                cost_of_non_alco = menu.at(i).getPrice();
                index_non_alco_bev=i;
            }
            else if(cost_of_non_alco>menu.at(i).getPrice()){
                cost_of_non_alco = menu.at(i).getPrice();
                index_non_alco_bev=i;

            }
        }
    }

    if(index_spicy_dish!=-1) {
        if (index_non_alco_bev!=-1){
            orders.push_back(index_non_alco_bev);
        }
        orders.push_back(index_spicy_dish);
    }

    return orders;      // orders is empty unless both dishes are accounted for.

}
SpicyCustomer::SpicyCustomer(const Customer &other): Customer(other){
    this->setType("SPC");
}
string SpicyCustomer::toString() const{
    return getName() + "," +"spc";
}

SpicyCustomer::~SpicyCustomer(){                  // might need to delete the int vector, not sure at the moment.
}

AlchoholicCustomer::AlchoholicCustomer(string name, int id): Customer(name,id) {
    this->setType("ALC");
}     // inherits the customer constractor


// we  collect all expected future orders just onced,
// and then, each time we reorder, we use the same vector we orignally created when we traversed the menu, and depending on startegy , we pop elements".
std::vector<int> AlchoholicCustomer::order(const std::vector<Dish> &menu){
    vector<int> orders;

    if(!getFailedToOrder())         // if didnt fail to order ,execute
        if(getIsFirstOrder()) {        // if didnt order at all so far, execute
            orders=orderFirst(menu);
            setIsFirstOrder(false);             //update -> first order is in process, therefor next order is not first.
            if(orders.empty()) {                 //customer did not order anything on first order , therefor must be blocked from ever ordering again.
                setFailedToOrder(true);
                return orders;
            }
            else{
                int dishId=orders.back();
                orders.pop_back();
                setOrders(orders);
                return vector<int> {dishId};

            }

        }

        else {                       // this is a re-order.
            if (getOrders().empty()) {                 //customer did not order anything on first order , therefor must be blocked from ever ordering again.
                setFailedToOrder(true);
                return orders;
            } else {
                int dishId = getOrders().back();
                getOrders().pop_back();
                return vector<int>{dishId};   // this one is difffffffff
            }
        }
    else                            // if already failed to order previously , blocked from ordering.
        return orders;

}
vector<int> AlchoholicCustomer::orderFirst(const std::vector<Dish> &menu){
    vector<int> output;
    for(unsigned int i=0;i<menu.size();i++) {
        if (menu.at(i).getType() == ALC) {
            output.push_back(i);
        }
    }
    sortByPrice(output,menu);
    return output;
}
AlchoholicCustomer::AlchoholicCustomer(const Customer &other): Customer(other){
    this->setType("ALC");
}
string AlchoholicCustomer::toString() const{
    return getName() + "," + "alc" ;

}



// we receive an int vector , which represent Dish location in the Menu vector,
// we then use the prices of the dishes in the menu vector to sort the int vector,
// so the int vector will have the highest priced dish at the start at the vector ,
// and the highed priced dish at the end.
// this is used mainly to order the alcoholic dishes.
void AlchoholicCustomer::sortByPrice(vector<int>& output,const vector<Dish>& menu){           // insertion sort

    int size=output.size();
    int i,key, j;
    for (i = 1; i < size; i++)
    {
        key = output.at(i);
        j = i-1;


        while (j >= 0 && menu.at(output.at(j)).getPrice() <= menu.at(key).getPrice())
        {
            output.at(j+1) = output.at(j);
            j = j-1;
        }
        output.at(j+1) = key;
    }


}

AlchoholicCustomer::~AlchoholicCustomer(){
}