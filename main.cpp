/*
 *            DO WHAT THE FUCK YOU WANT TO PUBLIC LICENSE
 *                    Version 2, December 2004
 *
 * Copyright (C) 2019 Suboch, Vadim <vssuboch@gmail.com>
 *
 * Everyone is permitted to copy and distribute verbatim or modified
 * copies of this license document, and changing it is allowed as long
 * as the name is changed.
 *
 *            DO WHAT THE FUCK YOU WANT TO PUBLIC LICENSE
 *   TERMS AND CONDITIONS FOR COPYING, DISTRIBUTION AND MODIFICATION
 *
 *  0. You just DO WHAT THE FUCK YOU WANT TO.
 */

#include <yaml-cpp/yaml.h>
//
#include <vector>
#include <map>


#include <iostream>
#include <string_view>
#include <fstream>
#include <cmath>

/*
 *  case Scalar: // ...
 *  case Sequence: // ...
 *  case Map: // ...
 *
 * */
enum class ObjectType {
  serialize,
  scalar,
  sequence,
  map
};

template<typename T>
struct Serializable {
  std::string_view name;
  std::string_view type_name;
  std::string_view desc;
  T value;
  YAML::Node node;
  Serializable(const char* name_, const char* desc_, const char* type_name_) : name{name_}, type_name{type_name_}, desc{desc_}, value{} {}
};

template<ObjectType objectType, typename O>
struct holder;

template<typename O>
struct holder<ObjectType::serialize, Serializable<O>> {
  using type = Serializable<O*>;
  type holder_object;
  std::vector<YAML::Node*> childs;
  YAML::Node root_class;
  holder(const char* name_, const char* desc_) : holder_object{name_, desc_, "serialize"}, childs{} {
    root_class[name_] = "!<serialize>";
    root_class[(std::string(name_) + "_desc").data()] = desc_;
  }

  void serialize() {
     YAML::Emitter out;
     out << YAML::BeginMap;
     out << root_class;
     for( auto *child : childs ) {
        out << *child;
     }
     out << YAML::EndMap;
     std::cout << "yaml\n" << out.c_str() << std::endl;
  }

  void print() {
    //std::cout << ": !<" << holder_object.type_name << "> " << holder_object.name <<" ("<< holder_object.desc <<")\n";
  }
};

template<typename O>
struct holder<ObjectType::scalar, O> {
  using type = Serializable<O>;
  type holder_object;
  holder(const char* name_, const char* desc_, std::vector<YAML::Node*>& childs) : holder_object{name_, desc_, "scalar"}{ 
    std::cout << std::is_same_v<O, holder<ObjectType::serialize, Serializable<O>>> << '\n';
    if constexpr (std::is_fundamental_v<O>) {
      childs.push_back(&holder_object.node);
      holder_object.value = 42;
      holder_object.node[holder_object.name.data()] = holder_object.value;
      holder_object.node[(std::string(holder_object.name.data()) + std::string("_desc")).data()] = holder_object.desc.data();
    } else {
    //  holder_object.value.serializer.holder_object.name = name_;
    //  holder_object.value.serializer.holder_object.desc = desc_;
    //  childs.push_back(&holder_object.value.serializer.root_class);
     // for(auto *child : childs) { 
     //   childs.push_back(child);
     // }
    }
  }
  void print() {
    //std::cout << holder_object.name << ": !<" << holder_object.type_name << "> " << holder_object.value <<" ("<< holder_object.desc <<")\n";
  }
  void set(const O &value) {
    holder_object.value = value;
    holder_object.node[holder_object.name.data()] = value;
    holder_object.node[(std::string(holder_object.name.data()) + std::string("_desc")).data()] = holder_object.desc.data();
    //holder_object.node[holder_object.name] = "";
  }
};

