
#include <windows.h>   // WinApi header
#include <iostream>
#include <fstream>
#include "Record.h"
#include <sstream>
#include "table.h"
#include "log_.h"

#ifndef DATABASE_
#define DATABASE_


class database_
{
private:
	//Attributes of database
	std::string tablefilename;
	fstream TABLEFILE;
	table MYTABLE;
	vector<string> INIT_ATT;
	string KEY;

	/****LOGGER CLASS****/
	log_ myLog;


	//Privates Methods 
	bool SYNC_TABLE();
	bool ADDRECORD();
	bool DELETERECORD();
	bool CHANGEVALUE();
	bool DELETECOLUMN();
	bool ADDCOLUMN();
	bool CHANGENAME();
	bool CHANGE_KEY();


	//Intefaces 
	string dataMENU();
	void dataMSG();
	bool PROMPT_EXIT();


	//Helper Methods
	bool col_cutter(string line);
	bool addFromSync(string);
	vector<string>getColumnvalue(string line);

public:
	//Constructor
	database_(string filename);

	//INTERFACES
	int dataMAIN();
	
	//Methods 
	void VIEWTABLE();
	bool WRITE();

	
	~database_() {}
};

#endif // DATABASE_

