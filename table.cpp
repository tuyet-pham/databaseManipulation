/*
*	table.cpp
*	Written by : Tuyet Pham
*/

#include "table.h"

/**********************************
			Constructors
***********************************/

table::table()
{
	this->NEWROWSIZE = MAXROWSIZE;
	myLog.LOG_T("table instantiation - No key, No tablename");
}

table::table(string tableName) : TABLENAME(tableName)
{
	this->NEWROWSIZE = MAXROWSIZE;
	this->TABLENAME = tableName;
	myLog.LOG_T("table instantiation - Table name : " + tableName);
}

table::table(string tableName, string key) : TABLENAME(tableName), KEY_ELE(key)
{
	this->NEWROWSIZE = MAXROWSIZE;
	this->TABLENAME = tableName;
	this->KEY_ELE = key;
	this->SET = true;
	myLog.LOG_T("table instantiation - Table name : " + tableName + ". Key element : " + key);
}


/**********************************
	To Set/Get the "key" value	
***********************************/

bool table::setKeyElement(string key)
{
	if (SET == false) {
		this->KEY_ELE = key;
		myLog.LOG_T("Setting key element of table successful - Key : " + key);
		return true;
	}
	else{
		myLog.LOG_T("Setting key element of table failed - Already Set");
		return false;
	}
}

string table::getKeyElement()
{
	myLog.LOG_T("Key Element Request from table - Key : " + this->KEY_ELE);
	return this->KEY_ELE;
}


/**********************************
	To Set/Get the tablename
***********************************/

void table::setTableName(string tablename)
{
	myLog.LOG_T("Setting Table name : " + tablename);
	this->TABLENAME = tablename;
}

string table::getTableName()
{
	myLog.LOG_T("Table name request : " + this->TABLENAME);
	return this->TABLENAME;
}


/**********************************
	   Important Functions
***********************************/

int table::INDEX_HASH(string key)
{

	int HASH_VAL = 0;
	for (int i = 0; i < key.size(); i++) {
		HASH_VAL = HASH_VAL + key[i];
	}

	HASH_VAL = (HASH_VAL * SALT_NUM / MAXCOLUMNSIZE) % MAXROWSIZE;

	myLog.LOG_T("Hashing Active . . . VAL - " + std::to_string(HASH_VAL) + " - ");
	return HASH_VAL;
}

bool table::CHECKnEXPAND()
{
	return false;
}


/**********************************
	   Assisting Functions
***********************************/

bool table::exists(string key)
{
	int INDEX = table::INDEX_HASH(key);
	if (HASHTABLE[INDEX] != NULL) {
		if (HASHTABLE[INDEX]->getKEY() == key) {
			myLog.LOG_T("Requesting existence of key : " + key + ". Index : " + std::to_string(INDEX));
			return true;
		}
	}
	else {
		INDEX = 0;
		for (INDEX; INDEX < MAXROWSIZE - 1; INDEX++) {
			if (HASHTABLE[INDEX] != NULL) {
				if (HASHTABLE[INDEX]->getKEY() == key) {
					return true;
				}
			}
		}
	}
	return false;
}

int table::tableSize()
{
	return this->TABLESIZE;
}

vector<string> table::getRec()
{
	vector<string> All_REC;

	for (int i = 0; i < MAXROWSIZE; i++) {
		if (HASHTABLE[i] != NULL) {
			string temp;
			temp.append(HASHTABLE[i]->getKEY());
			for (auto atr : COL_ELE) {
				temp.append("|");
				temp.append(HASHTABLE[i]->getColumnValue(atr));
			}
			All_REC.push_back(temp);
		}
	}

	return All_REC;
}


/**********************************
	 To make and delete a record.
***********************************/

