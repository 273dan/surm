// This example can be found at https://github.com/nlohmann/json/tree/develop/docs/mkdocs/docs/examples#readme

#include <nlohmann/json/json.hpp>
#include <iostream>
#include <iomanip>

using json = nlohmann::json;

int main()
{
    // create a JSON object
    json j =
    {
        {"pi", 3.141},
        {"happy", true},
        {"name", "Niels"},
        {"nothing", nullptr},
        {
            "answer", {
                {"everything", 42}
            }
        },
        {"list", {1, 0, 2}},
        {
            "object", {
                {"currency", "USD"},
                {"value", 42.99}
            }
        }
    };

    // add new values
    j["new"]["key"]["value"] = {"another", "list"};

    // count elements
    auto s = j.size();
    j["size"] = s;

    // pretty print with indent of 4 spaces
    std::cout << std::setw(4) << j << '\n';
}
