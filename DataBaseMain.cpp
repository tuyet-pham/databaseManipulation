/*
* DataBaseMain.cpp - made 12/28/2018
* Copyright 2013, Tuyet Pham, All rights reserved.
*/

#include "pch.h"
#include "database_.h"  
#include <iostream>
#include <fstream>
#include <string>
#include <regex>
#include <windows.h>   // WinApi header
#include "log_.h"
using namespace std;

/***OPENING LOGGING CLASS***/
log_ mainLOG;

/***THE LIST OF EXISTING 'DATABASES' OR 'TABLES'. INCASE FOR CHANGING***/
const string EXISTINGFILE = "existingfiles.txt";

string MAIN_MENU();
string ULIST_ENTRY();
string USER_ENTRY();
bool DELETE_FILE(string);
bool DELETE_CHANGE_LIST(string);
bool ADD_FILE(string);
bool ADD_CHANGE_LIST(string);
bool SYNC_FILEnLIST();
bool PROMPT_EXIT();
bool checkEXIST(string);
bool viewLIST();

bool OPEN_MANIP(string);

int main()
{
	fstream _file;
	bool exit = false;
	bool cred = false;
	const string MYUSERNAME = "root";
	const string MYPASSWORD = "toor";
	SYNC_FILEnLIST();


	do {
		string username;
		string password;
		cout << "\nUsername \n> ";
		cin >> username;
		cout << "Password \n> ";
		cin >> password;

		
		if (username == MYUSERNAME && password == MYPASSWORD) {
			mainLOG.LOG_T("User Verified");
			do
			{
				string choice = MAIN_MENU();									

				if (choice == "1") {
					mainLOG.LOG_T("Choice 1 chosen by user");

					string filename = ULIST_ENTRY();							//Getting user entry

					//If the existingfiles.txt opened correctly
					if (filename == "-1") {								
						cout << "Trouble opening existingfiles.txt" << endl;
					}
					else if(checkEXIST(filename) == false) {

							cout << "\nThe file '" + filename + "' choosen doesn't seem to exist in the directory - Try again. \n";
							mainLOG.LOG_T("File entry is not in the directory . . . ");
					}
					else {
						cout << "\n" + filename << " chosen." << endl;
						//OPEN DATA MANIPULATION HERE.
					}

				}
				else if (choice == "2") {

					mainLOG.LOG_T("Choice 2 chosen by user");
					cout << "\nIf this file exist in the directory but not on the list, we will add it to the list";
					string filename = USER_ENTRY();
					if (ADD_CHANGE_LIST(filename) == false) {
						cout << "\nThis file doesn't exist in the directory. If you want to add it to the list, place it in the same directory.";
					}
					else {
						cout << "\n" << filename << " was added!";
					}

				}
				else if (choice == "3") {

					mainLOG.LOG_T("Choice 3 chosen by user");
					string filename = USER_ENTRY();
					DELETE_FILE(filename);
				}
				else if (choice == "4") {

					mainLOG.LOG_T("Choice 4 chosen by user");
					string filename = USER_ENTRY();

					if (ADD_FILE(filename) == false) {
						cout << "File already exists. Try again\n";
					}
					else {
						cout << "File successfully added!\n";
					}
				}
				else if (choice == "5") {

					mainLOG.LOG_T("Choice 5 chosen by user");

					if (SYNC_FILEnLIST() == true) {
						cout << "Synching done. ";
					}
					else {
						cout << "Something went wrong while synching file to list";
					}

				}
				else if(choice == "6") {

					if (viewLIST() == false) {
						cout << "\nTrouble opening list! Try again.";
					}

				}
				else if (choice == "7") {
					mainLOG.LOG_T("Choice 7 chosen by user");
					if (PROMPT_EXIT() == true) {
						cout << "\nThanks for using my program. See you again soon!\n";
						exit = true;
						cred = true;
					}
					else {
						cout << "\nGlad to hear!";
						exit = false;
					}
				}
				else {
					cout << "\nIncorrect selection, try again \n";
					exit = false;
				}

			} while (exit == false);
		}
		else if (username == "0" && password == "0") {
			mainLOG.LOG_T("Exit by: 0/0");
			cred = true;
		}
		else {
			mainLOG.LOG_T("User Failed verification");
			cout << "\nIncorrect username and password. Try agian";
		}

	} while (cred == false);
	mainLOG.LOG_T("Exiting Program!");

	return 0;
}

/*
* _func_ : PROMPT_EXIT()
* return : bool
* task : Making sure the user actually wants to exit.
*/
bool PROMPT_EXIT()
{
	char answer;
	cout << "\nAre you sure you want to leave?[Y/N]\n>";
	cin >> answer;
	if (tolower(answer) == 'y') {
		return true;
	}
	else {
		mainLOG.LOG_T("False exit");
		return false;
	}
}



