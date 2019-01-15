#include "pch.h"
#include "Record.h"



/************************************
			Constructors
*************************************/
Record::Record() {
	myLog.LOG_T("Record instantiation - No key");
}

Record::Record(std::string key) : KEY(key)
{
	myLog.LOG_T("Record instantiation - Key : " + key);
	this->KEY = key;
}



/************************************
		Get key and Set key
*************************************/
void Record::setKEY(std::string key)
{
	myLog.LOG_T("Record key change - Key : " + key);
	this->KEY = key;
}

std::string Record::getKEY()
{
	myLog.LOG_T("Record key request - Key : " + this->KEY);
	return KEY;
}



/************************************
		Column Attributes mods
*************************************/

bool Record::addColumn(std::string colname, std::string colval)
{
	myLog.LOG_T("Record attempting to add column : " + colname + " = " + colval);

	bool exists = false;

	for (auto ele : ATTRIBUTES) {
		if (ele.first == colname) {
			exists = true;
		}
	}

	if (exists == false) {
		ATTRIBUTES.insert(std::make_pair(colname, colval));
		myLog.LOG_T("Record successfully added column : " + colname + " => " + colval);
		return true;
	}
	else {
		myLog.LOG_T("Record failed to add column : " + colname + " => " + colval);
		return false;
	}
}

bool Record::deleteColumn(std::string colname)
{
	auto it = ATTRIBUTES.begin();
	for (it; it != ATTRIBUTES.end(); it++) {
		if ((*it).first == colname) {
			ATTRIBUTES.erase(it);
			myLog.LOG_T("Record column deletion successful: " + colname);
			return true;
		}
	}

	myLog.LOG_T("Record column deletion failed: " + colname + ". No such column.");
	return false;
}

bool Record::changeColumnValue(std::string colname,std::string colval) {

	bool found = false;
	auto it = ATTRIBUTES.begin();

	for (it; it != ATTRIBUTES.end(); it++) {
		if (it->first == colname) {
			ATTRIBUTES.erase(it);
			ATTRIBUTES.insert(std::make_pair(colname, colval));
			found = true;
			myLog.LOG_T("Record successfully changed column : " + colname + " => " + colval);
			return found;
		}
	}

	myLog.LOG_T("Record attempted change failed column : " + colname + " => " + colval + ". No such column.");
	return found;
}

string Record::getColumnValue(string columnName)
{
	for (auto ele : ATTRIBUTES) {
		if (ele.first == columnName) {
			return ele.second;
		}
	}
	return "NOT FOUND";
}



/***************************************************
	Chainning functions to avoid collision
****************************************************/

Record * Record::getchained()
{
	if (chaining == NULL) {
		return nullptr;
	}
	else {
		return chaining;
	}
}

bool Record::setChained(Record & rec) {
	this->chaining = &rec;
	return true;
}



/************************************
			 Display 
*************************************/

void Record::print()
{
	cout << "\nThis Record =>"
		<< "\nKEY value: " + this->KEY << endl;
	for (auto ele : ATTRIBUTES) {
		cout << ele.first + " => " << ele.second << endl;
	}

}
