#include <list>
#include <map>
#include <string>
#include <time.h>
#include <unistd.h>	// to get hostname

#include "logger.h"
#include "Simulator.h"
#include "RamCloudKeyValueDB.h"
#include "RiakKeyValueDB.h"
#include "RandomAccessPattern.h"
#include "ReadOnlyAccessPattern.h"

// for debugging
#include <sstream>

Simulator::Simulator(map<string,string> databaseConfiguration, 
	map<string,string> accessPatternConfiguration)
{
	// Get the types to initialise from the configuration file
	string databaseType = databaseConfiguration["databaseType"];
	string accessPatternType = accessPatternConfiguration["accessPatternType"];
	// Create the requested database
	if(databaseType.compare("RamCloud") == 0)
	{
		keyValueDB = new RamCloudKeyValueDB(databaseConfiguration);
	}
	else if(databaseType.compare("Riak") == 0)
	{
		keyValueDB = new RiakKeyValueDB(databaseConfiguration);
	}
	else
	{
		// The requested database type does not exist
		// add exception handling
	}
	// Create the requested access pattern
	if(accessPatternType.compare("Random") == 0)
	{
		accessPattern = new RandomAccessPattern(accessPatternConfiguration);
	}
	else if(accessPatternType.compare("ReadOnly") == 0)
	{
		accessPattern = new ReadOnlyAccessPattern(accessPatternConfiguration);
	}
	else
	{
		// The requested accessPattern type does not exist
		// add exception handling
	}
	LOG_DEBUG("Initialised with configuration:");
	LOG_DEBUG(databaseType);
	LOG_DEBUG(accessPatternType);
}

Simulator::~Simulator()
{
	delete accessPattern;
	delete keyValueDB;
}

void Simulator::simulate(int runs)
{
	LOG_DEBUG("Called simulate");
	// Initialise the keyValueDatabase
	keyValueDB->initialise(accessPattern->getInitialisationKeyValuePairs());
	// Initialise timers
	LOG_DEBUG("INIT timer");
	struct timespec timespecStart;
	struct timespec timespecStop;
	const clockid_t id = CLOCK_MONOTONIC_RAW;
	LOG_DEBUG("Get time");
	clock_gettime(id, &timespecStart);
	LOG_DEBUG("time successful");
	// Do the actual simulation
	for(int i=0; i < runs; i++)
	{
		SingleAccess singleAccess = accessPattern->getNext();
		if(singleAccess.read == true)
		{
			keyValueDB->getValue(singleAccess.key);
		}
		else
		{
			keyValueDB->putValue(singleAccess.key, singleAccess.value);
		}
	}
	// Get final clock reading and print out difference
	clock_gettime(id, &timespecStop);
	double microsecondsPerOperation = calculateDurationMicroseconds(timespecStart, timespecStop) / runs;
	LOG_RESULTS("OUTPUT:");
	stringstream ss;
	ss << microsecondsPerOperation;
	LOG_RESULTS(ss.str());
}

map<string, string> Simulator::getResults()
{
	map<string, string> results;
	// Add hostname so that origin can be identified
	char* hostname = new char[256];
	gethostname(hostname, 256);
	results["hostname"]=string(hostname);
	// Add other results
	return results;
}

void Simulator::mergeResults(list<map<string, string>> results)
{
	LOG_DEBUG("mergeResults called");
}

double Simulator::calculateDurationMicroseconds(struct timespec start, struct timespec stop)
{
	uint64_t secondsDifference = stop.tv_sec - start.tv_sec;
	uint64_t nanosecondsDifference = stop.tv_nsec - start.tv_nsec;
	nanosecondsDifference += secondsDifference*1000000000;
	return nanosecondsDifference / 1000.0;
}