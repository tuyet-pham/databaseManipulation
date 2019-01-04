#include "pch.h"
#include "database_.h"


database_::database_()
{
	LogM.LOG_T("Database instantiation . . . ready for file");
}

bool database_::addFile(std::fstream & file)
{
	currentfile = &file;
	if (currentfile->is_open() == true) {
		LogM.LOG_T("Database instantiation . . . file successfully tossed!");
		return true;
	}
	else {
		LogM.LOG_T("Database instantiation . . . file did not open, something went wrong!");
		return false;
	}
}

/*
* _func_ : HASHTABLE 
* task : Makes the hashed index value
*/
int database_::HASHTABLE(std::string KEY)
{
	int add = 0;
	int multi = 23;

	for (int i = 0; i < KEY.length(); i++) {
		add += KEY[i];
	}
	return add * multi % INITIAL_LENGTH;
}


void database_::expand()
{
}

bool database_::delete_t()
{
	return false;
}


bool database_::setKeyattribute(std::string key)
{
	return false;
}




/*
* _func_ : getTableSize
* task : Get the amount of items in the table
*/
int database_::getTableSize()
{
	int count = 0;

	for (auto e : TABLE) {
		if (e != NULL) {
			count++;
		}
	}
	return count;
}



/*
* _func_ : searchfor
* param : string
* return : bool
* task : search the table to see if the item with the key given is in the HASHTABLE
*/
bool database_::searchfor(std::string KEY)
{
	for (auto g : TABLE) {
		if (g != NULL) {
			if (g->getKEY() == KEY) {
				return true;
			}
		}
	}
	return false;
}

bool database_::additem(std::string key)
{
	return false;
}

bool database_::deleteitem(std::string key)
{
	return false;
}

bool database_::addvalue(std::string key, std::string columnName, std::string columnValue)
{
	return false;
}

bool database_::deletevalue(std::string key, std::string columnName)
{
	return false;
}

bool database_::addColumn(std::string columnName)
{
	return false;
}

bool database_::deleteColumn(std::string columnName)
{
	return false;
}

bool database_::writeto()
{
	return false;
}

bool database_::setTablename(std::string T_name)
{
	return false;
}

std::string database_::getTablename()
{
	return std::string();
}

std::string database_::getKeyAttribute()
{
	return std::string();
}

void database_::DISPLAYTABLE()
{
}

void database_::DISPLAYROW(std::string key)
{
}

void database_::DISPLAYCOLUMN(std::string columnName)
{
}
