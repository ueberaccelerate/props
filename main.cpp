#include <yaml-cpp/yaml.h>

#include <iostream>
#include <string_view>
#include <fstream>
#include <cmath>

/*
 *  case Null: // ...
 *  case Scalar: // ...
 *  case Sequence: // ...
 *  case Map: // ...
 *  case Undefined: // ...
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

template<typename T>
struct SerializableClass : Serializable<T> {

};

#define serializable_v(param_name, param_type, param_name_text, param_desc) \
  private: \
    Serializable<param_type>  param_name{ # param_name, param_desc}; 

#define serializable_c(param_name, param_type, param_name_text, param_desc) \
  private: \
    SerializableClass<param_type>  param_name{ # param_name, param_desc}; 

#define serializable(param_name, param_type, param_desc) serializable_v(param_name, param_type, param_name, param_desc)

#define serialize_this(type_name)  \
  serializable_c(this_serialize, type_name, # type_name); \
  public: \
  friend YAML::Emitter& operator<<(YAML::Emitter& emitter, const type_name &);

class SerializeSystem {
  public:
    template<typename T>
    void serialize(const T &obj);
    template<typename T>
    void deserialize(T &obj);

    void load(const std::string &file);
    void save(const std::string &file);

  private:
    YAML::Emitter emitter_;
};


class Test {
serialize_this(Test);
serializable(param, int, "int value");
serializable(param2, std::vector<int>, "vector int value");
  public:
  void print() {
    std::cout << param.name  << '\n';
    std::cout << param.type_name  << '\n';
    std::cout << param.desc  << '\n';
    std::cout << param.value << '\n';
    std::cout << param2.name  << '\n';
    std::cout << param2.type_name  << '\n';
    std::cout << param2.desc  << '\n';
    std::cout << param2.value.size() << '\n';
  }
};

//#define serialize_this(type_name)  \
//  YAML::Node root_; \
//  public: \
//  friend YAML::Emitter& operator<<(YAML::Emitter& emitter, const type_name &);
//
//
//#define serializable(param_name, param_type) \
//  private: \
//  Serializable<param_type>  param_name{root_}; \
//  public: \
//  void set##param_name(const param_type &value) { \
//    param_name = value; \
//  }

//class Test2 {
//  serialize_this(Test2);
//  serializable(param1, int);
//  serializable(param2, float);
//  serializable(param3, std::string);
//  public:
//  Test2() = default;
//  YAML::Node &operator()() {
//    return root_;
//  }
//};

//class Test1 {
//  serialize_this(Test1);
//  serializable(param1, int);
//  serializable(param2, float);
//  serializable(param3, std::string);
////  serializable(param4, Test2);
//  public:
//  Test1() = default;
//};
//
//YAML::Emitter& operator<<(YAML::Emitter& emitter, const Test1 &test) {
//  std::cout << "operator <<\n";
//  emitter << test.root_;
//  return emitter;
//}

//YAML::Emitter& operator<<(YAML::Emitter& emitter, const Test2 &test) {
//  std::cout << "operator <<\n";
//  emitter << test.root_;
//  return emitter;
//}
/*
 *  case Null: // ...
 *  case Scalar: // ...
 *  case Sequence: // ...
 *  case Map: // ...
 *  case Undefined: // ...
 *
 * */

//template<typename T>
//class holder {
//  using type = T;
// // using reference = type&;
// // using pointer = type*;
// // using const_reference = const reference;
// // using const_pointer = const pointer;
//  std::string_view prop_name_;
//  type prop_value_;
//  public:
//  holder(const std::string_view &name, type t) : prop_name_{name}, prop_value_{t} {
//  }
//  void print() {
//    std::cout << "{\n" << "  " << prop_name_ << ": !<" << typeid(prop_value_).name() <<"> " << prop_value_ << "\n}";
//  }
//};
//
//template<typename ...Args>
//auto make_holder(const std::string &name, Args ...args) {
//  return holder<Args...>{name, std::forward<Args...>(args)...};
//}
//class empty {
//
//  public:
//    empty() {std::cout << "empty\n";}
//    ~empty() {std::cout << "!empty\n";}
//};
int main()
{
  Test t;
  t.print();
//  empty e;
//  holder h = make_holder("int_prop", 42);
//  h.print();
//  Test2 t2;
//  t2.setparam1(42);
//  t2.setparam2(2.f);
//  t2.setparam3("hell");

//  Test1 t1;
//  t1.setparam1(42);
//  t1.setparam2(2.f);
//  t1.setparam3("hell");
//  t1.setparam4(t2);
//  YAML::Emitter out;
//  YAML::Node node;  // starts out as null
//  node["key"] = "<int!> " + std::to_string(42);  // it now is a map node
//  node["seq"].push_back("first element");  // node["seq"] automatically becomes a sequence
//  node["seq"].push_back("second element");
//
//  node["mirror"] = node["seq"][0];  // this creates an alias
//  node["seq"][0] = "1st element";  // this also changes node["mirror"]
//  node["mirror"] = "element #1";  // and this changes node["seq"][0] - they're really the "same" node
//
//  node["self"] = node;  // you can even create self-aliases
//  node[node["mirror"]] = node["seq"];  // and strange loops
//  out << YAML::BeginMap;
//  out << t1;
//  out << YAML::EndMap;
//  std::cout << out.c_str() << std::endl;
//  std::ofstream file;
//
//  file.open("test.yml");
//  file << out.c_str();
//  file.close();

//  el::Loggers::addFlag(el::LoggingFlag::ColoredTerminalOutput);
//
//  TestClass_2 ser_test;
//
//  ser_test.set_random();
//
//  YAML::Emitter out;
//  out << YAML::BeginMap;
//  std::string name = "serialize";
//
//  el::Logger* businessLogger = el::Loggers::getLogger(name);
//
//  el::Configurations defaultConf;
//  defaultConf.setToDefault();
//  defaultConf.set(el::Level::Error, el::ConfigurationType::Format, "%datetime %level %msg");
//
//  el::Loggers::reconfigureLogger(name, defaultConf);
//
//  std::string name2 = name;
//
//  auto c1 = new TestClass_1;
//  auto c2 = new TestClass_1;
//  auto c3 = new TestClass_1;
//
//  c1->set_random();
//  c2->set_random();
//  c3->set_random();
//
//  ser_test.add_subprop(c1, "c1");
//  ser_test.add_subprop(c2, "c2");
//  ser_test.add_subprop(c3, "c3");
//
//  ser_test.set_logger(name);
//  ser_test.serialize(out, true);
//
//  out << YAML::EndMap;
//  std::cout << "yaml\n" << out.c_str() << std::endl;
//  std::ofstream file;
//
//  file.open("test.yml");
//  file << out.c_str() << std::endl;
//  file.close();
//
//  YAML::Node saved_yml = YAML::LoadFile("test.yml");
//
//  TestClass_2 deser_test;
//
//  deser_test.reset();
//
//  c1->set_random();
//  c2->set_random();
//  c3->set_random();
//
//  deser_test.add_subprop(c1, "c1");
//  deser_test.add_subprop(c2, "c2");
//  deser_test.add_subprop(c3, "c3");
//
//  std::cout << "==================================" << std::endl;
//  std::cout << saved_yml << std::endl;
//
//  deser_test.set_logger(name2);
//  proplib::res_t res = deser_test.deserialize(saved_yml);
//
//  bool test_passed = ser_test == deser_test;
//
//  std::cout << "Test passed: " << test_passed << std::endl;
//
//  int d;
//  d = 0;
}
