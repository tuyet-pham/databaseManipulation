
#include "Resources.h"
#include "log_.h"
using namespace myResources;

#ifndef DATABASE_
#define DATABASE_

class database_
{
private:
	static const int INITIAL_LENGTH = 2000;				//The length of the initial array
	const int threshold = 100;							//The threshold to expand the table
	item * TABLE[INITIAL_LENGTH];						//The table of item pointers
	std::string tableName;								
	std::string keyAttribute;

	log_ LogM;											//The logger. 
	std::fstream * currentfile;							//The file container.

	int HASHTABLE(std::string KEY);						//The hashfuntion that will give it the index
	void expand();										//The function to expand
	bool delete_t();									//The function that will delete the table
	bool setKeyattribute(std::string);					//Set the key attribute when the file is reading.
	int getTableSize();									//Get the amount of the item inside of the table
	bool searchfor(std::string KEY);					//look to see if this item exists.

	 
public:

	//Constructor
	database_();
	bool addFile(std::fstream & file);

	//Methods
	bool additem(std::string key);																//To add a new item by getting the key first.
	bool deleteitem(std::string key);															//Delete item by it's key
	bool addvalue(std::string key, std::string columnName, std::string columnValue);			//Adding value to a column by name, IF column exsits.
	
	bool deletevalue(std::string key, std::string columnName);									//Delete the value of the column
	bool addColumn(std::string columnName);
	bool deleteColumn(std::string columnName);


	bool writeto();														//Make sure to prompt user if they are sure to write over old data
	bool setTablename(std::string T_name);								//Setting table name of curent file.
	std::string getTablename();											//get the table name
	std::string getKeyAttribute();										//get the key attribute of the table. 

	void DISPLAYTABLE();
	void DISPLAYROW(std::string key);
	void DISPLAYCOLUMN(std::string columnName);

	~database_() {}
};

#endif // DATABASE_

