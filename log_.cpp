#include "pch.h"
#include "log_.h"
#include <string>
#pragma warning(disable : 4996)

log_::log_()
{
	log_stream.open(MYLOGGER, std::ios::app);
	try {
		if (log_stream.is_open() == false) {
			throw ex_msg;
		}
		else {

			// current date/time based on current system
			time_t now = time(0);
			// convert now to string form
			char* dt = std::ctime(&now);
			log_stream << ">> Logger opened sucessfully - " << "The local date and time is : " << dt;
			log_stream.close();
		}
	}
	catch (std::string & ex) {
		std::cout << ex;
	}
}

//LOG_T() : call this function to log movement
void log_::LOG_T(std::string message)
{
	log_stream.open(MYLOGGER, std::ios::app);
	try {
		if (log_stream.is_open() == false) {
			throw ex_msg;
		}
		else {
			// current date/time based on current system
			time_t now = time(0);
			// convert now to string form
			char* dt = std::ctime(&now);
			log_stream << message << " - " << dt;
			log_stream.close();
		}
	}
	catch (std::string & ex) {
		std::cout << ex;
	}
}
