#include <map>
#include <string>
#include <unistd.h>

#include "boost/program_options.hpp" 

#include "ConfigurationManager.h"
#include "logger.h"
#include "KeyValueDB.h"
#include "RamCloudKeyValueDB.h"
#include "AccessPattern.h"
#include "RandomAccessPattern.h"
#include "Simulator.h"
#include "SimulationController.h"
#include "SimulationWorker.h"

using namespace std;

int main(int argc, char *argv[])
{
	bool controller = false;
	string databaseCfgPath = "database.cfg";
	string accessPatternCfgPath = "accessPattern.cfg";

	try 
	{ 
		/** Define and parse the program options 
	     */ 
	    boost::program_options::options_description desc("Options"); 
	    desc.add_options() 
	      ("help", "Request help message") 
	      ("controller", "Indicate this instance is the controller node") 
	      ("databasecfg", boost::program_options::value<string>(&databaseCfgPath), "Path to the database configuration file")
	      ("accesspatterncfg", boost::program_options::value<string>(&accessPatternCfgPath), "Path to the accessPattern configuraiton file");
	    boost::program_options::variables_map vm; 
	    try 
	    { 
	      boost::program_options::store(boost::program_options::parse_command_line(argc, argv, desc), vm);
	      if ( vm.count("help")  ) 
	      { 
	        std::cout << "Performance measurement of key value stores" << std::endl << desc << std::endl; 
	        return 0; 
	      }  
	      boost::program_options::notify(vm);
	    } 
	    catch(boost::program_options::error& e) 
	    { 
	      std::cerr << "ERROR: " << e.what() << std::endl << std::endl; 
	      std::cerr << desc << std::endl; 
	      return 1; 
	    }
	    // Parse input into settings
	    if(vm.count("controller"))
	    {
	    	controller = true;
	    }
	} 
	catch(std::exception& e) 
	{ 
	   std::cerr << "Unhandled Exception"<< e.what() << std::endl; 
	   return 1; 
	}

	if(controller)
	{
		LOG_DEBUG("Starting in controller mode");
		SimulationController controller;
		controller.connect();
		controller.execute();
	}
	else
	{
		SimulationWorker worker(databaseCfgPath, accessPatternCfgPath);
		worker.openConnection();
		worker.listen();
	}

	/*
	// create a configuration manager
	ConfigurationManager cm;
	// Create database configuration
	map<string,string> databaseConfiguration = cm.readFile(databaseCfgPath);
	// Create accessPattern configuration
	map<string,string> accessPatternConfiguration = cm.readFile(accessPatternCfgPath);
	// Create the simulator
	Simulator* simulator = new Simulator(databaseConfiguration, accessPatternConfiguration);
	// Perform simulation
	simulator->simulate(10000);
	// Perform another simulation
	delete simulator;
	accessPatternConfiguration["accessPatternType"]="ReadOnly";
	simulator = new Simulator(databaseConfiguration, accessPatternConfiguration);
	simulator->simulate(10000);
	// Destroy the simulator
	delete simulator;
	*/
}