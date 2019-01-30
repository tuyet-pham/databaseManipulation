#include "database_.h"


/**********************************
			Constructors
***********************************/

database_::database_(string filename)
{
	myLog.LOG_T("Database Instantiation Successful with filename : " + filename);
	this->tablefilename = filename;
}


/**********************************
			Interfaces
***********************************/

int database_::dataMAIN()
{
	//Exit 1 failure : No table name was given 
	if (this->tablefilename.empty() == true) {
		myLog.LOG_T("FAILURE EXIT 1. tablename not satisfied");
		return 1;
	}

	//Exit 2 failure : Filename given did not open correctly open. 
	if (SYNC_TABLE() == false) {
		myLog.LOG_T("FAILURE EXIT 2. Synching failed. File doesn't exist.");
		return 2;
	}

	myLog.LOG_T("Database main in use . . .");
	dataMSG();
	bool done = false;

	do {

		string choice = dataMENU();

		if (choice == "A" || choice == "a") {
			myLog.LOG_T("User picked choice 'A' ");
			VIEWTABLE();
		}
		else if (choice == "B" || choice == "b") {
			myLog.LOG_T("User picked choice 'B' ");
			if (ADDRECORD() == false) {
				cout << "\n The key entered had already existed. Try again.\n";
			}
		}
		else if (choice == "C" || choice == "c") {
			myLog.LOG_T("User picked choice 'C' ");
			DELETERECORD();
		}
		else if (choice == "D" || choice == "d") {
			myLog.LOG_T("User picked choice 'D' ");
			CHANGEVALUE();
		}
		else if (choice == "E" || choice == "e") {
			myLog.LOG_T("User picked choice 'E' ");
			DELETECOLUMN();
		}
		else if (choice == "F" || choice == "f") {
			myLog.LOG_T("User picked choice 'F' ");
			ADDCOLUMN();
		}
		else if (choice == "G" || choice == "g") {
			myLog.LOG_T("User picked choice 'G' ");
			CHANGETABLENAME();
		}	
		else if (choice == "H" || choice == "h") {
			myLog.LOG_T("User picked choice 'H' ");
			WRITE();
		}
		else if (choice == "I" || choice == "i") {
			myLog.LOG_T("Choice I chosen by user");
			if (PROMPT_EXIT() == true) {
				done = true;
			}
			else {
				cout << "\nGlad to hear!";
				done = false;
			}
		}
		else {
			cout << "\nIncorrect selection, try again \n";
			done = false;
		}

	} while (done == false);

	TABLEFILE.close();
	return 0;
}

string database_::dataMENU()
{
	//WINDOWS COLORING
	HANDLE  hConsole;
	int k = 6;
	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, k);

	string choice;
	cout << "\nDataBase Menu"
		<< "\n- - - - - - - - - - - - - - - - - - - - - - - -"
		<< "\nA. View Table"
		<< "\nB. Add Record"
		<< "\nC. Delete Record"
		<< "\nD. Change value of column of -"
		<< "\nE. Delete entire column"
		<< "\nF. Add column to All"
		<< "\nG. Change table name "
		<< "\nH. Save"
		<< "\nI. Exit\nPlease enter choice #\n> ";
	SetConsoleTextAttribute(hConsole, 15);
	cin >> choice;
	return choice;
}

void database_::dataMSG()
{
	cout << "\n\n\n\n\n\n\n\n\n\n\n------------------------------------------------------------------------------------------------------"
		<< "\nThe database's purpose is to change .txt file by acting as the table's interface. "
		<< "\nPlease chose what you want to do with the choosen file. Make sure to select save to write to file. " 
		<< "\n-------------------------------------------------------------------------------------------------------" << endl;
}

bool database_::PROMPT_EXIT()
{
	char answer;
	cout << "\nAre you sure you want leave table view?[Y/N]\n>";
	cin >> answer;
	if (tolower(answer) == 'y') {
		return true;
	}
	else {
		myLog.LOG_T("False exit");
		return false;
	}
}

bool database_::FILEWRITEWARNING(){

	string answer;
	cout << "\Please proceed with caution for you are about to write over your file.";
	cout << "\nThis means your old file will be deleted.";
	cout << "\nAre you sure you would like to delete this file? [Y/N] \n> ";
	cin.ignore();
	getline(cin, answer);

	if (answer == "y" || answer == "Y") {
		return true;
	}
	else {
		return false;
	}
	return false;
}

