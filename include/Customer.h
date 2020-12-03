//
// Created by yoavgi@wincs.cs.bgu.ac.il on 11/5/18.
//

#ifndef CUSTOMER_H_
#define CUSTOMER_H_

#include <vector>
#include <string>
#include "Dish.h"
#include <algorithm>




class Customer{
public:
    Customer(std::string c_name, int c_id);
    Customer()=default;
    virtual std::vector<int> order(const std::vector<Dish> &menu)=0;
    virtual std::string toString() const = 0;
    std::string getName() const;
    int getId() const;
    bool getIsFirstOrder() const;           // added by user
    void setIsFirstOrder(bool val);         // added by user
    bool getFailedToOrder() const;          //added by user
    void setFailedToOrder(bool val);        // added by user
    void setOrders(std::vector<int> customers);
    void setType(std::string type);
    std::string getType() const;
    std::vector<int>& getOrders();
    //------------------------------Rule of 5-------------///
    virtual ~Customer();                                        //destructur.



private:
    const std::string name;
    const int id; //added the ; to the skeleton!!!+
    bool isFirstOrder;          // added by user, used to figure out if customer already ordered. (simplifies re-ordering process).
    bool failedToOrder;         // if customer failed to order once , he wont ever try to re-order
    std::vector<int> orders;
    std::string CustomerType;
};


class VegetarianCustomer : public Customer {
public:
    VegetarianCustomer(std::string name, int id);
    std::vector<int> order(const std::vector<Dish> &menu);
    std::vector<int> orderFirst(const std::vector<Dish> &menu);
    std::vector<int> reorder(const std::vector<Dish> &menu);
    std::string toString() const;
    VegetarianCustomer(const Customer &other);
    ~VegetarianCustomer();
private:
};


class CheapCustomer : public Customer {
public:
    CheapCustomer(std::string name, int id);
    std::vector<int> order(const std::vector<Dish> &menu);
    std::vector<int> orderFirst(const std::vector<Dish> &menu);
    std::string toString() const;
    CheapCustomer(const Customer &other);
    ~CheapCustomer();
private:
};


class SpicyCustomer : public Customer {
public:
    SpicyCustomer(std::string name, int id);
    std::vector<int> order(const std::vector<Dish> &menu);
    std::vector<int> orderFirst(const std::vector<Dish> &menu);
    std::string toString() const;
    SpicyCustomer(const Customer &other);
    ~SpicyCustomer();
private:
};


class AlchoholicCustomer : public Customer {
public:
    AlchoholicCustomer(std::string name, int id); //added to skeleton ;
    std::vector<int> order(const std::vector<Dish> &menu);
    std::vector<int> orderFirst(const std::vector<Dish> &menu);
    std::string toString() const;
    void sortByPrice(std::vector<int>& output,const std::vector<Dish> &menu);
    AlchoholicCustomer(const Customer &other);
    ~AlchoholicCustomer();

private:
};


#endif