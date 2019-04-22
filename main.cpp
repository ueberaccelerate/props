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
  Serializable(const char* name_, const char* desc_, T value_) : name{name_}, type_name{typeid(value).name()}, desc{desc_}, value{value_} {}
  Serializable(const char* name_, const char* desc_) : name{name_}, type_name{typeid(value).name()}, desc{desc_} {}
};

template<ObjectType objectType, typename O>
struct holder;

template<typename O>
struct holder<ObjectType::scalar, Serializable<O>> {
  using type = Serializable<O>;
  type holder_object;
  holder(const char* name_, const char* desc_) : holder_object{name_, desc_} {}
  void print() {
    std::cout << "----scalar----\n";
    std::cout << holder_object.name << ": " << holder_object.type_name << " ("<< holder_object.desc <<")\n";
    std::cout << "----scalar----\n";
  }
};

template<typename O>
struct holder<ObjectType::sequence, Serializable<std::vector<O>>> {
  using type = Serializable<std::vector<O>>;
  type holder_object;
  holder(const char* name_, const char* desc_) : holder_object{name_, desc_} {}
  void print() {
    std::cout << "----vector----\n";
    std::cout << holder_object.name << ": " << holder_object.type_name << " ("<< holder_object.desc <<")\n";
    std::cout << "----vector----\n";
  }
};

template<typename K, typename O>
struct holder<ObjectType::map, Serializable<std::map<K, O>>> {
  using type = Serializable<std::map<K,O>>;
  type holder_object;
  holder(const char* name_, const char* desc_) : holder_object{name_, desc_} {}
  void print() {
    std::cout << "----map----\n";
    std::cout << holder_object.name << ": " << holder_object.type_name << " ("<< holder_object.desc <<")\n";
    std::cout << "----map----\n";
  }
};


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

#define serializethis(type_name)  

class Test {
  SCALAR  (param,  int, "int value");
  SEQUENCE(param2, int, "int sequence value");
  MAP     (param3, int, int, "int map value");
  public:
  void print() {
      param .print();
      param2.print();
      param3.print();
  }
};

int main() {
  Test t;
  t.print();
  return 0;
}