/**********************************
		Private Methods
***********************************/

bool database_::SYNC_TABLE()
{
	//Opening file given.
	this->TABLEFILE.open(this->tablefilename);

	if (TABLEFILE.is_open() == false) {
		return false;
	}

	int count = 0;
	string templine;

	while (getline(TABLEFILE, templine)) {
		if (count == 0) {

			myLog.LOG_T("Setting table name of table : " + templine);
			MYTABLE.setTableName(templine);
		}
		else if (count == 1) {

			myLog.LOG_T("Setting table's key element : " + templine);
			MYTABLE.setKeyElement(templine);
		}
		else if (count == 2) {

			myLog.LOG_T("Setting table's column elements: " + templine);
			col_cutter(templine);
		}
		else {

			//ADDING EXISTING RECORDS TO FILE
			myLog.LOG_T("Adding existing records to table. . .");
			addFromSync(templine);   
		}
		count++;
	}

	this->TABLEFILE.close();


	return true;
}

void database_::VIEWTABLE() {

	/****************** Currently can only display the whole table *******************/
	MYTABLE.displayTable();
}

bool database_::ADDRECORD()
{
	myLog.LOG_T("Adding a record . . . ");


	cout << "\n******************* Adding Record **********************\n";
	bool leave = false;


	do {

		//Getting key and init checking if it exists already
		string key;
		cout << "\nPlease enter the Record's key value.\n>";
		cin.ignore();
		getline(cin, key);
		

		if(MYTABLE.exists(key) == true){
			myLog.LOG_T("Adding Record fail because : " + key + " already exists");
			return false;
		}

		/***************************************  IF KEY DOESN'T ALREADY EXIST  **************************************/
		
		Record * REC = new Record;
		string colVAL;
		string leaveanswer;

		
		cout << "\nThe Columns of the Record are : ";
		for (auto g : INIT_ATT) {
			cout << g << " ";
		}

		cout << endl;

		//Getting the values that was given 
		vector<string> col = getColumnvalue(getColValue());


		//Setting the key to  new record
		REC->setKEY(key);

		//Seeting the columns witht the values given
		for (int i = 0; i < INIT_ATT.size(); i++) {
			REC->addColumn(INIT_ATT[i], col[i]);
		}

		cout << "\nDo you want to keep adding a Record [Y/N]?\n>";
		cin >> leaveanswer;

		if (leaveanswer == "y" || leaveanswer == "Y") {

			if (MYTABLE.makeRecord(*REC) == false) {
				cout << "\nSorry but this record already exists. Try again.";
			}
			else {
				myLog.LOG_T("Record was added : " + REC->getKEY());
			}

			leave = false;
		}
		else {

			if (MYTABLE.makeRecord(*REC) == false) {
				cout << "\nSorry but this record already exists. Try again.";
			}
			else {
				myLog.LOG_T("Record was added : " + REC->getKEY());
			}

			leave = true;
		}

	} while (leave == false);
	return true;
}

bool database_::DELETERECORD()
{
	string recToDel = getString();
	
	if(MYTABLE.deleteRecord(recToDel) == true) {
		return true;
	}
	else {
		return false;
	}
}

bool database_::CHANGEVALUE()
{
	string key;
	string col;
	string colVal;

	cout << "Enter the key of the record that you would like to change.\n>";
	cin >> key;

	if (MYTABLE.exists(key) == true) {

		cout << "\nThey columns for this table are : ";
		for (auto col : INIT_ATT) {
			cout << col << " ";
		}
		cout << endl;

		cout << "\nPlease enter the column that would would like to change, exactly like it is.\n>";
		cin >> col;

		cout << "Enter the new value. If empty value then put '~'.\n>";
		cin.ignore();
		getline(cin, colVal);

		MYTABLE.updateColumnValueof(key, col, colVal);
		return true;
	}
	else {
		cout << "Sorry this record doesn't exist.";
		return false;
	}
}

bool database_::DELETECOLUMN()
{
	string colToDel = getString();
	bool found = false;
	for (auto colname = INIT_ATT.begin(); colname != INIT_ATT.end(); colname++) {
		if (colToDel == *colname) {
			found = true;
			colname = INIT_ATT.erase(colname);
		}
	}

	if (found == true) {
		if (MYTABLE.deleteColumn(colToDel) == true) {
			return true;
		}
	}
	return false;
}

