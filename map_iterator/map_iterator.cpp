/*
 * map_iterator.cpp
 *
 *  Created on: Jul 31, 2014
 *      Author: rpartridge
 */

#include <iostream>
#include <map>
#include <boost/foreach.hpp>

typedef std::map<int, int> PortMap;

int main(int argc, char* argv[])
{
	PortMap portMap;
	portMap[1] = 5111;
	PortMap portMap2 = portMap;

	std::cout << "start" << std::endl;

	std::cout << "forward iterator" << std::endl;
	for (PortMap::iterator it = portMap.begin(); it != portMap.end(); ++it)
	{
		std::cout << "entry[" << it->first << "]: " << it->second << std::endl;
	}

	std::cout << std::endl;
	std::cout << "forward removal" << std::endl;
	for (PortMap::iterator it = portMap.begin(); it != portMap.end(); ++it)
	{
		if (it->second == 5111)
		{
			std::cout << "removing item: " << it->first << " (" << it->second << ")" << std::endl;
			portMap.erase(it->first);
		}
	}
	std::cout << std::endl;
	std::cout << "reverse iterator" << std::endl;
	for (PortMap::reverse_iterator it = portMap2.rbegin(); it != portMap2.rend(); ++it)
	{
		std::cout << "entry[" << it->first << "]: " << it->second << std::endl;
	}

	std::cout << std::endl;
	std::cout << "reverse removal" << std::endl;
	for (PortMap::reverse_iterator it = portMap2.rbegin(); it != portMap2.rend(); ++it)
	{
		if (it->second == 5111)
		{
			std::cout << "removing item: " << it->first << " (" << it->second << ")" << std::endl;
			portMap2.erase(it->first);
		}
	}

	std::cout << "end" << std::endl;

    return 0;
}
