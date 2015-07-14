#include <map>
#include <string>
#include <sstream>
#include <iostream>
#include <fstream>

#include "ConfigurationManager.h"
#include "logger.h"

map<string, string> ConfigurationManager::readFile(string fileName)
{
	LOG_DEBUG("Reading file");
	LOG_DEBUG(fileName);
	ifstream file(fileName);
	stringstream ss;
	ss << file.rdbuf();
	LOG_DEBUG("contents");
	LOG_DEBUG(ss.str());
	return readString(ss.str());
}

void ConfigurationManager::writeFile(string fileName, map<string, string> configuration)
{
	ofstream file;
	file.open(fileName);
	file << writeString(configuration);
	file.close();
}

map<string, string> ConfigurationManager::readString(string str)
{
	map<string, string> configuration;
	size_t splitPosition;
	// split string line by line
	istringstream iss(str);
	string line;
	while(getline(iss,line))
	{
		// split line and add to map
		if((splitPosition = line.find("=")) != string::npos)
		{
			configuration[line.substr(0, splitPosition)] = line.substr(splitPosition + 1, string::npos);
		}
	}

	return configuration;
}

string ConfigurationManager::writeString(map<string, string> configuration)
{
	stringstream ss;
	map<string, string>::iterator it=configuration.begin();
	ss << it->first << "=" << configuration[it->first];
	it++;
	for(; it != configuration.end(); it++)
	{
		ss << endl << it->first << "=" << configuration[it->first];
	}
	return ss.str();
}