template<typename O>
struct holder<ObjectType::sequence, Serializable<std::vector<O>>> {
  using type = Serializable<std::vector<O>>;
  type holder_object;
  holder(const char* name_, const char* desc_, std::vector<YAML::Node*> &childs) : holder_object{name_, desc_, "sequence"} { childs.push_back(&holder_object.node);}
  void print() {
    auto out_vec = [&]() {
      std::cout << "[";
      for (auto &v : holder_object.value) {
        std::cout << v;
      }
      std::cout << "]";
      return "";
    };
    std::cout << holder_object.name << ": !<" << holder_object.type_name << "> " << out_vec() <<" ("<< holder_object.desc <<")\n";
  }
};

template<typename K, typename O>
struct holder<ObjectType::map, Serializable<std::map<K, O>>> {
  using type = Serializable<std::map<K,O>>;
  type holder_object;
  holder(const char* name_, const char* desc_, std::vector<YAML::Node*> &childs) : holder_object{name_, desc_, "map"} { childs.push_back(&holder_object.node);}
  void print() {
    auto out_map = [&]() {
      std::cout << "[";
      for (auto& [key,val] : holder_object.value) {
        std::cout << key << ": " << val <<", ";
      }
      std::cout << "]";
      return "";
    };
    std::cout << holder_object.name << ": !<" << holder_object.type_name << "> " << out_map() <<" ("<< holder_object.desc <<")\n";
  }
};


template<typename O>
using Serialize = holder<ObjectType::serialize, Serializable<O>>;
template<typename O>
using Scalar = holder<ObjectType::scalar, O>;
template<typename O>
using Sequence = holder<ObjectType::sequence, Serializable<std::vector<O>>>;
template<typename K, typename O>
using Map = holder<ObjectType::map, Serializable<std::map<K, O>>>;

#define serializable_s(param_name, param_type, param_name_text, param_desc) \
  private: \
    Scalar<param_type>  param_name{ # param_name, param_desc, this->serializer.childs}; 

#define serializable_v(param_name, param_type, param_name_text, param_desc) \
  private: \
    Sequence<param_type>  param_name{ # param_name, param_desc, this->serializer.childs}; 

#define serializable_m(param_name, param_type1, param_type2, param_name_text, param_desc) \
  private: \
    Map<param_type1, param_type2>  param_name{ # param_name, param_desc, this->serializer.childs}; 

#define SCALAR(param_name, param_type, param_desc) serializable_s(param_name, param_type, param_name, param_desc)
#define SEQUENCE(param_name, param_type, param_desc) serializable_v(param_name, param_type, param_name, param_desc)
#define MAP(param_name, param_type1, param_type2, param_desc) serializable_m(param_name, param_type1, param_type2, param_name, param_desc)

#define serializethis(type_name, desc) \
  public: \
    using type = std::true_type; \
    Serialize<type_name> serializer{#type_name, desc};

class Test {
  serializethis(Test, "The test class that you can ser/des")
//  SCALAR  (param,  float, "float value");
//  SEQUENCE(param2, int, "int sequence value");
//  MAP     (param3, int, int, "int map value");
  public:
  Test() = default;
  void print() {

  }
};
class Test2 {
  serializethis(Test2, "The test2 class that you can ser/des")
  SCALAR  (param,  float, "float value");
//  SEQUENCE(param2, int, "int sequence value");
//  MAP     (param3, int, int, "int map value");
  SCALAR  (param4,  Test, "Test value");

  public:
  void print() {
    serializer.serialize();
  }
};

int main() {
  Test2 t;
  t.print();
//  serialize::core::serialize("t", t);
  //std::string line = "param: !<float> 3.14000\n";
  //std::cout << line;
//  YAML::Emitter out;
//  out << YAML::BeginMap;
//  
//  out << YAML::EndMap;
//  std::cout << "yaml\n" << out.c_str() << std::endl;
//  std::ofstream file;
//  file.open("test.yml");
//  file << out.c_str() << std::endl;
//  file.close();
//
//  YAML::Node saved_yml = YAML::LoadFile("test.yml");
//
//  std::cout << std::string(32,'=') << std::endl;
//  std::cout << saved_yml << std::endl;
  return 0;
}
