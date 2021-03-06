#ifndef __WRITEONLYACCESSPATTERN_H__
#define __WRITEONLYACCESSPATTERN_H__

// C C++ includes
#include <map>
#include <string>
// KeyValueClusterPerf includes
#include "AccessPattern.h"

using namespace std;

/**
 * Access pattern representing randomness in the keys and the read-write cycles
 */
class WriteOnlyAccessPattern : public AccessPattern
{
	public:
		WriteOnlyAccessPattern(map<string,string> configuration);
		~WriteOnlyAccessPattern();

		SingleAccess getNext();
		map<string,string> getInitialisationKeyValuePairs();

	private:
		/*! lowest integer value representing a valid key */
		int minKey;
		/*! highest integer value representing a valid key */
		int maxKey;
};

#endif
//__WRITEONLYACCESSPATTERN_H__