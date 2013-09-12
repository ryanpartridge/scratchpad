/*
 * lockfree.cpp
 *
 *  Created on: Sep 10, 2013
 *      Author: rpartridge
 */

#include <iostream>
#include <string>
#include <boost/thread.hpp>
#include <boost/lockfree/spsc_queue.hpp>

boost::lockfree::spsc_queue<std::string, boost::lockfree::capacity<64> > queue;
const int iterations = 10;
boost::atomic<bool> done(false);

void produce()
{
    for (int i = 0; i < iterations; ++i)
    {
        boost::this_thread::sleep_for(boost::chrono::seconds(2));
        while(!queue.push("hello"));
    }
    done = true;
}

void consume()
{
    std::string value;
    while (!done)
    {
        while (queue.pop(value))
        {
            std::cout << "pop value: " << value << std::endl;
        }
    }

    // we know the producer isn't going to produce anymore, so drop out of the loop
    // when the queue is empty
    while (queue.pop(value))
    {
        std::cout << "pop value: " << value << std::endl;
    }
}

int main(int argc, char* argv[])
{
    boost::thread producer(produce);
    boost::thread consumer(consume);
    producer.join();
    consumer.join();
    return 0;
}
