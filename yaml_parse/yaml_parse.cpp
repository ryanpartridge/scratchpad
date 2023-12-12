#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <ryml.hpp>
#include <ryml_std.hpp>

void walkNode(ryml::ConstNodeRef node)
{
    if (node.has_key())
    {
        std::cout << "node: " << node.key() << " (" << (node.has_val() ? node.val() : "[no value]") << ")" << std::endl;
    }
    if (node.is_container())
    {
        std::cout << "node is container (" << (node.is_seq() ? "sequence" : (node.is_map() ? "map" : "unknown")) << ")" << std::endl;
        for (auto child: node.children())
        {
            walkNode(child);
        }
    }
}

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

    auto tree = ryml::parse_in_arena(ryml::to_csubstr(restrictionsPayload));
    walkNode(tree.crootref());
    std::cout << "exiting yaml_parse" << std::endl;
    return 0;
}
