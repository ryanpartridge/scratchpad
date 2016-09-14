/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * \brief       Locking Queue - thread safe queue                            *
 * \copyright   Copyright 2014 Control4 Corporation. All rights reserved.    *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#ifndef LOCKINGQUEUE_HPP_
#define LOCKINGQUEUE_HPP_

#include <iostream>
#include <deque>

#include <boost/thread/mutex.hpp>
#include <boost/thread/condition_variable.hpp>
#include <boost/cstdint.hpp>
#include <platformx/platform.hpp>

#if IS_DAVINCI2 || IS_ARMV6W
    // This function exists in libc.so, but was not declared in any header!
    extern "C" { int eventfd(unsigned int init, int flags); }
    #define EFD_CLOEXEC 02000000
#elif WIN32
	#define eventfd(a, b) -1
	#define EFD_CLOEXEC 02000000
	#include <io.h>
#else
    #include <sys/eventfd.h>
#endif

namespace c4
{
    template <class T>
    class LockingQueue
    {
    public:
        /// Default LockingQueue constructor.
        LockingQueue() :
            eventFd_(-1)
            {
            }

        /// Queue destructor.  Clear all elements of the queue.
        virtual ~LockingQueue()
            {
                boost::mutex::scoped_lock lock(mutex_);
                deque_.clear();

                // If an event file descriptor has been allocated, close it.
                if (eventFd_ != -1)
                    close(eventFd_);
                eventFd_ = -1;
            }

        /// Return true if the queue is empty.
        bool empty() const
            {
                boost::mutex::scoped_lock lock(mutex_);
                return deque_.empty();
            }

        /// Return the number of elements in the queue.
        std::size_t size() const
            {
                boost::mutex::scoped_lock lock(mutex_);
                return deque_.size();
            }

        /// Push an element onto the queue.
        ///
        /// Since this is a queue of @a T values, this will copy @a t once.  Notify all waiters
        /// that an element is available.  If an event file descriptor has been allocated,
        /// trigger the descriptor as well.
        ///
        /// @param t The value to push.
        void push(const T& t)
            {
                boost::mutex::scoped_lock lock(mutex_);
                deque_.push_back(t);
                available_.notify_all();

                // If we have an event file descriptor, notify it as well.  This is used with
                // an asio loop that wants to wake up when a value is enqueued.  We signal the
                // event with the value 1, which will accumulate in the event integer.
                uint64_t value = 1;
                if (eventFd_ != -1 &&
                    write(eventFd_, &value, sizeof(value)) == -1)
                {
                    throw boost::thread_resource_error(errno, "LockingQueue: cannot signal push event");
                }
            }

        /// Pop the top element from the queue.
        ///
        /// If the queue is not empty, wait for something to be pushed onto the queue.
        ///
        /// @param t A reference to a place to hold the front of the queue.
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

        /// Pop the top element from the queue.
        ///
        /// If the queue is not empty, wait for @a timeout units of time for something to be
        /// pushed onto the queue.  You can specify the duration units by providing a template
        /// argument, @a duration_type.  If the @a timeout value is zero, this function is
        /// identical to pop(T& t).
        ///
        /// @tparam duration_type A type for the time units, e.g., boost::chrono::milliseconds.
        /// @param t A reference to a place to hold the front of the queue.
        /// @param timeout The number of time units to wait for a queue element.
        /// @Return Return true if a value was pop from the queue.
        template <typename duration_type>
        bool pop(T& t, boost::uint32_t timeout)
            {
                return pop(t, duration_type(timeout));
            }

