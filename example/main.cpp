#include <propertysdk/property.hpp>

#include <fstream>
#include <iostream>
#include <string>

class Test SERIALIZETHIS(Test)
{
  SCALAR(age, int, "age of test");
  SEQUENCE(childs, std::string, "names vector");

public:
  CONSTRUCTORS(Test)

  bool operator==(const Test &test) const
  {
    if (this->age.get() != test.age.get())
      return false;
    if (this->childs.size() != test.childs.size())
      return false;

    for (size_t i = 0; i < this->childs.size(); ++i)
    {
      if (this->childs[i] != test.childs[i])
        return false;
    }
    return true;
  }
  bool operator!=(const Test &test) const { return !Test::operator==(test); }
};

class Test2 SERIALIZETHIS(Test2)
{
  SCALAR(test1, Test, "name of test");
  SEQUENCE(test_childs, Test, "names vector");

public:
  CONSTRUCTORS(Test2)

  bool operator==(const Test2 &test2) const
  {
    if (this->test1 != test2.test1)
      return false;

    if (this->test_childs.size() != test2.test_childs.size())
      return false;
    return true;
  }
  bool operator!=(const Test2 &test2) const { return !Test2::operator==(test2); }
};
class Test3 SERIALIZETHIS(Test3)
{
  SEQUENCE(test_childs, Test2, "names vector");
  MAP(map1, std::string, int, "simple map string<->int");
  MAP(map2, std::string, Test, "simple map string<->Test");

public:
  CONSTRUCTORS(Test3)

  bool operator==(const Test3 &test3) const
  {

    if (this->test_childs.size() != test3.test_childs.size())
      return false;
    if (this->map1.size() != test3.map1.size())
      return false;
    if (this->map2.size() != test3.map2.size())
      return false;

    auto is_equal_two_container = [](const auto &container1, const auto &container2) {
      for (size_t i = 0; i < container1.size(); ++i)
      {
        if (container1[i] != container2[i])
        {
          return false;
        }
      }
      return true;
    };

    return is_equal_two_container(this->test_childs, test3.test_childs);
  }
};

int main()
{
  int version = property::property_version();
  int major   = property::property_major();
  int minor   = property::property_minor();

  std::cout << "version: " << version << "\n";
  std::cout << "major: " << major << "\n";
  std::cout << "minor: " << minor << "\n";

  Test test{"test", "Simple Test class with scalar and basic sequnece"};
  test.age.set(77);
  test.childs.push_back("string one");
  test.childs.push_back("string two");

  Test2 test2{"test2", "Complex Test2 class with complex scalar and sequence"};
  test2.test1.age.set(42);
  test2.test_childs.push_back(test);
  Test3 test3{"test3", "Super hard complex Test3 class with all stuff"};
  test3.map1.insert_or_assign("ESC_KEY", 77);
  test3.map2.insert_or_assign("node_test", test);

  std::string serdata;
  test3.serialize([&serdata](const std::string &sd) {
    serdata = sd;
    std::ofstream out("out.yaml");
    out << sd;
    out.close();
  });

  Test3 ser_data;
  try
  {
    ser_data.deserialize(serdata);
  }
  catch (property::serialize_error &e)
  {
    std::cout << e.what();
    return -1;
  }

  ser_data.serialize([](const std::string &sd) { std::cout << "\n" << sd << "\n"; });

  if (test3 == ser_data)
  {
    std::cout << "propertysdk works\n";
    return 0;
  }

  std::cout << "propertysdk doesn't work\n";
  return 1;
}
