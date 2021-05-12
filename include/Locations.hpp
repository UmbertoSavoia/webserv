#ifndef LOCATIONS_HPP
#define LOCATIONS_HPP

#include "utils.hpp"

class Locations
{
	private:
		std::string path;
		std::map<std::string, std::string> locations;

	public:
		Locations() : path(), locations() { }
		~Locations() { }
		std::map<std::string, std::string>&	getLocations(void) { return locations; }
		std::string&						getPath(void) { return path; }
};

#endif