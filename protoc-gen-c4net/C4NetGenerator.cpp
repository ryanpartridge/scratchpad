/*
 * C4NetGenerator.cpp
 *
 *  Created on: Nov 10, 2014
 *      Author: rpartridge
 */

#include <iostream>

#include <google/protobuf/stubs/common.h>
#include <google/protobuf/descriptor.h>
#include <google/protobuf/io/zero_copy_stream.h>
#include <google/protobuf/io/printer.h>
#include <google/protobuf/compiler/cpp/cpp_helpers.h>

#include <C4NetGenerator.hpp>

C4NetGenerator::~C4NetGenerator()
{
}

bool C4NetGenerator::Generate(const google::protobuf::FileDescriptor* file,
    const std::string& parameter,
    google::protobuf::compiler::GeneratorContext* generator_context,
    std::string* error) const
{
    std::cerr << "Generating for: " << file->name() << std::endl;
    std::string basename = google::protobuf::compiler::cpp::StripProto(file->name());
    basename.append(".pb");
    google::protobuf::scoped_ptr<google::protobuf::io::ZeroCopyOutputStream> output(generator_context->OpenForInsert(basename + ".cc", "includes"));
    google::protobuf::io::Printer printer(output.get(), '$');
    printer.Print("here is a replacement");

    return true;
}
