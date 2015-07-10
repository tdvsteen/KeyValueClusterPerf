#include "RamCloud.h"

#include "logger.h"                     // For logging throughout application
#include "KeyValueDB.h"                 // For instantiating a keyvalue database
#include "RamCloudKeyValueDB.h"         // For instantiating a Ramcloud database

RamCloudKeyValueDB::RamCloudKeyValueDB(map<string,string> configuration)
{
	LOG_DEBUG("INSTANCE CREATED");

	// Generate coordinatorLocator string
	string coordinatorLocator;
	coordinatorLocator = configuration["communicationProtocol"];	// add protocol
	coordinatorLocator += ":host=";
	coordinatorLocator += configuration["coordinatorHost"];			// add hostname
	coordinatorLocator += ",port=";
	coordinatorLocator += configuration["coordinatorPort"];			// add port
	
	LOG_DEBUG(coordinatorLocator);

	// Setup the cluster connector
	ramcloudContext = new RAMCloud::Context(false);
	ramcloudDB = new RAMCloud::RamCloud(ramcloudContext, coordinatorLocator.c_str(), "");

	// Create a table in the cluster
	ramcloudDB->createTable("KeyValueClusterPerf");
	tableID = ramcloudDB->getTableId("KeyValueClusterPerf");
}

RamCloudKeyValueDB::~RamCloudKeyValueDB()
{
	LOG_DEBUG("INSTANCE DELETED");
	// Tear down created table
	ramcloudDB->dropTable("KeyValueClusterPerf");

	// Remove cluster connection
	delete ramcloudDB;
	delete ramcloudContext;
}

void RamCloudKeyValueDB::putValue(std::string key, std::string value)
{
	LOG_DEBUG("PUT CALLED");
	// table_id, key, keylength, buffer, bufferlength
    ramcloudDB->write(tableID, key.c_str(), key.size(), value.c_str(), value.size());
}

string RamCloudKeyValueDB::getValue(std::string key)
{
	LOG_DEBUG("GET CALLED");
	// Prepare buffer and read value
	RAMCloud::Buffer buffer;
    ramcloudDB->read(tableID, key.c_str(), key.size(), &buffer);
    int length = buffer.size();
    string s(static_cast<const char*>(buffer.getRange(0, length)), length );
    return s;
}

void RamCloudKeyValueDB::deleteValue(std::string key)
{
	LOG_DEBUG("DELETE CALLED");
	ramcloudDB->remove(tableID, key.c_str(), key.size(), NULL, NULL);
}
	