//
// Created by yoavgi@wincs.cs.bgu.ac.il on 11/5/18.
//

#ifndef ACTION_H_
#define ACTION_H_

#include <string>
#include <iostream>
#include "Customer.h"


enum ActionStatus{
    PENDING, COMPLETED, ERROR
};


//Forward declaration
class Restaurant;

class BaseAction{
public:
    BaseAction();
    ActionStatus getStatus() const;
    virtual void act(Restaurant& restaurant)=0;
    virtual std::string toString() const=0;
    void setType(std::string type); // added by us 14.11
    std::string getType() const; // added by us 14.11
// rule of 5
    virtual BaseAction* Clone()=0;
    virtual ~BaseAction();




protected:
    void complete();
    void error(std::string errorMsg);
    std::string getErrorMsg() const;
    std::string StatusAndErrorMsg() const ;
    void setStatus(const ActionStatus & Other_status); //added by us 17.11
private:
    std::string errorMsg;
    ActionStatus status;
    std::string ActionType;
};


class OpenTable : public BaseAction {
public:
    OpenTable(int id, std::vector<Customer *> &customersList);
    void act(Restaurant &restaurant);
    std::string toString() const;

    int getId() const; //added by us 14.11
    // rule of 5
    OpenTable* Clone();
    ~OpenTable();
    OpenTable(const OpenTable &other); //added by us 14.11
//    OpenTable& operator=(const OpenTable& other); //copy assignment operator - cant be implemented (FAQ Clarifications)
    OpenTable(OpenTable&& other); //move constructor
//    OpenTable& operator=(OpenTable&& other);//move assignment - cant be implemented (FAQ Clarifications)
private:
    const int tableId;
    std::vector<Customer *> customers; //CHANGED 13.11 - according to the SPL ASS_1 change log - removed const.
};


class Order : public BaseAction {
public:
    Order(int id);
    void act(Restaurant &restaurant);
    std::string toString() const;
// rule of 5
    Order* Clone();
    Order(const Order &other); //added by us 14.11
private:
    const int tableId;
};




class MoveCustomer : public BaseAction {
public:
    MoveCustomer(int src, int dst, int customerId);
    void act(Restaurant &restaurant);
    std::string toString() const;
// rule of 5 - not needed, cuz got no resources. doing only copy constructor to help us later.
    MoveCustomer* Clone();
    MoveCustomer(const MoveCustomer &other); //added by us 14.11
private:
    const int srcTable;
    const int dstTable;
    const int id;
};


class Close : public BaseAction {
public:
    Close(int id);
    void act(Restaurant &restaurant);
    std::string toString() const;
//    rule of 5
    Close* Clone();
    Close(const Close &other); //added by us 14.11
private:
    const int tableId;
};


class CloseAll : public BaseAction {
public:
    CloseAll();
    void act(Restaurant &restaurant);
    std::string toString() const;

    //rule of 5
    CloseAll* Clone();
    CloseAll(const CloseAll &other); //added by us 14.11
private:
};


class PrintMenu : public BaseAction {
public:
    PrintMenu();
    void act(Restaurant &restaurant);
    std::string toString() const;
    std::string Convert_DishType_to_string(const DishType &type);
    //rule of 5
    PrintMenu* Clone();
    PrintMenu(const PrintMenu &other); //added by us 14.11
private:
};


class PrintTableStatus : public BaseAction {
public:
    PrintTableStatus(int id);
    void act(Restaurant &restaurant);
    std::string toString() const;
    //rule of 5
    PrintTableStatus* Clone();
    PrintTableStatus(const PrintTableStatus &other); //added by us 14.11
private:
    const int tableId;
};


class PrintActionsLog : public BaseAction {
public:
    PrintActionsLog();
    void act(Restaurant &restaurant);
    std::string toString() const;
    //rule of 5
    PrintActionsLog* Clone();
    PrintActionsLog(const PrintActionsLog &other); //added by us 14.11
private:
};


class BackupRestaurant : public BaseAction {
public:
    BackupRestaurant();
    void act(Restaurant &restaurant);
    std::string toString() const;
    //rule of 5
    BackupRestaurant* Clone();
    BackupRestaurant(const BackupRestaurant &other); //added by us 14.11
    ~BackupRestaurant(); //added by us 14.11
private:
};


class RestoreResturant : public BaseAction {
public:
    RestoreResturant();
    void act(Restaurant &restaurant);
    std::string toString() const;
    //rule of 5
    RestoreResturant* Clone();
    RestoreResturant(const RestoreResturant &other); //added by us 14.11


};


#endif