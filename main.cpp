//#include <yaml-cpp/yaml.h>

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
  Serializable(const char* name_, const char* desc_, const char* type_name_) : name{name_}, type_name{type_name_}, desc{desc_}, value{} {}
};

template<ObjectType objectType, typename O>
struct holder;

template<typename O>
struct holder<ObjectType::serialize, Serializable<O>> {
  using type = Serializable<O*>;
  type holder_object;
  holder(const char* name_, const char* desc_) : holder_object{name_, desc_, "serialize"} {}
  void print() {
    std::cout << ": !<" << holder_object.type_name << "> " << holder_object.name <<" ("<< holder_object.desc <<")\n";
  }
};


template<typename O>
struct holder<ObjectType::scalar, Serializable<O>> {
  using type = Serializable<O>;
  type holder_object;
  holder(const char* name_, const char* desc_) : holder_object{name_, desc_, "scalar"} {}
  void print() {
    std::cout << holder_object.name << ": !<" << holder_object.type_name << "> " << holder_object.value <<" ("<< holder_object.desc <<")\n";
  }
};

template<typename O>
struct holder<ObjectType::sequence, Serializable<std::vector<O>>> {
  using type = Serializable<std::vector<O>>;
  type holder_object;
  holder(const char* name_, const char* desc_) : holder_object{name_, desc_, "sequence"} {}
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
  holder(const char* name_, const char* desc_) : holder_object{name_, desc_, "map"} {}
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
using Scalar = holder<ObjectType::scalar, Serializable<O>>;
template<typename O>
using Sequence = holder<ObjectType::sequence, Serializable<std::vector<O>>>;
template<typename K, typename O>
using Map = holder<ObjectType::map, Serializable<std::map<K, O>>>;

#define serializable_s(param_name, param_type, param_name_text, param_desc) \
  private: \
    Scalar<param_type>  param_name{ # param_name, param_desc}; 

#define serializable_v(param_name, param_type, param_name_text, param_desc) \
  private: \
    Sequence<param_type>  param_name{ # param_name, param_desc}; 

#define serializable_m(param_name, param_type1, param_type2, param_name_text, param_desc) \
  private: \
    Map<param_type1, param_type2>  param_name{ # param_name, param_desc}; 

#define SCALAR(param_name, param_type, param_desc) serializable_s(param_name, param_type, param_name, param_desc)
#define SEQUENCE(param_name, param_type, param_desc) serializable_v(param_name, param_type, param_name, param_desc)
#define MAP(param_name, param_type1, param_type2, param_desc) serializable_m(param_name, param_type1, param_type2, param_name, param_desc)

#define serializethis(type_name, desc) \
  public: \
    Serialize<type_name> serializer{#type_name, desc};

class Test {
  serializethis(Test, "The test class that you can ser/des")
  SCALAR  (param,  float, "float value");
  SEQUENCE(param2, int, "int sequence value");
  MAP     (param3, int, int, "int map value");

  public:
  Test() = default;
  void print() {
      serializer.print();
      param .print();
      param2.print();
      param3.print();
  }
};
namespace serialize::core {
template<typename T>
void serialize(const std::string_view& filename, T obj) {
  std::cout << filename;
  obj.print();
}
}
int main() {
  Test t;
  serialize::core::serialize("t", t);
  //std::string line = "param: !<float> 3.14000\n";
  //std::cout << line;
  return 0;
}
