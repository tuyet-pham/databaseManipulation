/*
*	table.h
*	Written by : Tuyet Pham 
*	table.h : Acts like any other table with rows and columns. 
*	The sizees are currently non expandable. 
*
*/


#ifndef TABLE_
#define TABLE_

#include <iostream>
#include <iomanip>
#include <string>
#include "Record.h"
#include "log_.h"
#include <vector>
using namespace std;


class table
{	
private:
	/***THIS LOGGER***/
	log_ myLog;

	//Some static numbers
	static const int MAXROWSIZE = 2000;						//Max row size
	static const int MAXCOLUMNSIZE = 30;					//Max column size
	static const int SALT_NUM = 75367;						//Additional salt-num
	static const int THRES = 700;							//Threshold in which to expand
	static const int GROWTH_MULTI = 3;						//Growth size Multiplier
	int NEWROWSIZE;

	//Table elements
	string TABLENAME;										//Store the name of the table
	string KEY_ELE;											//Key element
	vector<string> COL_ELE;									//column elements
	bool SET = false;
	Record * HASHTABLE[MAXROWSIZE];							//An Array of Record pointers. The contents of our table. 
	int TABLESIZE = 0;

	//Important function
	int INDEX_HASH(string);								//Hash Function


	/*******!!!! NOT APPLICABLE !!!!*******/
	bool CHECKnEXPAND();

public:
	// Constructor
	table();
	table(string tableName);
	table(string tableName, string key);

	// To Set/Get the "key" value
	bool setKeyElement(string key);
	string getKeyElement();


	// To Set/Get the tablename
	void setTableName(string tablename);
	string getTableName();

	//To make and delete a record.
	bool makeRecord(Record & newRec);
	bool deleteRecord(string key);

	//To set and get the attribute
	bool addColumn(string columnName);
	bool deleteColumn(string columnName);

	//To set the attribute values 
	bool updateColumnValueof(string key, string columnName, string columnValue);

	//displays functions
	void displayRow(string key);
	void displayColumnOf(string key, string columnName);
	void displayTable();

	//Assisting functions
	bool exists(string key);
	int tableSize();


	//GET THE RECORDS
	vector<string> getRec();


};

#endif // !TABLE_
