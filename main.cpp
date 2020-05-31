#include <property/property.hpp>
//
#include <iostream>
#include <string>


class Test SERIALIZETHIS(Test)
{
    SCALAR(age, int, "age of test");
    SCALAR(name, std::string, "name of test");
    public:

    CONSTRUCTORS(Test)
    
};
//
class Test2 SERIALIZETHIS(Test2)
{
    SCALAR(age, int, "age of test");
    SCALAR(is_booled, bool, "booled of test");
//  SCALAR(name, std::string, "name of test");
    SCALAR(test1, Test, "name of test");
//  SCALAR(test2, Test, "name of test");
//  SCALAR(test3, Test, "name of test");
//  SCALAR(test4, Test, "name of test");
//  SCALAR(test5, Test, "name of test");
//  SCALAR(test6, Test, "name of test");
//  SCALAR(name3, std::string, "name of test");
public:

    CONSTRUCTORS(Test2)
};
//
//class Test3 SERIALIZETHIS(Test3)
//{
//    SCALAR(age, int, "age of test");
////  SCALAR(name, std::string, "name of test");
//    SCALAR(test1, Test2, "name of test");
////  SCALAR(test2, Test, "name of test");
////  SCALAR(test3, Test, "name of test");
////  SCALAR(test4, Test, "name of test");
////  SCALAR(test5, Test, "name of test");
////  SCALAR(test6, Test, "name of test");
//    SCALAR(name3, std::string, "name of test");
//public:
//
//    CONSTRUCTORS(Test3)
//}


int main()
{
    int version = property::property_version();
    int major = property::property_major();
    int minor = property::property_minor();
//
    std::cout << "version: " << version << "\n";
    std::cout << "major: " <<  major << "\n";
    std::cout << "minor: " <<  minor << "\n";
    Test2 test;
    test.age.set(42);
    test.is_booled.set(true);
    std::string serdata;
    test.serialize([&serdata](const std::string &sd) {
        serdata = sd;
    });

    Test2 ser_data;
    try {
      ser_data.deserialize(serdata);
    }
    catch (property::serialize_error &e) {
      std::cout << e.what();
      return -1;
    }

    ser_data.serialize([](const std::string &sd) {
      std::cout << "\n" << sd << "\n";
    });

}
//#include <yaml-cpp/yaml.h>
//#include <string>
//#include <memory>
//#include <utility>

//#define SERIALIZE() private: prop root;
//
//struct prop {
//    using Root = prop*;
//public:
//    std::string_view name;
//    std::string_view type_name;
//    std::string_view type_doc;
//
//    prop() = default;
//    prop(const std::string_view name,const std::string_view type_name, const std::string_view type_doc, Root root ) :
//    name{name},
//    type_name{type_name},
//    type_doc{type_doc},
//    root{root}
//    {
//
//    }
//private:
//    YAML::Node node;
//    Root root;
//};
//template<typename T>
//constexpr std::string_view deduce_prop_type_name(T &&type) {
//    return "";
//}
//
//#define SCALAR(prop_name, prop_type, prop_doc) public: prop prop_name{#prop_name, deduce_prop_type_name(prop_type{}), prop_doc, &root};
//
//class Test final {
//    SERIALIZE();
//    SCALAR(age, int, "The age of Test class");
//
//public:
//};
//
//template<typename Serialize, typename ResultFunction, typename = std::enable_if_t<false, Serialize>>
//void serialize(Serialize object, ResultFunction result) {
//
//}

//struct prop {
//    std::string name;
//    std::string type_name;
//    int value;
//protected:
//    YAML::Node node;
//    YAML::Node *parent;
//    bool is_serialize;
//public:
//    prop() = default;
//    prop(std::string_view name, std::string_view type_name, YAML::Node *parent, bool is_serialize = false) : name{name}, type_name{type_name}, parent{parent}, is_serialize{is_serialize}
//    {
//        if (is_serialize) {
//            node[this->name] = "version: 1.0";
//            node[this->name].SetTag(type_name.data());
//        }
//        if (parent) {
//            if (is_serialize) {
//                (*parent)[this->name] = "version: 1.0";
//                (*parent)[this->name].SetTag(type_name.data());
//                node[this->name + "_doc"] = type_name.data();
//                node.remove(this->name);
//                (*parent)[this->name] = node;
//            } else {
//                (*parent)[this->name] = value;
//                (*parent)[this->name].SetTag(type_name.data());
//            }
//        }
//    }
//
//    void serialize(std::function<void (const std::string &serdata)> completed)
//    {
//        YAML::Emitter out;
//        if (parent) {
//            out << YAML::BeginMap;
//            out << *parent;
//            out << YAML::EndMap;
//        } else {
//            out << YAML::BeginMap;
////            YAML::Node first;
////            YAML::Node second;
////
////            YAML::Node root;
////
////            root["age"] = 42;
////            root["name"] = "kek";
////            {
////                YAML::Node child;
////                child.SetStyle(YAML::EmitterStyle::value::Block);
////                child["age"] = 42;
////                child["name"] = "kek";
////                root["child_class"] = child;
////            }
//            for (YAML::const_iterator citerator = node.begin(); citerator != node.end(); ++citerator) {
//                out << YAML::Key << citerator->first << YAML::Value << citerator->second;
//            }
//
//            out << YAML::EndMap;
////            out << node;
//        }
//        completed(out.c_str());
//    }
//
//    void deserialize(const std::string &serdata)
//    {
//        YAML::Node nodes = YAML::Load(serdata.c_str());
//
//        for (YAML::const_iterator it = nodes.begin(); it != nodes.end(); ++it) {
//            std::cout << it->first << it->second.Tag() <<  "\n";
//        }
//    }
//
//    int &operator=(const int v)
//    {
//        node[name.data()] = value = v;
//        node[name.data()].SetTag(type_name.data());
//        return value;
//    }
//};
//
//class Test : public prop {
//public:
//prop age{ "age", "int", &node };
//prop age2{ "age2", "int", &node };
//Test() : prop{"test1", "Test", nullptr, true}
//{
//}
//
//Test(std::string_view name, std::string_view type_name, YAML::Node *root) : prop{name, type_name, root, true}
//{
//}
//};
//
//class Test2 : public prop {
//public:
//prop age{ "age", "int", &node };
//Test test{ "test", "Test", &node };
//
//Test2() : prop{"t", "Test2", nullptr, true}
//{
//}
//
//Test2(std::string_view name, std::string_view type_name, YAML::Node *root) : prop{name, type_name, root, true}
//{
//}
//};
//
//class Test3 : public prop {
//public:
//prop age{ "age", "int", &node };
//Test2 test{ "test", "Test2", &node };
//
//Test3() : prop{"t", "Test3", nullptr, true}
//{
//}
//};
//#include <vector>
//#include <map>
//#include <iostream>
//#include <string_view>
//#include <fstream>
//#include <cmath>
//#include <functional>
//#include <type_traits>

