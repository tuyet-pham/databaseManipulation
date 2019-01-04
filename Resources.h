/*
*	contains namespace myResources
*	class item
*/
#include <iostream>
#include <fstream>
#include <set>
using std::cout;

namespace myResources {
	
#ifndef ITEM_
#define ITEM_

	class item {
	private:
		std::string KEY;
		std::set<std::pair<std::string, std::string>> ATTRIBUTES;				//The attribute first string will be the attribute name "KEY"
	public:
		item();
		item(std::string key);
		void setKEY(std::string key);
		std::string getKEY();
		bool addAttributes(std::string attribute_key, std::string attribute_value);
		bool deleteAttribute(std::string attribute_key);
		void print();
	};

#endif // !ITEM_

}