/*
*	Record.h
*	Written by : Tuyet Pham
*	Group Memebers : Pedro Sanchez, Trevor Kerchner, Tuyet Pham
*	Project 2 - 2110 ISLAM
*/


#ifndef _RECORD_
#define _RECORD_
#include <string>
#include <set>
#include "log_.h"
using namespace std;


typedef struct Record {
private:
	/***LOGGER CLASS***/
	log_ myLog;

	//Record Attributes
	std::string KEY;
	std::set<std::pair<std::string, std::string>> ATTRIBUTES;				//The attribute first string will be the attribute name "KEY"

	//Chainging to avoid collision. 
	Record * chaining;

public:
	//Constructors
	Record();
	Record(std::string key);

	//Get key and Set key
	void setKEY(std::string);
	std::string getKEY();

	//Column Attributes mods
	bool addColumn(std::string colname, std::string colval);
	bool deleteColumn(std::string colname);
	bool changeColumnValue(std::string colname, std::string colval);
	string getColumnValue(string columnName);

	//Chainning functions to avoid collision
	Record * getchained();
	bool setChained(Record & rec);

	//Display
	void print();

} Record;

#endif
