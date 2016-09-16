/*
 * QueueManager.hpp
 *
 *  Created on: Sep 15, 2016
 *      Author: rpartridge
 */

#ifndef QUEUEMANAGER_HPP_
#define QUEUEMANAGER_HPP_

#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>

#include <LockingQueue.hpp>

template <typename DataType>
class QueueManager
{
public:
    typedef LockingQueue<DataType> queue_type;

    static queue_type& get_in_queue() { return in_queue_; }
    static queue_type& get_out_queue() { return out_queue_; }

private:
    QueueManager()
    {

    }

    virtual ~QueueManager()
    {

    }

public:
    //static boost::shared_ptr<queue_type> in_queue_;
    //static boost::shared_ptr<queue_type> out_queue_;
    static queue_type in_queue_;
    static queue_type out_queue_;
};

//template <typename T> boost::shared_ptr<typename QueueManager<T>::queue_type> QueueManager<T>::in_queue_;
template <typename T> typename QueueManager<T>::queue_type QueueManager<T>::in_queue_;
template <typename T> typename QueueManager<T>::queue_type QueueManager<T>::out_queue_;

#endif /* QUEUEMANAGER_HPP_ */
