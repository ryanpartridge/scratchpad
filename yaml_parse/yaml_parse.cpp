#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <functional>
#include <ryml.hpp>
#include <ryml_std.hpp>
#include <c4/format.hpp>

void onError(const char* msg, size_t msg_len, ryml::Location location, void* /* user_data */)
{
    throw std::runtime_error("parse error");
}

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

    auto myErrorFunc = [](const char* msg, size_t msg_len, ryml::Location /* location */, void* /* user_data */)
        {
            throw std::runtime_error(ryml::formatrs<std::string>("parse error: {}", ryml::csubstr(msg, msg_len)));
        };
    ryml::Tree tree{ryml::Callbacks{nullptr, nullptr, nullptr, myErrorFunc}};
    try
    {
        ryml::parse_in_arena(ryml::to_csubstr(restrictionsPayload), &tree);
    }
    catch(std::runtime_error const& re)
    {
        std::cerr << re.what() << '\n';
    }

    auto rootNode = tree.crootref();
    if (rootNode.has_children())
    {
        walkNode(tree.crootref());
    }
    else
    {
        std::cout << "root node has no children -- cannot process" << std::endl;
    }
    std::cout << "exiting yaml_parse" << std::endl;
    return 0;
}
