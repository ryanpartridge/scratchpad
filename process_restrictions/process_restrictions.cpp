#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <functional>
#include <set>
#include <ryml.hpp>
#include <ryml_std.hpp>
#include <c4/format.hpp>

void globalOnError(const char* msg, size_t msg_len, ryml::Location location, void* /* user_data */)
{
    throw std::runtime_error(ryml::formatrs<std::string>("general error: {}:{}:{} ({}B): ERROR: {}", location.name, location.line, location.col, location.offset, ryml::csubstr(msg, msg_len)));
}

void onError(const char* msg, size_t msg_len, ryml::Location location, void* /* user_data */)
{
    throw std::runtime_error(ryml::formatrs<std::string>("parse error: {}:{}:{} ({}B): ERROR: {}", location.name, location.line, location.col, location.offset, ryml::csubstr(msg, msg_len)));
}

void walkNode(ryml::ConstNodeRef node)
{
    std::cout << "--- new node ---" << std::endl;
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

std::string getChildValue(ryml::ConstNodeRef const& node, std::string const& childName)
{
    auto childNode = node.find_child(ryml::to_csubstr(childName));
    if (!childNode.valid() || !childNode.has_val())
    {
        return std::string{};
    }
    std::string value;
    ryml::from_chars(childNode.val(), &value);
    return value;
}

std::string processDriver(ryml::ConstNodeRef const& driver)
{
    auto identifier = getChildValue(driver, "identifier");
    if (!identifier.empty())
    {
        //std::cout << "driver name: " << identifier << std::endl;
    }
    return identifier;
}

int main(int argc, char* argv[])
{
    std::cout << "staring process_restrictions" << std::endl;

    // set the global error callback
    ryml::set_callbacks(ryml::Callbacks{nullptr, nullptr, nullptr, globalOnError});

    // get the contents of the restrictions file
    const std::string restrictionsFilename{"../3.5.0-ProjectRestrictions.json"};
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
    //const auto restrictionsPayload = std::string("{\"system\": {\"update_version\": \"3.5.0\", \"minimum_version\": \"3.3.1\"}}");
    restrictionsFile.close();

    ryml::Tree tree{ryml::Callbacks{nullptr, nullptr, nullptr, onError}};
    try
    {
        ryml::parse_in_arena(ryml::to_csubstr(restrictionsPayload), &tree);
        auto rootNode = tree.crootref();
/*
        if (rootNode.has_children())
        {
            walkNode(rootNode);
        }
        else
        {
            std::cout << "root node has no children -- cannot process" << std::endl;
        }
*/

        std::cout << std::endl;
        std::cout << "querying the tree" << std::endl;

        auto drivers = rootNode.find_child("drivers");
        if (!drivers.valid())
        {
            std::cout << "missing drivers node" << std::endl;
            return 0;
        }

        std::set<std::string> driverNames;
        for (auto driver: drivers)
        {
            auto driverName = processDriver(driver);
            auto added = false;
            std::tie(std::ignore, added) = driverNames.emplace(driverName);
            if (!added)
            {
                std::cout << "duplicate driver detected: " << driverName << std::endl;
            }
        }

/*
        auto testsNode = rootNode["tests"];
        if (!testsNode.valid() || !testsNode.is_seq())
        {
            std::cout << "invalid tests node, or not a sequence" << std::endl;
        }
        else
        {
            if (testsNode.has_key())
            {
                std::cout << "tests node key: " << testsNode.key() << std::endl;
            }
        }

        std::string nodeName{"tests"};
        testsNode = rootNode.find_child(ryml::to_csubstr(nodeName));
        if (!testsNode.valid())
        {
            std::cout << "missing tests node" << std::endl;
        }
        else
        {
            if (testsNode.has_key())
            {
                std::cout << "tests node key: " << testsNode.key() << std::endl;
            }
        }

        auto fooNode = rootNode.find_child("foo");
        if (!fooNode.valid())
        {
            std::cout << "missing or invalid foo node" << std::endl;
        }
*/
    }
    catch (std::runtime_error const& re)
    {
        std::cerr << re.what() << '\n';
    }
    catch (...)
    {
        std::cerr << "caught an unknown error" << std::endl;
    }

    std::cout << "exiting process_restrictions" << std::endl;

    return 0;
}
