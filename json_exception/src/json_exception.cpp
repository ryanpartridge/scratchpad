#include <iostream>
#include <sstream>
#include <json/json.h>

int main(int argc, char* argv[])
{
    std::cout << "Hello JSON!" << std::endl;
    std::string payload = "{\"myarray\": [\"one\", \"two\"]}";
    std::istringstream stream;
    stream.str(payload);
    Json::CharReaderBuilder builder;
    builder["collectComments"] = false;
    JSONCPP_STRING errors;
    Json::Value root;
    if (!Json::parseFromStream(builder, stream, &root, &errors))
    {
        std::cerr << "Error parsing payload: " << errors << std::endl;
        exit(1);
    }

    auto const end = root.end();
    for (auto it = root.begin(); it != end; ++it)
    {
        auto const key = it.key().asString();
        try
        {
            if (it->isBool())
            {
                auto value = it->asBool();
                std::cout << "key: " << key << "\tboolean value: " << std::boolalpha << value << std::endl;
            }
            else
            {
                auto value = it->asString();
                std::cout << "key: " << key << "\tstring value: " << value << std::endl;
            }
        }
        catch (Json::Exception const& ex)
        {
            std::cerr << "exception caught parsing payload: " << ex.what() << std::endl;
        }
    }
}