bool table::makeRecord(Record & newRec)
{
	int INDEX = table::INDEX_HASH(newRec.getKEY());
	bool emptyslotfound = false;
	int counter = 0;

	if (TABLESIZE != NEWROWSIZE) {
		if (HASHTABLE[INDEX] != NULL) {
			if (HASHTABLE[INDEX]->getchained() != NULL) {
				while (emptyslotfound == false) {
					if (HASHTABLE[counter] == NULL) {
						
						for (int i = 0; i < COL_ELE.size(); i++) {
							newRec.addColumn(COL_ELE[i], "EMPTY");
						}

						HASHTABLE[counter] = &newRec;
						TABLESIZE++;

						emptyslotfound = true;
						return true;
					}
					else {
						counter++;
					}
				}
			}
			else if (HASHTABLE[INDEX]->getchained() == NULL) {
				HASHTABLE[INDEX]->setChained(newRec);
				for (int i = 0; i < COL_ELE.size(); i++) {
					newRec.addColumn(COL_ELE[i], "EMPTY");
				}
				TABLESIZE++;
				return true;
			}
		}
		else if (HASHTABLE[INDEX] == NULL) {
			HASHTABLE[INDEX] = &newRec;
			for (int i = 0; i < COL_ELE.size(); i++) {
				newRec.addColumn(COL_ELE[i], "EMPTY");
			}
			TABLESIZE++;
			return true;
		}
	}
	else {
		CHECKnEXPAND();
		table::makeRecord(newRec);
		return true;
	}
	return false;
}

bool table::deleteRecord(string key)
{

	int INDEX = table::INDEX_HASH(key);

	if (HASHTABLE[INDEX]->getKEY() != key) {
		for (int i = 0; i < MAXROWSIZE; i++) {
			if (HASHTABLE[i]->getKEY() == key) {
				delete HASHTABLE[i];
				HASHTABLE[i] = NULL;
				TABLESIZE--;
				return true;
			}
		
		}
	}
	else if (HASHTABLE[INDEX]->getKEY() == key) {
		delete HASHTABLE[INDEX];
		HASHTABLE[INDEX] = NULL;
		TABLESIZE--;
		return true;
	}
	return false;
}



/**********************************
	To add and delete a column.
***********************************/

bool table::addColumn(string columnName)
{
	bool found = false;
	string EMPTY = "~";


	for (auto columnEle = COL_ELE.begin(); columnEle != COL_ELE.end(); columnEle++) {
		if (*columnEle == columnName) {
			found = true;
		}
	}

	if (found == true) {
		
		return false;
	}
	else {

		COL_ELE.push_back(columnName);
		for (int i = 0; i < MAXROWSIZE; i++) {
			if (HASHTABLE[i] != NULL) {
				HASHTABLE[i]->addColumn(columnName, EMPTY);
			}
		}
		return true;
	}
}

bool table::deleteColumn(string columnName)
{
	bool found = false;

	for (auto columnEle = COL_ELE.begin(); columnEle != COL_ELE.end(); columnEle++) {
		if (*columnEle == columnName) {
			found = true;
			columnEle = COL_ELE.erase(columnEle);
		}
	}


	if (found == true) {
		for (auto rec : HASHTABLE) {
			if (rec != NULL) {
				rec->deleteColumn(columnName);
			}
		}
		return true;
	}
	else {
		return false;
	}
}


/************************************
	 To set the attribute values 
*************************************/

bool table::updateColumnValueof(string key, string columnName, string columnValue)
{
	int INDEX = INDEX_HASH(key);
	if (HASHTABLE[INDEX]->getKEY() != key || HASHTABLE[INDEX] == NULL) {
		for (auto ele : HASHTABLE) {
			if (ele != NULL) {
				if (ele->getKEY() == key) {
					ele->changeColumnValue(columnName, columnValue);
					return true;
				}
			}
		}
		return false;
	}
	else if (HASHTABLE[INDEX]->getKEY() == key || HASHTABLE[INDEX] != NULL) {
		HASHTABLE[INDEX]->changeColumnValue(columnName, columnValue);
		return true;
	}
	else {
		return false;
	}
}



/************************************
		Display Functions
*************************************/

void table::displayRow(string key)
{
}

void table::displayColumnOf(string key, string columnName)
{
}

void table::displayTable()
{
	cout << "\n\nTABLE NAME : " << this->getTableName() << endl;

	cout << left << setw(10) << this->KEY_ELE;
	for (auto col : COL_ELE) {
		cout << left << setw(15) << col; 
	}
	cout << endl;
	for (auto rec : HASHTABLE) {
		if (rec != NULL) {
			cout << left << setw(10) << rec->getKEY();
			for (auto atr : COL_ELE) {
				cout << left << setw(15) << rec->getColumnValue(atr);
			}
			cout << endl;
		}
		
	}

}
