/*
 * C4NetGenerator.hpp
 *
 *  Created on: Nov 10, 2014
 *      Author: rpartridge
 */

#ifndef C4NETGENERATOR_HPP_
#define C4NETGENERATOR_HPP_

#include <string>

#include <google/protobuf/compiler/code_generator.h>

class C4NetGenerator :
    public google::protobuf::compiler::CodeGenerator
{
public:
    inline C4NetGenerator() : google::protobuf::compiler::CodeGenerator() {};
    virtual ~C4NetGenerator();

    bool Generate(const google::protobuf::FileDescriptor* file,
        const std::string& parameter,
        google::protobuf::compiler::GeneratorContext* generator_context,
        std::string* error) const;

private:
    GOOGLE_DISALLOW_EVIL_CONSTRUCTORS(C4NetGenerator);
};

#endif /* C4NETGENERATOR_HPP_ */
