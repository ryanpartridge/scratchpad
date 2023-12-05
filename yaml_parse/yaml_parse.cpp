#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <ryml.hpp>
#include <ryml_std.hpp>

int main(int argc, char* argv[])
{
    std::cout << "staring yaml_parse" << std::endl;

    // get the contents of the restrictions file
    const std::string restrictionsFilename{"../restrictions.yaml"};
    std::cout << "parsing file: " << restrictionsFilename << std::endl;
    std::ifstream restrictionsFile{restrictionsFilename};
    if (!restrictionsFile)
    {
        std::cerr << "Error opening " << restrictionsFilename << std::endl;
        exit(1);
    }
    std::ostringstream payloadStream;
    payloadStream << restrictionsFile.rdbuf();
    const auto restrictionsPayload = payloadStream.str();
    restrictionsFile.close();

    ryml::Tree tree = ryml::parse_in_arena(ryml::to_csubstr(restrictionsPayload));
        
    std::cout << "exiting yaml_parse" << std::endl;
    return 0;
}

