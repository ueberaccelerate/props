#include <property/serialize_node.hpp>

#include <yaml-cpp/yaml.h>

#include <ueberlog/ueberlog.hpp>
#include <strstream>
namespace property {


SerializeNode::SerializeNode() = default;
SerializeNode::~SerializeNode() = default;

SerializeNode::SerializeNode(const char *name, const char *desc, const char *type_name, const ObjectType object_type, SerializeNode *parent) : name{name}, type_name{type_name}, desc{desc}, object_type{object_type}, parent{parent}
{
    DEBUG("created node: %s %s %s \n", name, desc, type_name);
}

//void SerializeNode::serialization(VoidFunction serializator)
//{
//  /*
//   std::string_view name;
//   std::string_view type_name;
//   std::string_view desc;
//   */
//    YAML::Emitter out;
//    YAML::Node root;
//    std::strstream root_stream;
//    root_stream <<  type_name.data() << " " << desc.data();
//    root[name.data()] = root_stream.str();
//
//    {
//      std::strstream prop_stream;
//      YAML::Node prop;
//      prop_stream << "int The age of test";
//      prop["age"] = prop_stream.str();
//      root["childs"].push_back(prop);
//    }
//
////    out << YAML::BeginDoc;
//    out << root;
////    out << YAML::EndDoc;
////    root << YAML::BeginMap;
////    root << YAML::Key << "name";
////    root << YAML::Value;
////
////    root << YAML::Key << "type_name";
////    root << YAML::Value << type_name.data();
////    root << YAML::Key << "desc";
////    root << YAML::Value << desc.data();
////
////    root << YAML::EndMap;
//
//
//
//    DEBUG("output:\n%s\n", out.c_str());
//}
}
