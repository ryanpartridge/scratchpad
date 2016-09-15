/*
 * QueueManager.hpp
 *
 *  Created on: Sep 15, 2016
 *      Author: rpartridge
 */

#ifndef QUEUEMANAGER_HPP_
#define QUEUEMANAGER_HPP_

#include <LockingQueue.hpp>

template <typename DataType>
class QueueManager
{
public:
    typedef LockingQueue<DataType> queue_type;

    static queue_type& get_in_queue()
    {
        return in_queue;
    }

    static queue_type& get_out_queue()
    {
        return out_queue;
    }

private:
    QueueManager()
    {

    }

    virtual ~QueueManager()
    {

    }

private:
    static queue_type in_queue;
    static queue_type out_queue;
};

#endif /* QUEUEMANAGER_HPP_ */