/*
* _func_ : MAIN_MENU()
* return : string
* task : Main menu for the entire program
*/
string MAIN_MENU() {
	
	//WINDOWS COLORING
	HANDLE  hConsole;
	int k = 3;
	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, k);

	string choice;
	cout << "\nMain Menu"
		<< "\n- - - - - - - - - - - - - - - - - - - - - - - -"
		<< "\n1. Pick file from list"
		<< "\n2. Enter file name that's not on the list, but is in directory."
		<< "\n3. Delete a file"
		<< "\n4. Add new file"
		<< "\n5. Synch list and file."
		<< "\n6. View list"
		<< "\n7. Shut Down \nPlease enter choice #\n> ";

	SetConsoleTextAttribute(hConsole, 15);
	cin >> choice;
	return choice;
}



/*
* _func_ : ULIST_ENTRY()
* return : string
* task : Showing the user the list of existing files(.txt) and then letting them choose.
*/
string ULIST_ENTRY()
{
	string usrentry;
	if (viewLIST() == false) {					//If fails -> then log & return -1; 
		mainLOG.LOG_T("Trouble opening existingfiles.txt");
		return "-1";
	}
	else {
		usrentry = USER_ENTRY();						//Get user entry from function.
		mainLOG.LOG_T("Successful entry");
		return usrentry;									//Returning the filename picked by user
	}
}



/*
* _func_ : USER_ENTRY()
* return : string
* task : All user entries;
*/
string USER_ENTRY()
{
	string filename;
	cout << "\nEnter the file name below with .txt at the end \n> ";
	cin >> filename;

	return filename;
}



/*
* _func_ : DELETE_FILE()
* return : bool
* param : string
* task : Deleting the file from directory
*/
bool DELETE_FILE(string filename)
{
	//Check to see if successful from deleting from directory, if successful then move to else statement.
	//Delete will fail if the file doesn't exist.
	if (std::remove(filename.c_str()) != 0) {									
		perror("\nDelete operation fails");
		mainLOG.LOG_T("Delete operation fails, because file doesn't exist");
		return false;
	}
	else {

		//Failure to delete from list bc can't find existingfiles.txt
		if (DELETE_CHANGE_LIST(filename) == false) {
			cout << "Trouble opening the \"existingfiles.txt\". Deleted file in system, but perhaps not in list.\nProceed with caution!";
			return false;
		}
		else {

			//If everything goes well
			cout << "\nDelete operation succeeded!";
			mainLOG.LOG_T("Delete operation Succeeded! Filename: " + filename + " deleted");
			SYNC_FILEnLIST();
			return true;
		}
	}
}



/*
* _func_ : DELETE_CHANGE_LIST()
* return : bool
* param : string
* task : Deleting the file/string from existingfiles.txt
*/
bool DELETE_CHANGE_LIST(string filename) {

	fstream openfile;
	openfile.open(EXISTINGFILE);

	//To store the wanted items.
	vector<string> listTemp;
	
	if (openfile.is_open() == false)
	{
		mainLOG.LOG_T("Trouble opening the \"existingfiles.txt\". Deleted file in system, but perhaps not in list");
		return false;
	}
	else {
		regex pattern(".txt");
		string templine;

		while (getline(openfile, templine))
		{
			//Keeping everything but the file name given to be deleted.
			if (templine != std::regex_replace(filename, pattern, "")) {
				listTemp.push_back(templine);
			}
		}

		mainLOG.LOG_T("Saving completed.");

		//Closing the old existingfile to make a new one
		openfile.close();

		//removing old existingfiles.txt
		if (std::remove(EXISTINGFILE.c_str()) != 0) {
			perror("\nDelete operation of old existingfiles.txt fails.");
			mainLOG.LOG_T("CAUTION. SERIOUS CONFLICTS CAN OCCUR");
			return false;
		}
		else {
			mainLOG.LOG_T("Converting, making new existing file");

			//Creating new existingfiles.txt 
			ofstream outfile(EXISTINGFILE);

			//Copying over kepted string
			for (auto ele : listTemp) {
				outfile << ele << endl;
				mainLOG.LOG_T("Copying. . . . .");
			}
			mainLOG.LOG_T("Changes succesfully set. Closing file. . .");
			outfile.close();
		}
		return true;
	}
}



