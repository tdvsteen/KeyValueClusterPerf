#ifndef __ACCESSPATTERN_H__
#define __ACCESSPATTERN_H__

#include <string>

using namespace std;

// Define a single access in a struct for speed and for correct clearing of memory
struct SingleAccess {
	bool read;
	string key;
	string value;
};

class AccessPattern
{
	public:
		virtual ~AccessPattern() {};

		virtual SingleAccess getNext() = 0;		// Get next access
};

#endif