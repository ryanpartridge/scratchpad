/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * \brief       Locking Queue - thread safe queue                            *
 * \copyright   Copyright 2013 Control4 Corporation. All rights reserved.    *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#ifndef LOCKINGQUEUE_HPP_
#define LOCKINGQUEUE_HPP_

#include <iostream>
#include <deque>
#include <boost/thread/mutex.hpp>
#include <boost/thread/condition_variable.hpp>
#include <boost/cstdint.hpp>

template<class T>
    class LockingQueue
    {
    public:
        LockingQueue()
        {
        }

        virtual ~LockingQueue()
        {
            boost::mutex::scoped_lock lock(mutex_);
            deque_.clear();
        }

        bool empty() const
        {
            boost::mutex::scoped_lock lock(mutex_);
            return deque_.empty();
        }

        std::size_t size() const
        {
            boost::mutex::scoped_lock lock(mutex_);
            return deque_.size();
        }

        void push(const T& t)
        {
            boost::mutex::scoped_lock lock(mutex_);
            deque_.push_back(t);
            available_.notify_all();
        }

        void pop(T& t)
        {
            boost::mutex::scoped_lock lock(mutex_);
            while (deque_.empty())
            {
                available_.wait(lock);
            }
            t = deque_.front();
            deque_.pop_front();
        }

        template<typename duration_type>
            bool pop(T& t, boost::uint32_t timeout)
            {
                boost::mutex::scoped_lock lock(mutex_);
                bool gotValue = false;
                if (timeout > 0)
                {
                    if (deque_.empty())
                    {
                        available_.wait_for(lock, duration_type(timeout));
                    }
                    if (!deque_.empty())
                    {
                        t = deque_.front();
                        gotValue = true;
                    }
                    if (gotValue)
                    {
                        deque_.pop_front();
                    }
                }
                else
                {
                    while (deque_.empty())
                    {
                        available_.wait(lock);
                    }
                    t = deque_.front();
                    deque_.pop_front();
                    gotValue = true;
                }
                return gotValue;
            }

        void pop()
        {
            boost::mutex::scoped_lock lock(mutex_);
            if (!deque_.empty())
            {
                deque_.pop_front();
            }
        }

        bool front(T& t)
        {
            boost::mutex::scoped_lock lock(mutex_);
            if (!deque_.empty())
            {
                t = deque_.front();
                return true;
            }
            return false;
        }

        void clear()
        {
            boost::mutex::scoped_lock lock(mutex_);
            deque_.clear();
        }

        void dump() const
        {
            boost::mutex::scoped_lock lock(mutex_);
            typename std::deque<T>::const_iterator it = deque_.begin(),
                end = deque_.end();

            std::cout << "queue contents (" << deque_.size() << ")"
                << std::endl;
            for (; it != end; ++it)
            {
                std::cout << "queue entry: " << *it << std::endl;
            }
        }

    private:
        std::deque<T> deque_;
        mutable boost::mutex mutex_;
        mutable boost::condition_variable available_;
    };

typedef LockingQueue<std::string> StringQueue;
#endif /* LOCKINGQUEUE_HPP_ */
