#include <iostream>
#include <ostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <algorithm>

#include <boost/variant.hpp>
#include <boost/mpl/at.hpp>

namespace TestData {
using TestDataNode = boost::make_recursive_variant<
    int,
    std::string,
    std::vector<boost::recursive_variant_>,
    std::unordered_map<std::string, boost::recursive_variant_>
>::type;

using TestDataVector = boost::mpl::at<TestDataNode::types, boost::mpl::int_<2>>::type;
using TestDataMap = boost::mpl::at<TestDataNode::types, boost::mpl::int_<3>>::type;
}

std::ostream& operator<<(std::ostream& os, const TestData::TestDataNode&)
{
    return os << "don't know how to output this";
}

class TestDataVisitor
    : public boost::static_visitor<>
{
public:
    void operator()(int& value) const
    {
        std::cout << "int value: " << value << std::endl;
    }

    void operator()(std::string& value) const
    {
        std::cout << "string value: " << value << std::endl;
    }

    void operator()(TestData::TestDataVector& value) const
    {
        std::cout << "vector node" << std::endl;
        std::for_each(value.begin(), value.end(), boost::apply_visitor(*this));
    }

    void operator()(TestData::TestDataMap& value) const
    {
        std::cout << "map node" << std::endl;
        std::for_each(value.begin(), value.end(), [this](TestData::TestDataMap::value_type nodePair)
            {
                std::cout << "key value: " << nodePair.first << std::endl;
                boost::apply_visitor(*this, nodePair.second);
            }
        );
    }
};

int main(int argc, char* argv[])
{
    std::cout << "entering main" << std::endl;

    TestData::TestDataNode rootNode = TestData::TestDataMap{};
    auto& rootRef = boost::get<TestData::TestDataMap>(rootNode);
    rootRef.emplace("key", "value");
    rootRef.emplace("vector", TestData::TestDataVector{1, 2, 3});
    rootRef.emplace("map", TestData::TestDataMap{{"x", 24}, {"y", 25}, {"z", 26}});

    std::cout << "original root: " << std::endl;
    boost::apply_visitor(TestDataVisitor(), rootNode);

    auto newRoot = std::move(rootNode);
    std::cout << "new root: " << std::endl;
    boost::apply_visitor(TestDataVisitor(), newRoot);

    std::cout << "original root after move: " << std::endl;
    boost::apply_visitor(TestDataVisitor(), rootNode);

    std::cout << "root node: " << rootNode << std::endl;
    std::cout << "new root: " << newRoot << std::endl;

    auto* rootInt = boost::get<int>(&newRoot);
    if (rootInt == nullptr)
    {
        std::cout << "root node is NOT an int" << std::endl;
    }

    auto* rootMap = boost::get<TestData::TestDataMap>(&newRoot);
    if (rootMap != nullptr)
    {
        std::cout << "root node is a map" << std::endl;
    }

    std::cout << "exiting main" << std::endl;
    return 0;
}

