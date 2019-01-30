/*
* log_ class - made 12/18/2018
* Copyright 2013, Tuyet Pham, All rights reserved.
* This class is meant to log all movement within the program. So far it runs on one log text file.
*/


#include <iostream>
#include <fstream>
#include <ctime>    

#ifndef LOG_
#define LOG_
class log_ {
private:
	const std::string MYLOGGER = "LOG_FILE.txt";
	std::fstream log_stream;
	std::string ex_msg = "\n!!CAUTION PROCEEDING!! --> PLEASE READ \n --> The logger file isn't open. "
		"The log proceeding will not be recorded.\n";
public:
	log_();
	void LOG_T(std::string message);
	~log_() {}
};
#endif // !LOG_