        /// Pop the top element from the queue.
        ///
        /// If the queue is not empty, wait for @a timeout units of time for something to be
        /// pushed onto the queue.  If the @a timeout value is zero, this function is identical
        /// to pop(T& t).
        ///
        /// @tparam duration_type A type for the time units, e.g., boost::chrono::milliseconds.
        /// @param t A reference to a place to hold the front of the queue.
        /// @param timeout The number of time units to wait for a queue element.
        /// @Return Return true if a value was pop from the queue.
        template <typename duration_type>
        bool pop(T& t, const duration_type& timeout)
            {
                boost::mutex::scoped_lock lock(mutex_);
                bool gotValue = false;
                if (timeout.count() > 0)
                {
                    if (deque_.empty())
                    {
                        available_.wait_for(lock, timeout);
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

        /// Remove the top element from the queue.
        ///
        /// If the queue is empty, the function does nothing.  This call does not block.
        void pop()
            {
                boost::mutex::scoped_lock lock(mutex_);
                if (!deque_.empty())
                {
                    deque_.pop_front();
                }
            }

        /// Copy the value at the front of the queue.
        ///
        /// If the queue is not empty, copy the value into @a t and return true.  Otherwise, @a
        /// t is unchanged and the function returns false.  This function does not block.
        ///
        /// @param t A reference to a place to hold the front of the queue.
        /// @return Returns true if a value was copied.
        bool front(T& t) const
            {
                boost::mutex::scoped_lock lock(mutex_);
                if (!deque_.empty())
                {
                    t = deque_.front();
                    return true;
                }
                return false;
            }

        /// Copy the value at the front of the queue.
        ///
        /// If the queue is not empty, copy the value into @a t and return true.  Otherwise,
        /// wait for something to be available in the queue.  This function blocks.
        ///
        /// @param t A reference to a place to hold the front of the queue.
        void frontBlock(T& t) const
            {
                boost::mutex::scoped_lock lock(mutex_);
                while (deque_.empty())
                {
                    available_.wait(lock);
                }
                t = deque_.front();
            }

        /// Copy the value at the front of the queue.
        ///
        /// If the queue is not empty, copy the value into @a t and return true.  Otherwise,
        /// wait for @a timeout units of time for something to be available in the queue.  This
        /// function blocks.
        ///
        /// @tparam duration_type A type for the time units, e.g., boost::chrono::milliseconds.
        /// @param t A reference to a place to hold the front of the queue.
        /// @param timeout The number of time units to wait for a queue element.
        /// @Return Return true if a value was copied from the queue.
        template <typename duration_type>
        bool frontBlock(T& t, boost::uint32_t timeout) const
            {
                return frontBlock(t, duration_type(timeout));
            }

        /// Copy the value at the front of the queue.
        ///
        /// If the queue is not empty, copy the value into @a t and return true.  Otherwise,
        /// wait for @a timeout units of time for something to be available in the queue.  This
        /// function blocks.
        ///
        /// @tparam duration_type A type for the time units, e.g., boost::chrono::milliseconds.
        /// @param t A reference to a place to hold the front of the queue.
        /// @param timeout The number of time units to wait for a queue element.
        /// @Return Return true if a value was copied from the queue.
        template <typename duration_type>
        bool frontBlock(T& t, duration_type timeout) const
            {
                boost::mutex::scoped_lock lock(mutex_);
                bool gotValue = false;
                if (timeout.count() > 0)
                {
                    if (deque_.empty())
                    {
                        available_.wait_for(lock, timeout);
                    }
                    if (!deque_.empty())
                    {
                        t = deque_.front();
                        gotValue = true;
                    }
                }
                else
                {
                    while (deque_.empty())
                    {
                        available_.wait(lock);
                    }
                    t = deque_.front();
                    gotValue = true;
                }
                return gotValue;
            }

        /// Clear all elements from the queue.
        void clear()
            {
                boost::mutex::scoped_lock lock(mutex_);
                deque_.clear();
            }

        /// Return a file descriptor suitable for use with boost asio.
        ///
        /// A LockingQueue can be used efficiently with boost asio by taking the event file
        /// descriptor returned by this function and wrapping it in a
        /// boost::posix::stream_descriptor and calling boost::async_read.  When a push
        /// operation is invoked, asio will awaken and call its registered callback.
        ///
        /// @return Returns a file descriptor suitable for use with select or epoll.  By
        /// wrapping this file descriptor in a boost::posix::stream_descriptor it can also be
        /// used with asio.
        int eventFd()
        {
            if (eventFd_ == -1)
            {
                // The initial value is the number of elements in the queue.  This means that if the
                // number of elements is zero a select will wait for elements, if the number of
                // elements is non-zero a select will return immediatley.
                eventFd_ = eventfd(deque_.size(), EFD_CLOEXEC);
                if (eventFd_ == -1)
                {
                    throw boost::thread_resource_error(errno, "LockingQueue: cannot create an event fd");
                }
            }

            return eventFd_;
        }

        /// Print the contents of the queue.
        void dump() const
            {
                boost::mutex::scoped_lock lock(mutex_);
                typename std::deque<T>::const_iterator it = deque_.begin(), end = deque_.end();

                std::cout << "queue contents (" << deque_.size() << ")" << std::endl;
                for (; it != end; ++it)
                {
                    std::cout << "queue entry: " << *it << std::endl;
                }
            }

    protected:
        // These are protected so we can effectively override push and pop (for instance, to
        // implement a priority queue).
        std::deque<T>                       deque_;
        mutable boost::mutex                mutex_;
        mutable boost::condition_variable   available_;
        int                                 eventFd_;
    };

    typedef LockingQueue<std::string> StringQueue;
} /* namespace c4 */
#endif /* LOCKINGQUEUE_HPP_ */
