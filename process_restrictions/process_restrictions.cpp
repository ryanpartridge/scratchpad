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

constexpr char indent[] = "  ";

std::string processDriver(ryml::ConstNodeRef const& driver)
{
    auto identifier = getChildValue(driver, "identifier");
    if (identifier.empty())
    {
        return std::string{};
    }
    auto version = getChildValue(driver, "version");
    auto condition = getChildValue(driver, "condition");
    auto message = getChildValue(driver, "message");
    std::cout << indent << "- type: driver" << std::endl;
    std::cout << indent << indent << "target-version: " << version << std::endl;
    std::cout << indent << indent << "title: End of Support" << std::endl;
    std::cout << indent << indent << "message: " << message << std::endl;
    std::cout << indent << indent << "parameters:" << std::endl;
    std::cout << indent << indent << indent << "identifier: " << identifier << std::endl;
    std::cout << indent << indent << indent << "status: eos" << std::endl;
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
    restrictionsFile.close();

    ryml::Tree tree{ryml::Callbacks{nullptr, nullptr, nullptr, onError}};
    try
    {
        ryml::parse_in_arena(ryml::to_csubstr(restrictionsPayload), &tree);
        auto rootNode = tree.crootref();

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
            if (!driverName.empty())
            {
                auto added = false;
                std::tie(std::ignore, added) = driverNames.emplace(driverName);
                if (!added)
                {
                    std::cout << "duplicate driver detected: " << driverName << std::endl;
                }
            }
        }
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
