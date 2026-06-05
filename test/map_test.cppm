#include <unordered_map>
#include <print>
#include <string>

//define
std::unordered_map<int, std::string> str;

//insert
void test_insert()
{
    std::unordered_map<std::string, std::string>mp;
    //[]
    mp["老师"] = "小李";
    mp["学生"] = "小明";

    //insert(pair)
    mp.insert({"x1", "y1"});
    mp.insert(std::make_pair("x2", "y2"));

    //emplace, current place construct
    mp.emplace("x2", "y22");

    mp["x2"] = "y22";

    std::println("[{}, {}]", "x2" ,mp["x2"]);

    //return false if insert fail(repeat)
    auto res = mp.insert({"x1", "y11"});
    if(!res.second)std::println("insert failed");
}

int main()
{
    test_insert();
    return 0;
}