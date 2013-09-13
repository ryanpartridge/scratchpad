/*
 * State.hpp
 *
 *  Created on: Sep 13, 2013
 *      Author: rpartridge
 */

#ifndef STATE_HPP_
#define STATE_HPP_

#include <boost/function.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/multi_index_container.hpp>
#include <boost/multi_index/identity.hpp>
#include <boost/multi_index/member.hpp>
#include <boost/multi_index/ordered_index.hpp>
#include <boost/multi_index/hashed_index.hpp>

enum StateType_t
{
    UNKNOWN = 0,
    INIT,
    IDLE,
    STOP
};


class State
{
public:
    State();
    virtual ~State();

    static boost::shared_ptr<State> createState();

private:
    std::string name_;
};

typedef boost::function<boost::shared_ptr<State>(void)> StateCreatorFunction;

struct StateEntry
{
    StateEntry(StateType_t type, const std::string& umStateName, StateCreatorFunction& creatorFunction) :
        type_(type),
        umStateName_(umStateName),
        creatorFunction_(creatorFunction)
    {

    }

    virtual ~StateEntry();

    const StateType_t type_;
    const std::string umStateName_;
    StateCreatorFunction creatorFunction_;

    bool operator<(const StateEntry& entry)
    {
        return type_ < entry.type_;
    }
};

typedef boost::multi_index_container<
    StateEntry,
    boost::multi_index::indexed_by<
        boost::multi_index::ordered_unique<boost::multi_index::identity<StateEntry> >,
        boost::multi_index::hashed_unique<boost::multi_index::member<StateEntry, const std::string, &StateEntry::umStateName_> >
    >
> StateSet;

#endif /* STATE_HPP_ */
