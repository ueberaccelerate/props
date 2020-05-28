#include <property/serialize_node.hpp>

#include <yaml-cpp/yaml.h>

#include <ueberlog/ueberlog.hpp>
#include <strstream>
namespace property {
struct SerializeNode::Impl {
    YAML::Node yaml_node;
    SerializeNode *node;
    Impl(SerializeNode *node) : node{node}
    {
    }
};

// it need to unique_ptr forward declaration
SerializeNode::~SerializeNode() = default;

SerializeNode::SerializeNode(const char *name, const char *desc, const char *type_name, const ObjectType object_type) :  SerializeInfo{name, type_name, desc, object_type, ""}, impl{std::make_unique<SerializeNode::Impl>(this)}
{
    DEBUG("created node: %s %s %s \n", name, desc, type_name);
}

std::string SerializeNode::commit(const SerializeQueue &out)
{
    SerializeQueue buffer = out;
    YAML::Emitter emitter;
    YAML::Node root;
    while (!buffer.empty()) {
        auto &element = buffer.front();
        buffer.pop();
        if (element.object_type == ObjectType::serialize) {
            std::stringstream stream;
            stream << element.type_name << ":" << element.desc;
            if(root.IsNull()) {
                root[element.name.data()] = stream.str();
            } else {
                YAML::Node node;
                node[element.name.data()] = stream.str();
                root["childs"].push_back(node);
            }
        }
        if (element.object_type == ObjectType::scalar) {
            YAML::Node node;
            std::stringstream stream;
            stream << element.data << ":" << element.type_name << ":" << element.desc;
            node[element.name.data()] = stream.str();
            root["childs"].push_back(node);
        }
    }
    emitter << root;
    return emitter.c_str();
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