bool database_::ADDCOLUMN()
{
	string colToAdd = getString();
	bool found = false;

	for (auto colname : INIT_ATT) {
		if (colToAdd == colname) {
			found = true;
		}
	}

	if (found == true) {
		return false;
	}
	else {
		INIT_ATT.push_back(colToAdd);
		if (MYTABLE.addColumn(colToAdd) == true) {
			return true;
		}
	}
	return false;
}

bool database_::CHANGETABLENAME()
{
	string newTableName = getString();
	MYTABLE.setTableName(newTableName);
	return true;
}

bool database_::WRITE()
{

	if (FILEWRITEWARNING() == false) {
		return false;
	}

	//Testing to see if the file exists.
	this->TABLEFILE.open(this->tablefilename);
	if (TABLEFILE.is_open() == false) {
		return false;
	}
	this->TABLEFILE.close();
	
	//removing old existingfiles.txt
	if (std::remove(tablefilename.c_str()) != 0) {
		perror("\nDelete operation of old existingfiles.txt fails.");
		return false;
	}
	else {

		//Creating new existingfiles.txt 
		ofstream outfile(tablefilename);

		vector<string> toPRINT;
		string colToPRINT;

		toPRINT.push_back(MYTABLE.getTableName());
		toPRINT.push_back(MYTABLE.getKeyElement());

		for (int i = 0; i < INIT_ATT.size(); i++) {
			colToPRINT = colToPRINT.append(INIT_ATT[i]);
			if (i != INIT_ATT.size() - 1) {
				colToPRINT.append("|");
			}
		}

		toPRINT.push_back(colToPRINT);
		vector<string> recToPRINT = MYTABLE.getRec();
		
		for (int i = 0; i < recToPRINT.size(); i++) {
			toPRINT.push_back(recToPRINT[i]);
		}

		for (int i = 0; i < toPRINT.size(); i++) {
			outfile << toPRINT[i] << endl;
		}


		outfile.close();
		return true;
	}

	
}




/**********************************
		Helper Methods
***********************************/

bool database_::addFromSync(string rec)
{
	Record * REC = new Record;
	istringstream ss(rec);
	string temp;					
	int count = 0;					//Keep track of column values
	int attributeindex = 0;			//Keep track of column names
	

	while (getline(ss, temp, '|')) {

		//Setting the keys of records
		if (count == 0) {
			REC->setKEY(temp);
		}
		else {
			REC->addColumn(INIT_ATT[attributeindex], temp);
			attributeindex++;
		}
		count++;
	}

	myLog.LOG_T("Record was added : " + REC->getKEY());
	this->MYTABLE.makeRecord(*REC);
	return true;
}

bool database_::col_cutter(string line)
{
	istringstream ss(line);
	string temp;
	int count = 0;


	// Creating the columns from the string given.
	while (getline(ss, temp, '|')) {
		if (count != 0) {

			INIT_ATT.push_back(temp);
			MYTABLE.addColumn(temp);
		}
		count++;
	}
	return true;
}

vector<string> database_::getColumnvalue(string line)
{
	vector<string> colval;
	istringstream ss(line);
	string temp;

	while (getline(ss, temp, '|')) {
		colval.push_back(temp);
	}

	return colval;
}

string database_::getColValue()
{
	bool CORRECT = false;
	string finalCol;

	do {

		string col;
		string answer;

		cout << "\nPlease enter the column values separated by a '|'. If empty, enter ~ .\ni.e. 12589|John|~|MaryLand's Street. . . |USA\n>";
		cin.ignore();
		getline(cin, col);


		// Is the above answer correct ?
		cout << "\nAre the values correct [Y/N] : " << col << "\n>";
		cin >> answer;

		if (answer == "y" || answer == "Y") {
			finalCol = col;
			CORRECT = true;
		}
		else {
			cout << "\nTry Again.";
			CORRECT = false;
		}

	} while (CORRECT == false);

	return finalCol;
}

string database_::getString()
{
	string NAME;
	cout << "\nPlease Enter name \n> ";
	cin.ignore();
	getline(cin, NAME);
	return NAME;

}

