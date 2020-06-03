#include <property/property.hpp>

#include <iostream>
#include <string>


class Test SERIALIZETHIS(Test)
{
    SCALAR(age, int, "age of test");
    SEQUENCE(childs, std::string, "names vector");
    public:
    CONSTRUCTORS(Test)
    
    bool operator==(const Test &test) const{
        if (this->age.get() != test.age.get()) return false;
        if (this->childs.size() != test.childs.size()) return false;
        
        for (size_t i = 0; i < this->childs.size(); ++i) {
            if(this->childs[i] != test.childs[i]) return false;
        }
        return true;
    }
    bool operator!=(const Test &test) const  {
        return !Test::operator==(test);
    }
};

class Test2 SERIALIZETHIS(Test2)
{
    SCALAR(age, int, "age of test");
    SCALAR(test1, Test, "name of test");
    SEQUENCE(test_childs, Test, "names vector");
public:
    CONSTRUCTORS(Test2)
    
    bool operator==(const Test2 &test2) const  {
        if (this->age.get() != test2.age.get()) return false;
        if (this->test1 != test2.test1) return false;
        
        if (this->test_childs.size() != test2.test_childs.size()) return false;
        return true;
    }
    bool operator!=(const Test2 &test2) const  {
        return !Test2::operator==(test2);
    }
};
class Test3 SERIALIZETHIS(Test3)
{
    SCALAR(age, int, "age of test");
    SEQUENCE(test_childs, Test2, "names vector");
    MAP(map1, std::string, int, "simple map stringe<->int");
    MAP(map2, std::string, Test, "simple map stringe<->Test");
public:
    CONSTRUCTORS(Test3)
    
    bool operator==(const Test3 &test3) const{
        if (this->age.get() != test3.age.get()) return false;
        if (this->test_childs.size() != test3.test_childs.size()) return false;
        for (size_t i = 0; i < this->test_childs.size(); ++i) {
            if(this->test_childs[i] != test3.test_childs[i]) return false;
        }
        return true;
    }
};


int main()
{
    int version = property::property_version();
    int major = property::property_major();
    int minor = property::property_minor();
    
    std::cout << "version: " << version << "\n";
    std::cout << "major: " <<  major << "\n";
    std::cout << "minor: " <<  minor << "\n";
    
    Test3 test{"test", ""};
    test.age.set(24);
    test.map1.insert_or_assign("speed", 40);
    test.map1.insert_or_assign("pressure", 70);
    auto test_map = Test("test_map", "empty");
    test.map2.insert_or_assign("just key", test_map);
    
    auto test_child = Test2("new_test_child","desc test_child");
    test_child.age.set(48);
    test_child.test1.age.set(42);
    test_child.test1.childs.push_back("new string");
    test_child.test1.childs.push_back("new string 2");
    test.test_childs.push_back(test_child);
    test_map.childs.push_back("new string");
    
    std::string serdata;
    test.serialize([&serdata](const std::string &sd) {
        serdata = sd;
    });
    
    Test3 ser_data;
    try {
      ser_data.deserialize(serdata);
    }
    catch (property::serialize_error &e) {
      std::cout << e.what();
      return -1;
    }
    
    ser_data.map1.insert_or_assign("speed", 50);
    ser_data.map1.insert_or_assign("pressure", 50);
    test_map.age.set(42);
    ser_data.map2.insert_or_assign("just key", test_map);
    test_map.childs.push_back("new string");
    ser_data.serialize([](const std::string &sd) {
        std::cout << "\n" << sd << "\n";
    });
    
    if(test == ser_data) {
        DEBUG("Test3 ser/deser is worked perfectly");
        return 0;
    }

    DEBUG("Test3 ser/deser is not worked perfectly");
    return 1;
}
