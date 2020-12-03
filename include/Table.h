//
// Created by yoavgi@wincs.cs.bgu.ac.il on 11/5/18.
//

#ifndef TABLE_H_
#define TABLE_H_

#include <vector>
#include "Customer.h"
#include "Dish.h"

typedef std::pair<int, Dish> OrderPair;

class Table{
public:
    Table(int t_capacity);
    unsigned int getCapacity() const;
    void addCustomer(Customer* customer);
    void removeCustomer(int id);
    int getCustomerLocation(int id);                    // user addition. return location if customer in Vector, -1 if not found.
    Customer* getCustomer(int id);
    std::vector<Customer*>& getCustomers();
    std::vector<OrderPair>& getOrders();
    void order(const std::vector<Dish> &menu);
    void openTable();
    void closeTable();
    int getBill();
    bool isOpen();
    void change_bill_if_move(int new_bill);             // user addition
    void change_orderList(std::vector <OrderPair> & other);     //user addition
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ rule of five for Table
    ~Table();
    Table(const Table &other);
    Table& operator=(const Table& other);
    Table(Table&& other); //move constructor
    Table& operator=(Table&& other);//move assignment

private:
    unsigned int capacity;   /// changed to unsigned by user
    bool open;
    int billAmount;
    std::vector<Customer*> customersList;
    std::vector<OrderPair> orderList; //A list of pairs for each order in a table - (customer_id, Dish)
};


#endif