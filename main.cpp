#include <yaml-cpp/yaml.h>

#include <vector>
#include <map>
#include <iostream>
#include <string_view>
#include <fstream>
#include <cmath>
#include <functional>
#include <type_traits>

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

struct SerializeNode {
  std::string_view name;
  std::string_view type_name;
  std::string_view desc;
  YAML::Node node;
  std::function<void ()> serialize;
  std::function<void ()> deserialize;
  SerializeNode(const char* name_, const char* desc_, const char* type_name_) : name{name_}, type_name{type_name_}, desc{desc_} {}
};

template<typename T>
struct Serializable : SerializeNode {
  T value;
  Serializable(const char* name_, const char* desc_, const char* type_name_) : SerializeNode{name_, desc_, type_name_} { 
  }

  void deserialize(const YAML::Node &n, T& value) {
    std::cout << n[name.data()];
  }

};


template<ObjectType objectType, typename O>
struct holder;

template<typename O>
struct holder<ObjectType::serialize, Serializable<O>> {
  using type = Serializable<O*>;
  type holder_object;
  std::vector<SerializeNode*> childs;
  YAML::Node root_class;
  public:
  holder(const char* name_, const char* desc_, const char* type_name_) : holder_object{name_, desc_, type_name_}, childs{} {
    root_class[name_] = type_name_;
    root_class[(std::string(name_) + "_desc").data()] = desc_;
    holder_object.serialize = [&] () {
      std::cout << holder_object.name << " " << holder_object.type_name << "\n";
    };
  }

  virtual ~holder() {}
  void serialize() {
     holder_object.serialize();
     for( auto child : childs ) {
       child->serialize();
     }
  }
};

template<typename T>
constexpr bool is_base_of_holder = std::is_base_of_v<holder<ObjectType::serialize, Serializable<T>>, T>;

template<typename T>
struct base_holder{
  using basename = T;
};

template<typename O>
struct holder<ObjectType::scalar, O> : base_holder<O> {
  using type = Serializable<O>;
  type holder_object;

  holder(const char* name_, const char* desc_, const char* type_name_) : holder_object{name_, desc_, "scalar"}{ 
      holder_object.value = 42;
      holder_object.node[holder_object.name.data()] = holder_object.value;
      holder_object.node[(std::string(holder_object.name.data()) + std::string("_desc")).data()] = holder_object.desc.data();
      holder_object.serialize = [&] () {
        std::cout << holder_object.name << " " << holder_object.type_name << '\n';
      };
  }

  holder(const char* name_, const char* desc_, const char* type_name_, std::vector<SerializeNode*>& childs) : holder_object{name_, desc_, "scalar"}{ 
      holder_object.serialize = [&] () {
        std::cout << holder_object.name << " " << holder_object.type_name << '\n';
      };
      childs.push_back(&holder_object);
      holder_object.value = 42;
      holder_object.node[holder_object.name.data()] = holder_object.value;
      holder_object.node[(std::string(holder_object.name.data()) + std::string("_desc")).data()] = holder_object.desc.data();
  }
  void set(const O &value) {
    holder_object.value = value;
    holder_object.node[holder_object.name.data()] = value;
  }
};

template<typename O>
struct holder<ObjectType::sequence, std::vector<O>> {
  using type = Serializable<std::vector<O>>;
  type holder_object;
  holder(const char* name_, const char* desc_, const char* type_name_, std::vector<SerializeNode*>& childs) : holder_object{name_, desc_, "sequence"} { 
      childs.push_back(&holder_object);
      holder_object.value = {};
      holder_object.node[holder_object.name.data()] = holder_object.value;
      holder_object.serialize = [&] () {
        std::cout << holder_object.name << " " << holder_object.type_name << "\n";
      };
  }
};

template<typename K, typename O>
struct holder<ObjectType::map, Serializable<std::map<K, O>>> {
  using type = Serializable<std::map<K,O>>;
  type holder_object;
  holder(const char* name_, const char* desc_, std::vector<SerializeNode*> &childs) : holder_object{name_, desc_, "map"} { 
    //childs.push_back(&holder_object.node);
    }
};


template<typename O>
using Serialize = holder<ObjectType::serialize, Serializable<O>>;
template<typename O>
using Scalar = std::conditional_t< is_base_of_holder<O>, O, holder<ObjectType::scalar, O>>;
template<typename O>
using Sequence = holder<ObjectType::sequence, std::vector<O>>;
template<typename K, typename O>
using Map = holder<ObjectType::map, Serializable<std::map<K, O>>>;

#define serializable_s(param_name, param_type, param_name_text, param_desc) \
  private: \
    Scalar<param_type>  param_name{ # param_name, param_desc, #param_type, this->childs}; 

#define serializable_v(param_name, param_type, param_name_text, param_desc) \
  private: \
    Sequence<param_type>  param_name{ # param_name, param_desc, #param_type, this->childs}; 

#define serializable_m(param_name, param_type1, param_type2, param_name_text, param_desc) \
  private: \
    Map<param_type1, param_type2>  param_name{ # param_name, param_desc, this->childs}; 

#define SCALAR(param_name, param_type, param_desc) serializable_s(param_name, param_type, param_name, param_desc)
#define SEQUENCE(param_name, param_type, param_desc) serializable_v(param_name, param_type, param_name, param_desc)
#define MAP(param_name, param_type1, param_type2, param_desc) serializable_m(param_name, param_type1, param_type2, param_name, param_desc)

#define serializethis(type_name) \
  public Serialize<type_name> 
#define CONSTRUCTORS(class_name) \
  public: \
  class_name(const std::string_view &name_, const std::string_view &desc_) : holder{name_.data(), desc_.data(), #class_name} {} \
  class_name(const std::string_view &name_, const std::string_view &desc_, const std::string_view& type_name_, std::vector<SerializeNode*> &childs_) : holder{name_.data(), desc_.data(), type_name_.data()} { \
    childs_.push_back(&holder_object); \
    for(auto child : childs) { \
      childs_.push_back(child); \
    } \
  }

class Test final : serializethis(Test) {
  SEQUENCE(param7, int, "int sequence value");
  SEQUENCE(param8, int, "int sequence value");
  SEQUENCE(param9, int, "int sequence value");
  SEQUENCE(param, int, "int sequence value");
//  MAP     (param3, int, int, "int map value");
  public:
  CONSTRUCTORS(Test)
};
class Test2 final : serializethis(Test2){
//  serializethis(Test2, "The test2 class that you can ser/des")
  SCALAR  (param1,  float, "float value");
  SCALAR  (param2,  float, "float value");
  SCALAR  (param3,  float, "float value");
  SCALAR  (param4,  float, "float value");
  SCALAR  (param5,  float, "float value");
  SCALAR  (param6,  Test, "Test value");
  CONSTRUCTORS(Test2)
};

int main() {
  Test2 t{"t", "The test2 class"};
  t.serialize();

  return 0;
}
