#include <yaml-cpp/yaml.h>

#include <iostream>
#include <fstream>
#include <cmath>

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

template<typename T>
class Serializable {
  using type      = T;
  using reference = T&;
  using pointer   = T*;

  YAML::Node &serializer_;
  type value_;

  void init_() {
    std::cout << "Serializable::init_\n";
  }
  public:
  Serializable(YAML::Node &root) : serializer_{root} {
    init_();
  }

  Serializable<type> &operator=(const type value) {
    value_ = value;
    serializer_[typeid(value_).name()] = value_;
    return *this;
  }
  Serializable<type> &operator=(const reference value) {
    value_ = value;
    serializer_[typeid(value_).name()] = value_;
    return *this;
  }
};

template<>
class Serializable<std::string> {

  using type      = std::string;
  using reference = type&;
  using pointer   = type*;

  YAML::Node &serializer_;
  type value_;

  void init_() {
    std::cout << "Serializable::init_\n";
    std::cout << "string" << "\n";
  }
  public:
  Serializable(YAML::Node &root) : serializer_{root} {
    init_();
  }
  Serializable<type> &operator=(const type value) {
    value_ = value;
    serializer_["string"] = value_;
    return *this;
  }

  Serializable<type> &operator=(const reference value) {
    value_ = value;
    serializer_["string"] = value_;
    return *this;
  }
};


#define serialize_this() YAML::Node root_

#define serializable(param_name, param_type) \
  private: \
  Serializable<param_type>  param_name{root_}; \
  public: \
  void set##param_name(const param_type &value) { \
    param_name = value; \
  }

class Test1 {
  YAML::Node root_;
  serializable(param1, int);
  serializable(param2, float);
  serializable(param3, std::string);
  public:
  Test1() = default;
  friend YAML::Emitter& operator<<(YAML::Emitter& emitter, const Test1 &test);
};

YAML::Emitter& operator<<(YAML::Emitter& emitter, const Test1 &test) {
  std::cout << "operator <<\n";
  emitter << test.root_;
  return emitter;
}

int main()
{
  Test1 t;
  t.setparam1(42);
  t.setparam2(2.f);
  t.setparam3("hell");

  YAML::Emitter out;
  YAML::Node node;  // starts out as null
  node["key"] = "<int!> " + std::to_string(42);  // it now is a map node
  node["seq"].push_back("first element");  // node["seq"] automatically becomes a sequence
  node["seq"].push_back("second element");

  node["mirror"] = node["seq"][0];  // this creates an alias
  node["seq"][0] = "1st element";  // this also changes node["mirror"]
  node["mirror"] = "element #1";  // and this changes node["seq"][0] - they're really the "same" node

  node["self"] = node;  // you can even create self-aliases
  node[node["mirror"]] = node["seq"];  // and strange loops
  out << YAML::BeginMap;
  out << node;
  out << YAML::EndMap;
  std::cout << out.c_str() << std::endl;
  std::ofstream file;

  file.open("test.yml");
  file << out.c_str();
  file.close();

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
