//
// Created by yoavgi@wincs.cs.bgu.ac.il on 11/5/18.
//
#ifndef RESTAURANT_H_
#define RESTAURANT_H_

#include <vector>
#include <string>
#include "Dish.h"
#include "Table.h"
#include "Action.h"
#include <fstream>

extern Restaurant* backup ;            //global var for backup.


/* @@@@@@@@@@Program Memory Design :
 * in this program, due to complexity of backup / actionslog / Restaurant , where  several objects points to the same things,
 * instead of insuring just one copy exists in memory , we decided it would be simpler to let each of them clone the object, and be responsible for its destruction
 * for example , opentable is problematic , since it has a vector of pointers to customers , instead of having same vector in both "actionslog ->opentable" , and in
 * "Restaurant ->table", we cloned the customers ,so each vector will have different pointers.


 */


class Restaurant{
public:
    Restaurant();
    Restaurant(const std::string &configFilePath);
    void start();
    int getNumOfTables() const;
    Table* getTable(int ind);
    const std::vector<BaseAction*>& getActionsLog() const; // Return a reference to the history of actions
    std::vector<Dish>& getMenu();
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ added by us
    void ReadConfile(const std::string &configFilePath);   //Added by user to simplify Restaurant constructor.
    void CreateTables(std::string &capacity);               // added by user
    void CreateDish(std::string &Dish,int id);                     //add by user
    DishType Convert(const std::string &str);                //added by user to convert String to enum
    int Convert_command_toInt(const std::string &str);      //added by user to convert String to enum  for SwitchCase.
    std::vector<Customer*> OpenTableParser(std::string line,int &customerIndex);          // added by user to parse openTable +create objects
    int IdParser(std::string line);                                     // added by user to parse the id.
    std::vector<int> MoveParser(std::string line);              // return 3 number vector :   < sourceTable,DestinationTable,CustomerID>

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Rule of five
    ~Restaurant(); //destructor
    Restaurant(const Restaurant &other); // copy constructor
    Restaurant& operator=(const Restaurant& other); // copy assignment operator;
    Restaurant(Restaurant&& other); //move constructor
    Restaurant& operator=(Restaurant&& other);//move assignment

private:
    bool open;
    std::vector<Table*> tables;
    std::vector<Dish> menu;
    std::vector<BaseAction*> actionsLog;
};

#endif