//
//template<typename O>
//struct holder<ObjectType::sequence, std::vector<O>> {
//  using type = Serializable<std::vector<O>>;
//  type holder_object;
//  holder(const char* name_, const char* desc_, const char* type_name_, std::vector<SerializeNode*>& childs) : holder_object{name_, desc_, "sequence"} {
//      childs.push_back(&holder_object);
//      holder_object.value = {};
//      holder_object.serialize = [&] () {
//        std::cout << holder_object.name << " " << holder_object.type_name << "\n";
//      };
//  }
//};
//
//template<typename K, typename O>
//struct holder<ObjectType::map, Serializable<std::map<K, O>>> {
//  using type = Serializable<std::map<K,O>>;
//  type holder_object;
//  holder(const char* name_, const char* desc_, std::vector<SerializeNode*> &childs) : holder_object{name_, desc_, "map"} {
//    //childs.push_back(&holder_object.node);
//    }
//};
//
//
//template<typename O>
//using Serialize = holder<ObjectType::serialize, Serializable<O>>;
//template<typename O>
//using Scalar = std::conditional_t< is_base_of_holder<O>, O, holder<ObjectType::scalar, O>>;
//template<typename O>
//using Sequence = holder<ObjectType::sequence, std::vector<O>>;
//template<typename K, typename O>
//using Map = holder<ObjectType::map, Serializable<std::map<K, O>>>;
//
//#define serializable_s(param_name, param_type, param_name_text, param_desc) \
//  private: \
//    Scalar<param_type>  param_name{ # param_name, param_desc, #param_type, this->childs};
//
//#define serializable_v(param_name, param_type, param_name_text, param_desc) \
//  private: \
//    Sequence<param_type>  param_name{ # param_name, param_desc, #param_type, this->childs};
//
//#define serializable_m(param_name, param_type1, param_type2, param_name_text, param_desc) \
//  private: \
//    Map<param_type1, param_type2>  param_name{ # param_name, param_desc, this->childs};
//
//#define SCALAR(param_name, param_type, param_desc) serializable_s(param_name, param_type, param_name, param_desc)
//#define SEQUENCE(param_name, param_type, param_desc) serializable_v(param_name, param_type, param_name, param_desc)
//#define MAP(param_name, param_type1, param_type2, param_desc) serializable_m(param_name, param_type1, param_type2, param_name, param_desc)
//
//#define SERIALIZETHIS(type_name) \
//  final : public Serialize<type_name>
//#define CONSTRUCTORS(class_name) \
//  public: \
//  class_name(const std::string_view &name_, const std::string_view &desc_) : holder{name_.data(), desc_.data(), #class_name} {} \
//  class_name(const std::string_view &name_, const std::string_view &desc_, const std::string_view& type_name_, std::vector<SerializeNode*> &childs_) : holder{name_.data(), desc_.data(), type_name_.data()} { \
//    childs_.push_back(&holder_object); \
//    std::copy(std::begin(childs), std::end(childs),  std::back_inserter(childs_)); \
//  }
//
//class Test SERIALIZETHIS(Test) {
//  SEQUENCE(param7, int, "int sequence value");
//  SEQUENCE(param8, int, "int sequence value");
//  SEQUENCE(param9, int, "int sequence value");
//  SEQUENCE(param, int, "int sequence value");
////  MAP     (param3, int, int, "int map value");
//  public:
//  CONSTRUCTORS(Test)
//};
//class Test2 SERIALIZETHIS(Test2){
////  serializethis(Test2, "The test2 class that you can ser/des")
//  SCALAR  (param1,  float, "float value");
//  SCALAR  (param2,  float, "float value");
//  SCALAR  (param3,  float, "float value");
//  SCALAR  (param4,  float, "float value");
//  SCALAR  (param5,  float, "float value");
//  SCALAR  (param6,  Test, "Test value");
//  CONSTRUCTORS(Test2)
//};
//
//int main() {
//  DEBUG("yaml start serialize\n");
//  Test2 t{"t", "The test2 class"};
//
//  t.serialize();
//
//  return 0;
//}
