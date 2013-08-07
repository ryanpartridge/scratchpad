/*
 * TemplateFuncClass.hpp
 *
 *  Created on: Aug 7, 2013
 *      Author: rpartridge
 */

#ifndef TEMPLATEFUNCCLASS_HPP_
#define TEMPLATEFUNCCLASS_HPP_

class TemplateFuncClass
{
public:
    TemplateFuncClass();
    virtual ~TemplateFuncClass();

    template<typename Func>
    void execFunc(Func func);
};

#include <TemplateFuncClass.ipp>
#endif /* TEMPLATEFUNCCLASS_HPP_ */