/*
* _func_ : ADD_FILE()
* return : bool
* param : string
* task : Adding new file to directory
*/
bool ADD_FILE(string filename)
{
	//Testing to see if the file already exists
	if (checkEXIST(filename) == true) {
		mainLOG.LOG_T("Adding file failed. File already exists.");
		return false;
	}
	else {

		//Creating new file
		ofstream newfile;
		newfile.open(filename);

		//adding the table name to the first line of the file.
		regex pattern(".txt");
		newfile << std::regex_replace(filename, pattern, "") << endl;

		//done -> do-while loop
		//DONE -> to make sure the user enters the correct attributes
		bool done = false;
		char DONE;


		do {

			mainLOG.LOG_T("Getting user input for attribute");

			string attributes;
			cout << "\nPLEASE READ\nThe file " << filename << " was made. List out it's attributes with no space, divided by a '|' character. DO NOT put a '|' at the end i.e.";
			cout << "\nEXAMPLE --> ID|FIRST|LAST|STREET|STATE . . . .|ZIP\n>";
			cin.ignore();
			getline(cin, attributes);

			cout << "\nYour attibutes for this table is " << attributes << ". Is this correct?[Y/N]>";
			cin >> DONE;

			//Check if the user entered correctly
			if (tolower(DONE) == 'y') {
				newfile << attributes;
				done = true;
				mainLOG.LOG_T("Adding intital attributes to " + filename);
				
			}
			else {
				cout << "\nTry again.";
				done = false;
				mainLOG.LOG_T("Redirrected. . .");
			}

		} while (done == false);

		mainLOG.LOG_T("Adding file Success! Filename: " + filename + " added.");
		mainLOG.LOG_T("Writting to existingfiles");

		//Giving the filename to add to list
		ADD_CHANGE_LIST(filename);
		newfile.close();
		SYNC_FILEnLIST();
		return true;
	}	
	
}



/*
* _func_ : ADD_CHANGE_LIST()
* return : bool
* param : string
* task : Adding the file/string to existingfiles.txt
*/
bool ADD_CHANGE_LIST(string filename) {

	if (checkEXIST(filename) == true) {
		//THIS FUNCTIONS STILLS NEEDS MORE CODE.
		mainLOG.LOG_T("Writting new file name to file. . .");

		fstream addingto;
		string templine;
		bool foundrepeat = false;
		regex pattern(".txt");
		addingto.open(EXISTINGFILE);


		while (getline(addingto, templine,'\n')) {
			if (templine == std::regex_replace(filename, pattern, "")) {
				foundrepeat = true;
			}
		}
		addingto.close();

		if (foundrepeat == false) {

			addingto.open(EXISTINGFILE, ios::app);
			addingto << std::regex_replace(filename, pattern, "") << endl;
			addingto.close();
		}

		return true;
	}
	else {
		return false;
	}
}



/*
* _func_ : checkEXIST()
* return : bool
* param : string
* task : Checking if the file exists in the directory
*/
bool checkEXIST(string filename)
{
	fstream checkFile;
	checkFile.open(filename);

	if (checkFile.is_open() == true) {
		mainLOG.LOG_T("File exists");
		checkFile.close();
		return true;
	}
	else {
		mainLOG.LOG_T("File does not exists");
		checkFile.close();
		return false;
	}
}



/*
* _func_ : viewLIST()
* return : bool
* task : Print out the list of existing files.
*/
bool viewLIST()
{
	fstream existingstream;
	string line;

	existingstream.open(EXISTINGFILE);							
	if (existingstream.is_open() == false) {					
		mainLOG.LOG_T("Trouble opening existingfiles.txt");
		existingstream.close();
		return false;
	}
	else {

		cout << endl;
		while (getline(existingstream, line)) {		//Printing out existingfile.txt 
			cout << line << endl;
		}

		mainLOG.LOG_T("Opening existingfiles.txt successful!");
		existingstream.close();
		return true;
	}
}


/*
* _func_ : SYNC_FILEnLIST()
* return : bool
* task : Link up the files on list witht he files in the directory. 
* It's to delete the files that are in the list that aren't in the directory.
*/
bool SYNC_FILEnLIST()
{
	fstream list;
	string filename;
	int countLine = 0;
	list.open(EXISTINGFILE);
	vector<string> deleteThese;

	while (getline(list, filename, '\n')) {
		string temp = filename + ".txt";
		if (countLine > 2) {
			if (checkEXIST(temp) == false) {
				deleteThese.push_back(temp);
			}
		}
		countLine++;
	}
	list.close();


	for (auto ele : deleteThese) {
		DELETE_CHANGE_LIST(ele);
	}
	
	return true;
}



/*
* _func_ : OPEN_MANIP()
* return : bool
* param : string
* task : OPENING THE DATABASE FUNCTION TO MANIPULATE THE DATA
*/
bool OPEN_MANIP(string filename)
{
	if (checkEXIST(filename) == true) {



		return true;
	}
	else {
		return false;
	}
}

