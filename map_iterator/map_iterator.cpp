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
	portMap[0] = 5800;
	portMap[1] = 5111;
	portMap[2] = 5020;

	std::cout << "start" << std::endl;

	std::cout << std::endl;
	std::cout << "forward removal" << std::endl;
	PortMap::iterator it = portMap.begin();
	PortMap::iterator end = portMap.end();
	while (it != end)
	{
		std::cout << "entry[" << it->first << "]: " << it->second << std::endl;
		if (it->second == 5020)
		{
			std::cout << "removing item: " << it->first << " (" << it->second << ")" << std::endl;
			portMap.erase(it++);
		}
		else
		{
			++it;
		}
	}

	std::cout << std::endl;
	std::cout << "forward iterator" << std::endl;
	it = portMap.begin();
	end = portMap.end();
	while (it != end)
	{
		std::cout << "entry[" << it->first << "]: " << it->second << std::endl;
		++it;
	}

	std::cout << std::endl;
	std::cout << "end" << std::endl;

    return 0;
}
