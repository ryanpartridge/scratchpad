/*
 * protoc-gen-c4net.cpp
 *
 *  Created on: Nov 10, 2014
 *      Author: rpartridge
 */

#include <google/protobuf/compiler/plugin.h>
#include <C4NetGenerator.hpp>

int main(int argc, char* argv[])
{
    C4NetGenerator generator;
    return google::protobuf::compiler::PluginMain(argc, argv, &generator);
}
